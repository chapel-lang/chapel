module ArrowDemo {
  require "Arrow.chpl";
  require "-larrow-glib", "-lglib-2.0", "-lgobject-2.0";
  use Arrow;
  use CPtr;

  proc main(){
    var arrowInt64Array: ArrowArray = new ArrowArray([1,2,3,4]);
    var arrowStringArray =  new ArrowArray(["foo", "bar", "baz", "some_invalid_value"],validIndices=[0,1,2]);
    var arrowBooleanArray = new ArrowArray([true, false, false, true], invalidIndices=[1]);

    writeln("Int Array:");
    printArray(arrowInt64Array);
    writeln("String Array:");
    printArray(arrowStringArray);
    writeln("Boolean Array:");
    printArray(arrowBooleanArray);
    
    var rcbatch: ArrowRecordBatch = new ArrowRecordBatch("field0", arrowInt64Array, "field1", arrowStringArray, "field2", arrowBooleanArray);
    var rcbatch2: ArrowRecordBatch = new ArrowRecordBatch("field0", arrowInt64Array, "field1", arrowStringArray);
    writeln("\nRecord batch from above arrays:");
    printRecordBatch(rcbatch);
    
    writeln("\n---------Parquet Demo-------------\n");

    // Taking the recordBatch above and putting it in a table
    //var table1 = Arrow.table([rcbatch, rcbatch]); // Alternative
    var table1 = new ArrowTable(rcbatch, rcbatch);
    printTable(table1);

    // Putting that table in a parquet file
    Arrow.writeTableToParquetFile(table1, "example.parquet");

    // Reading that file back
    var table2 = Arrow.readParquetFileToTable("example.parquet");
    printTable(table2);
  }
}
