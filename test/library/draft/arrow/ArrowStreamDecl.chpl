module ArrowStreamDecl {
    require "ArrayDecl.chpl", "ArrowRecordDecl.chpl";
    private use ArrayDecl, ArrowRecordDecl;
    use SysCTypes, CPtr;


    // Types used in arrowstream.c
    
    extern type GArrowResizableBuffer = c_void_ptr;
    
    extern type GArrowBufferOutputStream = c_void_ptr;
    
    extern type GArrowRecordBatchWriter = c_void_ptr;

    extern type GArrowRecordBatchStreamWriter = c_void_ptr;
    
    extern type GArrowBufferInputStream = c_void_ptr;
    
    extern type GArrowBuffer = c_void_ptr;

    extern type GArrowRecordBatchReader = c_void_ptr;
    
    extern type GArrowRecordBatchStreamReader = c_void_ptr;

    extern type GArrowSchema = c_void_ptr;

    extern type GArrowInputStream = c_void_ptr;

    extern type GArrowOutputStream = c_void_ptr;

    // Functions used in arrowstream.c

    extern proc garrow_resizable_buffer_new(initial_size: gint64, error: c_ptr(GErrorPtr)): GArrowResizableBuffer;

    extern proc garrow_buffer_output_stream_new(buffer: GArrowResizableBuffer): GArrowBufferOutputStream;

    extern proc garrow_record_batch_stream_writer_new(sink: GArrowOutputStream, schema: GArrowSchema, error: c_ptr(GErrorPtr)): GArrowRecordBatchStreamWriter;

    extern proc garrow_record_batch_get_schema(record_batch: GArrowRecordBatch): GArrowSchema;

    extern proc garrow_record_batch_writer_write_record_batch(writer: GArrowRecordBatchWriter, record_batch: GArrowRecordBatch, error: c_ptr(GErrorPtr)): gboolean;

    extern proc garrow_buffer_input_stream_new(buffer: GArrowBuffer): GArrowBufferInputStream;

    extern proc garrow_record_batch_stream_reader_new(stream: GArrowInputStream, error: c_ptr(GErrorPtr)): GArrowRecordBatchStreamReader;

    extern proc GARROW_INPUT_STREAM(ptr: gpointer): GArrowInputStream;

    extern proc GARROW_RECORD_BATCH_READER(ptr: gpointer): GArrowRecordBatchReader;

    extern proc garrow_record_batch_reader_read_next(reader: GArrowRecordBatchReader, error: c_ptr(GErrorPtr)): GArrowRecordBatch;
}
