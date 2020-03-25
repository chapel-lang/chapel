/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#ifndef _HDF5_HELPER_H_
#define _HDF5_HELPER_H_

// This file contains workarounds for pointer type differences that
// Chapel can run into for pointer to 64-bit integral types.
// It works around these issues by passing the pointer to these
// functions as a void* and casting to the appropriate type here.

#include "hdf5_hl.h"



/* Forward declarations for workaround wrappers */
static herr_t H5LTget_dataset_info_WAR(hid_t loc_id,
                                       const char* dset_name,
                                       const void* dims,
                                       H5T_class_t* type_class,
                                       size_t* type_size);

static herr_t H5LTmake_dataset_WAR(hid_t loc_id,
                                   const char* dset_name,
                                   int rank,
                                   const void* dims,
                                   hid_t type_id,
                                   void* buffer);

/* Wrappers for workarounds */
static herr_t H5LTget_dataset_info_WAR(hid_t loc_id,
                                       const char* dset_name,
                                       const void* dims,
                                       H5T_class_t* type_class,
                                       size_t* type_size) {
  return H5LTget_dataset_info(loc_id,
                              dset_name,
                              (unsigned long long*)dims,
                              type_class,
                              type_size);
}

static herr_t H5LTmake_dataset_WAR(hid_t loc_id,
                                   const char* dset_name,
                                   int rank,
                                   const void* dims,
                                   hid_t type_id,
                                   void* buffer) {
  return H5LTmake_dataset(loc_id,
                          dset_name,
                          rank,
                          (unsigned long long*)dims,
                          type_id,
                          buffer);
}



#endif
