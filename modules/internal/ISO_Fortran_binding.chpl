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

// generated from:
// jupiter:/opt/intel/compilers_and_libraries_2019.0.117/linux/compiler/include/ISO_Fortran_binding.h
module ISO_Fortran_binding {
  private use SysCTypes;
  private use ChapelBase, ChapelRange, CPtr;
  require "chpl-ISO_Fortran_binding.h";


  // #define'd integer literals:
  // Note: some of these may have been defined with an ifdef
  extern const CFI_attribute_pointer : int;
  extern const CFI_attribute_allocatable : int;
  extern const CFI_attribute_other : int;
  extern const CFI__max_attribute : int;

  extern const CFI_type_int8_t : int;
  extern const CFI_type_int16_t : int;
  extern const CFI_type_int32_t : int;
  extern const CFI_type_int64_t : int;
  extern const CFI_type_signed_char : int;
  extern const CFI_type_short : int;
  extern const CFI_type_int : int;
  extern const CFI_type_long : int;
  extern const CFI_type_long_long : int;
  extern const CFI_type_size_t : int;

  extern const CFI_type_int_least8_t : int;
  extern const CFI_type_int_least16_t : int;
  extern const CFI_type_int_least32_t : int;
  extern const CFI_type_int_least64_t : int;
  extern const CFI_type_int_fast8_t : int;
  extern const CFI_type_int_fast16_t : int;
  extern const CFI_type_int_fast32_t : int;
  extern const CFI_type_int_fast64_t : int;
  extern const CFI_type_intmax_t : int;
  extern const CFI_type_intptr_t : int;
  extern const CFI_type_ptrdiff_t : int;

  extern const CFI_type_float : int;
  extern const CFI_type_double : int;
  extern const CFI_type_long_double : int;
  extern const CFI_type_double_Complex : int;
  extern const CFI_type_long_double_Complex : int;
  extern const CFI_type_Bool : int;
  extern const CFI_type_char : int;
  extern const CFI_type_cptr : int;
  extern const CFI_type_struct : int;
  extern const CFI_type_other : int;
  extern const CFI__max_type : int;

  extern const CFI_SUCCESS : int;
  extern const CFI_ERROR_BASE_ADDR_NULL : int;
  extern const CFI_ERROR_BASE_ADDR_NOT_NULL : int;
  extern const CFI_INVALID_ELEM_LEN : int;
  extern const CFI_INVALID_RANK : int;
  extern const CFI_INVALID_TYPE : int;
  extern const CFI_INVALID_ATTRIBUTE : int;
  extern const CFI_INVALID_EXTENT : int;
  extern const CFI_INVALID_DESCRIPTOR : int;
  extern const CFI_ERROR_MEM_ALLOCATION : int;
  extern const CFI_ERROR_OUT_OF_BOUNDS : int;

  extern const CFI_MAX_RANK : int;
  extern const CFI_VERSION : int;
  extern const CFI__VERSION_0001 : int;

  // End of #define'd integer literals

  inline proc CFI_address(ref dv: CFI_cdesc_t, subscripts: c_ptr(CFI_index_t)) {
    return for_CFI_address(dv, subscripts);
  }

  pragma "no doc"
  extern proc for_CFI_address(ref dv : CFI_cdesc_t, subscripts : c_ptr(CFI_index_t)) : c_void_ptr;

  inline proc CFI_allocate(ref dv: CFI_cdesc_t, lower_bounds: c_ptr(CFI_index_t), upper_bounds: c_ptr(CFI_index_t), elem_len: size_t): c_int {
    return for_CFI_allocate(dv, lower_bounds, upper_bounds, elem_len);
  }

  pragma "no doc"
  extern proc for_CFI_allocate(ref dv : CFI_cdesc_t, lower_bounds : c_ptr(CFI_index_t), upper_bounds : c_ptr(CFI_index_t), elem_len : size_t) : c_int;

  inline proc CFI_deallocate(ref dv: CFI_cdesc_t): c_int {
    return for_CFI_deallocate(dv);
  }

  pragma "no doc"
  extern proc for_CFI_deallocate(ref dv : CFI_cdesc_t) : c_int;

