#include "cpp-arrow.h"

#include <iostream>
#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>
#include <parquet/exception.h>
#include <parquet/api/reader.h>
#include <chrono>
#include <ctime>

using namespace std;

void doWrite(int numElems, int rowGroupSize) {
  arrow::Int32Builder i32builder;
  for(int i = 0; i < numElems; i++)
    PARQUET_THROW_NOT_OK(i32builder.AppendValues({i}));
  std::shared_ptr<arrow::Array> i64array;
  PARQUET_THROW_NOT_OK(i32builder.Finish(&i64array));

  std::shared_ptr<arrow::Schema> schema = arrow::schema(
                 {arrow::field("int-col", arrow::int32())});

  auto table = arrow::Table::Make(schema, {i64array});

  std::shared_ptr<arrow::io::FileOutputStream> outfile;
  PARQUET_ASSIGN_OR_THROW(
      outfile,
      arrow::io::FileOutputStream::Open("test-cpp-file.parquet"));

  PARQUET_THROW_NOT_OK(
      parquet::arrow::WriteTable(*table, arrow::default_memory_pool(), outfile, rowGroupSize));
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

int cpp_getSize(const char* filename) {
  std::shared_ptr<arrow::io::ReadableFile> infile;
  PARQUET_ASSIGN_OR_THROW(
      infile,
      arrow::io::ReadableFile::Open(filename,
                                    arrow::default_memory_pool()));

  std::unique_ptr<parquet::arrow::FileReader> reader;
  PARQUET_THROW_NOT_OK(
      parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader));
  
  return reader -> parquet_reader() -> metadata() -> num_rows();
}

const char* cpp_getType(const char* filename, const char* colname) {
  std::shared_ptr<arrow::io::ReadableFile> infile;
  PARQUET_ASSIGN_OR_THROW(
      infile,
      arrow::io::ReadableFile::Open(filename,
                                    arrow::default_memory_pool()));

  std::unique_ptr<parquet::arrow::FileReader> reader;
  PARQUET_THROW_NOT_OK(
      parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader));
  
  std::shared_ptr<arrow::Schema> sc;
  std::shared_ptr<arrow::Schema>* out = &sc;
  PARQUET_THROW_NOT_OK(reader->GetSchema(out));

  auto ty = sc -> GetFieldByName(colname) -> type() -> name();
  auto ret = ty.c_str();
  return ret;
  
}

void cpp_readColumnByIndex(const char* filename, void* chpl_arr, int colNum, int numElems) {
  auto chpl_ptr = (int64_t*)chpl_arr;
  
  std::shared_ptr<arrow::io::ReadableFile> infile;
  PARQUET_ASSIGN_OR_THROW(
      infile,
      arrow::io::ReadableFile::Open(filename,
                                    arrow::default_memory_pool()));

  std::unique_ptr<parquet::arrow::FileReader> reader;
  PARQUET_THROW_NOT_OK(
      parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader));
  std::shared_ptr<arrow::ChunkedArray> array;
  PARQUET_THROW_NOT_OK(reader->ReadColumn(colNum, &array));

  std::shared_ptr<arrow::Array> regular = array->chunk(0);
  auto int_arr = std::static_pointer_cast<arrow::Int64Array>(regular);

  for(int i = 0; i < numElems; i++) {
    chpl_ptr[i] = int_arr->Value(i);
  }
}

void cpp_readColumnByName(const char* filename, void* chpl_arr, const char* colname, int numElems) {
  auto chpl_ptr = (int64_t*)chpl_arr;
  
  std::shared_ptr<arrow::io::ReadableFile> infile;
  PARQUET_ASSIGN_OR_THROW(
      infile,
      arrow::io::ReadableFile::Open(filename,
                                    arrow::default_memory_pool()));

  std::unique_ptr<parquet::arrow::FileReader> reader;
  PARQUET_THROW_NOT_OK(
      parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader));
  std::shared_ptr<arrow::ChunkedArray> array;

  std::shared_ptr<arrow::Schema> sc;
  std::shared_ptr<arrow::Schema>* out = &sc;
  PARQUET_THROW_NOT_OK(reader->GetSchema(out));

  auto idx = sc -> GetFieldIndex(colname);

  // TODO: give some kind of indication that it wasn't found
  if(idx == -1)
    idx = 0;
  
  PARQUET_THROW_NOT_OK(reader->ReadColumn(idx, &array));

  std::shared_ptr<arrow::Array> regular = array->chunk(0);
  auto int_arr = std::static_pointer_cast<arrow::Int64Array>(regular);

  for(int i = 0; i < numElems; i++) {
    chpl_ptr[i] = int_arr->Value(i);
  }
}

