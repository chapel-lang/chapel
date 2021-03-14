/*
 * Copyright 2008-2018 Cisco Systems, Inc.  All rights reserved.
 * Copyright 2007 Nuova Systems, Inc.  All rights reserved.
 *
 * LICENSE_BEGIN
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * LICENSE_END
 *
 *
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/delay.h>

#include "kcompat.h"
#include "vnic_dev.h"
#include "vnic_intr.h"

EXPORT_SYMBOL(vnic_intr_free);
void vnic_intr_free(struct vnic_intr *intr)
{
	intr->ctrl = NULL;
}

EXPORT_SYMBOL(vnic_intr_alloc);
int vnic_intr_alloc(struct vnic_dev *vdev, struct vnic_intr *intr,
	unsigned int index)
{
	intr->index = index;
	intr->vdev = vdev;

	intr->ctrl = vnic_dev_get_res(vdev, RES_TYPE_INTR_CTRL, index);
	if (!intr->ctrl) {
		pr_err("Failed to hook INTR[%d].ctrl resource\n", index);
		return -EINVAL;
	}

	return 0;
}

EXPORT_SYMBOL(vnic_intr_init);
void vnic_intr_init(struct vnic_intr *intr, u32 coalescing_timer,
	unsigned int coalescing_type, unsigned int mask_on_assertion)
{
	vnic_intr_coalescing_timer_set(intr, coalescing_timer);
	iowrite32(coalescing_type, &intr->ctrl->coalescing_type);
	iowrite32(mask_on_assertion, &intr->ctrl->mask_on_assertion);
	iowrite32(0, &intr->ctrl->int_credits);
}

void vnic_intr_coalescing_timer_set(struct vnic_intr *intr,
	u32 coalescing_timer)
{
	iowrite32(vnic_dev_intr_coal_timer_usec_to_hw(intr->vdev,
		coalescing_timer), &intr->ctrl->coalescing_timer);
}

void vnic_intr_clean(struct vnic_intr *intr)
{
	iowrite32(0, &intr->ctrl->int_credits);
}

void vnic_intr_raise(struct vnic_intr *intr)
{
	vnic_dev_raise_intr(intr->vdev, (u16)intr->index);
}

EXPORT_SYMBOL(vnic_grpmbrintr_free);
void vnic_grpmbrintr_free(struct vnic_intr *intr)
{
	intr->ctrl = NULL;
}

EXPORT_SYMBOL(vnic_grpmbrintr_alloc);
int vnic_grpmbrintr_alloc(struct vnic_dev *vdev, struct vnic_intr *intr,
	unsigned int index)
{
	intr->index = index;
	intr->vdev = vdev;

	intr->ctrl = vnic_dev_get_res(vdev, RES_TYPE_GRPMBR_INTR, index);
	if (!intr->ctrl) {
		pr_err("Failed to hook INTR[%d].ctrl resource\n", index);
		return -EINVAL;
	}

	return 0;
}
