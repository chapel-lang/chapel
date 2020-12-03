// This is based on:
// https://support.hdfgroup.org/ftp/HDF5/current/src/unpacked/hl/examples/ex_lite2.c

proc main {
  use HDF5.C_HDF5;
  use Hdf5PathHelp;

  var file_id: hid_t,
      data: [0..#6] c_int,
      dims: [0..#2] hsize_t,
      i, j, nrow, n_values: size_t;

  const filename = "ex_lite2_input.h5";
  var pathPrefix = readPrefixEnv();

  if pathPrefix != "" {
    use FileSystem;
    copyFile(filename, pathPrefix + filename);
  }

  /* open file from ex_lite1.chpl */
  file_id = H5Fopen((pathPrefix + filename).c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

  /* read the dataset */
  H5LTread_dataset_int(file_id, c"/dset", data[0]);

  /* get the dimensions of the dataset */
  H5LTget_dataset_info_WAR(file_id, c"/dset", c_ptrTo(dims), nil, nil);

  /* print it by rows */
  n_values = (dims[0]*dims[1]): size_t;
  nrow = dims[1]: size_t;
  for i in 0..#n_values/nrow {
    for j in 0..#nrow {
      write("  ", data[i*nrow:int + j]);
    }
    writeln();
  }

  H5Fclose(file_id);
}
