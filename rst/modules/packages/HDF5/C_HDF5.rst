.. default-domain:: chpl

.. module:: C_HDF5
   :synopsis: The C_HDF5 module defines the interface to the HDF5 library.

C_HDF5
======
**Usage**

.. code-block:: chapel

   use HDF5.C_HDF5;

The C_HDF5 module defines the interface to the HDF5 library.
Documentation for its functions, types, and constants can be found
at the official HDF5 web site:
https://portal.hdfgroup.org/display/HDF5/HDF5


.. function:: proc H5open(): herr_t

.. function:: proc H5close(): herr_t

.. function:: proc H5dont_atexit(): herr_t

.. function:: proc H5garbage_collect(): herr_t

.. function:: proc H5set_free_list_limits(reg_global_lim: c_int, reg_list_lim: c_int, arr_global_lim: c_int, arr_list_lim: c_int, blk_global_lim: c_int, blk_list_lim: c_int): herr_t

.. function:: proc H5get_libversion(ref majnum: c_uint, ref minnum: c_uint, ref relnum: c_uint): herr_t

.. function:: proc H5check_version(majnum: c_uint, minnum: c_uint, relnum: c_uint): herr_t

.. function:: proc H5is_library_threadsafe(ref is_ts: hbool_t): herr_t

.. function:: proc H5free_memory(mem: c_void_ptr): herr_t

.. function:: proc H5allocate_memory(size: size_t, clear: hbool_t): c_void_ptr

.. function:: proc H5resize_memory(mem: c_void_ptr, size: size_t): c_void_ptr

.. function:: proc H5Iregister(type_arg: H5I_type_t, object: c_void_ptr): hid_t

.. function:: proc H5Iobject_verify(id: hid_t, id_type: H5I_type_t): c_void_ptr

.. function:: proc H5Iremove_verify(id: hid_t, id_type: H5I_type_t): c_void_ptr

.. function:: proc H5Iget_type(id: hid_t): H5I_type_t

.. function:: proc H5Iget_file_id(id: hid_t): hid_t

.. function:: proc H5Iget_name(id: hid_t, name: c_string, size: size_t): ssize_t

.. function:: proc H5Iinc_ref(id: hid_t): c_int

.. function:: proc H5Idec_ref(id: hid_t): c_int

.. function:: proc H5Iget_ref(id: hid_t): c_int

.. function:: proc H5Iregister_type(hash_size: size_t, reserved: c_uint, free_func: H5I_free_t): H5I_type_t

.. function:: proc H5Iclear_type(type_arg: H5I_type_t, force: hbool_t): herr_t

.. function:: proc H5Idestroy_type(type_arg: H5I_type_t): herr_t

.. function:: proc H5Iinc_type_ref(type_arg: H5I_type_t): c_int

.. function:: proc H5Idec_type_ref(type_arg: H5I_type_t): c_int

.. function:: proc H5Iget_type_ref(type_arg: H5I_type_t): c_int

.. function:: proc H5Isearch(type_arg: H5I_type_t, func: H5I_search_func_t, key: c_void_ptr): c_void_ptr

.. function:: proc H5Inmembers(type_arg: H5I_type_t, ref num_members: hsize_t): herr_t

.. function:: proc H5Itype_exists(type_arg: H5I_type_t): htri_t

.. function:: proc H5Iis_valid(id: hid_t): htri_t

.. data:: var H5T_IEEE_F32BE_g: hid_t

.. data:: var H5T_IEEE_F32LE_g: hid_t

.. data:: var H5T_IEEE_F64BE_g: hid_t

.. data:: var H5T_IEEE_F64LE_g: hid_t

.. data:: var H5T_STD_I8BE_g: hid_t

.. data:: var H5T_STD_I8LE_g: hid_t

.. data:: var H5T_STD_I16BE_g: hid_t

.. data:: var H5T_STD_I16LE_g: hid_t

.. data:: var H5T_STD_I32BE_g: hid_t

.. data:: var H5T_STD_I32LE_g: hid_t

.. data:: var H5T_STD_I64BE_g: hid_t

.. data:: var H5T_STD_I64LE_g: hid_t

.. data:: var H5T_STD_U8BE_g: hid_t

.. data:: var H5T_STD_U8LE_g: hid_t

.. data:: var H5T_STD_U16BE_g: hid_t

.. data:: var H5T_STD_U16LE_g: hid_t

.. data:: var H5T_STD_U32BE_g: hid_t

.. data:: var H5T_STD_U32LE_g: hid_t

.. data:: var H5T_STD_U64BE_g: hid_t

.. data:: var H5T_STD_U64LE_g: hid_t

.. data:: var H5T_STD_B8BE_g: hid_t

.. data:: var H5T_STD_B8LE_g: hid_t

.. data:: var H5T_STD_B16BE_g: hid_t

.. data:: var H5T_STD_B16LE_g: hid_t

.. data:: var H5T_STD_B32BE_g: hid_t

.. data:: var H5T_STD_B32LE_g: hid_t

.. data:: var H5T_STD_B64BE_g: hid_t

.. data:: var H5T_STD_B64LE_g: hid_t

.. data:: var H5T_STD_REF_OBJ_g: hid_t

.. data:: var H5T_STD_REF_DSETREG_g: hid_t

.. data:: var H5T_UNIX_D32BE_g: hid_t

.. data:: var H5T_UNIX_D32LE_g: hid_t

.. data:: var H5T_UNIX_D64BE_g: hid_t

.. data:: var H5T_UNIX_D64LE_g: hid_t

.. data:: var H5T_C_S1_g: hid_t

.. data:: var H5T_FORTRAN_S1_g: hid_t

.. data:: var H5T_VAX_F32_g: hid_t

.. data:: var H5T_VAX_F64_g: hid_t

.. data:: var H5T_NATIVE_SCHAR_g: hid_t

.. data:: var H5T_NATIVE_UCHAR_g: hid_t

.. data:: var H5T_NATIVE_SHORT_g: hid_t

.. data:: var H5T_NATIVE_USHORT_g: hid_t

.. data:: var H5T_NATIVE_INT_g: hid_t

.. data:: var H5T_NATIVE_UINT_g: hid_t

.. data:: var H5T_NATIVE_LONG_g: hid_t

.. data:: var H5T_NATIVE_ULONG_g: hid_t

.. data:: var H5T_NATIVE_LLONG_g: hid_t

.. data:: var H5T_NATIVE_ULLONG_g: hid_t

.. data:: var H5T_NATIVE_FLOAT_g: hid_t

.. data:: var H5T_NATIVE_DOUBLE_g: hid_t

.. data:: var H5T_NATIVE_LDOUBLE_g: hid_t

.. data:: var H5T_NATIVE_B8_g: hid_t

.. data:: var H5T_NATIVE_B16_g: hid_t

.. data:: var H5T_NATIVE_B32_g: hid_t

.. data:: var H5T_NATIVE_B64_g: hid_t

.. data:: var H5T_NATIVE_OPAQUE_g: hid_t

.. data:: var H5T_NATIVE_HADDR_g: hid_t

.. data:: var H5T_NATIVE_HSIZE_g: hid_t

.. data:: var H5T_NATIVE_HSSIZE_g: hid_t

.. data:: var H5T_NATIVE_HERR_g: hid_t

.. data:: var H5T_NATIVE_HBOOL_g: hid_t

.. data:: var H5T_NATIVE_INT8_g: hid_t

.. data:: var H5T_NATIVE_UINT8_g: hid_t

.. data:: var H5T_NATIVE_INT_LEAST8_g: hid_t

.. data:: var H5T_NATIVE_UINT_LEAST8_g: hid_t

.. data:: var H5T_NATIVE_INT_FAST8_g: hid_t

.. data:: var H5T_NATIVE_UINT_FAST8_g: hid_t

.. data:: var H5T_NATIVE_INT16_g: hid_t

.. data:: var H5T_NATIVE_UINT16_g: hid_t

.. data:: var H5T_NATIVE_INT_LEAST16_g: hid_t

.. data:: var H5T_NATIVE_UINT_LEAST16_g: hid_t

.. data:: var H5T_NATIVE_INT_FAST16_g: hid_t

.. data:: var H5T_NATIVE_UINT_FAST16_g: hid_t

.. data:: var H5T_NATIVE_INT32_g: hid_t

.. data:: var H5T_NATIVE_UINT32_g: hid_t

.. data:: var H5T_NATIVE_INT_LEAST32_g: hid_t

.. data:: var H5T_NATIVE_UINT_LEAST32_g: hid_t

.. data:: var H5T_NATIVE_INT_FAST32_g: hid_t

.. data:: var H5T_NATIVE_UINT_FAST32_g: hid_t

.. data:: var H5T_NATIVE_INT64_g: hid_t

.. data:: var H5T_NATIVE_UINT64_g: hid_t

.. data:: var H5T_NATIVE_INT_LEAST64_g: hid_t

.. data:: var H5T_NATIVE_UINT_LEAST64_g: hid_t

.. data:: var H5T_NATIVE_INT_FAST64_g: hid_t

.. data:: var H5T_NATIVE_UINT_FAST64_g: hid_t

.. function:: proc H5Tcreate(type_arg: H5T_class_t, size: size_t): hid_t

.. function:: proc H5Tcopy(type_id: hid_t): hid_t

.. function:: proc H5Tclose(type_id: hid_t): herr_t

.. function:: proc H5Tequal(type1_id: hid_t, type2_id: hid_t): htri_t

.. function:: proc H5Tlock(type_id: hid_t): herr_t

.. function:: proc H5Tcommit2(loc_id: hid_t, name: c_string, type_id: hid_t, lcpl_id: hid_t, tcpl_id: hid_t, tapl_id: hid_t): herr_t

.. function:: proc H5Topen2(loc_id: hid_t, name: c_string, tapl_id: hid_t): hid_t

.. function:: proc H5Tcommit_anon(loc_id: hid_t, type_id: hid_t, tcpl_id: hid_t, tapl_id: hid_t): herr_t

.. function:: proc H5Tget_create_plist(type_id: hid_t): hid_t

.. function:: proc H5Tcommitted(type_id: hid_t): htri_t

.. function:: proc H5Tencode(obj_id: hid_t, buf: c_void_ptr, ref nalloc: size_t): herr_t

.. function:: proc H5Tdecode(buf: c_void_ptr): hid_t

.. function:: proc H5Tflush(type_id: hid_t): herr_t

.. function:: proc H5Trefresh(type_id: hid_t): herr_t

.. function:: proc H5Tinsert(parent_id: hid_t, name: c_string, offset: size_t, member_id: hid_t): herr_t

.. function:: proc H5Tpack(type_id: hid_t): herr_t

.. function:: proc H5Tenum_create(base_id: hid_t): hid_t

.. function:: proc H5Tenum_insert(type_arg: hid_t, name: c_string, value: c_void_ptr): herr_t

.. function:: proc H5Tenum_nameof(type_arg: hid_t, value: c_void_ptr, name: c_string, size: size_t): herr_t

.. function:: proc H5Tenum_valueof(type_arg: hid_t, name: c_string, value: c_void_ptr): herr_t

.. function:: proc H5Tvlen_create(base_id: hid_t): hid_t

.. function:: proc H5Tarray_create2(base_id: hid_t, ndims: c_uint, dim: c_ptr(hsize_t)): hid_t

.. function:: proc H5Tget_array_ndims(type_id: hid_t): c_int

.. function:: proc H5Tget_array_dims2(type_id: hid_t, dims: c_ptr(hsize_t)): c_int

.. function:: proc H5Tset_tag(type_arg: hid_t, tag: c_string): herr_t

.. function:: proc H5Tget_tag(type_arg: hid_t): c_string

.. function:: proc H5Tget_super(type_arg: hid_t): hid_t

.. function:: proc H5Tget_class(type_id: hid_t): H5T_class_t

.. function:: proc H5Tdetect_class(type_id: hid_t, cls: H5T_class_t): htri_t

.. function:: proc H5Tget_size(type_id: hid_t): size_t

.. function:: proc H5Tget_order(type_id: hid_t): H5T_order_t

.. function:: proc H5Tget_precision(type_id: hid_t): size_t

.. function:: proc H5Tget_offset(type_id: hid_t): c_int

.. function:: proc H5Tget_pad(type_id: hid_t, ref lsb: H5T_pad_t, ref msb: H5T_pad_t): herr_t

.. function:: proc H5Tget_sign(type_id: hid_t): H5T_sign_t

.. function:: proc H5Tget_fields(type_id: hid_t, ref spos: size_t, ref epos: size_t, ref esize: size_t, ref mpos: size_t, ref msize: size_t): herr_t

.. function:: proc H5Tget_ebias(type_id: hid_t): size_t

.. function:: proc H5Tget_norm(type_id: hid_t): H5T_norm_t

.. function:: proc H5Tget_inpad(type_id: hid_t): H5T_pad_t

.. function:: proc H5Tget_strpad(type_id: hid_t): H5T_str_t

.. function:: proc H5Tget_nmembers(type_id: hid_t): c_int

.. function:: proc H5Tget_member_name(type_id: hid_t, membno: c_uint): c_string

.. function:: proc H5Tget_member_index(type_id: hid_t, name: c_string): c_int

.. function:: proc H5Tget_member_offset(type_id: hid_t, membno: c_uint): size_t

.. function:: proc H5Tget_member_class(type_id: hid_t, membno: c_uint): H5T_class_t

.. function:: proc H5Tget_member_type(type_id: hid_t, membno: c_uint): hid_t

.. function:: proc H5Tget_member_value(type_id: hid_t, membno: c_uint, value: c_void_ptr): herr_t

.. function:: proc H5Tget_cset(type_id: hid_t): H5T_cset_t

.. function:: proc H5Tis_variable_str(type_id: hid_t): htri_t

.. function:: proc H5Tget_native_type(type_id: hid_t, direction: H5T_direction_t): hid_t

.. function:: proc H5Tset_size(type_id: hid_t, size: size_t): herr_t

.. function:: proc H5Tset_order(type_id: hid_t, order: H5T_order_t): herr_t

.. function:: proc H5Tset_precision(type_id: hid_t, prec: size_t): herr_t

.. function:: proc H5Tset_offset(type_id: hid_t, offset: size_t): herr_t

.. function:: proc H5Tset_pad(type_id: hid_t, lsb: H5T_pad_t, msb: H5T_pad_t): herr_t

.. function:: proc H5Tset_sign(type_id: hid_t, sign: H5T_sign_t): herr_t

.. function:: proc H5Tset_fields(type_id: hid_t, spos: size_t, epos: size_t, esize: size_t, mpos: size_t, msize: size_t): herr_t

.. function:: proc H5Tset_ebias(type_id: hid_t, ebias: size_t): herr_t

.. function:: proc H5Tset_norm(type_id: hid_t, norm: H5T_norm_t): herr_t

.. function:: proc H5Tset_inpad(type_id: hid_t, pad: H5T_pad_t): herr_t

.. function:: proc H5Tset_cset(type_id: hid_t, cset: H5T_cset_t): herr_t

.. function:: proc H5Tset_strpad(type_id: hid_t, strpad: H5T_str_t): herr_t

.. function:: proc H5Tregister(pers: H5T_pers_t, name: c_string, src_id: hid_t, dst_id: hid_t, func: H5T_conv_t): herr_t

.. function:: proc H5Tunregister(pers: H5T_pers_t, name: c_string, src_id: hid_t, dst_id: hid_t, func: H5T_conv_t): herr_t

.. function:: proc H5Tfind(src_id: hid_t, dst_id: hid_t, ref pcdata: c_ptr(H5T_cdata_t)): H5T_conv_t

.. function:: proc H5Tcompiler_conv(src_id: hid_t, dst_id: hid_t): htri_t

.. function:: proc H5Tconvert(src_id: hid_t, dst_id: hid_t, nelmts: size_t, buf: c_void_ptr, background: c_void_ptr, plist_id: hid_t): herr_t

.. function:: proc H5Tcommit1(loc_id: hid_t, name: c_string, type_id: hid_t): herr_t

.. function:: proc H5Topen1(loc_id: hid_t, name: c_string): hid_t

.. function:: proc H5Tarray_create1(base_id: hid_t, ndims: c_int, dim: c_ptr(hsize_t), perm: c_ptr(c_int)): hid_t

.. function:: proc H5Tget_array_dims1(type_id: hid_t, dims: c_ptr(hsize_t), perm: c_ptr(c_int)): c_int

.. function:: proc H5Lmove(src_loc: hid_t, src_name: c_string, dst_loc: hid_t, dst_name: c_string, lcpl_id: hid_t, lapl_id: hid_t): herr_t

.. function:: proc H5Lcopy(src_loc: hid_t, src_name: c_string, dst_loc: hid_t, dst_name: c_string, lcpl_id: hid_t, lapl_id: hid_t): herr_t

.. function:: proc H5Lcreate_hard(cur_loc: hid_t, cur_name: c_string, dst_loc: hid_t, dst_name: c_string, lcpl_id: hid_t, lapl_id: hid_t): herr_t

.. function:: proc H5Lcreate_soft(link_target: c_string, link_loc_id: hid_t, link_name: c_string, lcpl_id: hid_t, lapl_id: hid_t): herr_t

.. function:: proc H5Ldelete(loc_id: hid_t, name: c_string, lapl_id: hid_t): herr_t

.. function:: proc H5Ldelete_by_idx(loc_id: hid_t, group_name: c_string, idx_type: H5_index_t, order: H5_iter_order_t, n: hsize_t, lapl_id: hid_t): herr_t

.. function:: proc H5Lget_val(loc_id: hid_t, name: c_string, buf: c_void_ptr, size: size_t, lapl_id: hid_t): herr_t

.. function:: proc H5Lget_val_by_idx(loc_id: hid_t, group_name: c_string, idx_type: H5_index_t, order: H5_iter_order_t, n: hsize_t, buf: c_void_ptr, size: size_t, lapl_id: hid_t): herr_t

.. function:: proc H5Lexists(loc_id: hid_t, name: c_string, lapl_id: hid_t): htri_t

.. function:: proc H5Lget_info(loc_id: hid_t, name: c_string, ref linfo: H5L_info_t, lapl_id: hid_t): herr_t

.. function:: proc H5Lget_info_by_idx(loc_id: hid_t, group_name: c_string, idx_type: H5_index_t, order: H5_iter_order_t, n: hsize_t, ref linfo: H5L_info_t, lapl_id: hid_t): herr_t

.. function:: proc H5Lget_name_by_idx(loc_id: hid_t, group_name: c_string, idx_type: H5_index_t, order: H5_iter_order_t, n: hsize_t, name: c_string, size: size_t, lapl_id: hid_t): ssize_t

.. function:: proc H5Literate(grp_id: hid_t, idx_type: H5_index_t, order: H5_iter_order_t, ref idx: hsize_t, op: H5L_iterate_t, op_data: c_void_ptr): herr_t

.. function:: proc H5Literate_by_name(loc_id: hid_t, group_name: c_string, idx_type: H5_index_t, order: H5_iter_order_t, ref idx: hsize_t, op: H5L_iterate_t, op_data: c_void_ptr, lapl_id: hid_t): herr_t

.. function:: proc H5Lvisit(grp_id: hid_t, idx_type: H5_index_t, order: H5_iter_order_t, op: H5L_iterate_t, op_data: c_void_ptr): herr_t

.. function:: proc H5Lvisit_by_name(loc_id: hid_t, group_name: c_string, idx_type: H5_index_t, order: H5_iter_order_t, op: H5L_iterate_t, op_data: c_void_ptr, lapl_id: hid_t): herr_t

