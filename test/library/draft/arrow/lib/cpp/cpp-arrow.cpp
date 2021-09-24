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

void doWrite(void) {
  arrow::Int64Builder i64builder;
  for(int i = 0; i < NUMVALS; i++)
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

extern "C" {
  void writeParquet(void) {
    doWrite();
  }
}
