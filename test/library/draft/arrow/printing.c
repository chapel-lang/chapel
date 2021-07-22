/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "printing.h"

void print_array(GArrowArray *array){
  GArrowType value_type;
  gint64 i, n;

  value_type = garrow_array_get_value_type(array);

  g_print("[");
  n = garrow_array_get_length(array);

#define ARRAY_CASE(type, Type, TYPE, format, val)                       \
  case GARROW_TYPE_ ## TYPE:                                            \
    {                                                                   \
      GArrow ## Type ## Array *real_array;                              \
      real_array = GARROW_ ## TYPE ## _ARRAY(array);                    \
      for (i = 0; i < n; i++) {                                         \
        if (i > 0) {                                                    \
          g_print(", ");                                                \
        }                                                               \
        if(garrow_array_is_valid (array,i)) {                           \
          g_print(format,                                               \
                garrow_ ## type ## _array_get_ ## val (real_array, i)); \
        } else                                                          \
          g_print("NULL");                                              \
      }                                                                 \
    }                                                                   \
    break

  switch (value_type) {
    ARRAY_CASE(uint8,  UInt8,  UINT8,  "%hhu", value);
    ARRAY_CASE(uint16, UInt16, UINT16, "%" G_GUINT16_FORMAT, value);
    ARRAY_CASE(uint32, UInt32, UINT32, "%" G_GUINT32_FORMAT, value);
    ARRAY_CASE(uint64, UInt64, UINT64, "%" G_GUINT64_FORMAT, value);
    ARRAY_CASE( int8,   Int8,   INT8,  "%hhd", value);
    ARRAY_CASE( int16,  Int16,  INT16, "%" G_GINT16_FORMAT, value);
    ARRAY_CASE( int32,  Int32,  INT32, "%" G_GINT32_FORMAT, value);
    ARRAY_CASE( int64,  Int64,  INT64, "%" G_GINT64_FORMAT, value);
    ARRAY_CASE( float,  Float,  FLOAT, "%g", value);
    ARRAY_CASE(double, Double, DOUBLE, "%g", value);
    ARRAY_CASE(boolean, Boolean, BOOLEAN, "%d", value);
    ARRAY_CASE(string, String, STRING, "%s", string);
    ARRAY_CASE(timestamp, Timestamp, TIMESTAMP, "%" G_GINT64_FORMAT, value);
  default:
    break;
  }
#undef ARRAY_CASE

  g_print("]\n");
}

void print_record_batch(GArrowRecordBatch *record_batch){
  guint nth_column, n_columns;

  n_columns = garrow_record_batch_get_n_columns(record_batch);
  for (nth_column = 0; nth_column < n_columns; nth_column++) {
    GArrowArray *array;

    g_print("columns[%u](%s): ",
            nth_column,
            garrow_record_batch_get_column_name(record_batch, nth_column));
    array = garrow_record_batch_get_column_data(record_batch, nth_column);
    print_array(array);
    g_object_unref(array);
  }
}