.. function:: proc H5Lcreate_ud(link_loc_id: hid_t, link_name: c_string, link_type: H5L_type_t, udata: c_void_ptr, udata_size: size_t, lcpl_id: hid_t, lapl_id: hid_t): herr_t

.. function:: proc H5Lregister(ref cls: H5L_class_t): herr_t

.. function:: proc H5Lunregister(id: H5L_type_t): herr_t

.. function:: proc H5Lis_registered(id: H5L_type_t): htri_t

.. function:: proc H5Lunpack_elink_val(ext_linkval: c_void_ptr, link_size: size_t, ref flags: c_uint, ref filename: c_string, ref obj_path: c_string): herr_t

.. function:: proc H5Lcreate_external(file_name: c_string, obj_name: c_string, link_loc_id: hid_t, link_name: c_string, lcpl_id: hid_t, lapl_id: hid_t): herr_t

.. function:: proc H5Oopen(loc_id: hid_t, name: c_string, lapl_id: hid_t): hid_t

.. function:: proc H5Oopen_by_addr(loc_id: hid_t, addr: haddr_t): hid_t

.. function:: proc H5Oopen_by_idx(loc_id: hid_t, group_name: c_string, idx_type: H5_index_t, order: H5_iter_order_t, n: hsize_t, lapl_id: hid_t): hid_t

.. function:: proc H5Oexists_by_name(loc_id: hid_t, name: c_string, lapl_id: hid_t): htri_t

.. function:: proc H5Oget_info(loc_id: hid_t, ref oinfo: H5O_info_t): herr_t

.. function:: proc H5Oget_info_by_name(loc_id: hid_t, name: c_string, ref oinfo: H5O_info_t, lapl_id: hid_t): herr_t

.. function:: proc H5Oget_info_by_idx(loc_id: hid_t, group_name: c_string, idx_type: H5_index_t, order: H5_iter_order_t, n: hsize_t, ref oinfo: H5O_info_t, lapl_id: hid_t): herr_t

.. function:: proc H5Olink(obj_id: hid_t, new_loc_id: hid_t, new_name: c_string, lcpl_id: hid_t, lapl_id: hid_t): herr_t

.. function:: proc H5Oincr_refcount(object_id: hid_t): herr_t

.. function:: proc H5Odecr_refcount(object_id: hid_t): herr_t

.. function:: proc H5Ocopy(src_loc_id: hid_t, src_name: c_string, dst_loc_id: hid_t, dst_name: c_string, ocpypl_id: hid_t, lcpl_id: hid_t): herr_t

.. function:: proc H5Oset_comment(obj_id: hid_t, comment: c_string): herr_t

.. function:: proc H5Oset_comment_by_name(loc_id: hid_t, name: c_string, comment: c_string, lapl_id: hid_t): herr_t

.. function:: proc H5Oget_comment(obj_id: hid_t, comment: c_string, bufsize: size_t): ssize_t

.. function:: proc H5Oget_comment_by_name(loc_id: hid_t, name: c_string, comment: c_string, bufsize: size_t, lapl_id: hid_t): ssize_t

.. function:: proc H5Ovisit(obj_id: hid_t, idx_type: H5_index_t, order: H5_iter_order_t, op: H5O_iterate_t, op_data: c_void_ptr): herr_t

.. function:: proc H5Ovisit_by_name(loc_id: hid_t, obj_name: c_string, idx_type: H5_index_t, order: H5_iter_order_t, op: H5O_iterate_t, op_data: c_void_ptr, lapl_id: hid_t): herr_t

.. function:: proc H5Oclose(object_id: hid_t): herr_t

.. function:: proc H5Oflush(obj_id: hid_t): herr_t

.. function:: proc H5Orefresh(oid: hid_t): herr_t

.. function:: proc H5Odisable_mdc_flushes(object_id: hid_t): herr_t

.. function:: proc H5Oenable_mdc_flushes(object_id: hid_t): herr_t

.. function:: proc H5Oare_mdc_flushes_disabled(object_id: hid_t, ref are_disabled: hbool_t): herr_t

.. function:: proc H5Acreate2(loc_id: hid_t, attr_name: c_string, type_id: hid_t, space_id: hid_t, acpl_id: hid_t, aapl_id: hid_t): hid_t

.. function:: proc H5Acreate_by_name(loc_id: hid_t, obj_name: c_string, attr_name: c_string, type_id: hid_t, space_id: hid_t, acpl_id: hid_t, aapl_id: hid_t, lapl_id: hid_t): hid_t

.. function:: proc H5Aopen(obj_id: hid_t, attr_name: c_string, aapl_id: hid_t): hid_t

.. function:: proc H5Aopen_by_name(loc_id: hid_t, obj_name: c_string, attr_name: c_string, aapl_id: hid_t, lapl_id: hid_t): hid_t

.. function:: proc H5Aopen_by_idx(loc_id: hid_t, obj_name: c_string, idx_type: H5_index_t, order: H5_iter_order_t, n: hsize_t, aapl_id: hid_t, lapl_id: hid_t): hid_t

.. function:: proc H5Awrite(attr_id: hid_t, type_id: hid_t, buf: c_void_ptr): herr_t

.. function:: proc H5Aread(attr_id: hid_t, type_id: hid_t, buf: c_void_ptr): herr_t

.. function:: proc H5Aclose(attr_id: hid_t): herr_t

.. function:: proc H5Aget_space(attr_id: hid_t): hid_t

.. function:: proc H5Aget_type(attr_id: hid_t): hid_t

.. function:: proc H5Aget_create_plist(attr_id: hid_t): hid_t

.. function:: proc H5Aget_name(attr_id: hid_t, buf_size: size_t, buf: c_string): ssize_t

.. function:: proc H5Aget_name_by_idx(loc_id: hid_t, obj_name: c_string, idx_type: H5_index_t, order: H5_iter_order_t, n: hsize_t, name: c_string, size: size_t, lapl_id: hid_t): ssize_t

.. function:: proc H5Aget_storage_size(attr_id: hid_t): hsize_t

.. function:: proc H5Aget_info(attr_id: hid_t, ref ainfo: H5A_info_t): herr_t

.. function:: proc H5Aget_info_by_name(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref ainfo: H5A_info_t, lapl_id: hid_t): herr_t

.. function:: proc H5Aget_info_by_idx(loc_id: hid_t, obj_name: c_string, idx_type: H5_index_t, order: H5_iter_order_t, n: hsize_t, ref ainfo: H5A_info_t, lapl_id: hid_t): herr_t

.. function:: proc H5Arename(loc_id: hid_t, old_name: c_string, new_name: c_string): herr_t

.. function:: proc H5Arename_by_name(loc_id: hid_t, obj_name: c_string, old_attr_name: c_string, new_attr_name: c_string, lapl_id: hid_t): herr_t

.. function:: proc H5Aiterate2(loc_id: hid_t, idx_type: H5_index_t, order: H5_iter_order_t, ref idx: hsize_t, op: H5A_operator2_t, op_data: c_void_ptr): herr_t

.. function:: proc H5Aiterate_by_name(loc_id: hid_t, obj_name: c_string, idx_type: H5_index_t, order: H5_iter_order_t, ref idx: hsize_t, op: H5A_operator2_t, op_data: c_void_ptr, lapd_id: hid_t): herr_t

.. function:: proc H5Adelete(loc_id: hid_t, name: c_string): herr_t

.. function:: proc H5Adelete_by_name(loc_id: hid_t, obj_name: c_string, attr_name: c_string, lapl_id: hid_t): herr_t

.. function:: proc H5Adelete_by_idx(loc_id: hid_t, obj_name: c_string, idx_type: H5_index_t, order: H5_iter_order_t, n: hsize_t, lapl_id: hid_t): herr_t

.. function:: proc H5Aexists(obj_id: hid_t, attr_name: c_string): htri_t

.. function:: proc H5Aexists_by_name(obj_id: hid_t, obj_name: c_string, attr_name: c_string, lapl_id: hid_t): htri_t

.. function:: proc H5Acreate1(loc_id: hid_t, name: c_string, type_id: hid_t, space_id: hid_t, acpl_id: hid_t): hid_t

.. function:: proc H5Aopen_name(loc_id: hid_t, name: c_string): hid_t

.. function:: proc H5Aopen_idx(loc_id: hid_t, idx: c_uint): hid_t

.. function:: proc H5Aget_num_attrs(loc_id: hid_t): c_int

.. function:: proc H5Aiterate1(loc_id: hid_t, ref attr_num: c_uint, op: H5A_operator1_t, op_data: c_void_ptr): herr_t

.. type:: type H5C_cache_incr_mode = c_int

.. data:: const H5C_incr__off: H5C_cache_incr_mode

.. data:: const H5C_incr__threshold: H5C_cache_incr_mode

.. type:: type H5C_cache_flash_incr_mode = c_int

.. data:: const H5C_flash_incr__off: H5C_cache_flash_incr_mode

.. data:: const H5C_flash_incr__add_space: H5C_cache_flash_incr_mode

.. type:: type H5C_cache_decr_mode = c_int

.. data:: const H5C_decr__off: H5C_cache_decr_mode

.. data:: const H5C_decr__threshold: H5C_cache_decr_mode

.. data:: const H5C_decr__age_out: H5C_cache_decr_mode

.. data:: const H5C_decr__age_out_with_threshold: H5C_cache_decr_mode

.. function:: proc H5Dcreate2(loc_id: hid_t, name: c_string, type_id: hid_t, space_id: hid_t, lcpl_id: hid_t, dcpl_id: hid_t, dapl_id: hid_t): hid_t

.. function:: proc H5Dcreate_anon(file_id: hid_t, type_id: hid_t, space_id: hid_t, plist_id: hid_t, dapl_id: hid_t): hid_t

.. function:: proc H5Dopen2(file_id: hid_t, name: c_string, dapl_id: hid_t): hid_t

.. function:: proc H5Dopen(file_id: hid_t, name: c_string, dapl_id: hid_t): hid_t

.. function:: proc H5Dclose(dset_id: hid_t): herr_t

.. function:: proc H5Dget_space(dset_id: hid_t): hid_t

.. function:: proc H5Dget_space_status(dset_id: hid_t, ref allocation: H5D_space_status_t): herr_t

.. function:: proc H5Dget_type(dset_id: hid_t): hid_t

.. function:: proc H5Dget_create_plist(dset_id: hid_t): hid_t

.. function:: proc H5Dget_access_plist(dset_id: hid_t): hid_t

.. function:: proc H5Dget_storage_size(dset_id: hid_t): hsize_t

.. function:: proc H5Dget_offset(dset_id: hid_t): haddr_t

.. function:: proc H5Dread(dset_id: hid_t, mem_type_id: hid_t, mem_space_id: hid_t, file_space_id: hid_t, plist_id: hid_t, buf: c_void_ptr): herr_t

.. function:: proc H5Dwrite(dset_id: hid_t, mem_type_id: hid_t, mem_space_id: hid_t, file_space_id: hid_t, plist_id: hid_t, buf: c_void_ptr): herr_t

.. function:: proc H5Diterate(buf: c_void_ptr, type_id: hid_t, space_id: hid_t, op: H5D_operator_t, operator_data: c_void_ptr): herr_t

.. function:: proc H5Dvlen_reclaim(type_id: hid_t, space_id: hid_t, plist_id: hid_t, buf: c_void_ptr): herr_t

.. function:: proc H5Dvlen_get_buf_size(dataset_id: hid_t, type_id: hid_t, space_id: hid_t, ref size: hsize_t): herr_t

.. function:: proc H5Dfill(fill: c_void_ptr, fill_type: hid_t, buf: c_void_ptr, buf_type: hid_t, space: hid_t): herr_t

.. function:: proc H5Dset_extent(dset_id: hid_t, size: c_ptr(hsize_t)): herr_t

.. function:: proc H5Dflush(dset_id: hid_t): herr_t

.. function:: proc H5Drefresh(dset_id: hid_t): herr_t

.. function:: proc H5Dscatter(op: H5D_scatter_func_t, op_data: c_void_ptr, type_id: hid_t, dst_space_id: hid_t, dst_buf: c_void_ptr): herr_t

.. function:: proc H5Dgather(src_space_id: hid_t, src_buf: c_void_ptr, type_id: hid_t, dst_buf_size: size_t, dst_buf: c_void_ptr, op: H5D_gather_func_t, op_data: c_void_ptr): herr_t

.. function:: proc H5Ddebug(dset_id: hid_t): herr_t

.. function:: proc H5Dformat_convert(dset_id: hid_t): herr_t

.. function:: proc H5Dget_chunk_index_type(did: hid_t, ref idx_type: H5D_chunk_index_t): herr_t

.. function:: proc H5Dcreate1(file_id: hid_t, name: c_string, type_id: hid_t, space_id: hid_t, dcpl_id: hid_t): hid_t

.. function:: proc H5Dopen1(file_id: hid_t, name: c_string): hid_t

.. function:: proc H5Dextend(dset_id: hid_t, size: c_ptr(hsize_t)): herr_t

.. data:: var H5E_ERR_CLS_g: hid_t

.. data:: var H5E_FUNC_g: hid_t

.. data:: var H5E_FILE_g: hid_t

.. data:: var H5E_SOHM_g: hid_t

.. data:: var H5E_SYM_g: hid_t

.. data:: var H5E_PLUGIN_g: hid_t

.. data:: var H5E_VFL_g: hid_t

.. data:: var H5E_INTERNAL_g: hid_t

.. data:: var H5E_BTREE_g: hid_t

.. data:: var H5E_REFERENCE_g: hid_t

.. data:: var H5E_DATASPACE_g: hid_t

.. data:: var H5E_RESOURCE_g: hid_t

.. data:: var H5E_RS_g: hid_t

.. data:: var H5E_FARRAY_g: hid_t

.. data:: var H5E_HEAP_g: hid_t

.. data:: var H5E_ATTR_g: hid_t

.. data:: var H5E_IO_g: hid_t

.. data:: var H5E_EFL_g: hid_t

.. data:: var H5E_TST_g: hid_t

.. data:: var H5E_PAGEBUF_g: hid_t

.. data:: var H5E_FSPACE_g: hid_t

.. data:: var H5E_DATASET_g: hid_t

.. data:: var H5E_STORAGE_g: hid_t

.. data:: var H5E_LINK_g: hid_t

.. data:: var H5E_PLIST_g: hid_t

.. data:: var H5E_DATATYPE_g: hid_t

.. data:: var H5E_OHDR_g: hid_t

.. data:: var H5E_ATOM_g: hid_t

.. data:: var H5E_NONE_MAJOR_g: hid_t

.. data:: var H5E_SLIST_g: hid_t

.. data:: var H5E_ARGS_g: hid_t

.. data:: var H5E_EARRAY_g: hid_t

.. data:: var H5E_PLINE_g: hid_t

.. data:: var H5E_ERROR_g: hid_t

.. data:: var H5E_CACHE_g: hid_t

.. data:: var H5E_SEEKERROR_g: hid_t

.. data:: var H5E_READERROR_g: hid_t

.. data:: var H5E_WRITEERROR_g: hid_t

.. data:: var H5E_CLOSEERROR_g: hid_t

.. data:: var H5E_OVERFLOW_g: hid_t

.. data:: var H5E_FCNTL_g: hid_t

.. data:: var H5E_NOSPACE_g: hid_t

.. data:: var H5E_CANTALLOC_g: hid_t

.. data:: var H5E_CANTCOPY_g: hid_t

.. data:: var H5E_CANTFREE_g: hid_t

.. data:: var H5E_ALREADYEXISTS_g: hid_t

.. data:: var H5E_CANTLOCK_g: hid_t

.. data:: var H5E_CANTUNLOCK_g: hid_t

.. data:: var H5E_CANTGC_g: hid_t

.. data:: var H5E_CANTGETSIZE_g: hid_t

.. data:: var H5E_OBJOPEN_g: hid_t

.. data:: var H5E_CANTRESTORE_g: hid_t

.. data:: var H5E_CANTCOMPUTE_g: hid_t

.. data:: var H5E_CANTEXTEND_g: hid_t

.. data:: var H5E_CANTATTACH_g: hid_t

.. data:: var H5E_CANTUPDATE_g: hid_t

.. data:: var H5E_CANTOPERATE_g: hid_t

.. data:: var H5E_CANTINIT_g: hid_t

.. data:: var H5E_ALREADYINIT_g: hid_t

.. data:: var H5E_CANTRELEASE_g: hid_t

.. data:: var H5E_CANTGET_g: hid_t

.. data:: var H5E_CANTSET_g: hid_t

.. data:: var H5E_DUPCLASS_g: hid_t

.. data:: var H5E_SETDISALLOWED_g: hid_t

.. data:: var H5E_CANTMERGE_g: hid_t

.. data:: var H5E_CANTREVIVE_g: hid_t

.. data:: var H5E_CANTSHRINK_g: hid_t

.. data:: var H5E_LINKCOUNT_g: hid_t

.. data:: var H5E_VERSION_g: hid_t

.. data:: var H5E_ALIGNMENT_g: hid_t

.. data:: var H5E_BADMESG_g: hid_t

.. data:: var H5E_CANTDELETE_g: hid_t

.. data:: var H5E_BADITER_g: hid_t

.. data:: var H5E_CANTPACK_g: hid_t

.. data:: var H5E_CANTRESET_g: hid_t

.. data:: var H5E_CANTRENAME_g: hid_t

.. data:: var H5E_SYSERRSTR_g: hid_t

.. data:: var H5E_NOFILTER_g: hid_t

.. data:: var H5E_CALLBACK_g: hid_t

.. data:: var H5E_CANAPPLY_g: hid_t

.. data:: var H5E_SETLOCAL_g: hid_t

.. data:: var H5E_NOENCODER_g: hid_t

.. data:: var H5E_CANTFILTER_g: hid_t

.. data:: var H5E_CANTOPENOBJ_g: hid_t

.. data:: var H5E_CANTCLOSEOBJ_g: hid_t

.. data:: var H5E_COMPLEN_g: hid_t

.. data:: var H5E_PATH_g: hid_t

.. data:: var H5E_NONE_MINOR_g: hid_t

.. data:: var H5E_OPENERROR_g: hid_t

.. data:: var H5E_FILEEXISTS_g: hid_t

.. data:: var H5E_FILEOPEN_g: hid_t

.. data:: var H5E_CANTCREATE_g: hid_t

.. data:: var H5E_CANTOPENFILE_g: hid_t

.. data:: var H5E_CANTCLOSEFILE_g: hid_t

.. data:: var H5E_NOTHDF5_g: hid_t

.. data:: var H5E_BADFILE_g: hid_t

.. data:: var H5E_TRUNCATED_g: hid_t

.. data:: var H5E_MOUNT_g: hid_t

.. data:: var H5E_BADATOM_g: hid_t

.. data:: var H5E_BADGROUP_g: hid_t

.. data:: var H5E_CANTREGISTER_g: hid_t

.. data:: var H5E_CANTINC_g: hid_t

.. data:: var H5E_CANTDEC_g: hid_t

.. data:: var H5E_NOIDS_g: hid_t

.. data:: var H5E_CANTFLUSH_g: hid_t

.. data:: var H5E_CANTUNSERIALIZE_g: hid_t

.. data:: var H5E_CANTSERIALIZE_g: hid_t

.. data:: var H5E_CANTTAG_g: hid_t

.. data:: var H5E_CANTLOAD_g: hid_t

.. data:: var H5E_PROTECT_g: hid_t

.. data:: var H5E_NOTCACHED_g: hid_t

