/*
* Copyright (c) 2016 Intel Corporation.  All rights reserved.
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

#pragma once

#include <stdint.h>
#include "ofi_osd.h"

/* Unix specific macro */
#define MAP_SHARED	0x01		/* Share changes.  */
#define MAP_PRIVATE	0x02		/* Changes are private.  */
#define MAP_FAILED	((void*)-1)	/* Return value of `mmap' in case of an error.  */

#define PROT_READ	0x1	/* Page can be read.  */
#define PROT_WRITE	0x2	/* Page can be written.  */
#define PROT_EXEC	0x4	/* Page can be executed.  */
#define PROT_NONE	0x0	/* Page can not be accessed.  */

/* Unix specific macro */
#define S_IRUSR S_IREAD
#define S_IWUSR S_IWRITE

typedef uint32_t mode_t;

/* stubs for Linux only functions */
static inline void *mremap(void *old_address, size_t old_size, size_t new_size, int flags)
{
	OFI_UNUSED(old_address);
	OFI_UNUSED(old_size);
	OFI_UNUSED(new_size);
	OFI_UNUSED(flags);

	return MAP_FAILED;
}

static inline int shm_open(const char *name, int oflag, mode_t mode)
{
	OFI_UNUSED(name);
	OFI_UNUSED(oflag);
	OFI_UNUSED(mode);

	return -1;
}

static inline int shm_unlink(const char *name)
{
	OFI_UNUSED(name);
	return -1;
}

static inline void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
	OFI_UNUSED(addr);
	OFI_UNUSED(length);
	OFI_UNUSED(prot);
	OFI_UNUSED(flags);
	OFI_UNUSED(fd);
	OFI_UNUSED(offset);

	return MAP_FAILED;
}

static inline int munmap(void *addr, size_t length)
{
	OFI_UNUSED(addr);
	OFI_UNUSED(length);

	return -1;
}

static inline int ftruncate(int fd, off_t length)
{
	OFI_UNUSED(fd);
	OFI_UNUSED(length);

	return -1;
}


