#include "pcie.h"


/* size_t size0, size_t arg0, size_t size1, size_t arg1, ... */
uint8_t* pcie_concat_split_data(size_t argc, ...) {
    size_t size;
    size_t arg;
    size_t ret_i = 0;
    static uint8_t ret[64];
    va_list ap;

    va_start(ap, argc);

    for (size_t i = 0; i < argc; i++) {
        size = va_arg(ap, size_t);
        arg = va_arg(ap, size_t);

        for (size_t j = 0; j < size; j++) {
            ret[ret_i++] = (arg >> j * 8) & (0xff);
        }
    }

    va_end(ap);

    return ret;
}
uint64_t* pcie_get_config_space_addrs(ACPI_MCFG* mcfg) {
    size_t config_space_entries = (mcfg->header.len - sizeof(ACPI_SDT_HEADER) - sizeof(mcfg->res)) / sizeof(ACPI_MCFG_CONFIGURATION_SPACE_ENTRY);
    static uint64_t config_space_addrs[0xfff];
    uint8_t index = 0;

    for (size_t i = 0; i < 0xfff; i++) {
        config_space_addrs[i] = 0;
    }
    for (size_t i = 0; i < config_space_entries; i++) {
        for (size_t j = mcfg->config_space[i].pci_num_start; j < mcfg->config_space[i].pci_num_end; j++) {
            for (uint8_t k = 0; k < 32; k++) {
                for (uint8_t l = 0; l < 8; l++) {
                    config_space_addrs[index++] = (mcfg->config_space[i].base_addr + ((size_t)j << 20) + ((size_t)k << 15) + ((size_t)l << 12));
                }
            }
        }
    }

    return config_space_addrs;
}
PCIE_CONFIGURATION_SPACE_HEADER pcie_read_config_space_header(uint64_t config_space_addr) {
    PCIE_CONFIGURATION_SPACE_HEADER ret = {0, };

    for (size_t i = 0; i < PCIE_CONFIGURATION_SPACE_HEADER_SIZE / 4; i++) {
        *(uint32_t*)((uintptr_t)&ret + i * 4) = *(uint32_t*)(config_space_addr + i * 4);
    }

    return ret;
}