.. data:: var H5E_SYSTEM_g: hid_t

.. data:: var H5E_CANTINS_g: hid_t

.. data:: var H5E_CANTPROTECT_g: hid_t

.. data:: var H5E_CANTUNPROTECT_g: hid_t

.. data:: var H5E_CANTPIN_g: hid_t

.. data:: var H5E_CANTUNPIN_g: hid_t

.. data:: var H5E_CANTMARKDIRTY_g: hid_t

.. data:: var H5E_CANTMARKCLEAN_g: hid_t

.. data:: var H5E_CANTMARKUNSERIALIZED_g: hid_t

.. data:: var H5E_CANTMARKSERIALIZED_g: hid_t

.. data:: var H5E_CANTDIRTY_g: hid_t

.. data:: var H5E_CANTCLEAN_g: hid_t

.. data:: var H5E_CANTEXPUNGE_g: hid_t

.. data:: var H5E_CANTRESIZE_g: hid_t

.. data:: var H5E_CANTDEPEND_g: hid_t

.. data:: var H5E_CANTUNDEPEND_g: hid_t

.. data:: var H5E_CANTNOTIFY_g: hid_t

.. data:: var H5E_LOGFAIL_g: hid_t

.. data:: var H5E_CANTCORK_g: hid_t

.. data:: var H5E_CANTUNCORK_g: hid_t

.. data:: var H5E_TRAVERSE_g: hid_t

.. data:: var H5E_NLINKS_g: hid_t

.. data:: var H5E_NOTREGISTERED_g: hid_t

.. data:: var H5E_CANTMOVE_g: hid_t

.. data:: var H5E_CANTSORT_g: hid_t

.. data:: var H5E_MPI_g: hid_t

.. data:: var H5E_MPIERRSTR_g: hid_t

.. data:: var H5E_CANTRECV_g: hid_t

.. data:: var H5E_CANTCLIP_g: hid_t

.. data:: var H5E_CANTCOUNT_g: hid_t

.. data:: var H5E_CANTSELECT_g: hid_t

.. data:: var H5E_CANTNEXT_g: hid_t

.. data:: var H5E_BADSELECT_g: hid_t

.. data:: var H5E_CANTCOMPARE_g: hid_t

.. data:: var H5E_CANTAPPEND_g: hid_t

.. data:: var H5E_UNINITIALIZED_g: hid_t

.. data:: var H5E_UNSUPPORTED_g: hid_t

.. data:: var H5E_BADTYPE_g: hid_t

.. data:: var H5E_BADRANGE_g: hid_t

.. data:: var H5E_BADVALUE_g: hid_t

.. data:: var H5E_NOTFOUND_g: hid_t

.. data:: var H5E_EXISTS_g: hid_t

.. data:: var H5E_CANTENCODE_g: hid_t

.. data:: var H5E_CANTDECODE_g: hid_t

.. data:: var H5E_CANTSPLIT_g: hid_t

.. data:: var H5E_CANTREDISTRIBUTE_g: hid_t

.. data:: var H5E_CANTSWAP_g: hid_t

.. data:: var H5E_CANTINSERT_g: hid_t

.. data:: var H5E_CANTLIST_g: hid_t

.. data:: var H5E_CANTMODIFY_g: hid_t

.. data:: var H5E_CANTREMOVE_g: hid_t

.. data:: var H5E_CANTCONVERT_g: hid_t

.. data:: var H5E_BADSIZE_g: hid_t

.. function:: proc H5Eregister_class(cls_name: c_string, lib_name: c_string, version: c_string): hid_t

.. function:: proc H5Eunregister_class(class_id: hid_t): herr_t

.. function:: proc H5Eclose_msg(err_id: hid_t): herr_t

.. function:: proc H5Ecreate_msg(cls: hid_t, msg_type: H5E_type_t, msg: c_string): hid_t

.. function:: proc H5Ecreate_stack(): hid_t

.. function:: proc H5Eget_current_stack(): hid_t

.. function:: proc H5Eclose_stack(stack_id: hid_t): herr_t

.. function:: proc H5Eget_class_name(class_id: hid_t, name: c_string, size: size_t): ssize_t

.. function:: proc H5Eset_current_stack(err_stack_id: hid_t): herr_t

.. function:: proc H5Epush2(err_stack: hid_t, file: c_string, func: c_string, line: c_uint, cls_id: hid_t, maj_id: hid_t, min_id: hid_t, msg: c_string, c__varargs ...): herr_t

.. function:: proc H5Epush2(err_stack: hid_t, file: c_string, func: c_string, line: c_uint, cls_id: hid_t, maj_id: hid_t, min_id: hid_t, msg: c_string): herr_t

.. function:: proc H5Epop(err_stack: hid_t, count: size_t): herr_t

.. function:: proc H5Eprint2(err_stack: hid_t, ref stream: _file): herr_t

.. function:: proc H5Ewalk2(err_stack: hid_t, direction: H5E_direction_t, func: H5E_walk2_t, client_data: c_void_ptr): herr_t

.. function:: proc H5Eget_auto2(estack_id: hid_t, ref func: H5E_auto2_t, ref client_data: c_void_ptr): herr_t

.. function:: proc H5Eset_auto2(estack_id: hid_t, func: H5E_auto2_t, client_data: c_void_ptr): herr_t

.. function:: proc H5Eclear2(err_stack: hid_t): herr_t

.. function:: proc H5Eauto_is_v2(err_stack: hid_t, ref is_stack: c_uint): herr_t

.. function:: proc H5Eget_msg(msg_id: hid_t, ref type_arg: H5E_type_t, msg: c_string, size: size_t): ssize_t

.. function:: proc H5Eget_num(error_stack_id: hid_t): ssize_t

.. function:: proc H5Eclear1(): herr_t

.. function:: proc H5Eget_auto1(ref func: H5E_auto1_t, ref client_data: c_void_ptr): herr_t

.. function:: proc H5Epush1(file: c_string, func: c_string, line: c_uint, maj: H5E_major_t, min: H5E_minor_t, str: c_string): herr_t

.. function:: proc H5Eprint1(ref stream: _file): herr_t

.. function:: proc H5Eset_auto1(func: H5E_auto1_t, client_data: c_void_ptr): herr_t

.. function:: proc H5Ewalk1(direction: H5E_direction_t, func: H5E_walk1_t, client_data: c_void_ptr): herr_t

.. function:: proc H5Eget_major(maj: H5E_major_t): c_string

.. function:: proc H5Eget_minor(min: H5E_minor_t): c_string

.. function:: proc H5Fis_hdf5(filename: c_string): htri_t

.. function:: proc H5Fcreate(filename: c_string, flags: c_uint, create_plist: hid_t, access_plist: hid_t): hid_t

.. function:: proc H5Fopen(filename: c_string, flags: c_uint, access_plist: hid_t): hid_t

.. function:: proc H5Freopen(file_id: hid_t): hid_t

.. function:: proc H5Fflush(object_id: hid_t, scope: H5F_scope_t): herr_t

.. function:: proc H5Fclose(file_id: hid_t): herr_t

.. function:: proc H5Fget_create_plist(file_id: hid_t): hid_t

.. function:: proc H5Fget_access_plist(file_id: hid_t): hid_t

.. function:: proc H5Fget_intent(file_id: hid_t, ref intent: c_uint): herr_t

.. function:: proc H5Fget_obj_count(file_id: hid_t, types: c_uint): ssize_t

.. function:: proc H5Fget_obj_ids(file_id: hid_t, types: c_uint, max_objs: size_t, ref obj_id_list: hid_t): ssize_t

.. function:: proc H5Fget_vfd_handle(file_id: hid_t, fapl: hid_t, ref file_handle: c_void_ptr): herr_t

.. function:: proc H5Fmount(loc: hid_t, name: c_string, child: hid_t, plist: hid_t): herr_t

.. function:: proc H5Funmount(loc: hid_t, name: c_string): herr_t

.. function:: proc H5Fget_freespace(file_id: hid_t): hssize_t

.. function:: proc H5Fget_filesize(file_id: hid_t, ref size: hsize_t): herr_t

.. function:: proc H5Fget_file_image(file_id: hid_t, buf_ptr: c_void_ptr, buf_len: size_t): ssize_t

.. function:: proc H5Fget_mdc_config(file_id: hid_t, ref config_ptr: H5AC_cache_config_t): herr_t

.. function:: proc H5Fset_mdc_config(file_id: hid_t, ref config_ptr: H5AC_cache_config_t): herr_t

.. function:: proc H5Fget_mdc_hit_rate(file_id: hid_t, ref hit_rate_ptr: c_double): herr_t

.. function:: proc H5Fget_mdc_size(file_id: hid_t, ref max_size_ptr: size_t, ref min_clean_size_ptr: size_t, ref cur_size_ptr: size_t, ref cur_num_entries_ptr: c_int): herr_t

.. function:: proc H5Freset_mdc_hit_rate_stats(file_id: hid_t): herr_t

.. function:: proc H5Fget_name(obj_id: hid_t, name: c_string, size: size_t): ssize_t

.. function:: proc H5Fget_info2(obj_id: hid_t, ref finfo: H5F_info2_t): herr_t

.. function:: proc H5Fget_metadata_read_retry_info(file_id: hid_t, ref info: H5F_retry_info_t): herr_t

.. function:: proc H5Fstart_swmr_write(file_id: hid_t): herr_t

.. function:: proc H5Fget_free_sections(file_id: hid_t, type_arg: H5F_mem_t, nsects: size_t, ref sect_info: H5F_sect_info_t): ssize_t

.. function:: proc H5Fclear_elink_file_cache(file_id: hid_t): herr_t

.. function:: proc H5Fset_latest_format(file_id: hid_t, latest_format: hbool_t): herr_t

.. function:: proc H5Fstart_mdc_logging(file_id: hid_t): herr_t

.. function:: proc H5Fstop_mdc_logging(file_id: hid_t): herr_t

.. function:: proc H5Fget_mdc_logging_status(file_id: hid_t, ref is_enabled: hbool_t, ref is_currently_logging: hbool_t): herr_t

.. function:: proc H5Fformat_convert(fid: hid_t): herr_t

.. function:: proc H5Freset_page_buffering_stats(file_id: hid_t): herr_t

.. function:: proc H5Fget_page_buffering_stats(file_id: hid_t, accesses: c_ptr(c_uint), hits: c_ptr(c_uint), misses: c_ptr(c_uint), evictions: c_ptr(c_uint), bypasses: c_ptr(c_uint)): herr_t

.. function:: proc H5Fget_mdc_image_info(file_id: hid_t, ref image_addr: haddr_t, ref image_size: hsize_t): herr_t

.. function:: proc H5Fget_info1(obj_id: hid_t, ref finfo: H5F_info1_t): herr_t

.. record:: H5FD_t

   .. attribute:: var driver_id: hid_t

   .. attribute:: var cls: c_ptr(H5FD_class_t)

   .. attribute:: var fileno: c_ulong

   .. attribute:: var access_flags: c_uint

   .. attribute:: var feature_flags: c_ulong

   .. attribute:: var maxaddr: haddr_t

   .. attribute:: var base_addr: haddr_t

   .. attribute:: var threshold: hsize_t

   .. attribute:: var alignment: hsize_t

   .. attribute:: var paged_aggr: hbool_t

.. function:: proc H5FDregister(ref cls: H5FD_class_t): hid_t

.. function:: proc H5FDunregister(driver_id: hid_t): herr_t

.. function:: proc H5FDopen(name: c_string, flags: c_uint, fapl_id: hid_t, maxaddr: haddr_t): c_ptr(H5FD_t)

.. function:: proc H5FDclose(ref file: H5FD_t): herr_t

.. function:: proc H5FDcmp(ref f1: H5FD_t, ref f2: H5FD_t): c_int

.. function:: proc H5FDquery(ref f: H5FD_t, ref flags: c_ulong): c_int

.. function:: proc H5FDalloc(ref file: H5FD_t, type_arg: H5FD_mem_t, dxpl_id: hid_t, size: hsize_t): haddr_t

.. function:: proc H5FDfree(ref file: H5FD_t, type_arg: H5FD_mem_t, dxpl_id: hid_t, addr: haddr_t, size: hsize_t): herr_t

.. function:: proc H5FDget_eoa(ref file: H5FD_t, type_arg: H5FD_mem_t): haddr_t

.. function:: proc H5FDset_eoa(ref file: H5FD_t, type_arg: H5FD_mem_t, eoa: haddr_t): herr_t

.. function:: proc H5FDget_eof(ref file: H5FD_t, type_arg: H5FD_mem_t): haddr_t

.. function:: proc H5FDget_vfd_handle(ref file: H5FD_t, fapl: hid_t, ref file_handle: c_void_ptr): herr_t

.. function:: proc H5FDread(ref file: H5FD_t, type_arg: H5FD_mem_t, dxpl_id: hid_t, addr: haddr_t, size: size_t, buf: c_void_ptr): herr_t

.. function:: proc H5FDwrite(ref file: H5FD_t, type_arg: H5FD_mem_t, dxpl_id: hid_t, addr: haddr_t, size: size_t, buf: c_void_ptr): herr_t

.. function:: proc H5FDflush(ref file: H5FD_t, dxpl_id: hid_t, closing: hbool_t): herr_t

.. function:: proc H5FDtruncate(ref file: H5FD_t, dxpl_id: hid_t, closing: hbool_t): herr_t

.. function:: proc H5FDlock(ref file: H5FD_t, rw: hbool_t): herr_t

.. function:: proc H5FDunlock(ref file: H5FD_t): herr_t

.. function:: proc H5Gcreate2(loc_id: hid_t, name: c_string, lcpl_id: hid_t, gcpl_id: hid_t, gapl_id: hid_t): hid_t

.. function:: proc H5Gcreate_anon(loc_id: hid_t, gcpl_id: hid_t, gapl_id: hid_t): hid_t

.. function:: proc H5Gopen2(loc_id: hid_t, name: c_string, gapl_id: hid_t): hid_t

.. function:: proc H5Gget_create_plist(group_id: hid_t): hid_t

.. function:: proc H5Gget_info(loc_id: hid_t, ref ginfo: H5G_info_t): herr_t

.. function:: proc H5Gget_info_by_name(loc_id: hid_t, name: c_string, ref ginfo: H5G_info_t, lapl_id: hid_t): herr_t

.. function:: proc H5Gget_info_by_idx(loc_id: hid_t, group_name: c_string, idx_type: H5_index_t, order: H5_iter_order_t, n: hsize_t, ref ginfo: H5G_info_t, lapl_id: hid_t): herr_t

.. function:: proc H5Gclose(group_id: hid_t): herr_t

.. function:: proc H5Gflush(group_id: hid_t): herr_t

.. function:: proc H5Grefresh(group_id: hid_t): herr_t

.. function:: proc H5Gcreate1(loc_id: hid_t, name: c_string, size_hint: size_t): hid_t

.. function:: proc H5Gopen1(loc_id: hid_t, name: c_string): hid_t

.. function:: proc H5Glink(cur_loc_id: hid_t, type_arg: H5L_type_t, cur_name: c_string, new_name: c_string): herr_t

.. function:: proc H5Glink2(cur_loc_id: hid_t, cur_name: c_string, type_arg: H5L_type_t, new_loc_id: hid_t, new_name: c_string): herr_t

.. function:: proc H5Gmove(src_loc_id: hid_t, src_name: c_string, dst_name: c_string): herr_t

.. function:: proc H5Gmove2(src_loc_id: hid_t, src_name: c_string, dst_loc_id: hid_t, dst_name: c_string): herr_t

.. function:: proc H5Gunlink(loc_id: hid_t, name: c_string): herr_t

.. function:: proc H5Gget_linkval(loc_id: hid_t, name: c_string, size: size_t, buf: c_string): herr_t

.. function:: proc H5Gset_comment(loc_id: hid_t, name: c_string, comment: c_string): herr_t

.. function:: proc H5Gget_comment(loc_id: hid_t, name: c_string, bufsize: size_t, buf: c_string): c_int

.. function:: proc H5Giterate(loc_id: hid_t, name: c_string, ref idx: c_int, op: H5G_iterate_t, op_data: c_void_ptr): herr_t

.. function:: proc H5Gget_num_objs(loc_id: hid_t, ref num_objs: hsize_t): herr_t

.. function:: proc H5Gget_objinfo(loc_id: hid_t, name: c_string, follow_link: hbool_t, ref statbuf: H5G_stat_t): herr_t

.. function:: proc H5Gget_objname_by_idx(loc_id: hid_t, idx: hsize_t, name: c_string, size: size_t): ssize_t

.. function:: proc H5Gget_objtype_by_idx(loc_id: hid_t, idx: hsize_t): H5G_obj_t

.. function:: proc H5Zregister(cls: c_void_ptr): herr_t

.. function:: proc H5Zunregister(id: H5Z_filter_t): herr_t

.. function:: proc H5Zfilter_avail(id: H5Z_filter_t): htri_t

.. function:: proc H5Zget_filter_info(filter: H5Z_filter_t, ref filter_config_flags: c_uint): herr_t

.. data:: var H5P_CLS_ROOT_ID_g: hid_t

.. data:: var H5P_CLS_OBJECT_CREATE_ID_g: hid_t

.. data:: var H5P_CLS_FILE_CREATE_ID_g: hid_t

.. data:: var H5P_CLS_FILE_ACCESS_ID_g: hid_t

.. data:: var H5P_CLS_DATASET_CREATE_ID_g: hid_t

.. data:: var H5P_CLS_DATASET_ACCESS_ID_g: hid_t

.. data:: var H5P_CLS_DATASET_XFER_ID_g: hid_t

.. data:: var H5P_CLS_FILE_MOUNT_ID_g: hid_t

.. data:: var H5P_CLS_GROUP_CREATE_ID_g: hid_t

.. data:: var H5P_CLS_GROUP_ACCESS_ID_g: hid_t

.. data:: var H5P_CLS_DATATYPE_CREATE_ID_g: hid_t

.. data:: var H5P_CLS_DATATYPE_ACCESS_ID_g: hid_t

.. data:: var H5P_CLS_STRING_CREATE_ID_g: hid_t

.. data:: var H5P_CLS_ATTRIBUTE_CREATE_ID_g: hid_t

.. data:: var H5P_CLS_ATTRIBUTE_ACCESS_ID_g: hid_t

.. data:: var H5P_CLS_OBJECT_COPY_ID_g: hid_t

.. data:: var H5P_CLS_LINK_CREATE_ID_g: hid_t

.. data:: var H5P_CLS_LINK_ACCESS_ID_g: hid_t

.. data:: var H5P_LST_FILE_CREATE_ID_g: hid_t

.. data:: var H5P_LST_FILE_ACCESS_ID_g: hid_t

.. data:: var H5P_LST_DATASET_CREATE_ID_g: hid_t

.. data:: var H5P_LST_DATASET_ACCESS_ID_g: hid_t

.. data:: var H5P_LST_DATASET_XFER_ID_g: hid_t

.. data:: var H5P_LST_FILE_MOUNT_ID_g: hid_t

.. data:: var H5P_LST_GROUP_CREATE_ID_g: hid_t

.. data:: var H5P_LST_GROUP_ACCESS_ID_g: hid_t

.. data:: var H5P_LST_DATATYPE_CREATE_ID_g: hid_t

.. data:: var H5P_LST_DATATYPE_ACCESS_ID_g: hid_t

.. data:: var H5P_LST_ATTRIBUTE_CREATE_ID_g: hid_t

.. data:: var H5P_LST_ATTRIBUTE_ACCESS_ID_g: hid_t

