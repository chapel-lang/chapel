module PrintingDecl {
    require "arrow-glib/arrow-glib.h", "-larrow-glib", "-lglib-2.0", "-lgobject-2.0";
    require "printing.h", "printing.c";
    require "ArrayDecl.chpl";
    private use ArrayDecl;
    use SysCTypes, CPtr;

    // Types used for printing
    extern type GArrowRecordBatch = c_void_ptr;

    // Functions for printing

    extern proc print_array(array: GArrowArray);

    extern proc print_record_batch(record_batch: GArrowRecordBatch);

    // Used to build standard record batch in examples
    //extern proc build_record_batch_for_me_please(): GArrowRecordBatch;
}