/* GCC-StarPU
   Copyright (C) 2011, 2012 Institut National de Recherche en Informatique et Automatique

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

/* Testing library, including stubs of StarPU functions.  */

#ifndef STARPU_GCC_PLUGIN
# error barf!
#endif

#ifndef STARPU_USE_CPU
# error damn it!
#endif

#undef NDEBUG

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <common/uthash.h>


/* Stub used for testing purposes.  */

/* Number of tasks submitted.  */
static unsigned int tasks_submitted;

struct insert_task_argument
{
  /* `STARPU_VALUE', etc. */
  int type;

  /* Pointer to the expected value.  */
  const void *pointer;

  /* Size in bytes of the data pointed to.  */
  size_t size;
};

/* Pointer to a zero-terminated array listing the expected
   `starpu_insert_task' arguments.  */
const struct insert_task_argument *expected_insert_task_arguments;

int
starpu_insert_task (struct starpu_codelet *cl, ...)
{
  assert (cl->name != NULL && strlen (cl->name) > 0);
  assert (cl->where == (STARPU_CPU | STARPU_OPENCL));

  /* TODO: Call `cpu_func' & co. and check whether they do the right
     thing.  */

  assert (cl->cpu_funcs[0] != NULL);
  assert (cl->opencl_funcs[0] != NULL);
  assert (cl->cuda_funcs[0] == NULL);

  va_list args;
  size_t pointer_arg;

  va_start (args, cl);

  const struct insert_task_argument *expected;
  for (expected = expected_insert_task_arguments, pointer_arg = 0;
       expected->type != 0;
       expected++)
    {
      int type;

      type = va_arg (args, int);
      assert (type == expected->type);

      switch (type)
	{
	case STARPU_VALUE:
	  {
	    void *arg;
	    size_t size;

	    arg = va_arg (args, void *);
	    size = va_arg (args, size_t);

	    assert (size == expected->size);
	    assert (arg != NULL);
	    assert (!memcmp (arg, expected->pointer, size));
	    break;
	  }

	case STARPU_RW:
	case STARPU_R:
	case STARPU_W:
	  {
	    starpu_data_handle_t handle;
	    handle = starpu_data_lookup (expected->pointer);
	    assert (type == cl->modes[pointer_arg++]);
	    assert (va_arg (args, void *) == handle);
	    break;
	  }

	default:
	  abort ();
	}
    }

  va_end (args);

  /* Make sure all the arguments were consumed.  */
  assert (expected->type == 0);

  tasks_submitted++;

  return 0;
}

/* Our own implementation of `starpu_codelet_unpack_args', for debugging
   purposes.  */

void
starpu_codelet_unpack_args (void *cl_raw_arg, ...)
{
  va_list args;
  size_t nargs, arg, offset, size;
  unsigned char *cl_arg;

  cl_arg = (unsigned char *) cl_raw_arg;

  nargs = *cl_arg;

  va_start (args, cl_raw_arg);

  for (arg = 0, offset = 1, size = 0;
       arg < nargs;
       arg++, offset += sizeof (size_t) + size)
    {
      void *argp;

      argp = va_arg (args, void *);
      size = *(size_t *) &cl_arg[size];

      memcpy (argp, &cl_arg[offset], size);
    }

  va_end (args);
}


/* Data handles.  A hash table mapping pointers to handles is maintained,
   which allows us to mimic the actual behavior of libstarpu.  */

/* Entry in the `registered_handles' hash table.  `starpu_data_handle_t' is
   assumed to be a pointer to this structure.  */
struct handle_entry
{
  UT_hash_handle hh;
  void *pointer;
  starpu_data_handle_t handle;
};

#define handle_to_entry(h) ((struct handle_entry *) (h))
#define handle_to_pointer(h)				\
  ({							\
    assert ((h) != NULL);				\
    assert (handle_to_entry (h)->handle == (h));	\
    handle_to_entry (h)->pointer;			\
   })

static struct handle_entry *registered_handles;