.. data:: var H5P_LST_OBJECT_COPY_ID_g: hid_t

.. data:: var H5P_LST_LINK_CREATE_ID_g: hid_t

.. data:: var H5P_LST_LINK_ACCESS_ID_g: hid_t

.. function:: proc H5Pcreate_class(parent: hid_t, name: c_string, cls_create: H5P_cls_create_func_t, create_data: c_void_ptr, cls_copy: H5P_cls_copy_func_t, copy_data: c_void_ptr, cls_close: H5P_cls_close_func_t, close_data: c_void_ptr): hid_t

.. function:: proc H5Pget_class_name(pclass_id: hid_t): c_string

.. function:: proc H5Pcreate(cls_id: hid_t): hid_t

.. function:: proc H5Pregister2(cls_id: hid_t, name: c_string, size: size_t, def_value: c_void_ptr, prp_create: H5P_prp_create_func_t, prp_set: H5P_prp_set_func_t, prp_get: H5P_prp_get_func_t, prp_del: H5P_prp_delete_func_t, prp_copy: H5P_prp_copy_func_t, prp_cmp: H5P_prp_compare_func_t, prp_close: H5P_prp_close_func_t): herr_t

.. function:: proc H5Pinsert2(plist_id: hid_t, name: c_string, size: size_t, value: c_void_ptr, prp_set: H5P_prp_set_func_t, prp_get: H5P_prp_get_func_t, prp_delete: H5P_prp_delete_func_t, prp_copy: H5P_prp_copy_func_t, prp_cmp: H5P_prp_compare_func_t, prp_close: H5P_prp_close_func_t): herr_t

.. function:: proc H5Pset(plist_id: hid_t, name: c_string, value: c_void_ptr): herr_t

.. function:: proc H5Pexist(plist_id: hid_t, name: c_string): htri_t

.. function:: proc H5Pencode(plist_id: hid_t, buf: c_void_ptr, ref nalloc: size_t): herr_t

.. function:: proc H5Pdecode(buf: c_void_ptr): hid_t

.. function:: proc H5Pget_size(id: hid_t, name: c_string, ref size: size_t): herr_t

.. function:: proc H5Pget_nprops(id: hid_t, ref nprops: size_t): herr_t

.. function:: proc H5Pget_class(plist_id: hid_t): hid_t

.. function:: proc H5Pget_class_parent(pclass_id: hid_t): hid_t

.. function:: proc H5Pget(plist_id: hid_t, name: c_string, value: c_void_ptr): herr_t

.. function:: proc H5Pequal(id1: hid_t, id2: hid_t): htri_t

.. function:: proc H5Pisa_class(plist_id: hid_t, pclass_id: hid_t): htri_t

.. function:: proc H5Piterate(id: hid_t, ref idx: c_int, iter_func: H5P_iterate_t, iter_data: c_void_ptr): c_int

.. function:: proc H5Pcopy_prop(dst_id: hid_t, src_id: hid_t, name: c_string): herr_t

.. function:: proc H5Premove(plist_id: hid_t, name: c_string): herr_t

.. function:: proc H5Punregister(pclass_id: hid_t, name: c_string): herr_t

.. function:: proc H5Pclose_class(plist_id: hid_t): herr_t

.. function:: proc H5Pclose(plist_id: hid_t): herr_t

.. function:: proc H5Pcopy(plist_id: hid_t): hid_t

.. function:: proc H5Pset_attr_phase_change(plist_id: hid_t, max_compact: c_uint, min_dense: c_uint): herr_t

.. function:: proc H5Pget_attr_phase_change(plist_id: hid_t, ref max_compact: c_uint, ref min_dense: c_uint): herr_t

.. function:: proc H5Pset_attr_creation_order(plist_id: hid_t, crt_order_flags: c_uint): herr_t

.. function:: proc H5Pget_attr_creation_order(plist_id: hid_t, ref crt_order_flags: c_uint): herr_t

.. function:: proc H5Pset_obj_track_times(plist_id: hid_t, track_times: hbool_t): herr_t

.. function:: proc H5Pget_obj_track_times(plist_id: hid_t, ref track_times: hbool_t): herr_t

.. function:: proc H5Pmodify_filter(plist_id: hid_t, filter: H5Z_filter_t, flags: c_uint, cd_nelmts: size_t, cd_values: c_ptr(c_uint)): herr_t

.. function:: proc H5Pset_filter(plist_id: hid_t, filter: H5Z_filter_t, flags: c_uint, cd_nelmts: size_t, c_values: c_ptr(c_uint)): herr_t

.. function:: proc H5Pget_nfilters(plist_id: hid_t): c_int

.. function:: proc H5Pget_filter2(plist_id: hid_t, filter: c_uint, ref flags: c_uint, ref cd_nelmts: size_t, cd_values: c_ptr(c_uint), namelen: size_t, name: c_ptr(c_char), ref filter_config: c_uint): H5Z_filter_t

.. function:: proc H5Pget_filter_by_id2(plist_id: hid_t, id: H5Z_filter_t, ref flags: c_uint, ref cd_nelmts: size_t, cd_values: c_ptr(c_uint), namelen: size_t, name: c_ptr(c_char), ref filter_config: c_uint): herr_t

.. function:: proc H5Pall_filters_avail(plist_id: hid_t): htri_t

.. function:: proc H5Premove_filter(plist_id: hid_t, filter: H5Z_filter_t): herr_t

.. function:: proc H5Pset_deflate(plist_id: hid_t, aggression: c_uint): herr_t

.. function:: proc H5Pset_fletcher32(plist_id: hid_t): herr_t

.. function:: proc H5Pset_userblock(plist_id: hid_t, size: hsize_t): herr_t

.. function:: proc H5Pget_userblock(plist_id: hid_t, ref size: hsize_t): herr_t

.. function:: proc H5Pset_sizes(plist_id: hid_t, sizeof_addr: size_t, sizeof_size: size_t): herr_t

.. function:: proc H5Pget_sizes(plist_id: hid_t, ref sizeof_addr: size_t, ref sizeof_size: size_t): herr_t

.. function:: proc H5Pset_sym_k(plist_id: hid_t, ik: c_uint, lk: c_uint): herr_t

.. function:: proc H5Pget_sym_k(plist_id: hid_t, ref ik: c_uint, ref lk: c_uint): herr_t

.. function:: proc H5Pset_istore_k(plist_id: hid_t, ik: c_uint): herr_t

.. function:: proc H5Pget_istore_k(plist_id: hid_t, ref ik: c_uint): herr_t

.. function:: proc H5Pset_shared_mesg_nindexes(plist_id: hid_t, nindexes: c_uint): herr_t

.. function:: proc H5Pget_shared_mesg_nindexes(plist_id: hid_t, ref nindexes: c_uint): herr_t

.. function:: proc H5Pset_shared_mesg_index(plist_id: hid_t, index_num: c_uint, mesg_type_flags: c_uint, min_mesg_size: c_uint): herr_t

.. function:: proc H5Pget_shared_mesg_index(plist_id: hid_t, index_num: c_uint, ref mesg_type_flags: c_uint, ref min_mesg_size: c_uint): herr_t

.. function:: proc H5Pset_shared_mesg_phase_change(plist_id: hid_t, max_list: c_uint, min_btree: c_uint): herr_t

.. function:: proc H5Pget_shared_mesg_phase_change(plist_id: hid_t, ref max_list: c_uint, ref min_btree: c_uint): herr_t

.. function:: proc H5Pset_file_space_strategy(plist_id: hid_t, strategy: H5F_fspace_strategy_t, persist: hbool_t, threshold: hsize_t): herr_t

.. function:: proc H5Pget_file_space_strategy(plist_id: hid_t, ref strategy: H5F_fspace_strategy_t, ref persist: hbool_t, ref threshold: hsize_t): herr_t

.. function:: proc H5Pset_file_space_page_size(plist_id: hid_t, fsp_size: hsize_t): herr_t

.. function:: proc H5Pget_file_space_page_size(plist_id: hid_t, ref fsp_size: hsize_t): herr_t

.. function:: proc H5Pset_alignment(fapl_id: hid_t, threshold: hsize_t, alignment: hsize_t): herr_t

.. function:: proc H5Pget_alignment(fapl_id: hid_t, ref threshold: hsize_t, ref alignment: hsize_t): herr_t

.. function:: proc H5Pset_driver(plist_id: hid_t, driver_id: hid_t, driver_info: c_void_ptr): herr_t

.. function:: proc H5Pget_driver(plist_id: hid_t): hid_t

.. function:: proc H5Pget_driver_info(plist_id: hid_t): c_void_ptr

.. function:: proc H5Pset_family_offset(fapl_id: hid_t, offset: hsize_t): herr_t

.. function:: proc H5Pget_family_offset(fapl_id: hid_t, ref offset: hsize_t): herr_t

.. function:: proc H5Pset_multi_type(fapl_id: hid_t, type_arg: H5FD_mem_t): herr_t

.. function:: proc H5Pget_multi_type(fapl_id: hid_t, ref type_arg: H5FD_mem_t): herr_t

.. function:: proc H5Pset_cache(plist_id: hid_t, mdc_nelmts: c_int, rdcc_nslots: size_t, rdcc_nbytes: size_t, rdcc_w0: c_double): herr_t

.. function:: proc H5Pget_cache(plist_id: hid_t, ref mdc_nelmts: c_int, ref rdcc_nslots: size_t, ref rdcc_nbytes: size_t, ref rdcc_w0: c_double): herr_t

.. function:: proc H5Pset_mdc_config(plist_id: hid_t, ref config_ptr: H5AC_cache_config_t): herr_t

.. function:: proc H5Pget_mdc_config(plist_id: hid_t, ref config_ptr: H5AC_cache_config_t): herr_t

.. function:: proc H5Pset_gc_references(fapl_id: hid_t, gc_ref: c_uint): herr_t

.. function:: proc H5Pget_gc_references(fapl_id: hid_t, ref gc_ref: c_uint): herr_t

.. function:: proc H5Pset_fclose_degree(fapl_id: hid_t, degree: H5F_close_degree_t): herr_t

.. function:: proc H5Pget_fclose_degree(fapl_id: hid_t, ref degree: H5F_close_degree_t): herr_t

.. function:: proc H5Pset_meta_block_size(fapl_id: hid_t, size: hsize_t): herr_t

.. function:: proc H5Pget_meta_block_size(fapl_id: hid_t, ref size: hsize_t): herr_t

.. function:: proc H5Pset_sieve_buf_size(fapl_id: hid_t, size: size_t): herr_t

.. function:: proc H5Pget_sieve_buf_size(fapl_id: hid_t, ref size: size_t): herr_t

.. function:: proc H5Pset_small_data_block_size(fapl_id: hid_t, size: hsize_t): herr_t

.. function:: proc H5Pget_small_data_block_size(fapl_id: hid_t, ref size: hsize_t): herr_t

.. function:: proc H5Pset_libver_bounds(plist_id: hid_t, low: H5F_libver_t, high: H5F_libver_t): herr_t

.. function:: proc H5Pget_libver_bounds(plist_id: hid_t, ref low: H5F_libver_t, ref high: H5F_libver_t): herr_t

.. function:: proc H5Pset_elink_file_cache_size(plist_id: hid_t, efc_size: c_uint): herr_t

.. function:: proc H5Pget_elink_file_cache_size(plist_id: hid_t, ref efc_size: c_uint): herr_t

.. function:: proc H5Pset_file_image(fapl_id: hid_t, buf_ptr: c_void_ptr, buf_len: size_t): herr_t

.. function:: proc H5Pget_file_image(fapl_id: hid_t, ref buf_ptr_ptr: c_void_ptr, ref buf_len_ptr: size_t): herr_t

.. function:: proc H5Pset_file_image_callbacks(fapl_id: hid_t, ref callbacks_ptr: H5FD_file_image_callbacks_t): herr_t

.. function:: proc H5Pget_file_image_callbacks(fapl_id: hid_t, ref callbacks_ptr: H5FD_file_image_callbacks_t): herr_t

.. function:: proc H5Pset_core_write_tracking(fapl_id: hid_t, is_enabled: hbool_t, page_size: size_t): herr_t

.. function:: proc H5Pget_core_write_tracking(fapl_id: hid_t, ref is_enabled: hbool_t, ref page_size: size_t): herr_t

.. function:: proc H5Pset_metadata_read_attempts(plist_id: hid_t, attempts: c_uint): herr_t

.. function:: proc H5Pget_metadata_read_attempts(plist_id: hid_t, ref attempts: c_uint): herr_t

.. function:: proc H5Pset_object_flush_cb(plist_id: hid_t, func: H5F_flush_cb_t, udata: c_void_ptr): herr_t

.. function:: proc H5Pget_object_flush_cb(plist_id: hid_t, ref func: H5F_flush_cb_t, ref udata: c_void_ptr): herr_t

.. function:: proc H5Pset_mdc_log_options(plist_id: hid_t, is_enabled: hbool_t, location: c_string, start_on_access: hbool_t): herr_t

.. function:: proc H5Pget_mdc_log_options(plist_id: hid_t, ref is_enabled: hbool_t, location: c_string, ref location_size: size_t, ref start_on_access: hbool_t): herr_t

.. function:: proc H5Pset_evict_on_close(fapl_id: hid_t, evict_on_close: hbool_t): herr_t

.. function:: proc H5Pget_evict_on_close(fapl_id: hid_t, ref evict_on_close: hbool_t): herr_t

.. function:: proc H5Pset_mdc_image_config(plist_id: hid_t, ref config_ptr: H5AC_cache_image_config_t): herr_t

.. function:: proc H5Pget_mdc_image_config(plist_id: hid_t, ref config_ptr: H5AC_cache_image_config_t): herr_t

.. function:: proc H5Pset_page_buffer_size(plist_id: hid_t, buf_size: size_t, min_meta_per: c_uint, min_raw_per: c_uint): herr_t

.. function:: proc H5Pget_page_buffer_size(plist_id: hid_t, ref buf_size: size_t, ref min_meta_per: c_uint, ref min_raw_per: c_uint): herr_t

.. function:: proc H5Pset_layout(plist_id: hid_t, layout: H5D_layout_t): herr_t

.. function:: proc H5Pget_layout(plist_id: hid_t): H5D_layout_t

.. function:: proc H5Pset_chunk(plist_id: hid_t, ndims: c_int, dim: c_ptr(hsize_t)): herr_t

.. function:: proc H5Pget_chunk(plist_id: hid_t, max_ndims: c_int, dim: c_ptr(hsize_t)): c_int

.. function:: proc H5Pset_virtual(dcpl_id: hid_t, vspace_id: hid_t, src_file_name: c_string, src_dset_name: c_string, src_space_id: hid_t): herr_t

.. function:: proc H5Pget_virtual_count(dcpl_id: hid_t, ref count: size_t): herr_t

.. function:: proc H5Pget_virtual_vspace(dcpl_id: hid_t, index_arg: size_t): hid_t

.. function:: proc H5Pget_virtual_srcspace(dcpl_id: hid_t, index_arg: size_t): hid_t

.. function:: proc H5Pget_virtual_filename(dcpl_id: hid_t, index_arg: size_t, name: c_string, size: size_t): ssize_t

.. function:: proc H5Pget_virtual_dsetname(dcpl_id: hid_t, index_arg: size_t, name: c_string, size: size_t): ssize_t

.. function:: proc H5Pset_external(plist_id: hid_t, name: c_string, offset: off_t, size: hsize_t): herr_t

.. function:: proc H5Pset_chunk_opts(plist_id: hid_t, opts: c_uint): herr_t

.. function:: proc H5Pget_chunk_opts(plist_id: hid_t, ref opts: c_uint): herr_t

.. function:: proc H5Pget_external_count(plist_id: hid_t): c_int

.. function:: proc H5Pget_external(plist_id: hid_t, idx: c_uint, name_size: size_t, name: c_string, ref offset: off_t, ref size: hsize_t): herr_t

.. function:: proc H5Pset_szip(plist_id: hid_t, options_mask: c_uint, pixels_per_block: c_uint): herr_t

.. function:: proc H5Pset_shuffle(plist_id: hid_t): herr_t

.. function:: proc H5Pset_nbit(plist_id: hid_t): herr_t

.. function:: proc H5Pset_scaleoffset(plist_id: hid_t, scale_type: H5Z_SO_scale_type_t, scale_factor: c_int): herr_t

.. function:: proc H5Pset_fill_value(plist_id: hid_t, type_id: hid_t, value: c_void_ptr): herr_t

.. function:: proc H5Pget_fill_value(plist_id: hid_t, type_id: hid_t, value: c_void_ptr): herr_t

.. function:: proc H5Pfill_value_defined(plist: hid_t, ref status: H5D_fill_value_t): herr_t

.. function:: proc H5Pset_alloc_time(plist_id: hid_t, alloc_time: H5D_alloc_time_t): herr_t

.. function:: proc H5Pget_alloc_time(plist_id: hid_t, ref alloc_time: H5D_alloc_time_t): herr_t

.. function:: proc H5Pset_fill_time(plist_id: hid_t, fill_time: H5D_fill_time_t): herr_t

.. function:: proc H5Pget_fill_time(plist_id: hid_t, ref fill_time: H5D_fill_time_t): herr_t

.. function:: proc H5Pset_chunk_cache(dapl_id: hid_t, rdcc_nslots: size_t, rdcc_nbytes: size_t, rdcc_w0: c_double): herr_t

.. function:: proc H5Pget_chunk_cache(dapl_id: hid_t, ref rdcc_nslots: size_t, ref rdcc_nbytes: size_t, ref rdcc_w0: c_double): herr_t

.. function:: proc H5Pset_virtual_view(plist_id: hid_t, view: H5D_vds_view_t): herr_t

.. function:: proc H5Pget_virtual_view(plist_id: hid_t, ref view: H5D_vds_view_t): herr_t

.. function:: proc H5Pset_virtual_printf_gap(plist_id: hid_t, gap_size: hsize_t): herr_t

.. function:: proc H5Pget_virtual_printf_gap(plist_id: hid_t, ref gap_size: hsize_t): herr_t

.. function:: proc H5Pset_append_flush(plist_id: hid_t, ndims: c_uint, boundary: c_ptr(hsize_t), func: H5D_append_cb_t, udata: c_void_ptr): herr_t

.. function:: proc H5Pget_append_flush(plist_id: hid_t, dims: c_uint, boundary: c_ptr(hsize_t), ref func: H5D_append_cb_t, ref udata: c_void_ptr): herr_t

.. function:: proc H5Pset_efile_prefix(dapl_id: hid_t, prefix: c_string): herr_t

.. function:: proc H5Pget_efile_prefix(dapl_id: hid_t, prefix: c_string, size: size_t): ssize_t

.. function:: proc H5Pset_data_transform(plist_id: hid_t, expression: c_string): herr_t

.. function:: proc H5Pget_data_transform(plist_id: hid_t, expression: c_string, size: size_t): ssize_t

.. function:: proc H5Pset_buffer(plist_id: hid_t, size: size_t, tconv: c_void_ptr, bkg: c_void_ptr): herr_t

.. function:: proc H5Pget_buffer(plist_id: hid_t, ref tconv: c_void_ptr, ref bkg: c_void_ptr): size_t

.. function:: proc H5Pset_preserve(plist_id: hid_t, status: hbool_t): herr_t

.. function:: proc H5Pget_preserve(plist_id: hid_t): c_int

.. function:: proc H5Pset_edc_check(plist_id: hid_t, check: H5Z_EDC_t): herr_t

