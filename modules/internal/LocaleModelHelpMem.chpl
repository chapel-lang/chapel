/*
 * Copyright 2004-2016 Cray Inc.
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

// LocaleModelHelp.chpl
//
// Provides for declarations common to locale models
// but that do not have to be the same in order to meet the
// interface.

// They are in this file as a practical matter to avoid code
// duplication. If necessary, a locale model using this file
// should feel free to reimplement them in some other way.
module LocaleModelHelpMem {

  use LocaleModelHelp;


  //////////////////////////////////////////
  //
  // support for memory management
  //

  private extern proc chpl_memhook_md_num(): chpl_mem_descInt_t;

  // The allocator pragma is used by scalar replacement.
  pragma "allocator"
  pragma "locale model alloc"
  proc chpl_here_alloc(size:int, md:chpl_mem_descInt_t): c_void_ptr {
    pragma "insert line file info"
      extern proc chpl_mem_alloc(size:size_t, md:chpl_mem_descInt_t) : c_void_ptr;
    return chpl_mem_alloc(size.safeCast(size_t), md + chpl_memhook_md_num());
  }

  pragma "allocator"
  proc chpl_here_calloc(size:int, number:int, md:chpl_mem_descInt_t): c_void_ptr {
    pragma "insert line file info"
      extern proc chpl_mem_calloc(number:size_t, size:size_t, md:chpl_mem_descInt_t) : c_void_ptr;
    return chpl_mem_calloc(number.safeCast(size_t), size.safeCast(size_t), md + chpl_memhook_md_num());
  }

  pragma "allocator"
  proc chpl_here_realloc(ptr:c_void_ptr, size:int, md:chpl_mem_descInt_t): c_void_ptr {
    pragma "insert line file info"
      extern proc chpl_mem_realloc(ptr:c_void_ptr, size:size_t, md:chpl_mem_descInt_t) : c_void_ptr;
    return chpl_mem_realloc(ptr, size.safeCast(size_t), md + chpl_memhook_md_num());
  }

  proc chpl_here_good_alloc_size(min_size:int): int {
    pragma "insert line file info"
      extern proc chpl_mem_good_alloc_size(min_size:size_t) : size_t;
    return chpl_mem_good_alloc_size(min_size.safeCast(size_t)).safeCast(int);
  }

  pragma "locale model free"
  proc chpl_here_free(ptr:c_void_ptr): void {
    pragma "insert line file info"
      extern proc chpl_mem_free(ptr:c_void_ptr) : void;
    chpl_mem_free(ptr);
  }
}
