#ifndef _DEV_NVME_H
#define _DEV_NVME_H

#include "../../sys/stdint.h"
#include "../../sys/stddef.h"
#include "../../sys/string.h"
#include "../../inc/asm.h"
#include "../../boot/kernel/paging.h"
#include "../../boot/kernel/print.h"

#define NVME_ADMIN_QUEUE_ENTRIES    32
#define NVME_SQE_SIZE               64
#define NVME_CQE_SIZE               16
#define NVME_SQE_SIZE_LOG           6
#define NVME_CQE_SIZE_LOG           4

#define NVME_CAP_OFFSET             0x00
#define NVME_VS_OFFSET              0x08
#define NVME_INTMS_OFFSET           0x0c
#define NVME_INTMC_OFFSET           0x10
#define NVME_CC_OFFSET              0x14
#define NVME_CSTS_OFFSET            0x1c
#define NVME_NSSR_OFFSET            0x20
#define NVME_AQA_OFFSET             0x24
#define NVME_ASQ_OFFSET             0x28
#define NVME_ACQ_OFFSET             0x30


// I'll leave these structures for now.
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
typedef struct {
    uint16_t opc : 8;
    uint16_t fuse : 2;
    uint16_t res : 4;
    uint16_t psdt : 2;
    uint16_t cid : 16;
}__attribute__((packed)) NVME_COMMAND_COMMON_DWORD_0;
typedef struct {
    NVME_COMMAND_COMMON_DWORD_0 cdw0;
    uint32_t nsid;
    uint32_t cdw2;
    uint32_t cdw3;
    uint64_t mptr;
    uint64_t prp1;
    uint64_t prp2;
    uint32_t cdw10;
    uint32_t cdw11;
    uint32_t cdw12;
    uint32_t cdw13;
    uint32_t cdw14;
    uint32_t cdw15;
}__attribute__((packed)) NVME_COMMAND_COMMON;
typedef struct {
    uint8_t cns;
    uint8_t res;
    uint16_t cntid;
}__attribute__((packed)) NVME_COMMAND_IDENTIFY_DWORD_10;
typedef struct {
    uint16_t cnssid;
    uint8_t res;
    uint8_t csi;
}__attribute__((packed)) NVME_COMMAND_IDENTIFY_DWORD_11;
typedef struct {
    NVME_COMMAND_COMMON_DWORD_0 cdw0;
    uint32_t nsid;
    uint32_t cdw2;
    uint32_t cdw3;
    uint64_t mptr;
    uint64_t prp1;
    uint64_t prp2;
    NVME_COMMAND_IDENTIFY_DWORD_10 cdw10;
    NVME_COMMAND_IDENTIFY_DWORD_11 cdw11;
    uint32_t cdw12;
    uint32_t cdw13;
    uint32_t cdw14;
    uint32_t cdw15;
}__attribute__((packed)) NVME_COMMAND_IDENTIFY;
typedef struct {
    uint16_t sqhd;
    uint16_t sqid;
}__attribute__((packed)) NVME_CQE_COMMON_DWORD_2;
typedef struct {
    uint16_t cid : 16;
    uint16_t p : 1;
    uint16_t sc : 8;
    uint16_t sct : 3;
    uint16_t crd : 2;
    uint16_t m : 1;
    uint16_t dnr : 1;
}__attribute__((packed)) NVME_CQE_COMMON_DWORD_3;
typedef struct {
    uint32_t cdw0;
    uint32_t cdw1;
    NVME_CQE_COMMON_DWORD_2 cdw2;
    NVME_CQE_COMMON_DWORD_3 cdw3;
}__attribute__((packed)) NVME_CQE_COMMON;
typedef struct {
    uint16_t vid;
    uint16_t ssvid;
    uint8_t sn[20];
    uint8_t mn[40];
    uint8_t fr[8];
    uint8_t rab;
    uint8_t ieee[3];
    uint8_t cmic;
    uint8_t mdts;
    uint16_t cntlid;
    uint32_t ver;
    uint32_t rtd3r;
    uint32_t rtd3e;
    uint32_t oaes;
    uint32_t ctratt;
    uint16_t rrls;
    uint8_t bpcap;
    uint8_t res0;
    uint32_t nssl;
    uint16_t res1;
    uint8_t plsi;
    uint8_t cntrltype;
    uint8_t fguid[16];
    uint16_t crdt1;
    uint16_t crdt2;
    uint16_t crdt3;
    uint8_t crcap;
    uint8_t ciu;
    uint64_t cirn;
    uint8_t res2[96];
    uint8_t res3[13];
    uint8_t nvmsr;
    uint8_t vwci;
    uint8_t mec;
    uint16_t oacs;
    uint8_t acl;
    uint8_t aerl;
    uint8_t frmw;
    uint8_t lpa;
    uint8_t elpe;
    uint8_t npss;
    uint8_t avscc;
    uint8_t apsta;
    uint16_t wctemp;
    uint16_t cctemp;
    uint16_t mtfa;
    uint32_t hmpre;
    uint32_t hmmin;
    uint8_t tnvmcap[16];
    uint8_t unvmcap[16];
    uint32_t rpmbs;
    uint16_t edstt;
    uint8_t dsto;
    uint8_t fwug;
    uint16_t kas;
    uint16_t hctma;
    uint16_t mntmt;
    uint16_t mxtmt;
    uint32_t sanicap;
    uint32_t hmminds;
    uint16_t hmmaxd;
    uint16_t nsetidmax;
    uint16_t endgidmax;
    uint8_t anatt;
    uint8_t anacap;
    uint32_t anagrpmax;
    uint32_t nanagrpid;
    uint32_t pels;
    uint16_t did;
    uint8_t kpioc;
    uint8_t res4;
    uint16_t mptfawr;
    uint8_t res5[6];
    uint8_t megcap[16];
    uint8_t tmpthha;
    uint8_t mupa;
    uint16_t cqt;
    uint16_t cdpa;
    uint16_t mup;
    uint16_t ipmsr;
    uint16_t msmt;
    uint8_t res6[116];
    uint8_t sqes;
    uint8_t cqes;
    uint16_t maxcmd;
    uint32_t nn;
    uint16_t oncs;
    uint16_t fuses;
    uint8_t fna;
    uint8_t vwc;
    uint16_t awun;
    uint16_t awupf;
    uint8_t icsvscc;
    uint8_t nwpc;
    uint16_t acwu;
    uint16_t cdfs;
    uint32_t sgls;
    uint32_t mnan;
    uint8_t maxdna[16];
    uint32_t maxcna;
    uint32_t oaqd;
    uint8_t rhiri;
    uint8_t hirt;
    uint16_t cmmrtd;
    uint16_t nmmrtd;
    uint8_t minmrtg;
    uint8_t maxmrtg;
    uint8_t trattr;
    uint8_t res7;
    uint16_t mcudmq;
    uint16_t mnsudmq;
    uint16_t mcmr;
    uint16_t nmcmr;
    uint16_t mcdqpc;
    uint8_t res8[180];
    uint8_t subnqn[256];
    uint8_t res9[768];
    uint32_t ioccsz;
    uint32_t iorcsz;
    uint16_t icdoff;
    uint8_t fcatt;
    uint8_t msdbd;
    uint16_t ofcs;
    uint8_t dctype;
    uint8_t ccrl;
    uint8_t res10[240];
    uint8_t psd[32][32];
    uint8_t vs[1024];
}__attribute__((packed)) NVME_IDENTIFY_CONTROLLER_DATA_STRUCTURE;


void nvme_mmio_write_32(uint64_t addr, size_t offs1, size_t offs2, uint32_t value, size_t size);
uint32_t nvme_mmio_read_32(uint64_t addr, size_t offs1, size_t offs2, size_t size);
void nvme_mmio_write_64(uint64_t addr, size_t offs1, size_t offs2, uint64_t value, size_t size);
uint64_t nvme_mmio_read_64(uint64_t addr, size_t offs1, size_t offs2, size_t size);
void nvme_ring_asq_doorbell(uint64_t addr, uint32_t qid);
void nvme_ring_acq_doorbell(uint64_t addr, uint32_t qid);
void nvme_command_identify(uint64_t addr);


#endif