/*
File: ArrayDecl.chpl
Purpose: This file contains some functions and types used to build
        and deal with Arrow Arrays
*/
module ArrayDecl {
  require "arrow-glib/arrow-glib.h", "-larrow-glib", "-lglib-2.0", "-lgobject-2.0";
  require "null.h", "null.c";

  extern const EXIT_FAILURE: int ;
  extern const EXIT_SUCCESS: int;
  extern const G_GINT64_FORMAT: c_string;
  extern const FALSE : int;
  extern const TRUE: int;

  // Types used in build.c

  use SysCTypes;

  extern type GArrowArray = c_void_ptr;

  extern type GArrowInt32ArrayBuilder = c_void_ptr;

  extern type GArrowInt64ArrayBuilder = c_void_ptr;
  
  extern type GArrowStringArrayBuilder = c_void_ptr;
  
  extern type GArrowBooleanArrayBuilder = c_void_ptr;

  extern type GArrowArrayBuilder = c_void_ptr;

  extern type GArrowInt32Array = c_void_ptr;

  extern type GArrowInt64Array = c_void_ptr;
  
  extern type GArrowStringArray = c_void_ptr;
  
  extern type GArrowBooleanArray = c_void_ptr;

  extern type gboolean = c_int;

  extern type gint = c_int;

  extern type guint = c_uint;

  extern type guint32 = guint;

  extern type guint64 = uint(64);

  extern type GQuark = c_uint;

  extern type gint32 = int(32);

  extern type gint64 = int(64);

  extern type gpointer = c_void_ptr;

  extern type GArrowDataType = c_void_ptr;

  extern type GArrowInt32DataType = GArrowDataType;
  
  extern type GArrowInt64DataType = GArrowDataType;

  extern type GArrowStringDataType = GArrowDataType;

  extern type GArrowBooleanDataType = GArrowDataType;

  // Type functions

  extern proc garrow_int32_data_type_new(): GArrowInt32DataType;
  
  extern proc garrow_int64_data_type_new(): GArrowInt64DataType;

  extern proc garrow_string_data_type_new(): GArrowStringDataType;

  extern proc garrow_boolean_data_type_new(): GArrowBooleanDataType;


  // Typedef'd structs

  extern record GError {
      extern "domain" var domains: GQuark;
      var code: gint;
      var message: c_string;
  }

  type GErrorPtr = c_ptr(GError);
  
  // Functions used in build.c

  extern proc garrow_int32_array_builder_new(): GArrowInt32ArrayBuilder;
  extern proc garrow_int64_array_builder_new(): GArrowInt64ArrayBuilder;
  extern proc garrow_string_array_builder_new(): GArrowStringArrayBuilder;
  extern proc garrow_boolean_array_builder_new(): GArrowBooleanArrayBuilder;

  extern proc garrow_int32_array_builder_append_value(
                                  builder: GArrowInt32ArrayBuilder,
                                  value: gint32,
                                  error: c_ptr(c_ptr(GError))): gboolean;
  extern proc garrow_int64_array_builder_append_value(
                                  builder: GArrowInt64ArrayBuilder,
                                  value: gint64,
                                  error: c_ptr(c_ptr(GError))): gboolean;
  extern proc garrow_string_array_builder_append_string(
                                  builder: GArrowStringArrayBuilder,
                                  value: c_string,
                                  error: c_ptr(c_ptr(GError))): gboolean;
  extern proc garrow_boolean_array_builder_append_value(
                                  builder: GArrowBooleanArrayBuilder,
                                  value: gboolean,
                                  error: c_ptr(c_ptr(GError))): gboolean;

  extern proc garrow_int32_array_builder_append_values(
                                  builder: GArrowInt32ArrayBuilder, 
                                  values: [] gint32, 
                                  values_length: gint64, 
                                  is_valids: [] gboolean,
                                  is_valids_length: gint64, 
                                  error: c_ptr(GErrorPtr)): gboolean;
  extern proc garrow_int64_array_builder_append_values(
                                  builder: GArrowInt64ArrayBuilder, 
                                  values: [] gint64, 
                                  values_length: gint64, 
                                  is_valids: [] gboolean,
                                  is_valids_length: gint64, 
                                  error: c_ptr(GErrorPtr)): gboolean;
  extern proc garrow_string_array_builder_append_strings(
                                  builder: GArrowStringArrayBuilder, 
                                  values: [] c_string, 
                                  values_length: gint64, 
                                  is_valids: [] gboolean,
                                  is_valids_length: gint64, 
                                  error: c_ptr(GErrorPtr)): gboolean;
  extern proc garrow_boolean_array_builder_append_values(
                                  builder: GArrowBooleanArrayBuilder, 
                                  values: [] gboolean, 
                                  values_length: gint64, 
                                  is_valids: [] gboolean,
                                  is_valids_length: gint64, 
                                  error: c_ptr(GErrorPtr)): gboolean;


  extern proc g_print(fmt: c_string, vals...?numvals);

  extern proc g_error_free(error: c_ptr(GError));

  extern proc g_object_unref(object: gpointer);

  extern proc garrow_array_builder_finish(builder: GArrowArrayBuilder, error: c_ptr(c_ptr(GError))): GArrowArray;

  extern proc GARROW_ARRAY_BUILDER(ptr: gpointer): GArrowArrayBuilder;

  extern proc garrow_array_get_value_data_type (array: GArrowArray): GArrowDataType;

  extern proc garrow_array_get_length(array: GArrowArray): gint64;

  extern proc GARROW_INT32_ARRAY(ptr: gpointer): GArrowInt32Array;

  extern proc garrow_int32_array_get_value(array: GArrowInt32Array, i: gint64): gint32;

  extern proc garrow_array_to_string (array: GArrowArray, error: c_ptr(GErrorPtr)): c_string;

  extern proc isNull(ptr: c_void_ptr): c_int;

  extern proc getNULL(): c_void_ptr;



  // Temporary Error caller for convenience.
  inline proc printGError(msg: string, error: GErrorPtr){
    g_print("%s %s\n", msg.c_str(), error.deref().message);
    g_error_free(error);
  }
}
