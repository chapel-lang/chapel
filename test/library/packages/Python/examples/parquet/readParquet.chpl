use Python, List;

config const filename = "data.parquet";


proc getArray(type eltType, ref data_chunks, num_rows) {
  // Creates an array to store all the unified chunks
  var arr: [0..<num_rows] eltType;
  var i = 0;
  // Traverse the list of Python `Value`s provided to us
  for chunk in data_chunks {
    // Call `to_numpy` on the individual Python `Value`s
    // This returns a `PyArray` of the specified element type
    var chunk_arr =
      chunk!.call(
        owned PyArray(eltType, 1),
        "to_numpy", kwargs=["zero_copy_only"=>false, "writable"=>true]);
    // (Continue to) fill in `arr` with the contents of that numpy array
    arr[i..#chunk_arr.size] = chunk_arr.array();
    i += chunk_arr.size;
  }
  return arr;
}

proc main() {

  var interp = new Interpreter();

  var pa = interp.importModule("pyarrow");
  var pq = interp.importModule("pyarrow.parquet");

  // Open a Parquet file
  var parquet_file = pq.call("ParquetFile", filename);
  var columns = parquet_file.get("schema").get("names"):list(string);
  var num_rows = parquet_file.get("metadata").get("num_rows"):int;
  // Create an array of lists to store the result
  var data_chunks: [0..<columns.size] list(owned Value?);

  // Iterate over the Parquet file
  for batch in parquet_file.call("iter_batches", kwargs=["batch_size"=>300]) {
    // In each batch, get each column and store it into a list in the array
    for (col, idx) in zip(columns, 0..) {
      data_chunks[idx].pushBack(batch.call("__getitem__", col));
    }
  }

  // schema_arrow is used to get the type stored in the column
  var schema_arrow = parquet_file.get('schema_arrow');
  for (col, idx) in zip(columns, 0..) {
    write("Column: ", col);

    var rowType = schema_arrow.call('field', col).get('type');
    if pa.call("int64") == rowType {
      // getArray traverses the list stored at each idx, using numpy to gather
      // chunks into a single array
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


