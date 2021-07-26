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
        
        var rcbatch = Arrow.recordBatch("field0", arrowInt64Array, "field1", arrowStringArray, "field2", arrowBooleanArray);

        writeln("\nRecord batch from above arrays:");
        printRecordBatch(rcbatch);
        
    }
}
