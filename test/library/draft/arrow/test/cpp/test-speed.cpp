#include <iostream>
#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>
#include <parquet/exception.h>
#include <chrono>
#include<ctime>
using namespace std;

const int NUMVALS = 100000;
const int ROWGROUPSIZE = 1000;

// #0 Build dummy data to pass around
// To have some input data, we first create an Arrow Table that holds
// some data.
std::shared_ptr<arrow::Table> generate_table() {
  arrow::Int64Builder i64builder;
  for(int i = 0; i < NUMVALS; i++)
    PARQUET_THROW_NOT_OK(i64builder.AppendValues({i}));
  std::shared_ptr<arrow::Array> i64array;
  PARQUET_THROW_NOT_OK(i64builder.Finish(&i64array));

  std::shared_ptr<arrow::Schema> schema = arrow::schema(
                 {arrow::field("int", arrow::int64())});

  return arrow::Table::Make(schema, {i64array});
}

// #1 Write out the data as a Parquet file
void write_parquet_file(const arrow::Table& table) {
  std::shared_ptr<arrow::io::FileOutputStream> outfile;
  PARQUET_ASSIGN_OR_THROW(
      outfile,
      arrow::io::FileOutputStream::Open("test-cpp-file.parquet"));
  // The last argument to the function call is the size of the RowGroup in
  // the parquet file. Normally you would choose this to be rather large but
  // for the example, we use a small value to have multiple RowGroups.
  PARQUET_THROW_NOT_OK(
      parquet::arrow::WriteTable(table, arrow::default_memory_pool(), outfile, ROWGROUPSIZE));
}

// #4: Read only a single column of the whole parquet file
void read_single_column() {
  std::cout << "Reading first column of test-cpp-file.parquet" << std::endl;
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
  PARQUET_THROW_NOT_OK(arrow::PrettyPrint(*array, 4, &std::cout));
  std::cout << std::endl;
}

int main(int argc, char** argv) {
  std::shared_ptr<arrow::Table> table = generate_table();
  write_parquet_file(*table);
  auto start = std::chrono::system_clock::now();
  read_single_column();
  cout << "read column took: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-start).count()/1000.0 << endl;
}
