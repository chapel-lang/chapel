/* Provide access to the HDF5 Table interface using Chapel records.

   The module currently allows the user to create, append and read from tables.
   The Chapel record is translated to the corresponding HDF5 table names and types.

   When reading, one can read a subset of fields by just defining a record with the
   desired fields.

   Currently, only types mappable using the `getHDF5Type` function in the `HDF5` module,
   and `c_array` arrays of these types are allowed. Furthermore, `string` types are not
   allowed (since they are not fixed size).

   .. note::

      The primary routines here use the high-level table interface. It is likely that
      these cannot be used to read a generic HDF5 structured type back.

      However, the `H5MetaTable` record stores the information necessary to
      create a new HDF5 type. Later versions of the code might add in a method that
      returns the corresponding HDF5 structured type.
*/
module HTable {

  public use HDF5;
  public use HDF5.C_HDF5;
  use IO;
  use Reflection;

  /* Create an HDF5 table out of an array of records `arr`. 

     :arg arr:  Array to create the HDF5 table from. 
     :type arr: [] R
     :arg loc: Location to create the table (as an HDF5 identifier)
     :type loc: hid_t
     :arg name: Name of the table
     :type name: string
     :arg compressTable: compress the table (defaults to `false`)
     :type compressTable: bool
     :arg chunkSize: chunk size for the HDF5 table (aids with eg. appending etc). (defaults to 10)
     :type chunkSize: int

     .. note::

        The chunksize default is almost certainly non-optimal. Please refer to the HDF5 documentation
        to determine the appropriate value for your application.


  */
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
    var nrecords = arr.size: hsize_t;
    var type_size = meta.Rsize : hsize_t;
    var chunk_size = chunkSize:hsize_t;
    var fill_data = c_nil;
    var compress = (if compressTable then 1 else 0):c_int;
    var data = if (nrecords==0) then c_nil else c_ptrTo(arr);

    H5TBmake_table(cname, loc, cname, nfields, nrecords, type_size,
                   meta.names, meta.offsets, meta.types, chunk_size, fill_data,
                   compress, data);
  }

  /* Create an HDF5 table based on record type `R`

     This is an overload of the `createH5Table` routine allowing the user to create
     a table based on a type instead of an existing array. See above for a description
     of the remaining parameters.
  */
  proc createH5Table(type R, loc : hid_t, name : string, compressTable=false, chunkSize=10)
    where isRecord(R)
  {
    var tmp : [1..0] R;
    createH5Table(tmp, loc, name, compressTable, chunkSize);
  }

  /* Append records to an existing HDF5 table.

     :arg arr:  Array of records to append to the table.
     :type arr: [] R
     :arg loc: Location of the table (as an HDF5 identifier)
     :type loc: hid_t
     :arg name: Name of the table
     :type name: string
  */
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
    var nrecords = arr.size: hsize_t;
    var data = if (nrecords==0) then c_nil else c_ptrTo(arr);

    H5TBappend_records(loc, cname, nrecords, type_size,
                       meta.offsets, meta.sizes, data);
  }


  /* Read an HDF5 table.

     :arg R: Type variable (defining the type to read in)
     :arg loc: Location of the table (as an HDF5 identifier)
     :type loc: hid_t
     :arg name: Name of the table
     :type name: string

     .. note::

        The type `R` need not include all the fields in the HDF5
        table, allowing the user to selectively read only the fields
        of interest.
  */
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


  /* This is an internal record used to translate between Chapel
     and HDF5 types. Users of the HDF5 table interface should not need
     to use this, but we document it here since it might be useful
     when working with generic types.

     The `names`, `offsets`, `types` and `sizes` elements are the most
     relevant and may be passed on directly to the HDF5 routines.
  */
  record H5MetaTable {
    /* The chplType of the record. */
    type R;

    /* The number of fields */
    param nFields : int;

    /* The size of `R` as reported by C `sizeof` */
    const Rsize : size_t;

    /* A c_array of names of fields */
    var names : c_array(c_string, nFields);
    /* Offsets of fields */
    var offsets : c_array(hsize_t, nFields);
    /* HDF5 types of fields */
    var types : c_array(hid_t, nFields);
    /* Sizes of fields */
    var sizes : c_array(size_t, nFields);

    /* Are the types created by this routine (currently,
     `c_array` types fall into this class). If so, this
     flags that these need to be deallocated at destruction.
    */
    var ownedtypes : [0..<nFields] bool;

    /* A string with all the names of the fields, needed
       for the table interface. */
    var nameList : string = "";

    /* Initializer for type `R` */
    proc init(type R) where isRecord(R) {
      this.R = R;
      nFields = numFields(R);
      Rsize = c_sizeof(R);
      this.complete();

      var r : R;
      for param ii in 0..<nFields {
        param fname = getFieldName(R, ii);
        if (ii==0) {
          nameList = fname;
        } else {
          nameList += ","+fname;
        }
        names[ii] = fname.c_str();
        offsets[ii] = c_offsetof(R, fname) : hsize_t;
        ref ifield = getField(r, ii);
        type fieldtype = ifield.type;
        // Handle some special cases
        if (fieldtype==string) then halt("String types not supported in HDF5 tables currently");
        // Arrays
        if (is_c_array(ifield)) {
          var dim : hsize_t = ifield.size : hsize_t; 
          types[ii] = H5Tarray_create2(getHDF5Type(ifield.eltType), 1, c_ptrTo(dim));
          ownedtypes[ii] = true;
          sizes[ii] = (c_sizeof(ifield.eltType)*dim):size_t;
          continue;
        }
        // All other cases
        types[ii] = getHDF5Type(fieldtype);
        sizes[ii] = c_sizeof(fieldtype) : size_t;
        ownedtypes[ii] = false;
      }

    }

    /* Debugging routine, prints a human-readable version of how the
       routine has parsed the field.
    */
    proc writeThis(f) {
      for ii in 1..nFields {
        f <~> names[ii]:string <~> new ioLiteral(" ") <~> offsets[ii] <~> new ioNewline();
      }
    }

    /* De-initializer. Cleans up any types the routine created. */
    proc deinit() {
      for param ii in 0..<nFields {
        if ownedtypes[ii] then H5Tclose(types[ii]);
      }
    }
  }



  
  // From David Iten -- thanks!
  pragma "no doc"
  proc is_c_array(x: c_array(?)) param return true;
  pragma "no doc"
  proc is_c_array(x) param return false;


}
