#include "acpi.h"


void* acpi_get_table(ACPI_XSDT* xsdt, const char* name) {
    ACPI_SDT_HEADER* header;
    size_t entries = (xsdt->header.len - sizeof(ACPI_SDT_HEADER)) / 8;
    
    for (size_t i = 0; i < entries; i++) {
        header = (ACPI_SDT_HEADER*)xsdt->entry[i];

        if (!strncmp(header->sign, name, 4)) {
            return (void*)header;
        }
    }

    return NULL;
}
void acpi_set_apic_timer(ACPI_MADT* madt, int int_per_second) {
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
    uint8_t tmp = (inb(0x61) & 0xfe);
    outb(tmp, 0x61);
    outb(tmp | 1, 0x61);

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

    uint32_t tmp0 = *(apic_base + APIC_REGISTER_TIMER_ICR);
    uint32_t tmp1 = (((uint64_t)((0xffffffff - tmp0 + 1) << 4) * 100) / int_per_second) >> 4;

/* sanity check*/
    if (tmp1 >= 0x10) {
        *(apic_base + APIC_REGISTER_TIMER_ICR) = tmp1;

/* re-enable timer in periodic mode */
        *(apic_base + APIC_REGISTER_LVT_TIMER) = 0x20 | 0x20000;
    }
}