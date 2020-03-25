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

/* NetCDF bindings for Chapel

This module implements the C-API for netCDF version 4.6.1.

Compiling with netCDF
---------------------

In order to compile a Chapel program that uses this module, the netCDF library
must be installed on the system.  The paths to the ``netcdf.h`` header file
and netCDF library must be known to the compiler, either by finding them in
a default search path, or by using the ``-I`` and ``-L`` compiler arguments.

The compilation command should look something like this:

.. code-block:: sh

    chpl -I$PATH_TO_NETCDF_DIR \
         -L$PATH_TO_NETCDF_LIBS -lnetcdf source.chpl

On Cray systems with the ``cray-netcdf`` module loaded, no compiler flags
are necessary to use the HDF5 module.
*/
module NetCDF {
  /* The NetCDF module defines the interface to the netCDF library.
     Documentation for its functions, types, and constants can be found
     at the official netCDF web site:
     https://www.unidata.ucar.edu/software/netcdf/docs
   */
  module C_NetCDF {
    // Generated with c2chapel version 0.1.0

    // Header given to c2chapel:
    require "netcdf.h";
    require "-lnetcdf";

    // #define'd literals:
    extern const NC_NAT : c_int;
    extern const NC_BYTE : c_int;
    extern const NC_CHAR : c_int;
    extern const NC_SHORT : c_int;
    extern const NC_INT : c_int;
    extern const NC_LONG : c_int;
    extern const NC_FLOAT : c_int;
    extern const NC_DOUBLE : c_int;
    extern const NC_UBYTE : c_int;
    extern const NC_USHORT : c_int;
    extern const NC_UINT : c_int;
    extern const NC_INT64 : c_int;
    extern const NC_UINT64 : c_int;
    extern const NC_STRING : c_int;
    extern const NC_MAX_ATOMIC_TYPE : c_int;
    extern const NC_VLEN : c_int;
    extern const NC_OPAQUE : c_int;
    extern const NC_ENUM : c_int;
    extern const NC_COMPOUND : c_int;
    extern const NC_FIRSTUSERTYPEID : c_int;
    extern const NC_FILL_BYTE : c_char;
    extern const NC_FILL_CHAR : c_char;
    extern const NC_FILL_SHORT : c_short;
    extern const NC_FILL_INT : c_int;
    extern const NC_FILL_FLOAT : real(32);
    extern const NC_FILL_DOUBLE : real;
    extern const NC_FILL_UBYTE : c_int;
    extern const NC_FILL_USHORT : c_int;
    extern const NC_FILL_UINT : c_int;
    extern const NC_FILL_INT64 : c_longlong;
    extern const NC_FILL_UINT64 : c_ulonglong;
    extern const NC_FILL_STRING : c_string;
    extern const NC_MAX_BYTE : c_int;
    extern const NC_MIN_BYTE : c_int;
    extern const NC_MAX_CHAR : c_int;
    extern const NC_MAX_SHORT : c_int;
    extern const NC_MIN_SHORT : c_int;
    extern const NC_MAX_INT : c_int;
    extern const NC_MIN_INT : c_int;
    extern const NC_MAX_FLOAT : real(32);
    extern const NC_MIN_FLOAT : real(32);
    extern const NC_MAX_DOUBLE : real;
    extern const NC_MIN_DOUBLE : real;
    extern const NC_MAX_UBYTE : c_int;
    extern const NC_MAX_USHORT : c_int;
    extern const NC_MAX_UINT : c_int;
    extern const NC_MAX_INT64 : c_longlong;
    extern const NC_MIN_INT64 : c_longlong;
    extern const NC_MAX_UINT64 : c_ulonglong;
    extern const _FillValue : c_string;
    extern const NC_FILL : c_int;
    extern const NC_NOFILL : c_int;
    extern const NC_NOWRITE : c_int;
    extern const NC_WRITE : c_int;
    extern const NC_CLOBBER : c_int;
    extern const NC_NOCLOBBER : c_int;
    extern const NC_DISKLESS : c_int;
    extern const NC_MMAP : c_int;
    extern const NC_64BIT_DATA : c_int;
    extern const NC_CDF5 : c_int;
    extern const NC_CLASSIC_MODEL : c_int;
    extern const NC_64BIT_OFFSET : c_int;
    extern const NC_LOCK : c_int;
    extern const NC_SHARE : c_int;
    extern const NC_NETCDF4 : c_int;
    extern const NC_MPIIO : c_int;
    extern const NC_MPIPOSIX : c_int;
    extern const NC_INMEMORY : c_int;
    extern const NC_PNETCDF : c_int;
    extern const NC_FORMAT_CLASSIC : c_int;
    extern const NC_FORMAT_64BIT_OFFSET : c_int;
    extern const NC_FORMAT_64BIT : c_int;
    extern const NC_FORMAT_NETCDF4 : c_int;
    extern const NC_FORMAT_NETCDF4_CLASSIC : c_int;
    extern const NC_FORMAT_64BIT_DATA : c_int;
    extern const NC_FORMAT_CDF5 : c_int;
    extern const NC_FORMATX_NC3 : c_int;
    extern const NC_FORMATX_NC_HDF5 : c_int;
    extern const NC_FORMATX_NC4 : c_int;
    extern const NC_FORMATX_NC_HDF4 : c_int;
    extern const NC_FORMATX_PNETCDF : c_int;
    extern const NC_FORMATX_DAP2 : c_int;
    extern const NC_FORMATX_DAP4 : c_int;
    extern const NC_FORMATX_UNDEFINED : c_int;
    extern const NC_FORMAT_NC3 : c_int;
    extern const NC_FORMAT_NC_HDF5 : c_int;
    extern const NC_FORMAT_NC4 : c_int;
    extern const NC_FORMAT_NC_HDF4 : c_int;
    extern const NC_FORMAT_PNETCDF : c_int;
    extern const NC_FORMAT_DAP2 : c_int;
    extern const NC_FORMAT_DAP4 : c_int;
    extern const NC_FORMAT_UNDEFINED : c_int;
    extern const NC_SIZEHINT_DEFAULT : c_int;
    extern const NC_ALIGN_CHUNK : size_t;
    extern const NC_UNLIMITED : c_int;
    extern const NC_GLOBAL : c_int;
    extern const NC_MAX_DIMS : c_int;
    extern const NC_MAX_ATTRS : c_int;
    extern const NC_MAX_VARS : c_int;
    extern const NC_MAX_NAME : c_int;
    extern const NC_MAX_VAR_DIMS : c_int;
    extern const NC_MAX_HDF4_NAME : c_int;
    extern const NC_ENDIAN_NATIVE : c_int;
    extern const NC_ENDIAN_LITTLE : c_int;
    extern const NC_ENDIAN_BIG : c_int;
    extern const NC_CHUNKED : c_int;
    extern const NC_CONTIGUOUS : c_int;
    extern const NC_NOCHECKSUM : c_int;
    extern const NC_FLETCHER32 : c_int;
    extern const NC_NOSHUFFLE : c_int;
    extern const NC_SHUFFLE : c_int;
    extern const NC_MIN_DEFLATE_LEVEL : c_int;
    extern const NC_MAX_DEFLATE_LEVEL : c_int;
    extern const NC_NOERR : c_int;
    extern const NC2_ERR : c_int;
    extern const NC_EBADID : c_int;
    extern const NC_ENFILE : c_int;
    extern const NC_EEXIST : c_int;
    extern const NC_EINVAL : c_int;
    extern const NC_EPERM : c_int;
    extern const NC_ENOTINDEFINE : c_int;
    extern const NC_EINDEFINE : c_int;
    extern const NC_EINVALCOORDS : c_int;
    extern const NC_EMAXDIMS : c_int;
    extern const NC_ENAMEINUSE : c_int;
    extern const NC_ENOTATT : c_int;
    extern const NC_EMAXATTS : c_int;
    extern const NC_EBADTYPE : c_int;
    extern const NC_EBADDIM : c_int;
    extern const NC_EUNLIMPOS : c_int;
    extern const NC_EMAXVARS : c_int;
    extern const NC_ENOTVAR : c_int;
    extern const NC_EGLOBAL : c_int;
    extern const NC_ENOTNC : c_int;
    extern const NC_ESTS : c_int;
    extern const NC_EMAXNAME : c_int;
    extern const NC_EUNLIMIT : c_int;
    extern const NC_ENORECVARS : c_int;
    extern const NC_ECHAR : c_int;
    extern const NC_EEDGE : c_int;
    extern const NC_ESTRIDE : c_int;
    extern const NC_EBADNAME : c_int;
    extern const NC_ERANGE : c_int;
    extern const NC_ENOMEM : c_int;
    extern const NC_EVARSIZE : c_int;
    extern const NC_EDIMSIZE : c_int;
    extern const NC_ETRUNC : c_int;
    extern const NC_EAXISTYPE : c_int;
    extern const NC_EDAP : c_int;
    extern const NC_ECURL : c_int;
    extern const NC_EIO : c_int;
    extern const NC_ENODATA : c_int;
    extern const NC_EDAPSVC : c_int;
    extern const NC_EDAS : c_int;
    extern const NC_EDDS : c_int;
    extern const NC_EDMR : c_int;
    extern const NC_EDATADDS : c_int;
    extern const NC_EDATADAP : c_int;
    extern const NC_EDAPURL : c_int;
    extern const NC_EDAPCONSTRAINT : c_int;
    extern const NC_ETRANSLATION : c_int;
    extern const NC_EACCESS : c_int;
    extern const NC_EAUTH : c_int;
    extern const NC_ENOTFOUND : c_int;
    extern const NC_ECANTREMOVE : c_int;
    extern const NC_EINTERNAL : c_int;
    extern const NC4_FIRST_ERROR : c_int;
    extern const NC_EHDFERR : c_int;
    extern const NC_ECANTREAD : c_int;
    extern const NC_ECANTWRITE : c_int;
    extern const NC_ECANTCREATE : c_int;
    extern const NC_EFILEMETA : c_int;
    extern const NC_EDIMMETA : c_int;
    extern const NC_EATTMETA : c_int;
    extern const NC_EVARMETA : c_int;
    extern const NC_ENOCOMPOUND : c_int;
    extern const NC_EATTEXISTS : c_int;
    extern const NC_ENOTNC4 : c_int;
    extern const NC_ESTRICTNC3 : c_int;
    extern const NC_ENOTNC3 : c_int;
    extern const NC_ENOPAR : c_int;
    extern const NC_EPARINIT : c_int;
    extern const NC_EBADGRPID : c_int;
    extern const NC_EBADTYPID : c_int;
    extern const NC_ETYPDEFINED : c_int;
    extern const NC_EBADFIELD : c_int;
    extern const NC_EBADCLASS : c_int;
    extern const NC_EMAPTYPE : c_int;
    extern const NC_ELATEFILL : c_int;
    extern const NC_ELATEDEF : c_int;
    extern const NC_EDIMSCALE : c_int;
    extern const NC_ENOGRP : c_int;
    extern const NC_ESTORAGE : c_int;
    extern const NC_EBADCHUNK : c_int;
    extern const NC_ENOTBUILT : c_int;
    extern const NC_EDISKLESS : c_int;
    extern const NC_ECANTEXTEND : c_int;
    extern const NC_EMPI : c_int;
    extern const NC_EFILTER : c_int;
    extern const NC_ERCFILE : c_int;
    extern const NC_ENULLPAD : c_int;
    extern const NC4_LAST_ERROR : c_int;
    extern const DIM_WITHOUT_VARIABLE : c_string;
    extern const NC_HAVE_NEW_CHUNKING_API : c_int;
    extern const NC_EURL : c_int;
    extern const NC_ECONSTRAINT : c_int;
    extern const NC_TURN_OFF_LOGGING : c_int;
    extern const FILL_BYTE : c_char;
    extern const FILL_CHAR : c_char;
    extern const FILL_SHORT : c_short;
    extern const FILL_LONG : c_int;
    extern const FILL_FLOAT : real(32);
    extern const FILL_DOUBLE : real;
    extern const MAX_NC_DIMS : c_int;
    extern const MAX_NC_ATTRS : c_int;
    extern const MAX_NC_VARS : c_int;
    extern const MAX_NC_NAME : c_int;
    extern const MAX_VAR_DIMS : c_int;
    extern const NC_ENTOOL : c_int;
    extern const NC_EXDR : c_int;
    extern const NC_SYSERR : c_int;
    extern const NC_FATAL : c_int;
    extern const NC_VERBOSE : c_int;

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
