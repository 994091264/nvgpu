/*
 * Copyright (c) 2017-2019, NVIDIA CORPORATION.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <nvgpu/enabled.h>
#include <nvgpu/gk20a.h>
#include <nvgpu/vgpu/vgpu.h>

#include "vgpu_gv11b.h"
#include "common/vgpu/init/init_vgpu.h"

void vgpu_gv11b_init_gpu_characteristics(struct gk20a *g)
{
	nvgpu_log_fn(g, " ");

	vgpu_init_gpu_characteristics(g);

	nvgpu_set_enabled(g, NVGPU_SUPPORT_TSG_SUBCONTEXTS, true);
	nvgpu_set_enabled(g, NVGPU_USE_COHERENT_SYSMEM, true);
	nvgpu_set_enabled(g, NVGPU_SUPPORT_IO_COHERENCE, true);
	nvgpu_set_enabled(g, NVGPU_SUPPORT_SCG, true);
	nvgpu_set_enabled(g, NVGPU_SUPPORT_SYNCPOINT_ADDRESS, true);
	nvgpu_set_enabled(g, NVGPU_SUPPORT_USER_SYNCPOINT, true);
	nvgpu_set_enabled(g, NVGPU_SUPPORT_USERMODE_SUBMIT, true);
#ifdef CONFIG_NVGPU_GRAPHICS
	nvgpu_set_enabled(g, NVGPU_SUPPORT_ZBC_STENCIL, true);
#endif
	nvgpu_set_enabled(g, NVGPU_SUPPORT_PREEMPTION_GFXP, true);
	nvgpu_set_enabled(g, NVGPU_SUPPORT_PLATFORM_ATOMIC, true);
	nvgpu_set_enabled(g, NVGPU_SUPPORT_SET_CTX_MMU_DEBUG_MODE, false);
}