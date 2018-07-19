module NetCDF {
  module C_NetCDF {
    // Generated with c2chapel version 0.1.0

    // Header given to c2chapel:
    require "netcdf.h";

    // #define'd integer literals:
    extern const NC_FIRSTUSERTYPEID : int;
    extern const NC_MAX_BYTE : int;
    extern const NC_MAX_CHAR : int;
    extern const NC_MAX_SHORT : int;
    extern const NC_MAX_INT : int;
    extern const NC_SIZEHINT_DEFAULT : int;
    extern const NC_MAX_NAME : int;
    extern const NC_MAX_HDF4_NAME : int;
    extern const NC_ENDIAN_NATIVE : int;
    extern const NC_ENDIAN_LITTLE : int;
    extern const NC_ENDIAN_BIG : int;
    extern const NC_CHUNKED : int;
    extern const NC_CONTIGUOUS : int;
    extern const NC_NOCHECKSUM : int;
    extern const NC_FLETCHER32 : int;
    extern const NC_NOSHUFFLE : int;
    extern const NC_SHUFFLE : int;
    extern const NC_HAVE_NEW_CHUNKING_API : int;

    // End of #define'd integer literals

    extern proc nc_inq_libvers() : c_string;

    extern proc nc_strerror(ncerr : c_int) : c_string;

    extern proc nc__create(path : c_string, cmode : c_int, initialsz : size_t, ref chunksizehintp : size_t, ref ncidp : c_int) : c_int;

    extern proc nc_create(path : c_string, cmode : c_int, ref ncidp : c_int) : c_int;

    extern proc nc__open(path : c_string, mode : c_int, ref chunksizehintp : size_t, ref ncidp : c_int) : c_int;

    extern proc nc_open(path : c_string, mode : c_int, ref ncidp : c_int) : c_int;

    extern proc nc_inq_path(ncid : c_int, ref pathlen : size_t, path : c_string) : c_int;

    extern proc nc_inq_ncid(ncid : c_int, name : c_string, ref grp_ncid : c_int) : c_int;

    extern proc nc_inq_grps(ncid : c_int, ref numgrps : c_int, ref ncids : c_int) : c_int;

    extern proc nc_inq_grpname(ncid : c_int, name : c_string) : c_int;

    extern proc nc_inq_grpname_full(ncid : c_int, ref lenp : size_t, full_name : c_string) : c_int;

    extern proc nc_inq_grpname_len(ncid : c_int, ref lenp : size_t) : c_int;

    extern proc nc_inq_grp_parent(ncid : c_int, ref parent_ncid : c_int) : c_int;

    extern proc nc_inq_grp_ncid(ncid : c_int, grp_name : c_string, ref grp_ncid : c_int) : c_int;

    extern proc nc_inq_grp_full_ncid(ncid : c_int, full_name : c_string, ref grp_ncid : c_int) : c_int;

    extern proc nc_inq_varids(ncid : c_int, ref nvars : c_int, ref varids : c_int) : c_int;

    extern proc nc_inq_dimids(ncid : c_int, ref ndims : c_int, ref dimids : c_int, include_parents : c_int) : c_int;

    extern proc nc_inq_typeids(ncid : c_int, ref ntypes : c_int, ref typeids : c_int) : c_int;

    extern proc nc_inq_type_equal(ncid1 : c_int, typeid1 : nc_type, ncid2 : c_int, typeid2 : nc_type, ref equal : c_int) : c_int;

    extern proc nc_def_grp(parent_ncid : c_int, name : c_string, ref new_ncid : c_int) : c_int;

    extern proc nc_rename_grp(grpid : c_int, name : c_string) : c_int;

    extern proc nc_def_compound(ncid : c_int, size : size_t, name : c_string, ref typeidp : nc_type) : c_int;

    extern proc nc_insert_compound(ncid : c_int, xtype : nc_type, name : c_string, offset : size_t, field_typeid : nc_type) : c_int;

    extern proc nc_insert_array_compound(ncid : c_int, xtype : nc_type, name : c_string, offset : size_t, field_typeid : nc_type, ndims : c_int, ref dim_sizes : c_int) : c_int;

    extern proc nc_inq_type(ncid : c_int, xtype : nc_type, name : c_string, ref size : size_t) : c_int;

    extern proc nc_inq_typeid(ncid : c_int, name : c_string, ref typeidp : nc_type) : c_int;

    extern proc nc_inq_compound(ncid : c_int, xtype : nc_type, name : c_string, ref sizep : size_t, ref nfieldsp : size_t) : c_int;

    extern proc nc_inq_compound_name(ncid : c_int, xtype : nc_type, name : c_string) : c_int;

    extern proc nc_inq_compound_size(ncid : c_int, xtype : nc_type, ref sizep : size_t) : c_int;

    extern proc nc_inq_compound_nfields(ncid : c_int, xtype : nc_type, ref nfieldsp : size_t) : c_int;

    extern proc nc_inq_compound_field(ncid : c_int, xtype : nc_type, fieldid : c_int, name : c_string, ref offsetp : size_t, ref field_typeidp : nc_type, ref ndimsp : c_int, ref dim_sizesp : c_int) : c_int;

    extern proc nc_inq_compound_fieldname(ncid : c_int, xtype : nc_type, fieldid : c_int, name : c_string) : c_int;

    extern proc nc_inq_compound_fieldindex(ncid : c_int, xtype : nc_type, name : c_string, ref fieldidp : c_int) : c_int;

    extern proc nc_inq_compound_fieldoffset(ncid : c_int, xtype : nc_type, fieldid : c_int, ref offsetp : size_t) : c_int;

    extern proc nc_inq_compound_fieldtype(ncid : c_int, xtype : nc_type, fieldid : c_int, ref field_typeidp : nc_type) : c_int;

    extern proc nc_inq_compound_fieldndims(ncid : c_int, xtype : nc_type, fieldid : c_int, ref ndimsp : c_int) : c_int;

    extern proc nc_inq_compound_fielddim_sizes(ncid : c_int, xtype : nc_type, fieldid : c_int, ref dim_sizes : c_int) : c_int;

    extern proc nc_def_vlen(ncid : c_int, name : c_string, base_typeid : nc_type, ref xtypep : nc_type) : c_int;

    extern proc nc_inq_vlen(ncid : c_int, xtype : nc_type, name : c_string, ref datum_sizep : size_t, ref base_nc_typep : nc_type) : c_int;

    extern proc nc_free_vlen(ref vl : nc_vlen_t) : c_int;

    extern proc nc_free_vlens(len : size_t, vlens : c_ptr(nc_vlen_t)) : c_int;

    extern proc nc_put_vlen_element(ncid : c_int, typeid1 : c_int, vlen_element : c_void_ptr, len : size_t, data : c_void_ptr) : c_int;

    extern proc nc_get_vlen_element(ncid : c_int, typeid1 : c_int, vlen_element : c_void_ptr, ref len : size_t, data : c_void_ptr) : c_int;

    extern proc nc_free_string(len : size_t, ref data : c_string) : c_int;

    extern proc nc_inq_user_type(ncid : c_int, xtype : nc_type, name : c_string, ref size : size_t, ref base_nc_typep : nc_type, ref nfieldsp : size_t, ref classp : c_int) : c_int;

    extern proc nc_put_att(ncid : c_int, varid : c_int, name : c_string, xtype : nc_type, len : size_t, op : c_void_ptr) : c_int;

    extern proc nc_get_att(ncid : c_int, varid : c_int, name : c_string, ip : c_void_ptr) : c_int;

    extern proc nc_def_enum(ncid : c_int, base_typeid : nc_type, name : c_string, ref typeidp : nc_type) : c_int;

    extern proc nc_insert_enum(ncid : c_int, xtype : nc_type, name : c_string, value : c_void_ptr) : c_int;

    extern proc nc_inq_enum(ncid : c_int, xtype : nc_type, name : c_string, ref base_nc_typep : nc_type, ref base_sizep : size_t, ref num_membersp : size_t) : c_int;

    extern proc nc_inq_enum_member(ncid : c_int, xtype : nc_type, idx : c_int, name : c_string, value : c_void_ptr) : c_int;

    extern proc nc_inq_enum_ident(ncid : c_int, xtype : nc_type, value : c_longlong, identifier : c_string) : c_int;

    extern proc nc_def_opaque(ncid : c_int, size : size_t, name : c_string, ref xtypep : nc_type) : c_int;

    extern proc nc_inq_opaque(ncid : c_int, xtype : nc_type, name : c_string, ref sizep : size_t) : c_int;

    extern proc nc_put_var(ncid : c_int, varid : c_int, op : c_void_ptr) : c_int;

    extern proc nc_get_var(ncid : c_int, varid : c_int, ip : c_void_ptr) : c_int;

    extern proc nc_put_var1(ncid : c_int, varid : c_int, ref indexp : size_t, op : c_void_ptr) : c_int;

    extern proc nc_get_var1(ncid : c_int, varid : c_int, ref indexp : size_t, ip : c_void_ptr) : c_int;

    extern proc nc_put_vara(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, op : c_void_ptr) : c_int;

    extern proc nc_get_vara(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ip : c_void_ptr) : c_int;

    extern proc nc_put_vars(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, op : c_void_ptr) : c_int;

    extern proc nc_get_vars(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ip : c_void_ptr) : c_int;

    extern proc nc_put_varm(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, op : c_void_ptr) : c_int;

    extern proc nc_get_varm(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ip : c_void_ptr) : c_int;

    extern proc nc_def_var_deflate(ncid : c_int, varid : c_int, shuffle : c_int, deflate : c_int, deflate_level : c_int) : c_int;

    extern proc nc_inq_var_deflate(ncid : c_int, varid : c_int, ref shufflep : c_int, ref deflatep : c_int, ref deflate_levelp : c_int) : c_int;

    extern proc nc_inq_var_szip(ncid : c_int, varid : c_int, ref options_maskp : c_int, ref pixels_per_blockp : c_int) : c_int;

    extern proc nc_def_var_fletcher32(ncid : c_int, varid : c_int, fletcher32 : c_int) : c_int;

    extern proc nc_inq_var_fletcher32(ncid : c_int, varid : c_int, ref fletcher32p : c_int) : c_int;

    extern proc nc_def_var_chunking(ncid : c_int, varid : c_int, storage : c_int, ref chunksizesp : size_t) : c_int;

    extern proc nc_inq_var_chunking(ncid : c_int, varid : c_int, ref storagep : c_int, ref chunksizesp : size_t) : c_int;

    extern proc nc_def_var_fill(ncid : c_int, varid : c_int, no_fill : c_int, fill_value : c_void_ptr) : c_int;

    extern proc nc_inq_var_fill(ncid : c_int, varid : c_int, ref no_fill : c_int, fill_valuep : c_void_ptr) : c_int;

    extern proc nc_def_var_endian(ncid : c_int, varid : c_int, endian : c_int) : c_int;

    extern proc nc_inq_var_endian(ncid : c_int, varid : c_int, ref endianp : c_int) : c_int;

    extern proc nc_def_var_filter(ncid : c_int, varid : c_int, id : c_uint, nparams : size_t, ref parms : c_uint) : c_int;

    extern proc nc_inq_var_filter(ncid : c_int, varid : c_int, ref idp : c_uint, ref nparams : size_t, ref params : c_uint) : c_int;

    extern proc nc_set_fill(ncid : c_int, fillmode : c_int, ref old_modep : c_int) : c_int;

    extern proc nc_set_default_format(format : c_int, ref old_formatp : c_int) : c_int;

    extern proc nc_set_chunk_cache(size : size_t, nelems : size_t, preemption : c_float) : c_int;

    extern proc nc_get_chunk_cache(ref sizep : size_t, ref nelemsp : size_t, ref preemptionp : c_float) : c_int;

    extern proc nc_set_var_chunk_cache(ncid : c_int, varid : c_int, size : size_t, nelems : size_t, preemption : c_float) : c_int;

    extern proc nc_get_var_chunk_cache(ncid : c_int, varid : c_int, ref sizep : size_t, ref nelemsp : size_t, ref preemptionp : c_float) : c_int;

    extern proc nc_redef(ncid : c_int) : c_int;

    extern proc nc__enddef(ncid : c_int, h_minfree : size_t, v_align : size_t, v_minfree : size_t, r_align : size_t) : c_int;

    extern proc nc_enddef(ncid : c_int) : c_int;

    extern proc nc_sync(ncid : c_int) : c_int;

    extern proc nc_abort(ncid : c_int) : c_int;

    extern proc nc_close(ncid : c_int) : c_int;

    extern proc nc_inq(ncid : c_int, ref ndimsp : c_int, ref nvarsp : c_int, ref nattsp : c_int, ref unlimdimidp : c_int) : c_int;

    extern proc nc_inq_ndims(ncid : c_int, ref ndimsp : c_int) : c_int;

    extern proc nc_inq_nvars(ncid : c_int, ref nvarsp : c_int) : c_int;

    extern proc nc_inq_natts(ncid : c_int, ref nattsp : c_int) : c_int;

    extern proc nc_inq_unlimdim(ncid : c_int, ref unlimdimidp : c_int) : c_int;

    extern proc nc_inq_unlimdims(ncid : c_int, ref nunlimdimsp : c_int, ref unlimdimidsp : c_int) : c_int;

    extern proc nc_inq_format(ncid : c_int, ref formatp : c_int) : c_int;

    extern proc nc_inq_format_extended(ncid : c_int, ref formatp : c_int, ref modep : c_int) : c_int;

    extern proc nc_def_dim(ncid : c_int, name : c_string, len : size_t, ref idp : c_int) : c_int;

    extern proc nc_inq_dimid(ncid : c_int, name : c_string, ref idp : c_int) : c_int;

    extern proc nc_inq_dim(ncid : c_int, dimid : c_int, name : c_string, ref lenp : size_t) : c_int;

    extern proc nc_inq_dimname(ncid : c_int, dimid : c_int, name : c_string) : c_int;

    extern proc nc_inq_dimlen(ncid : c_int, dimid : c_int, ref lenp : size_t) : c_int;

    extern proc nc_rename_dim(ncid : c_int, dimid : c_int, name : c_string) : c_int;

    extern proc nc_inq_att(ncid : c_int, varid : c_int, name : c_string, ref xtypep : nc_type, ref lenp : size_t) : c_int;

    extern proc nc_inq_attid(ncid : c_int, varid : c_int, name : c_string, ref idp : c_int) : c_int;

    extern proc nc_inq_atttype(ncid : c_int, varid : c_int, name : c_string, ref xtypep : nc_type) : c_int;

    extern proc nc_inq_attlen(ncid : c_int, varid : c_int, name : c_string, ref lenp : size_t) : c_int;

    extern proc nc_inq_attname(ncid : c_int, varid : c_int, attnum : c_int, name : c_string) : c_int;

    extern proc nc_copy_att(ncid_in : c_int, varid_in : c_int, name : c_string, ncid_out : c_int, varid_out : c_int) : c_int;

    extern proc nc_rename_att(ncid : c_int, varid : c_int, name : c_string, newname : c_string) : c_int;

    extern proc nc_del_att(ncid : c_int, varid : c_int, name : c_string) : c_int;

    extern proc nc_put_att_text(ncid : c_int, varid : c_int, name : c_string, len : size_t, op : c_string) : c_int;

    extern proc nc_get_att_text(ncid : c_int, varid : c_int, name : c_string, ip : c_string) : c_int;

    extern proc nc_put_att_string(ncid : c_int, varid : c_int, name : c_string, len : size_t, ref op : c_string) : c_int;

    extern proc nc_get_att_string(ncid : c_int, varid : c_int, name : c_string, ref ip : c_string) : c_int;

    extern proc nc_put_att_uchar(ncid : c_int, varid : c_int, name : c_string, xtype : nc_type, len : size_t, ref op : c_uchar) : c_int;

    extern proc nc_get_att_uchar(ncid : c_int, varid : c_int, name : c_string, ref ip : c_uchar) : c_int;

    extern proc nc_put_att_schar(ncid : c_int, varid : c_int, name : c_string, xtype : nc_type, len : size_t, ref op : c_schar) : c_int;

    extern proc nc_get_att_schar(ncid : c_int, varid : c_int, name : c_string, ref ip : c_schar) : c_int;

    extern proc nc_put_att_short(ncid : c_int, varid : c_int, name : c_string, xtype : nc_type, len : size_t, ref op : c_short) : c_int;

    extern proc nc_get_att_short(ncid : c_int, varid : c_int, name : c_string, ref ip : c_short) : c_int;

    extern proc nc_put_att_int(ncid : c_int, varid : c_int, name : c_string, xtype : nc_type, len : size_t, ref op : c_int) : c_int;

    extern proc nc_get_att_int(ncid : c_int, varid : c_int, name : c_string, ref ip : c_int) : c_int;

    extern proc nc_put_att_long(ncid : c_int, varid : c_int, name : c_string, xtype : nc_type, len : size_t, ref op : c_long) : c_int;

    extern proc nc_get_att_long(ncid : c_int, varid : c_int, name : c_string, ref ip : c_long) : c_int;

    extern proc nc_put_att_float(ncid : c_int, varid : c_int, name : c_string, xtype : nc_type, len : size_t, ref op : c_float) : c_int;

    extern proc nc_get_att_float(ncid : c_int, varid : c_int, name : c_string, ref ip : c_float) : c_int;

    extern proc nc_put_att_double(ncid : c_int, varid : c_int, name : c_string, xtype : nc_type, len : size_t, ref op : c_double) : c_int;

    extern proc nc_get_att_double(ncid : c_int, varid : c_int, name : c_string, ref ip : c_double) : c_int;

    extern proc nc_put_att_ushort(ncid : c_int, varid : c_int, name : c_string, xtype : nc_type, len : size_t, ref op : c_ushort) : c_int;

    extern proc nc_get_att_ushort(ncid : c_int, varid : c_int, name : c_string, ref ip : c_ushort) : c_int;

    extern proc nc_put_att_uint(ncid : c_int, varid : c_int, name : c_string, xtype : nc_type, len : size_t, ref op : c_uint) : c_int;

    extern proc nc_get_att_uint(ncid : c_int, varid : c_int, name : c_string, ref ip : c_uint) : c_int;

    extern proc nc_put_att_longlong(ncid : c_int, varid : c_int, name : c_string, xtype : nc_type, len : size_t, ref op : c_longlong) : c_int;

    extern proc nc_get_att_longlong(ncid : c_int, varid : c_int, name : c_string, ref ip : c_longlong) : c_int;

    extern proc nc_put_att_ulonglong(ncid : c_int, varid : c_int, name : c_string, xtype : nc_type, len : size_t, ref op : c_ulonglong) : c_int;

    extern proc nc_get_att_ulonglong(ncid : c_int, varid : c_int, name : c_string, ref ip : c_ulonglong) : c_int;

    extern proc nc_def_var(ncid : c_int, name : c_string, xtype : nc_type, ndims : c_int, ref dimidsp : c_int, ref varidp : c_int) : c_int;

    extern proc nc_inq_var(ncid : c_int, varid : c_int, name : c_string, ref xtypep : nc_type, ref ndimsp : c_int, ref dimidsp : c_int, ref nattsp : c_int) : c_int;

    extern proc nc_inq_varid(ncid : c_int, name : c_string, ref varidp : c_int) : c_int;

    extern proc nc_inq_varname(ncid : c_int, varid : c_int, name : c_string) : c_int;

    extern proc nc_inq_vartype(ncid : c_int, varid : c_int, ref xtypep : nc_type) : c_int;

    extern proc nc_inq_varndims(ncid : c_int, varid : c_int, ref ndimsp : c_int) : c_int;

    extern proc nc_inq_vardimid(ncid : c_int, varid : c_int, ref dimidsp : c_int) : c_int;

    extern proc nc_inq_varnatts(ncid : c_int, varid : c_int, ref nattsp : c_int) : c_int;

    extern proc nc_rename_var(ncid : c_int, varid : c_int, name : c_string) : c_int;

    extern proc nc_copy_var(ncid_in : c_int, varid : c_int, ncid_out : c_int) : c_int;

    extern proc nc_put_var1_text(ncid : c_int, varid : c_int, ref indexp : size_t, op : c_string) : c_int;

    extern proc nc_get_var1_text(ncid : c_int, varid : c_int, ref indexp : size_t, ip : c_string) : c_int;

    extern proc nc_put_var1_uchar(ncid : c_int, varid : c_int, ref indexp : size_t, ref op : c_uchar) : c_int;

    extern proc nc_get_var1_uchar(ncid : c_int, varid : c_int, ref indexp : size_t, ref ip : c_uchar) : c_int;

    extern proc nc_put_var1_schar(ncid : c_int, varid : c_int, ref indexp : size_t, ref op : c_schar) : c_int;

    extern proc nc_get_var1_schar(ncid : c_int, varid : c_int, ref indexp : size_t, ref ip : c_schar) : c_int;

    extern proc nc_put_var1_short(ncid : c_int, varid : c_int, ref indexp : size_t, ref op : c_short) : c_int;

    extern proc nc_get_var1_short(ncid : c_int, varid : c_int, ref indexp : size_t, ref ip : c_short) : c_int;

    extern proc nc_put_var1_int(ncid : c_int, varid : c_int, ref indexp : size_t, ref op : c_int) : c_int;

    extern proc nc_get_var1_int(ncid : c_int, varid : c_int, ref indexp : size_t, ref ip : c_int) : c_int;

    extern proc nc_put_var1_long(ncid : c_int, varid : c_int, ref indexp : size_t, ref op : c_long) : c_int;

    extern proc nc_get_var1_long(ncid : c_int, varid : c_int, ref indexp : size_t, ref ip : c_long) : c_int;

    extern proc nc_put_var1_float(ncid : c_int, varid : c_int, ref indexp : size_t, ref op : c_float) : c_int;

    extern proc nc_get_var1_float(ncid : c_int, varid : c_int, ref indexp : size_t, ref ip : c_float) : c_int;

    extern proc nc_put_var1_double(ncid : c_int, varid : c_int, ref indexp : size_t, ref op : c_double) : c_int;

    extern proc nc_get_var1_double(ncid : c_int, varid : c_int, ref indexp : size_t, ref ip : c_double) : c_int;

    extern proc nc_put_var1_ushort(ncid : c_int, varid : c_int, ref indexp : size_t, ref op : c_ushort) : c_int;

    extern proc nc_get_var1_ushort(ncid : c_int, varid : c_int, ref indexp : size_t, ref ip : c_ushort) : c_int;

    extern proc nc_put_var1_uint(ncid : c_int, varid : c_int, ref indexp : size_t, ref op : c_uint) : c_int;

    extern proc nc_get_var1_uint(ncid : c_int, varid : c_int, ref indexp : size_t, ref ip : c_uint) : c_int;

    extern proc nc_put_var1_longlong(ncid : c_int, varid : c_int, ref indexp : size_t, ref op : c_longlong) : c_int;

    extern proc nc_get_var1_longlong(ncid : c_int, varid : c_int, ref indexp : size_t, ref ip : c_longlong) : c_int;

    extern proc nc_put_var1_ulonglong(ncid : c_int, varid : c_int, ref indexp : size_t, ref op : c_ulonglong) : c_int;

    extern proc nc_get_var1_ulonglong(ncid : c_int, varid : c_int, ref indexp : size_t, ref ip : c_ulonglong) : c_int;

    extern proc nc_put_var1_string(ncid : c_int, varid : c_int, ref indexp : size_t, ref op : c_string) : c_int;

    extern proc nc_get_var1_string(ncid : c_int, varid : c_int, ref indexp : size_t, ref ip : c_string) : c_int;

    extern proc nc_put_vara_text(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, op : c_string) : c_int;

    extern proc nc_get_vara_text(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ip : c_string) : c_int;

    extern proc nc_put_vara_uchar(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref op : c_uchar) : c_int;

    extern proc nc_get_vara_uchar(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref ip : c_uchar) : c_int;

    extern proc nc_put_vara_schar(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref op : c_schar) : c_int;

    extern proc nc_get_vara_schar(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref ip : c_schar) : c_int;

    extern proc nc_put_vara_short(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref op : c_short) : c_int;

    extern proc nc_get_vara_short(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref ip : c_short) : c_int;

    extern proc nc_put_vara_int(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref op : c_int) : c_int;

    extern proc nc_get_vara_int(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref ip : c_int) : c_int;

    extern proc nc_put_vara_long(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref op : c_long) : c_int;

    extern proc nc_get_vara_long(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref ip : c_long) : c_int;

    extern proc nc_put_vara_float(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref op : c_float) : c_int;

    extern proc nc_get_vara_float(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref ip : c_float) : c_int;

    extern proc nc_put_vara_double(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref op : c_double) : c_int;

    extern proc nc_get_vara_double(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref ip : c_double) : c_int;

    extern proc nc_put_vara_ushort(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref op : c_ushort) : c_int;

    extern proc nc_get_vara_ushort(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref ip : c_ushort) : c_int;

    extern proc nc_put_vara_uint(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref op : c_uint) : c_int;

    extern proc nc_get_vara_uint(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref ip : c_uint) : c_int;

    extern proc nc_put_vara_longlong(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref op : c_longlong) : c_int;

    extern proc nc_get_vara_longlong(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref ip : c_longlong) : c_int;

    extern proc nc_put_vara_ulonglong(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref op : c_ulonglong) : c_int;

    extern proc nc_get_vara_ulonglong(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref ip : c_ulonglong) : c_int;

    extern proc nc_put_vara_string(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref op : c_string) : c_int;

    extern proc nc_get_vara_string(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref ip : c_string) : c_int;

    extern proc nc_put_vars_text(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, op : c_string) : c_int;

    extern proc nc_get_vars_text(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ip : c_string) : c_int;

    extern proc nc_put_vars_uchar(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref op : c_uchar) : c_int;

    extern proc nc_get_vars_uchar(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref ip : c_uchar) : c_int;

    extern proc nc_put_vars_schar(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref op : c_schar) : c_int;

    extern proc nc_get_vars_schar(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref ip : c_schar) : c_int;

    extern proc nc_put_vars_short(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref op : c_short) : c_int;

    extern proc nc_get_vars_short(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref ip : c_short) : c_int;

    extern proc nc_put_vars_int(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref op : c_int) : c_int;

    extern proc nc_get_vars_int(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref ip : c_int) : c_int;

    extern proc nc_put_vars_long(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref op : c_long) : c_int;

    extern proc nc_get_vars_long(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref ip : c_long) : c_int;

    extern proc nc_put_vars_float(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref op : c_float) : c_int;

    extern proc nc_get_vars_float(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref ip : c_float) : c_int;

    extern proc nc_put_vars_double(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref op : c_double) : c_int;

    extern proc nc_get_vars_double(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref ip : c_double) : c_int;

    extern proc nc_put_vars_ushort(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref op : c_ushort) : c_int;

    extern proc nc_get_vars_ushort(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref ip : c_ushort) : c_int;

    extern proc nc_put_vars_uint(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref op : c_uint) : c_int;

    extern proc nc_get_vars_uint(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref ip : c_uint) : c_int;

    extern proc nc_put_vars_longlong(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref op : c_longlong) : c_int;

    extern proc nc_get_vars_longlong(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref ip : c_longlong) : c_int;

    extern proc nc_put_vars_ulonglong(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref op : c_ulonglong) : c_int;

    extern proc nc_get_vars_ulonglong(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref ip : c_ulonglong) : c_int;

    extern proc nc_put_vars_string(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref op : c_string) : c_int;

    extern proc nc_get_vars_string(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref ip : c_string) : c_int;

    extern proc nc_put_varm_text(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, op : c_string) : c_int;

    extern proc nc_get_varm_text(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ip : c_string) : c_int;

    extern proc nc_put_varm_uchar(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref op : c_uchar) : c_int;

    extern proc nc_get_varm_uchar(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref ip : c_uchar) : c_int;

    extern proc nc_put_varm_schar(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref op : c_schar) : c_int;

    extern proc nc_get_varm_schar(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref ip : c_schar) : c_int;

    extern proc nc_put_varm_short(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref op : c_short) : c_int;

    extern proc nc_get_varm_short(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref ip : c_short) : c_int;

    extern proc nc_put_varm_int(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref op : c_int) : c_int;

    extern proc nc_get_varm_int(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref ip : c_int) : c_int;

    extern proc nc_put_varm_long(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref op : c_long) : c_int;

    extern proc nc_get_varm_long(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref ip : c_long) : c_int;

    extern proc nc_put_varm_float(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref op : c_float) : c_int;

    extern proc nc_get_varm_float(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref ip : c_float) : c_int;

    extern proc nc_put_varm_double(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref op : c_double) : c_int;

    extern proc nc_get_varm_double(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref ip : c_double) : c_int;

    extern proc nc_put_varm_ushort(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref op : c_ushort) : c_int;

    extern proc nc_get_varm_ushort(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref ip : c_ushort) : c_int;

    extern proc nc_put_varm_uint(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref op : c_uint) : c_int;

    extern proc nc_get_varm_uint(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref ip : c_uint) : c_int;

    extern proc nc_put_varm_longlong(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref op : c_longlong) : c_int;

    extern proc nc_get_varm_longlong(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref ip : c_longlong) : c_int;

    extern proc nc_put_varm_ulonglong(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref op : c_ulonglong) : c_int;

    extern proc nc_get_varm_ulonglong(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref ip : c_ulonglong) : c_int;

    extern proc nc_put_varm_string(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref op : c_string) : c_int;

    extern proc nc_get_varm_string(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref ip : c_string) : c_int;

    extern proc nc_put_var_text(ncid : c_int, varid : c_int, op : c_string) : c_int;

    extern proc nc_get_var_text(ncid : c_int, varid : c_int, ip : c_string) : c_int;

    extern proc nc_put_var_uchar(ncid : c_int, varid : c_int, ref op : c_uchar) : c_int;

    extern proc nc_get_var_uchar(ncid : c_int, varid : c_int, ref ip : c_uchar) : c_int;

    extern proc nc_put_var_schar(ncid : c_int, varid : c_int, ref op : c_schar) : c_int;

    extern proc nc_get_var_schar(ncid : c_int, varid : c_int, ref ip : c_schar) : c_int;

    extern proc nc_put_var_short(ncid : c_int, varid : c_int, ref op : c_short) : c_int;

    extern proc nc_get_var_short(ncid : c_int, varid : c_int, ref ip : c_short) : c_int;

    extern proc nc_put_var_int(ncid : c_int, varid : c_int, ref op : c_int) : c_int;

    extern proc nc_get_var_int(ncid : c_int, varid : c_int, ref ip : c_int) : c_int;

    extern proc nc_put_var_long(ncid : c_int, varid : c_int, ref op : c_long) : c_int;

    extern proc nc_get_var_long(ncid : c_int, varid : c_int, ref ip : c_long) : c_int;

    extern proc nc_put_var_float(ncid : c_int, varid : c_int, ref op : c_float) : c_int;

    extern proc nc_get_var_float(ncid : c_int, varid : c_int, ref ip : c_float) : c_int;

    extern proc nc_put_var_double(ncid : c_int, varid : c_int, ref op : c_double) : c_int;

    extern proc nc_get_var_double(ncid : c_int, varid : c_int, ref ip : c_double) : c_int;

    extern proc nc_put_var_ushort(ncid : c_int, varid : c_int, ref op : c_ushort) : c_int;

    extern proc nc_get_var_ushort(ncid : c_int, varid : c_int, ref ip : c_ushort) : c_int;

    extern proc nc_put_var_uint(ncid : c_int, varid : c_int, ref op : c_uint) : c_int;

    extern proc nc_get_var_uint(ncid : c_int, varid : c_int, ref ip : c_uint) : c_int;

    extern proc nc_put_var_longlong(ncid : c_int, varid : c_int, ref op : c_longlong) : c_int;

    extern proc nc_get_var_longlong(ncid : c_int, varid : c_int, ref ip : c_longlong) : c_int;

    extern proc nc_put_var_ulonglong(ncid : c_int, varid : c_int, ref op : c_ulonglong) : c_int;

    extern proc nc_get_var_ulonglong(ncid : c_int, varid : c_int, ref ip : c_ulonglong) : c_int;

    extern proc nc_put_var_string(ncid : c_int, varid : c_int, ref op : c_string) : c_int;

    extern proc nc_get_var_string(ncid : c_int, varid : c_int, ref ip : c_string) : c_int;

    extern proc nc_put_att_ubyte(ncid : c_int, varid : c_int, name : c_string, xtype : nc_type, len : size_t, ref op : c_uchar) : c_int;

    extern proc nc_get_att_ubyte(ncid : c_int, varid : c_int, name : c_string, ref ip : c_uchar) : c_int;

    extern proc nc_put_var1_ubyte(ncid : c_int, varid : c_int, ref indexp : size_t, ref op : c_uchar) : c_int;

    extern proc nc_get_var1_ubyte(ncid : c_int, varid : c_int, ref indexp : size_t, ref ip : c_uchar) : c_int;

    extern proc nc_put_vara_ubyte(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref op : c_uchar) : c_int;

    extern proc nc_get_vara_ubyte(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref ip : c_uchar) : c_int;

    extern proc nc_put_vars_ubyte(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref op : c_uchar) : c_int;

    extern proc nc_get_vars_ubyte(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref ip : c_uchar) : c_int;

    extern proc nc_put_varm_ubyte(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref op : c_uchar) : c_int;

    extern proc nc_get_varm_ubyte(ncid : c_int, varid : c_int, ref startp : size_t, ref countp : size_t, ref stridep : c_ptrdiff, ref imapp : c_ptrdiff, ref ip : c_uchar) : c_int;

    extern proc nc_put_var_ubyte(ncid : c_int, varid : c_int, ref op : c_uchar) : c_int;

    extern proc nc_get_var_ubyte(ncid : c_int, varid : c_int, ref ip : c_uchar) : c_int;

    extern proc nc_show_metadata(ncid : c_int) : c_int;

    extern proc nc__create_mp(path : c_string, cmode : c_int, initialsz : size_t, basepe : c_int, ref chunksizehintp : size_t, ref ncidp : c_int) : c_int;

    extern proc nc__open_mp(path : c_string, mode : c_int, basepe : c_int, ref chunksizehintp : size_t, ref ncidp : c_int) : c_int;

    extern proc nc_delete(path : c_string) : c_int;

    extern proc nc_delete_mp(path : c_string, basepe : c_int) : c_int;

    extern proc nc_set_base_pe(ncid : c_int, pe : c_int) : c_int;

    extern proc nc_inq_base_pe(ncid : c_int, ref pe : c_int) : c_int;

    extern proc nctypelen(datatype : nc_type) : c_int;

    extern var ncerr : c_int;

    extern var ncopts : c_int;

    extern proc nc_advise(cdf_routine_name : c_string, err : c_int, fmt : c_string, c__varargs ...) : void;

    // Overload for empty varargs
    extern proc nc_advise(cdf_routine_name : c_string,err : c_int,fmt : c_string) : void;

    extern proc nccreate(path : c_string, cmode : c_int) : c_int;

    extern proc ncopen(path : c_string, mode : c_int) : c_int;

    extern proc ncsetfill(ncid : c_int, fillmode : c_int) : c_int;

    extern proc ncredef(ncid : c_int) : c_int;

    extern proc ncendef(ncid : c_int) : c_int;

    extern proc ncsync(ncid : c_int) : c_int;

    extern proc ncabort(ncid : c_int) : c_int;

    extern proc ncclose(ncid : c_int) : c_int;

    extern proc ncinquire(ncid : c_int, ref ndimsp : c_int, ref nvarsp : c_int, ref nattsp : c_int, ref unlimdimp : c_int) : c_int;

    extern proc ncdimdef(ncid : c_int, name : c_string, len : c_long) : c_int;

    extern proc ncdimid(ncid : c_int, name : c_string) : c_int;

    extern proc ncdiminq(ncid : c_int, dimid : c_int, name : c_string, ref lenp : c_long) : c_int;

    extern proc ncdimrename(ncid : c_int, dimid : c_int, name : c_string) : c_int;

    extern proc ncattput(ncid : c_int, varid : c_int, name : c_string, xtype : nc_type, len : c_int, op : c_void_ptr) : c_int;

    extern proc ncattinq(ncid : c_int, varid : c_int, name : c_string, ref xtypep : nc_type, ref lenp : c_int) : c_int;

    extern proc ncattget(ncid : c_int, varid : c_int, name : c_string, ip : c_void_ptr) : c_int;

    extern proc ncattcopy(ncid_in : c_int, varid_in : c_int, name : c_string, ncid_out : c_int, varid_out : c_int) : c_int;

    extern proc ncattname(ncid : c_int, varid : c_int, attnum : c_int, name : c_string) : c_int;

    extern proc ncattrename(ncid : c_int, varid : c_int, name : c_string, newname : c_string) : c_int;

    extern proc ncattdel(ncid : c_int, varid : c_int, name : c_string) : c_int;

    extern proc ncvardef(ncid : c_int, name : c_string, xtype : nc_type, ndims : c_int, ref dimidsp : c_int) : c_int;

    extern proc ncvarid(ncid : c_int, name : c_string) : c_int;

    extern proc ncvarinq(ncid : c_int, varid : c_int, name : c_string, ref xtypep : nc_type, ref ndimsp : c_int, ref dimidsp : c_int, ref nattsp : c_int) : c_int;

    extern proc ncvarput1(ncid : c_int, varid : c_int, ref indexp : c_long, op : c_void_ptr) : c_int;

    extern proc ncvarget1(ncid : c_int, varid : c_int, ref indexp : c_long, ip : c_void_ptr) : c_int;

    extern proc ncvarput(ncid : c_int, varid : c_int, ref startp : c_long, ref countp : c_long, op : c_void_ptr) : c_int;

    extern proc ncvarget(ncid : c_int, varid : c_int, ref startp : c_long, ref countp : c_long, ip : c_void_ptr) : c_int;

    extern proc ncvarputs(ncid : c_int, varid : c_int, ref startp : c_long, ref countp : c_long, ref stridep : c_long, op : c_void_ptr) : c_int;

    extern proc ncvargets(ncid : c_int, varid : c_int, ref startp : c_long, ref countp : c_long, ref stridep : c_long, ip : c_void_ptr) : c_int;

    extern proc ncvarputg(ncid : c_int, varid : c_int, ref startp : c_long, ref countp : c_long, ref stridep : c_long, ref imapp : c_long, op : c_void_ptr) : c_int;

    extern proc ncvargetg(ncid : c_int, varid : c_int, ref startp : c_long, ref countp : c_long, ref stridep : c_long, ref imapp : c_long, ip : c_void_ptr) : c_int;

    extern proc ncvarrename(ncid : c_int, varid : c_int, name : c_string) : c_int;

    extern proc ncrecinq(ncid : c_int, ref nrecvarsp : c_int, ref recvaridsp : c_int, ref recsizesp : c_long) : c_int;

    extern proc ncrecget(ncid : c_int, recnum : c_long, ref datap : c_void_ptr) : c_int;

    extern proc ncrecput(ncid : c_int, recnum : c_long, ref datap : c_void_ptr) : c_int;

    extern proc nc_finalize() : c_int;

    // ==== c2chapel typedefs ====

    extern type nc_type = c_int;

    extern record nc_vlen_t {
      var len : size_t;
      var p : c_void_ptr;
    }

    extern type nclong = c_int;
  }
}
