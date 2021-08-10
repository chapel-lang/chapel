module ArrowPrinting {
  require "ArrayDecl.chpl", "ArrowRecordDecl.chpl", "ParquetDecl.chpl", "Arrow.chpl";
  private use ArrayDecl, ArrowRecordDecl, ParquetDecl, Arrow;
  use SysCTypes, CPtr;

  // Functions for printing
  proc printArray(arr: ArrowArray) {
    printArray(arr.val);
  }
  proc printArray(array: GArrowArray) {
    var error: GErrorPtr;
    var str: c_string = garrow_array_to_string(array, c_ptrTo(error));
    if(str == ""){
      g_print("Failed to print: %s\n", error.deref().message);
      g_error_free(error);
      return;
    }
    g_print("%s\n",str);
  }

  proc printRecordBatch(recordBatch: ArrowRecordBatch){
    printRecordBatch(recordBatch.rcbatch);
  }
  proc printRecordBatch(recordBatch: GArrowRecordBatch) {
    var error: GErrorPtr;
    var str = garrow_record_batch_to_string(recordBatch, c_ptrTo(error));
    if(str == ""){
      g_print("Failed to print: %s\n", error.deref().message);
      g_error_free(error);
      return;
    }
    g_print("%s\n",str);
  }

  proc printTable(table: ArrowTable) {
    printTable(table.tbl);
  }
  proc printTable(table: GArrowTable) {
    if(isNull(table)) then return;
    var error: GErrorPtr;
    var str = garrow_table_to_string(table, c_ptrTo(error));
    if(str == ""){
      g_print("Failed to print: %s\n", error.deref().message);
      g_error_free(error);
      return;
    }
    g_print("%s\n",str);
  }

}
