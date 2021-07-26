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

    proc array(array: [?arrDom] ?arrayType, validity: [?validityDom] int = [-1], 
                specifiedType = nil): GArrowArray {
        var fakeReturn: GArrowArray;
        // Sorting based on the specifiedType and calling the appropriate function
        //if(!specifiedType)
        select arrayType {
            when int do return int64Array(array, validity);
            when string do return stringArray(array, validity);
            when bool do return boolArray(array, validity);
            otherwise {
                writeln ("Unsupported type, \nreturning nil"); 
                return fakeReturn;
            }
        }
    }
    
    proc int64Array(array: [?arrDom] int, validity: [?validityDom] int) : GArrowInt64Array {
        
        var retval: GArrowInt64Array;
        
        var builder: GArrowInt64ArrayBuilder;
        var success: gboolean = 1;
        var error: GErrorPtr;

        builder = garrow_int64_array_builder_new();
        //writeln("0");

        if (success) {
            //writeln("1");
            var intArrLen: gint64 = array.size: gint64;
            //writeln("1.1 + ", intArrLen, " " , array.size);
            var intValArr: [0..#intArrLen] gint64;
            forall i in 0..#intArrLen {
                intValArr[i] = array[i]: gint64;
            }
            //writeln("2 + ", intValArr);

            var intValidityArr: [0..#intArrLen] gboolean;
            forall i in 0..#intValidityArr.size {
                intValidityArr[i] = 1;
            }
            if(validity[0] != -1){
                forall i in 0..#validity.size {
                    intValidityArr[validity[i]] = 0;
                }
            }
            //writeln("3 + " , intValidityArr);
            var intValidityArrLen: gint64 = intArrLen;
            success = garrow_int64_array_builder_append_values(
                builder, intValArr, intArrLen, intValidityArr, intValidityArrLen, c_ptrTo(error));
        }
        if (!success) {
        g_print("failed to append: %s\n", error.deref().message);
        g_error_free(error);
        g_object_unref(builder);
        return retval;
        }
        retval = garrow_array_builder_finish(GARROW_ARRAY_BUILDER(builder), c_ptrTo(error));
        if (isNull(retval)) {
            g_print("failed to finish: %s\n", error.deref().message);
            g_error_free(error);
            g_object_unref(builder);
            return retval;
        }
        g_object_unref(builder); 

        return retval;
    }
    proc int32Array(array: [?arrDom] int(32), validity: [?validityDom] int) : GArrowInt32Array {
        
        var retval: GArrowInt32Array;
        
        var builder: GArrowInt32ArrayBuilder;
        var success: gboolean = 1;
        var error: GErrorPtr;

        builder = garrow_int32_array_builder_new();
        //writeln("0");

        if (success) {
            //writeln("1");
            var intArrLen: gint64 = array.size: gint64;
            //writeln("1.1 + ", intArrLen, " " , array.size);
            var intValArr: [0..#intArrLen] gint32;
            forall i in 0..#intArrLen {
                intValArr[i] = array[i]: gint32;
            }
            //writeln("2 + ", intValArr);

            var intValidityArr: [0..#intArrLen] gboolean;
            forall i in 0..#intValidityArr.size {
                intValidityArr[i] = 1;
            }
            if(validity[0] != -1){
                forall i in 0..#validity.size {
                    intValidityArr[validity[i]] = 0;
                }
            }
            //writeln("3 + " , intValidityArr);
            var intValidityArrLen: gint64 = intArrLen;
            success = garrow_int32_array_builder_append_values(
                builder, intValArr, intArrLen, intValidityArr, intValidityArrLen, c_ptrTo(error));
        }
        if (!success) {
        g_print("failed to append: %s\n", error.deref().message);
        g_error_free(error);
        g_object_unref(builder);
        return retval;
        }
        retval = garrow_array_builder_finish(GARROW_ARRAY_BUILDER(builder), c_ptrTo(error));
        if (isNull(retval)) {
            g_print("failed to finish: %s\n", error.deref().message);
            g_error_free(error);
            g_object_unref(builder);
            return retval;
        }
        g_object_unref(builder); 

        return retval;
    }
    proc stringArray(array: [?arrDom] string, validity: [?validityDom] int) : GArrowStringArray {
        
        var retval: GArrowStringArray;
        
        var builder: GArrowStringArrayBuilder;
        var success: gboolean = 1;
        var error: GErrorPtr;

        builder = garrow_string_array_builder_new();

        if (success) {

            var strArrLen: gint64 = array.size: gint64;

            var strValArr: [0..#strArrLen] c_string;
            forall i in 0..#strArrLen {
                strValArr[i] = array[i].c_str();
            }

            var strValidityArr: [0..#strArrLen] gboolean;
            forall i in 0..#strValidityArr.size {
                strValidityArr[i] = 1;
            }
            if(validity[0] != -1){
                forall i in 0..#validity.size {
                    strValidityArr[validity[i]] = 0;
                }
            }
            var strValidityArrLen: gint64 = strArrLen;
            success = garrow_string_array_builder_append_strings(
                builder, strValArr, strArrLen, strValidityArr, strValidityArrLen, c_ptrTo(error));
        }
        if (!success) {
        g_print("failed to append: %s\n", error.deref().message);
        g_error_free(error);
        g_object_unref(builder);
        return retval;
        }
        retval = garrow_array_builder_finish(GARROW_ARRAY_BUILDER(builder), c_ptrTo(error));
        if (isNull(retval)) {
            g_print("failed to finish: %s\n", error.deref().message);
            g_error_free(error);
            g_object_unref(builder);
            return retval;
        }
        g_object_unref(builder); 

        return retval;
    }
    proc boolArray(array: [?arrDom] bool, validity: [?validityDom] int) : GArrowBooleanArray {
        
        var retval: GArrowBooleanArray;
        
        var builder: GArrowBooleanArrayBuilder;
        var success: gboolean = 1;
        var error: GErrorPtr;

        builder = garrow_boolean_array_builder_new();

        if (success) {

            var boolArrLen: gint64 = array.size: gint64;

            var boolValArr: [0..#boolArrLen] gboolean;
            forall i in 0..#boolArrLen {
                boolValArr[i] = array[i]: gboolean;
            }

            var boolValidityArr: [0..#boolArrLen] gboolean;
            forall i in 0..#boolValidityArr.size {
                boolValidityArr[i] = 1;
            }
            if(validity[0] != -1){
                forall i in 0..#validity.size {
                    boolValidityArr[validity[i]] = 0;
                }
            }
            var boolValidityArrLen: gint64 = boolArrLen;
            success = garrow_boolean_array_builder_append_values(
                builder, boolValArr, boolArrLen, boolValidityArr, boolValidityArrLen, c_ptrTo(error));
        }
        if (!success) {
        g_print("failed to append: %s\n", error.deref().message);
        g_error_free(error);
        g_object_unref(builder);
        return retval;
        }
        retval = garrow_array_builder_finish(GARROW_ARRAY_BUILDER(builder), c_ptrTo(error));
        if (isNull(retval)) {
            g_print("failed to finish: %s\n", error.deref().message);
            g_error_free(error);
            g_object_unref(builder);
            return retval;
        }
        g_object_unref(builder); 

        return retval;
    }

    // ---------------------- Record Batches and schemas ----------------------

    require "ArrowRecord.chpl";
    public use ArrowRecord;
}