.. function:: proc H5Pget_edc_check(plist_id: hid_t): H5Z_EDC_t

.. function:: proc H5Pset_filter_callback(plist_id: hid_t, func: H5Z_filter_func_t, op_data: c_void_ptr): herr_t

.. function:: proc H5Pset_btree_ratios(plist_id: hid_t, left: c_double, middle: c_double, right: c_double): herr_t

.. function:: proc H5Pget_btree_ratios(plist_id: hid_t, ref left: c_double, ref middle: c_double, ref right: c_double): herr_t

.. function:: proc H5Pset_vlen_mem_manager(plist_id: hid_t, alloc_func: H5MM_allocate_t, alloc_info: c_void_ptr, free_func: H5MM_free_t, free_info: c_void_ptr): herr_t

.. function:: proc H5Pget_vlen_mem_manager(plist_id: hid_t, ref alloc_func: H5MM_allocate_t, ref alloc_info: c_void_ptr, ref free_func: H5MM_free_t, ref free_info: c_void_ptr): herr_t

.. function:: proc H5Pset_hyper_vector_size(fapl_id: hid_t, size: size_t): herr_t

.. function:: proc H5Pget_hyper_vector_size(fapl_id: hid_t, ref size: size_t): herr_t

.. function:: proc H5Pset_type_conv_cb(dxpl_id: hid_t, op: H5T_conv_except_func_t, operate_data: c_void_ptr): herr_t

.. function:: proc H5Pget_type_conv_cb(dxpl_id: hid_t, ref op: H5T_conv_except_func_t, ref operate_data: c_void_ptr): herr_t

.. function:: proc H5Pset_create_intermediate_group(plist_id: hid_t, crt_intmd: c_uint): herr_t

.. function:: proc H5Pget_create_intermediate_group(plist_id: hid_t, ref crt_intmd: c_uint): herr_t

.. function:: proc H5Pset_local_heap_size_hint(plist_id: hid_t, size_hint: size_t): herr_t

.. function:: proc H5Pget_local_heap_size_hint(plist_id: hid_t, ref size_hint: size_t): herr_t

.. function:: proc H5Pset_link_phase_change(plist_id: hid_t, max_compact: c_uint, min_dense: c_uint): herr_t

.. function:: proc H5Pget_link_phase_change(plist_id: hid_t, ref max_compact: c_uint, ref min_dense: c_uint): herr_t

.. function:: proc H5Pset_est_link_info(plist_id: hid_t, est_num_entries: c_uint, est_name_len: c_uint): herr_t

.. function:: proc H5Pget_est_link_info(plist_id: hid_t, ref est_num_entries: c_uint, ref est_name_len: c_uint): herr_t

.. function:: proc H5Pset_link_creation_order(plist_id: hid_t, crt_order_flags: c_uint): herr_t

.. function:: proc H5Pget_link_creation_order(plist_id: hid_t, ref crt_order_flags: c_uint): herr_t

.. function:: proc H5Pset_char_encoding(plist_id: hid_t, encoding: H5T_cset_t): herr_t

.. function:: proc H5Pget_char_encoding(plist_id: hid_t, ref encoding: H5T_cset_t): herr_t

.. function:: proc H5Pset_nlinks(plist_id: hid_t, nlinks: size_t): herr_t

.. function:: proc H5Pget_nlinks(plist_id: hid_t, ref nlinks: size_t): herr_t

.. function:: proc H5Pset_elink_prefix(plist_id: hid_t, prefix: c_string): herr_t

.. function:: proc H5Pget_elink_prefix(plist_id: hid_t, prefix: c_string, size: size_t): ssize_t

.. function:: proc H5Pget_elink_fapl(lapl_id: hid_t): hid_t

.. function:: proc H5Pset_elink_fapl(lapl_id: hid_t, fapl_id: hid_t): herr_t

.. function:: proc H5Pset_elink_acc_flags(lapl_id: hid_t, flags: c_uint): herr_t

.. function:: proc H5Pget_elink_acc_flags(lapl_id: hid_t, ref flags: c_uint): herr_t

.. function:: proc H5Pset_elink_cb(lapl_id: hid_t, func: H5L_elink_traverse_t, op_data: c_void_ptr): herr_t

.. function:: proc H5Pget_elink_cb(lapl_id: hid_t, ref func: H5L_elink_traverse_t, ref op_data: c_void_ptr): herr_t

.. function:: proc H5Pset_copy_object(plist_id: hid_t, crt_intmd: c_uint): herr_t

.. function:: proc H5Pget_copy_object(plist_id: hid_t, ref crt_intmd: c_uint): herr_t

.. function:: proc H5Padd_merge_committed_dtype_path(plist_id: hid_t, path: c_string): herr_t

.. function:: proc H5Pfree_merge_committed_dtype_paths(plist_id: hid_t): herr_t

.. function:: proc H5Pset_mcdt_search_cb(plist_id: hid_t, func: H5O_mcdt_search_cb_t, op_data: c_void_ptr): herr_t

.. function:: proc H5Pget_mcdt_search_cb(plist_id: hid_t, ref func: H5O_mcdt_search_cb_t, ref op_data: c_void_ptr): herr_t

.. function:: proc H5Pregister1(cls_id: hid_t, name: c_string, size: size_t, def_value: c_void_ptr, prp_create: H5P_prp_create_func_t, prp_set: H5P_prp_set_func_t, prp_get: H5P_prp_get_func_t, prp_del: H5P_prp_delete_func_t, prp_copy: H5P_prp_copy_func_t, prp_close: H5P_prp_close_func_t): herr_t

.. function:: proc H5Pinsert1(plist_id: hid_t, name: c_string, size: size_t, value: c_void_ptr, prp_set: H5P_prp_set_func_t, prp_get: H5P_prp_get_func_t, prp_delete: H5P_prp_delete_func_t, prp_copy: H5P_prp_copy_func_t, prp_close: H5P_prp_close_func_t): herr_t

.. function:: proc H5Pget_filter1(plist_id: hid_t, filter: c_uint, ref flags: c_uint, ref cd_nelmts: size_t, cd_values: c_ptr(c_uint), namelen: size_t, name: c_ptr(c_char)): H5Z_filter_t

.. function:: proc H5Pget_filter_by_id1(plist_id: hid_t, id: H5Z_filter_t, ref flags: c_uint, ref cd_nelmts: size_t, cd_values: c_ptr(c_uint), namelen: size_t, name: c_ptr(c_char)): herr_t

.. function:: proc H5Pget_version(plist_id: hid_t, ref boot: c_uint, ref freelist: c_uint, ref stab: c_uint, ref shhdr: c_uint): herr_t

.. function:: proc H5Pset_file_space(plist_id: hid_t, strategy: H5F_file_space_type_t, threshold: hsize_t): herr_t

.. function:: proc H5Pget_file_space(plist_id: hid_t, ref strategy: H5F_file_space_type_t, ref threshold: hsize_t): herr_t

.. function:: proc H5PLset_loading_state(plugin_type: c_uint): herr_t

.. function:: proc H5PLget_loading_state(ref plugin_type: c_uint): herr_t

.. function:: proc H5PLappend(plugin_path: c_string): herr_t

.. function:: proc H5PLprepend(plugin_path: c_string): herr_t

.. function:: proc H5PLreplace(plugin_path: c_string, index_arg: c_uint): herr_t

.. function:: proc H5PLinsert(plugin_path: c_string, index_arg: c_uint): herr_t

.. function:: proc H5PLremove(index_arg: c_uint): herr_t

.. function:: proc H5PLget(index_arg: c_uint, pathname: c_string, size: size_t): ssize_t

.. function:: proc H5PLsize(ref listsize: c_uint): herr_t

.. function:: proc H5Rcreate(ref_arg: c_void_ptr, loc_id: hid_t, name: c_string, ref_type: H5R_type_t, space_id: hid_t): herr_t

.. function:: proc H5Rdereference2(obj_id: hid_t, oapl_id: hid_t, ref_type: H5R_type_t, ref_arg: c_void_ptr): hid_t

.. function:: proc H5Rget_region(dataset: hid_t, ref_type: H5R_type_t, ref_arg: c_void_ptr): hid_t

.. function:: proc H5Rget_obj_type2(id: hid_t, ref_type: H5R_type_t, _ref: c_void_ptr, ref obj_type: H5O_type_t): herr_t

.. function:: proc H5Rget_name(loc_id: hid_t, ref_type: H5R_type_t, ref_arg: c_void_ptr, name: c_string, size: size_t): ssize_t

.. function:: proc H5Rget_obj_type1(id: hid_t, ref_type: H5R_type_t, _ref: c_void_ptr): H5G_obj_t

.. function:: proc H5Rdereference1(obj_id: hid_t, ref_type: H5R_type_t, ref_arg: c_void_ptr): hid_t

.. function:: proc H5Screate(type_arg: H5S_class_t): hid_t

.. function:: proc H5Screate_simple(rank: c_int, dims: c_ptr(hsize_t), maxdims: c_ptr(hsize_t)): hid_t

.. function:: proc H5Sset_extent_simple(space_id: hid_t, rank: c_int, dims: c_ptr(hsize_t), max: c_ptr(hsize_t)): herr_t

.. function:: proc H5Scopy(space_id: hid_t): hid_t

.. function:: proc H5Sclose(space_id: hid_t): herr_t

.. function:: proc H5Sencode(obj_id: hid_t, buf: c_void_ptr, ref nalloc: size_t): herr_t

.. function:: proc H5Sdecode(buf: c_void_ptr): hid_t

.. function:: proc H5Sget_simple_extent_npoints(space_id: hid_t): hssize_t

.. function:: proc H5Sget_simple_extent_ndims(space_id: hid_t): c_int

.. function:: proc H5Sget_simple_extent_dims(space_id: hid_t, dims: c_ptr(hsize_t), maxdims: c_ptr(hsize_t)): c_int

.. function:: proc H5Sis_simple(space_id: hid_t): htri_t

.. function:: proc H5Sget_select_npoints(spaceid: hid_t): hssize_t

.. function:: proc H5Sselect_hyperslab(space_id: hid_t, op: H5S_seloper_t, start: c_ptr(hsize_t), _stride: c_ptr(hsize_t), count: c_ptr(hsize_t), _block: c_ptr(hsize_t)): herr_t

.. function:: proc H5Sselect_elements(space_id: hid_t, op: H5S_seloper_t, num_elem: size_t, ref coord: hsize_t): herr_t

.. function:: proc H5Sget_simple_extent_type(space_id: hid_t): H5S_class_t

.. function:: proc H5Sset_extent_none(space_id: hid_t): herr_t

.. function:: proc H5Sextent_copy(dst_id: hid_t, src_id: hid_t): herr_t

.. function:: proc H5Sextent_equal(sid1: hid_t, sid2: hid_t): htri_t

.. function:: proc H5Sselect_all(spaceid: hid_t): herr_t

.. function:: proc H5Sselect_none(spaceid: hid_t): herr_t

.. function:: proc H5Soffset_simple(space_id: hid_t, ref offset: hssize_t): herr_t

.. function:: proc H5Sselect_valid(spaceid: hid_t): htri_t

.. function:: proc H5Sis_regular_hyperslab(spaceid: hid_t): htri_t

.. function:: proc H5Sget_regular_hyperslab(spaceid: hid_t, start: c_ptr(hsize_t), stride: c_ptr(hsize_t), count: c_ptr(hsize_t), block: c_ptr(hsize_t)): htri_t

.. function:: proc H5Sget_select_hyper_nblocks(spaceid: hid_t): hssize_t

.. function:: proc H5Sget_select_elem_npoints(spaceid: hid_t): hssize_t

.. function:: proc H5Sget_select_hyper_blocklist(spaceid: hid_t, startblock: hsize_t, numblocks: hsize_t, buf: c_ptr(hsize_t)): herr_t

.. function:: proc H5Sget_select_elem_pointlist(spaceid: hid_t, startpoint: hsize_t, numpoints: hsize_t, buf: c_ptr(hsize_t)): herr_t

.. function:: proc H5Sget_select_bounds(spaceid: hid_t, start: c_ptr(hsize_t), end: c_ptr(hsize_t)): herr_t

.. function:: proc H5Sget_select_type(spaceid: hid_t): H5S_sel_type

.. function:: proc H5FD_core_init(): hid_t

.. function:: proc H5Pset_fapl_core(fapl_id: hid_t, increment: size_t, backing_store: hbool_t): herr_t

.. function:: proc H5Pget_fapl_core(fapl_id: hid_t, ref increment: size_t, ref backing_store: hbool_t): herr_t

.. function:: proc H5FD_family_init(): hid_t

.. function:: proc H5Pset_fapl_family(fapl_id: hid_t, memb_size: hsize_t, memb_fapl_id: hid_t): herr_t

.. function:: proc H5Pget_fapl_family(fapl_id: hid_t, ref memb_size: hsize_t, ref memb_fapl_id: hid_t): herr_t

.. function:: proc H5FD_log_init(): hid_t

.. function:: proc H5Pset_fapl_log(fapl_id: hid_t, logfile: c_string, flags: c_ulonglong, buf_size: size_t): herr_t

.. function:: proc H5FD_multi_init(): hid_t

.. function:: proc H5Pset_fapl_multi(fapl_id: hid_t, ref memb_map: H5FD_mem_t, ref memb_fapl: hid_t, ref memb_name: c_string, ref memb_addr: haddr_t, relax: hbool_t): herr_t

.. function:: proc H5Pget_fapl_multi(fapl_id: hid_t, ref memb_map: H5FD_mem_t, ref memb_fapl: hid_t, ref memb_name: c_string, ref memb_addr: haddr_t, ref relax: hbool_t): herr_t

.. function:: proc H5Pset_fapl_split(fapl: hid_t, meta_ext: c_string, meta_plist_id: hid_t, raw_ext: c_string, raw_plist_id: hid_t): herr_t

.. function:: proc H5FD_sec2_init(): hid_t

.. function:: proc H5Pset_fapl_sec2(fapl_id: hid_t): herr_t

.. function:: proc H5FD_stdio_init(): hid_t

.. function:: proc H5Pset_fapl_stdio(fapl_id: hid_t): herr_t

.. function:: proc H5DOwrite_chunk(dset_id: hid_t, dxpl_id: hid_t, filters: uint(32), ref offset: hsize_t, data_size: size_t, buf: c_void_ptr): herr_t

.. function:: proc H5DOappend(dset_id: hid_t, dxpl_id: hid_t, axis: c_uint, extension: size_t, memtype: hid_t, buf: c_void_ptr): herr_t

.. function:: proc H5DSattach_scale(did: hid_t, dsid: hid_t, idx: c_uint): herr_t

.. function:: proc H5DSdetach_scale(did: hid_t, dsid: hid_t, idx: c_uint): herr_t

.. function:: proc H5DSset_scale(dsid: hid_t, dimname: c_string): herr_t

.. function:: proc H5DSget_num_scales(did: hid_t, dim: c_uint): c_int

.. function:: proc H5DSset_label(did: hid_t, idx: c_uint, label_arg: c_string): herr_t

.. function:: proc H5DSget_label(did: hid_t, idx: c_uint, label_arg: c_string, size: size_t): ssize_t

.. function:: proc H5DSget_scale_name(did: hid_t, name: c_string, size: size_t): ssize_t

.. function:: proc H5DSis_scale(did: hid_t): htri_t

.. function:: proc H5DSiterate_scales(did: hid_t, dim: c_uint, ref idx: c_int, visitor: H5DS_iterate_t, visitor_data: c_void_ptr): herr_t

.. function:: proc H5DSis_attached(did: hid_t, dsid: hid_t, idx: c_uint): htri_t

.. function:: proc H5LTmake_dataset(loc_id: hid_t, dset_name: c_string, rank: c_int, ref dims: hsize_t, type_id: hid_t, buffer: c_void_ptr): herr_t

.. function:: proc H5LTmake_dataset_char(loc_id: hid_t, dset_name: c_string, rank: c_int, ref dims: hsize_t, buffer: c_string): herr_t

.. function:: proc H5LTmake_dataset_short(loc_id: hid_t, dset_name: c_string, rank: c_int, ref dims: hsize_t, ref buffer: c_short): herr_t

.. function:: proc H5LTmake_dataset_int(loc_id: hid_t, dset_name: c_string, rank: c_int, ref dims: hsize_t, ref buffer: c_int): herr_t

.. function:: proc H5LTmake_dataset_long(loc_id: hid_t, dset_name: c_string, rank: c_int, ref dims: hsize_t, ref buffer: c_long): herr_t

.. function:: proc H5LTmake_dataset_float(loc_id: hid_t, dset_name: c_string, rank: c_int, ref dims: hsize_t, ref buffer: c_float): herr_t

.. function:: proc H5LTmake_dataset_double(loc_id: hid_t, dset_name: c_string, rank: c_int, ref dims: hsize_t, ref buffer: c_double): herr_t

.. function:: proc H5LTmake_dataset_string(loc_id: hid_t, dset_name: c_string, buf: c_string): herr_t

.. function:: proc H5LTread_dataset(loc_id: hid_t, dset_name: c_string, type_id: hid_t, buffer: c_void_ptr): herr_t

.. function:: proc H5LTread_dataset_char(loc_id: hid_t, dset_name: c_string, buffer: c_string): herr_t

.. function:: proc H5LTread_dataset_short(loc_id: hid_t, dset_name: c_string, ref buffer: c_short): herr_t

.. function:: proc H5LTread_dataset_int(loc_id: hid_t, dset_name: c_string, ref buffer: c_int): herr_t

.. function:: proc H5LTread_dataset_long(loc_id: hid_t, dset_name: c_string, ref buffer: c_long): herr_t

.. function:: proc H5LTread_dataset_float(loc_id: hid_t, dset_name: c_string, ref buffer: c_float): herr_t

.. function:: proc H5LTread_dataset_double(loc_id: hid_t, dset_name: c_string, ref buffer: c_double): herr_t

.. function:: proc H5LTread_dataset_string(loc_id: hid_t, dset_name: c_string, buf: c_string): herr_t

.. function:: proc H5LTget_dataset_ndims(loc_id: hid_t, dset_name: c_string, ref rank: c_int): herr_t

.. function:: proc H5LTget_dataset_info(loc_id: hid_t, dset_name: c_string, ref dims: hsize_t, ref type_class: H5T_class_t, ref type_size: size_t): herr_t

.. function:: proc H5LTget_dataset_info(loc_id: hid_t, dset_name: c_string, ref dims: hsize_t, type_class: c_ptr(H5T_class_t), type_size: c_ptr(size_t)): herr_t

.. function:: proc H5LTfind_dataset(loc_id: hid_t, name: c_string): herr_t

.. function:: proc H5LTset_attribute_string(loc_id: hid_t, obj_name: c_string, attr_name: c_string, attr_data: c_string): herr_t

.. function:: proc H5LTset_attribute_char(loc_id: hid_t, obj_name: c_string, attr_name: c_string, buffer: c_string, size: size_t): herr_t

.. function:: proc H5LTset_attribute_uchar(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref buffer: c_uchar, size: size_t): herr_t

.. function:: proc H5LTset_attribute_short(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref buffer: c_short, size: size_t): herr_t

.. function:: proc H5LTset_attribute_ushort(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref buffer: c_ushort, size: size_t): herr_t

.. function:: proc H5LTset_attribute_int(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref buffer: c_int, size: size_t): herr_t

.. function:: proc H5LTset_attribute_uint(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref buffer: c_uint, size: size_t): herr_t

