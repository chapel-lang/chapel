// This is based on:
// https://support.hdfgroup.org/ftp/HDF5/current/src/unpacked/hl/examples/ex_lite3.c

param ATTR_SIZE = 5;

proc main {
  use HDF5.C_HDF5;
  use Hdf5PathHelp;

  var file_id, dset_id, space_id: hid_t;
  var dims: [0..0] hsize_t = ATTR_SIZE;
  var data: [0..#ATTR_SIZE] c_int = [1:c_int, 2:c_int, 3:c_int, 4:c_int, 5:c_int];
  const filename = "ex_lite3.h5";
  const pathPrefix = readPrefixEnv();

  /* Create a file */
  file_id = H5Fcreate((pathPrefix+filename).c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  /* Create a data space */
  space_id = H5Screate_simple(1, c_ptrTo(dims), nil);

  /* create a dataset named "dset" */
  dset_id = H5Dcreate2(file_id, "dset", H5T_NATIVE_INT,  space_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  H5Dclose(dset_id);
  H5Sclose(space_id);

  /* create and write the attribute "attr1" on the dataset "dset" */
  H5LTset_attribute_int(file_id, "dset", "attr1", data[0], ATTR_SIZE);

  /* get the attribute "attr1" from the dataset "dset" */
  H5LTget_attribute_int(file_id, "dset", "attr1", data[0]);

  for i in 0..#ATTR_SIZE {
    write("  ", data[i]);
  }
  writeln();

  H5Fclose(file_id);
}
