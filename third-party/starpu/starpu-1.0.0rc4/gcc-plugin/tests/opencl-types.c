/* GCC-StarPU
   Copyright (C) 2012 Institut National de Recherche en Informatique et Automatique

   GCC-StarPU is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   GCC-StarPU is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC-StarPU.  If not, see <http://www.gnu.org/licenses/>.  */

/* Make sure use of `size_t' as a task argument type is flagged.  */

/* (instructions compile) */

#undef NDEBUG

/* Please gimme a broken `cl_long'!  */
#define BREAK_CL_LONG

#include <mocks.h>
#include <unistd.h>
#include <sys/types.h>				  /* for `uint' & co. */


/* Make sure `size_t' is flagged.  */

static void my_task (size_t size, int x[size])
  __attribute__ ((task));

static void my_task_cpu (size_t size, int x[size])
  __attribute__ ((task_implementation ("cpu", my_task)));
static void my_task_opencl (size_t size, int x[size]) /* (warning "size_t.*not.*known OpenCL type") */
  __attribute__ ((task_implementation ("opencl", my_task)));

static void
my_task_cpu (size_t size, int x[size])
{
}

static void
my_task_opencl (size_t size, int x[size])
{
}


/* Make sure types that have the same name in C and OpenCL but are actually
   different are flagged.  We assume `sizeof (long) == 4' here.  */

static void my_long_task (unsigned long size, int x[size])
  __attribute__ ((task));

static void my_long_task_cpu (unsigned long size, int x[size])
  __attribute__ ((task_implementation ("cpu", my_long_task)));
static void my_long_task_opencl (unsigned long size,  /* (warning "differs from the same-named OpenCL type") */
				 int x[size])
  __attribute__ ((task_implementation ("opencl", my_long_task)));

static void
my_long_task_cpu (unsigned long size, int x[size])
{
}

static void
my_long_task_opencl (unsigned long size, int x[size])
{
}


/* Same with a pointer-to-long.  */

static void my_long_ptr_task (unsigned long *p)
  __attribute__ ((task));

static void my_long_ptr_task_cpu (unsigned long *p)
  __attribute__ ((task_implementation ("cpu", my_long_ptr_task)));
static void my_long_ptr_task_opencl (unsigned long *p) /* (warning "differs from the same-named OpenCL type") */
  __attribute__ ((task_implementation ("opencl", my_long_ptr_task)));

static void
my_long_ptr_task_cpu (unsigned long *p)
{
}

static void
my_long_ptr_task_opencl (unsigned long *p)
{
}


/* Same with an array of unsigned chars.  */

static void my_uchar_task (char c[])
  __attribute__ ((task));

static void my_uchar_task_cpu (char c[])
  __attribute__ ((task_implementation ("cpu", my_uchar_task)));
static void my_uchar_task_opencl (char c[]) /* (warning "differs in signedness from the same-named OpenCL type") */
  __attribute__ ((task_implementation ("opencl", my_uchar_task)));

static void
my_uchar_task_cpu (char c[])
{
}

static void
my_uchar_task_opencl (char c[])
{
}


/* "unsigned int" is aka. "uint".  */

static void my_uint_task (const uint *c)
  __attribute__ ((task));
static void my_uint_task_cpu (const uint *c)
  __attribute__ ((task_implementation ("cpu", my_uint_task)));
static void my_uint_task_opencl (const uint *c)	  /* no warning */
  __attribute__ ((task_implementation ("opencl", my_uint_task)));

static void
my_uint_task_cpu (const uint *c)
{
}

static void
my_uint_task_opencl (const uint *c)
{
}


/* "unsigned char" is aka. "uchar".  */

typedef float uchar;				  /* not a real `uchar'! */

static void my_fake_uchar_task (const uchar *c)
  __attribute__ ((task));
static void my_fake_uchar_task_cpu (const uchar *c)
  __attribute__ ((task_implementation ("cpu", my_fake_uchar_task)));
static void my_fake_uchar_task_opencl (const uchar *c) /* (warning "differs from the same-named OpenCL type") */
  __attribute__ ((task_implementation ("opencl", my_fake_uchar_task)));

static void
my_fake_uchar_task_cpu (const uchar *c)
{
}

static void
my_fake_uchar_task_opencl (const uchar *c)
{
}



/* No OpenCL, no problems.  */

static void my_cool_task (size_t size, long long x[size])
  __attribute__ ((task));

static void my_cool_task_cpu (size_t size, long long x[size])
  __attribute__ ((task_implementation ("cpu", my_cool_task)));

static void
my_cool_task_cpu (size_t size, long long x[size])
{
}