.. function:: proc H5LTset_attribute_long(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref buffer: c_long, size: size_t): herr_t

.. function:: proc H5LTset_attribute_long_long(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref buffer: c_longlong, size: size_t): herr_t

.. function:: proc H5LTset_attribute_ulong(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref buffer: c_ulong, size: size_t): herr_t

.. function:: proc H5LTset_attribute_float(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref buffer: c_float, size: size_t): herr_t

.. function:: proc H5LTset_attribute_double(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref buffer: c_double, size: size_t): herr_t

.. function:: proc H5LTget_attribute(loc_id: hid_t, obj_name: c_string, attr_name: c_string, mem_type_id: hid_t, data: c_void_ptr): herr_t

.. function:: proc H5LTget_attribute_string(loc_id: hid_t, obj_name: c_string, attr_name: c_string, data: c_string): herr_t

.. function:: proc H5LTget_attribute_char(loc_id: hid_t, obj_name: c_string, attr_name: c_string, data: c_string): herr_t

.. function:: proc H5LTget_attribute_uchar(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref data: c_uchar): herr_t

.. function:: proc H5LTget_attribute_short(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref data: c_short): herr_t

.. function:: proc H5LTget_attribute_ushort(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref data: c_ushort): herr_t

.. function:: proc H5LTget_attribute_int(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref data: c_int): herr_t

.. function:: proc H5LTget_attribute_uint(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref data: c_uint): herr_t

.. function:: proc H5LTget_attribute_long(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref data: c_long): herr_t

.. function:: proc H5LTget_attribute_long_long(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref data: c_longlong): herr_t

.. function:: proc H5LTget_attribute_ulong(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref data: c_ulong): herr_t

.. function:: proc H5LTget_attribute_float(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref data: c_float): herr_t

.. function:: proc H5LTget_attribute_double(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref data: c_double): herr_t

.. function:: proc H5LTget_attribute_ndims(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref rank: c_int): herr_t

.. function:: proc H5LTget_attribute_info(loc_id: hid_t, obj_name: c_string, attr_name: c_string, ref dims: hsize_t, ref type_class: H5T_class_t, ref type_size: size_t): herr_t

.. function:: proc H5LTtext_to_dtype(text: c_string, lang_type: H5LT_lang_t): hid_t

.. function:: proc H5LTdtype_to_text(dtype: hid_t, str: c_string, lang_type: H5LT_lang_t, ref len: size_t): herr_t

.. function:: proc H5LTfind_attribute(loc_id: hid_t, name: c_string): herr_t

.. function:: proc H5LTpath_valid(loc_id: hid_t, path: c_string, check_object_valid: hbool_t): htri_t

.. function:: proc H5LTopen_file_image(buf_ptr: c_void_ptr, buf_size: size_t, flags: c_uint): hid_t

.. function:: proc H5IMmake_image_8bit(loc_id: hid_t, dset_name: c_string, width: hsize_t, height: hsize_t, ref buffer: c_uchar): herr_t

.. function:: proc H5IMmake_image_24bit(loc_id: hid_t, dset_name: c_string, width: hsize_t, height: hsize_t, interlace: c_string, ref buffer: c_uchar): herr_t

.. function:: proc H5IMget_image_info(loc_id: hid_t, dset_name: c_string, ref width: hsize_t, ref height: hsize_t, ref planes: hsize_t, interlace: c_string, ref npals: hssize_t): herr_t

.. function:: proc H5IMread_image(loc_id: hid_t, dset_name: c_string, ref buffer: c_uchar): herr_t

.. function:: proc H5IMmake_palette(loc_id: hid_t, pal_name: c_string, ref pal_dims: hsize_t, ref pal_data: c_uchar): herr_t

.. function:: proc H5IMlink_palette(loc_id: hid_t, image_name: c_string, pal_name: c_string): herr_t

.. function:: proc H5IMunlink_palette(loc_id: hid_t, image_name: c_string, pal_name: c_string): herr_t

.. function:: proc H5IMget_npalettes(loc_id: hid_t, image_name: c_string, ref npals: hssize_t): herr_t

.. function:: proc H5IMget_palette_info(loc_id: hid_t, image_name: c_string, pal_number: c_int, ref pal_dims: hsize_t): herr_t

.. function:: proc H5IMget_palette(loc_id: hid_t, image_name: c_string, pal_number: c_int, ref pal_data: c_uchar): herr_t

.. function:: proc H5IMis_image(loc_id: hid_t, dset_name: c_string): herr_t

.. function:: proc H5IMis_palette(loc_id: hid_t, dset_name: c_string): herr_t

.. function:: proc H5TBmake_table(table_title: c_string, loc_id: hid_t, dset_name: c_string, nfields: hsize_t, nrecords: hsize_t, type_size: size_t, field_names: c_ptr(c_string), ref field_offset: size_t, ref field_types: hid_t, chunk_size: hsize_t, fill_data: c_void_ptr, compress: c_int, buf: c_void_ptr): herr_t

.. function:: proc H5TBappend_records(loc_id: hid_t, dset_name: c_string, nrecords: hsize_t, type_size: size_t, ref field_offset: size_t, ref dst_sizes: size_t, buf: c_void_ptr): herr_t

.. function:: proc H5TBwrite_records(loc_id: hid_t, dset_name: c_string, start: hsize_t, nrecords: hsize_t, type_size: size_t, ref field_offset: size_t, ref dst_sizes: size_t, buf: c_void_ptr): herr_t

.. function:: proc H5TBwrite_fields_name(loc_id: hid_t, dset_name: c_string, field_names: c_string, start: hsize_t, nrecords: hsize_t, type_size: size_t, ref field_offset: size_t, ref dst_sizes: size_t, buf: c_void_ptr): herr_t

.. function:: proc H5TBwrite_fields_index(loc_id: hid_t, dset_name: c_string, nfields: hsize_t, ref field_index: c_int, start: hsize_t, nrecords: hsize_t, type_size: size_t, ref field_offset: size_t, ref dst_sizes: size_t, buf: c_void_ptr): herr_t

.. function:: proc H5TBread_table(loc_id: hid_t, dset_name: c_string, dst_size: size_t, ref dst_offset: size_t, ref dst_sizes: size_t, dst_buf: c_void_ptr): herr_t

.. function:: proc H5TBread_fields_name(loc_id: hid_t, dset_name: c_string, field_names: c_string, start: hsize_t, nrecords: hsize_t, type_size: size_t, ref field_offset: size_t, ref dst_sizes: size_t, buf: c_void_ptr): herr_t

.. function:: proc H5TBread_fields_index(loc_id: hid_t, dset_name: c_string, nfields: hsize_t, ref field_index: c_int, start: hsize_t, nrecords: hsize_t, type_size: size_t, ref field_offset: size_t, ref dst_sizes: size_t, buf: c_void_ptr): herr_t

.. function:: proc H5TBread_records(loc_id: hid_t, dset_name: c_string, start: hsize_t, nrecords: hsize_t, type_size: size_t, ref dst_offset: size_t, ref dst_sizes: size_t, buf: c_void_ptr): herr_t

.. function:: proc H5TBget_table_info(loc_id: hid_t, dset_name: c_string, ref nfields: hsize_t, ref nrecords: hsize_t): herr_t

.. function:: proc H5TBget_field_info(loc_id: hid_t, dset_name: c_string, field_names: c_ptr(c_string), ref field_sizes: size_t, ref field_offsets: size_t, ref type_size: size_t): herr_t

.. function:: proc H5TBdelete_record(loc_id: hid_t, dset_name: c_string, start: hsize_t, nrecords: hsize_t): herr_t

.. function:: proc H5TBinsert_record(loc_id: hid_t, dset_name: c_string, start: hsize_t, nrecords: hsize_t, dst_size: size_t, ref dst_offset: size_t, ref dst_sizes: size_t, buf: c_void_ptr): herr_t

.. function:: proc H5TBadd_records_from(loc_id: hid_t, dset_name1: c_string, start1: hsize_t, nrecords: hsize_t, dset_name2: c_string, start2: hsize_t): herr_t

.. function:: proc H5TBcombine_tables(loc_id1: hid_t, dset_name1: c_string, loc_id2: hid_t, dset_name2: c_string, dset_name3: c_string): herr_t

.. function:: proc H5TBinsert_field(loc_id: hid_t, dset_name: c_string, field_name: c_string, field_type: hid_t, position: hsize_t, fill_data: c_void_ptr, buf: c_void_ptr): herr_t

.. function:: proc H5TBdelete_field(loc_id: hid_t, dset_name: c_string, field_name: c_string): herr_t

.. function:: proc H5TBAget_title(loc_id: hid_t, table_title: c_string): herr_t

.. function:: proc H5TBAget_fill(loc_id: hid_t, dset_name: c_string, dset_id: hid_t, ref dst_buf: c_uchar): htri_t

.. function:: proc H5PTcreate(loc_id: hid_t, dset_name: c_string, dtype_id: hid_t, chunk_size: hsize_t, plist_id: hid_t): hid_t

.. function:: proc H5PTopen(loc_id: hid_t, dset_name: c_string): hid_t

.. function:: proc H5PTclose(table_id: hid_t): herr_t

.. function:: proc H5PTcreate_fl(loc_id: hid_t, dset_name: c_string, dtype_id: hid_t, chunk_size: hsize_t, compression: c_int): hid_t

.. function:: proc H5PTappend(table_id: hid_t, nrecords: size_t, data: c_void_ptr): herr_t

.. function:: proc H5PTget_next(table_id: hid_t, nrecords: size_t, data: c_void_ptr): herr_t

.. function:: proc H5PTread_packets(table_id: hid_t, start: hsize_t, nrecords: size_t, data: c_void_ptr): herr_t

.. function:: proc H5PTget_num_packets(table_id: hid_t, ref nrecords: hsize_t): herr_t

.. function:: proc H5PTis_valid(table_id: hid_t): herr_t

.. function:: proc H5PTis_varlen(table_id: hid_t): herr_t

.. function:: proc H5PTget_dataset(table_id: hid_t): hid_t

.. function:: proc H5PTget_type(table_id: hid_t): hid_t

.. function:: proc H5PTcreate_index(table_id: hid_t): herr_t

.. function:: proc H5PTset_index(table_id: hid_t, pt_index: hsize_t): herr_t

.. function:: proc H5PTget_index(table_id: hid_t, ref pt_index: hsize_t): herr_t

.. function:: proc H5PTfree_vlen_buff(table_id: hid_t, bufflen: size_t, buff: c_void_ptr): herr_t

.. function:: proc H5LDget_dset_dims(did: hid_t, ref cur_dims: hsize_t): herr_t

.. function:: proc H5LDget_dset_type_size(did: hid_t, fields: c_string): size_t

.. function:: proc H5LDget_dset_elmts(did: hid_t, ref prev_dims: hsize_t, ref cur_dims: hsize_t, fields: c_string, buf: c_void_ptr): herr_t

.. record:: H5AC_cache_config_t

   .. attribute:: var version: c_int

   .. attribute:: var rpt_fcn_enabled: hbool_t

   .. attribute:: var open_trace_file: hbool_t

   .. attribute:: var close_trace_file: hbool_t

   .. attribute:: var trace_file_name: c_ptr(c_char)

   .. attribute:: var evictions_enabled: hbool_t

   .. attribute:: var set_initial_size: hbool_t

   .. attribute:: var initial_size: size_t

   .. attribute:: var min_clean_fraction: c_double

   .. attribute:: var max_size: size_t

   .. attribute:: var min_size: size_t

   .. attribute:: var epoch_length: c_long

   .. attribute:: var incr_mode: H5C_cache_incr_mode

   .. attribute:: var lower_hr_threshold: c_double

   .. attribute:: var increment: c_double

   .. attribute:: var apply_max_increment: hbool_t

   .. attribute:: var max_increment: size_t

   .. attribute:: var flash_incr_mode: H5C_cache_flash_incr_mode

   .. attribute:: var flash_multiple: c_double

   .. attribute:: var flash_threshold: c_double

   .. attribute:: var decr_mode: H5C_cache_decr_mode

   .. attribute:: var upper_hr_threshold: c_double

   .. attribute:: var decrement: c_double

   .. attribute:: var apply_max_decrement: hbool_t

   .. attribute:: var max_decrement: size_t

   .. attribute:: var epochs_before_eviction: c_int

   .. attribute:: var apply_empty_reserve: hbool_t

   .. attribute:: var empty_reserve: c_double

   .. attribute:: var dirty_bytes_threshold: size_t

   .. attribute:: var metadata_write_strategy: c_int

.. record:: H5AC_cache_image_config_t

   .. attribute:: var version: c_int

   .. attribute:: var generate_image: hbool_t

   .. attribute:: var save_resize_status: hbool_t

   .. attribute:: var entry_ageout: c_int

.. record:: H5A_info_t

   .. attribute:: var corder_valid: hbool_t

   .. attribute:: var corder: H5O_msg_crt_idx_t

   .. attribute:: var cset: H5T_cset_t

   .. attribute:: var data_size: hsize_t

.. type:: type H5A_operator1_t = c_fn_ptr

.. type:: type H5A_operator2_t = c_fn_ptr

.. type:: type H5DS_iterate_t = c_fn_ptr

.. type:: type H5D_alloc_time_t = c_int

.. data:: const H5D_ALLOC_TIME_ERROR: H5D_alloc_time_t

.. data:: const H5D_ALLOC_TIME_DEFAULT: H5D_alloc_time_t

.. data:: const H5D_ALLOC_TIME_EARLY: H5D_alloc_time_t

.. data:: const H5D_ALLOC_TIME_LATE: H5D_alloc_time_t

.. data:: const H5D_ALLOC_TIME_INCR: H5D_alloc_time_t

.. type:: type H5D_append_cb_t = c_fn_ptr

.. type:: type H5D_chunk_index_t = c_int

.. data:: const H5D_CHUNK_IDX_BTREE: H5D_chunk_index_t

.. data:: const H5D_CHUNK_IDX_SINGLE: H5D_chunk_index_t

.. data:: const H5D_CHUNK_IDX_NONE: H5D_chunk_index_t

.. data:: const H5D_CHUNK_IDX_FARRAY: H5D_chunk_index_t

.. data:: const H5D_CHUNK_IDX_EARRAY: H5D_chunk_index_t

.. data:: const H5D_CHUNK_IDX_BT2: H5D_chunk_index_t

.. data:: const H5D_CHUNK_IDX_NTYPES: H5D_chunk_index_t

.. type:: type H5D_fill_time_t = c_int

.. data:: const H5D_FILL_TIME_ERROR: H5D_fill_time_t

.. data:: const H5D_FILL_TIME_ALLOC: H5D_fill_time_t

.. data:: const H5D_FILL_TIME_NEVER: H5D_fill_time_t

.. data:: const H5D_FILL_TIME_IFSET: H5D_fill_time_t

.. type:: type H5D_fill_value_t = c_int

.. data:: const H5D_FILL_VALUE_ERROR: H5D_fill_value_t

.. data:: const H5D_FILL_VALUE_UNDEFINED: H5D_fill_value_t

.. data:: const H5D_FILL_VALUE_DEFAULT: H5D_fill_value_t

.. data:: const H5D_FILL_VALUE_USER_DEFINED: H5D_fill_value_t

.. type:: type H5D_gather_func_t = c_fn_ptr

.. type:: type H5D_layout_t = c_int

.. data:: const H5D_LAYOUT_ERROR: H5D_layout_t

.. data:: const H5D_COMPACT: H5D_layout_t

.. data:: const H5D_CONTIGUOUS: H5D_layout_t

.. data:: const H5D_CHUNKED: H5D_layout_t

.. data:: const H5D_VIRTUAL: H5D_layout_t

.. data:: const H5D_NLAYOUTS: H5D_layout_t

.. type:: type H5D_mpio_actual_chunk_opt_mode_t = c_int

.. data:: const H5D_MPIO_NO_CHUNK_OPTIMIZATION: H5D_mpio_actual_chunk_opt_mode_t

.. data:: const H5D_MPIO_LINK_CHUNK: H5D_mpio_actual_chunk_opt_mode_t

.. data:: const H5D_MPIO_MULTI_CHUNK: H5D_mpio_actual_chunk_opt_mode_t

.. type:: type H5D_mpio_actual_io_mode_t = c_int

.. data:: const H5D_MPIO_NO_COLLECTIVE: H5D_mpio_actual_io_mode_t

.. data:: const H5D_MPIO_CHUNK_INDEPENDENT: H5D_mpio_actual_io_mode_t

.. data:: const H5D_MPIO_CHUNK_COLLECTIVE: H5D_mpio_actual_io_mode_t

.. data:: const H5D_MPIO_CHUNK_MIXED: H5D_mpio_actual_io_mode_t

.. data:: const H5D_MPIO_CONTIGUOUS_COLLECTIVE: H5D_mpio_actual_io_mode_t

.. type:: type H5D_mpio_no_collective_cause_t = c_int

.. data:: const H5D_MPIO_COLLECTIVE: H5D_mpio_no_collective_cause_t

.. data:: const H5D_MPIO_SET_INDEPENDENT: H5D_mpio_no_collective_cause_t

.. data:: const H5D_MPIO_DATATYPE_CONVERSION: H5D_mpio_no_collective_cause_t

.. data:: const H5D_MPIO_DATA_TRANSFORMS: H5D_mpio_no_collective_cause_t

.. data:: const H5D_MPIO_MPI_OPT_TYPES_ENV_VAR_DISABLED: H5D_mpio_no_collective_cause_t

.. data:: const H5D_MPIO_NOT_SIMPLE_OR_SCALAR_DATASPACES: H5D_mpio_no_collective_cause_t

.. data:: const H5D_MPIO_NOT_CONTIGUOUS_OR_CHUNKED_DATASET: H5D_mpio_no_collective_cause_t

.. data:: const H5D_MPIO_FILTERS: H5D_mpio_no_collective_cause_t

.. type:: type H5D_operator_t = c_fn_ptr

.. type:: type H5D_scatter_func_t = c_fn_ptr

.. type:: type H5D_space_status_t = c_int

.. data:: const H5D_SPACE_STATUS_ERROR: H5D_space_status_t

.. data:: const H5D_SPACE_STATUS_NOT_ALLOCATED: H5D_space_status_t

.. data:: const H5D_SPACE_STATUS_PART_ALLOCATED: H5D_space_status_t

.. data:: const H5D_SPACE_STATUS_ALLOCATED: H5D_space_status_t

.. type:: type H5D_vds_view_t = c_int

.. data:: const H5D_VDS_ERROR: H5D_vds_view_t

.. data:: const H5D_VDS_FIRST_MISSING: H5D_vds_view_t

.. data:: const H5D_VDS_LAST_AVAILABLE: H5D_vds_view_t

.. type:: type H5E_auto1_t = c_fn_ptr

.. type:: type H5E_auto2_t = c_fn_ptr

.. type:: type H5E_direction_t = c_int

.. data:: const H5E_WALK_UPWARD: H5E_direction_t

.. data:: const H5E_WALK_DOWNWARD: H5E_direction_t

.. record:: H5E_error1_t

   .. attribute:: var maj_num: H5E_major_t

   .. attribute:: var min_num: H5E_minor_t

   .. attribute:: var func_name: c_string

   .. attribute:: var file_name: c_string

   .. attribute:: var line: c_uint

   .. attribute:: var desc: c_string

.. record:: H5E_error2_t

   .. attribute:: var cls_id: hid_t

   .. attribute:: var maj_num: hid_t

   .. attribute:: var min_num: hid_t

   .. attribute:: var line: c_uint

   .. attribute:: var func_name: c_string

   .. attribute:: var file_name: c_string

   .. attribute:: var desc: c_string

