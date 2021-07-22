module ArrowDemo {
    require "Arrow.chpl";
    import Arrow;
    use ArrowDecl;

    proc main(){
        var arrowInt64Array = Arrow.array([1,2,3,4]);
        var arrowStringArray = Arrow.array(["foo", "bar", "baz", "some_invalid_value"],[3]);
        var arrowBooleanArray = Arrow.array([true, false, false, true], [1]);

        print_array(arrowInt64Array);
        print_array(arrowStringArray);
        print_array(arrowBooleanArray);

        // TODO implement
        /*
        var schema = Arrow.schema([("field0", ca.int32()),
            ("field1", ca.string()),
            ("field2", ca.bool())]);
        */
        
        var rcbatch = Arrow.recordBatch("field0", arrowInt64Array, "field1", arrowStringArray, "field2", arrowBooleanArray);
        print_record_batch(rcbatch);
        
    }
}