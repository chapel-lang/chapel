module ArrowParquet {
    require "ArrayDecl.chpl", "ParquetDecl.chpl";
    use ArrayDecl, ParquetDecl;
    use SysCTypes, CPtr;

    proc writeTableToParquetFile(table: GArrowTable, path: string) {
        
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