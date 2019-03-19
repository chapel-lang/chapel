use BlockDist, HDF5;

/* Read one HDF5 file/dataset per locale into a single block distributed
   array.  The files can have any number of elements in them and are named
   using the configuration constants 'filenameBase' and 'filenameExt'.
   The dataset within the file is named with the configuration constant
   'dsetName'.

   For example, to use the settings with their default values below, files
   named:

   file0.h5, file1.h5, file2.h5, ..., file<N>.h5

   where '<N>' is the number of locales, should exist and contain 1D
   datasets named "dset" with 'c_int' elements.

   'chunkSize' is the maximum number of elements to read into local buffers
   before copying them into the distributed array.
 */

config const filenameBase = "file";
config const filenameExt = ".h5";
config const dsetName = "dset";
config const chunkSize = 1024*1024;

config type eltType = c_int;

config const printCommCounts = false;


/* Read the files in `filenames` into the distributed array `distribArr`.
   The sizes array lists the number of elements in each corresponding file.
   The `filenames` and `sizes` arrays must be `numLocales` elements long.
 */
proc distributedRead(distribArr, filenames, sizes) {
  // copy the sizes array into an array with the first element set
  // to '0' to turn the scan into an exclusive scan.
  var fileSizes: [1..sizes.numElements+1] int;
  fileSizes[2..] = sizes;
  const totSizes = + scan fileSizes;

  // Start tasks on each locale to read one file each
  coforall (loc, i) in zip(Locales, 0..) do on loc {
    const filename = filenames[i+1];
    const chunkShape = {0..#chunkSize};

    // Read the HDF5 data into a local array in chunks, then assign
    // those chunks to the distributed array.
    for (chunk, j) in zip(hdf5ReadChunks(filename,
                                         dsetName,
                                         chunkShape=chunkShape,
                                         eltType = c_int),
                          0..) {
      const size = chunk.numElements,
            start = distribArr.domain.low + totSizes[i+1] + j*chunkSize,
            end = start + size - 1;
      ref blockSlice = distribArr[start..end];
      blockSlice = chunk;
    }
  }
}

/* Return the number of elements in the dataset. This assumes that the
   dataset is 1-D.
 */
proc numElementsInDset(filename, dsetName) {
  var file_id = C_HDF5.H5Fopen(filename.c_str(),
                               C_HDF5.H5F_ACC_RDONLY,
                               C_HDF5.H5P_DEFAULT);
  var dims: C_HDF5.hsize_t;
  C_HDF5.HDF5_WAR.H5LTget_dataset_info_WAR(file_id, dsetName.c_str(),
                                           c_ptrTo(dims), nil, nil);
  C_HDF5.H5Fclose(file_id);
  return dims: int;
}

proc main() {
  var filenames: [1..numLocales] string;
  var fileSizes: [1..numLocales] int;

  for i in 1..numLocales {
    // Create the file names and find the number of elements in the data sets
    filenames[i] = filenameBase + (i-1) + filenameExt;
    fileSizes[i] = numElementsInDset(filenames[i], dsetName);
  }

  const numElements = + reduce fileSizes;

  // Declare the distributed array that holds the same number of elements
  // as all of the files involved combined.
  const bbox = {0..#numElements};
  const blockDom = newBlockDom(bbox);
  var blockArr: [blockDom] eltType;

  // Read the files into the distributed array
  writeln("Reading the files...");

  if printCommCounts then
    startCommDiagnostics();

  distributedRead(blockArr, filenames, fileSizes);

  if printCommCounts {
    stopCommDiagnostics();
    var gets = + reduce getCommDiagnostics().get;
    var puts = + reduce getCommDiagnostics().put;
    writeln("gets=", gets, " puts=", puts);
  }


  // Validate that the data matches what is expected
  writeln("validating data...");
  var error: atomic bool = false;
  forall i in blockDom {
    // with my example files, expect blockArr[i] == i
    if blockArr[i] != i {
      writeln("error: blockArr[", i, "] = ", blockArr[i]);
      error.write(true);
    }
  }
  if !error.read() {
    writeln("Success!");
  }
}
