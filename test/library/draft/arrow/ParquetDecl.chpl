module ParquetDecl {
    require "parquet-glib/parquet-glib.h", "-lparquet-glib";
    require "ArrayDecl.chpl", "ArrowRecordDecl.chpl";
    private use ArrayDecl, ArrowRecordDecl;
    use SysCTypes, CPtr;


    // Types used in parquet-arrow.c
    extern type GParquetArrowFileReader = c_void_ptr;
    
    extern type GParquetArrowFileWriter = c_void_ptr;
    
    extern type GParquetWriterProperties = c_void_ptr;

    extern type GArrowTable = c_void_ptr;

    extern type gchar = c_char;

    // Functions used in parquet-arrow.c

    extern proc gparquet_arrow_file_reader_new_path(path: c_string, error: c_ptr(c_ptr(GError))): GParquetArrowFileReader;

    extern proc gparquet_arrow_file_reader_read_table(reader: GParquetArrowFileReader, error: c_ptr(c_ptr(GError))): GArrowTable;

    extern proc garrow_table_to_string(table: GArrowTable, error: c_ptr(c_ptr(GError))): c_string;

    extern proc gparquet_arrow_file_writer_new_path(schema : GArrowSchema, path : c_string, 
                                                    writer_properties : GParquetWriterProperties,
                                                    error : c_ptr(c_ptr(GError))) : GParquetArrowFileWriter;
    
    extern proc gparquet_arrow_file_writer_write_table(writer : GParquetArrowFileWriter,
                                                        table : GArrowTable,
                                                        chunk_size : guint64,
                                                        error : c_ptr(c_ptr(GError))) : gboolean;
    
    extern proc gparquet_arrow_file_writer_close(writer : GParquetArrowFileWriter, error : c_ptr(c_ptr(GError))) : gboolean;



}
