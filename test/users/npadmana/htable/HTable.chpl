module HTable {

  public use HDF5;
  public use HDF5.C_HDF5;
  use IO;
  use Reflection;

  record H5MetaTable {
    type R; // The record type
    param nFields : int;

    const Rsize : size_t;
    var names : c_array(c_string, nFields);
    var offsets : c_array(hsize_t, nFields);
    var types : c_array(hid_t, nFields);
    var sizes : c_array(size_t, nFields);
    var ownedtypes : [1..nFields] bool;
    var nameList : string = "";

    proc init(type R) where isRecord(R) {
      this.R = R;
      nFields = numFields(R);
      Rsize = c_sizeof(R);
      this.complete();

      var r : R;
      for param ii in 1..nFields {
        param fname = getFieldName(R, ii);
        if (ii==1) {
          nameList = fname;
        } else {
          nameList += ","+fname;
        }
        names[ii-1] = fname.c_str();
        offsets[ii-1] = c_offsetof(R, fname) : hsize_t;
        ref ifield = getField(r, ii);
        type fieldtype = ifield.type;
        // Handle some special cases
        if (fieldtype==string) then halt("String types not supported in HDF5 tables currently");
        // Arrays
        if (is_c_array(ifield)) {
          var dim : hsize_t = ifield.size : hsize_t; 
          types[ii-1] = H5Tarray_create2(getHDF5Type(ifield.eltType), 1, c_ptrTo(dim));
          ownedtypes[ii] = true;
          sizes[ii-1] = (c_sizeof(ifield.eltType)*dim):size_t;
          continue;
        }
        // All other cases
        types[ii-1] = getHDF5Type(fieldtype);
        sizes[ii-1] = c_sizeof(fieldtype) : size_t;
        ownedtypes[ii] = false;
      }

    }

    proc writeThis(f) {
      for ii in 1..nFields {
        f <~> names[ii]:string <~> new ioLiteral(" ") <~> offsets[ii] <~> new ioNewline();
      }
    }

    proc deinit() {
      for param ii in 1..nFields {
        if ownedtypes[ii] then H5Tclose(types[ii-1]);
      }
    }
  }

  proc createH5Table(arr : [] ?R, loc : hid_t, name : string, compressTable=false, chunkSize=10)
    where isRecord(R)
  {
    // This is defined in C_HDF5, but the definition there was causing isseus with
    // field_names, field_offset, field_types, so I define an overload there. 
    // It's more than likely I was doing something silly 
    extern proc H5TBmake_table(table_title : c_string, file_id : hid_t, dset_name: c_string,
                               numFields : hsize_t, numRecords : hsize_t, type_size : hsize_t,
                               field_names : c_array(c_string), field_offset : c_array(hsize_t),
                               field_types : c_array(hid_t),
                               chunk_size : hsize_t, fill_data : c_void_ptr, compress : c_int,
                               data : c_void_ptr);
    var meta = new H5MetaTable(R);
    var cname = name.c_str();
    var nfields = meta.nFields : hsize_t;
    var nrecords = arr.numElements: hsize_t;
    var type_size = meta.Rsize : hsize_t;
    var chunk_size = chunkSize:hsize_t;
    var fill_data = c_nil;
    var compress = (if compressTable then 1 else 0):c_int;
    var data = if (nrecords==0) then c_nil else c_ptrTo(arr);

    H5TBmake_table(cname, loc, cname, nfields, nrecords, type_size,
                   meta.names, meta.offsets, meta.types, chunk_size, fill_data,
                   compress, data);
  }

  proc createH5Table(type R, loc : hid_t, name : string, compressTable=false, chunkSize=10)
    where isRecord(R)
  {
    var tmp : [1..0] R;
    createH5Table(tmp, loc, name, compressTable, chunkSize);
  }


  proc appendH5Table(arr : [] ?R, loc : hid_t, name : string)
    where isRecord(R)
  {
    // Defining here for safety again.
    extern proc H5TBappend_records(loc_id : hid_t, dset_name: c_string,
                                   numRecords : hsize_t, type_size : hsize_t,
                                   field_offset : c_array(hsize_t),
                                   field_sizes : c_array(size_t),
                                   data : c_void_ptr);
    var meta = new H5MetaTable(R);
    var cname = name.c_str();
    var type_size = meta.Rsize : hsize_t;
    var nrecords = arr.numElements: hsize_t;
    var data = if (nrecords==0) then c_nil else c_ptrTo(arr);

    H5TBappend_records(loc, cname, nrecords, type_size,
                       meta.offsets, meta.sizes, data);
  }


  proc readH5Table(type R, loc : hid_t, name : string)
    where isRecord(R)
  {
    extern proc H5TBread_fields_name(loc_id : hid_t, dset_name : c_string,
                                     field_names : c_string, start : hsize_t,
                                     nrecords : hsize_t, type_size : size_t,
                                     field_offset : c_array(hsize_t),
                                     field_sizes : c_array(size_t),
                                     data : c_void_ptr);
    extern proc H5TBget_table_info(loc_id : hid_t, table_name : c_string,
                                   nfields : c_ptr(hsize_t), nrecords : c_ptr(hsize_t));
    var meta = new H5MetaTable(R);
    var cname = name.c_str();
    var type_size = meta.Rsize : hsize_t;
    var field_names = meta.nameList.c_str();
    var nrecords, nfields : hsize_t;
    H5TBget_table_info(loc, cname, c_ptrTo(nfields), c_ptrTo(nrecords));

    // Define records
    var retval : [0.. #(nrecords:int)] R;
    var data = c_ptrTo(retval);

    // Read in table
    H5TBread_fields_name(loc, cname,
                         field_names, 0, nrecords, type_size,
                         meta.offsets, meta.sizes, data);

    return retval;
  }

  // From David Iten -- thanks!
  proc is_c_array(x: c_array(?)) param return true;
  proc is_c_array(x) param return false;


}