.. type:: type H5E_major_t = hid_t

.. type:: type H5E_minor_t = hid_t

.. type:: type H5E_type_t = c_int

.. data:: const H5E_MAJOR: H5E_type_t

.. data:: const H5E_MINOR: H5E_type_t

.. type:: type H5E_walk1_t = c_fn_ptr

.. type:: type H5E_walk2_t = c_fn_ptr

.. record:: H5FD_class_t

   .. attribute:: var name: c_string

   .. attribute:: var maxaddr: haddr_t

   .. attribute:: var fc_degree: H5F_close_degree_t

   .. attribute:: var terminate: c_fn_ptr

   .. attribute:: var sb_size: c_fn_ptr

   .. attribute:: var sb_encode: c_fn_ptr

   .. attribute:: var sb_decode: c_fn_ptr

   .. attribute:: var fapl_size: size_t

   .. attribute:: var fapl_get: c_fn_ptr

   .. attribute:: var fapl_copy: c_fn_ptr

   .. attribute:: var fapl_free: c_fn_ptr

   .. attribute:: var dxpl_size: size_t

   .. attribute:: var dxpl_copy: c_fn_ptr

   .. attribute:: var dxpl_free: c_fn_ptr

   .. attribute:: var open: c_fn_ptr

   .. attribute:: var close: c_fn_ptr

   .. attribute:: var cmp: c_fn_ptr

   .. attribute:: var query: c_fn_ptr

   .. attribute:: var get_type_map: c_fn_ptr

   .. attribute:: var alloc: c_fn_ptr

   .. attribute:: var free: c_fn_ptr

   .. attribute:: var get_eoa: c_fn_ptr

   .. attribute:: var set_eoa: c_fn_ptr

   .. attribute:: var get_eof: c_fn_ptr

   .. attribute:: var get_handle: c_fn_ptr

   .. attribute:: var read: c_fn_ptr

   .. attribute:: var write: c_fn_ptr

   .. attribute:: var flush: c_fn_ptr

   .. attribute:: var truncate: c_fn_ptr

   .. attribute:: var lock: c_fn_ptr

   .. attribute:: var unlock: c_fn_ptr

   .. attribute:: var fl_map: c_ptr(H5FD_mem_t)

.. record:: H5FD_file_image_callbacks_t

   .. attribute:: var image_malloc: c_fn_ptr

   .. attribute:: var image_memcpy: c_fn_ptr

   .. attribute:: var image_realloc: c_fn_ptr

   .. attribute:: var image_free: c_fn_ptr

   .. attribute:: var udata_copy: c_fn_ptr

   .. attribute:: var udata_free: c_fn_ptr

   .. attribute:: var udata: c_void_ptr

.. type:: type H5FD_file_image_op_t = c_int

.. data:: const H5FD_FILE_IMAGE_OP_NO_OP: H5FD_file_image_op_t

.. data:: const H5FD_FILE_IMAGE_OP_PROPERTY_LIST_SET: H5FD_file_image_op_t

.. data:: const H5FD_FILE_IMAGE_OP_PROPERTY_LIST_COPY: H5FD_file_image_op_t

.. data:: const H5FD_FILE_IMAGE_OP_PROPERTY_LIST_GET: H5FD_file_image_op_t

.. data:: const H5FD_FILE_IMAGE_OP_PROPERTY_LIST_CLOSE: H5FD_file_image_op_t

.. data:: const H5FD_FILE_IMAGE_OP_FILE_OPEN: H5FD_file_image_op_t

.. data:: const H5FD_FILE_IMAGE_OP_FILE_RESIZE: H5FD_file_image_op_t

.. data:: const H5FD_FILE_IMAGE_OP_FILE_CLOSE: H5FD_file_image_op_t

.. record:: H5FD_free_t

   .. attribute:: var addr: haddr_t

   .. attribute:: var size: hsize_t

   .. attribute:: var next: c_ptr(H5FD_free_t)

.. type:: type H5FD_mem_t = c_int

.. type:: type H5FD_mpio_chunk_opt_t = c_int

.. data:: const H5FD_MPIO_CHUNK_DEFAULT: H5FD_mpio_chunk_opt_t

.. data:: const H5FD_MPIO_CHUNK_ONE_IO: H5FD_mpio_chunk_opt_t

.. data:: const H5FD_MPIO_CHUNK_MULTI_IO: H5FD_mpio_chunk_opt_t

.. type:: type H5FD_mpio_collective_opt_t = c_int

.. data:: const H5FD_MPIO_COLLECTIVE_IO: H5FD_mpio_collective_opt_t

.. data:: const H5FD_MPIO_INDIVIDUAL_IO: H5FD_mpio_collective_opt_t

.. type:: type H5FD_mpio_xfer_t = c_int

.. data:: const H5FD_MPIO_INDEPENDENT: H5FD_mpio_xfer_t

.. data:: const H5FD_MPIO_COLLECTIVE: H5FD_mpio_xfer_t

.. type:: type H5F_close_degree_t = c_int

.. data:: const H5F_CLOSE_DEFAULT: H5F_close_degree_t

.. data:: const H5F_CLOSE_WEAK: H5F_close_degree_t

.. data:: const H5F_CLOSE_SEMI: H5F_close_degree_t

.. data:: const H5F_CLOSE_STRONG: H5F_close_degree_t

.. type:: type H5F_file_space_type_t = c_int

.. data:: const H5F_FILE_SPACE_DEFAULT: H5F_file_space_type_t

.. data:: const H5F_FILE_SPACE_ALL_PERSIST: H5F_file_space_type_t

.. data:: const H5F_FILE_SPACE_ALL: H5F_file_space_type_t

.. data:: const H5F_FILE_SPACE_AGGR_VFD: H5F_file_space_type_t

.. data:: const H5F_FILE_SPACE_VFD: H5F_file_space_type_t

.. data:: const H5F_FILE_SPACE_NTYPES: H5F_file_space_type_t

.. type:: type H5F_flush_cb_t = c_fn_ptr

.. type:: type H5F_fspace_strategy_t = c_int

.. data:: const H5F_FSPACE_STRATEGY_FSM_AGGR: H5F_fspace_strategy_t

.. data:: const H5F_FSPACE_STRATEGY_PAGE: H5F_fspace_strategy_t

.. data:: const H5F_FSPACE_STRATEGY_AGGR: H5F_fspace_strategy_t

.. data:: const H5F_FSPACE_STRATEGY_NONE: H5F_fspace_strategy_t

.. data:: const H5F_FSPACE_STRATEGY_NTYPES: H5F_fspace_strategy_t

.. record:: unnamedStruct0

   .. attribute:: var hdr_size: hsize_t

   .. attribute:: var msgs_info: H5_ih_info_t

.. record:: H5F_info1_t

   .. attribute:: var super_ext_size: hsize_t

   .. attribute:: var sohm: unnamedStruct0

.. record:: unnamedStruct1

   .. attribute:: var version: c_uint

   .. attribute:: var super_size: hsize_t

   .. attribute:: var super_ext_size: hsize_t

.. record:: unnamedStruct2

   .. attribute:: var version: c_uint

   .. attribute:: var meta_size: hsize_t

   .. attribute:: var tot_space: hsize_t

.. record:: unnamedStruct3

   .. attribute:: var version: c_uint

   .. attribute:: var hdr_size: hsize_t

   .. attribute:: var msgs_info: H5_ih_info_t

.. record:: H5F_info2_t

   .. attribute:: var super: unnamedStruct1

   .. attribute:: var free: unnamedStruct2

   .. attribute:: var sohm: unnamedStruct3

.. type:: type H5F_libver_t = c_int

.. data:: const H5F_LIBVER_EARLIEST: H5F_libver_t

.. data:: const H5F_LIBVER_LATEST: H5F_libver_t

.. type:: type H5F_mem_t = c_int

.. data:: const H5FD_MEM_NOLIST: H5F_mem_t

.. data:: const H5FD_MEM_DEFAULT: H5F_mem_t

.. data:: const H5FD_MEM_SUPER: H5F_mem_t

.. data:: const H5FD_MEM_BTREE: H5F_mem_t

.. data:: const H5FD_MEM_DRAW: H5F_mem_t

.. data:: const H5FD_MEM_GHEAP: H5F_mem_t

.. data:: const H5FD_MEM_LHEAP: H5F_mem_t

.. data:: const H5FD_MEM_OHDR: H5F_mem_t

.. data:: const H5FD_MEM_NTYPES: H5F_mem_t

.. record:: H5F_retry_info_t

   .. attribute:: var nbins: c_uint

   .. attribute:: var retries: c_ptr(c_ptr(uint(32)))

.. type:: type H5F_scope_t = c_int

.. data:: const H5F_SCOPE_LOCAL: H5F_scope_t

.. data:: const H5F_SCOPE_GLOBAL: H5F_scope_t

.. record:: H5F_sect_info_t

   .. attribute:: var addr: haddr_t

   .. attribute:: var size: hsize_t

.. record:: H5G_info_t

   .. attribute:: var storage_type: H5G_storage_type_t

   .. attribute:: var nlinks: hsize_t

   .. attribute:: var max_corder: int(64)

   .. attribute:: var mounted: hbool_t

.. type:: type H5G_iterate_t = c_fn_ptr

.. type:: type H5G_obj_t = c_int

.. data:: const H5G_UNKNOWN: H5G_obj_t

.. data:: const H5G_GROUP: H5G_obj_t

.. data:: const H5G_DATASET: H5G_obj_t

.. data:: const H5G_TYPE: H5G_obj_t

.. data:: const H5G_LINK: H5G_obj_t

.. data:: const H5G_UDLINK: H5G_obj_t

.. data:: const H5G_RESERVED_5: H5G_obj_t

.. data:: const H5G_RESERVED_6: H5G_obj_t

.. data:: const H5G_RESERVED_7: H5G_obj_t

.. record:: H5G_stat_t

.. type:: type H5G_storage_type_t = c_int

.. data:: const H5G_STORAGE_TYPE_UNKNOWN: H5G_storage_type_t

.. data:: const H5G_STORAGE_TYPE_SYMBOL_TABLE: H5G_storage_type_t

.. data:: const H5G_STORAGE_TYPE_COMPACT: H5G_storage_type_t

.. data:: const H5G_STORAGE_TYPE_DENSE: H5G_storage_type_t

.. type:: type H5I_free_t = c_fn_ptr

.. type:: type H5I_search_func_t = c_fn_ptr

.. type:: type H5I_type_t = c_int

.. data:: const H5I_UNINIT: H5I_type_t

.. data:: const H5I_BADID: H5I_type_t

.. data:: const H5I_FILE: H5I_type_t

.. data:: const H5I_GROUP: H5I_type_t

.. data:: const H5I_DATATYPE: H5I_type_t

.. data:: const H5I_DATASPACE: H5I_type_t

.. data:: const H5I_DATASET: H5I_type_t

.. data:: const H5I_ATTR: H5I_type_t

.. data:: const H5I_REFERENCE: H5I_type_t

.. data:: const H5I_VFL: H5I_type_t

.. data:: const H5I_GENPROP_CLS: H5I_type_t

.. data:: const H5I_GENPROP_LST: H5I_type_t

.. data:: const H5I_ERROR_CLASS: H5I_type_t

.. data:: const H5I_ERROR_MSG: H5I_type_t

.. data:: const H5I_ERROR_STACK: H5I_type_t

.. data:: const H5I_NTYPES: H5I_type_t

.. type:: type H5LT_lang_t = c_int

.. data:: const H5LT_LANG_ERR: H5LT_lang_t

.. data:: const H5LT_DDL: H5LT_lang_t

.. data:: const H5LT_C: H5LT_lang_t

.. data:: const H5LT_FORTRAN: H5LT_lang_t

.. data:: const H5LT_NO_LANG: H5LT_lang_t

.. record:: H5L_class_t

   .. attribute:: var version: c_int

   .. attribute:: var id: H5L_type_t

   .. attribute:: var comment: c_string

   .. attribute:: var create_func: H5L_create_func_t

   .. attribute:: var move_func: H5L_move_func_t

   .. attribute:: var copy_func: H5L_copy_func_t

   .. attribute:: var trav_func: H5L_traverse_func_t

   .. attribute:: var del_func: H5L_delete_func_t

   .. attribute:: var query_func: H5L_query_func_t

.. type:: type H5L_copy_func_t = c_fn_ptr

.. type:: type H5L_create_func_t = c_fn_ptr

.. type:: type H5L_delete_func_t = c_fn_ptr

.. type:: type H5L_elink_traverse_t = c_fn_ptr

.. record:: H5L_info_t

.. type:: type H5L_iterate_t = c_fn_ptr

.. type:: type H5L_move_func_t = c_fn_ptr

.. type:: type H5L_query_func_t = c_fn_ptr

.. type:: type H5L_traverse_func_t = c_fn_ptr

.. type:: type H5L_type_t = c_int

.. data:: const H5L_TYPE_ERROR: H5L_type_t

.. data:: const H5L_TYPE_HARD: H5L_type_t

.. data:: const H5L_TYPE_SOFT: H5L_type_t

.. data:: const H5L_TYPE_EXTERNAL: H5L_type_t

.. data:: const H5L_TYPE_MAX: H5L_type_t

.. type:: type H5MM_allocate_t = c_fn_ptr

.. type:: type H5MM_free_t = c_fn_ptr

.. record:: unnamedStruct4

   .. attribute:: var total: hsize_t

   .. attribute:: var meta: hsize_t

   .. attribute:: var mesg: hsize_t

   .. attribute:: var free: hsize_t

.. record:: unnamedStruct5

   .. attribute:: var present: uint(64)

   .. attribute:: var isShared: uint(64)

.. record:: H5O_hdr_info_t

   .. attribute:: var version: c_uint

   .. attribute:: var nmesgs: c_uint

   .. attribute:: var nchunks: c_uint

   .. attribute:: var flags: c_uint

   .. attribute:: var space: unnamedStruct4

   .. attribute:: var mesg: unnamedStruct5

.. record:: H5O_info_t

.. type:: type H5O_iterate_t = c_fn_ptr

.. type:: type H5O_mcdt_search_cb_t = c_fn_ptr

.. type:: type H5O_mcdt_search_ret_t = c_int

.. data:: const H5O_MCDT_SEARCH_ERROR: H5O_mcdt_search_ret_t

.. data:: const H5O_MCDT_SEARCH_CONT: H5O_mcdt_search_ret_t

.. data:: const H5O_MCDT_SEARCH_STOP: H5O_mcdt_search_ret_t

.. type:: type H5O_msg_crt_idx_t = uint(32)

.. record:: H5O_stat_t

   .. attribute:: var size: hsize_t

   .. attribute:: var free: hsize_t

   .. attribute:: var nmesgs: c_uint

   .. attribute:: var nchunks: c_uint

.. type:: type H5O_type_t = c_int

.. data:: const H5O_TYPE_UNKNOWN: H5O_type_t

.. data:: const H5O_TYPE_GROUP: H5O_type_t

.. data:: const H5O_TYPE_DATASET: H5O_type_t

.. data:: const H5O_TYPE_NAMED_DATATYPE: H5O_type_t

.. data:: const H5O_TYPE_NTYPES: H5O_type_t

.. type:: type H5PL_type_t = c_int

.. data:: const H5PL_TYPE_ERROR: H5PL_type_t

.. data:: const H5PL_TYPE_FILTER: H5PL_type_t

.. data:: const H5PL_TYPE_NONE: H5PL_type_t

.. type:: type H5P_cls_close_func_t = c_fn_ptr

.. type:: type H5P_cls_copy_func_t = c_fn_ptr

.. type:: type H5P_cls_create_func_t = c_fn_ptr

.. type:: type H5P_iterate_t = c_fn_ptr

.. type:: type H5P_prp_cb1_t = c_fn_ptr

.. type:: type H5P_prp_cb2_t = c_fn_ptr

.. type:: type H5P_prp_close_func_t = H5P_prp_cb1_t

.. type:: type H5P_prp_compare_func_t = c_fn_ptr

.. type:: type H5P_prp_copy_func_t = H5P_prp_cb1_t

.. type:: type H5P_prp_create_func_t = H5P_prp_cb1_t

.. type:: type H5P_prp_decode_func_t = c_fn_ptr

.. type:: type H5P_prp_delete_func_t = H5P_prp_cb2_t

.. type:: type H5P_prp_encode_func_t = c_fn_ptr

.. type:: type H5P_prp_get_func_t = H5P_prp_cb2_t

.. type:: type H5P_prp_set_func_t = H5P_prp_cb2_t

.. type:: type H5R_type_t = c_int

.. data:: const H5R_BADTYPE: H5R_type_t

.. data:: const H5R_OBJECT: H5R_type_t

.. data:: const H5R_DATASET_REGION: H5R_type_t

.. data:: const H5R_MAXTYPE: H5R_type_t

.. type:: type H5S_class_t = c_int

.. data:: const H5S_NO_CLASS: H5S_class_t

.. data:: const H5S_SCALAR: H5S_class_t

.. data:: const H5S_SIMPLE: H5S_class_t

.. data:: const H5S_NULL: H5S_class_t

.. type:: type H5S_sel_type = c_int

.. data:: const H5S_SEL_ERROR: H5S_sel_type

.. data:: const H5S_SEL_NONE: H5S_sel_type

.. data:: const H5S_SEL_POINTS: H5S_sel_type

.. data:: const H5S_SEL_HYPERSLABS: H5S_sel_type

.. data:: const H5S_SEL_ALL: H5S_sel_type

.. data:: const H5S_SEL_N: H5S_sel_type

.. type:: type H5S_seloper_t = c_int

.. data:: const H5S_SELECT_NOOP: H5S_seloper_t

.. data:: const H5S_SELECT_SET: H5S_seloper_t

.. data:: const H5S_SELECT_OR: H5S_seloper_t

.. data:: const H5S_SELECT_AND: H5S_seloper_t

.. data:: const H5S_SELECT_XOR: H5S_seloper_t

.. data:: const H5S_SELECT_NOTB: H5S_seloper_t

.. data:: const H5S_SELECT_NOTA: H5S_seloper_t

.. data:: const H5S_SELECT_APPEND: H5S_seloper_t

.. data:: const H5S_SELECT_PREPEND: H5S_seloper_t

.. data:: const H5S_SELECT_INVALID: H5S_seloper_t

.. type:: type H5T_bkg_t = c_int

.. data:: const H5T_BKG_NO: H5T_bkg_t

.. data:: const H5T_BKG_TEMP: H5T_bkg_t

.. data:: const H5T_BKG_YES: H5T_bkg_t

.. record:: H5T_cdata_t

   .. attribute:: var command: H5T_cmd_t

   .. attribute:: var need_bkg: H5T_bkg_t

   .. attribute:: var recalc: hbool_t

   .. attribute:: var priv: c_void_ptr

.. type:: type H5T_class_t = c_int

.. data:: const H5T_NO_CLASS: H5T_class_t

.. data:: const H5T_INTEGER: H5T_class_t

.. data:: const H5T_FLOAT: H5T_class_t

.. data:: const H5T_TIME: H5T_class_t

.. data:: const H5T_STRING: H5T_class_t

.. data:: const H5T_BITFIELD: H5T_class_t

.. data:: const H5T_OPAQUE: H5T_class_t

.. data:: const H5T_COMPOUND: H5T_class_t

.. data:: const H5T_REFERENCE: H5T_class_t