starpu_data_handle_t
starpu_data_lookup (const void *ptr)
{
  starpu_data_handle_t result;

  struct handle_entry *entry;

  HASH_FIND_PTR (registered_handles, &ptr, entry);
  if (STARPU_UNLIKELY (entry == NULL))
    result = NULL;
  else
    result = entry->handle;

  return result;
}

void *
starpu_handle_get_local_ptr (starpu_data_handle_t handle)
{
  return handle_to_pointer (handle);
}


/* Data registration.  */

struct data_register_arguments
{
  /* A pointer to the vector being registered.  */
  void *pointer;

  /* Number of elements in the vector.  */
  size_t elements;

  /* Size of individual elements.  */
  size_t element_size;
};

/* Number of `starpu_vector_data_register' calls.  */
static unsigned int data_register_calls;

/* Variable describing the expected `starpu_vector_data_register'
   arguments.  */
struct data_register_arguments expected_register_arguments;

void
starpu_vector_data_register (starpu_data_handle_t *handle,
			     uint32_t home_node, uintptr_t ptr,
			     uint32_t count, size_t elemsize)
{
  assert ((void *) ptr == expected_register_arguments.pointer);
  assert (count == expected_register_arguments.elements);
  assert (elemsize == expected_register_arguments.element_size);

  data_register_calls++;

  /* Add PTR to the REGISTERED_HANDLES hash table.  */

  struct handle_entry *entry = malloc (sizeof (*entry));
  assert (entry != NULL);

  entry->pointer = (void *) ptr;
  entry->handle = (starpu_data_handle_t) entry;

  HASH_ADD_PTR(registered_handles, pointer, entry);

  *handle = (starpu_data_handle_t) entry;
}


/* Data acquisition.  */

struct data_acquire_arguments
{
  /* Pointer to the data being acquired.  */
  void *pointer;
};

/* Number of `starpu_data_acquire' calls.  */
static unsigned int data_acquire_calls;

/* Variable describing the expected `starpu_data_acquire' arguments.  */
struct data_acquire_arguments expected_acquire_arguments;

int
starpu_data_acquire (starpu_data_handle_t handle, enum starpu_access_mode mode)
{
  /* XXX: Currently only `STARPU_RW'.  */
  assert (mode == STARPU_RW);

  assert (handle_to_pointer (handle) == expected_acquire_arguments.pointer);
  data_acquire_calls++;

  return 0;
}


/* Data acquisition.  */

struct data_unregister_arguments
{
  /* Pointer to the data being unregistered.  */
  void *pointer;
};

/* Number of `starpu_data_unregister' calls.  */
static unsigned int data_unregister_calls;

/* Variable describing the expected `starpu_data_unregister' arguments.  */
struct data_unregister_arguments expected_unregister_arguments;

void
starpu_data_unregister (starpu_data_handle_t handle)
{
  assert (handle != NULL);

  struct handle_entry *entry = handle_to_entry (handle);

  assert (entry->pointer != NULL);
  assert (entry->pointer == expected_unregister_arguments.pointer);

  /* Remove the PTR -> HANDLE mapping.  If a mapping from PTR to another
     handle existed before (e.g., when using filters), it becomes visible
     again.  */
  HASH_DEL (registered_handles, entry);
  entry->pointer = NULL;
  free (entry);

  data_unregister_calls++;
}


/* Heap allocation.  */

/* Number of `starpu_malloc' and `starpu_free' calls.  */
static unsigned int malloc_calls, free_calls;

static size_t expected_malloc_argument;
static void *expected_free_argument;

int
starpu_malloc (void **ptr, size_t size)
{
  assert (size == expected_malloc_argument);

  *ptr = malloc (size);
  malloc_calls++;

  return 0;
}

int
starpu_free (void *ptr)
{
  assert (starpu_data_lookup (ptr) == NULL);
  assert (ptr == expected_free_argument);
  free_calls++;
  return 0;
}


/* Initialization.  */

static int initialized;

int
starpu_init (struct starpu_conf *config)
{
  initialized++;
  return 0;
}


/* Shutdown.  */

void
starpu_shutdown (void)
{
  initialized--;
}
