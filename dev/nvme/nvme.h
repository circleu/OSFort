#ifndef _DEV_NVME_H
#define _DEV_NVME_H

#include "../../sys/stdint.h"

#define NVME_ADMIN_QUEUE_ENTRIES    32
#define NVME_SQE_SIZE               64
#define NVME_CQE_SIZE               16
#define NVME_SQE_SIZE_LOG           6
#define NVME_CQE_SIZE_LOG           4


/* 
 * *    Implementation Specification
 * ^    See Above
 * H    HwInit
 *
 * RO   Read-only
 * RW   Read-and-Write
 * RWC  Read-and-Write '1' to Clear
 * RWS  Read-and-Wrtie '1' to Set;
 */
//                                 Type Reset
typedef struct {
    uint16_t mqes : 16;         // RO   *
    uint16_t cqr : 1;           // RO   *
    uint16_t ams : 2;           // RO   *
    uint16_t res0 : 5;          // RO   0x00
    uint16_t to : 8;            // RO   *
    uint16_t dstrd : 4;         // RO   *
    uint16_t nssrs : 1;         // RO   *
    uint16_t css : 8;           // RO   *
    uint16_t bps : 1;           // RO   *
    uint16_t cps : 2;           // RO   *
    uint16_t mpsmin : 4;        // RO   *
    uint16_t mpsmax : 4;        // RO   *
    uint16_t pmrs : 1;          // RO   *
    uint16_t cmbs : 1;          // RO   *
    uint16_t nsss : 1;          // RO   *
    uint16_t crms : 2;          // RO   *
    uint16_t nsses : 1;         // RO   *
    uint16_t res1 : 2;          // RO   0x00
}__attribute__((packed)) NVME_CONTROLLER_CAPABILITIES;
/*
 * Reset Values by NVMe Versions:
 * Version  MJR         MNR         TER
 * 
 * 1.0      0x01        0x00        0x00
 * 1.1      0x01        0x01        0x00
 * 1.2      0x01        0x02        0x00
 * 1.2.1    0x01        0x02        0x01
 * 1.3      0x01        0x03        0x00
 * 1.4      0x01        0x04        0x00
 * 2.0      0x02        0x00        0x00
 * 2.1      0x02        0x01        0x00
 * 2.2      0x02        0x02        0x00
 * 2.3      0x02        0x03        0x00
 */
