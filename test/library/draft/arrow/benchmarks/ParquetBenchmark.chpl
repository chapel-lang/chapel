require '../lib/ArrowAll.chpl';
use ArrowAll as Arrow;
use Time;

config const n = 10_000;

proc main() {
  var t: Timer;
  t.start();
  createFile();
  writeln(n, " size file created in: ", t.elapsed());
  var done = t.elapsed();

  readFile();
  writeln(n, " size file read in: ", t.elapsed()-done);
}

proc readFile() {
  var t: Timer;
  t.start();
  var error: GErrorPtr;
  var pqFileReader = gparquet_arrow_file_reader_new_path("test-file.parquet".c_str(): c_ptr(gchar), c_ptrTo(error));
  if isNull(pqFileReader) {
    printGError("failed to open file: ", error);
    exit(EXIT_FAILURE);
  }
  var schema = gparquet_arrow_file_reader_get_schema(pqFileReader, c_ptrTo(error));

  writeln("before read column: ", t.elapsed());
  var done = t.elapsed();
  var chunk = gparquet_arrow_file_reader_read_column_data(pqFileReader, 0: gint, c_ptrTo(error));
  writeln("read column took: ", t.elapsed()-done);
  done = t.elapsed();
  var len = garrow_chunked_array_get_n_rows(chunk);
  writeln("get num rows: ", t.elapsed()-done);
  done = t.elapsed();
  var ret: [0..#len] int;
  var loc = garrow_chunked_array_get_chunk(chunk, 0:guint):c_ptr(GArrowInt64Array);
  writeln("get chunk: ", t.elapsed()-done);
  done = t.elapsed();
  forall i in 0..#len {
    ret[i] = garrow_int64_array_get_value(loc, i);
  }
  writeln("after copy: ", t.elapsed()-done);
  return ret;
}

proc createFile() {
  var t: Timer;
  var toParquet = [i in 0..#n] i;

  var table: c_ptr(GArrowTable) = nil;
  var colName = "col-1";

  t.start();
  var col = (new arrowArray(toParquet)).val;
  writeln("creating column took: ", t.elapsed());
  var done = t.elapsed();
  
  var field = garrow_field_new(colName.c_str(): c_ptr(gchar), garrow_array_get_value_data_type(col: c_ptr(GArrowArray)));
  writeln("create field took ", t.elapsed() - done);
  done = t.elapsed();
  var e: GErrorPtr;
  // need to create table
  if table == nil {
    var f: c_ptr(GList) = nil;

    f = g_list_append(f, field);
    var schema = garrow_schema_new(f);

    var vals: c_ptr(GList);
    vals = g_list_append(vals, col);

    table = garrow_table_new_values(schema, vals, c_ptrTo(e));
    writeln("create table took ", t.elapsed() - done);
    done = t.elapsed();
    if isNull(table) {
      printGError("failed to create table: ", e);
    }
  } else { // can just add the array
    var l:c_ptr(GList);
    l = g_list_append(l, col);
    var chunk = garrow_chunked_array_new(l);
    table = garrow_table_add_column(table, 0: guint, field, chunk, c_ptrTo(e));
  }
  var props: c_ptr(GParquetWriterProperties);
  var writer = gparquet_arrow_file_writer_new_path(
                 garrow_table_get_schema(table),
                 "test-file.parquet".c_str(): c_ptr(gchar),
                 props,
                 c_ptrTo(e)
               );
  writeln("before write table took ", t.elapsed() - done);
  done = t.elapsed();
  if(!gparquet_arrow_file_writer_write_table(writer, table, 1:guint64, c_ptrTo(e))) {
    printGError("failed to write table:", e);
    exit(EXIT_FAILURE);
  }
  writeln("write table took ", t.elapsed() - done);
  done = t.elapsed();
  if(!gparquet_arrow_file_writer_close(writer, c_ptrTo(e))){
    printGError("could not close writer:", e);
    exit(EXIT_FAILURE);
  }
  
}
