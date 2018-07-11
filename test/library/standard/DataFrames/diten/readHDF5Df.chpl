use DataFrames, HDF5_HL;

config const inputDir = "hdf5_files";

var colDom: domain(string);
var columns: [colDom] Series;
var dataFrame = new DataFrame(columns);

type inputTypes = (int, real);

for param i in 1..inputTypes.size {
  type inType = inputTypes(i);
  var col: Series;
  param typeName = inType:string;

  var arrays = readAllHDF5Files(Locales, inputDir, "/dset",
                                typeName, inType, rank=1);

  for (arr, j) in zip(arrays, 1..) {
    col = new TypedSeries(arr.A);
    var name = "s" + i + "_" + j;
    dataFrame.insert(name, col);
  }
}

writeln(dataFrame);