.. data:: const H5T_ENUM: H5T_class_t

.. data:: const H5T_VLEN: H5T_class_t

.. data:: const H5T_ARRAY: H5T_class_t

.. data:: const H5T_NCLASSES: H5T_class_t

.. type:: type H5T_cmd_t = c_int

.. data:: const H5T_CONV_INIT: H5T_cmd_t

.. data:: const H5T_CONV_CONV: H5T_cmd_t

.. data:: const H5T_CONV_FREE: H5T_cmd_t

.. type:: type H5T_conv_except_func_t = c_fn_ptr

.. type:: type H5T_conv_except_t = c_int

.. data:: const H5T_CONV_EXCEPT_RANGE_HI: H5T_conv_except_t

.. data:: const H5T_CONV_EXCEPT_RANGE_LOW: H5T_conv_except_t

.. data:: const H5T_CONV_EXCEPT_PRECISION: H5T_conv_except_t

.. data:: const H5T_CONV_EXCEPT_TRUNCATE: H5T_conv_except_t

.. data:: const H5T_CONV_EXCEPT_PINF: H5T_conv_except_t

.. data:: const H5T_CONV_EXCEPT_NINF: H5T_conv_except_t

.. data:: const H5T_CONV_EXCEPT_NAN: H5T_conv_except_t

.. type:: type H5T_conv_ret_t = c_int

.. data:: const H5T_CONV_ABORT: H5T_conv_ret_t

.. data:: const H5T_CONV_UNHANDLED: H5T_conv_ret_t

.. data:: const H5T_CONV_HANDLED: H5T_conv_ret_t

.. type:: type H5T_conv_t = c_fn_ptr

.. type:: type H5T_cset_t = c_int

.. data:: const H5T_CSET_ERROR: H5T_cset_t

.. data:: const H5T_CSET_ASCII: H5T_cset_t

.. data:: const H5T_CSET_UTF8: H5T_cset_t

.. data:: const H5T_CSET_RESERVED_2: H5T_cset_t

.. data:: const H5T_CSET_RESERVED_3: H5T_cset_t

.. data:: const H5T_CSET_RESERVED_4: H5T_cset_t

.. data:: const H5T_CSET_RESERVED_5: H5T_cset_t

.. data:: const H5T_CSET_RESERVED_6: H5T_cset_t

.. data:: const H5T_CSET_RESERVED_7: H5T_cset_t

.. data:: const H5T_CSET_RESERVED_8: H5T_cset_t

.. data:: const H5T_CSET_RESERVED_9: H5T_cset_t

.. data:: const H5T_CSET_RESERVED_10: H5T_cset_t

.. data:: const H5T_CSET_RESERVED_11: H5T_cset_t

.. data:: const H5T_CSET_RESERVED_12: H5T_cset_t

.. data:: const H5T_CSET_RESERVED_13: H5T_cset_t

.. data:: const H5T_CSET_RESERVED_14: H5T_cset_t

.. data:: const H5T_CSET_RESERVED_15: H5T_cset_t

.. type:: type H5T_direction_t = c_int

.. data:: const H5T_DIR_DEFAULT: H5T_direction_t

.. data:: const H5T_DIR_ASCEND: H5T_direction_t

.. data:: const H5T_DIR_DESCEND: H5T_direction_t

.. type:: type H5T_norm_t = c_int

.. data:: const H5T_NORM_ERROR: H5T_norm_t

.. data:: const H5T_NORM_IMPLIED: H5T_norm_t

.. data:: const H5T_NORM_MSBSET: H5T_norm_t

.. data:: const H5T_NORM_NONE: H5T_norm_t

.. type:: type H5T_order_t = c_int

.. data:: const H5T_ORDER_ERROR: H5T_order_t

.. data:: const H5T_ORDER_LE: H5T_order_t

.. data:: const H5T_ORDER_BE: H5T_order_t

.. data:: const H5T_ORDER_VAX: H5T_order_t

.. data:: const H5T_ORDER_MIXED: H5T_order_t

.. data:: const H5T_ORDER_NONE: H5T_order_t

.. type:: type H5T_pad_t = c_int

.. data:: const H5T_PAD_ERROR: H5T_pad_t

.. data:: const H5T_PAD_ZERO: H5T_pad_t

.. data:: const H5T_PAD_ONE: H5T_pad_t

.. data:: const H5T_PAD_BACKGROUND: H5T_pad_t

.. data:: const H5T_NPAD: H5T_pad_t

.. type:: type H5T_pers_t = c_int

.. data:: const H5T_PERS_DONTCARE: H5T_pers_t

.. data:: const H5T_PERS_HARD: H5T_pers_t

.. data:: const H5T_PERS_SOFT: H5T_pers_t

.. type:: type H5T_sign_t = c_int

.. data:: const H5T_SGN_ERROR: H5T_sign_t

.. data:: const H5T_SGN_NONE: H5T_sign_t

.. data:: const H5T_SGN_2: H5T_sign_t

.. data:: const H5T_NSGN: H5T_sign_t

.. type:: type H5T_str_t = c_int

.. data:: const H5T_STR_ERROR: H5T_str_t

.. data:: const H5T_STR_NULLTERM: H5T_str_t

.. data:: const H5T_STR_NULLPAD: H5T_str_t

.. data:: const H5T_STR_SPACEPAD: H5T_str_t

.. data:: const H5T_STR_RESERVED_3: H5T_str_t

.. data:: const H5T_STR_RESERVED_4: H5T_str_t

.. data:: const H5T_STR_RESERVED_5: H5T_str_t

.. data:: const H5T_STR_RESERVED_6: H5T_str_t

.. data:: const H5T_STR_RESERVED_7: H5T_str_t

.. data:: const H5T_STR_RESERVED_8: H5T_str_t

.. data:: const H5T_STR_RESERVED_9: H5T_str_t

.. data:: const H5T_STR_RESERVED_10: H5T_str_t

.. data:: const H5T_STR_RESERVED_11: H5T_str_t

.. data:: const H5T_STR_RESERVED_12: H5T_str_t

.. data:: const H5T_STR_RESERVED_13: H5T_str_t

.. data:: const H5T_STR_RESERVED_14: H5T_str_t

.. data:: const H5T_STR_RESERVED_15: H5T_str_t

.. type:: type H5Z_EDC_t = c_int

.. data:: const H5Z_ERROR_EDC: H5Z_EDC_t

.. data:: const H5Z_DISABLE_EDC: H5Z_EDC_t

.. data:: const H5Z_ENABLE_EDC: H5Z_EDC_t

.. data:: const H5Z_NO_EDC: H5Z_EDC_t

.. type:: type H5Z_SO_scale_type_t = c_int

.. data:: const H5Z_SO_FLOAT_DSCALE: H5Z_SO_scale_type_t

.. data:: const H5Z_SO_FLOAT_ESCALE: H5Z_SO_scale_type_t

.. data:: const H5Z_SO_INT: H5Z_SO_scale_type_t

.. type:: type H5Z_can_apply_func_t = c_fn_ptr

.. type:: type H5Z_cb_return_t = c_int

.. data:: const H5Z_CB_ERROR: H5Z_cb_return_t

.. data:: const H5Z_CB_FAIL: H5Z_cb_return_t

.. data:: const H5Z_CB_CONT: H5Z_cb_return_t

.. data:: const H5Z_CB_NO: H5Z_cb_return_t

.. record:: H5Z_cb_t

   .. attribute:: var func: H5Z_filter_func_t

   .. attribute:: var op_data: c_void_ptr

.. record:: H5Z_class1_t

   .. attribute:: var id: H5Z_filter_t

   .. attribute:: var name: c_string

   .. attribute:: var can_apply: H5Z_can_apply_func_t

   .. attribute:: var set_local: H5Z_set_local_func_t

   .. attribute:: var filter: H5Z_func_t

.. record:: H5Z_class2_t

   .. attribute:: var version: c_int

   .. attribute:: var id: H5Z_filter_t

   .. attribute:: var encoder_present: c_uint

   .. attribute:: var decoder_present: c_uint

   .. attribute:: var name: c_string

   .. attribute:: var can_apply: H5Z_can_apply_func_t

   .. attribute:: var set_local: H5Z_set_local_func_t

   .. attribute:: var filter: H5Z_func_t

.. type:: type H5Z_filter_func_t = c_fn_ptr

.. type:: type H5Z_filter_t = c_int

.. type:: type H5Z_func_t = c_fn_ptr

.. type:: type H5Z_set_local_func_t = c_fn_ptr

.. record:: H5_ih_info_t

   .. attribute:: var index_size: hsize_t

   .. attribute:: var heap_size: hsize_t

.. type:: type H5_index_t = c_int

.. data:: const H5_INDEX_UNKNOWN: H5_index_t

.. data:: const H5_INDEX_NAME: H5_index_t

.. data:: const H5_INDEX_CRT_ORDER: H5_index_t

.. data:: const H5_INDEX_N: H5_index_t

.. type:: type H5_iter_order_t = c_int

.. data:: const H5_ITER_UNKNOWN: H5_iter_order_t

.. data:: const H5_ITER_INC: H5_iter_order_t

.. data:: const H5_ITER_DEC: H5_iter_order_t

.. data:: const H5_ITER_NATIVE: H5_iter_order_t

.. data:: const H5_ITER_N: H5_iter_order_t

.. type:: type haddr_t = c_ulong

.. type:: type hbool_t = bool

.. type:: type herr_t = c_int

.. type:: type hid_t = int(64)

.. type:: type hobj_ref_t = haddr_t

.. type:: type hsize_t = c_ulonglong

.. type:: type hssize_t = c_longlong

.. type:: type htri_t = c_int

.. record:: hvl_t

   .. attribute:: var len: size_t

   .. attribute:: var p: c_void_ptr

.. data:: const H5_VERS_MAJOR: c_uint

   Macros defined in H5public.h 

.. data:: const H5_VERS_MINOR: c_uint

.. data:: const H5_VERS_RELEASE: c_uint

.. data:: const H5_VERS_SUBRELEASE: c_string

.. function:: proc H5check()

.. data:: param H5F_ACC_RDONLY = 0: c_uint

   absence of rdwr => rd-only 

.. data:: param H5F_ACC_RDWR = 1: c_uint

   open for read and write 

.. data:: param H5F_ACC_TRUNC = 2: c_uint

   overwrite existing files 

.. data:: param H5F_ACC_EXCL = 4: c_uint

   fail if file already exists 

.. data:: param H5F_ACC_CREAT = 16: c_uint

   create non-existing files 

.. data:: param H5F_ACC_SWMR_WRITE = 32: c_uint

   indicate that this file is open for writing in a
   single-writer/multi-reader (SWMR) scenario.  Note that the
   process(es) opening the file for reading must open the file
   with RDONLY access, and use the special "SWMR_READ" access flag. 

.. data:: param H5F_ACC_SWMR_READ = 64: c_uint

   indicate that this file is open for reading in a
   single-writer/multi-reader (SWMR) scenario.  Note that the
   process(es) opening the file for SWMR reading must also
   open the file with the RDONLY flag.  

.. data:: param H5F_ACC_DEFAULT = 65535: c_uint

   Value passed to H5Pset_elink_acc_flags to cause flags to be taken
   from the parent file.  Ignore setting on lapl 

.. data:: param H5F_OBJ_FILE = 1: c_uint

   File objects 

.. data:: param H5F_OBJ_DATASET = 2: c_uint

   Dataset objects 

.. data:: param H5F_OBJ_GROUP = 4: c_uint

   Group objects 

.. data:: param H5F_OBJ_DATATYPE = 8: c_uint

   Named datatype objects 

.. data:: param H5F_OBJ_ATTR = 16: c_uint

   Attribute objects 

.. data:: param H5F_OBJ_ALL = H5F_OBJ_FILE|H5F_OBJ_DATASET|H5F_OBJ_GROUP|H5F_OBJ_DATATYPE|H5F_OBJ_ATTR

.. data:: param H5F_OBJ_LOCAL = 32: c_uint

   Restrict search to objects opened through current file ID
   as opposed to objects opened through any file ID accessing this file 

.. data:: param H5F_FAMILY_DEFAULT = 0: hsize_t

.. function:: proc H5P_ROOT

   
   * The library's property list classes
   

.. function:: proc H5P_OBJECT_CREATE

.. function:: proc H5P_FILE_CREATE

.. function:: proc H5P_FILE_ACCESS

.. function:: proc H5P_DATASET_CREATE

.. function:: proc H5P_DATASET_ACCESS

.. function:: proc H5P_DATASET_XFER

.. function:: proc H5P_FILE_MOUNT

.. function:: proc H5P_GROUP_CREATE

.. function:: proc H5P_GROUP_ACCESS

.. function:: proc H5P_DATATYPE_CREATE

.. function:: proc H5P_DATATYPE_ACCESS

.. function:: proc H5P_STRING_CREATE

.. function:: proc H5P_ATTRIBUTE_CREATE

.. function:: proc H5P_ATTRIBUTE_ACCESS

.. function:: proc H5P_OBJECT_COPY

.. function:: proc H5P_LINK_CREATE

.. function:: proc H5P_LINK_ACCESS

.. function:: proc H5P_FILE_CREATE_DEFAULT

   The library's default property lists 

.. function:: proc H5P_FILE_ACCESS_DEFAULT

.. function:: proc H5P_DATASET_CREATE_DEFAULT

.. function:: proc H5P_DATASET_ACCESS_DEFAULT

.. function:: proc H5P_DATASET_XFER_DEFAULT

.. function:: proc H5P_FILE_MOUNT_DEFAULT

.. function:: proc H5P_GROUP_CREATE_DEFAULT

.. function:: proc H5P_GROUP_ACCESS_DEFAULT

.. function:: proc H5P_DATATYPE_CREATE_DEFAULT

.. function:: proc H5P_DATATYPE_ACCESS_DEFAULT

.. function:: proc H5P_ATTRIBUTE_CREATE_DEFAULT

.. function:: proc H5P_ATTRIBUTE_ACCESS_DEFAULT

.. function:: proc H5P_OBJECT_COPY_DEFAULT

.. function:: proc H5P_LINK_CREATE_DEFAULT

.. function:: proc H5P_LINK_ACCESS_DEFAULT

.. data:: param H5P_CRT_ORDER_TRACKED = 1: c_int

   Common creation order flags (for links in groups and
   * attributes on objects) 

.. data:: param H5P_CRT_ORDER_INDEXED = 2: c_int

.. data:: param H5P_DEFAULT = 0: hid_t

   Default value for all property list classes 

.. data:: const H5S_ALL: hid_t

.. data:: const H5S_UNLIMITED: hid_t

.. data:: const CHAR_MIN: c_int

   CHAR_MIN is defined in limits.h 

.. function:: proc H5T_NATIVE_CHAR

.. function:: proc H5T_NATIVE_SCHAR

.. function:: proc H5T_NATIVE_UCHAR

.. function:: proc H5T_NATIVE_SHORT

.. function:: proc H5T_NATIVE_USHORT

.. function:: proc H5T_NATIVE_INT

.. function:: proc H5T_NATIVE_UINT

.. function:: proc H5T_NATIVE_LONG

.. function:: proc H5T_NATIVE_ULONG

.. function:: proc H5T_NATIVE_LLONG

.. function:: proc H5T_NATIVE_ULLONG

.. function:: proc H5T_NATIVE_FLOAT

.. function:: proc H5T_NATIVE_DOUBLE

.. function:: proc H5T_NATIVE_LDOUBLE

.. function:: proc H5T_NATIVE_B8

.. function:: proc H5T_NATIVE_B16

.. function:: proc H5T_NATIVE_B32

.. function:: proc H5T_NATIVE_B64

.. function:: proc H5T_NATIVE_OPAQUE

.. function:: proc H5T_NATIVE_HADDR

.. function:: proc H5T_NATIVE_HSIZE

.. function:: proc H5T_NATIVE_HSSIZE

.. function:: proc H5T_NATIVE_HERR

.. function:: proc H5T_NATIVE_HBOOL

.. function:: proc H5T_NATIVE_INT8

   
   C9x types
   

.. function:: proc H5T_NATIVE_UINT8

.. function:: proc H5T_NATIVE_INT_LEAST8

.. function:: proc H5T_NATIVE_UINT_LEAST8

.. function:: proc H5T_NATIVE_INT_FAST8

.. function:: proc H5T_NATIVE_UINT_FAST8

.. function:: proc H5T_NATIVE_INT16

.. function:: proc H5T_NATIVE_UINT16

.. function:: proc H5T_NATIVE_INT_LEAST16

.. function:: proc H5T_NATIVE_UINT_LEAST16

.. function:: proc H5T_NATIVE_INT_FAST16

.. function:: proc H5T_NATIVE_UINT_FAST16

.. function:: proc H5T_NATIVE_INT32

.. function:: proc H5T_NATIVE_UINT32

.. function:: proc H5T_NATIVE_INT_LEAST32

.. function:: proc H5T_NATIVE_UINT_LEAST32

.. function:: proc H5T_NATIVE_INT_FAST32

.. function:: proc H5T_NATIVE_UINT_FAST32

.. function:: proc H5T_NATIVE_INT64

.. function:: proc H5T_NATIVE_UINT64

.. function:: proc H5T_NATIVE_INT_LEAST64

.. function:: proc H5T_NATIVE_UINT_LEAST64

.. function:: proc H5T_NATIVE_INT_FAST64

.. function:: proc H5T_NATIVE_UINT_FAST64

.. function:: proc H5T_IEEE_F32BE

   
   * The IEEE floating point types in various byte orders.
   

.. function:: proc H5T_IEEE_F32LE

.. function:: proc H5T_IEEE_F64BE

.. function:: proc H5T_IEEE_F64LE

.. function:: proc H5T_STD_I8BE

   
   * These are "standard" types.  For instance, signed (2's complement) and
   * unsigned integers of various sizes and byte orders.
   

.. function:: proc H5T_STD_I8LE

.. function:: proc H5T_STD_I16BE

.. function:: proc H5T_STD_I16LE

.. function:: proc H5T_STD_I32BE

.. function:: proc H5T_STD_I32LE

.. function:: proc H5T_STD_I64BE

.. function:: proc H5T_STD_I64LE

.. function:: proc H5T_STD_U8BE

.. function:: proc H5T_STD_U8LE

.. function:: proc H5T_STD_U16BE

.. function:: proc H5T_STD_U16LE

.. function:: proc H5T_STD_U32BE

.. function:: proc H5T_STD_U32LE

.. function:: proc H5T_STD_U64BE

.. function:: proc H5T_STD_U64LE

.. function:: proc H5T_STD_B8BE

.. function:: proc H5T_STD_B8LE

.. function:: proc H5T_STD_B16BE

.. function:: proc H5T_STD_B16LE

.. function:: proc H5T_STD_B32BE

.. function:: proc H5T_STD_B32LE

.. function:: proc H5T_STD_B64BE

.. function:: proc H5T_STD_B64LE

.. function:: proc H5T_STD_REF_OBJ

.. function:: proc H5T_STD_REF_DSETREG

.. function:: proc H5T_UNIX_D32BE

   
   * Types which are particular to Unix.
   

.. function:: proc H5T_UNIX_D32LE

.. function:: proc H5T_UNIX_D64BE

.. function:: proc H5T_UNIX_D64LE

.. function:: proc H5T_C_S1

   Types particular to the C language.  String types use 'bytes' instead
   of 'bits' as their size.
   

.. function:: proc H5T_VARIABLE

.. function:: proc H5T_FORTRAN_S1

   Types particular to Fortran.  

