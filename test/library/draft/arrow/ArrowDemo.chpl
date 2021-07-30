module ArrowDemo {
  require "Arrow.chpl";
  import Arrow;
  use ArrowDecl;
  use CPtr;

  proc main(){
    var arrowInt64Array = Arrow.array([1,2,3,4]);
    var arrowStringArray = Arrow.array(["foo", "bar", "baz", "some_invalid_value"],[3]);
    var arrowBooleanArray = Arrow.array([true, false, false, true], [1]);

    var error: GErrorPtr;
    writeln("Int Array:");
    printArray(arrowInt64Array);
    writeln("String Array:");
    printArray(arrowStringArray);
    writeln("Int Array:");
    printArray(arrowBooleanArray);
    
    var rcbatch: GArrowRecordBatch;
    rcbatch = Arrow.recordBatch("field0", arrowInt64Array, "field1", arrowStringArray, "field2", arrowBooleanArray);

    writeln("\nRecord batch from above arrays:");
    printRecordBatch(rcbatch);
    
    writeln("----Parquet Demo-----");

    // Taking the recordBatch above and putting it in a table
    var table1 = Arrow.table([rcbatch, rcbatch]);
    printTable(table1);

    // Putting that table in a parquet file
    Arrow.writeTableToParquetFile(table1, "example.parquet");

    // Reading that file back
    var table2 = Arrow.readParquetFileToTable("example.parquet");
    printTable(table2);
  }
}
