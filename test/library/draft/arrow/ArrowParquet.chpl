module ArrowParquet {
  require "ArrayDecl.chpl", "ParquetDecl.chpl", "ArrowRecordDecl.chpl";
  use ArrayDecl, ParquetDecl, ArrowRecordDecl;
  use SysCTypes, CPtr;

  proc writeTableToParquetFile(table: GArrowTable, path: string) {
    var error: GErrorPtr;
    var writer_properties: GParquetWriterProperties = gparquet_writer_properties_new();
    var writer: GParquetArrowFileWriter = gparquet_arrow_file_writer_new_path(
                                                  garrow_table_get_schema(table),
                                                  path.c_str(), 
                                                  writer_properties,
                                                  c_ptrTo(error));
  if(isNull(writer)){
      g_print("failed to initialize writer: %s\n", error.deref().message);
      g_error_free(error);
      exit(EXIT_FAILURE);
    }
    var success: gboolean = gparquet_arrow_file_writer_write_table(writer,
                                                      table ,
                                                      10 : guint64,
                                                      c_ptrTo(error));
    if(!success){
      g_print("failed to write table: %s\n", error.deref().message);
      g_error_free(error);
      exit(EXIT_FAILURE);
    }

    success = gparquet_arrow_file_writer_close(writer, c_ptrTo(error));

    if(!success){
      g_print("could not close writer: %s\n", error.deref().message);
      g_error_free(error);
      exit(EXIT_FAILURE);
    }
  }

  proc readParquetFileToTable(path: string): GArrowTable {
    var pqFileReader: GParquetArrowFileReader;
    var error: GError;
    var errptr = c_ptrTo(error);
    pqFileReader = gparquet_arrow_file_reader_new_path(path.c_str(), c_ptrTo(errptr));

    if(isNull(pqFileReader)){
      g_print("failed to open the file: %s\n", error.message);
      g_error_free(c_ptrTo(error));
      exit(EXIT_FAILURE);
    }

    // Reading the whole table
    var table: GArrowTable;
    table = gparquet_arrow_file_reader_read_table(pqFileReader, c_ptrTo(errptr));
    if(isNull(table)){
      g_print("failed to read the table: %s\n", error.message);
      g_error_free(c_ptrTo(error));
      exit(EXIT_FAILURE);
    }
    return table;
  }
}