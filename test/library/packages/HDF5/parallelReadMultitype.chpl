//use BlockDist, FileSystem, HDF5_HL;
use FileSystem, HDF5_HL;

type inputTypes = (int, real, c_string);

config const inputDir = "HDF5files";

config const printFileInfo = false;
config const printTiming = false;

/*
record HDF5_file {
  const filename: string;
  const file_id: hid_t;

  proc init(name: string, access: c_uint = H5F_ACC_RDONLY) {
    filename = name;
    file_id = H5Fopen(name.c_str(), access, H5P_DEFAULT);
  }

  proc deinit() {
    H5Fclose(file_id);
  }
}
*/
/*
proc readAllHDF5Files(locs: [] locale, dirname: string, dsetName: string, type eltType, param rank) {

  var filenames: [1..0] string;
  for f in findfiles(dirname) {
    if f.startsWith(dirname + '/' + eltType:string) && f.endsWith(".h5") {
      filenames.push_back(f);
    }
  }
  const Space = {1..filenames.size};
  const BlockSpace = Space dmapped Block(Space, locs, 1);
  var files: [BlockSpace] ArrRec(eltType);
  forall (f, name) in zip(files, filenames) {
    var locName = name; // copy this string to be local
    var file_id = H5Fopen(locName.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    var dims: [0..#rank] hsize_t;
    H5LTget_dataset_info_WAR(file_id, dsetName.c_str(), c_ptrTo(dims), nil, nil);
    var data: [0..# (* reduce dims)] eltType;
    readHDF5Dataset(file_id, dsetName, data);

    var rngTup: rank*range;

    for i in 0..#rank {
      rngTup[i+1] = 1..dims[i]:int;
    }

    const D = {(...rngTup)};

    //for (i,j) in zip (data, 1..) {
      //writeln((i:string,j));
    //}
    f = new ArrRec(data.eltType, D, reshape(data, D));
    H5Fclose(file_id);
  }
  return files;
}
*/

/*
proc readHDF5Dataset(file_id, dsetName: string, data) {
  type eltType = data.eltType;

  proc getHDF5Type(type eltType) {
    var hdf5Type: hid_t;
    select eltType {
      when int(8) {
        hdf5Type = H5T_STD_I8LE;
      }
      when int(16) {
        hdf5Type = H5T_STD_I16LE;
      }
      when int(32) {
        hdf5Type = H5T_STD_I32LE;
      }
      when int(64) {
        hdf5Type = H5T_STD_I64LE;
      }
      when uint(8) {
        hdf5Type = H5T_STD_U8LE;
      }
      when uint(16) {
        hdf5Type = H5T_STD_U16LE;
      }
      when uint(32) {
        hdf5Type = H5T_STD_U32LE;
      }
      when uint(64) {
        hdf5Type = H5T_STD_U64LE;
      }
      when real(32) {
        hdf5Type = H5T_IEEE_F32LE;
      }
      when real(64) {
        hdf5Type = H5T_IEEE_F64LE;
      }
      when c_string {
        hdf5Type = H5Tcopy(H5T_C_S1);
        H5Tset_size(hdf5Type, H5T_VARIABLE);
      }
      otherwise {
        halt("Unhandled type in getHDF5Type: ", eltType:string);
      }
    }
    return hdf5Type;
  }

  const hdf5Type = getHDF5Type(eltType);
  H5LTread_dataset(file_id, dsetName.c_str(), hdf5Type, c_ptrTo(data));
}
*/
/*
record ArrRec {
  type eltType;
  var D: domain(2);
  var A: [D] eltType;
}
*/


proc main {
  use HDF5_HL, Time;

  var t = new Timer();

  for param i in 1..inputTypes.size {
    type inType = inputTypes(i);
    param typeName = inType:string;
    t.start();
    var files = readAllHDF5Files(Locales, inputDir, "/dset", inType, rank=2);
    t.stop();

    for (f, i) in zip (files, 1..) {
      if printFileInfo {
        writeln("file ", i, " (", typeName, "): ");
        writeln("  D: ", f.D);
        writeln("  A: ", f.A.size, " elements");
        writeln(f.A[10, 10]:string);
        writeln();
      }
    }
    if printTiming {
      writeln(t.elapsed(), " seconds for type ", typeName);
    } else {
      writeln("read type ", typeName);
    }
    t.clear();
  }

}
