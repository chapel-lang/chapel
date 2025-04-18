use Python, List;

config const filename = "data.parquet";


proc getArray(type eltType, ref data_chunks, num_rows) {
  var arr: [0..<num_rows] eltType;
  var i = 0;
  for chunk in data_chunks {
    var chunk_arr =
      chunk!.call(
        owned PyArray(eltType, 1),
        "to_numpy", kwargs=["zero_copy_only"=>false, "writable"=>true]);
    arr[i..#chunk_arr.size] = chunk_arr.array();
    i += chunk_arr.size;
  }
  return arr;
}

proc main() {

  var interp = new Interpreter();

  var pa = interp.importModule("pyarrow");
  var pq = interp.importModule("pyarrow.parquet");

  var parquet_file = pq.call("ParquetFile", filename);
  var columns = parquet_file.get("schema").get("names"):list(string);
  var num_rows = parquet_file.get("metadata").get("num_rows"):int;
  var data_chunks: [0..<columns.size] list(owned Value?);

  for batch in parquet_file.call("iter_batches", kwargs=["batch_size"=>300]) {
    for (col, idx) in zip(columns, 0..) {
      data_chunks[idx].pushBack(batch.call("__getitem__", col));
    }
  }

  var schema_arrow = parquet_file.get('schema_arrow');
  for (col, idx) in zip(columns, 0..) {
    write("Column: ", col);

    var rowType = schema_arrow.call('field', col).get('type');
    if pa.call("int64") == rowType {
      var arr = getArray(int(64), data_chunks[idx], num_rows);
      writeln(" Sum: ", + reduce arr);
    } else if pa.call("float64") == rowType {
      var arr = getArray(real(64), data_chunks[idx], num_rows);
      writeln(" Sum: ", + reduce arr);
    } else {
      writeln("Unknown type");
    }
  }
}


