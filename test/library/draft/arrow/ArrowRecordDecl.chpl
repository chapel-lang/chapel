module ArrowRecordDecl {
    require "arrow-glib/arrow-glib.h", "-larrow-glib", "-lglib-2.0", "-lgobject-2.0";
    require "ArrayDecl.chpl";
    private use ArrayDecl;
    use SysCTypes, CPtr;


    // Types used in arrowrecord.c
    
    extern type GArrowRecordBatch = c_void_ptr;

    extern type GArrowField = c_void_ptr;

    extern type GList = c_void_ptr;

    extern type GArrowSchema = c_void_ptr;

    // Types and type function also in buildDecl.chpl
    
    // Functions used in arrowrecord.c

    extern proc garrow_field_new(name: c_string, data_type: GArrowDataType): GArrowField;

    extern proc g_list_append(list: GList, data: gpointer): GList;

    extern proc garrow_schema_new(fields: GList): GArrowSchema;

    extern proc garrow_record_batch_new(schema: GArrowSchema, n_rows: guint32, columns: GList, error: c_ptr(GErrorPtr)): GArrowRecordBatch;

    extern proc garrow_record_batch_to_string (record_batch: GArrowRecordBatch, error: c_ptr(GErrorPtr)): c_string;

}