  inline proc CFI_establish(ref dv: CFI_cdesc_t, base_addr: c_void_ptr, attribute: CFI_attribute_t, type_arg: CFI_type_t, elem_len: size_t, rank: CFI_rank_t, extents: c_ptr(CFI_index_t)): c_int {
    return for_CFI_establish(dv, base_addr, attribute, type_arg, elem_len, rank, extents, CFI_VERSION);
  }

  pragma "no doc"
  extern proc for_CFI_establish(ref dv : CFI_cdesc_t, base_addr : c_void_ptr, attribute : CFI_attribute_t, type_arg : CFI_type_t, elem_len : size_t, rank : CFI_rank_t, extents : c_ptr(CFI_index_t), version : c_int) : c_int;

  inline proc CFI_is_contiguous(ref dv: CFI_cdesc_t): c_int {
    return for_CFI_is_contiguous(dv);
  }

  pragma "no doc"
  extern proc for_CFI_is_contiguous(ref dv : CFI_cdesc_t) : c_int;

  inline proc CFI_section(ref result: CFI_cdesc_t, ref source: CFI_cdesc_t, lower_bounds: c_ptr(CFI_index_t), upper_bounds: c_ptr(CFI_index_t), strides: c_ptr(CFI_index_t)): c_int {
    return for_CFI_section(result, source, lower_bounds, upper_bounds, strides);
  }

  pragma "no doc"
  extern proc for_CFI_section(ref result : CFI_cdesc_t, ref source : CFI_cdesc_t, lower_bounds : c_ptr(CFI_index_t), upper_bounds : c_ptr(CFI_index_t), strides : c_ptr(CFI_index_t)) : c_int;

  inline proc CFI_select_part(ref result: CFI_cdesc_t, ref source: CFI_cdesc_t, displacement: size_t, elem_len: size_t): c_int {
    return for_CFI_select_part(result, source, displacement, elem_len);
  }

  pragma "no doc"
  extern proc for_CFI_select_part(ref result : CFI_cdesc_t, ref source : CFI_cdesc_t, displacement : size_t, elem_len : size_t) : c_int;

  inline proc CFI_setpointer(ref result: CFI_cdesc_t, ref source: CFI_cdesc_t, lower_bounds: c_ptr(CFI_index_t)): c_int {
    return for_CFI_setpointer(result, source, lower_bounds);
  }

  pragma "no doc"
  extern proc for_CFI_setpointer(ref result : CFI_cdesc_t, ref source : CFI_cdesc_t, lower_bounds : c_ptr(CFI_index_t)) : c_int;

  // ==== c2chapel typedefs ====

  extern type CFI_attribute_t = c_ptrdiff;

  extern record CFI_cdesc_t {
    var base_addr: c_void_ptr;
    var elem_len: size_t;
    var version: c_int;
    var attribute: CFI_attribute_t;
    var rank: CFI_rank_t;
    extern "type" var ctype: CFI_type_t;
    var dim: c_ptr(CFI_dim_t);
  }
/*
  extern record CFI_CDESC_T {
    param r: int;
    var base_addr: c_void_ptr;
    var elem_len: size_t;
    var version: c_int;
    var attribute: CFI_attribute_t;
    var rank: CFI_rank_t;
    //var type: CFI_type_t;
    var dim: [0..#r] CFI_dim_t;
  }
*/

  extern record CFI_dim_t {
    var extent : CFI_index_t;
    var sm : CFI_index_t;
    var lower_bound : CFI_index_t;
  }

  extern type CFI_index_t = c_ptrdiff;

  extern type CFI_rank_t = c_ptrdiff;

  extern type CFI_type_t = c_ptrdiff;

  proc makeArrayFromFortranArray(ref FA: CFI_cdesc_t,
                                 type eltType,
                                 param rank=1) {
    assert(CFI_is_contiguous(FA) == 1);
    assert(FA.rank == rank);

    var cumulativeLength = 1;
    for param i in 1..rank {
      const eltSize = c_sizeof(eltType): int;
      assert(FA.dim[i-1].sm == cumulativeLength * eltSize);
      cumulativeLength *= FA.dim[i-1].extent;
    }

    var dims: rank*range;
    for param i in 1..rank {
      assert(FA.dim[i-1].lower_bound == 0);
      dims[i] = 1..#FA.dim[i-1].extent;
    }
    var D = {(...dims)};
    var A = D.buildArrayWith(eltType,
                             FA.base_addr: _ddata(eltType),
                             D.size);
    A._unowned = true;
    return A;
  }
}
