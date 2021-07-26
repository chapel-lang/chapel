module ArrowPrinting {
    require "ArrayDecl.chpl", "ArrowRecordDecl.chpl";
    private use ArrayDecl, ArrowRecordDecl;
    use SysCTypes, CPtr;

    // Functions for printing

    proc printArray(array: GArrowArray){
        var error: GErrorPtr;
        var str: c_string = garrow_array_to_string(array, c_ptrTo(error));
        if(str == ""){
            g_print("Failed to print: %s\n", error.deref().message);
            g_error_free(error);
            return;
        }
        g_print("%s\n",str);
    }

    proc printRecordBatch(recordBatch: GArrowRecordBatch){
        var error: GErrorPtr;
        var str = garrow_record_batch_to_string(recordBatch, c_ptrTo(error));
        if(str == ""){
            g_print("Failed to print: %s\n", error.deref().message);
            g_error_free(error);
            return;
        }
        g_print("%s\n",str);
    }

}
