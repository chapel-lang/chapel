/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2012  Université de Bordeaux 1
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

#ifndef __IMPLICIT_DATA_DEPS_H__
#define __IMPLICIT_DATA_DEPS_H__

#include <starpu.h>
#include <common/config.h>

struct starpu_task *_starpu_detect_implicit_data_deps_with_handle(struct starpu_task *pre_sync_task, struct starpu_task *post_sync_task,
						   starpu_data_handle_t handle, enum starpu_access_mode mode);
void _starpu_detect_implicit_data_deps(struct starpu_task *task);
void _starpu_release_data_enforce_sequential_consistency(struct starpu_task *task, starpu_data_handle_t handle);

void _starpu_add_post_sync_tasks(struct starpu_task *post_sync_task, starpu_data_handle_t handle);
void _starpu_unlock_post_sync_tasks(starpu_data_handle_t handle);

/* This function blocks until the handle is available in the requested mode */
int _starpu_data_wait_until_available(starpu_data_handle_t handle, enum starpu_access_mode mode);

#endif // __IMPLICIT_DATA_DEPS_H__

