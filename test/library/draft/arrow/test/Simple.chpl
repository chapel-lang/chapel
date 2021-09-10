require "../lib/ArrowAll.chpl";
require "-larrow-glib", "-lglib-2.0", "-lgobject-2.0";
use ArrowAll as Arrow;
use CPtr;

proc main() {
  var pf = new parquetFileReader("test.parquet");
  writeln(pf.readColumn(0));
}
