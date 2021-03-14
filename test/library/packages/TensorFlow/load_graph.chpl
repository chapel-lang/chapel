// https://github.com/Neargye/hello_tf_c_api/blob/ea8800887c/src/load_graph.cpp

// This is a test that reads some data from a file and puts it into a
// TensorFlow graph. It then deletes the graph and declares success.

use TensorFlow.C_TensorFlow;

config const filename = "graph.pb";

proc deallocateBuffer(buf: c_void_ptr, size: size_t) {
  extern "free" proc c_free(ptr: c_void_ptr);
  c_free(buf);
}

proc readBufferFromFile(filename: string) {
  use FileSystem;
  extern type FILE;
  extern "fopen"  proc c_fopen(filename: c_string, mode: c_string): c_ptr(FILE);
  extern "fclose" proc c_fclose(fp: c_ptr(FILE)): c_int;
  extern "fread"  proc c_fread(ptr: c_void_ptr, size: size_t, nitems: size_t, fp: c_ptr(FILE)): size_t;
  extern "malloc" proc c_malloc(length: size_t): c_void_ptr;

  const length = getFileSize(filename);
  var data = c_malloc(length: size_t);

  var infile = c_fopen(filename.c_str(), c"r");
  const readLen = c_fread(data, length:size_t, 1, infile);
  assert(readLen == 1);
  c_fclose(infile);

  var buf: c_ptr(TF_Buffer) = TF_NewBuffer();
  buf.deref().data = data;
  buf.deref().size = length:size_t;
  buf.deref().data_deallocator = c_ptrTo(deallocateBuffer);

  return buf;
}

proc main {
  var buffer = readBufferFromFile(filename),
      graph = TF_NewGraph(),
      status = TF_NewStatus(),
      opts = TF_NewImportGraphDefOptions();

  TF_GraphImportGraphDef(graph, buffer, opts, status);
  TF_DeleteImportGraphDefOptions(opts);
  TF_DeleteBuffer(buffer);

  if TF_GetCode(status) != TF_OK {
    TF_DeleteStatus(status);
    TF_DeleteGraph(graph);
    halt("Can't import GraphDef");
  }

  writeln("Load graph success");
  TF_DeleteStatus(status);
  TF_DeleteGraph(graph);
}