typedef struct {
    uint16_t ter : 8;           // RO   ^
    uint16_t mnr : 8;           // RO   ^
    uint16_t mjr : 16;          // RO   ^
}__attribute((packed)) NVME_SPECIFICATION_VERSION_DESCRIPTOR;
typedef struct {
    uint32_t ivms : 32;         // RWS  0x00
}__attribute__((packed)) NVME_INTERRUPT_MASK_SET;
typedef struct {
    uint32_t ivmc : 32;         // RWC  0x00
}__attribute__((packed)) NVME_INTERRUPT_MASK_CLEAR;
typedef struct {
    uint16_t en : 1;            // RW   0b0
    uint16_t res0 : 3;          // RO   0b000
    uint16_t css : 3;           // RW   0b000
    uint16_t mps : 4;           // RW   0x00
    uint16_t ams : 3;           // RW   0b000
    uint16_t shn : 2;           // RW   0b00
    uint16_t iosqes : 4;        // RW   0x00
    uint16_t iocqes : 4;        // RW   0x00
    uint16_t crime : 1;         // RW   0b0
    uint16_t res1 : 7;          // RO   0x00
}__attribute__((packed)) NVME_CONTROLLER_CONFIGURATION;
typedef struct {
    uint16_t rdy : 1;           // RO   0b0
    uint16_t cfs : 1;           // RO   H
    uint16_t shst : 2;          // RO   0b00
    uint16_t nssro : 1;         // RWC  H
    uint16_t pp : 1;            // RO   0b0
    uint16_t st : 1;            // RO   H
    uint32_t res : 25;          // RO   0x00
}__attribute__((packed)) NVME_CONTROLLER_STATUS;
typedef struct {
    uint32_t nssrc : 32;        // RW   0x00
}__attribute__((packed)) NVME_NVM_SUBSYSTEM_RESET;
typedef struct {
    uint16_t asqs : 12;         // RW   0x00
    uint16_t res0 : 4;          // RO   0x00
    uint16_t acqs : 12;         // RW   0x00
    uint16_t res1 : 4;          // RO   0x00
}__attribute__((packed)) NVME_ADMIN_QUEUE_ATTRIBUTES;
typedef struct {
    uint16_t res : 12;          // RO   0x00
    uint64_t asqb : 52;         // RW   *
}__attribute__((packed)) NVME_ADMIN_SUBMISSION_QUEUE_BASE_ADDRESS;
typedef struct {
    uint16_t res : 12;          // RO   0x00
    uint64_t acqb : 52;         // RW   *
}__attribute__((packed)) NVME_ADMIN_COMPLETION_QUEUE_BASE_ADDRESS;
typedef struct {
    uint16_t bir : 3;           // RO   *
    uint16_t cqmms : 1;         // RO   *
    uint16_t cqpds : 1;         // RO   *
    uint16_t cdpmls : 1;        // RO   *
    uint16_t cdpcils : 1;       // RO   *
    uint16_t cdmmms : 1;        // RO   *
    uint16_t cqda : 1;          // RO   *
    uint16_t res : 3;           // RO   0b000
    uint32_t ofst : 20;         // RO   *
}__attribute__((packed)) NVME_CONTROLLER_MEMORY_BUFFER_LOCATION;
typedef struct {
    uint16_t sqs : 1;           // RO   *
    uint16_t cqs : 1;           // RO   *
    uint16_t lists : 1;         // RO   *
    uint16_t rds : 1;           // RO   *
    uint16_t wds : 1;           // RO   *
    uint16_t res : 3;           // RO   0b000
    uint16_t szu : 4;           // RO   *
    uint32_t sz : 20;           // RO   *
}__attribute__((packed)) NVME_CONTROLLER_MEMORY_BUFFER_SIZE;
typedef struct {
    uint16_t bpsz : 15;         // RO   *
    uint16_t res0 : 9;          // RO   0x00
    uint16_t brs : 2;           // RO   0b00
    uint16_t res1 : 5;          // RO   0x00
    uint16_t abpid : 1;         // RO   *
}__attribute__((packed)) NVME_BOOT_PARTITION_INFORMATION;
typedef struct {
    uint16_t bprsz : 10;        // RW   0x00
    uint32_t bprof : 20;        // RW   0x00
    uint16_t res : 1;           // RO   0b0
    uint16_t bpid : 1;          // RW   0b0
}__attribute__((packed)) NVME_BOOT_PARTITION_READ_SELECT;
typedef struct {
    uint16_t res : 12;          // RO   0x00
    uint64_t bmbba : 52;        // RW   0x00
}__attribute__((packed)) NVME_BOOT_PARTITION_MEMORY_BUFFER_LOCATION;
typedef struct {
    uint16_t cre : 1;           // RW   0b0
    uint16_t cmse : 1;          // RW   0b0
    uint16_t res : 10;          // RO   0x00
    uint64_t cba : 52;          // RW   0x00
}__attribute__((packed)) NVME_CONTROLLER_MEMORY_BUFFER_MEMORY_SPCAE_CONTROL;
typedef struct {
    uint16_t cbai : 1;          // RO   0b0
    uint32_t res : 31;          // RO   0x00
}__attribute__((packed)) NVME_CONTROLLER_MEMORY_BUFFER_STATUS;
typedef struct {
    uint16_t cmbszu : 4;        // RO   *
    uint16_t cmbrbb : 1;        // RO   *
    uint16_t res : 3;           // RO   0x00
    uint32_t cmbwbz : 24;       // RO   *
}__attribute__((packed)) NVME_CONTROLLER_MEMORY_BUFFER_ELASTICITY_BUFFER_SIZE;
typedef struct {
    uint16_t cmbswtu : 4;       // RO   *
    uint16_t res : 4;           // RO   0x00
    uint32_t cmbswtv : 24;      // RO   *
}__attribute__((packed)) NVME_CONTROLLER_MEMORY_BUFFER_SUSTAINED_WRITE_THROUGHPUT;
typedef struct {
    uint32_t nssc : 32;         // RW   0x00
}__attribute__((packed)) NVME_NVM_SUBSYSTEM_SHUTDOWN;
typedef struct {
    uint16_t crwmt : 16;        // RO   *
    uint16_t crimt : 16;        // RO   *
}__attribute__((packed)) NVME_CONTROLLER_READY_TIMEOUTS;
typedef struct {
    uint16_t res0 : 3;          // RO   0b000
    uint16_t rds : 1;           // RO   *
    uint16_t wds : 1;           // RO   *
    uint16_t bir : 3;           // RO   *
    uint16_t pmrtu : 2;         // RO   *
    uint16_t pmrwbm : 4;        // RO   *
    uint16_t res1 : 2;          // RO   0b00
    uint16_t pmrto : 8;         // RO   *
    uint16_t cmss : 1;          // RO   *
    uint16_t res2 : 7;          // RO   0x00
}__attribute__((packed)) NVME_PERSISTENT_MEMORY_REGION_CAPABILITIES;
typedef struct {
    uint16_t en : 1;            // RW   0b0
    uint32_t res : 31;          // RO   0x00
}__attribute__((packed)) NVME_PERSISTENT_MEMORY_REGION_CONTROL;
typedef struct {
    uint16_t err : 8;           // RO   0x00
    uint16_t nrdy : 1;          // RO   0b0
    uint16_t hsts : 3;          // RO   0b000
    uint16_t cbai : 1;          // RO   0b0
    uint32_t res : 19;          // RO   0x00
}__attribute__((packed)) NVME_PERSISTENT_MEMORY_REGION_STATUS;
typedef struct {
    uint16_t pmrszu : 4;        // RO   *
    uint16_t pmrrbb : 1;        // RO   *
    uint16_t res : 3;           // RO   0b000
    uint32_t pmrwbs : 24;       // RO   *
}__attribute__((packed)) NVME_PERSISTENT_MEMORY_REGION_ELASTICITY_BUFFER_SIZE;
typedef struct {
    uint16_t pmrswtu : 4;       // RO   *
    uint16_t res : 4;           // RO   0x00
    uint32_t pmrswtv : 24;      // RO   *
}__attribute__((packed)) NVME_PERSISTENT_MEMORY_REGION_SUSTAINED_WRITE_THROUGHPUT;
typedef struct {
    uint16_t res0 : 1;          // RO   0b0
    uint16_t cmse : 1;          // RW   0b0
    uint16_t res1 : 10;         // RO   0x00
    uint32_t cba : 20;          // RW   0x00
}__attribute__((packed)) NVME_PERSISTENT_MEMORY_REGION_MEMORY_SPACE_CONTROL_LOWER;
typedef struct {
    uint32_t cba : 32;          // RW   0x00
}__attribute__((packed)) NVME_PERSISTENT_MEMORY_REGION_MEMORY_SPACE_CONTROL_UPPER;
typedef struct {
    NVME_CONTROLLER_CAPABILITIES cap;
    NVME_SPECIFICATION_VERSION_DESCRIPTOR vs;
    NVME_INTERRUPT_MASK_SET intms;
    NVME_INTERRUPT_MASK_CLEAR intmc;
    NVME_CONTROLLER_CONFIGURATION cc;
    uint32_t res0;
    NVME_CONTROLLER_STATUS csts;
    NVME_NVM_SUBSYSTEM_RESET nssr;
    NVME_ADMIN_QUEUE_ATTRIBUTES aqa;
    NVME_ADMIN_SUBMISSION_QUEUE_BASE_ADDRESS asq;
    NVME_ADMIN_COMPLETION_QUEUE_BASE_ADDRESS acq;
    NVME_CONTROLLER_MEMORY_BUFFER_LOCATION cmbloc;
    NVME_CONTROLLER_MEMORY_BUFFER_SIZE cmbsz;
    NVME_BOOT_PARTITION_INFORMATION bpinfo;
    NVME_BOOT_PARTITION_READ_SELECT bprsel;
    NVME_BOOT_PARTITION_MEMORY_BUFFER_LOCATION bpmbl;
    NVME_CONTROLLER_MEMORY_BUFFER_MEMORY_SPCAE_CONTROL cmbmsc;
    NVME_CONTROLLER_MEMORY_BUFFER_STATUS cmbsts;
    NVME_CONTROLLER_MEMORY_BUFFER_ELASTICITY_BUFFER_SIZE cmbebs;
    NVME_CONTROLLER_MEMORY_BUFFER_SUSTAINED_WRITE_THROUGHPUT cmbswtp;
    NVME_NVM_SUBSYSTEM_SHUTDOWN nssd;
    NVME_CONTROLLER_READY_TIMEOUTS crto;
    uint8_t res1[0xd94];
    NVME_PERSISTENT_MEMORY_REGION_CAPABILITIES pmrcap;
    NVME_PERSISTENT_MEMORY_REGION_CONTROL pmrctl;
    NVME_PERSISTENT_MEMORY_REGION_STATUS pmrsts;
    NVME_PERSISTENT_MEMORY_REGION_ELASTICITY_BUFFER_SIZE pmrebs;
    NVME_PERSISTENT_MEMORY_REGION_SUSTAINED_WRITE_THROUGHPUT pmrswtp;
    NVME_PERSISTENT_MEMORY_REGION_MEMORY_SPACE_CONTROL_LOWER pmrmscl;
    NVME_PERSISTENT_MEMORY_REGION_MEMORY_SPACE_CONTROL_UPPER pmrmscu;
    uint8_t res2[0x1e4];
}__attribute__((packed)) NVME_CONTROLLER_REGISTERS;


#endif