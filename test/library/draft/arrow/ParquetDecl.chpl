module ParquetDecl {
    require "parquet-glib/parquet-glib.h", "-lparquet-glib";
    require "ArrayDecl.chpl";
    private use ArrayDecl;
    use SysCTypes;


    // Types used in parquet-arrow.c
    extern type GParquetArrowFileReader = c_void_ptr;

    extern type GArrowTable = c_void_ptr;

    extern type gchar = c_char;

    // Functions used in parquet-arrow.c

    extern proc gparquet_arrow_file_reader_new_path(path: c_string, error: c_ptr(c_ptr(GError))): GParquetArrowFileReader;

    extern proc gparquet_arrow_file_reader_read_table(reader: GParquetArrowFileReader, error: c_ptr(c_ptr(GError))): GArrowTable;

    extern proc garrow_table_to_string(table: GArrowTable, error: c_ptr(c_ptr(GError))): c_ptr(gchar);
}