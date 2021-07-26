module ArrowRecord {
    // As opposed to other modules with similar names to Chapel files, this one is not
    // a transliteration of arrowrecord.c into Chapel.
    // Rather it is the high-level module used to build a record batch and schema and tables 
    // and stuff.
    require "ArrayDecl.chpl", "ArrowRecordDecl.chpl";
    private use ArrayDecl, ArrowRecordDecl;
    use CPtr;

    proc recordBatch (args ...?n): GArrowRecordBatch {

        
        // Verifying the Integrity of the arguments
        if(n%2==1) then
            compilerError("Mismatched arguments");
        for param i in 0..#n {
            if i%2 == 0 {
                if args[i].type != string then
                    compilerError("Wrong even argument type");
            } else {
                if args[i].type != GArrowArray then
                    compilerError("Wrong odd argument type");
            }
        }
        
        // for param i in 0..#n {
        //     writeln(args(i));
        // }
        // writeln(args[1]);
        // Making a schema for the record batch.

        
        var fields: GList = getNULL();
        for param i in 1..n by 2{
            // Building the (column)
            var col: GArrowField = garrow_field_new(
                args[i-1].c_str(), 
                garrow_array_get_value_data_type(args[i]: GArrowArray));

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
        var n_rows: guint32 = garrow_array_get_length(args[1]): guint32;

        var arrays: GList = getNULL();
        for param j in 1..n by 2 {
            // Adding the array to the list
            arrays = g_list_append(arrays, args[j]);
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

}
