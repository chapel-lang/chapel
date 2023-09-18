/*
 * Copyright (C) 2021 by Cornelis Networks.
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _FI_PROV_OPX_SYSFS_H_
#define _FI_PROV_OPX_SYSFS_H_

const char *opx_sysfs_path(void);
size_t opx_sysfs_path_len(void);
int opx_sysfs_stat(const char *attr,struct stat *sbuf);
int opx_sysfs_open(const char *attr, int flags);
int opx_sysfs_printf(const char *attr, const char *fmt, ...);
int opx_sysfs_unit_open(uint32_t unit, const char *attr, int flags);
int opx_sysfs_port_open(uint32_t unit, uint32_t port, const char *attr, int flags);
int opx_sysfs_port_printf(uint32_t unit, uint32_t port, const char *attr, const char *fmt, ...);
int opx_sysfs_unit_printf(uint32_t unit, const char *attr, const char *fmt, ...);
int opx_sysfs_read(const char *attr, char **datap);
int opx_sysfs_unit_read(uint32_t unit, const char *attr, char **datap);
int opx_sysfs_port_read(uint32_t unit, uint32_t port, const char *attr, char **datap);
int opx_sysfs_unit_write(uint32_t unit, const char *attr, const void *data, size_t len);
int opx_sysfs_read_s64(const char *attr, int64_t *valp, int base);
int opx_sysfs_unit_read_s64(uint32_t unit, const char *attr, int64_t *valp, int base);
int64_t opx_hfi_sysfs_unit_read_node_s64(uint32_t unit);
int opx_sysfs_port_read_s64(uint32_t unit, uint32_t port, const char *attr, int64_t *valp, int base);
#endif /* _FI_PROV_OPX_SYSFS_H__ */
