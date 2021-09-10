require '../lib/ArrowAll.chpl';
use ArrowAll as Arrow;
use BlockDist;

proc main() {
  var d1 = [i in 0..#20] i;
  var d2 = [i in 0..#15] i;
  var d3 = [i in 0..#4] i;
  var d4 = [i in 0..#8] i;
  var d5 = [i in 0..#50] i;
  var f1 = new parquetFileWriter("f1.parquet");
  var f2 = new parquetFileWriter("f2.parquet");
  var f3 = new parquetFileWriter("f3.parquet");
  var f4 = new parquetFileWriter("f4.parquet");
  var f5 = new parquetFileWriter("f5.parquet");
  
  f1.addColumn(d1, 0, "col1");
  f1.finish();
  f2.addColumn(d2, 0, "col1");
  f2.finish();
  f3.addColumn(d3, 0, "col1");
  f3.finish();
  f4.addColumn(d4, 0, "col1");
  f4.finish();
  f5.addColumn(d5, 0, "col1");
  f5.finish();
  
  var filenames = ["f1.parquet", "f2.parquet", "f3.parquet", "f4.parquet", "f5.parquet"];
  var (sizes, ty) = getArrSizeAndType(filenames, 0);
  
  var A = newBlockArr(0..#(+ reduce sizes), int);

  readFiles(A, filenames, sizes);
  writeln(A);

  writeDistArray(A, "dist-file");

  var B = newBlockArr(0..#10, int);
  readFileColumnToDist(B, "test.parquet", 0);
  writeln(B);
  for val in B do
    writeln(val.locale);
}