void cpp_writeColumnToParquet(const char* filename, void* chpl_arr,
                              int colnum, const char* dsetname, int numelems,
                              int rowGroupSize) {
  auto chpl_ptr = (int64_t*)chpl_arr;
  arrow::Int64Builder i64builder;
  for(int i = 0; i < numelems; i++)
    PARQUET_THROW_NOT_OK(i64builder.AppendValues({chpl_ptr[i]}));
  std::shared_ptr<arrow::Array> i64array;
  PARQUET_THROW_NOT_OK(i64builder.Finish(&i64array));

  std::shared_ptr<arrow::Schema> schema = arrow::schema(
                 {arrow::field(dsetname, arrow::int64())});

  auto table = arrow::Table::Make(schema, {i64array});

  std::shared_ptr<arrow::io::FileOutputStream> outfile;
  PARQUET_ASSIGN_OR_THROW(
      outfile,
      arrow::io::FileOutputStream::Open(filename));

  PARQUET_THROW_NOT_OK(
  parquet::arrow::WriteTable(*table, arrow::default_memory_pool(), outfile, rowGroupSize));
}

int cpp_lowLevelRead(const char* filename, void* chpl_arr, const char* colname, int numElems) {
  auto chpl_ptr = (int64_t*)chpl_arr;
  std::shared_ptr<arrow::io::ReadableFile> infile;
  PARQUET_ASSIGN_OR_THROW(
      infile,
      arrow::io::ReadableFile::Open(filename,
                                    arrow::default_memory_pool()));

  std::unique_ptr<parquet::arrow::FileReader> reader;
  PARQUET_THROW_NOT_OK(
      parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader));

  std::shared_ptr<arrow::Schema> sc;
  std::shared_ptr<arrow::Schema>* out = &sc;
  PARQUET_THROW_NOT_OK(reader->GetSchema(out));

  auto idx = sc -> GetFieldIndex(colname);
  if(idx == -1) idx = 0;
  
  try {
    // Create a ParquetReader instance
    std::unique_ptr<parquet::ParquetFileReader> parquet_reader =
        parquet::ParquetFileReader::OpenFile(filename, false);

    // Get the File MetaData
    std::shared_ptr<parquet::FileMetaData> file_metadata = parquet_reader->metadata();

    // Get the number of RowGroups
    int num_row_groups = file_metadata->num_row_groups();

    // Get the number of Columns
    int num_columns = file_metadata->num_columns();

    // Iterate over all the RowGroups in the file
    for (int r = 0; r < num_row_groups; ++r) {
      // Get the RowGroup Reader
      std::shared_ptr<parquet::RowGroupReader> row_group_reader =
        parquet_reader->RowGroup(r);

      int64_t values_read = 0;
      int64_t rows_read = 0;
      int16_t definition_level;
      int16_t repetition_level;
      int i;
      std::shared_ptr<parquet::ColumnReader> column_reader;

      ARROW_UNUSED(rows_read); // prevent warning in release build

      // Get the Column Reader
      column_reader = row_group_reader->Column(idx);
      parquet::Int64Reader* int64_reader =
        static_cast<parquet::Int64Reader*>(column_reader.get());
      
      // Read all the rows in the column
      i = 0;
      while (int64_reader->HasNext()) {
        // Read one value at a time. The number of rows read is returned. values_read
        // contains the number of non-null rows
        rows_read = int64_reader->ReadBatch(10, nullptr, nullptr, &chpl_ptr[i], &values_read);
        i+=10;
      }
    }
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Parquet write error: " << e.what() << std::endl;
    return -1;
  }
}

const char* cpp_getVersionInfo(void) {
  return arrow::GetBuildInfo().version_string.c_str();
}

extern "C" {
  // test function, writes 0..#numElems to file
  // can't write your own array yet
  void writeParquet(int numElems, int rowGroupSize) {
    doWrite(numElems, rowGroupSize);
  }

  // read the first column from a hardcoded string
  void readParquet(void* chpl_arr, int numElems) {
    doRead(chpl_arr, numElems);
  }

  // get size from metadata
  // don't see these calls documented anywhere, but I
  // stole them from the GLib implementation
  int c_doSize(const char* chpl_str) {
    return cpp_getSize(chpl_str);
  }

  // first attempt at something that could actually be used
  void c_readColumnByIndex(const char* filename, void* chpl_arr, int colNum, int numElems) {
    cpp_readColumnByIndex(filename, chpl_arr, colNum, numElems);
  }

  // :) turn me up
  void c_readColumnByName(const char* filename, void* chpl_arr, const char* colname, int numElems) {
    cpp_readColumnByName(filename, chpl_arr, colname, numElems);
  }

  const char* c_getType(const char* filename, const char* colname) {
    return cpp_getType(filename, colname);
  }

  void c_writeColumnToParquet(const char* filename, void* chpl_arr,
                              int colnum, const char* dsetname, int numelems,
                              int rowGroupSize) {
    cpp_writeColumnToParquet(filename, chpl_arr, colnum, dsetname,
                             numelems, rowGroupSize);
  }

  void c_lowLevelRead(const char* filename, void* chpl_arr, const char* colname, int numElems) {
    cpp_lowLevelRead(filename, chpl_arr, colname, numElems);
  }

  const char* c_getVersionInfo(void) {
    return cpp_getVersionInfo();
  }
}
