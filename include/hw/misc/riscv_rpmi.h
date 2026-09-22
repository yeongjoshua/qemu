 /*
  * riscv_rpmi.h
  * RPMI message IO handling header file
  *
  * Copyright (c) 2024 Ventana Micro Systems Inc.
  *
  * Authors:
  * Subrahmanya Lingappa <slingappa@ventanamicro.com>
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.

  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.

  * You should have received a copy of the GNU General Public License along
  * with this program; if not, see <http://www.gnu.org/licenses/>.
  */

#ifndef HW_riscv_rpmi_H
#define HW_riscv_rpmi_H

#include "hw/core/sysbus.h"
#include "qom/object.h"

#define TYPE_RISCV_RPMI             "riscv.riscv.rpmi"
#define TYPE_RISCV_RPMI_PERF        "riscv.riscv.rpmi.perf"
#define TYPE_RISCV_RPMI_PERF_SHMEM  "riscv.riscv.rpmi.perf.shmem"

#define RISCV_RISCV_RPMI(obj) \
    OBJECT_CHECK(RiscvRpmiState, (obj), TYPE_RISCV_RPMI)
typedef struct RiscvRpmiState RiscvRpmiState;
DECLARE_INSTANCE_CHECKER(RiscvRpmiState, RISCV_RPMI,
                         TYPE_RISCV_RPMI)

#define RISCV_RISCV_RPMI_PERF(obj) \
    OBJECT_CHECK(RiscvRpmiPerfState, (obj), TYPE_RISCV_RPMI_PERF)
typedef struct RiscvRpmiPerfState RiscvRpmiPerfState;
DECLARE_INSTANCE_CHECKER(RiscvRpmiPerfState, RISCV_RPMI_PERF,
                         TYPE_RISCV_RPMI_PERF)

#define __UNUSED__     __attribute__ ((unused))

#define MAX_HARTS 64
#define MAX_XPORTS2 16
#define RPMI_ALL_NUM_QUEUES (4)
#define RPMI_A2P_NUM_QUEUES (2)

#define RPMI_QUEUE_SLOT_SIZE 64
#define RPMI_DBREG_SIZE (0x1000)
#define RPMI_ALL_NUM_REGS (RPMI_ALL_NUM_QUEUES + 1)
#define RPMI_A2P_NUM_REGS (RPMI_A2P_NUM_QUEUES + 1)


struct RiscvRpmiPerfState {
    SysBusDevice parent_obj;
    MemoryRegion mmio;
    MemoryRegion ram;
    uint8_t *ram_ptr;
};

struct RiscvRpmiState {
    /*< private >*/
    SysBusDevice parent_obj;

    /*< public >*/
    MemoryRegion mmio;
    uint32_t doorbell;

    uint64_t harts_mask;
    uint32_t flags;
};

 enum {
     RISCV_RPMI_MAX_HARTS             = 4095,
 };

DeviceState *riscv_rpmi_create(hwaddr db_addr, hwaddr shm_addr, int shm_sz,
                               uint32_t a2preq_qsz, uint32_t p2areq_qsz,
                               uint64_t harts_mask, uint32_t flags,
                               MachineState *ms);

void handle_rpmi_event(void);

DeviceState *rpmi_perf_init(hwaddr base_shmem, hwaddr base_db, void *data);

/**
 * Describes one emulated RPMI performance domain to the device tree builder.
 *
 * @level_count is how many levels the domain advertises, and @min_khz and
 * @max_khz are the lowest and the highest clock frequency among them.
 * @set_level says whether a supervisor is allowed to change the level.
 */
struct riscv_rpmi_perf_domain_info {
    const char *name;
    uint32_t level_count;
    uint32_t min_khz;
    uint32_t max_khz;
    bool set_level;
};

typedef struct riscv_rpmi_perf_domain_info RISCVRPMIPerfDomainInfo;

uint32_t riscv_rpmi_perf_domain_count(void);
bool riscv_rpmi_perf_domain_info(uint32_t id, RISCVRPMIPerfDomainInfo *info);

#endif
