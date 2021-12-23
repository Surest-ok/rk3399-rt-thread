/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2021-10-16     gong.lee       first version
 */

#ifndef DRV_TIMER_H__
#define DRV_TIMER_H__

#include "rk3399.h"

#define RK_TIMER_LOAD_COUNT0(BASE)          HWREG32(BASE + 0x0000)
#define RK_TIMER_LOAD_COUNT1(BASE)          HWREG32(BASE + 0x0004)
#define RK_TIMER_CUR_VALUE0(BASE)           HWREG32(BASE + 0x0008)
#define RK_TIMER_CUR_VALUE1(BASE)           HWREG32(BASE + 0x000C)
#define RK_TIMER_LOAD_COUNT2(BASE)          HWREG32(BASE + 0x0010)
#define RK_TIMER_LOAD_COUNT3(BASE)          HWREG32(BASE + 0x0014)
#define RK_TIMER_INIT_STATUS(BASE)          HWREG32(BASE + 0x0018)
#define RK_TIMER_CTRL_REG(BASE)             HWREG32(BASE + 0x001c)

#endif /* DRV_TIMER_H__ */
