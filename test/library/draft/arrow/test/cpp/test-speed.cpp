#include <iostream>
#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>
#include <parquet/exception.h>
#include <chrono>
#include <ctime>

using namespace std;

const int NUMVALS = 100000;
const int ROWGROUPSIZE = 1000;

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

void write_parquet_file(const arrow::Table& table) {
  std::shared_ptr<arrow::io::FileOutputStream> outfile;
  PARQUET_ASSIGN_OR_THROW(
      outfile,
      arrow::io::FileOutputStream::Open("test-cpp-file.parquet"));

  PARQUET_THROW_NOT_OK(
      parquet::arrow::WriteTable(table, arrow::default_memory_pool(), outfile, ROWGROUPSIZE));
}

std::shared_ptr<arrow::ChunkedArray> read_single_column() {
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
  
  return array;
}

int64_t get_value(std::shared_ptr<arrow::Array> arrow_arr, int i) {
  auto asd = std::static_pointer_cast<arrow::Int64Array>(arrow_arr);
  return asd->Value(i);
}

int64_t* copy_arrow_array(std::shared_ptr<arrow::ChunkedArray> arrow_arr) {
  int64_t cpp_arr[NUMVALS];

  std::shared_ptr<arrow::Array> regular = arrow_arr->chunk(0);
  auto int_arr = std::static_pointer_cast<arrow::Int64Array>(regular);
  
  for(int i = 0; i < NUMVALS; i++)
    cpp_arr[i] = int_arr->Value(i);
  return cpp_arr;
}

int main(int argc, char** argv) {
  std::shared_ptr<arrow::Table> table = generate_table();
  write_parquet_file(*table);
  
  auto start = std::chrono::system_clock::now();
  auto arrow_arr = read_single_column();
  cout << "read column took: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-start).count()/1000.0 << endl;
  
  start = std::chrono::system_clock::now();
  auto cpp_arr = copy_arrow_array(arrow_arr);
  cout << "copy array to C++ array took: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-start).count()/1000.0 << endl;
}
