  module Arrow {
  require "ArrowDecl.chpl";
  public use ArrowDecl;
  public use SysCTypes, CPtr;
  
  // -------------------------- Type Declarations and Functions -------------

  record ArrowArray{
    var val: GArrowArray;

    proc init(arr: [] ?arrayType, validIndices: [] int = [-1], 
                invalidIndices: [] int = [-1]){
      this.val = array(arr, validIndices, invalidIndices);
    }

    proc init=()
  }

  proc arrowInt32(): GArrowInt32DataType {
    return garrow_int32_data_type_new();
  }

  proc arrowInt64(): GArrowInt32DataType {
    return garrow_int64_data_type_new();
  }
  proc arrowString(): GArrowStringDataType {
    return garrow_string_data_type_new();
  }
  proc arrowBool(): GArrowBooleanDataType {
    return garrow_boolean_data_type_new();
  }
  //--------------------------- Array building functions --------------------
  
  proc array(arr: [] ?arrayType, validIndices: [] int = [-1], 
              invalidIndices: [] int = [-1]): GArrowArray {
    // Build full validity array here since each function needs it anyways
    var validityArr: [0..#arr.size] gboolean;
    if(validIndices[0] != -1 && invalidIndices[0] != -1){
      // Both valid indices and invalid indices was passed which is not good
      // Maybe raise a compilerError here instead of the normal error handling
      writeln("Only one of two possible validity arguments may be passed.\
      [both validIndices and invalidIndices were passed]");
      var fakeReturn: GArrowArray;
      return fakeReturn;
    } else if(validIndices[0] != -1){
      validityArr = [i in 0..#arr.size] 0: gboolean;
      forall i in 0..#validIndices.size {
        validityArr[validIndices[i]] = 1: gboolean;
      }
    } else if(invalidIndices[0] != -1){
      validityArr = [i in 0..#arr.size] 1: gboolean;
      forall i in 0..#invalidIndices.size {
        validityArr[invalidIndices[i]] = 0: gboolean;
      }
    } else {
      validityArr = [i in 0..#arr.size] 1: gboolean;
    }
    select arrayType {
      when int do return int64Array(arr, validityArr);
      when string do return stringArray(arr, validityArr);
      when bool do return boolArray(arr, validityArr);
      otherwise {
        writeln("Unsupported type, \nreturning nil"); 
        var fakeReturn: GArrowArray;
        return fakeReturn;
      }
    }
  }

  proc int64Array(arr: [] int, validity: [] gboolean) : GArrowInt64Array {
      
    var retval: GArrowInt64Array;
    var builder: GArrowInt64ArrayBuilder;
    var success: gboolean = 1;
    var error: GErrorPtr;

    builder = garrow_int64_array_builder_new();

    if (success) {
      var intArrLen: gint64 = arr.size;
      var intValArr = [val in arr] val;
      var intValidityArrLen = intArrLen;
      success = garrow_int64_array_builder_append_values(
          builder, intValArr, intArrLen, validity, intValidityArrLen, c_ptrTo(error));
    }
    if (!success) {
      printGError("failed to append:", error);
      g_object_unref(builder);
      return retval;
    }
    retval = garrow_array_builder_finish(GARROW_ARRAY_BUILDER(builder), c_ptrTo(error));
    if (isNull(retval)) {
      printGError("failed to finish:", error);
      g_object_unref(builder);
      return retval;
    }
    g_object_unref(builder);

    return retval;
  }
  proc int32Array(arr: [] int(32), validity: [] gboolean) : GArrowInt32Array {
      
    var retval: GArrowInt32Array;
    
    var builder: GArrowInt32ArrayBuilder;
    var success: gboolean = 1;
    var error: GErrorPtr;

    builder = garrow_int32_array_builder_new();

    if (success) {
      var intArrLen: gint64 = arr.size;
      var intValArr = [val in arr] val: gint32;
      var intValidityArrLen: gint64 = intArrLen;
      success = garrow_int32_array_builder_append_values(
          builder, intValArr, intArrLen, validity, intValidityArrLen, c_ptrTo(error));
    }
    if (!success) {
      printGError("failed to append:", error);
      g_object_unref(builder);
      return retval;
    }
    retval = garrow_array_builder_finish(GARROW_ARRAY_BUILDER(builder), c_ptrTo(error));
    if (isNull(retval)) {
      printGError("failed to finish:", error);
      g_object_unref(builder);
      return retval;
    }
    g_object_unref(builder); 

    return retval;
  }
  proc stringArray(arr: [] string, validity: [] gboolean) : GArrowStringArray {
      
    var retval: GArrowStringArray;
    
    var builder: GArrowStringArrayBuilder;
    var success: gboolean = 1;
    var error: GErrorPtr;

    builder = garrow_string_array_builder_new();

    if (success) {
      var strArrLen: gint64 = arr.size: gint64;
      var strValArr = [val in arr] val.c_str();
      var strValidityArrLen: gint64 = strArrLen;
      success = garrow_string_array_builder_append_strings(
          builder, strValArr, strArrLen, validity, strValidityArrLen, c_ptrTo(error));
    }
    if (!success) {
      printGError("failed to append:", error);
      g_object_unref(builder);
      return retval;
    }
    retval = garrow_array_builder_finish(GARROW_ARRAY_BUILDER(builder), c_ptrTo(error));
    if (isNull(retval)) {
      printGError("failed to finish:", error);
      g_object_unref(builder);
      return retval;
    }
    g_object_unref(builder); 

    return retval;
  }

  proc boolArray(arr: [] bool, validity: [] gboolean) : GArrowBooleanArray {
    
    var retval: GArrowBooleanArray;
    
    var builder: GArrowBooleanArrayBuilder;
    var success: gboolean = 1;
    var error: GErrorPtr;

    builder = garrow_boolean_array_builder_new();

    if (success) {

      var boolArrLen: gint64 = arr.size: gint64;
      var boolValArr = [val in arr] val: gboolean;
      var boolValidityArrLen: gint64 = boolArrLen;
      success = garrow_boolean_array_builder_append_values(
          builder, boolValArr, boolArrLen, validity, boolValidityArrLen, c_ptrTo(error));
    }
    if (!success) {
      printGError("failed to append:", error);
      g_object_unref(builder);
      return retval;
    }
    retval = garrow_array_builder_finish(GARROW_ARRAY_BUILDER(builder), c_ptrTo(error));
    if (isNull(retval)) {
      printGError("failed to finish:", error);
      g_object_unref(builder);
      return retval;
  }
    g_object_unref(builder); 

    return retval;
  }

  // ---------------------- Record Batches and schemas ----------------------

  record ArrowRecordBatch {
    var rcbatch: GArrowRecordBatch;

    proc init(args ...?n){
      this.rcbatch = recordBatch( (...args) ); // Unpacking the tuple using ...
    }

  }
  proc recordBatch (args ...?n): GArrowRecordBatch {
    
    // Verifying the Integrity of the arguments
    if(n%2!=0) then
      compilerError("Mismatched arguments");
    for param i in 0..#n {
      if i%2 == 0 {
        if args[i].type != string then
          compilerError("Wrong even argument type");
      } else {
        if args[i].type != ArrowArray then
          compilerError("Wrong odd argument type");
      }
    }

    var fields: GList = getNULL();
    for param i in 1..n by 2{
      // Building the (column)
      var col: GArrowField = garrow_field_new(
                              args[i-1].c_str(), 
                              garrow_array_get_value_data_type(args[i].val: GArrowArray));

      // Adding the column to the list
      fields = g_list_append(fields, col);

      // Moving on the the next pair of arguments
    }

    // Gotta build this schema now
    var schema: GArrowSchema;
    //schema = garrow_schema_new(fieldsSimple);
    schema = garrow_schema_new(fields);

    // We might want to check the equality of the arrays length but the error will give it to us
    // anyway if they are not equal
    var n_rows: guint32 = garrow_array_get_length(args[1].val): guint32;

    var arrays: GList = getNULL();
    for param j in 1..n by 2 {
      // Adding the array to the list
      arrays = g_list_append(arrays, args[j].val);
    }

    var error: GErrorPtr;
    var record_batch: GArrowRecordBatch = garrow_record_batch_new(schema, n_rows, arrays, c_ptrTo(error));
    if(isNull(record_batch)){
      g_print("%s\n", error.deref().message);
    }
    // And after a lot of lines of code we have created the record batch.
    // The last part can also be done using a record batch builder class.
    //print_record_batch(record_batch);
    return record_batch;
  }

  record ArrowTable {
    var tbl: GArrowTable;
  
    proc init(args: ArrowRecordBatch ...?n){
      this.tbl = table( (...args) ); // Unpacking the tuple using ...
    }

    proc init(recordBatches: [] ArrowRecordBatch){
      this.tbl = table(recordBatches);
    }

    proc init(table: GArrowTable){
      this.tbl = table;
    }
    
  }
  proc table(recordBatches: [] ArrowRecordBatch): GArrowTable {
    var error: GErrorPtr;
    var schema: GArrowSchema = garrow_record_batch_get_schema(recordBatches[0].rcbatch);
    var retval: GArrowTable;
    var rbArray = [rb in recordBatches] rb.rcbatch;
    retval = garrow_table_new_record_batches(schema, rbArray, recordBatches.size : guint64, c_ptrTo(error));
    
    if(isNull(retval)){
      g_print("Error creating table: %s\n", error.deref().message);
    }
    return retval;
  }

  proc table(recordBatches: ArrowRecordBatch ...?n){
    var error: GErrorPtr;
    var schema: GArrowSchema = garrow_record_batch_get_schema(recordBatches[0].rcbatch: GArrowRecordBatch);
    var retval: GArrowTable;
    var rbArray = [rb in recordBatches] rb.rcbatch;
    retval = garrow_table_new_record_batches(schema, rbArray, recordBatches.size : guint64, c_ptrTo(error));
    
    if(isNull(retval)){
      printGError("Error creating table:", error);
    }
    return retval;
  }


  // -------------------------- Parquet -------------------------------------

  proc writeTableToParquetFile(table: ArrowTable, path: string) {
    var error: GErrorPtr;
    var writer_properties: GParquetWriterProperties = gparquet_writer_properties_new();
    var writer: GParquetArrowFileWriter = gparquet_arrow_file_writer_new_path(
                                                  garrow_table_get_schema(table.tbl),
                                                  path.c_str(), 
                                                  writer_properties,
                                                  c_ptrTo(error));
  if(isNull(writer)){
      printGError("failed to initialize writer:", error);
      exit(EXIT_FAILURE);
    }
    var success: gboolean = gparquet_arrow_file_writer_write_table(writer,
                                                      table.tbl ,
                                                      10 : guint64, // Should not be hardcoded
                                                      c_ptrTo(error));
    if(!success){
      printGError("failed to write table:", error);
      exit(EXIT_FAILURE);
    }

    success = gparquet_arrow_file_writer_close(writer, c_ptrTo(error));

    if(!success){
      printGError("could not close writer:", error);
      exit(EXIT_FAILURE);
    }
  }

  proc readParquetFileToTable(path: string): ArrowTable {
    var pqFileReader: GParquetArrowFileReader;
    var error: GErrorPtr;
    pqFileReader = gparquet_arrow_file_reader_new_path(path.c_str(), c_ptrTo(error));

    if(isNull(pqFileReader)){
      printGError("failed to open the file:", error);
      exit(EXIT_FAILURE);
    }

    // Reading the whole table
    var table: GArrowTable;
    table = gparquet_arrow_file_reader_read_table(pqFileReader, c_ptrTo(error));
    if(isNull(table)){
      printGError("failed to read the table:", error);
      exit(EXIT_FAILURE);
    }
    var retval = new ArrowTable(table);
    return retval;
  }

}
