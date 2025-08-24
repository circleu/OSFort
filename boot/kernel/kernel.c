#include "kernel.h"

#define KERNEL_START 0x100000000


extern void _start(void* BootInfo) {
    GRAPHICS graphics = {
        ((BOOTINFO*)BootInfo)->gop.framebuffer_base,
        ((BOOTINFO*)BootInfo)->gop.framebuffer_size,
        ((BOOTINFO*)BootInfo)->gop.width,
        ((BOOTINFO*)BootInfo)->gop.height,
        ((BOOTINFO*)BootInfo)->gop.pixels_per_scanline,
        0,
        0
    };
    MEMORY_MAP efi_mmap = ((BOOTINFO*)BootInfo)->mem_map;
    ACPI_XSDT* xsdt = (ACPI_XSDT*)(((BOOTINFO*)BootInfo)->xsdt_addr);
    ACPI_MADT* madt;
    ACPI_MCFG* mcfg;
    uint32_t* apic_base;
    PAGE_TABLE* pml4;

/*******************************************************************/

    kputs("Loading GDT...", &graphics);
    {
        load_gdt(&gdtr);
    }
    kputs("Done\r\n", &graphics);
    
    kputs("Setting Paging...", &graphics);
    {
        size_t mmap_entries = efi_mmap.size / efi_mmap.desc_size;
        void* largest_desc = NULL;
        size_t largest_desc_size = 0;
        size_t total_mem_size = 0;

        for (size_t i = 0; i < mmap_entries; i++) {
            MEMORY_DESCRIPTOR* desc = (MEMORY_DESCRIPTOR*)((uint8_t*)efi_mmap.map + efi_mmap.desc_size * i);
            total_mem_size += desc->num_pages * 0x1000;

            /* EfiConventionalMemory */
            if (desc->type == 7) {
                if (desc->num_pages * 0x1000 > largest_desc_size) {
                    largest_desc = (void*)desc->phys_addr;
                    largest_desc_size = desc->num_pages * 0x1000;
                }
            }
        }

        page_bitmap.free = total_mem_size;
        page_bitmap.bitmap.size = total_mem_size / 0x1000 / 8 + 1;
        page_bitmap.bitmap.buffer = (uint8_t*)largest_desc;
        memset(page_bitmap.bitmap.buffer, 0, page_bitmap.bitmap.size);
        lock_pages(&page_bitmap, (void*)page_bitmap.bitmap.buffer, page_bitmap.bitmap.size / 0x1000 + 1);

        for (size_t i = 0; i < mmap_entries; i++) {
            MEMORY_DESCRIPTOR* desc = (MEMORY_DESCRIPTOR*)((uint8_t*)efi_mmap.map + efi_mmap.desc_size * i);

            if (desc->type != 7) {
                reserve_pages(&page_bitmap, (void*)desc->phys_addr, desc->num_pages);
            }
        }

        size_t kernel_size = (size_t)&_kernel_end - (size_t)&_kernel_start;
        size_t kernel_pages = kernel_size / 0x1000 + 1;
        reserve_pages(&page_bitmap, &_kernel_start, kernel_pages);

        pml4 = (PAGE_TABLE*)req_page(&page_bitmap);
        memset(pml4, 0, 0x1000);

        for (size_t i = 0; i < total_mem_size; i += 0x1000) {
            map_page(&page_bitmap, pml4, (void*)i, (void*)i);
        }

        lock_pages(&page_bitmap, (void*)graphics.framebuffer_base, graphics.framebuffer_size / 0x1000 + 1);
        for (size_t i = 0; i < (size_t)graphics.framebuffer_size; i += 0x1000) {
            map_page(&page_bitmap, pml4, (void*)(graphics.framebuffer_base + i), (void*)(graphics.framebuffer_base + i));
        }

        for (size_t i = 0; i < kernel_size; i += 0x1000) {
            map_page(&page_bitmap, pml4, (void*)((size_t)_kernel_start + i), (void*)((size_t)_kernel_start + i));
        }

        madt = acpi_get_table(xsdt, "APIC");
        apic_base = (uint32_t*)((uint64_t)madt->local_apic_addr);
        map_page(&page_bitmap, pml4, (void*)apic_base, (void*)apic_base);

        mcfg = acpi_get_table(xsdt, "MCFG");
        size_t mcfg_pages = mcfg->header.len / 0x1000 + 1;
        for (size_t i = 0; i < mcfg_pages; i++) {
            map_page(&page_bitmap, pml4, (void*)((uint64_t)mcfg + i), (void*)((uint64_t)mcfg + i));
        }
        
        __asm__ volatile ("movq %0, %%cr3"::"r"(pml4));
    }
    kputs("Done\r\n", &graphics);

    kputs("Setting IDT...", &graphics);
    {
        set_idt();

        __asm__ volatile ("cli");

        outb(ICW1_INIT | ICW1_ICW4, PIC1_COM);
        io_wait();
        outb(ICW1_INIT | ICW1_ICW4, PIC2_COM);
        io_wait();
        outb(0x20, PIC1_DATA); // Timer
        io_wait();
        outb(0x28, PIC2_DATA); // Keyboard
        io_wait();
        outb(0x04, PIC1_DATA);
        io_wait();
        outb(0x02, PIC2_DATA);
        io_wait();
        outb(ICW4_8086, PIC1_DATA);
        io_wait();
        outb(ICW4_8086, PIC2_DATA);
        io_wait();

/* Mask both PICs to use APIC */
        outb(0x00, PIC1_DATA);
        io_wait();
        outb(0x00, PIC2_DATA);
        io_wait();

        __asm__ volatile ("sti");
    }
    kputs("Done\r\n", &graphics);

    kputs("Setting APIC Timer...", &graphics);
    {
        uint32_t* apic_base = (uint32_t*)((uint64_t)madt->local_apic_addr);
    
/* map spurious interrupt */
        *(apic_base + APIC_REGISTER_SPURIOUS) = 0x27;

/* map timer interrupt */
        *(apic_base + APIC_REGISTER_LVT_TIMER) = 0x20;
        *(apic_base + APIC_REGISTER_LVT_TIMER) = 0x21;

/* setup divide value to 16 */
        *(apic_base + APIC_REGISTER_TIMER_DIV) = 0x03;

/**************************************************************
 * from osdev wiki > APIC timer
 * init PIC ch2 in one-shot mode
 *************************************************************/
        outb(((inb(0x61) & 0xfd) | 1), 0x61);
        outb(0xb2, 0x43);

/* 1193180/100 Hz = 11931 = 0x2e9b */
        outb(0x9b, 0x42);
        io_wait();
        outb(0x2e, 0x42);

/* reset PIT one-shot? counter (start counting) */
        {
            uint8_t tmp = (inb(0x61) & 0xfe);
            outb(tmp, 0x61);
            outb(tmp | 1, 0x61);
        }

/* reset APIC timer (set counter to -1) */
        *(apic_base + APIC_REGISTER_TIMER_ICR) = (uint32_t)-1;

/* wait untile PIT counter reaches zero */
        while (inb(0x61) & 0x20 == 0);

/* stop APIC timer */
        *(apic_base + APIC_REGISTER_LVT_TIMER) = 0x10000;

/**************************************************************
 *  get current counter value
 * 
 *  make tmp0 to positive (cos it counts from -1)
 *  -> the value is divided by 16 so multiply by 16
 *  -> multiply 100 (Hz)
 *  -> divide by n: task will be done n times in a second -> HZ = 100
 *  -> divide by 16? - Maybe the value multiplied by 16 so divide it back?
 *
 *  => tmp0 -> tmp1
 *************************************************************/

        {
            uint32_t tmp0 = *(apic_base + APIC_REGISTER_TIMER_ICR);
            uint32_t tmp1 = (((uint64_t)((0xffffffff - tmp0 + 1) << 4) * 100) / 100) >> 4;

/* sanity check*/
            if (tmp1 >= 0x10) {
                *(apic_base + APIC_REGISTER_TIMER_ICR) = tmp1;

/* re-enable timer in periodic mode */
                *(apic_base + APIC_REGISTER_LVT_TIMER) = 0x20 | 0x20000;
            }
        }
    }
    kputs("Done\r\n", &graphics);

    kputs("Setting PCIe...", &graphics);
    {
        size_t entries = (mcfg->header.len - sizeof(ACPI_SDT_HEADER) - sizeof(mcfg->res)) / sizeof(ACPI_MCFG_CONFIGURATION_SPACE_ENTRY);
        for (size_t i = 0; i < entries; i++) {
            for (size_t j = mcfg->config_space[i].pci_num_start; j < mcfg->config_space[i].pci_num_end; j++) {
                for (uint8_t k = 0; k < 32; k++) {
                    for (uint8_t l = 0; l < 8; l++) {
                        PCIE_CONFIGURATION_SPACE_HEADER_TYPE_0* ecam_addr = (PCIE_CONFIGURATION_SPACE_HEADER_TYPE_0*)(mcfg->config_space[i].base_addr + ((j << 20) | (k << 15) | (l << 12)));
                        // map_page(&page_bitmap, pml4, (void*)ecam_addr, (void*)ecam_addr);
                        // invlpg((uint64_t)ecam_addr); 
                        if (pcie_array_to_uint64(ecam_addr->class_code, sizeof(ecam_addr->class_code)) == 0x01) kputs("mass_storage!!!!!\n", &graphics);
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Exception #GP here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
                    }
                }
            }
        }
    }
    kputs("Done\r\n", &graphics);

    kputs("Setting syscall...", &graphics);
    {
/* set IA32_EFER.SCE (bit 0) to enable syscall/sysret instruction */
        wrmsr(0xc0000080, rdmsr(0xc0000080) | 1);
        
/* write syscall handler address to IA32_LSTAR */
        wrmsr(0xc0000082, (uint64_t)syscall_handler);
            
/* write segment selectors to IA32_STAR */
/* kernel code for syscall, user code for sysret */
        wrmsr(0xc0000081, (rdmsr(0xc0000081) & 0xffffffff) | ((uint64_t)0x08 << 32) | ((uint64_t)0x18 << 48));
        wrmsr(0xc0000084, rdmsr(0xc0000084) | (uint64_t)(1 << 9));
    }
    kputs("Done\r\n", &graphics);

    kputs("Entering user mode...\r\n\r\n", &graphics);
    

    
    
    while (1);
}