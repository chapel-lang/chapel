  module Arrow {
  require "ArrowDecl.chpl";
  public use ArrowDecl;
  public use SysCTypes, CPtr;
  
  // -------------------------- Type Declarations and Functions -------------

  type ArrowArray = GArrowArray;

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

  require "ArrowRecord.chpl";
  public use ArrowRecord;


  // -------------------------- Parquet -------------------------------------

  require "ArrowParquet.chpl";
  public use ArrowParquet;

}
