#ifndef _DEV_AHCI_H
#define _DEV_AHCI_H

#include "../../sys/stdint.h"
#include "../../sys/stddef.h"


typedef struct {
    uint32_t id;
    uint16_t cmd;
    uint16_t sts;
    uint8_t rid;
    uint32_t cc : 24;
    uint8_t mlt;
    uint8_t htype;
    uint8_t bist;
    uint8_t bars[14];
    uint32_t abar;
    uint32_t res0;
    uint32_t ss;
    uint32_t erom;
    uint8_t cap;
    uint16_t intr;
    uint8_t mgnt;
    uint8_t mlat;
}__attribute__((packed)) PCI_HEADER;
typedef struct {
    uint8_t pid;
    uint16_t pc;
    uint16_t pmcs;
}__attribute__((packed)) PCI_POWER_MANAGEMENT_CAPABILITIES;


#endif