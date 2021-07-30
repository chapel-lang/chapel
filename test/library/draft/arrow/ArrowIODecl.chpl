/*
File: ArrowIODecl.chpl
Purpose: This file contains some functions and types used to deal with
          IO in Arrow.
*/
module ArrowIODecl {
  require "ArrayDecl.chpl", "ArrowRecordDecl.chpl";
  private use ArrayDecl, ArrowRecordDecl;
  use SysCTypes, CPtr;


  // Types used in arrowio.chpl
  extern type GArrowMemoryMappedInputStream = c_void_ptr;

  extern type GArrowRecordBatchFileReader = c_void_ptr;

  extern type GArrowSeekableInputStream = c_void_ptr;

  // Functions used in arrowio.chpl

  extern proc garrow_memory_mapped_input_stream_new(path: c_string, error: c_ptr(GErrorPtr)): GArrowMemoryMappedInputStream;

  extern proc garrow_record_batch_file_reader_new(file: GArrowSeekableInputStream, error: c_ptr(GErrorPtr)): GArrowRecordBatchFileReader;

  extern proc GARROW_SEEKABLE_INPUT_STREAM(ptr: gpointer): GArrowSeekableInputStream;

  extern proc garrow_record_batch_file_reader_get_n_record_batches(reader: GArrowRecordBatchFileReader): guint;

  extern proc garrow_record_batch_file_reader_read_record_batch(reader: GArrowRecordBatchFileReader, i: guint, error: c_ptr(GErrorPtr)): GArrowRecordBatch;

}
