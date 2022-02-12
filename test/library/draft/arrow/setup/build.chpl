module Build{
  require "../parquetHeaders.chpl";
  public use parquetHeaders;
  
  proc main(): int{

    use CTypes, CPtr;
    var array: c_ptr(GArrowArray);

    {
      var builder: c_ptr(GArrowInt32ArrayBuilder);
      var success: gboolean = 1;
      var error: GError;
      var errptr: GErrorPtr = c_ptrTo(error);

      builder = garrow_int32_array_builder_new();
      if (success) {
        success = garrow_int32_array_builder_append_value(builder, 29, c_ptrTo(errptr));
      }
      if (success) {
        success = garrow_int32_array_builder_append_value(builder, 2929, c_ptrTo(errptr));
      }
      if (success) {
        success = garrow_int32_array_builder_append_value(builder, 292929, c_ptrTo(errptr));
      }
      if (!success) {
        g_print("failed to append: %s\n".c_str(): c_ptr(gchar), error.message);
        g_error_free(c_ptrTo(error));
        g_object_unref(builder);
        return EXIT_FAILURE;
      }
      array = garrow_array_builder_finish(GARROW_ARRAY_BUILDER(builder), c_ptrTo(errptr));
      if (isNull(array)) {
        g_print("failed to finish: %s\n".c_str(): c_ptr(gchar), error.message);
        g_error_free(c_ptrTo(error));
        g_object_unref(builder);
        return EXIT_FAILURE;
      }
      g_object_unref(builder);
    }

    {
      var i, n: gint64;

      n = garrow_array_get_length(array);
      g_print("length: %lli\n".c_str() : c_ptr(gchar), n);
      for i in 0..#n {
        var value: gint32;

        value = garrow_int32_array_get_value(GARROW_INT32_ARRAY(array), i);
        g_print("array[%lli] = %d\n".c_str(): c_ptr(gchar),
                i, value);
      }
    }

    g_object_unref(array);

    return EXIT_SUCCESS;
  }
}
