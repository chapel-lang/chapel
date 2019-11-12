use MPI, HDF5, HDF5.C_HDF5;

module MPI_Constants {
  extern type MPI_Info;
  extern const MPI_INFO_NULL: MPI_Info;
}

module HDF5_Constants {
  extern const H5P_FILE_ACCESS: hid_t;
  extern const H5F_ACC_TRUNC: c_uint;
  extern const H5P_DEFAULT: hid_t;
  extern const H5S_SELECT_SET: H5S_seloper_t;
  extern const H5P_DATASET_XFER: hid_t;
  extern const H5FD_MPIO_COLLECTIVE: H5FD_mpio_xfer_t;
  const FAIL = -1;
  extern proc H5Pset_fapl_mpio(fapl_id: hid_t, comm: MPI_Comm, info): herr_t;
  extern proc H5Pset_dxpl_mpio(xferPlist: hid_t, flag: H5FD_mpio_xfer_t): herr_t;
  //extern proc H5Dcreate(loc_id: hid_t, name: c_string, type_id: hid_t, space_id: hid_t, dcpl_id: hid_t): hid_t;
}

proc writeArrayToFile(filename: string, dsetName: string, A: []) {
  use MPI_Constants, HDF5_Constants;

  coforall loc in A.domain.targetLocales() do on loc {
    const locFilename = filename;
    const jobSize = commSize(CHPL_COMM_WORLD),
          jobRank = commRank(CHPL_COMM_WORLD);

    const hdf5Type = getHDF5Type(A.eltType);

    var info = MPI_INFO_NULL;
    const accessTemplate = H5Pcreate(H5P_FILE_ACCESS);

    H5Pset_fapl_mpio(accessTemplate, CHPL_COMM_WORLD, info);
    var fid = H5Fcreate(locFilename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, accessTemplate);
    const locDom = A.domain.localSubdomain();

    H5Pclose(accessTemplate);

    var dims: c_array(uint, A.rank);
    for param i in 0..A.rank-1 {
      dims[i] = A.domain.dim(i+1).size: uint;
    }

    var sid = H5Screate_simple(A.rank, dims, nil);
    //assert(sid != FAIL);

    var dataset = H5Dcreate1(fid, dsetName.c_str(), hdf5Type, sid, H5P_DEFAULT);

    // create a file dataspace
    var fileDataspace = H5Dget_space(dataset);
    assert(fileDataspace != FAIL);

    var stride: c_array(uint, A.rank);
    var count: c_array(uint, A.rank);
    var start: c_array(uint, A.rank);

    for i in 0..#A.rank {
      stride[i] = 1;
      count[i] = locDom.dim(i+1).size: uint;
      start[i] = (locDom.dim(i+1).low - A.domain.dim(i+1).low): uint;
    }

    var ret = H5Sselect_hyperslab(fileDataspace, H5S_SELECT_SET, start, stride, count, nil);
    assert(ret != FAIL);

    // create a memory dataspace
    var memDataspace = H5Screate_simple(A.rank, count, nil);
    assert(memDataspace != FAIL);

    // set up the transfer properties list
    var xferPlist = H5Pcreate(H5P_DATASET_XFER);
    assert(xferPlist != FAIL);
    ret = H5Pset_dxpl_mpio(xferPlist, H5FD_MPIO_COLLECTIVE);
    assert(ret != FAIL);

    // write data
    ret = H5Dwrite(dataset, hdf5Type, memDataspace, fileDataspace, xferPlist, c_ptrTo(A._value.myLocArr.myElems));
    assert(ret != FAIL);

    // release temporary handles
    H5Sclose(fileDataspace);
    H5Sclose(memDataspace);
    H5Pclose(xferPlist);
  }
}


proc main {
  use BlockDist;
  var A = newBlockArr({1..100, 1..100}, c_int);

  for (i,j) in A.domain {
    A[i,j] = (i*1000+j): c_int;
  }

  writeArrayToFile("mydata.h5", "dset", A);
}
