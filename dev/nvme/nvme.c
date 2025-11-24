#include "nvme.h"


__attribute__((aligned(0x1000))) NVME_COMMAND_COMMON asq[NVME_ADMIN_QUEUE_ENTRIES] = {0, };
__attribute__((aligned(0x1000))) NVME_CQE_COMMON acq[NVME_ADMIN_QUEUE_ENTRIES] = {0, };
__attribute__((aligned(0x1000))) NVME_IDENTIFY_CONTROLLER_DATA_STRUCTURE identify_buf = {0, };
static uint32_t asq_tail = 0;
static uint32_t acq_head = 0;
static uint8_t acq_phase = 1;
static uint16_t exp_cid = 0x1000;

void nvme_mmio_write_32(uint64_t addr, size_t offs1, size_t offs2, uint32_t value, size_t size) {
    volatile uint32_t tmp = *(uint32_t*)(addr + offs1);

    tmp = (tmp & ~((uint32_t)(0xFFFFFFFF >> (32 - size)) << offs2)) | (value << offs2);
    *(uint32_t*)(addr + offs1) = tmp;
}
uint32_t nvme_mmio_read_32(uint64_t addr, size_t offs1, size_t offs2, size_t size) {
    volatile uint32_t tmp = *(uint32_t*)(addr + offs1);

    tmp = (tmp >> offs2) & (0xFFFFFFFF >> (32 - size));

    return tmp;
}
void nvme_mmio_write_64(uint64_t addr, size_t offs1, size_t offs2, uint64_t value, size_t size) {
    volatile uint32_t low = *(uint32_t*)(addr + offs1);
    volatile uint32_t high = *(uint32_t*)(addr + offs1 + 4);
    volatile uint64_t tmp = ((uint64_t)high << 32) | low;

    tmp = (tmp & ~((uint64_t)(0xFFFFFFFFFFFFFFFF >> (64 - size)) << offs2)) | (value << offs2);
    low = tmp & 0xFFFFFFFF;
    high = tmp >> 32;
    *(uint32_t*)(addr + offs1) = low;
    *(uint32_t*)(addr + offs1 + 4) = high;
}
uint64_t nvme_mmio_read_64(uint64_t addr, size_t offs1, size_t offs2, size_t size) {
    volatile uint32_t low = *(uint32_t*)(addr + offs1);
    volatile uint32_t high = *(uint32_t*)(addr + offs1 + 4);
    volatile uint64_t tmp = ((uint64_t)high << 32) | low;

    tmp = (tmp >> offs2) & (0xFFFFFFFFFFFFFFFF >> (64 - size));

    return tmp;
}
void nvme_ring_asq_doorbell(uint64_t addr, uint32_t qid) {
    uint8_t dstrd = nvme_mmio_read_64(addr, NVME_CAP_OFFSET, 32, 4);
    uint64_t sq_doorbell_addr = addr + 0x1000 + ((2 * qid + 0) * (4 << dstrd));
    uint32_t new_tail = (asq_tail + 1) % NVME_ADMIN_QUEUE_ENTRIES;

    __asm__ volatile ("mfence":::"memory");

    nvme_mmio_write_32(sq_doorbell_addr, 0, 0, new_tail, 32);

    asq_tail = new_tail;
}
void nvme_ring_acq_doorbell(uint64_t addr, uint32_t qid) {
    uint8_t dstrd = nvme_mmio_read_64(addr, NVME_CAP_OFFSET, 32, 4);
    uint64_t cq_doorbell_addr = addr + 0x1000 + ((2 * qid + 1) * (4 << dstrd));
    uint32_t new_head = (acq_head + 1) % NVME_ADMIN_QUEUE_ENTRIES;

    __asm__ volatile ("mfence":::"memory");

    nvme_mmio_write_32(cq_doorbell_addr, 0, 0, new_head, 32);

    acq_head = new_head;
}
void nvme_command_identify(uint64_t addr) {
    NVME_COMMAND_IDENTIFY cmd = {0, };
    cmd.cdw0.opc = 0x06;
    cmd.cdw0.cid = exp_cid;
    cmd.cdw10.cns = 1;
    cmd.nsid = 0;
    cmd.cdw10.cntid = 0;
    cmd.cdw11.csi = 0;

    cmd.prp1 = vir_to_phys((uint64_t)&identify_buf);
    cmd.prp2 = 0;

    cmd.cdw11.cnssid = 0;

    ((NVME_COMMAND_IDENTIFY*)asq)[asq_tail] = cmd;

    __asm__ volatile ("mfence":::"memory");

    nvme_ring_asq_doorbell(addr, 0);

    for (size_t i = 0; i < 0xffffff; i++);

    NVME_CQE_COMMON* cqe = &acq[acq_head];
    int timeout = 0xffffff;
    while (1) {
        if (cqe->cdw3.p == acq_phase) {
            break;
        }

        timeout--;
        if (timeout == 0) {
            timeout = 0xffffff;
            // kprintf("Timeout reached.\n");
            // acq_head++;
            kprintf("%d %d\r", cqe->cdw3.sc, cqe->cdw3.sct);
        }
    }

    nvme_ring_acq_doorbell(addr, 0);

    for (size_t i = 0; i < 0xffffff; i++);

    if (acq_head == 0) {
        acq_phase = !acq_phase;
        exp_cid++;
    }
}