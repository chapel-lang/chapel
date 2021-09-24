#include "cpp-arrow.h"

#include <iostream>
#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>
#include <parquet/exception.h>
#include <chrono>
#include <ctime>

using namespace std;

void doWrite(int numElems) {
  arrow::Int64Builder i64builder;
  for(int i = 0; i < numElems; i++)
    PARQUET_THROW_NOT_OK(i64builder.AppendValues({i}));
  std::shared_ptr<arrow::Array> i64array;
  PARQUET_THROW_NOT_OK(i64builder.Finish(&i64array));

  std::shared_ptr<arrow::Schema> schema = arrow::schema(
                 {arrow::field("int", arrow::int64())});

  auto table = arrow::Table::Make(schema, {i64array});

  std::shared_ptr<arrow::io::FileOutputStream> outfile;
  PARQUET_ASSIGN_OR_THROW(
      outfile,
      arrow::io::FileOutputStream::Open("test-cpp-file.parquet"));

  PARQUET_THROW_NOT_OK(
      parquet::arrow::WriteTable(*table, arrow::default_memory_pool(), outfile, ROWGROUPSIZE));
}

void doRead(void* chpl_arr, int numElems) {
  auto chpl_ptr = (int64_t*)chpl_arr;
  
  std::shared_ptr<arrow::io::ReadableFile> infile;
  PARQUET_ASSIGN_OR_THROW(
      infile,
      arrow::io::ReadableFile::Open("test-cpp-file.parquet",
                                    arrow::default_memory_pool()));

  std::unique_ptr<parquet::arrow::FileReader> reader;
  PARQUET_THROW_NOT_OK(
      parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader));
  std::shared_ptr<arrow::ChunkedArray> array;
  PARQUET_THROW_NOT_OK(reader->ReadColumn(0, &array));

  std::shared_ptr<arrow::Array> regular = array->chunk(0);
  auto int_arr = std::static_pointer_cast<arrow::Int64Array>(regular);

  for(int i = 0; i < numElems; i++) {
    chpl_ptr[i] = int_arr->Value(i);
  }
}

extern "C" {
  void writeParquet(int numElems) {
    doWrite(numElems);
  }

  void readParquet(void* chpl_arr, int numElems) {
    doRead(chpl_arr, numElems);
  }
}
