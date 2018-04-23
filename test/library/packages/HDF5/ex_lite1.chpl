// This is based on:
// https://bitbucket.hdfgroup.org/projects/HDFFV/repos/hdf5/browse/examples/h5_crtdat.c

param RANK = 2:c_int;

proc main {
  use HDF5_HL;

  var file_id: hid_t;
  var dims: [0..#RANK] hsize_t = [2:hsize_t, 3:hsize_t];
  var data: [0..#6] c_int = [1:c_int, 2:c_int, 3:c_int,
                             4:c_int, 5:c_int, 6:c_int];

  /* create HDF5 file */
  file_id = H5Fcreate(c"ex_lite1.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  /* create and write an integer type dataset named "dset" */
  H5LTmake_dataset(file_id, c"dset", RANK,
                   dims[0], H5T_NATIVE_INT, c_ptrTo(data));

  H5Fclose(file_id);
}
