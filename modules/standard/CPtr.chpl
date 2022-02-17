/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*

In previous releases, this module contained Chapel types that
represent C pointers and related routines.  It has been deprecated and
its contents have been moved to :mod:`CTypes`.

.. warning::

    This module has been deprecated - please use :mod:`CTypes` instead.
*/

deprecated "'CPtr' is deprecated; please use the 'CTypes' module instead"
module CPtr {
  public use CTypes only c_void_ptr, c_nil, is_c_nil, c_ptr, c_array,
                         c_pointer_return, c_pointer_diff, c_ptrTo,
                         c_sizeof, c_offsetof, c_calloc, c_malloc,
                         c_aligned_alloc, c_free, isAnyCPtr, c_memmove,
                         c_memcpy, c_memcmp, c_memset, =, :, ==, !=;
}
