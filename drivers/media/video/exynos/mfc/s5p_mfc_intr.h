/*
 * drivers/media/video/exynos/mfc/s5p_mfc_intr.h
 *
 * Header file for Samsung MFC (Multi Function Codec - FIMV) driver
 * It contains waiting functions declarations.
 *
 * Kamil Debski, Copyright (c) 2010 Samsung Electronics
 * http://www.samsung.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _S5P_MFC_INTR_H_
#define _S5P_MFC_INTR_H_

#include "s5p_mfc_common.h"

int s5p_mfc_wait_for_done_ctx(struct s5p_mfc_ctx *ctx, int command);
int s5p_mfc_wait_for_done_dev(struct s5p_mfc_dev *dev, int command);
void s5p_mfc_clean_ctx_int_flags(struct s5p_mfc_ctx *ctx);
void s5p_mfc_clean_dev_int_flags(struct s5p_mfc_dev *dev);

#endif /* _S5P_MFC_INTR_H_ */
