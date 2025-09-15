#include "nvme.h"


__attribute__((aligned(0x1000))) uint8_t asq[NVME_ADMIN_QUEUE_ENTRIES * NVME_SQE_SIZE] = {0, };
__attribute__((aligned(0x1000))) uint8_t acq[NVME_ADMIN_QUEUE_ENTRIES * NVME_CQE_SIZE] = {0, };