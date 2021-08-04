module ArrowDemo {
  require "Arrow.chpl";
  import Arrow;
  use ArrowDecl;
  use CPtr;

  proc main(){
    var arrowInt64Array = Arrow.array([1,2,3,4]);
    var arrowStringArray = Arrow.array(["foo", "bar", "baz", "some_invalid_value"],validIndices=[0,1,2]);
    var arrowBooleanArray = Arrow.array([true, false, false, true], invalidIndices=[1]);

    writeln("Int Array:");
    printArray(arrowInt64Array);
    writeln("String Array:");
    printArray(arrowStringArray);
    writeln("Boolean Array:");
    printArray(arrowBooleanArray);
    
    var rcbatch: GArrowRecordBatch = Arrow.recordBatch("field0", arrowInt64Array, "field1", arrowStringArray, "field2", arrowBooleanArray);
    var rcbatch2: GArrowRecordBatch = Arrow.recordBatch("field0", arrowInt64Array, "field1", arrowStringArray);
    writeln("\nRecord batch from above arrays:");
    printRecordBatch(rcbatch);
    
    writeln("\n---------Parquet Demo-------------\n");

    // Taking the recordBatch above and putting it in a table
    //var table1 = Arrow.table([rcbatch, rcbatch]); // Alternative
    var table1 = Arrow.table(rcbatch, rcbatch);
    printTable(table1);

    // Putting that table in a parquet file
    Arrow.writeTableToParquetFile(table1, "example.parquet");

    // Reading that file back
    var table2 = Arrow.readParquetFileToTable("example.parquet");
    printTable(table2);
  }
}
