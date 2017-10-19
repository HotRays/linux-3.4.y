/* linux/i2c/tps6507x-ts.h
 *
 * Functions to access TPS65070 touch screen chip.
 *
 * Copyright (c) 2009 RidgeRun (todd.fischer@ridgerun.com)
 *
 *
 *  For licencing details see kernel-base/COPYING
 */

#ifndef __LINUX_I2C_GT9XX_TS_H
#define __LINUX_I2C_GT9XX_TS_H

/* Board specific touch screen initial values */
struct gt9xx_platform_data {
        int     rst;    /* gpio reset */
        int     irq;           /* gpio int */
	void     (*reset)(int addr);
};

#endif /*  __LINUX_I2C_TPS6507X_TS_H */
