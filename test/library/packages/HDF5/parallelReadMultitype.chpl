use FileSystem, HDF5;

type inputTypes = (int, real, c_string);

config const inputDir = "HDF5files";

config const printFileInfo = false;
config const printTiming = false;

proc main {
  use HDF5, Time;

  var t = new Timer();

  for param i in 0..<inputTypes.size {
    type inType = inputTypes(i);
    param typeName = inType:string;
    t.start();
    var files = readAllHDF5Files(Locales, inputDir, "/dset",
                                 inType:string, inType, rank=2);
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
