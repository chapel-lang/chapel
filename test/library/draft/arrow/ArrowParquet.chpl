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
      printGError("failed to initialize writer:", error);
      exit(EXIT_FAILURE);
    }
    var success: gboolean = gparquet_arrow_file_writer_write_table(writer,
                                                      table ,
                                                      10 : guint64, // Should not be hardcoded
                                                      c_ptrTo(error));
    if(!success){
      printGError("failed to write table:", error);
      exit(EXIT_FAILURE);
    }

    success = gparquet_arrow_file_writer_close(writer, c_ptrTo(error));

    if(!success){
      printGError("could not close writer:", error);
      exit(EXIT_FAILURE);
    }
  }

  proc readParquetFileToTable(path: string): GArrowTable {
    var pqFileReader: GParquetArrowFileReader;
    var error: GErrorPtr;
    pqFileReader = gparquet_arrow_file_reader_new_path(path.c_str(), c_ptrTo(error));

    if(isNull(pqFileReader)){
      printGError("failed to open the file:", error);
      exit(EXIT_FAILURE);
    }

    // Reading the whole table
    var table: GArrowTable;
    table = gparquet_arrow_file_reader_read_table(pqFileReader, c_ptrTo(error));
    if(isNull(table)){
      printGError("failed to read the table:", error);
      exit(EXIT_FAILURE);
    }
    return table;
  }
}