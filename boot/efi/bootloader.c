#include "efi.h"
#include "efilib.h"
#include "inc/elf.h"
#include "inc/bootinfo.h"
#include "inc/gpt.h"
#include "inc/ext2.h"
#include "inc/acpi.h"


BOOLEAN memcmp(VOID* _mem1, VOID* _mem2, UINTN size) {
    UINT8* mem1 = (UINT8*)_mem1;
    UINT8* mem2 = (UINT8*)_mem2;

    for (UINTN i = 0; i < size; i++) {
        if (mem1[i] != mem2[i]) return FALSE;
    }
    return TRUE;
}
BOOLEAN guidcmp(EFI_GUID g1, EFI_GUID g2) {
    if (
        g1.Data1 == g2.Data1 &&
        g1.Data2 == g2.Data2 &&
        g1.Data3 == g2.Data3 &&
        memcmp((VOID*)g1.Data4, (VOID*)g2.Data4, 8)
    ) return TRUE;
    return FALSE;
}
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    InitializeLib(ImageHandle, SystemTable);

    EFI_STATUS Status;

    /*********************** Locate GOP **********************/
    EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP;
    {
        Status = uefi_call_wrapper(BS->LocateProtocol, 3, &gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&GOP);
        if (EFI_ERROR(Status)) {
            Print(L"Could not locate GOP; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info;
        UINTN info_size, mode_count, native_mode;

        Status  = uefi_call_wrapper(GOP->QueryMode, 4, GOP, GOP->Mode == NULL ? 0 : GOP->Mode->Mode, &info_size, &info);
        if (Status == EFI_NOT_STARTED) {
            Status = uefi_call_wrapper(GOP->SetMode, 2, GOP, 0);
            if (EFI_ERROR(Status)) {
                Print(L"Could not set graphic mode; Error Code: 0x%02x\r\n", Status);
                while (1);
            }
        }

        native_mode = GOP->Mode->Mode;
        mode_count = GOP->Mode->MaxMode;

        for (UINTN i = 0; i < mode_count; i++) {
            Status = uefi_call_wrapper(GOP->QueryMode, 4, GOP, i, &info_size, &info);
            if (EFI_ERROR(Status)) {
                Print(L"Could not get graphic mode; Error Code: 0x%02x\r\n", Status);
                while (1);
            }
            if (info->HorizontalResolution == 1920 && info->VerticalResolution == 1080) {
                Status = uefi_call_wrapper(GOP->SetMode, 2, GOP, i);
                if (EFI_ERROR(Status)) {
                    Print(L"Could not set graphic mode; Error Code: 0x%02x\r\n", Status);
                    while (1);
                }
                break;
            }
        }
    }
    /*********************************************************/

    /******************* Get Kernel *******************/
    UINT8* Kernel;
    EFI_BLOCK_IO_PROTOCOL* block_io;
    UINT32 media_id;
    UINTN lba_size = 512;
    BOOTDISK_ADDRESS bootdisk_addr;
    {
        EFI_HANDLE* block_ios;
        UINTN block_io_num;

        Status = uefi_call_wrapper(BS->LocateHandleBuffer, 5, ByProtocol, &gEfiBlockIo2ProtocolGuid, NULL, &block_io_num, &block_ios);
        if (EFI_ERROR(Status)) {
            Print(L"Could not locate BlockIoProtocol; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        GPT_HEADER* gpt_header = NULL;
        BOOLEAN disk_found = 0;

        Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, lba_size, (VOID**)&gpt_header);
        if (EFI_ERROR(Status)) {
            Print(L"Could not allocate buffer for BlockIoProtocol; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        for (UINTN i = 0; i < block_io_num; i++) {
            Status = uefi_call_wrapper(BS->HandleProtocol, 3, block_ios[i], &gEfiBlockIoProtocolGuid, (VOID**)&block_io);
            if (EFI_ERROR(Status)) continue;
            if ((block_io->Media->LogicalPartition)) continue;

            media_id = block_io->Media->MediaId;

            Status = uefi_call_wrapper(block_io->ReadBlocks, 5, block_io, media_id, 1, lba_size, (VOID*)gpt_header);
            if (EFI_ERROR(Status)) continue;

            if (strncmpa(gpt_header->sign, "EFI PART", 8) == 0) {
                Print(L"Found Disk.\r\n");
                disk_found = 1;

                EFI_DEVICE_PATH_PROTOCOL* bootdisk_path;
                Status = uefi_call_wrapper(BS->HandleProtocol, 3, block_ios[i], &gEfiDevicePathProtocolGuid, (VOID**)&bootdisk_path);
                if (EFI_ERROR(Status)) {
                    Print(L"Could not locate DevicePathProtocol; Error Code: 0x%02x\r\n", Status);
                    while (1);
                }

                EFI_HANDLE bootdisk_handle;
                Status = uefi_call_wrapper(BS->LocateDevicePath, 3, &gEfiPciIoProtocolGuid, &bootdisk_path, &bootdisk_handle);
                if (EFI_ERROR(Status)) {
                    Print(L"Could not locate PciIoProtocol handle; Error Code: 0x%02x\r\n", Status);
                    while (1);
                }
                EFI_PCI_IO_PROTOCOL* bootdisk;
                Status = uefi_call_wrapper(BS->HandleProtocol, 3, bootdisk_handle, &gEfiPciIoProtocolGuid, (VOID**)&bootdisk);
                if (EFI_ERROR(Status)) {
                    Print(L"Could not locate PciIoProtocol; Error Code: 0x%02x\r\n", Status);
                    while (1);
                }

                UINTN seg, bus, dev, func;
                Status = uefi_call_wrapper(bootdisk->GetLocation, 5, bootdisk, &seg, &bus, &dev, &func);
                if (EFI_ERROR(Status)) {
                    Print(L"Could not get location of bootdisk; Error Code: 0x%02x\r\n", Status);
                    while (1);
                }

                bootdisk_addr.seg = (UINTN)seg;
                bootdisk_addr.bus = (UINT8)bus;
                bootdisk_addr.dev = (UINT8)dev;
                bootdisk_addr.func = (UINT8)func;
                
                
                break;
            }
        }
        if (!(disk_found)) {
            Print(L"Could not locate BlockIoProtocol of disk; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        EXT2_SUPERBLOCK* primary_part = NULL;
        GPT_PARTITION_ENTRY* part_tbl = NULL;
        UINT64 part_tbl_start_lba = gpt_header->part_tbl_start_lba;
        UINT32 part_entry_num = gpt_header->part_entry_num;
        UINT8 tbl_size = 0x80;
        BOOLEAN part_found = 0;
        UINT64 primary_part_start_lba;

        Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, tbl_size * part_entry_num, (VOID**)&part_tbl);
        if (EFI_ERROR(Status)) {
            Print(L"Could not allocate buffer for GPT partition entry table; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, 512, (VOID**)&primary_part);
        if (EFI_ERROR(Status)) {
            Print(L"Could not allocate buffer for primary partition; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        Status = uefi_call_wrapper(block_io->ReadBlocks, 5, block_io, media_id, part_tbl_start_lba, tbl_size * part_entry_num, (VOID*)part_tbl);
        if (EFI_ERROR(Status)) {
            Print(L"Could not read partition table; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        for (UINTN i = 0; i < part_entry_num; i++) {
            Status = uefi_call_wrapper(block_io->ReadBlocks, 5, block_io, media_id, part_tbl[i].part_start_lba + 2, 512, (VOID*)primary_part); // SuperBlock of ext2 locates at LBA 2.
            if (EFI_ERROR(Status)) continue;

            if (primary_part->magic == 0xef53) {
                Print(L"Found Primary Partition.\r\n");
                part_found = 1;
                primary_part_start_lba = part_tbl[i].part_start_lba;

                break;
            }
        }
        if (!(part_found)) {
            Print(L"Could not load primary partition; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        Status = uefi_call_wrapper(BS->FreePool, 1, (VOID*)part_tbl);
        if (EFI_ERROR(Status)) {
            Print(L"Could not free buffer partition table table; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        Status = uefi_call_wrapper(BS->FreePool, 1, (VOID*)gpt_header);
        if (EFI_ERROR(Status)) {
            Print(L"Could not free buffer for gpt header; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        EXT2_BLOCK_GROUP_DESCRIPTOR* group_desc_tbl = NULL;
        UINT64 block_size = 1024 << primary_part->log_block_size;
        UINT16 inode_size = primary_part->inode_size;
        UINT32 inode_per_group = primary_part->inode_per_group;
        VOID* inode_tbl = NULL;
        EXT2_INODE* root_dir_inode;

        Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, block_size, (VOID**)&group_desc_tbl);
        if (EFI_ERROR(Status)) {
            Print(L"Could not allocate buffer for group descriptor table; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, block_size, (VOID**)&inode_tbl);
        if (EFI_ERROR(Status)) {
            Print(L"Could not allocate buffer for inode table; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        Status = uefi_call_wrapper(block_io->ReadBlocks, 5, block_io, media_id, primary_part_start_lba + (block_size / lba_size * 1), block_size, (VOID*)group_desc_tbl);
        if (EFI_ERROR(Status)) {
            Print(L"Could not read group descriptor table; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        Status = uefi_call_wrapper(block_io->ReadBlocks, 5, block_io, media_id, primary_part_start_lba + (group_desc_tbl[0].inode_table * block_size / lba_size), block_size, (VOID*)inode_tbl);
        if (EFI_ERROR(Status)) {
            Print(L"Could not read inode table; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        root_dir_inode = (EXT2_INODE*)((UINT8*)inode_tbl + inode_size * 1);

        if ((root_dir_inode->mode & 0xF000) != 0x4000) {
            Print(L"Could not find /.\r\n");
            while (1);
        }
        else Print(L"Found /.\r\n");

        VOID* root_dir = NULL;

        Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, root_dir_inode->block_cnt * 512, (VOID**)&root_dir);
        if (EFI_ERROR(Status)) {
            Print(L"Could not allocate buffer for root directory; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        for (UINTN i = 0; i < root_dir_inode->block_cnt * 512 / block_size; i++) {
            Status = uefi_call_wrapper(block_io->ReadBlocks, 5, block_io, media_id, primary_part_start_lba + (root_dir_inode->block[i] * block_size / lba_size), block_size, (VOID*)((UINT8*)root_dir + block_size * i));
            if (EFI_ERROR(Status)) {
                Print(L"Could not read root directory; Error Code: 0x%02x\r\n", Status);
                while (1);
            }
        }

        EXT2_DIRECTORY_ENTRY* boot_dir_entry;
        {
            VOID* tmp = (VOID*)root_dir;

            for (UINTN i = 0; ; i++) {
                if (strncmpa(((EXT2_DIRECTORY_ENTRY*)tmp)->name, "boot", 4) == 0) {
                    boot_dir_entry = (EXT2_DIRECTORY_ENTRY*)tmp;
                    Print(L"Found /boot.\r\n");

                    break;
                }
                if (((EXT2_DIRECTORY_ENTRY*)tmp)->name_len > ((EXT2_DIRECTORY_ENTRY*)tmp)->rec_len - 8)
                    continue;
                if (i > root_dir_inode->block_cnt * 512) {
                    Print(L"Could not find /boot; Error Code: 0x%02x\r\n", Status);
                    while (1);
                }
                
                tmp += (UINT16)(((EXT2_DIRECTORY_ENTRY*)tmp)->rec_len);
            }
        }

        Status = uefi_call_wrapper(block_io->ReadBlocks, 5, block_io, media_id, primary_part_start_lba + (group_desc_tbl[boot_dir_entry->inode / inode_per_group].inode_table * block_size / lba_size), block_size, inode_tbl);
        if (EFI_ERROR(Status)) {
            Print(L"Could not free buffer for inode table; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        EXT2_INODE* boot_dir_inode = (EXT2_INODE*)((UINT8*)inode_tbl + inode_size * (boot_dir_entry->inode % inode_per_group - 1));
        VOID* boot_dir = NULL;

        Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, boot_dir_inode->block_cnt * 512, (VOID**)&boot_dir);
        if (EFI_ERROR(Status)) {
            Print(L"Could not allocate buffer for boot directory; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        for (UINTN i = 0; i < boot_dir_inode->block_cnt * 512 / block_size; i++) {
            Status = uefi_call_wrapper(block_io->ReadBlocks, 5, block_io, media_id, primary_part_start_lba + (boot_dir_inode->block[i] * block_size / lba_size), block_size, (VOID*)((UINT8*)boot_dir + (block_size * i)));
            if (EFI_ERROR(Status)) {
                Print(L"Could not read boot directory; Error Code: 0x%02x\r\n", Status);
                while (1);
            }
        }

        EXT2_DIRECTORY_ENTRY* kernel_entry;
        {
            VOID* tmp = (VOID*)boot_dir;

            for (UINTN i = 0; ; i++) {
                if (strncmpa(((EXT2_DIRECTORY_ENTRY*)tmp)->name, "kernel", 6) == 0) {
                    kernel_entry = (EXT2_DIRECTORY_ENTRY*)tmp;
                    Print(L"Found /boot/kernel.\r\n");

                    break;
                }
                if (((EXT2_DIRECTORY_ENTRY*)tmp)->name_len > ((EXT2_DIRECTORY_ENTRY*)tmp)->rec_len - 8)
                    continue;
                if (i > boot_dir_inode->block_cnt * 512) {
                    Print(L"Could not find /boot/kernel; Error Code: 0x%02x\r\n", Status);
                    while (1);
                }

                tmp += (UINT16)(((EXT2_DIRECTORY_ENTRY*)tmp)->rec_len);
            }
        }

        Status = uefi_call_wrapper(block_io->ReadBlocks, 5, block_io, media_id, primary_part_start_lba + (group_desc_tbl[kernel_entry->inode / inode_per_group].inode_table * block_size / lba_size), block_size, inode_tbl);
        if (EFI_ERROR(Status)) {
            Print(L"Could not free buffer for inode table; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        EXT2_INODE* kernel_file_inode = (EXT2_INODE*)((UINT8*)inode_tbl + inode_size * (kernel_entry->inode % inode_per_group - 1));
        VOID* kernel_file = NULL;

        Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, kernel_file_inode->block_cnt * 512, (VOID**)&kernel_file);
        if (EFI_ERROR(Status)) {
            Print(L"Could not allocate buffer for kernel loader; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        for (UINTN i = 0; i < kernel_file_inode->block_cnt * 512 / block_size; i++) {
            Status = uefi_call_wrapper(block_io->ReadBlocks, 5, block_io, media_id, primary_part_start_lba + (kernel_file_inode->block[i] * block_size / lba_size), block_size, (VOID*)((UINT8*)kernel_file + (block_size * i)));
            if (EFI_ERROR(Status)) {
                Print(L"Could not read kernel loader; Error Code: 0x%02x\r\n", Status);
                while (1);
            }
        }

        Status = uefi_call_wrapper(BS->FreePool, 1, (VOID*)group_desc_tbl);
        if (EFI_ERROR(Status)) {
            Print(L"Could not free buffer for group descriptor table; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        Status = uefi_call_wrapper(BS->FreePool, 1, (VOID*)primary_part);
        if (EFI_ERROR(Status)) {
            Print(L"Could not free buffer for primary partition; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        Status = uefi_call_wrapper(BS->FreePool, 1, (VOID*)inode_tbl);
        if (EFI_ERROR(Status)) {
            Print(L"Could not free buffer for inode table; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        Status = uefi_call_wrapper(BS->FreePool, 1, (VOID*)root_dir);
        if (EFI_ERROR(Status)) {
            Print(L"Could not free buffer for root directory entry; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        Status = uefi_call_wrapper(BS->FreePool, 1, (VOID*)boot_dir);
        if (EFI_ERROR(Status)) {
            Print(L"Could not free buffer for boot directory entry; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        Kernel = kernel_file;
    }
    /*********************************************************/

    /*********************** Parse ELF ***********************/
    Elf64_Addr KernelEntry;
    {
        Elf64_Ehdr ehdr;
        UINTN ehdr_size = sizeof(ehdr);
        
        for (UINTN i = 0; i < ehdr_size; i++) 
            ((UINT8*)(&ehdr))[i] = Kernel[i];
        
        if (
            !memcmp(&ehdr.e_ident[EI_MAG0], ELFMAG, SELFMAG) ||
            ehdr.e_ident[EI_CLASS] != ELFCLASS64 ||
            ehdr.e_ident[EI_DATA] != ELFDATA2LSB ||
            ehdr.e_type != ET_EXEC ||
            ehdr.e_machine != EM_X86_64 ||
            ehdr.e_version != EV_CURRENT
        ) Status = !EFI_SUCCESS;
        if (EFI_ERROR(Status)) {
            Print(L"Invalid ELF header.\r\n");
            while (1);
        }

        Elf64_Phdr* phdr_buffer;
        UINTN phdr_buffer_size = ehdr.e_phnum * ehdr.e_phentsize;

        Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, phdr_buffer_size, (VOID**)&phdr_buffer);
        if (EFI_ERROR(Status)) {
            Print(L"Could not allocate buffer for phdr buffer; Error Code: 0x%02x\r\n", Status);
            while (1);
        }

        for (UINTN i = 0; i < phdr_buffer_size; i++)
            ((UINT8*)phdr_buffer)[i] = Kernel[ehdr.e_phoff + i];
        for (
            Elf64_Phdr* phdr = phdr_buffer;
            (UINT8*)phdr < (UINT8*)phdr_buffer + phdr_buffer_size;
            phdr = (Elf64_Phdr*)((UINT8*)phdr + ehdr.e_phentsize)
        ) {
            if (phdr->p_type == PT_LOAD) {
                UINTN pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
                Elf64_Addr seg = phdr->p_paddr;
                
                Status = uefi_call_wrapper(BS->AllocatePages, 4, AllocateAddress, EfiLoaderData, pages, &seg);
                if (EFI_ERROR(Status)) {
                    Print(L"Could not allocate buffer for ELF segment; Error Code: 0x%02x\r\n", Status);
                    while (1);
                }
                
                UINTN phdr_size = phdr->p_filesz;
                
                for (UINTN i = 0; i < phdr_size; i++)
                    ((UINT8*)seg)[i] = Kernel[phdr->p_offset + i];
            }
        }

        KernelEntry = ehdr.e_entry;
    }
    /*********************************************************/

    /******************* Get EFI Memory Map ******************/
    EFI_MEMORY_DESCRIPTOR* mem_map = NULL;
    UINTN mem_map_size, mem_map_key, desc_size;
    UINT32 desc_ver;
    {
        uefi_call_wrapper(BS->GetMemoryMap, 5, &mem_map_size, mem_map, &mem_map_key, &desc_size, &desc_ver);
        uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, mem_map_size, (VOID**)&mem_map);

        mem_map_size += sizeof(EFI_MEMORY_DESCRIPTOR) + sizeof(VOID*);

        Status = uefi_call_wrapper(BS->GetMemoryMap, 5, &mem_map_size, mem_map, &mem_map_key, &desc_size, &desc_ver);
        if (EFI_ERROR(Status)) {
            Print(L"Could not get EFI memory map; Error Code: 0x%02x\r\n", Status);
            while (1);
        }
    }
    /*********************************************************/

    /************************ Get XSDT ***********************/
    ACPI_XSDP* xsdp;
    ACPI_XSDT* xsdt;
    {
        EFI_GUID stct;
        EFI_GUID acpi20 = ACPI_20_TABLE_GUID;
        for (UINT32 i = 0; i < ST->NumberOfTableEntries; i++) {
            stct = ST->ConfigurationTable[i].VendorGuid;
            if (guidcmp(acpi20, stct)) xsdp = (ACPI_XSDP*)(ST->ConfigurationTable[i].VendorTable);
        }
        if (strncmpa((VOID*)xsdp->sign, (VOID*)"RSD PTR ", 8)) {
            Print(L"Invalid XSDP.\r\n");
            while (1);
        }
            
        UINT8 ret = 0;
        for (UINT8 i = 0; i < 20; i++)
            ret += ((UINT8*)xsdp)[i];
        if (ret != 0) {
            Print(L"Invalid XSDP.\r\n");
            while (1);
        }
        for (UINT8 i = 0; i < 36; i++)
            ret += ((UINT8*)xsdp)[i];
        if (ret != 0) {
            Print(L"Invalid XSDP.\r\n");
            while (1);
        }
        
        xsdt = (ACPI_XSDT*)xsdp->xsdt_addr;
        if (strncmpa((VOID*)xsdt->header.sign, (VOID*)"XSDT", 4)) {
            Print(L"Invalid XSDT.\r\n");
            while (1);
        }
        for (UINT64 i = 0; i < xsdt->header.len; i++) {
            ret += ((UINT8*)xsdt)[i];
        }
        if (ret != 0) {
            Print(L"Invalid XSDT.\r\n");
            while (1);
        }
    }

    /****************** Start Kernel ******************/
    BOOTINFO BootInfo = {
        {
            GOP->Mode->FrameBufferBase,
            GOP->Mode->FrameBufferSize,
            GOP->Mode->Info->HorizontalResolution,
            GOP->Mode->Info->VerticalResolution,
            GOP->Mode->Info->PixelsPerScanLine
        },
        {
            (VOID*)mem_map,
            mem_map_size,
            desc_size
        },
        (UINT64)xsdt,
        (BOOTDISK_ADDRESS)bootdisk_addr
    };

    VOID (*KernelStart)(VOID*) = ((__attribute__((sysv_abi)) VOID (*)(VOID*)) KernelEntry);

    Print(L"Loading Kernel...\r\n");

    uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
    uefi_call_wrapper(BS->ExitBootServices, 2, ImageHandle, mem_map_key);

    KernelStart(&BootInfo);
    /*********************************************************/

    return EFI_SUCCESS;
}