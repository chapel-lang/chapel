require '../lib/ArrowAll.chpl';
use ArrowAll as Arrow;

proc main() {
  var toParquet = [i in 0..#10] i;
  var table = new arrowTable(new arrowRecordBatch("col1", new arrowArray(toParquet)));
  Arrow.writeTableToParquetFile(table, "test.parquet");

  var pqReader = new parquetFileReader("test.parquet");
  var fromParquet = pqReader.readColumn(0);
  writeln(toParquet);
  writeln(fromParquet);

  var arr = toParquet+10;
  var strArr = [i in 0..#10] "asd";
  var parquetWriter = new parquetFileWriter("test2.parquet");
  parquetWriter.addColumn(toParquet, 0, "first-int-col");
  parquetWriter.addColumn(arr, 1, "second-int-col");
  parquetWriter.addColumn(strArr, 1, "str-col");
  parquetWriter.finish();

  parquetWriter = new parquetFileWriter("str-file.parquet");
  parquetWriter.addColumn(strArr, 0, "str-col");
  parquetWriter.finish();

  pqReader = new parquetFileReader("test2.parquet");
  fromParquet = pqReader.readColumn(0);
  writeln(fromParquet);
  writeln(pqReader.readColumnStr(1));
  writeln(pqReader.readColumn(2));

  pqReader.writeSchema();

  writeln(pqReader.readColumnByName("first-int-col"));
}
