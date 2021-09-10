module ArrowAll {
  // require "ArrowDecl.chpl";
  // public use ArrowDecl;
  //require "../ArrowHeaders.chpl";
  //use ArrowHeaders;
  public use SysCTypes, CPtr;
  /* -----------------GLib----------------------- */
  extern const EXIT_FAILURE: int ;
  extern const EXIT_SUCCESS: int;
  require "null.h", "null.c";
  extern proc isNull(ptr: c_void_ptr): c_int;
  extern proc getNULL(): c_void_ptr;
  type GErrorPtr = c_ptr(GError);
  // Generated with c2chapel version 0.1.0
  // Header given to c2chapel:
  require "/usr/local/include/parquet-glib/parquet-glib.h";
  require "-larrow-glib", "-lparquet-glib", "-lglib-2.0", "-lgobject-2.0";

  // All records
  extern "struct _GTimeVal" record _GTimeVal {    var tv_sec : glong;
    var tv_usec : glong;
  }

  extern "struct _GArray" record _GArray {    var data : c_ptr(gchar);
    var len : guint;
  }

  extern "struct _GByteArray" record _GByteArray {    var data : c_ptr(guint8);
    var len : guint;
  }

  extern "struct _GPtrArray" record _GPtrArray {    var pdata : c_ptr(gpointer);
    var len : guint;
  }

  extern "struct _GError" record _GError {    extern "domain" var dom: GQuark;
    var code: gint;
    var message: c_ptr(gchar);
  }

  extern "struct _GDebugKey" record _GDebugKey {    var key : c_ptr(gchar);
    var value : guint;
  }

  extern "struct _GRWLock" record _GRWLock {    var p : gpointer;
    var i : c_ptr(guint);
  }

  extern "struct _GCond" record _GCond {    var p : gpointer;
    var i : c_ptr(guint);
  }

  extern "struct _GRecMutex" record _GRecMutex {    var p : gpointer;
    var i : c_ptr(guint);
  }

  extern "struct _GPrivate" record _GPrivate {    var p : gpointer;
    var notify : GDestroyNotify;
    var future : c_ptr(gpointer);
  }

  extern "struct _GOnce" record _GOnce {    var status : GOnceStatus;
    var retval : gpointer;
  }

  extern "struct _GDate" record _GDate {    var julian_days : guint;
    var julian : guint;
    var dmy : guint;
    var day : guint;
    var month : guint;
    var year : guint;
  }

  extern "struct _GMemVTable" record _GMemVTable {    var malloc : c_ptr(c_fn_ptr);
    var realloc : c_ptr(c_fn_ptr);
    var free : c_ptr(c_fn_ptr);
    var calloc : c_ptr(c_fn_ptr);
    var try_malloc : c_ptr(c_fn_ptr);
    var try_realloc : c_ptr(c_fn_ptr);
  }

  extern "struct _GNode" record _GNode {    var data : gpointer;
    var next : c_ptr(_GNode);
    var prev : c_ptr(_GNode);
    var parent : c_ptr(_GNode);
    var children : c_ptr(_GNode);
  }

  extern "struct _GHashTableIter" record _GHashTableIter {    var dummy1 : gpointer;
    var dummy2 : gpointer;
    var dummy3 : gpointer;
    var dummy4 : c_int;
    var dummy5 : gboolean;
    var dummy6 : gpointer;
  }

  extern "struct _GHookList" record _GHookList {    var seq_id : gulong;
    var hook_size : guint;
    var is_setup : guint;
    var hooks : c_ptr(GHook);
    var dummy3 : gpointer;
    var finalize_hook : GHookFinalizeFunc;
    var dummy : c_ptr(gpointer);
  }

  extern "struct _GHook" record _GHook {    var data : gpointer;
    var next : c_ptr(_GHook);
    var prev : c_ptr(_GHook);
    var ref_count : guint;
    var hook_id : gulong;
    var flags : guint;
    var func : gpointer;
    var destroy : GDestroyNotify;
  }

  extern "struct _GPollFD" record _GPollFD {    var fd : gint;
    var events : gushort;
    var revents : gushort;
  }

  extern "struct _GSList" record _GSList {    var data : gpointer;
    var next : c_ptr(GSList);
  }

  extern "struct _GSource" record _GSource {    var callback_data : gpointer;
    var callback_funcs : c_ptr(GSourceCallbackFuncs);
    var source_funcs : c_ptr(GSourceFuncs);
    var ref_count : guint;
    var context : c_ptr(GMainContext);
    var priority : gint;
    var flags : guint;
    var source_id : guint;
    var poll_fds : c_ptr(GSList);
    var prev : c_ptr(_GSource);
    var next : c_ptr(_GSource);
    var name : c_string;
    var priv : c_ptr(GSourcePrivate);
  }

  extern "struct _GSourceFuncs" record _GSourceFuncs {    var prepare : c_ptr(c_fn_ptr);
    var check : c_ptr(c_fn_ptr);
    var dispatch : c_ptr(c_fn_ptr);
    var finalize : c_ptr(c_fn_ptr);
    var closure_callback : GSourceFunc;
    var closure_marshal : GSourceDummyMarshal;
  }

  extern "struct _GString" record _GString {    var str : c_ptr(gchar);
    var len : gsize;
    var allocated_len : gsize;
  }

  extern "struct _GIOChannel" record _GIOChannel {    var ref_count : gint;
    var funcs : c_ptr(GIOFuncs);
    var encoding : c_ptr(gchar);
    var read_cd : GIConv;
    var write_cd : GIConv;
    var line_term : c_ptr(gchar);
    var line_term_len : guint;
    var buf_size : gsize;
    var read_buf : c_ptr(GString);
    var encoded_read_buf : c_ptr(GString);
    var write_buf : c_ptr(GString);
    var partial_write_buf : c_ptr(gchar);
    var use_buffer : guint;
    var do_encode : guint;
    var close_on_unref : guint;
    var is_readable : guint;
    var is_writeable : guint;
    var is_seekable : guint;
    var reserved1 : gpointer;
    var reserved2 : gpointer;
  }

  extern "struct _GIOFuncs" record _GIOFuncs {    var io_read : c_ptr(c_fn_ptr);
    var io_write : c_ptr(c_fn_ptr);
    var io_seek : c_ptr(c_fn_ptr);
    var io_close : c_ptr(c_fn_ptr);
    var io_create_watch : c_ptr(c_fn_ptr);
    var io_free : c_ptr(c_fn_ptr);
    var io_set_flags : c_ptr(c_fn_ptr);
    var io_get_flags : c_ptr(c_fn_ptr);
  }

  extern "struct _GMarkupParser" record _GMarkupParser {    var start_element : c_ptr(c_fn_ptr);
    var end_element : c_ptr(c_fn_ptr);
    var text : c_ptr(c_fn_ptr);
    var passthrough : c_ptr(c_fn_ptr);
    var error : c_ptr(c_fn_ptr);
  }

  extern "struct _GVariantIter" record _GVariantIter {    var x : c_ptr(gsize);
  }

  extern "struct _GLogField" record _GLogField {    var key : c_ptr(gchar);
    var value : gconstpointer;
    var length : gssize;
  }

  extern "struct _GOptionEntry" record _GOptionEntry {    var long_name : c_ptr(gchar);
    var short_name : gchar;
    var flags : gint;
    var arg : GOptionArg;
    var arg_data : gpointer;
    var description : c_ptr(gchar);
    var arg_description : c_ptr(gchar);
  }

  extern "struct _GQueue" record _GQueue {    var head : c_ptr(GList);
    var tail : c_ptr(GList);
    var length : guint;
  }

  extern "struct _GScannerConfig" record _GScannerConfig {    var cset_skip_characters : c_ptr(gchar);
    var cset_identifier_first : c_ptr(gchar);
    var cset_identifier_nth : c_ptr(gchar);
    var cpair_comment_single : c_ptr(gchar);
    var case_sensitive : guint;
    var skip_comment_multi : guint;
    var skip_comment_single : guint;
    var scan_comment_multi : guint;
    var scan_identifier : guint;
    var scan_identifier_1char : guint;
    var scan_identifier_NULL : guint;
    var scan_symbols : guint;
    var scan_binary : guint;
    var scan_octal : guint;
    var scan_float : guint;
    var scan_hex : guint;
    var scan_hex_dollar : guint;
    var scan_string_sq : guint;
    var scan_string_dq : guint;
    var numbers_2_int : guint;
    var int_2_float : guint;
    var identifier_2_string : guint;
    var char_2_token : guint;
    var symbol_2_token : guint;
    var scope_0_fallback : guint;
    var store_int64 : guint;
    var padding_dummy : guint;
  }

  extern "struct _GThreadPool" record _GThreadPool {    var func : GFunc;
    var user_data : gpointer;
    var exclusive : gboolean;
  }

  extern "struct _GTrashStack" record _GTrashStack {    var next : c_ptr(_GTrashStack);
  }

  extern "struct _GUriParamsIter" record _GUriParamsIter {    var dummy0 : gint;
    var dummy1 : gpointer;
    var dummy2 : gpointer;
    var dummy3 : c_ptr(guint8);
  }

  extern "struct _GCompletion" record _GCompletion {    var items : c_ptr(GList);
    var func : GCompletionFunc;
    var prefix : c_ptr(gchar);
    var cache : c_ptr(GList);
    var strncmp_func : GCompletionStrncmpFunc;
  }

  extern "struct _GTuples" record _GTuples {    var len : guint;
  }

  extern "struct _GThread" record _GThread {    var func : GThreadFunc;
    var data : gpointer;
    var joinable : gboolean;
    var priority : GThreadPriority;
  }

  extern "struct _GThreadFunctions" record _GThreadFunctions {    var mutex_new : c_ptr(c_fn_ptr);
    var mutex_lock : c_ptr(c_fn_ptr);
    var mutex_trylock : c_ptr(c_fn_ptr);
    var mutex_unlock : c_ptr(c_fn_ptr);
    var mutex_free : c_ptr(c_fn_ptr);
    var cond_new : c_ptr(c_fn_ptr);
    var cond_signal : c_ptr(c_fn_ptr);
    var cond_broadcast : c_ptr(c_fn_ptr);
    var cond_wait : c_ptr(c_fn_ptr);
    var cond_timed_wait : c_ptr(c_fn_ptr);
    var cond_free : c_ptr(c_fn_ptr);
    var private_new : c_ptr(c_fn_ptr);
    var private_get : c_ptr(c_fn_ptr);
    var private_set : c_ptr(c_fn_ptr);
    var thread_create : c_ptr(c_fn_ptr);
    var thread_yield : c_ptr(c_fn_ptr);
    var thread_join : c_ptr(c_fn_ptr);
    var thread_exit : c_ptr(c_fn_ptr);
    var thread_set_priority : c_ptr(c_fn_ptr);
    var thread_self : c_ptr(c_fn_ptr);
    var thread_equal : c_ptr(c_fn_ptr);
  }

  extern "struct _GStaticRecMutex" record _GStaticRecMutex {    var mutex : GStaticMutex;
    var depth : guint;
  }

  extern "struct _GStaticRWLock" record _GStaticRWLock {    var mutex : GStaticMutex;
    var read_cond : c_ptr(GCond);
    var write_cond : c_ptr(GCond);
    var read_counter : guint;
    var have_writer : gboolean;
    var want_to_read : guint;
    var want_to_write : guint;
  }

  extern "struct _GTypeClass" record _GTypeClass {    var g_type : GType;
  }

  extern "struct _GTypeInstance" record _GTypeInstance {    var g_class : c_ptr(GTypeClass);
  }

  extern "struct _GTypeInterface" record _GTypeInterface {    var g_type : GType;
    var g_instance_type : GType;
  }

  extern "struct _GTypeInfo" record _GTypeInfo {    var class_size : guint16;
    var base_init : GBaseInitFunc;
    var base_finalize : GBaseFinalizeFunc;
    var class_init : GClassInitFunc;
    var class_finalize : GClassFinalizeFunc;
    var class_data : gconstpointer;
    var instance_size : guint16;
    var n_preallocs : guint16;
    var instance_init : GInstanceInitFunc;
    var value_table : c_ptr(GTypeValueTable);
  }

  extern "struct _GTypeFundamentalInfo" record _GTypeFundamentalInfo {    var type_flags : GTypeFundamentalFlags;
  }

  extern "struct _GInterfaceInfo" record _GInterfaceInfo {    var interface_init : GInterfaceInitFunc;
    var interface_finalize : GInterfaceFinalizeFunc;
    var interface_data : gpointer;
  }

  extern "struct _GTypeValueTable" record _GTypeValueTable {    var value_init : c_ptr(c_fn_ptr);
    var value_free : c_ptr(c_fn_ptr);
    var value_copy : c_ptr(c_fn_ptr);
    var value_peek_pointer : c_ptr(c_fn_ptr);
    var collect_format : c_ptr(gchar);
    var collect_value : c_ptr(c_fn_ptr);
    var lcopy_format : c_ptr(gchar);
    var lcopy_value : c_ptr(c_fn_ptr);
  }

  extern "struct _GValue" record _GValue {    var g_type : GType;
  }

  extern "struct _GParamSpec" record _GParamSpec {    var g_type_instance : GTypeInstance;
    var name : c_ptr(gchar);
    var flags : GParamFlags;
    var value_type : GType;
    var owner_type : GType;
    var _nick : c_ptr(gchar);
    var _blurb : c_ptr(gchar);
    var qdata : c_ptr(GData);
    var ref_count : guint;
    var param_id : guint;
  }

  extern "struct _GParamSpecClass" record _GParamSpecClass {    var g_type_class : GTypeClass;
    var value_type : GType;
    var finalize : c_ptr(c_fn_ptr);
    var value_set_default : c_ptr(c_fn_ptr);
    var value_validate : c_ptr(c_fn_ptr);
    var values_cmp : c_ptr(c_fn_ptr);
    var dummy : c_ptr(gpointer);
  }

  extern "struct _GParameter" record _GParameter {    var name : c_ptr(gchar);
    var value : GValue;
  }

  extern "struct _GParamSpecTypeInfo" record _GParamSpecTypeInfo {    var instance_size : guint16;
    var n_preallocs : guint16;
    var instance_init : c_ptr(c_fn_ptr);
    var value_type : GType;
    var finalize : c_ptr(c_fn_ptr);
    var value_set_default : c_ptr(c_fn_ptr);
    var value_validate : c_ptr(c_fn_ptr);
    var values_cmp : c_ptr(c_fn_ptr);
  }

  extern "struct _GClosureNotifyData" record _GClosureNotifyData {    var data : gpointer;
    var notify : GClosureNotify;
  }

  extern "struct _GClosure" record _GClosure {    var ref_count : guint;
    var meta_marshal_nouse : guint;
    var n_guards : guint;
    var n_fnotifiers : guint;
    var n_inotifiers : guint;
    var in_inotify : guint;
    var floating : guint;
    var derivative_flag : guint;
    var in_marshal : guint;
    var is_invalid : guint;
    var marshal : c_ptr(c_fn_ptr);
    var data : gpointer;
    var notifiers : c_ptr(GClosureNotifyData);
  }

  extern "struct _GCClosure" record _GCClosure {    var closure : GClosure;
    var callback : gpointer;
  }

  extern "struct _GSignalInvocationHint" record _GSignalInvocationHint {    var signal_id : guint;
    var detail : GQuark;
    var run_type : GSignalFlags;
  }

  extern "struct _GSignalQuery" record _GSignalQuery {    var signal_id : guint;
    var signal_name : c_ptr(gchar);
    var itype : GType;
    var signal_flags : GSignalFlags;
    var return_type : GType;
    var n_params : guint;
    var param_types : c_ptr(GType);
  }

  extern "struct _GObject" record _GObject {    var g_type_instance : GTypeInstance;
    var ref_count : guint;
    var qdata : c_ptr(GData);
  }

  extern "struct _GObjectClass" record _GObjectClass {    var g_type_class : GTypeClass;
    var construct_properties : c_ptr(GSList);
    var constructor : c_ptr(c_fn_ptr);
    var set_property : c_ptr(c_fn_ptr);
    var get_property : c_ptr(c_fn_ptr);
    var dispose : c_ptr(c_fn_ptr);
    var finalize : c_ptr(c_fn_ptr);
    var dispatch_properties_changed : c_ptr(c_fn_ptr);
    var notify : c_ptr(c_fn_ptr);
    var constructed : c_ptr(c_fn_ptr);
    var flags : gsize;
    var pdummy : c_ptr(gpointer);
  }

  extern "struct _GArrowDecimal128Class" record _GArrowDecimal128Class {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowDecimal256" record _GArrowDecimal256 {    var parent_instance : GObject;
  }

  extern "struct _GArrowDecimal256Class" record _GArrowDecimal256Class {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowDataType" record _GArrowDataType {    var parent_instance : GObject;
  }

  extern "struct _GArrowDataTypeClass" record _GArrowDataTypeClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowFixedWidthDataType" record _GArrowFixedWidthDataType {    var parent_instance : GArrowDataType;
  }

  extern "struct _GArrowFixedWidthDataTypeClass" record _GArrowFixedWidthDataTypeClass {    var parent_class : GArrowDataTypeClass;
  }

  extern "struct _GArrowNullDataType" record _GArrowNullDataType {    var parent_instance : GArrowDataType;
  }

  extern "struct _GArrowNullDataTypeClass" record _GArrowNullDataTypeClass {    var parent_class : GArrowDataTypeClass;
  }

  extern "struct _GArrowBooleanDataType" record _GArrowBooleanDataType {    var parent_instance : GArrowFixedWidthDataType;
  }

  extern "struct _GArrowBooleanDataTypeClass" record _GArrowBooleanDataTypeClass {    var parent_class : GArrowFixedWidthDataTypeClass;
  }

  extern "struct _GArrowNumericDataType" record _GArrowNumericDataType {    var parent_instance : GArrowFixedWidthDataType;
  }

  extern "struct _GArrowNumericDataTypeClass" record _GArrowNumericDataTypeClass {    var parent_class : GArrowFixedWidthDataTypeClass;
  }

  extern "struct _GArrowIntegerDataType" record _GArrowIntegerDataType {    var parent_instance : GArrowNumericDataType;
  }

  extern "struct _GArrowIntegerDataTypeClass" record _GArrowIntegerDataTypeClass {    var parent_class : GArrowNumericDataTypeClass;
  }

  extern "struct _GArrowInt8DataType" record _GArrowInt8DataType {    var parent_instance : GArrowIntegerDataType;
  }

  extern "struct _GArrowInt8DataTypeClass" record _GArrowInt8DataTypeClass {    var parent_class : GArrowIntegerDataTypeClass;
  }

  extern "struct _GArrowUInt8DataType" record _GArrowUInt8DataType {    var parent_instance : GArrowIntegerDataType;
  }

  extern "struct _GArrowUInt8DataTypeClass" record _GArrowUInt8DataTypeClass {    var parent_class : GArrowIntegerDataTypeClass;
  }

  extern "struct _GArrowInt16DataType" record _GArrowInt16DataType {    var parent_instance : GArrowIntegerDataType;
  }

  extern "struct _GArrowInt16DataTypeClass" record _GArrowInt16DataTypeClass {    var parent_class : GArrowIntegerDataTypeClass;
  }

  extern "struct _GArrowUInt16DataType" record _GArrowUInt16DataType {    var parent_instance : GArrowIntegerDataType;
  }

  extern "struct _GArrowUInt16DataTypeClass" record _GArrowUInt16DataTypeClass {    var parent_class : GArrowIntegerDataTypeClass;
  }

  extern "struct _GArrowInt32DataType" record _GArrowInt32DataType {    var parent_instance : GArrowIntegerDataType;
  }

  extern "struct _GArrowInt32DataTypeClass" record _GArrowInt32DataTypeClass {    var parent_class : GArrowIntegerDataTypeClass;
  }

  extern "struct _GArrowUInt32DataType" record _GArrowUInt32DataType {    var parent_instance : GArrowIntegerDataType;
  }

  extern "struct _GArrowUInt32DataTypeClass" record _GArrowUInt32DataTypeClass {    var parent_class : GArrowIntegerDataTypeClass;
  }

  extern "struct _GArrowInt64DataType" record _GArrowInt64DataType {    var parent_instance : GArrowIntegerDataType;
  }

  extern "struct _GArrowInt64DataTypeClass" record _GArrowInt64DataTypeClass {    var parent_class : GArrowIntegerDataTypeClass;
  }

  extern "struct _GArrowUInt64DataType" record _GArrowUInt64DataType {    var parent_instance : GArrowIntegerDataType;
  }

  extern "struct _GArrowUInt64DataTypeClass" record _GArrowUInt64DataTypeClass {    var parent_class : GArrowIntegerDataTypeClass;
  }

  extern "struct _GArrowFloatingPointDataType" record _GArrowFloatingPointDataType {    var parent_instance : GArrowNumericDataType;
  }

  extern "struct _GArrowFloatingPointDataTypeClass" record _GArrowFloatingPointDataTypeClass {    var parent_class : GArrowNumericDataTypeClass;
  }

  extern "struct _GArrowFloatDataType" record _GArrowFloatDataType {    var parent_instance : GArrowFloatingPointDataType;
  }

  extern "struct _GArrowFloatDataTypeClass" record _GArrowFloatDataTypeClass {    var parent_class : GArrowFloatingPointDataTypeClass;
  }

  extern "struct _GArrowDoubleDataType" record _GArrowDoubleDataType {    var parent_instance : GArrowFloatingPointDataType;
  }

  extern "struct _GArrowDoubleDataTypeClass" record _GArrowDoubleDataTypeClass {    var parent_class : GArrowFloatingPointDataTypeClass;
  }

  extern "struct _GArrowBinaryDataType" record _GArrowBinaryDataType {    var parent_instance : GArrowDataType;
  }

  extern "struct _GArrowBinaryDataTypeClass" record _GArrowBinaryDataTypeClass {    var parent_class : GArrowDataTypeClass;
  }

  extern "struct _GArrowFixedSizeBinaryDataType" record _GArrowFixedSizeBinaryDataType {    var parent_instance : GArrowDataType;
  }

  extern "struct _GArrowFixedSizeBinaryDataTypeClass" record _GArrowFixedSizeBinaryDataTypeClass {    var parent_class : GArrowFixedWidthDataTypeClass;
  }

  extern "struct _GArrowLargeBinaryDataType" record _GArrowLargeBinaryDataType {    var parent_instance : GArrowDataType;
  }

  extern "struct _GArrowLargeBinaryDataTypeClass" record _GArrowLargeBinaryDataTypeClass {    var parent_class : GArrowDataTypeClass;
  }

  extern "struct _GArrowStringDataType" record _GArrowStringDataType {    var parent_instance : GArrowBinaryDataType;
  }

  extern "struct _GArrowStringDataTypeClass" record _GArrowStringDataTypeClass {    var parent_class : GArrowBinaryDataTypeClass;
  }

  extern "struct _GArrowLargeStringDataType" record _GArrowLargeStringDataType {    var parent_instance : GArrowLargeBinaryDataType;
  }

  extern "struct _GArrowLargeStringDataTypeClass" record _GArrowLargeStringDataTypeClass {    var parent_class : GArrowLargeBinaryDataTypeClass;
  }

  extern "struct _GArrowDate32DataType" record _GArrowDate32DataType {    var parent_instance : GArrowDataType;
  }

  extern "struct _GArrowDate32DataTypeClass" record _GArrowDate32DataTypeClass {    var parent_class : GArrowDataTypeClass;
  }

  extern "struct _GArrowDate64DataType" record _GArrowDate64DataType {    var parent_instance : GArrowDataType;
  }

  extern "struct _GArrowDate64DataTypeClass" record _GArrowDate64DataTypeClass {    var parent_class : GArrowDataTypeClass;
  }

  extern "struct _GArrowTimestampDataType" record _GArrowTimestampDataType {    var parent_instance : GArrowDataType;
  }

  extern "struct _GArrowTimestampDataTypeClass" record _GArrowTimestampDataTypeClass {    var parent_class : GArrowDataTypeClass;
  }

  extern "struct _GArrowTimeDataType" record _GArrowTimeDataType {    var parent_instance : GArrowDataType;
  }

  extern "struct _GArrowTimeDataTypeClass" record _GArrowTimeDataTypeClass {    var parent_class : GArrowDataTypeClass;
  }

  extern "struct _GArrowTime32DataType" record _GArrowTime32DataType {    var parent_instance : GArrowTimeDataType;
  }

  extern "struct _GArrowTime32DataTypeClass" record _GArrowTime32DataTypeClass {    var parent_class : GArrowTimeDataTypeClass;
  }

  extern "struct _GArrowTime64DataType" record _GArrowTime64DataType {    var parent_instance : GArrowTimeDataType;
  }

  extern "struct _GArrowTime64DataTypeClass" record _GArrowTime64DataTypeClass {    var parent_class : GArrowTimeDataTypeClass;
  }

  extern "struct _GArrowDecimalDataType" record _GArrowDecimalDataType {    var parent_instance : GArrowFixedSizeBinaryDataType;
  }

  extern "struct _GArrowDecimalDataTypeClass" record _GArrowDecimalDataTypeClass {    var parent_class : GArrowFixedSizeBinaryDataTypeClass;
  }

  extern "struct _GArrowDecimal128DataType" record _GArrowDecimal128DataType {    var parent_instance : GArrowDecimalDataType;
  }

  extern "struct _GArrowDecimal128DataTypeClass" record _GArrowDecimal128DataTypeClass {    var parent_class : GArrowDecimalDataTypeClass;
  }

  extern "struct _GArrowDecimal256DataType" record _GArrowDecimal256DataType {    var parent_instance : GArrowDecimalDataType;
  }

  extern "struct _GArrowDecimal256DataTypeClass" record _GArrowDecimal256DataTypeClass {    var parent_class : GArrowDecimalDataTypeClass;
  }

  extern "struct _GArrowExtensionDataType" record _GArrowExtensionDataType {    var parent_instance : GArrowDataType;
  }

  extern "struct _GArrowExtensionDataTypeClass" record _GArrowExtensionDataTypeClass {    var parent_class : GArrowDataTypeClass;
    var get_extension_name : c_ptr(c_fn_ptr);
    var equal : c_ptr(c_fn_ptr);
    var deserialize : c_ptr(c_fn_ptr);
    var serialize : c_ptr(c_fn_ptr);
    var get_array_gtype : c_ptr(c_fn_ptr);
  }

  extern "struct _GArrowExtensionDataTypeRegistry" record _GArrowExtensionDataTypeRegistry {    var parent_instance : GObject;
  }

  extern "struct _GArrowExtensionDataTypeRegistryClass" record _GArrowExtensionDataTypeRegistryClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowBuffer" record _GArrowBuffer {    var parent_instance : GObject;
  }

  extern "struct _GArrowBufferClass" record _GArrowBufferClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowMutableBuffer" record _GArrowMutableBuffer {    var parent_instance : GArrowBuffer;
  }

  extern "struct _GArrowMutableBufferClass" record _GArrowMutableBufferClass {    var parent_class : GArrowBufferClass;
  }

  extern "struct _GArrowResizableBuffer" record _GArrowResizableBuffer {    var parent_instance : GArrowMutableBuffer;
  }

  extern "struct _GArrowResizableBufferClass" record _GArrowResizableBufferClass {    var parent_class : GArrowMutableBufferClass;
  }

  extern "struct _GArrowEqualOptions" record _GArrowEqualOptions {    var parent_instance : GObject;
  }

  extern "struct _GArrowEqualOptionsClass" record _GArrowEqualOptionsClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowArray" record _GArrowArray {    var parent_instance : GObject;
  }

  extern "struct _GArrowArrayClass" record _GArrowArrayClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowNullArray" record _GArrowNullArray {    var parent_instance : GArrowArray;
  }

  extern "struct _GArrowNullArrayClass" record _GArrowNullArrayClass {    var parent_class : GArrowArrayClass;
  }

  extern "struct _GArrowPrimitiveArray" record _GArrowPrimitiveArray {    var parent_instance : GArrowArray;
  }

  extern "struct _GArrowPrimitiveArrayClass" record _GArrowPrimitiveArrayClass {    var parent_class : GArrowArrayClass;
  }

  extern "struct _GArrowBooleanArray" record _GArrowBooleanArray {    var parent_instance : GArrowPrimitiveArray;
  }

  extern "struct _GArrowBooleanArrayClass" record _GArrowBooleanArrayClass {    var parent_class : GArrowPrimitiveArrayClass;
  }

  extern "struct _GArrowNumericArray" record _GArrowNumericArray {    var parent_instance : GArrowPrimitiveArray;
  }

  extern "struct _GArrowNumericArrayClass" record _GArrowNumericArrayClass {    var parent_class : GArrowPrimitiveArrayClass;
  }

  extern "struct _GArrowInt8Array" record _GArrowInt8Array {    var parent_instance : GArrowNumericArray;
  }

  extern "struct _GArrowInt8ArrayClass" record _GArrowInt8ArrayClass {    var parent_class : GArrowNumericArrayClass;
  }

  extern "struct _GArrowUInt8Array" record _GArrowUInt8Array {    var parent_instance : GArrowNumericArray;
  }

  extern "struct _GArrowUInt8ArrayClass" record _GArrowUInt8ArrayClass {    var parent_class : GArrowNumericArrayClass;
  }

  extern "struct _GArrowInt16Array" record _GArrowInt16Array {    var parent_instance : GArrowNumericArray;
  }

  extern "struct _GArrowInt16ArrayClass" record _GArrowInt16ArrayClass {    var parent_class : GArrowNumericArrayClass;
  }

  extern "struct _GArrowUInt16Array" record _GArrowUInt16Array {    var parent_instance : GArrowNumericArray;
  }

  extern "struct _GArrowUInt16ArrayClass" record _GArrowUInt16ArrayClass {    var parent_class : GArrowNumericArrayClass;
  }

  extern "struct _GArrowInt32Array" record _GArrowInt32Array {    var parent_instance : GArrowNumericArray;
  }

  extern "struct _GArrowInt32ArrayClass" record _GArrowInt32ArrayClass {    var parent_class : GArrowNumericArrayClass;
  }

  extern "struct _GArrowUInt32Array" record _GArrowUInt32Array {    var parent_instance : GArrowNumericArray;
  }

  extern "struct _GArrowUInt32ArrayClass" record _GArrowUInt32ArrayClass {    var parent_class : GArrowNumericArrayClass;
  }

  extern "struct _GArrowInt64Array" record _GArrowInt64Array {    var parent_instance : GArrowNumericArray;
  }

  extern "struct _GArrowInt64ArrayClass" record _GArrowInt64ArrayClass {    var parent_class : GArrowNumericArrayClass;
  }

  extern "struct _GArrowUInt64Array" record _GArrowUInt64Array {    var parent_instance : GArrowNumericArray;
  }

  extern "struct _GArrowUInt64ArrayClass" record _GArrowUInt64ArrayClass {    var parent_class : GArrowNumericArrayClass;
  }

  extern "struct _GArrowFloatArray" record _GArrowFloatArray {    var parent_instance : GArrowNumericArray;
  }

  extern "struct _GArrowFloatArrayClass" record _GArrowFloatArrayClass {    var parent_class : GArrowNumericArrayClass;
  }

  extern "struct _GArrowDoubleArray" record _GArrowDoubleArray {    var parent_instance : GArrowNumericArray;
  }

  extern "struct _GArrowDoubleArrayClass" record _GArrowDoubleArrayClass {    var parent_class : GArrowNumericArrayClass;
  }

  extern "struct _GArrowBinaryArray" record _GArrowBinaryArray {    var parent_instance : GArrowArray;
  }

  extern "struct _GArrowBinaryArrayClass" record _GArrowBinaryArrayClass {    var parent_class : GArrowArrayClass;
  }

  extern "struct _GArrowLargeBinaryArray" record _GArrowLargeBinaryArray {    var parent_instance : GArrowArray;
  }

  extern "struct _GArrowLargeBinaryArrayClass" record _GArrowLargeBinaryArrayClass {    var parent_class : GArrowArrayClass;
  }

  extern "struct _GArrowStringArray" record _GArrowStringArray {    var parent_instance : GArrowBinaryArray;
  }

  extern "struct _GArrowStringArrayClass" record _GArrowStringArrayClass {    var parent_class : GArrowBinaryArrayClass;
  }

  extern "struct _GArrowLargeStringArray" record _GArrowLargeStringArray {    var parent_instance : GArrowLargeBinaryArray;
  }

  extern "struct _GArrowLargeStringArrayClass" record _GArrowLargeStringArrayClass {    var parent_class : GArrowLargeBinaryArrayClass;
  }

  extern "struct _GArrowDate32Array" record _GArrowDate32Array {    var parent_instance : GArrowNumericArray;
  }

  extern "struct _GArrowDate32ArrayClass" record _GArrowDate32ArrayClass {    var parent_class : GArrowNumericArrayClass;
  }

  extern "struct _GArrowDate64Array" record _GArrowDate64Array {    var parent_instance : GArrowNumericArray;
  }

  extern "struct _GArrowDate64ArrayClass" record _GArrowDate64ArrayClass {    var parent_class : GArrowNumericArrayClass;
  }

  extern "struct _GArrowTimestampArray" record _GArrowTimestampArray {    var parent_instance : GArrowNumericArray;
  }

  extern "struct _GArrowTimestampArrayClass" record _GArrowTimestampArrayClass {    var parent_class : GArrowNumericArrayClass;
  }

  extern "struct _GArrowTime32Array" record _GArrowTime32Array {    var parent_instance : GArrowNumericArray;
  }

  extern "struct _GArrowTime32ArrayClass" record _GArrowTime32ArrayClass {    var parent_class : GArrowNumericArrayClass;
  }

  extern "struct _GArrowTime64Array" record _GArrowTime64Array {    var parent_instance : GArrowNumericArray;
  }

  extern "struct _GArrowTime64ArrayClass" record _GArrowTime64ArrayClass {    var parent_class : GArrowNumericArrayClass;
  }

  extern "struct _GArrowFixedSizeBinaryArray" record _GArrowFixedSizeBinaryArray {    var parent_instance : GArrowPrimitiveArray;
  }

  extern "struct _GArrowFixedSizeBinaryArrayClass" record _GArrowFixedSizeBinaryArrayClass {    var parent_class : GArrowPrimitiveArrayClass;
  }

  extern "struct _GArrowDecimal128Array" record _GArrowDecimal128Array {    var parent_instance : GArrowFixedSizeBinaryArray;
  }

  extern "struct _GArrowDecimal128ArrayClass" record _GArrowDecimal128ArrayClass {    var parent_class : GArrowFixedSizeBinaryArrayClass;
  }

  extern "struct _GArrowDecimal256Array" record _GArrowDecimal256Array {    var parent_instance : GArrowFixedSizeBinaryArray;
  }

  extern "struct _GArrowDecimal256ArrayClass" record _GArrowDecimal256ArrayClass {    var parent_class : GArrowFixedSizeBinaryArrayClass;
  }

  extern "struct _GArrowExtensionArray" record _GArrowExtensionArray {    var parent_instance : GArrowArray;
  }

  extern "struct _GArrowExtensionArrayClass" record _GArrowExtensionArrayClass {    var parent_class : GArrowArrayClass;
  }

  extern "struct _GArrowField" record _GArrowField {    var parent_instance : GObject;
  }

  extern "struct _GArrowFieldClass" record _GArrowFieldClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowListDataType" record _GArrowListDataType {    var parent_instance : GArrowDataType;
  }

  extern "struct _GArrowListDataTypeClass" record _GArrowListDataTypeClass {    var parent_class : GArrowDataTypeClass;
  }

  extern "struct _GArrowLargeListDataType" record _GArrowLargeListDataType {    var parent_instance : GArrowDataType;
  }

  extern "struct _GArrowLargeListDataTypeClass" record _GArrowLargeListDataTypeClass {    var parent_class : GArrowDataTypeClass;
  }

  extern "struct _GArrowStructDataType" record _GArrowStructDataType {    var parent_instance : GArrowDataType;
  }

  extern "struct _GArrowStructDataTypeClass" record _GArrowStructDataTypeClass {    var parent_class : GArrowDataTypeClass;
  }

  extern "struct _GArrowMapDataType" record _GArrowMapDataType {    var parent_instance : GArrowListDataType;
  }

  extern "struct _GArrowMapDataTypeClass" record _GArrowMapDataTypeClass {    var parent_class : GArrowListDataTypeClass;
  }

  extern "struct _GArrowUnionDataType" record _GArrowUnionDataType {    var parent_instance : GArrowDataType;
  }

  extern "struct _GArrowUnionDataTypeClass" record _GArrowUnionDataTypeClass {    var parent_class : GArrowDataTypeClass;
  }

  extern "struct _GArrowSparseUnionDataType" record _GArrowSparseUnionDataType {    var parent_instance : GArrowUnionDataType;
  }

  extern "struct _GArrowSparseUnionDataTypeClass" record _GArrowSparseUnionDataTypeClass {    var parent_class : GArrowUnionDataTypeClass;
  }

  extern "struct _GArrowDenseUnionDataType" record _GArrowDenseUnionDataType {    var parent_instance : GArrowUnionDataType;
  }

  extern "struct _GArrowDenseUnionDataTypeClass" record _GArrowDenseUnionDataTypeClass {    var parent_class : GArrowUnionDataTypeClass;
  }

  extern "struct _GArrowDictionaryDataType" record _GArrowDictionaryDataType {    var parent_instance : GArrowFixedWidthDataType;
  }

  extern "struct _GArrowDictionaryDataTypeClass" record _GArrowDictionaryDataTypeClass {    var parent_class : GArrowFixedWidthDataTypeClass;
  }

  extern "struct _GArrowListArray" record _GArrowListArray {    var parent_instance : GArrowArray;
  }

  extern "struct _GArrowListArrayClass" record _GArrowListArrayClass {    var parent_class : GArrowArrayClass;
  }

  extern "struct _GArrowLargeListArray" record _GArrowLargeListArray {    var parent_instance : GArrowArray;
  }

  extern "struct _GArrowLargeListArrayClass" record _GArrowLargeListArrayClass {    var parent_class : GArrowArrayClass;
  }

  extern "struct _GArrowStructArray" record _GArrowStructArray {    var parent_instance : GArrowArray;
  }

  extern "struct _GArrowStructArrayClass" record _GArrowStructArrayClass {    var parent_class : GArrowArrayClass;
  }

  extern "struct _GArrowMapArray" record _GArrowMapArray {    var parent_instance : GArrowListArray;
  }

  extern "struct _GArrowMapArrayClass" record _GArrowMapArrayClass {    var parent_class : GArrowListArrayClass;
  }

  extern "struct _GArrowUnionArray" record _GArrowUnionArray {    var parent_instance : GArrowArray;
  }

  extern "struct _GArrowUnionArrayClass" record _GArrowUnionArrayClass {    var parent_class : GArrowArrayClass;
  }

  extern "struct _GArrowSparseUnionArray" record _GArrowSparseUnionArray {    var parent_instance : GArrowUnionArray;
  }

  extern "struct _GArrowSparseUnionArrayClass" record _GArrowSparseUnionArrayClass {    var parent_class : GArrowUnionArrayClass;
  }

  extern "struct _GArrowDenseUnionArray" record _GArrowDenseUnionArray {    var parent_instance : GArrowUnionArray;
  }

  extern "struct _GArrowDenseUnionArrayClass" record _GArrowDenseUnionArrayClass {    var parent_class : GArrowUnionArrayClass;
  }

  extern "struct _GArrowDictionaryArray" record _GArrowDictionaryArray {    var parent_instance : GArrowArray;
  }

  extern "struct _GArrowDictionaryArrayClass" record _GArrowDictionaryArrayClass {    var parent_class : GArrowArrayClass;
  }

  extern "struct _GArrowArrayBuilder" record _GArrowArrayBuilder {    var parent_instance : GObject;
  }

  extern "struct _GArrowArrayBuilderClass" record _GArrowArrayBuilderClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowNullArrayBuilder" record _GArrowNullArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowNullArrayBuilderClass" record _GArrowNullArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowBooleanArrayBuilder" record _GArrowBooleanArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowBooleanArrayBuilderClass" record _GArrowBooleanArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowIntArrayBuilder" record _GArrowIntArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowIntArrayBuilderClass" record _GArrowIntArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowUIntArrayBuilder" record _GArrowUIntArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowUIntArrayBuilderClass" record _GArrowUIntArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowInt8ArrayBuilder" record _GArrowInt8ArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowInt8ArrayBuilderClass" record _GArrowInt8ArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowUInt8ArrayBuilder" record _GArrowUInt8ArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowUInt8ArrayBuilderClass" record _GArrowUInt8ArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowInt16ArrayBuilder" record _GArrowInt16ArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowInt16ArrayBuilderClass" record _GArrowInt16ArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowUInt16ArrayBuilder" record _GArrowUInt16ArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowUInt16ArrayBuilderClass" record _GArrowUInt16ArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowInt32ArrayBuilder" record _GArrowInt32ArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowInt32ArrayBuilderClass" record _GArrowInt32ArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowUInt32ArrayBuilder" record _GArrowUInt32ArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowUInt32ArrayBuilderClass" record _GArrowUInt32ArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowInt64ArrayBuilder" record _GArrowInt64ArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowInt64ArrayBuilderClass" record _GArrowInt64ArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowUInt64ArrayBuilder" record _GArrowUInt64ArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowUInt64ArrayBuilderClass" record _GArrowUInt64ArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowFloatArrayBuilder" record _GArrowFloatArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowFloatArrayBuilderClass" record _GArrowFloatArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowDoubleArrayBuilder" record _GArrowDoubleArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowDoubleArrayBuilderClass" record _GArrowDoubleArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowBinaryArrayBuilder" record _GArrowBinaryArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowBinaryArrayBuilderClass" record _GArrowBinaryArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowLargeBinaryArrayBuilder" record _GArrowLargeBinaryArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowLargeBinaryArrayBuilderClass" record _GArrowLargeBinaryArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowStringArrayBuilder" record _GArrowStringArrayBuilder {    var parent_instance : GArrowBinaryArrayBuilder;
  }

  extern "struct _GArrowStringArrayBuilderClass" record _GArrowStringArrayBuilderClass {    var parent_class : GArrowBinaryArrayBuilderClass;
  }

  extern "struct _GArrowLargeStringArrayBuilder" record _GArrowLargeStringArrayBuilder {    var parent_instance : GArrowLargeBinaryArrayBuilder;
  }

  extern "struct _GArrowLargeStringArrayBuilderClass" record _GArrowLargeStringArrayBuilderClass {    var parent_class : GArrowLargeBinaryArrayBuilderClass;
  }

  extern "struct _GArrowFixedSizeBinaryArrayBuilder" record _GArrowFixedSizeBinaryArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowFixedSizeBinaryArrayBuilderClass" record _GArrowFixedSizeBinaryArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowDate32ArrayBuilder" record _GArrowDate32ArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowDate32ArrayBuilderClass" record _GArrowDate32ArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowDate64ArrayBuilder" record _GArrowDate64ArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowDate64ArrayBuilderClass" record _GArrowDate64ArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowTimestampArrayBuilder" record _GArrowTimestampArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowTimestampArrayBuilderClass" record _GArrowTimestampArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowTime32ArrayBuilder" record _GArrowTime32ArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowTime32ArrayBuilderClass" record _GArrowTime32ArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowTime64ArrayBuilder" record _GArrowTime64ArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowTime64ArrayBuilderClass" record _GArrowTime64ArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowBinaryDictionaryArrayBuilder" record _GArrowBinaryDictionaryArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowBinaryDictionaryArrayBuilderClass" record _GArrowBinaryDictionaryArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowStringDictionaryArrayBuilder" record _GArrowStringDictionaryArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowStringDictionaryArrayBuilderClass" record _GArrowStringDictionaryArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowListArrayBuilder" record _GArrowListArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowListArrayBuilderClass" record _GArrowListArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowLargeListArrayBuilder" record _GArrowLargeListArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowLargeListArrayBuilderClass" record _GArrowLargeListArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowStructArrayBuilder" record _GArrowStructArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowStructArrayBuilderClass" record _GArrowStructArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowMapArrayBuilder" record _GArrowMapArrayBuilder {    var parent_instance : GArrowArrayBuilder;
  }

  extern "struct _GArrowMapArrayBuilderClass" record _GArrowMapArrayBuilderClass {    var parent_class : GArrowArrayBuilderClass;
  }

  extern "struct _GArrowDecimal128ArrayBuilder" record _GArrowDecimal128ArrayBuilder {    var parent_instance : GArrowFixedSizeBinaryArrayBuilder;
  }

  extern "struct _GArrowDecimal128ArrayBuilderClass" record _GArrowDecimal128ArrayBuilderClass {    var parent_class : GArrowFixedSizeBinaryArrayBuilderClass;
  }

  extern "struct _GArrowDecimal256ArrayBuilder" record _GArrowDecimal256ArrayBuilder {    var parent_instance : GArrowFixedSizeBinaryArrayBuilder;
  }

  extern "struct _GArrowDecimal256ArrayBuilderClass" record _GArrowDecimal256ArrayBuilderClass {    var parent_class : GArrowFixedSizeBinaryArrayBuilderClass;
  }

  extern "struct _GArrowChunkedArray" record _GArrowChunkedArray {    var parent_instance : GObject;
  }

  extern "struct _GArrowChunkedArrayClass" record _GArrowChunkedArrayClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowCodec" record _GArrowCodec {    var parent_instance : GObject;
  }

  extern "struct _GArrowCodecClass" record _GArrowCodecClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowReadOptions" record _GArrowReadOptions {    var parent_instance : GObject;
  }

  extern "struct _GArrowReadOptionsClass" record _GArrowReadOptionsClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowWriteOptions" record _GArrowWriteOptions {    var parent_instance : GObject;
  }

  extern "struct _GArrowWriteOptionsClass" record _GArrowWriteOptionsClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowSchema" record _GArrowSchema {    var parent_instance : GObject;
  }

  extern "struct _GArrowSchemaClass" record _GArrowSchemaClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowRecordBatch" record _GArrowRecordBatch {    var parent_instance : GObject;
  }

  extern "struct _GArrowRecordBatchClass" record _GArrowRecordBatchClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowRecordBatchIterator" record _GArrowRecordBatchIterator {    var parent_instance : GObject;
  }

  extern "struct _GArrowRecordBatchIteratorClass" record _GArrowRecordBatchIteratorClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowScalar" record _GArrowScalar {    var parent_instance : GObject;
  }

  extern "struct _GArrowScalarClass" record _GArrowScalarClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowNullScalar" record _GArrowNullScalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowNullScalarClass" record _GArrowNullScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowBooleanScalar" record _GArrowBooleanScalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowBooleanScalarClass" record _GArrowBooleanScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowInt8Scalar" record _GArrowInt8Scalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowInt8ScalarClass" record _GArrowInt8ScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowInt16Scalar" record _GArrowInt16Scalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowInt16ScalarClass" record _GArrowInt16ScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowInt32Scalar" record _GArrowInt32Scalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowInt32ScalarClass" record _GArrowInt32ScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowInt64Scalar" record _GArrowInt64Scalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowInt64ScalarClass" record _GArrowInt64ScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowUInt8Scalar" record _GArrowUInt8Scalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowUInt8ScalarClass" record _GArrowUInt8ScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowUInt16Scalar" record _GArrowUInt16Scalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowUInt16ScalarClass" record _GArrowUInt16ScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowUInt32Scalar" record _GArrowUInt32Scalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowUInt32ScalarClass" record _GArrowUInt32ScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowUInt64Scalar" record _GArrowUInt64Scalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowUInt64ScalarClass" record _GArrowUInt64ScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowFloatScalar" record _GArrowFloatScalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowFloatScalarClass" record _GArrowFloatScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowDoubleScalar" record _GArrowDoubleScalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowDoubleScalarClass" record _GArrowDoubleScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowBaseBinaryScalar" record _GArrowBaseBinaryScalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowBaseBinaryScalarClass" record _GArrowBaseBinaryScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowBinaryScalar" record _GArrowBinaryScalar {    var parent_instance : GArrowBaseBinaryScalar;
  }

  extern "struct _GArrowBinaryScalarClass" record _GArrowBinaryScalarClass {    var parent_class : GArrowBaseBinaryScalarClass;
  }

  extern "struct _GArrowStringScalar" record _GArrowStringScalar {    var parent_instance : GArrowBaseBinaryScalar;
  }

  extern "struct _GArrowStringScalarClass" record _GArrowStringScalarClass {    var parent_class : GArrowBaseBinaryScalarClass;
  }

  extern "struct _GArrowLargeBinaryScalar" record _GArrowLargeBinaryScalar {    var parent_instance : GArrowBaseBinaryScalar;
  }

  extern "struct _GArrowLargeBinaryScalarClass" record _GArrowLargeBinaryScalarClass {    var parent_class : GArrowBaseBinaryScalarClass;
  }

  extern "struct _GArrowLargeStringScalar" record _GArrowLargeStringScalar {    var parent_instance : GArrowBaseBinaryScalar;
  }

  extern "struct _GArrowLargeStringScalarClass" record _GArrowLargeStringScalarClass {    var parent_class : GArrowBaseBinaryScalarClass;
  }

  extern "struct _GArrowFixedSizeBinaryScalar" record _GArrowFixedSizeBinaryScalar {    var parent_instance : GArrowBaseBinaryScalar;
  }

  extern "struct _GArrowFixedSizeBinaryScalarClass" record _GArrowFixedSizeBinaryScalarClass {    var parent_class : GArrowBaseBinaryScalarClass;
  }

  extern "struct _GArrowDate32Scalar" record _GArrowDate32Scalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowDate32ScalarClass" record _GArrowDate32ScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowDate64Scalar" record _GArrowDate64Scalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowDate64ScalarClass" record _GArrowDate64ScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowTime32Scalar" record _GArrowTime32Scalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowTime32ScalarClass" record _GArrowTime32ScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowTime64Scalar" record _GArrowTime64Scalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowTime64ScalarClass" record _GArrowTime64ScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowTimestampScalar" record _GArrowTimestampScalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowTimestampScalarClass" record _GArrowTimestampScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowDecimal128Scalar" record _GArrowDecimal128Scalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowDecimal128ScalarClass" record _GArrowDecimal128ScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowDecimal256Scalar" record _GArrowDecimal256Scalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowDecimal256ScalarClass" record _GArrowDecimal256ScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowBaseListScalar" record _GArrowBaseListScalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowBaseListScalarClass" record _GArrowBaseListScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowListScalar" record _GArrowListScalar {    var parent_instance : GArrowBaseListScalar;
  }

  extern "struct _GArrowListScalarClass" record _GArrowListScalarClass {    var parent_class : GArrowBaseListScalarClass;
  }

  extern "struct _GArrowLargeListScalar" record _GArrowLargeListScalar {    var parent_instance : GArrowBaseListScalar;
  }

  extern "struct _GArrowLargeListScalarClass" record _GArrowLargeListScalarClass {    var parent_class : GArrowBaseListScalarClass;
  }

  extern "struct _GArrowMapScalar" record _GArrowMapScalar {    var parent_instance : GArrowBaseListScalar;
  }

  extern "struct _GArrowMapScalarClass" record _GArrowMapScalarClass {    var parent_class : GArrowBaseListScalarClass;
  }

  extern "struct _GArrowStructScalar" record _GArrowStructScalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowStructScalarClass" record _GArrowStructScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowUnionScalar" record _GArrowUnionScalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowUnionScalarClass" record _GArrowUnionScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GArrowSparseUnionScalar" record _GArrowSparseUnionScalar {    var parent_instance : GArrowUnionScalar;
  }

  extern "struct _GArrowSparseUnionScalarClass" record _GArrowSparseUnionScalarClass {    var parent_class : GArrowUnionScalarClass;
  }

  extern "struct _GArrowDenseUnionScalar" record _GArrowDenseUnionScalar {    var parent_instance : GArrowUnionScalar;
  }

  extern "struct _GArrowDenseUnionScalarClass" record _GArrowDenseUnionScalarClass {    var parent_class : GArrowUnionScalarClass;
  }

  extern "struct _GArrowExtensionScalar" record _GArrowExtensionScalar {    var parent_instance : GArrowScalar;
  }

  extern "struct _GArrowExtensionScalarClass" record _GArrowExtensionScalarClass {    var parent_class : GArrowScalarClass;
  }

  extern "struct _GInputVector" record _GInputVector {    var buffer : gpointer;
    var size : gsize;
  }

  extern "struct _GInputMessage" record _GInputMessage {    var address : c_ptr(c_ptr(GSocketAddress));
    var vectors : c_ptr(GInputVector);
    var num_vectors : guint;
    var bytes_received : gsize;
    var flags : gint;
    var control_messages : c_ptr(c_ptr(c_ptr(GSocketControlMessage)));
    var num_control_messages : c_ptr(guint);
  }

  extern "struct _GOutputVector" record _GOutputVector {    var buffer : gconstpointer;
    var size : gsize;
  }

  extern "struct _GOutputMessage" record _GOutputMessage {    var address : c_ptr(GSocketAddress);
    var vectors : c_ptr(GOutputVector);
    var num_vectors : guint;
    var bytes_sent : guint;
    var control_messages : c_ptr(c_ptr(GSocketControlMessage));
    var num_control_messages : guint;
  }

  extern "struct _GActionInterface" record _GActionInterface {    var g_iface : GTypeInterface;
    var get_name : c_ptr(c_fn_ptr);
    var get_parameter_type : c_ptr(c_fn_ptr);
    var get_state_type : c_ptr(c_fn_ptr);
    var get_state_hint : c_ptr(c_fn_ptr);
    var get_enabled : c_ptr(c_fn_ptr);
    var get_state : c_ptr(c_fn_ptr);
    var change_state : c_ptr(c_fn_ptr);
    var activate : c_ptr(c_fn_ptr);
  }

  extern "struct _GActionGroupInterface" record _GActionGroupInterface {    var g_iface : GTypeInterface;
    var has_action : c_ptr(c_fn_ptr);
    var list_actions : c_ptr(c_fn_ptr);
    var get_action_enabled : c_ptr(c_fn_ptr);
    var get_action_parameter_type : c_ptr(c_fn_ptr);
    var get_action_state_type : c_ptr(c_fn_ptr);
    var get_action_state_hint : c_ptr(c_fn_ptr);
    var get_action_state : c_ptr(c_fn_ptr);
    var change_action_state : c_ptr(c_fn_ptr);
    var activate_action : c_ptr(c_fn_ptr);
    var action_added : c_ptr(c_fn_ptr);
    var action_removed : c_ptr(c_fn_ptr);
    var action_enabled_changed : c_ptr(c_fn_ptr);
    var action_state_changed : c_ptr(c_fn_ptr);
    var query_action : c_ptr(c_fn_ptr);
  }

  extern "struct _GActionMapInterface" record _GActionMapInterface {    var g_iface : GTypeInterface;
    var lookup_action : c_ptr(c_fn_ptr);
    var add_action : c_ptr(c_fn_ptr);
    var remove_action : c_ptr(c_fn_ptr);
  }

  extern "struct _GActionEntry" record _GActionEntry {    var name : c_ptr(gchar);
    var activate : c_ptr(c_fn_ptr);
    var parameter_type : c_ptr(gchar);
    var state : c_ptr(gchar);
    var change_state : c_ptr(c_fn_ptr);
    var padding : c_ptr(gsize);
  }

  extern "struct _GAppInfoIface" record _GAppInfoIface {    var g_iface : GTypeInterface;
    var dup : c_ptr(c_fn_ptr);
    var equal : c_ptr(c_fn_ptr);
    var get_id : c_ptr(c_fn_ptr);
    var get_name : c_ptr(c_fn_ptr);
    var get_description : c_ptr(c_fn_ptr);
    var get_executable : c_ptr(c_fn_ptr);
    var get_icon : c_ptr(c_fn_ptr);
    var launch : c_ptr(c_fn_ptr);
    var supports_uris : c_ptr(c_fn_ptr);
    var supports_files : c_ptr(c_fn_ptr);
    var launch_uris : c_ptr(c_fn_ptr);
    var should_show : c_ptr(c_fn_ptr);
    var set_as_default_for_type : c_ptr(c_fn_ptr);
    var set_as_default_for_extension : c_ptr(c_fn_ptr);
    var add_supports_type : c_ptr(c_fn_ptr);
    var can_remove_supports_type : c_ptr(c_fn_ptr);
    var remove_supports_type : c_ptr(c_fn_ptr);
    var can_delete : c_ptr(c_fn_ptr);
    var do_delete : c_ptr(c_fn_ptr);
    var get_commandline : c_ptr(c_fn_ptr);
    var get_display_name : c_ptr(c_fn_ptr);
    var set_as_last_used_for_type : c_ptr(c_fn_ptr);
    var get_supported_types : c_ptr(c_fn_ptr);
    var launch_uris_async : c_ptr(c_fn_ptr);
    var launch_uris_finish : c_ptr(c_fn_ptr);
  }

  extern "struct _GAppLaunchContext" record _GAppLaunchContext {    var parent_instance : GObject;
    var priv : c_ptr(GAppLaunchContextPrivate);
  }

  extern "struct _GAppLaunchContextClass" record _GAppLaunchContextClass {    var parent_class : GObjectClass;
    var get_display : c_ptr(c_fn_ptr);
    var get_startup_notify_id : c_ptr(c_fn_ptr);
    var launch_failed : c_ptr(c_fn_ptr);
    var launched : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
  }

  extern "struct _GApplication" record _GApplication {    var parent_instance : GObject;
    var priv : c_ptr(GApplicationPrivate);
  }

  extern "struct _GApplicationClass" record _GApplicationClass {    var parent_class : GObjectClass;
    var startup : c_ptr(c_fn_ptr);
    var activate : c_ptr(c_fn_ptr);
    var open : c_ptr(c_fn_ptr);
    var command_line : c_ptr(c_fn_ptr);
    var local_command_line : c_ptr(c_fn_ptr);
    var before_emit : c_ptr(c_fn_ptr);
    var after_emit : c_ptr(c_fn_ptr);
    var add_platform_data : c_ptr(c_fn_ptr);
    var quit_mainloop : c_ptr(c_fn_ptr);
    var run_mainloop : c_ptr(c_fn_ptr);
    var shutdown : c_ptr(c_fn_ptr);
    var dbus_register : c_ptr(c_fn_ptr);
    var dbus_unregister : c_ptr(c_fn_ptr);
    var handle_local_options : c_ptr(c_fn_ptr);
    var name_lost : c_ptr(c_fn_ptr);
    var padding : c_ptr(gpointer);
  }

  extern "struct _GApplicationCommandLine" record _GApplicationCommandLine {    var parent_instance : GObject;
    var priv : c_ptr(GApplicationCommandLinePrivate);
  }

  extern "struct _GApplicationCommandLineClass" record _GApplicationCommandLineClass {    var parent_class : GObjectClass;
    var print_literal : c_ptr(c_fn_ptr);
    var printerr_literal : c_ptr(c_fn_ptr);
    var get_stdin : c_ptr(c_fn_ptr);
    var padding : c_ptr(gpointer);
  }

  extern "struct _GInitableIface" record _GInitableIface {    var g_iface : GTypeInterface;
    var init : c_ptr(c_fn_ptr);
  }

  extern "struct _GAsyncInitableIface" record _GAsyncInitableIface {    var g_iface : GTypeInterface;
    var init_async : c_ptr(c_fn_ptr);
    var init_finish : c_ptr(c_fn_ptr);
  }

  extern "struct _GAsyncResultIface" record _GAsyncResultIface {    var g_iface : GTypeInterface;
    var get_user_data : c_ptr(c_fn_ptr);
    var get_source_object : c_ptr(c_fn_ptr);
    var is_tagged : c_ptr(c_fn_ptr);
  }

  extern "struct _GInputStream" record _GInputStream {    var parent_instance : GObject;
    var priv : c_ptr(GInputStreamPrivate);
  }

  extern "struct _GInputStreamClass" record _GInputStreamClass {    var parent_class : GObjectClass;
    var read_fn : c_ptr(c_fn_ptr);
    var skip : c_ptr(c_fn_ptr);
    var close_fn : c_ptr(c_fn_ptr);
    var read_async : c_ptr(c_fn_ptr);
    var read_finish : c_ptr(c_fn_ptr);
    var skip_async : c_ptr(c_fn_ptr);
    var skip_finish : c_ptr(c_fn_ptr);
    var close_async : c_ptr(c_fn_ptr);
    var close_finish : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GFilterInputStream" record _GFilterInputStream {    var parent_instance : GInputStream;
    var base_stream : c_ptr(GInputStream);
  }

  extern "struct _GFilterInputStreamClass" record _GFilterInputStreamClass {    var parent_class : GInputStreamClass;
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
  }

  extern "struct _GBufferedInputStream" record _GBufferedInputStream {    var parent_instance : GFilterInputStream;
    var priv : c_ptr(GBufferedInputStreamPrivate);
  }

  extern "struct _GBufferedInputStreamClass" record _GBufferedInputStreamClass {    var parent_class : GFilterInputStreamClass;
    var fill : c_ptr(c_fn_ptr);
    var fill_async : c_ptr(c_fn_ptr);
    var fill_finish : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GOutputStream" record _GOutputStream {    var parent_instance : GObject;
    var priv : c_ptr(GOutputStreamPrivate);
  }

  extern "struct _GOutputStreamClass" record _GOutputStreamClass {    var parent_class : GObjectClass;
    var write_fn : c_ptr(c_fn_ptr);
    var splice : c_ptr(c_fn_ptr);
    var flush : c_ptr(c_fn_ptr);
    var close_fn : c_ptr(c_fn_ptr);
    var write_async : c_ptr(c_fn_ptr);
    var write_finish : c_ptr(c_fn_ptr);
    var splice_async : c_ptr(c_fn_ptr);
    var splice_finish : c_ptr(c_fn_ptr);
    var flush_async : c_ptr(c_fn_ptr);
    var flush_finish : c_ptr(c_fn_ptr);
    var close_async : c_ptr(c_fn_ptr);
    var close_finish : c_ptr(c_fn_ptr);
    var writev_fn : c_ptr(c_fn_ptr);
    var writev_async : c_ptr(c_fn_ptr);
    var writev_finish : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
    var _g_reserved6 : c_ptr(c_fn_ptr);
    var _g_reserved7 : c_ptr(c_fn_ptr);
    var _g_reserved8 : c_ptr(c_fn_ptr);
  }

  extern "struct _GFilterOutputStream" record _GFilterOutputStream {    var parent_instance : GOutputStream;
    var base_stream : c_ptr(GOutputStream);
  }

  extern "struct _GFilterOutputStreamClass" record _GFilterOutputStreamClass {    var parent_class : GOutputStreamClass;
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
  }

  extern "struct _GBufferedOutputStream" record _GBufferedOutputStream {    var parent_instance : GFilterOutputStream;
    var priv : c_ptr(GBufferedOutputStreamPrivate);
  }

  extern "struct _GBufferedOutputStreamClass" record _GBufferedOutputStreamClass {    var parent_class : GFilterOutputStreamClass;
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
  }

  extern "struct _GCancellable" record _GCancellable {    var parent_instance : GObject;
    var priv : c_ptr(GCancellablePrivate);
  }

  extern "struct _GCancellableClass" record _GCancellableClass {    var parent_class : GObjectClass;
    var cancelled : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GConverterIface" record _GConverterIface {    var g_iface : GTypeInterface;
    var convert : c_ptr(c_fn_ptr);
    var reset : c_ptr(c_fn_ptr);
  }

  extern "struct _GCharsetConverterClass" record _GCharsetConverterClass {    var parent_class : GObjectClass;
  }

  extern "struct _GConverterInputStream" record _GConverterInputStream {    var parent_instance : GFilterInputStream;
    var priv : c_ptr(GConverterInputStreamPrivate);
  }

  extern "struct _GConverterInputStreamClass" record _GConverterInputStreamClass {    var parent_class : GFilterInputStreamClass;
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GConverterOutputStream" record _GConverterOutputStream {    var parent_instance : GFilterOutputStream;
    var priv : c_ptr(GConverterOutputStreamPrivate);
  }

  extern "struct _GConverterOutputStreamClass" record _GConverterOutputStreamClass {    var parent_class : GFilterOutputStreamClass;
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GDatagramBasedInterface" record _GDatagramBasedInterface {    var g_iface : GTypeInterface;
    var receive_messages : c_ptr(c_fn_ptr);
    var send_messages : c_ptr(c_fn_ptr);
    var create_source : c_ptr(c_fn_ptr);
    var condition_check : c_ptr(c_fn_ptr);
    var condition_wait : c_ptr(c_fn_ptr);
  }

  extern "struct _GDataInputStream" record _GDataInputStream {    var parent_instance : GBufferedInputStream;
    var priv : c_ptr(GDataInputStreamPrivate);
  }

  extern "struct _GDataInputStreamClass" record _GDataInputStreamClass {    var parent_class : GBufferedInputStreamClass;
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GDataOutputStream" record _GDataOutputStream {    var parent_instance : GFilterOutputStream;
    var priv : c_ptr(GDataOutputStreamPrivate);
  }

  extern "struct _GDataOutputStreamClass" record _GDataOutputStreamClass {    var parent_class : GFilterOutputStreamClass;
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GDBusInterfaceVTable" record _GDBusInterfaceVTable {    var method_call : GDBusInterfaceMethodCallFunc;
    var get_property : GDBusInterfaceGetPropertyFunc;
    var set_property : GDBusInterfaceSetPropertyFunc;
    var padding : c_ptr(gpointer);
  }

  extern "struct _GDBusSubtreeVTable" record _GDBusSubtreeVTable {    var enumerate : GDBusSubtreeEnumerateFunc;
    var introspect : GDBusSubtreeIntrospectFunc;
    var dispatch : GDBusSubtreeDispatchFunc;
    var padding : c_ptr(gpointer);
  }

  extern "struct _GDBusErrorEntry" record _GDBusErrorEntry {    var error_code : gint;
    var dbus_error_name : c_ptr(gchar);
  }

  extern "struct _GDBusInterfaceIface" record _GDBusInterfaceIface {    var parent_iface : GTypeInterface;
    var get_info : c_ptr(c_fn_ptr);
    var get_object : c_ptr(c_fn_ptr);
    var set_object : c_ptr(c_fn_ptr);
    var dup_object : c_ptr(c_fn_ptr);
  }

  extern "struct _GDBusInterfaceSkeleton" record _GDBusInterfaceSkeleton {    var parent_instance : GObject;
    var priv : c_ptr(GDBusInterfaceSkeletonPrivate);
  }

  extern "struct _GDBusInterfaceSkeletonClass" record _GDBusInterfaceSkeletonClass {    var parent_class : GObjectClass;
    var get_info : c_ptr(c_fn_ptr);
    var get_vtable : c_ptr(c_fn_ptr);
    var get_properties : c_ptr(c_fn_ptr);
    var flush : c_ptr(c_fn_ptr);
    var vfunc_padding : c_ptr(gpointer);
    var g_authorize_method : c_ptr(c_fn_ptr);
    var signal_padding : c_ptr(gpointer);
  }

  extern "struct _GDBusAnnotationInfo" record _GDBusAnnotationInfo {    var ref_count : gint;
    var key : c_ptr(gchar);
    var value : c_ptr(gchar);
    var annotations : c_ptr(c_ptr(_GDBusAnnotationInfo));
  }

  extern "struct _GDBusArgInfo" record _GDBusArgInfo {    var ref_count : gint;
    var name : c_ptr(gchar);
    var signature : c_ptr(gchar);
    var annotations : c_ptr(c_ptr(GDBusAnnotationInfo));
  }

  extern "struct _GDBusMethodInfo" record _GDBusMethodInfo {    var ref_count : gint;
    var name : c_ptr(gchar);
    var in_args : c_ptr(c_ptr(GDBusArgInfo));
    var out_args : c_ptr(c_ptr(GDBusArgInfo));
    var annotations : c_ptr(c_ptr(GDBusAnnotationInfo));
  }

  extern "struct _GDBusSignalInfo" record _GDBusSignalInfo {    var ref_count : gint;
    var name : c_ptr(gchar);
    var args : c_ptr(c_ptr(GDBusArgInfo));
    var annotations : c_ptr(c_ptr(GDBusAnnotationInfo));
  }

  extern "struct _GDBusPropertyInfo" record _GDBusPropertyInfo {    var ref_count : gint;
    var name : c_ptr(gchar);
    var signature : c_ptr(gchar);
    var flags : GDBusPropertyInfoFlags;
    var annotations : c_ptr(c_ptr(GDBusAnnotationInfo));
  }

  extern "struct _GDBusInterfaceInfo" record _GDBusInterfaceInfo {    var ref_count : gint;
    var name : c_ptr(gchar);
    var methods : c_ptr(c_ptr(GDBusMethodInfo));
    var signals : c_ptr(c_ptr(GDBusSignalInfo));
    var properties : c_ptr(c_ptr(GDBusPropertyInfo));
    var annotations : c_ptr(c_ptr(GDBusAnnotationInfo));
  }

  extern "struct _GDBusNodeInfo" record _GDBusNodeInfo {    var ref_count : gint;
    var path : c_ptr(gchar);
    var interfaces : c_ptr(c_ptr(GDBusInterfaceInfo));
    var nodes : c_ptr(c_ptr(_GDBusNodeInfo));
    var annotations : c_ptr(c_ptr(GDBusAnnotationInfo));
  }

  extern "struct _GDBusObjectIface" record _GDBusObjectIface {    var parent_iface : GTypeInterface;
    var get_object_path : c_ptr(c_fn_ptr);
    var get_interfaces : c_ptr(c_fn_ptr);
    var get_interface : c_ptr(c_fn_ptr);
    var interface_added : c_ptr(c_fn_ptr);
    var interface_removed : c_ptr(c_fn_ptr);
  }

  extern "struct _GDBusObjectManagerIface" record _GDBusObjectManagerIface {    var parent_iface : GTypeInterface;
    var get_object_path : c_ptr(c_fn_ptr);
    var get_objects : c_ptr(c_fn_ptr);
    var get_object : c_ptr(c_fn_ptr);
    var get_interface : c_ptr(c_fn_ptr);
    var object_added : c_ptr(c_fn_ptr);
    var object_removed : c_ptr(c_fn_ptr);
    var interface_added : c_ptr(c_fn_ptr);
    var interface_removed : c_ptr(c_fn_ptr);
  }

  extern "struct _GDBusObjectManagerClient" record _GDBusObjectManagerClient {    var parent_instance : GObject;
    var priv : c_ptr(GDBusObjectManagerClientPrivate);
  }

  extern "struct _GDBusObjectManagerClientClass" record _GDBusObjectManagerClientClass {    var parent_class : GObjectClass;
    var interface_proxy_signal : c_ptr(c_fn_ptr);
    var interface_proxy_properties_changed : c_ptr(c_fn_ptr);
    var padding : c_ptr(gpointer);
  }

  extern "struct _GDBusObjectManagerServer" record _GDBusObjectManagerServer {    var parent_instance : GObject;
    var priv : c_ptr(GDBusObjectManagerServerPrivate);
  }

  extern "struct _GDBusObjectManagerServerClass" record _GDBusObjectManagerServerClass {    var parent_class : GObjectClass;
    var padding : c_ptr(gpointer);
  }

  extern "struct _GDBusObjectProxy" record _GDBusObjectProxy {    var parent_instance : GObject;
    var priv : c_ptr(GDBusObjectProxyPrivate);
  }

  extern "struct _GDBusObjectProxyClass" record _GDBusObjectProxyClass {    var parent_class : GObjectClass;
    var padding : c_ptr(gpointer);
  }

  extern "struct _GDBusObjectSkeleton" record _GDBusObjectSkeleton {    var parent_instance : GObject;
    var priv : c_ptr(GDBusObjectSkeletonPrivate);
  }

  extern "struct _GDBusObjectSkeletonClass" record _GDBusObjectSkeletonClass {    var parent_class : GObjectClass;
    var authorize_method : c_ptr(c_fn_ptr);
    var padding : c_ptr(gpointer);
  }

  extern "struct _GDBusProxy" record _GDBusProxy {    var parent_instance : GObject;
    var priv : c_ptr(GDBusProxyPrivate);
  }

  extern "struct _GDBusProxyClass" record _GDBusProxyClass {    var parent_class : GObjectClass;
    var g_properties_changed : c_ptr(c_fn_ptr);
    var g_signal : c_ptr(c_fn_ptr);
    var padding : c_ptr(gpointer);
  }

  extern "struct _GDriveIface" record _GDriveIface {    var g_iface : GTypeInterface;
    var changed : c_ptr(c_fn_ptr);
    var disconnected : c_ptr(c_fn_ptr);
    var eject_button : c_ptr(c_fn_ptr);
    var get_name : c_ptr(c_fn_ptr);
    var get_icon : c_ptr(c_fn_ptr);
    var has_volumes : c_ptr(c_fn_ptr);
    var get_volumes : c_ptr(c_fn_ptr);
    var is_media_removable : c_ptr(c_fn_ptr);
    var has_media : c_ptr(c_fn_ptr);
    var is_media_check_automatic : c_ptr(c_fn_ptr);
    var can_eject : c_ptr(c_fn_ptr);
    var can_poll_for_media : c_ptr(c_fn_ptr);
    var eject : c_ptr(c_fn_ptr);
    var eject_finish : c_ptr(c_fn_ptr);
    var poll_for_media : c_ptr(c_fn_ptr);
    var poll_for_media_finish : c_ptr(c_fn_ptr);
    var get_identifier : c_ptr(c_fn_ptr);
    var enumerate_identifiers : c_ptr(c_fn_ptr);
    var get_start_stop_type : c_ptr(c_fn_ptr);
    var can_start : c_ptr(c_fn_ptr);
    var can_start_degraded : c_ptr(c_fn_ptr);
    var start : c_ptr(c_fn_ptr);
    var start_finish : c_ptr(c_fn_ptr);
    var can_stop : c_ptr(c_fn_ptr);
    var stop : c_ptr(c_fn_ptr);
    var stop_finish : c_ptr(c_fn_ptr);
    var stop_button : c_ptr(c_fn_ptr);
    var eject_with_operation : c_ptr(c_fn_ptr);
    var eject_with_operation_finish : c_ptr(c_fn_ptr);
    var get_sort_key : c_ptr(c_fn_ptr);
    var get_symbolic_icon : c_ptr(c_fn_ptr);
    var is_removable : c_ptr(c_fn_ptr);
  }

  extern "struct _GDtlsConnectionInterface" record _GDtlsConnectionInterface {    var g_iface : GTypeInterface;
    var accept_certificate : c_ptr(c_fn_ptr);
    var handshake : c_ptr(c_fn_ptr);
    var handshake_async : c_ptr(c_fn_ptr);
    var handshake_finish : c_ptr(c_fn_ptr);
    var shutdown : c_ptr(c_fn_ptr);
    var shutdown_async : c_ptr(c_fn_ptr);
    var shutdown_finish : c_ptr(c_fn_ptr);
    var set_advertised_protocols : c_ptr(c_fn_ptr);
    var get_negotiated_protocol : c_ptr(c_fn_ptr);
  }

  extern "struct _GDtlsClientConnectionInterface" record _GDtlsClientConnectionInterface {    var g_iface : GTypeInterface;
  }

  extern "struct _GDtlsServerConnectionInterface" record _GDtlsServerConnectionInterface {    var g_iface : GTypeInterface;
  }

  extern "struct _GIconIface" record _GIconIface {    var g_iface : GTypeInterface;
    var hash : c_ptr(c_fn_ptr);
    var equal : c_ptr(c_fn_ptr);
    var to_tokens : c_ptr(c_fn_ptr);
    var from_tokens : c_ptr(c_fn_ptr);
    var serialize : c_ptr(c_fn_ptr);
  }

  extern "struct _GEmblemedIcon" record _GEmblemedIcon {    var parent_instance : GObject;
    var priv : c_ptr(GEmblemedIconPrivate);
  }

  extern "struct _GEmblemedIconClass" record _GEmblemedIconClass {    var parent_class : GObjectClass;
  }

  extern "struct _GFileIface" record _GFileIface {    var g_iface : GTypeInterface;
    var dup : c_ptr(c_fn_ptr);
    var hash : c_ptr(c_fn_ptr);
    var equal : c_ptr(c_fn_ptr);
    var is_native : c_ptr(c_fn_ptr);
    var has_uri_scheme : c_ptr(c_fn_ptr);
    var get_uri_scheme : c_ptr(c_fn_ptr);
    var get_basename : c_ptr(c_fn_ptr);
    var get_path : c_ptr(c_fn_ptr);
    var get_uri : c_ptr(c_fn_ptr);
    var get_parse_name : c_ptr(c_fn_ptr);
    var get_parent : c_ptr(c_fn_ptr);
    var prefix_matches : c_ptr(c_fn_ptr);
    var get_relative_path : c_ptr(c_fn_ptr);
    var resolve_relative_path : c_ptr(c_fn_ptr);
    var get_child_for_display_name : c_ptr(c_fn_ptr);
    var enumerate_children : c_ptr(c_fn_ptr);
    var enumerate_children_async : c_ptr(c_fn_ptr);
    var enumerate_children_finish : c_ptr(c_fn_ptr);
    var query_info : c_ptr(c_fn_ptr);
    var query_info_async : c_ptr(c_fn_ptr);
    var query_info_finish : c_ptr(c_fn_ptr);
    var query_filesystem_info : c_ptr(c_fn_ptr);
    var query_filesystem_info_async : c_ptr(c_fn_ptr);
    var query_filesystem_info_finish : c_ptr(c_fn_ptr);
    var find_enclosing_mount : c_ptr(c_fn_ptr);
    var find_enclosing_mount_async : c_ptr(c_fn_ptr);
    var find_enclosing_mount_finish : c_ptr(c_fn_ptr);
    var set_display_name : c_ptr(c_fn_ptr);
    var set_display_name_async : c_ptr(c_fn_ptr);
    var set_display_name_finish : c_ptr(c_fn_ptr);
    var query_settable_attributes : c_ptr(c_fn_ptr);
    var _query_settable_attributes_async : c_ptr(c_fn_ptr);
    var _query_settable_attributes_finish : c_ptr(c_fn_ptr);
    var query_writable_namespaces : c_ptr(c_fn_ptr);
    var _query_writable_namespaces_async : c_ptr(c_fn_ptr);
    var _query_writable_namespaces_finish : c_ptr(c_fn_ptr);
    var set_attribute : c_ptr(c_fn_ptr);
    var set_attributes_from_info : c_ptr(c_fn_ptr);
    var set_attributes_async : c_ptr(c_fn_ptr);
    var set_attributes_finish : c_ptr(c_fn_ptr);
    var read_fn : c_ptr(c_fn_ptr);
    var read_async : c_ptr(c_fn_ptr);
    var read_finish : c_ptr(c_fn_ptr);
    var append_to : c_ptr(c_fn_ptr);
    var append_to_async : c_ptr(c_fn_ptr);
    var append_to_finish : c_ptr(c_fn_ptr);
    var create : c_ptr(c_fn_ptr);
    var create_async : c_ptr(c_fn_ptr);
    var create_finish : c_ptr(c_fn_ptr);
    var replace : c_ptr(c_fn_ptr);
    var replace_async : c_ptr(c_fn_ptr);
    var replace_finish : c_ptr(c_fn_ptr);
    var delete_file : c_ptr(c_fn_ptr);
    var delete_file_async : c_ptr(c_fn_ptr);
    var delete_file_finish : c_ptr(c_fn_ptr);
    var trash : c_ptr(c_fn_ptr);
    var trash_async : c_ptr(c_fn_ptr);
    var trash_finish : c_ptr(c_fn_ptr);
    var make_directory : c_ptr(c_fn_ptr);
    var make_directory_async : c_ptr(c_fn_ptr);
    var make_directory_finish : c_ptr(c_fn_ptr);
    var make_symbolic_link : c_ptr(c_fn_ptr);
    var _make_symbolic_link_async : c_ptr(c_fn_ptr);
    var _make_symbolic_link_finish : c_ptr(c_fn_ptr);
    var copy : c_ptr(c_fn_ptr);
    var copy_async : c_ptr(c_fn_ptr);
    var copy_finish : c_ptr(c_fn_ptr);
    var move : c_ptr(c_fn_ptr);
    var _move_async : c_ptr(c_fn_ptr);
    var _move_finish : c_ptr(c_fn_ptr);
    var mount_mountable : c_ptr(c_fn_ptr);
    var mount_mountable_finish : c_ptr(c_fn_ptr);
    var unmount_mountable : c_ptr(c_fn_ptr);
    var unmount_mountable_finish : c_ptr(c_fn_ptr);
    var eject_mountable : c_ptr(c_fn_ptr);
    var eject_mountable_finish : c_ptr(c_fn_ptr);
    var mount_enclosing_volume : c_ptr(c_fn_ptr);
    var mount_enclosing_volume_finish : c_ptr(c_fn_ptr);
    var monitor_dir : c_ptr(c_fn_ptr);
    var monitor_file : c_ptr(c_fn_ptr);
    var open_readwrite : c_ptr(c_fn_ptr);
    var open_readwrite_async : c_ptr(c_fn_ptr);
    var open_readwrite_finish : c_ptr(c_fn_ptr);
    var create_readwrite : c_ptr(c_fn_ptr);
    var create_readwrite_async : c_ptr(c_fn_ptr);
    var create_readwrite_finish : c_ptr(c_fn_ptr);
    var replace_readwrite : c_ptr(c_fn_ptr);
    var replace_readwrite_async : c_ptr(c_fn_ptr);
    var replace_readwrite_finish : c_ptr(c_fn_ptr);
    var start_mountable : c_ptr(c_fn_ptr);
    var start_mountable_finish : c_ptr(c_fn_ptr);
    var stop_mountable : c_ptr(c_fn_ptr);
    var stop_mountable_finish : c_ptr(c_fn_ptr);
    var supports_thread_contexts : gboolean;
    var unmount_mountable_with_operation : c_ptr(c_fn_ptr);
    var unmount_mountable_with_operation_finish : c_ptr(c_fn_ptr);
    var eject_mountable_with_operation : c_ptr(c_fn_ptr);
    var eject_mountable_with_operation_finish : c_ptr(c_fn_ptr);
    var poll_mountable : c_ptr(c_fn_ptr);
    var poll_mountable_finish : c_ptr(c_fn_ptr);
    var measure_disk_usage : c_ptr(c_fn_ptr);
    var measure_disk_usage_async : c_ptr(c_fn_ptr);
    var measure_disk_usage_finish : c_ptr(c_fn_ptr);
  }

  extern record _GFileAttributeInfo {}

  extern "struct _GFileAttributeInfoList" record _GFileAttributeInfoList {    var infos : c_ptr(GFileAttributeInfo);
    var n_infos : c_int;
  }

  extern "struct _GFileEnumerator" record _GFileEnumerator {    var parent_instance : GObject;
    var priv : c_ptr(GFileEnumeratorPrivate);
  }

  extern "struct _GFileEnumeratorClass" record _GFileEnumeratorClass {    var parent_class : GObjectClass;
    var next_file : c_ptr(c_fn_ptr);
    var close_fn : c_ptr(c_fn_ptr);
    var next_files_async : c_ptr(c_fn_ptr);
    var next_files_finish : c_ptr(c_fn_ptr);
    var close_async : c_ptr(c_fn_ptr);
    var close_finish : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
    var _g_reserved6 : c_ptr(c_fn_ptr);
    var _g_reserved7 : c_ptr(c_fn_ptr);
  }

  extern "struct _GFileInputStream" record _GFileInputStream {    var parent_instance : GInputStream;
    var priv : c_ptr(GFileInputStreamPrivate);
  }

  extern "struct _GFileInputStreamClass" record _GFileInputStreamClass {    var parent_class : GInputStreamClass;
    var tell : c_ptr(c_fn_ptr);
    var can_seek : c_ptr(c_fn_ptr);
    var seek : c_ptr(c_fn_ptr);
    var query_info : c_ptr(c_fn_ptr);
    var query_info_async : c_ptr(c_fn_ptr);
    var query_info_finish : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GIOStream" record _GIOStream {    var parent_instance : GObject;
    var priv : c_ptr(GIOStreamPrivate);
  }

  extern "struct _GIOStreamClass" record _GIOStreamClass {    var parent_class : GObjectClass;
    var get_input_stream : c_ptr(c_fn_ptr);
    var get_output_stream : c_ptr(c_fn_ptr);
    var close_fn : c_ptr(c_fn_ptr);
    var close_async : c_ptr(c_fn_ptr);
    var close_finish : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
    var _g_reserved6 : c_ptr(c_fn_ptr);
    var _g_reserved7 : c_ptr(c_fn_ptr);
    var _g_reserved8 : c_ptr(c_fn_ptr);
    var _g_reserved9 : c_ptr(c_fn_ptr);
    var _g_reserved10 : c_ptr(c_fn_ptr);
  }

  extern "struct _GFileIOStream" record _GFileIOStream {    var parent_instance : GIOStream;
    var priv : c_ptr(GFileIOStreamPrivate);
  }

  extern "struct _GFileIOStreamClass" record _GFileIOStreamClass {    var parent_class : GIOStreamClass;
    var tell : c_ptr(c_fn_ptr);
    var can_seek : c_ptr(c_fn_ptr);
    var seek : c_ptr(c_fn_ptr);
    var can_truncate : c_ptr(c_fn_ptr);
    var truncate_fn : c_ptr(c_fn_ptr);
    var query_info : c_ptr(c_fn_ptr);
    var query_info_async : c_ptr(c_fn_ptr);
    var query_info_finish : c_ptr(c_fn_ptr);
    var get_etag : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GFileMonitor" record _GFileMonitor {    var parent_instance : GObject;
    var priv : c_ptr(GFileMonitorPrivate);
  }

  extern "struct _GFileMonitorClass" record _GFileMonitorClass {    var parent_class : GObjectClass;
    var changed : c_ptr(c_fn_ptr);
    var cancel : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GFilenameCompleterClass" record _GFilenameCompleterClass {    var parent_class : GObjectClass;
    var got_completion_data : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
  }

  extern "struct _GFileOutputStream" record _GFileOutputStream {    var parent_instance : GOutputStream;
    var priv : c_ptr(GFileOutputStreamPrivate);
  }

  extern "struct _GFileOutputStreamClass" record _GFileOutputStreamClass {    var parent_class : GOutputStreamClass;
    var tell : c_ptr(c_fn_ptr);
    var can_seek : c_ptr(c_fn_ptr);
    var seek : c_ptr(c_fn_ptr);
    var can_truncate : c_ptr(c_fn_ptr);
    var truncate_fn : c_ptr(c_fn_ptr);
    var query_info : c_ptr(c_fn_ptr);
    var query_info_async : c_ptr(c_fn_ptr);
    var query_info_finish : c_ptr(c_fn_ptr);
    var get_etag : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GInetAddress" record _GInetAddress {    var parent_instance : GObject;
    var priv : c_ptr(GInetAddressPrivate);
  }

  extern "struct _GInetAddressClass" record _GInetAddressClass {    var parent_class : GObjectClass;
    var to_string : c_ptr(c_fn_ptr);
    var to_bytes : c_ptr(c_fn_ptr);
  }

  extern "struct _GInetAddressMask" record _GInetAddressMask {    var parent_instance : GObject;
    var priv : c_ptr(GInetAddressMaskPrivate);
  }

  extern "struct _GInetAddressMaskClass" record _GInetAddressMaskClass {    var parent_class : GObjectClass;
  }

  extern "struct _GSocketAddress" record _GSocketAddress {    var parent_instance : GObject;
  }

  extern "struct _GSocketAddressClass" record _GSocketAddressClass {    var parent_class : GObjectClass;
    var get_family : c_ptr(c_fn_ptr);
    var get_native_size : c_ptr(c_fn_ptr);
    var to_native : c_ptr(c_fn_ptr);
  }

  extern "struct _GInetSocketAddress" record _GInetSocketAddress {    var parent_instance : GSocketAddress;
    var priv : c_ptr(GInetSocketAddressPrivate);
  }

  extern "struct _GInetSocketAddressClass" record _GInetSocketAddressClass {    var parent_class : GSocketAddressClass;
  }

  extern "struct _GListModelInterface" record _GListModelInterface {    var g_iface : GTypeInterface;
    var get_item_type : c_ptr(c_fn_ptr);
    var get_n_items : c_ptr(c_fn_ptr);
    var get_item : c_ptr(c_fn_ptr);
  }

  extern "struct _GLoadableIconIface" record _GLoadableIconIface {    var g_iface : GTypeInterface;
    var load : c_ptr(c_fn_ptr);
    var load_async : c_ptr(c_fn_ptr);
    var load_finish : c_ptr(c_fn_ptr);
  }

  extern "struct _GMemoryInputStream" record _GMemoryInputStream {    var parent_instance : GInputStream;
    var priv : c_ptr(GMemoryInputStreamPrivate);
  }

  extern "struct _GMemoryInputStreamClass" record _GMemoryInputStreamClass {    var parent_class : GInputStreamClass;
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GMemoryMonitorInterface" record _GMemoryMonitorInterface {    var g_iface : GTypeInterface;
    var low_memory_warning : c_ptr(c_fn_ptr);
  }

  extern "struct _GMemoryOutputStream" record _GMemoryOutputStream {    var parent_instance : GOutputStream;
    var priv : c_ptr(GMemoryOutputStreamPrivate);
  }

  extern "struct _GMemoryOutputStreamClass" record _GMemoryOutputStreamClass {    var parent_class : GOutputStreamClass;
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GMenuModel" record _GMenuModel {    var parent_instance : GObject;
    var priv : c_ptr(GMenuModelPrivate);
  }

  extern "struct _GMenuModelClass" record _GMenuModelClass {    var parent_class : GObjectClass;
    var is_mutable : c_ptr(c_fn_ptr);
    var get_n_items : c_ptr(c_fn_ptr);
    var get_item_attributes : c_ptr(c_fn_ptr);
    var iterate_item_attributes : c_ptr(c_fn_ptr);
    var get_item_attribute_value : c_ptr(c_fn_ptr);
    var get_item_links : c_ptr(c_fn_ptr);
    var iterate_item_links : c_ptr(c_fn_ptr);
    var get_item_link : c_ptr(c_fn_ptr);
  }

  extern "struct _GMenuAttributeIter" record _GMenuAttributeIter {    var parent_instance : GObject;
    var priv : c_ptr(GMenuAttributeIterPrivate);
  }

  extern "struct _GMenuAttributeIterClass" record _GMenuAttributeIterClass {    var parent_class : GObjectClass;
    var get_next : c_ptr(c_fn_ptr);
  }

  extern "struct _GMenuLinkIter" record _GMenuLinkIter {    var parent_instance : GObject;
    var priv : c_ptr(GMenuLinkIterPrivate);
  }

  extern "struct _GMenuLinkIterClass" record _GMenuLinkIterClass {    var parent_class : GObjectClass;
    var get_next : c_ptr(c_fn_ptr);
  }

  extern "struct _GMountIface" record _GMountIface {    var g_iface : GTypeInterface;
    var changed : c_ptr(c_fn_ptr);
    var unmounted : c_ptr(c_fn_ptr);
    var get_root : c_ptr(c_fn_ptr);
    var get_name : c_ptr(c_fn_ptr);
    var get_icon : c_ptr(c_fn_ptr);
    var get_uuid : c_ptr(c_fn_ptr);
    var get_volume : c_ptr(c_fn_ptr);
    var get_drive : c_ptr(c_fn_ptr);
    var can_unmount : c_ptr(c_fn_ptr);
    var can_eject : c_ptr(c_fn_ptr);
    var unmount : c_ptr(c_fn_ptr);
    var unmount_finish : c_ptr(c_fn_ptr);
    var eject : c_ptr(c_fn_ptr);
    var eject_finish : c_ptr(c_fn_ptr);
    var remount : c_ptr(c_fn_ptr);
    var remount_finish : c_ptr(c_fn_ptr);
    var guess_content_type : c_ptr(c_fn_ptr);
    var guess_content_type_finish : c_ptr(c_fn_ptr);
    var guess_content_type_sync : c_ptr(c_fn_ptr);
    var pre_unmount : c_ptr(c_fn_ptr);
    var unmount_with_operation : c_ptr(c_fn_ptr);
    var unmount_with_operation_finish : c_ptr(c_fn_ptr);
    var eject_with_operation : c_ptr(c_fn_ptr);
    var eject_with_operation_finish : c_ptr(c_fn_ptr);
    var get_default_location : c_ptr(c_fn_ptr);
    var get_sort_key : c_ptr(c_fn_ptr);
    var get_symbolic_icon : c_ptr(c_fn_ptr);
  }

  extern "struct _GMountOperation" record _GMountOperation {    var parent_instance : GObject;
    var priv : c_ptr(GMountOperationPrivate);
  }

  extern "struct _GMountOperationClass" record _GMountOperationClass {    var parent_class : GObjectClass;
    var ask_password : c_ptr(c_fn_ptr);
    var ask_question : c_ptr(c_fn_ptr);
    var reply : c_ptr(c_fn_ptr);
    var aborted : c_ptr(c_fn_ptr);
    var show_processes : c_ptr(c_fn_ptr);
    var show_unmount_progress : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
    var _g_reserved6 : c_ptr(c_fn_ptr);
    var _g_reserved7 : c_ptr(c_fn_ptr);
    var _g_reserved8 : c_ptr(c_fn_ptr);
    var _g_reserved9 : c_ptr(c_fn_ptr);
  }

  extern "struct _GNativeSocketAddress" record _GNativeSocketAddress {    var parent_instance : GSocketAddress;
    var priv : c_ptr(GNativeSocketAddressPrivate);
  }

  extern "struct _GNativeSocketAddressClass" record _GNativeSocketAddressClass {    var parent_class : GSocketAddressClass;
  }

  extern "struct _GVolumeMonitor" record _GVolumeMonitor {    var parent_instance : GObject;
    var priv : gpointer;
  }

  extern "struct _GVolumeMonitorClass" record _GVolumeMonitorClass {    var parent_class : GObjectClass;
    var volume_added : c_ptr(c_fn_ptr);
    var volume_removed : c_ptr(c_fn_ptr);
    var volume_changed : c_ptr(c_fn_ptr);
    var mount_added : c_ptr(c_fn_ptr);
    var mount_removed : c_ptr(c_fn_ptr);
    var mount_pre_unmount : c_ptr(c_fn_ptr);
    var mount_changed : c_ptr(c_fn_ptr);
    var drive_connected : c_ptr(c_fn_ptr);
    var drive_disconnected : c_ptr(c_fn_ptr);
    var drive_changed : c_ptr(c_fn_ptr);
    var is_supported : c_ptr(c_fn_ptr);
    var get_connected_drives : c_ptr(c_fn_ptr);
    var get_volumes : c_ptr(c_fn_ptr);
    var get_mounts : c_ptr(c_fn_ptr);
    var get_volume_for_uuid : c_ptr(c_fn_ptr);
    var get_mount_for_uuid : c_ptr(c_fn_ptr);
    var adopt_orphan_mount : c_ptr(c_fn_ptr);
    var drive_eject_button : c_ptr(c_fn_ptr);
    var drive_stop_button : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
    var _g_reserved6 : c_ptr(c_fn_ptr);
  }

  extern "struct _GNativeVolumeMonitor" record _GNativeVolumeMonitor {    var parent_instance : GVolumeMonitor;
  }

  extern "struct _GNativeVolumeMonitorClass" record _GNativeVolumeMonitorClass {    var parent_class : GVolumeMonitorClass;
    var get_mount_for_mount_path : c_ptr(c_fn_ptr);
  }

  extern "struct _GNetworkAddress" record _GNetworkAddress {    var parent_instance : GObject;
    var priv : c_ptr(GNetworkAddressPrivate);
  }

  extern "struct _GNetworkAddressClass" record _GNetworkAddressClass {    var parent_class : GObjectClass;
  }

  extern "struct _GNetworkMonitorInterface" record _GNetworkMonitorInterface {    var g_iface : GTypeInterface;
    var network_changed : c_ptr(c_fn_ptr);
    var can_reach : c_ptr(c_fn_ptr);
    var can_reach_async : c_ptr(c_fn_ptr);
    var can_reach_finish : c_ptr(c_fn_ptr);
  }

  extern "struct _GNetworkService" record _GNetworkService {    var parent_instance : GObject;
    var priv : c_ptr(GNetworkServicePrivate);
  }

  extern "struct _GNetworkServiceClass" record _GNetworkServiceClass {    var parent_class : GObjectClass;
  }

  extern "struct _GPermission" record _GPermission {    var parent_instance : GObject;
    var priv : c_ptr(GPermissionPrivate);
  }

  extern "struct _GPermissionClass" record _GPermissionClass {    var parent_class : GObjectClass;
    var acquire : c_ptr(c_fn_ptr);
    var acquire_async : c_ptr(c_fn_ptr);
    var acquire_finish : c_ptr(c_fn_ptr);
    var release : c_ptr(c_fn_ptr);
    var release_async : c_ptr(c_fn_ptr);
    var release_finish : c_ptr(c_fn_ptr);
    var reserved : c_ptr(gpointer);
  }

  extern "struct _GPollableInputStreamInterface" record _GPollableInputStreamInterface {    var g_iface : GTypeInterface;
    var can_poll : c_ptr(c_fn_ptr);
    var is_readable : c_ptr(c_fn_ptr);
    var create_source : c_ptr(c_fn_ptr);
    var read_nonblocking : c_ptr(c_fn_ptr);
  }

  extern "struct _GPollableOutputStreamInterface" record _GPollableOutputStreamInterface {    var g_iface : GTypeInterface;
    var can_poll : c_ptr(c_fn_ptr);
    var is_writable : c_ptr(c_fn_ptr);
    var create_source : c_ptr(c_fn_ptr);
    var write_nonblocking : c_ptr(c_fn_ptr);
    var writev_nonblocking : c_ptr(c_fn_ptr);
  }

  extern "struct _GProxyInterface" record _GProxyInterface {    var g_iface : GTypeInterface;
    var connect : c_ptr(c_fn_ptr);
    var connect_async : c_ptr(c_fn_ptr);
    var connect_finish : c_ptr(c_fn_ptr);
    var supports_hostname : c_ptr(c_fn_ptr);
  }

  extern "struct _GProxyAddress" record _GProxyAddress {    var parent_instance : GInetSocketAddress;
    var priv : c_ptr(GProxyAddressPrivate);
  }

  extern "struct _GProxyAddressClass" record _GProxyAddressClass {    var parent_class : GInetSocketAddressClass;
  }

  extern "struct _GSocketAddressEnumerator" record _GSocketAddressEnumerator {    var parent_instance : GObject;
  }

  extern "struct _GSocketAddressEnumeratorClass" record _GSocketAddressEnumeratorClass {    var parent_class : GObjectClass;
    var next : c_ptr(c_fn_ptr);
    var next_async : c_ptr(c_fn_ptr);
    var next_finish : c_ptr(c_fn_ptr);
  }

  extern "struct _GProxyAddressEnumerator" record _GProxyAddressEnumerator {    var parent_instance : GSocketAddressEnumerator;
    var priv : c_ptr(GProxyAddressEnumeratorPrivate);
  }

  extern "struct _GProxyAddressEnumeratorClass" record _GProxyAddressEnumeratorClass {    var parent_class : GSocketAddressEnumeratorClass;
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
    var _g_reserved6 : c_ptr(c_fn_ptr);
    var _g_reserved7 : c_ptr(c_fn_ptr);
  }

  extern "struct _GProxyResolverInterface" record _GProxyResolverInterface {    var g_iface : GTypeInterface;
    var is_supported : c_ptr(c_fn_ptr);
    var lookup : c_ptr(c_fn_ptr);
    var lookup_async : c_ptr(c_fn_ptr);
    var lookup_finish : c_ptr(c_fn_ptr);
  }

  extern "struct _GRemoteActionGroupInterface" record _GRemoteActionGroupInterface {    var g_iface : GTypeInterface;
    var activate_action_full : c_ptr(c_fn_ptr);
    var change_action_state_full : c_ptr(c_fn_ptr);
  }

  extern "struct _GResolver" record _GResolver {    var parent_instance : GObject;
    var priv : c_ptr(GResolverPrivate);
  }

  extern "struct _GResolverClass" record _GResolverClass {    var parent_class : GObjectClass;
    var reload : c_ptr(c_fn_ptr);
    var lookup_by_name : c_ptr(c_fn_ptr);
    var lookup_by_name_async : c_ptr(c_fn_ptr);
    var lookup_by_name_finish : c_ptr(c_fn_ptr);
    var lookup_by_address : c_ptr(c_fn_ptr);
    var lookup_by_address_async : c_ptr(c_fn_ptr);
    var lookup_by_address_finish : c_ptr(c_fn_ptr);
    var lookup_service : c_ptr(c_fn_ptr);
    var lookup_service_async : c_ptr(c_fn_ptr);
    var lookup_service_finish : c_ptr(c_fn_ptr);
    var lookup_records : c_ptr(c_fn_ptr);
    var lookup_records_async : c_ptr(c_fn_ptr);
    var lookup_records_finish : c_ptr(c_fn_ptr);
    var lookup_by_name_with_flags_async : c_ptr(c_fn_ptr);
    var lookup_by_name_with_flags_finish : c_ptr(c_fn_ptr);
    var lookup_by_name_with_flags : c_ptr(c_fn_ptr);
  }

  extern "struct _GStaticResource" record _GStaticResource {    var data : c_ptr(guint8);
    var data_len : gsize;
    var resource : c_ptr(GResource);
    var next : c_ptr(_GStaticResource);
    var padding : gpointer;
  }

  extern "struct _GSeekableIface" record _GSeekableIface {    var g_iface : GTypeInterface;
    var tell : c_ptr(c_fn_ptr);
    var can_seek : c_ptr(c_fn_ptr);
    var seek : c_ptr(c_fn_ptr);
    var can_truncate : c_ptr(c_fn_ptr);
    var truncate_fn : c_ptr(c_fn_ptr);
  }

  extern "struct _GSettingsClass" record _GSettingsClass {    var parent_class : GObjectClass;
    var writable_changed : c_ptr(c_fn_ptr);
    var changed : c_ptr(c_fn_ptr);
    var writable_change_event : c_ptr(c_fn_ptr);
    var change_event : c_ptr(c_fn_ptr);
    var padding : c_ptr(gpointer);
  }

  extern "struct _GSettings" record _GSettings {    var parent_instance : GObject;
    var priv : c_ptr(GSettingsPrivate);
  }

  extern "struct _GSimpleActionGroup" record _GSimpleActionGroup {    var parent_instance : GObject;
    var priv : c_ptr(GSimpleActionGroupPrivate);
  }

  extern "struct _GSimpleActionGroupClass" record _GSimpleActionGroupClass {    var parent_class : GObjectClass;
    var padding : c_ptr(gpointer);
  }

  extern "struct _GSimpleProxyResolver" record _GSimpleProxyResolver {    var parent_instance : GObject;
    var priv : c_ptr(GSimpleProxyResolverPrivate);
  }

  extern "struct _GSimpleProxyResolverClass" record _GSimpleProxyResolverClass {    var parent_class : GObjectClass;
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GSocketClass" record _GSocketClass {    var parent_class : GObjectClass;
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
    var _g_reserved6 : c_ptr(c_fn_ptr);
    var _g_reserved7 : c_ptr(c_fn_ptr);
    var _g_reserved8 : c_ptr(c_fn_ptr);
    var _g_reserved9 : c_ptr(c_fn_ptr);
    var _g_reserved10 : c_ptr(c_fn_ptr);
  }

  extern "struct _GSocket" record _GSocket {    var parent_instance : GObject;
    var priv : c_ptr(GSocketPrivate);
  }

  extern "struct _GSocketClientClass" record _GSocketClientClass {    var parent_class : GObjectClass;
    var event : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
  }

  extern "struct _GSocketClient" record _GSocketClient {    var parent_instance : GObject;
    var priv : c_ptr(GSocketClientPrivate);
  }

  extern "struct _GSocketConnectableIface" record _GSocketConnectableIface {    var g_iface : GTypeInterface;
    var enumerate : c_ptr(c_fn_ptr);
    var proxy_enumerate : c_ptr(c_fn_ptr);
    var to_string : c_ptr(c_fn_ptr);
  }

  extern "struct _GSocketConnectionClass" record _GSocketConnectionClass {    var parent_class : GIOStreamClass;
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
    var _g_reserved6 : c_ptr(c_fn_ptr);
  }

  extern "struct _GSocketConnection" record _GSocketConnection {    var parent_instance : GIOStream;
    var priv : c_ptr(GSocketConnectionPrivate);
  }

  extern "struct _GSocketControlMessageClass" record _GSocketControlMessageClass {    var parent_class : GObjectClass;
    var get_size : c_ptr(c_fn_ptr);
    var get_level : c_ptr(c_fn_ptr);
    var get_type : c_ptr(c_fn_ptr);
    var serialize : c_ptr(c_fn_ptr);
    var deserialize : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GSocketControlMessage" record _GSocketControlMessage {    var parent_instance : GObject;
    var priv : c_ptr(GSocketControlMessagePrivate);
  }

  extern "struct _GSocketListenerClass" record _GSocketListenerClass {    var parent_class : GObjectClass;
    var changed : c_ptr(c_fn_ptr);
    var event : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
    var _g_reserved6 : c_ptr(c_fn_ptr);
  }

  extern "struct _GSocketListener" record _GSocketListener {    var parent_instance : GObject;
    var priv : c_ptr(GSocketListenerPrivate);
  }

  extern "struct _GSocketServiceClass" record _GSocketServiceClass {    var parent_class : GSocketListenerClass;
    var incoming : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
    var _g_reserved6 : c_ptr(c_fn_ptr);
  }

  extern "struct _GSocketService" record _GSocketService {    var parent_instance : GSocketListener;
    var priv : c_ptr(GSocketServicePrivate);
  }

  extern "struct _GTcpConnectionClass" record _GTcpConnectionClass {    var parent_class : GSocketConnectionClass;
  }

  extern "struct _GTcpConnection" record _GTcpConnection {    var parent_instance : GSocketConnection;
    var priv : c_ptr(GTcpConnectionPrivate);
  }

  extern "struct _GTcpWrapperConnectionClass" record _GTcpWrapperConnectionClass {    var parent_class : GTcpConnectionClass;
  }

  extern "struct _GTcpWrapperConnection" record _GTcpWrapperConnection {    var parent_instance : GTcpConnection;
    var priv : c_ptr(GTcpWrapperConnectionPrivate);
  }

  extern "struct _GThreadedSocketServiceClass" record _GThreadedSocketServiceClass {    var parent_class : GSocketServiceClass;
    var run : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
  }

  extern "struct _GThreadedSocketService" record _GThreadedSocketService {    var parent_instance : GSocketService;
    var priv : c_ptr(GThreadedSocketServicePrivate);
  }

  extern "struct _GTlsBackendInterface" record _GTlsBackendInterface {    var g_iface : GTypeInterface;
    var supports_tls : c_ptr(c_fn_ptr);
    var get_certificate_type : c_ptr(c_fn_ptr);
    var get_client_connection_type : c_ptr(c_fn_ptr);
    var get_server_connection_type : c_ptr(c_fn_ptr);
    var get_file_database_type : c_ptr(c_fn_ptr);
    var get_default_database : c_ptr(c_fn_ptr);
    var supports_dtls : c_ptr(c_fn_ptr);
    var get_dtls_client_connection_type : c_ptr(c_fn_ptr);
    var get_dtls_server_connection_type : c_ptr(c_fn_ptr);
  }

  extern "struct _GTlsCertificate" record _GTlsCertificate {    var parent_instance : GObject;
    var priv : c_ptr(GTlsCertificatePrivate);
  }

  extern "struct _GTlsCertificateClass" record _GTlsCertificateClass {    var parent_class : GObjectClass;
    var verify : c_ptr(c_fn_ptr);
    var padding : c_ptr(gpointer);
  }

  extern "struct _GTlsConnection" record _GTlsConnection {    var parent_instance : GIOStream;
    var priv : c_ptr(GTlsConnectionPrivate);
  }

  extern "struct _GTlsConnectionClass" record _GTlsConnectionClass {    var parent_class : GIOStreamClass;
    var accept_certificate : c_ptr(c_fn_ptr);
    var handshake : c_ptr(c_fn_ptr);
    var handshake_async : c_ptr(c_fn_ptr);
    var handshake_finish : c_ptr(c_fn_ptr);
  }

  extern "struct _GTlsClientConnectionInterface" record _GTlsClientConnectionInterface {    var g_iface : GTypeInterface;
    var copy_session_state : c_ptr(c_fn_ptr);
  }

  extern "struct _GTlsDatabase" record _GTlsDatabase {    var parent_instance : GObject;
    var priv : c_ptr(GTlsDatabasePrivate);
  }

  extern "struct _GTlsDatabaseClass" record _GTlsDatabaseClass {    var parent_class : GObjectClass;
    var verify_chain : c_ptr(c_fn_ptr);
    var verify_chain_async : c_ptr(c_fn_ptr);
    var verify_chain_finish : c_ptr(c_fn_ptr);
    var create_certificate_handle : c_ptr(c_fn_ptr);
    var lookup_certificate_for_handle : c_ptr(c_fn_ptr);
    var lookup_certificate_for_handle_async : c_ptr(c_fn_ptr);
    var lookup_certificate_for_handle_finish : c_ptr(c_fn_ptr);
    var lookup_certificate_issuer : c_ptr(c_fn_ptr);
    var lookup_certificate_issuer_async : c_ptr(c_fn_ptr);
    var lookup_certificate_issuer_finish : c_ptr(c_fn_ptr);
    var lookup_certificates_issued_by : c_ptr(c_fn_ptr);
    var lookup_certificates_issued_by_async : c_ptr(c_fn_ptr);
    var lookup_certificates_issued_by_finish : c_ptr(c_fn_ptr);
    var padding : c_ptr(gpointer);
  }

  extern "struct _GTlsFileDatabaseInterface" record _GTlsFileDatabaseInterface {    var g_iface : GTypeInterface;
    var padding : c_ptr(gpointer);
  }

  extern "struct _GTlsInteraction" record _GTlsInteraction {    var parent_instance : GObject;
    var priv : c_ptr(GTlsInteractionPrivate);
  }

  extern "struct _GTlsInteractionClass" record _GTlsInteractionClass {    var parent_class : GObjectClass;
    var ask_password : c_ptr(c_fn_ptr);
    var ask_password_async : c_ptr(c_fn_ptr);
    var ask_password_finish : c_ptr(c_fn_ptr);
    var request_certificate : c_ptr(c_fn_ptr);
    var request_certificate_async : c_ptr(c_fn_ptr);
    var request_certificate_finish : c_ptr(c_fn_ptr);
    var padding : c_ptr(gpointer);
  }

  extern "struct _GTlsPassword" record _GTlsPassword {    var parent_instance : GObject;
    var priv : c_ptr(GTlsPasswordPrivate);
  }

  extern "struct _GTlsPasswordClass" record _GTlsPasswordClass {    var parent_class : GObjectClass;
    var get_value : c_ptr(c_fn_ptr);
    var set_value : c_ptr(c_fn_ptr);
    var get_default_warning : c_ptr(c_fn_ptr);
    var padding : c_ptr(gpointer);
  }

  extern "struct _GTlsServerConnectionInterface" record _GTlsServerConnectionInterface {    var g_iface : GTypeInterface;
  }

  extern "struct _GVfs" record _GVfs {    var parent_instance : GObject;
  }

  extern "struct _GVfsClass" record _GVfsClass {    var parent_class : GObjectClass;
    var is_active : c_ptr(c_fn_ptr);
    var get_file_for_path : c_ptr(c_fn_ptr);
    var get_file_for_uri : c_ptr(c_fn_ptr);
    var get_supported_uri_schemes : c_ptr(c_fn_ptr);
    var parse_name : c_ptr(c_fn_ptr);
    var local_file_add_info : c_ptr(c_fn_ptr);
    var add_writable_namespaces : c_ptr(c_fn_ptr);
    var local_file_set_attributes : c_ptr(c_fn_ptr);
    var local_file_removed : c_ptr(c_fn_ptr);
    var local_file_moved : c_ptr(c_fn_ptr);
    var deserialize_icon : c_ptr(c_fn_ptr);
    var _g_reserved1 : c_ptr(c_fn_ptr);
    var _g_reserved2 : c_ptr(c_fn_ptr);
    var _g_reserved3 : c_ptr(c_fn_ptr);
    var _g_reserved4 : c_ptr(c_fn_ptr);
    var _g_reserved5 : c_ptr(c_fn_ptr);
    var _g_reserved6 : c_ptr(c_fn_ptr);
  }

  extern "struct _GVolumeIface" record _GVolumeIface {    var g_iface : GTypeInterface;
    var changed : c_ptr(c_fn_ptr);
    var removed : c_ptr(c_fn_ptr);
    var get_name : c_ptr(c_fn_ptr);
    var get_icon : c_ptr(c_fn_ptr);
    var get_uuid : c_ptr(c_fn_ptr);
    var get_drive : c_ptr(c_fn_ptr);
    var get_mount : c_ptr(c_fn_ptr);
    var can_mount : c_ptr(c_fn_ptr);
    var can_eject : c_ptr(c_fn_ptr);
    var mount_fn : c_ptr(c_fn_ptr);
    var mount_finish : c_ptr(c_fn_ptr);
    var eject : c_ptr(c_fn_ptr);
    var eject_finish : c_ptr(c_fn_ptr);
    var get_identifier : c_ptr(c_fn_ptr);
    var enumerate_identifiers : c_ptr(c_fn_ptr);
    var should_automount : c_ptr(c_fn_ptr);
    var get_activation_root : c_ptr(c_fn_ptr);
    var eject_with_operation : c_ptr(c_fn_ptr);
    var eject_with_operation_finish : c_ptr(c_fn_ptr);
    var get_sort_key : c_ptr(c_fn_ptr);
    var get_symbolic_icon : c_ptr(c_fn_ptr);
  }

  extern "struct _GZlibCompressorClass" record _GZlibCompressorClass {    var parent_class : GObjectClass;
  }

  extern "struct _GZlibDecompressorClass" record _GZlibDecompressorClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowTensor" record _GArrowTensor {    var parent_instance : GObject;
  }

  extern "struct _GArrowTensorClass" record _GArrowTensorClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowOutputStream" record _GArrowOutputStream {    var parent_instance : GObject;
  }

  extern "struct _GArrowOutputStreamClass" record _GArrowOutputStreamClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowFileOutputStream" record _GArrowFileOutputStream {    var parent_instance : GArrowOutputStream;
  }

  extern "struct _GArrowFileOutputStreamClass" record _GArrowFileOutputStreamClass {    var parent_class : GArrowOutputStreamClass;
  }

  extern "struct _GArrowBufferOutputStream" record _GArrowBufferOutputStream {    var parent_instance : GArrowOutputStream;
  }

  extern "struct _GArrowBufferOutputStreamClass" record _GArrowBufferOutputStreamClass {    var parent_class : GArrowOutputStreamClass;
  }

  extern "struct _GArrowGIOOutputStream" record _GArrowGIOOutputStream {    var parent_instance : GArrowOutputStream;
  }

  extern "struct _GArrowGIOOutputStreamClass" record _GArrowGIOOutputStreamClass {    var parent_class : GArrowOutputStreamClass;
  }

  extern "struct _GArrowCompressedOutputStream" record _GArrowCompressedOutputStream {    var parent_instance : GArrowOutputStream;
  }

  extern "struct _GArrowCompressedOutputStreamClass" record _GArrowCompressedOutputStreamClass {    var parent_class : GArrowOutputStreamClass;
  }

  extern "struct _GArrowTable" record _GArrowTable {    var parent_instance : GObject;
  }

  extern "struct _GArrowTableClass" record _GArrowTableClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowFeatherWriteProperties" record _GArrowFeatherWriteProperties {    var parent_instance : GObject;
  }

  extern "struct _GArrowFeatherWritePropertiesClass" record _GArrowFeatherWritePropertiesClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowDatum" record _GArrowDatum {    var parent_instance : GObject;
  }

  extern "struct _GArrowDatumClass" record _GArrowDatumClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowScalarDatum" record _GArrowScalarDatum {    var parent_instance : GArrowDatum;
  }

  extern "struct _GArrowScalarDatumClass" record _GArrowScalarDatumClass {    var parent_class : GArrowDatumClass;
  }

  extern "struct _GArrowArrayDatum" record _GArrowArrayDatum {    var parent_instance : GArrowDatum;
  }

  extern "struct _GArrowArrayDatumClass" record _GArrowArrayDatumClass {    var parent_class : GArrowDatumClass;
  }

  extern "struct _GArrowChunkedArrayDatum" record _GArrowChunkedArrayDatum {    var parent_instance : GArrowDatum;
  }

  extern "struct _GArrowChunkedArrayDatumClass" record _GArrowChunkedArrayDatumClass {    var parent_class : GArrowDatumClass;
  }

  extern "struct _GArrowRecordBatchDatum" record _GArrowRecordBatchDatum {    var parent_instance : GArrowDatum;
  }

  extern "struct _GArrowRecordBatchDatumClass" record _GArrowRecordBatchDatumClass {    var parent_class : GArrowDatumClass;
  }

  extern "struct _GArrowTableDatum" record _GArrowTableDatum {    var parent_instance : GArrowDatum;
  }

  extern "struct _GArrowTableDatumClass" record _GArrowTableDatumClass {    var parent_class : GArrowDatumClass;
  }

  extern "struct _GArrowExecuteContext" record _GArrowExecuteContext {    var parent_instance : GObject;
  }

  extern "struct _GArrowExecuteContextClass" record _GArrowExecuteContextClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowFunction" record _GArrowFunction {    var parent_instance : GObject;
  }

  extern "struct _GArrowFunctionClass" record _GArrowFunctionClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowCastOptions" record _GArrowCastOptions {    var parent_instance : GObject;
  }

  extern "struct _GArrowCastOptionsClass" record _GArrowCastOptionsClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowScalarAggregateOptions" record _GArrowScalarAggregateOptions {    var parent_instance : GObject;
  }

  extern "struct _GArrowScalarAggregateOptionsClass" record _GArrowScalarAggregateOptionsClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowFilterOptions" record _GArrowFilterOptions {    var parent_instance : GObject;
  }

  extern "struct _GArrowFilterOptionsClass" record _GArrowFilterOptionsClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowTakeOptions" record _GArrowTakeOptions {    var parent_instance : GObject;
  }

  extern "struct _GArrowTakeOptionsClass" record _GArrowTakeOptionsClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowArraySortOptions" record _GArrowArraySortOptions {    var parent_instance : GObject;
  }

  extern "struct _GArrowArraySortOptionsClass" record _GArrowArraySortOptionsClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowSortKey" record _GArrowSortKey {    var parent_instance : GObject;
  }

  extern "struct _GArrowSortKeyClass" record _GArrowSortKeyClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowSortOptions" record _GArrowSortOptions {    var parent_instance : GObject;
  }

  extern "struct _GArrowSortOptionsClass" record _GArrowSortOptionsClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowRecordBatchBuilder" record _GArrowRecordBatchBuilder {    var parent_instance : GObject;
  }

  extern "struct _GArrowRecordBatchBuilderClass" record _GArrowRecordBatchBuilderClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowInputStream" record _GArrowInputStream {    var parent_instance : GInputStream;
  }

  extern "struct _GArrowInputStreamClass" record _GArrowInputStreamClass {    var parent_class : GInputStreamClass;
  }

  extern "struct _GArrowSeekableInputStream" record _GArrowSeekableInputStream {    var parent_instance : GArrowInputStream;
  }

  extern "struct _GArrowSeekableInputStreamClass" record _GArrowSeekableInputStreamClass {    var parent_class : GArrowInputStreamClass;
  }

  extern "struct _GArrowBufferInputStream" record _GArrowBufferInputStream {    var parent_instance : GArrowSeekableInputStream;
  }

  extern "struct _GArrowBufferInputStreamClass" record _GArrowBufferInputStreamClass {    var parent_class : GArrowSeekableInputStreamClass;
  }

  extern "struct _GArrowMemoryMappedInputStream" record _GArrowMemoryMappedInputStream {    var parent_instance : GArrowSeekableInputStream;
  }

  extern "struct _GArrowMemoryMappedInputStreamClass" record _GArrowMemoryMappedInputStreamClass {    var parent_class : GArrowSeekableInputStreamClass;
  }

  extern "struct _GArrowGIOInputStream" record _GArrowGIOInputStream {    var parent_instance : GArrowSeekableInputStream;
  }

  extern "struct _GArrowGIOInputStreamClass" record _GArrowGIOInputStreamClass {    var parent_class : GArrowSeekableInputStreamClass;
  }

  extern "struct _GArrowCompressedInputStream" record _GArrowCompressedInputStream {    var parent_instance : GArrowInputStream;
  }

  extern "struct _GArrowCompressedInputStreamClass" record _GArrowCompressedInputStreamClass {    var parent_class : GArrowInputStreamClass;
  }

  extern "struct _GArrowRecordBatchReader" record _GArrowRecordBatchReader {    var parent_instance : GObject;
  }

  extern "struct _GArrowRecordBatchReaderClass" record _GArrowRecordBatchReaderClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowTableBatchReader" record _GArrowTableBatchReader {    var parent_instance : GArrowRecordBatchReader;
  }

  extern "struct _GArrowTableBatchReaderClass" record _GArrowTableBatchReaderClass {    var parent_class : GArrowRecordBatchReaderClass;
  }

  extern "struct _GArrowRecordBatchStreamReader" record _GArrowRecordBatchStreamReader {    var parent_instance : GArrowRecordBatchReader;
  }

  extern "struct _GArrowRecordBatchStreamReaderClass" record _GArrowRecordBatchStreamReaderClass {    var parent_class : GArrowRecordBatchReaderClass;
  }

  extern "struct _GArrowRecordBatchFileReader" record _GArrowRecordBatchFileReader {    var parent_instance : GObject;
  }

  extern "struct _GArrowRecordBatchFileReaderClass" record _GArrowRecordBatchFileReaderClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowFeatherFileReader" record _GArrowFeatherFileReader {    var parent_instance : GObject;
  }

  extern "struct _GArrowFeatherFileReaderClass" record _GArrowFeatherFileReaderClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowCSVReadOptions" record _GArrowCSVReadOptions {    var parent_instance : GObject;
  }

  extern "struct _GArrowCSVReadOptionsClass" record _GArrowCSVReadOptionsClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowCSVReader" record _GArrowCSVReader {    var parent_instance : GObject;
  }

  extern "struct _GArrowCSVReaderClass" record _GArrowCSVReaderClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowJSONReadOptions" record _GArrowJSONReadOptions {    var parent_instance : GObject;
  }

  extern "struct _GArrowJSONReadOptionsClass" record _GArrowJSONReadOptionsClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowJSONReader" record _GArrowJSONReader {    var parent_instance : GObject;
  }

  extern "struct _GArrowJSONReaderClass" record _GArrowJSONReaderClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowRecordBatchWriter" record _GArrowRecordBatchWriter {    var parent_instance : GObject;
  }

  extern "struct _GArrowRecordBatchWriterClass" record _GArrowRecordBatchWriterClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowRecordBatchStreamWriter" record _GArrowRecordBatchStreamWriter {    var parent_instance : GArrowRecordBatchWriter;
  }

  extern "struct _GArrowRecordBatchStreamWriterClass" record _GArrowRecordBatchStreamWriterClass {    var parent_class : GArrowRecordBatchWriterClass;
  }

  extern "struct _GArrowRecordBatchFileWriter" record _GArrowRecordBatchFileWriter {    var parent_instance : GArrowRecordBatchStreamWriter;
  }

  extern "struct _GArrowRecordBatchFileWriterClass" record _GArrowRecordBatchFileWriterClass {    var parent_class : GArrowRecordBatchStreamWriterClass;
  }

  extern "struct _GArrowFileInfo" record _GArrowFileInfo {    var parent_instance : GObject;
  }

  extern "struct _GArrowFileInfoClass" record _GArrowFileInfoClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowFileSelector" record _GArrowFileSelector {    var parent_instance : GObject;
  }

  extern "struct _GArrowFileSelectorClass" record _GArrowFileSelectorClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowFileSystem" record _GArrowFileSystem {    var parent_instance : GObject;
  }

  extern "struct _GArrowFileSystemClass" record _GArrowFileSystemClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowSubTreeFileSystem" record _GArrowSubTreeFileSystem {    var parent_instance : GArrowFileSystem;
  }

  extern "struct _GArrowSubTreeFileSystemClass" record _GArrowSubTreeFileSystemClass {    var parent_class : GArrowFileSystemClass;
  }

  extern "struct _GArrowSlowFileSystem" record _GArrowSlowFileSystem {    var parent_instance : GArrowFileSystem;
  }

  extern "struct _GArrowSlowFileSystemClass" record _GArrowSlowFileSystemClass {    var parent_class : GArrowFileSystemClass;
  }

  extern "struct _GArrowMockFileSystem" record _GArrowMockFileSystem {    var parent_instance : GArrowFileSystem;
  }

  extern "struct _GArrowMockFileSystemClass" record _GArrowMockFileSystemClass {    var parent_class : GArrowFileSystemClass;
  }

  extern "struct _GArrowHDFSFileSystem" record _GArrowHDFSFileSystem {    var parent_instance : GArrowFileSystem;
  }

  extern "struct _GArrowHDFSFileSystemClass" record _GArrowHDFSFileSystemClass {    var parent_class : GArrowFileSystemClass;
  }

  extern "struct _GArrowS3FileSystem" record _GArrowS3FileSystem {    var parent_instance : GArrowFileSystem;
  }

  extern "struct _GArrowS3FileSystemClass" record _GArrowS3FileSystemClass {    var parent_class : GArrowFileSystemClass;
  }

  extern "struct _GArrowLocalFileSystemOptions" record _GArrowLocalFileSystemOptions {    var parent_instance : GObject;
  }

  extern "struct _GArrowLocalFileSystemOptionsClass" record _GArrowLocalFileSystemOptionsClass {    var parent_class : GObjectClass;
  }

  extern "struct _GArrowLocalFileSystem" record _GArrowLocalFileSystem {    var parent_instance : GArrowFileSystem;
  }

  extern "struct _GArrowLocalFileSystemClass" record _GArrowLocalFileSystemClass {    var parent_class : GArrowFileSystemClass;
  }

  extern "struct _GParquetArrowFileReader" record _GParquetArrowFileReader {    var parent_instance : GObject;
  }

  extern "struct _GParquetArrowFileReaderClass" record _GParquetArrowFileReaderClass {    var parent_class : GObjectClass;
  }

  extern "struct _GParquetWriterProperties" record _GParquetWriterProperties {    var parent_instance : GObject;
  }

  extern "struct _GParquetWriterPropertiesClass" record _GParquetWriterPropertiesClass {    var parent_class : GObjectClass;
  }

  extern "struct _GParquetArrowFileWriter" record _GParquetArrowFileWriter {    var parent_instance : GObject;
  }

  extern "struct _GParquetArrowFileWriterClass" record _GParquetArrowFileWriterClass {    var parent_class : GObjectClass;
  }

  extern record GAction {};

  extern record GActionGroup {};

  extern record GActionMap {};

  extern record GAllocator {};

  extern record GAppInfo {};

  extern record GAppInfoMonitor {};

  extern record GAppLaunchContextPrivate {};

  extern record GApplicationCommandLinePrivate {};

  extern record GApplicationPrivate {};

  extern record GArrowFile {};

  extern record GArrowFileInterface {};

  extern record GArrowFunctionOptions {};

  extern record GArrowFunctionOptionsInterface {};

  extern record GArrowReadable {};

  extern record GArrowReadableInterface {};

  extern record GArrowWritable {};

  extern record GArrowWritableFile {};

  extern record GArrowWritableFileInterface {};

  extern record GArrowWritableInterface {};

  
  // Note: Generated with fake std headers
  extern type time_t;
  extern type tm;
  extern type pid_t;
  extern type uid_t;
  extern type pthread_mutex_t;
  extern proc garrow_decimal128_get_type() : GType;

  // All types
  extern type GError = _GError;
  extern type GType;
  extern type GDestroyNotify;
  extern type GOnceStatus;
  extern type GHook;
  extern type GHookFinalizeFunc;
  extern type GSList;
  extern type GSourceCallbackFuncs;
  extern type GSourceFuncs;
  extern type GMainContext;
  extern type GSourcePrivate;
  extern type GSourceFunc;
  extern type GSourceDummyMarshal;
  extern type GIOFuncs;
  extern type GIConv;
  extern type GString;
  extern type GOptionArg;
  extern type GList;
  extern type GFunc;
  extern type GCompletionFunc;
  extern type GCompletionStrncmpFunc;
  extern type GThreadFunc;
  extern type GThreadPriority;
  extern type GStaticMutex;
  extern type GCond;
  extern type GTypeClass;
  extern type GBaseInitFunc;
  extern type GBaseFinalizeFunc;
  extern type GClassInitFunc;
  extern type GClassFinalizeFunc;
  extern type GInstanceInitFunc;
  extern type GTypeValueTable;
  extern type GTypeFundamentalFlags;
  extern type GInterfaceInitFunc;
  extern type GInterfaceFinalizeFunc;
  extern type GTypeInstance;
  extern type GParamFlags;
  extern type GData;
  extern type GValue;
  extern type GClosureNotify;
  extern type GClosureNotifyData;
  extern type GClosure;
  extern type GSignalFlags;
  extern type GParamSpec;
  extern type GEnumValue;
  extern type GFlagsValue;
  extern type GEnumClass;
  extern type GFlagsClass;
  extern type GObject;
  extern type GObjectClass;
  extern type GTypeInterface;
  extern type GTypePluginUse;
  extern type GTypePluginUnuse;
  extern type GTypePluginCompleteTypeInfo;
  extern type GTypePluginCompleteInterfaceInfo;
  extern type GBytes;
  extern type GHashTable;
  extern type GSocketAddress;
  extern type GInputVector;
  extern type GSocketControlMessage;
  extern type GOutputVector;
  extern type GInputStreamPrivate;
  extern type GInputStream;
  extern type GInputStreamClass;
  extern type GFilterInputStream;
  extern type GBufferedInputStreamPrivate;
  extern type GFilterInputStreamClass;
  extern type GOutputStreamPrivate;
  extern type GOutputStream;
  extern type GOutputStreamClass;
  extern type GFilterOutputStream;
  extern type GBufferedOutputStreamPrivate;
  extern type GFilterOutputStreamClass;
  extern type GCancellablePrivate;
  extern type GConverterInputStreamPrivate;
  extern type GConverterOutputStreamPrivate;
  extern type GBufferedInputStream;
  extern type GDataInputStreamPrivate;
  extern type GBufferedInputStreamClass;
  extern type GDataOutputStreamPrivate;
  extern type GDBusInterfaceMethodCallFunc;
  extern type GDBusInterfaceGetPropertyFunc;
  extern type GDBusInterfaceSetPropertyFunc;
  extern type GDBusSubtreeEnumerateFunc;
  extern type GDBusSubtreeIntrospectFunc;
  extern type GDBusSubtreeDispatchFunc;
  extern type GDBusInterfaceSkeletonPrivate;
  extern type GDBusAnnotationInfo;
  extern type GDBusArgInfo;
  extern type GDBusPropertyInfoFlags;
  extern type GDBusMethodInfo;
  extern type GDBusSignalInfo;
  extern type GDBusPropertyInfo;
  extern type GDBusInterfaceInfo;
  extern type GDBusObjectManagerClientPrivate;
  extern type GDBusObjectManagerServerPrivate;
  extern type GDBusObjectProxyPrivate;
  extern type GDBusObjectSkeletonPrivate;
  extern type GDBusProxyPrivate;
  extern type GEmblemedIconPrivate;
  extern type GFileAttributeInfo;
  extern type GFileEnumeratorPrivate;
  extern type GFileInputStreamPrivate;
  extern type GIOStreamPrivate;
  extern type GIOStream;
  extern type GFileIOStreamPrivate;
  extern type GIOStreamClass;
  extern type GFileMonitorPrivate;
  extern type GFileOutputStreamPrivate;
  extern type GInetAddressPrivate;
  extern type GInetAddressMaskPrivate;
  extern type GInetSocketAddressPrivate;
  extern type GSocketAddressClass;
  extern type GMemoryInputStreamPrivate;
  extern type GMemoryOutputStreamPrivate;
  extern type GMenuModelPrivate;
  extern type GMenuAttributeIterPrivate;
  extern type GMenuLinkIterPrivate;
  extern type GMountOperationPrivate;
  extern type GNativeSocketAddressPrivate;
  extern type GVolumeMonitor;
  extern type GVolumeMonitorClass;
  extern type GNetworkAddressPrivate;
  extern type GNetworkServicePrivate;
  extern type GPermissionPrivate;
  extern type GInetSocketAddress;
  extern type GProxyAddressPrivate;
  extern type GInetSocketAddressClass;
  extern type GSocketAddressEnumerator;
  extern type GProxyAddressEnumeratorPrivate;
  extern type GSocketAddressEnumeratorClass;
  extern type GResolverPrivate;
  extern type GResource;
  extern type GSettingsPrivate;
  extern type GSimpleActionGroupPrivate;
  extern type GSimpleProxyResolverPrivate;
  extern type GSocketPrivate;
  extern type GSocketClientPrivate;
  extern type GSocketConnectionPrivate;
  extern type GSocketControlMessagePrivate;
  extern type GSocketListenerPrivate;
  extern type GSocketListenerClass;
  extern type GSocketListener;
  extern type GSocketServicePrivate;
  extern type GSocketConnectionClass;
  extern type GSocketConnection;
  extern type GTcpConnectionPrivate;
  extern type GTcpConnectionClass;
  extern type GTcpConnection;
  extern type GTcpWrapperConnectionPrivate;
  extern type GSocketServiceClass;
  extern type GSocketService;
  extern type GThreadedSocketServicePrivate;
  extern type GTlsCertificatePrivate;
  extern type GTlsConnectionPrivate;
  extern type GTlsDatabasePrivate;
  extern type GTlsInteractionPrivate;
  extern type GTlsPasswordPrivate;
  extern type GParquetArrowFileReader;
  extern type GParquetArrowFileReaderClass;
  extern type GParquetWriterProperties;
  extern type GParquetWriterPropertiesClass;
  extern type GParquetArrowFileWriter;
  extern type GParquetArrowFileWriterClass;
  extern type GQueue;
  extern type _GArrowDecimal128;

  
  // All records

  
  extern type GQuark;
  extern type _GDoubleIEEE754;
  extern type _GFloatIEEE754;

  extern type _GList;


  
  extern proc GARROW_DECIMAL128(ptr : gpointer) : c_ptr(GArrowDecimal128);
  extern proc GARROW_DECIMAL128_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal128Class);
  extern proc GARROW_IS_DECIMAL128(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DECIMAL128_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DECIMAL128_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal128Class);
  extern proc garrow_decimal128_new_string(ref data : gchar) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_new_string(data : c_ptr(gchar)) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_new_integer(data : gint64) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_copy(ref decimal : GArrowDecimal128) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_copy(decimal : c_ptr(GArrowDecimal128)) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_equal(ref decimal : GArrowDecimal128, ref other_decimal : GArrowDecimal128) : gboolean;
  extern proc garrow_decimal128_equal(decimal : c_ptr(GArrowDecimal128), other_decimal : c_ptr(GArrowDecimal128)) : gboolean;
  extern proc garrow_decimal128_not_equal(ref decimal : GArrowDecimal128, ref other_decimal : GArrowDecimal128) : gboolean;
  extern proc garrow_decimal128_not_equal(decimal : c_ptr(GArrowDecimal128), other_decimal : c_ptr(GArrowDecimal128)) : gboolean;
  extern proc garrow_decimal128_less_than(ref decimal : GArrowDecimal128, ref other_decimal : GArrowDecimal128) : gboolean;
  extern proc garrow_decimal128_less_than(decimal : c_ptr(GArrowDecimal128), other_decimal : c_ptr(GArrowDecimal128)) : gboolean;
  extern proc garrow_decimal128_less_than_or_equal(ref decimal : GArrowDecimal128, ref other_decimal : GArrowDecimal128) : gboolean;
  extern proc garrow_decimal128_less_than_or_equal(decimal : c_ptr(GArrowDecimal128), other_decimal : c_ptr(GArrowDecimal128)) : gboolean;
  extern proc garrow_decimal128_greater_than(ref decimal : GArrowDecimal128, ref other_decimal : GArrowDecimal128) : gboolean;
  extern proc garrow_decimal128_greater_than(decimal : c_ptr(GArrowDecimal128), other_decimal : c_ptr(GArrowDecimal128)) : gboolean;
  extern proc garrow_decimal128_greater_than_or_equal(ref decimal : GArrowDecimal128, ref other_decimal : GArrowDecimal128) : gboolean;
  extern proc garrow_decimal128_greater_than_or_equal(decimal : c_ptr(GArrowDecimal128), other_decimal : c_ptr(GArrowDecimal128)) : gboolean;
  extern proc garrow_decimal128_to_string_scale(ref decimal : GArrowDecimal128, scale : gint32) : c_ptr(gchar);
  extern proc garrow_decimal128_to_string_scale(decimal : c_ptr(GArrowDecimal128), scale : gint32) : c_ptr(gchar);
  extern proc garrow_decimal128_to_string(ref decimal : GArrowDecimal128) : c_ptr(gchar);
  extern proc garrow_decimal128_to_string(decimal : c_ptr(GArrowDecimal128)) : c_ptr(gchar);
  extern proc garrow_decimal128_to_bytes(ref decimal : GArrowDecimal128) : c_ptr(GBytes);
  extern proc garrow_decimal128_to_bytes(decimal : c_ptr(GArrowDecimal128)) : c_ptr(GBytes);
  extern proc garrow_decimal128_abs(ref decimal : GArrowDecimal128) : void;
  extern proc garrow_decimal128_abs(decimal : c_ptr(GArrowDecimal128)) : void;
  extern proc garrow_decimal128_negate(ref decimal : GArrowDecimal128) : void;
  extern proc garrow_decimal128_negate(decimal : c_ptr(GArrowDecimal128)) : void;
  extern proc garrow_decimal128_to_integer(ref decimal : GArrowDecimal128) : gint64;
  extern proc garrow_decimal128_to_integer(decimal : c_ptr(GArrowDecimal128)) : gint64;
  extern proc garrow_decimal128_plus(ref left : GArrowDecimal128, ref right : GArrowDecimal128) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_plus(left : c_ptr(GArrowDecimal128), right : c_ptr(GArrowDecimal128)) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_minus(ref left : GArrowDecimal128, ref right : GArrowDecimal128) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_minus(left : c_ptr(GArrowDecimal128), right : c_ptr(GArrowDecimal128)) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_multiply(ref left : GArrowDecimal128, ref right : GArrowDecimal128) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_multiply(left : c_ptr(GArrowDecimal128), right : c_ptr(GArrowDecimal128)) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_divide(ref left : GArrowDecimal128, ref right : GArrowDecimal128, ref remainder : c_ptr(GArrowDecimal128), ref error : c_ptr(GError)) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_divide(left : c_ptr(GArrowDecimal128), right : c_ptr(GArrowDecimal128), remainder : c_ptr(c_ptr(GArrowDecimal128)), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_rescale(ref decimal : GArrowDecimal128, original_scale : gint32, new_scale : gint32, ref error : c_ptr(GError)) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_rescale(decimal : c_ptr(GArrowDecimal128), original_scale : gint32, new_scale : gint32, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal256_get_type() : GType;
  extern proc GARROW_DECIMAL256(ptr : gpointer) : c_ptr(GArrowDecimal256);
  extern proc GARROW_DECIMAL256_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal256Class);
  extern proc GARROW_IS_DECIMAL256(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DECIMAL256_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DECIMAL256_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal256Class);
  extern proc garrow_decimal256_new_string(ref data : gchar) : c_ptr(GArrowDecimal256);
  extern proc garrow_decimal256_new_string(data : c_ptr(gchar)) : c_ptr(GArrowDecimal256);
  extern proc garrow_decimal256_new_integer(data : gint64) : c_ptr(GArrowDecimal256);
  extern proc garrow_decimal256_copy(ref decimal : GArrowDecimal256) : c_ptr(GArrowDecimal256);
  extern proc garrow_decimal256_copy(decimal : c_ptr(GArrowDecimal256)) : c_ptr(GArrowDecimal256);
  extern proc garrow_decimal256_equal(ref decimal : GArrowDecimal256, ref other_decimal : GArrowDecimal256) : gboolean;
  extern proc garrow_decimal256_equal(decimal : c_ptr(GArrowDecimal256), other_decimal : c_ptr(GArrowDecimal256)) : gboolean;
  extern proc garrow_decimal256_not_equal(ref decimal : GArrowDecimal256, ref other_decimal : GArrowDecimal256) : gboolean;
  extern proc garrow_decimal256_not_equal(decimal : c_ptr(GArrowDecimal256), other_decimal : c_ptr(GArrowDecimal256)) : gboolean;
  extern proc garrow_decimal256_less_than(ref decimal : GArrowDecimal256, ref other_decimal : GArrowDecimal256) : gboolean;
  extern proc garrow_decimal256_less_than(decimal : c_ptr(GArrowDecimal256), other_decimal : c_ptr(GArrowDecimal256)) : gboolean;
  extern proc garrow_decimal256_less_than_or_equal(ref decimal : GArrowDecimal256, ref other_decimal : GArrowDecimal256) : gboolean;
  extern proc garrow_decimal256_less_than_or_equal(decimal : c_ptr(GArrowDecimal256), other_decimal : c_ptr(GArrowDecimal256)) : gboolean;
  extern proc garrow_decimal256_greater_than(ref decimal : GArrowDecimal256, ref other_decimal : GArrowDecimal256) : gboolean;
  extern proc garrow_decimal256_greater_than(decimal : c_ptr(GArrowDecimal256), other_decimal : c_ptr(GArrowDecimal256)) : gboolean;
  extern proc garrow_decimal256_greater_than_or_equal(ref decimal : GArrowDecimal256, ref other_decimal : GArrowDecimal256) : gboolean;
  extern proc garrow_decimal256_greater_than_or_equal(decimal : c_ptr(GArrowDecimal256), other_decimal : c_ptr(GArrowDecimal256)) : gboolean;
  extern proc garrow_decimal256_to_string_scale(ref decimal : GArrowDecimal256, scale : gint32) : c_ptr(gchar);
  extern proc garrow_decimal256_to_string_scale(decimal : c_ptr(GArrowDecimal256), scale : gint32) : c_ptr(gchar);
  extern proc garrow_decimal256_to_string(ref decimal : GArrowDecimal256) : c_ptr(gchar);
  extern proc garrow_decimal256_to_string(decimal : c_ptr(GArrowDecimal256)) : c_ptr(gchar);
  extern proc garrow_decimal256_to_bytes(ref decimal : GArrowDecimal256) : c_ptr(GBytes);
  extern proc garrow_decimal256_to_bytes(decimal : c_ptr(GArrowDecimal256)) : c_ptr(GBytes);
  extern proc garrow_decimal256_abs(ref decimal : GArrowDecimal256) : void;
  extern proc garrow_decimal256_abs(decimal : c_ptr(GArrowDecimal256)) : void;
  extern proc garrow_decimal256_negate(ref decimal : GArrowDecimal256) : void;
  extern proc garrow_decimal256_negate(decimal : c_ptr(GArrowDecimal256)) : void;
  extern proc garrow_decimal256_plus(ref left : GArrowDecimal256, ref right : GArrowDecimal256) : c_ptr(GArrowDecimal256);
  extern proc garrow_decimal256_plus(left : c_ptr(GArrowDecimal256), right : c_ptr(GArrowDecimal256)) : c_ptr(GArrowDecimal256);
  extern proc garrow_decimal256_multiply(ref left : GArrowDecimal256, ref right : GArrowDecimal256) : c_ptr(GArrowDecimal256);
  extern proc garrow_decimal256_multiply(left : c_ptr(GArrowDecimal256), right : c_ptr(GArrowDecimal256)) : c_ptr(GArrowDecimal256);
  extern proc garrow_decimal256_divide(ref left : GArrowDecimal256, ref right : GArrowDecimal256, ref remainder : c_ptr(GArrowDecimal256), ref error : c_ptr(GError)) : c_ptr(GArrowDecimal256);
  extern proc garrow_decimal256_divide(left : c_ptr(GArrowDecimal256), right : c_ptr(GArrowDecimal256), remainder : c_ptr(c_ptr(GArrowDecimal256)), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowDecimal256);
  extern proc garrow_decimal256_rescale(ref decimal : GArrowDecimal256, original_scale : gint32, new_scale : gint32, ref error : c_ptr(GError)) : c_ptr(GArrowDecimal256);
  extern proc garrow_decimal256_rescale(decimal : c_ptr(GArrowDecimal256), original_scale : gint32, new_scale : gint32, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowDecimal256);
  extern proc garrow_data_type_get_type() : GType;
  extern proc GARROW_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowDataType);
  extern proc GARROW_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowDataTypeClass);
  extern proc GARROW_IS_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDataTypeClass);
  extern proc garrow_data_type_equal(ref data_type : GArrowDataType, ref other_data_type : GArrowDataType) : gboolean;
  extern proc garrow_data_type_equal(data_type : c_ptr(GArrowDataType), other_data_type : c_ptr(GArrowDataType)) : gboolean;
  extern proc garrow_data_type_to_string(ref data_type : GArrowDataType) : c_ptr(gchar);
  extern proc garrow_data_type_to_string(data_type : c_ptr(GArrowDataType)) : c_ptr(gchar);
  extern proc garrow_data_type_get_id(ref data_type : GArrowDataType) : GArrowType;
  extern proc garrow_data_type_get_id(data_type : c_ptr(GArrowDataType)) : GArrowType;
  extern proc garrow_data_type_get_name(ref data_type : GArrowDataType) : c_ptr(gchar);
  extern proc garrow_data_type_get_name(data_type : c_ptr(GArrowDataType)) : c_ptr(gchar);
  extern proc garrow_fixed_width_data_type_get_type() : GType;
  extern proc GARROW_FIXED_WIDTH_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowFixedWidthDataType);
  extern proc GARROW_FIXED_WIDTH_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowFixedWidthDataTypeClass);
  extern proc GARROW_IS_FIXED_WIDTH_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FIXED_WIDTH_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FIXED_WIDTH_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFixedWidthDataTypeClass);
  extern proc garrow_fixed_width_data_type_get_bit_width(ref data_type : GArrowFixedWidthDataType) : gint;
  extern proc garrow_fixed_width_data_type_get_bit_width(data_type : c_ptr(GArrowFixedWidthDataType)) : gint;
  extern proc garrow_null_data_type_get_type() : GType;
  extern proc GARROW_NULL_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowNullDataType);
  extern proc GARROW_NULL_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowNullDataTypeClass);
  extern proc GARROW_IS_NULL_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_NULL_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_NULL_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowNullDataTypeClass);
  extern proc garrow_null_data_type_new() : c_ptr(GArrowNullDataType);
  extern proc garrow_boolean_data_type_get_type() : GType;
  extern proc GARROW_BOOLEAN_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowBooleanDataType);
  extern proc GARROW_BOOLEAN_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowBooleanDataTypeClass);
  extern proc GARROW_IS_BOOLEAN_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_BOOLEAN_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_BOOLEAN_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowBooleanDataTypeClass);
  extern proc garrow_boolean_data_type_new() : c_ptr(GArrowBooleanDataType);
  extern proc garrow_numeric_data_type_get_type() : GType;
  extern proc GARROW_NUMERIC_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowNumericDataType);
  extern proc GARROW_NUMERIC_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowNumericDataTypeClass);
  extern proc GARROW_IS_NUMERIC_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_NUMERIC_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_NUMERIC_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowNumericDataTypeClass);
  extern proc garrow_integer_data_type_get_type() : GType;
  extern proc GARROW_INTEGER_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowIntegerDataType);
  extern proc GARROW_INTEGER_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowIntegerDataTypeClass);
  extern proc GARROW_IS_INTEGER_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INTEGER_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INTEGER_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowIntegerDataTypeClass);
  extern proc garrow_integer_data_type_is_signed(ref data_type : GArrowIntegerDataType) : gboolean;
  extern proc garrow_integer_data_type_is_signed(data_type : c_ptr(GArrowIntegerDataType)) : gboolean;
  extern proc garrow_int8_data_type_get_type() : GType;
  extern proc GARROW_INT8_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowInt8DataType);
  extern proc GARROW_INT8_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowInt8DataTypeClass);
  extern proc GARROW_IS_INT8_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT8_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT8_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt8DataTypeClass);
  extern proc garrow_int8_data_type_new() : c_ptr(GArrowInt8DataType);
  extern proc garrow_uint8_data_type_get_type() : GType;
  extern proc GARROW_UINT8_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowUInt8DataType);
  extern proc GARROW_UINT8_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowUInt8DataTypeClass);
  extern proc GARROW_IS_UINT8_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT8_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT8_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt8DataTypeClass);
  extern proc garrow_uint8_data_type_new() : c_ptr(GArrowUInt8DataType);
  extern proc garrow_int16_data_type_get_type() : GType;
  extern proc GARROW_INT16_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowInt16DataType);
  extern proc GARROW_INT16_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowInt16DataTypeClass);
  extern proc GARROW_IS_INT16_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT16_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT16_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt16DataTypeClass);
  extern proc garrow_int16_data_type_new() : c_ptr(GArrowInt16DataType);
  extern proc garrow_uint16_data_type_get_type() : GType;
  extern proc GARROW_UINT16_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowUInt16DataType);
  extern proc GARROW_UINT16_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowUInt16DataTypeClass);
  extern proc GARROW_IS_UINT16_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT16_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT16_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt16DataTypeClass);
  extern proc garrow_uint16_data_type_new() : c_ptr(GArrowUInt16DataType);
  extern proc garrow_int32_data_type_get_type() : GType;
  extern proc GARROW_INT32_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowInt32DataType);
  extern proc GARROW_INT32_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowInt32DataTypeClass);
  extern proc GARROW_IS_INT32_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT32_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT32_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt32DataTypeClass);
  extern proc garrow_int32_data_type_new() : c_ptr(GArrowInt32DataType);
  extern proc garrow_uint32_data_type_get_type() : GType;
  extern proc GARROW_UINT32_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowUInt32DataType);
  extern proc GARROW_UINT32_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowUInt32DataTypeClass);
  extern proc GARROW_IS_UINT32_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT32_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT32_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt32DataTypeClass);
  extern proc garrow_uint32_data_type_new() : c_ptr(GArrowUInt32DataType);
  extern proc garrow_int64_data_type_get_type() : GType;
  extern proc GARROW_INT64_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowInt64DataType);
  extern proc GARROW_INT64_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowInt64DataTypeClass);
  extern proc GARROW_IS_INT64_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT64_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT64_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt64DataTypeClass);
  extern proc garrow_int64_data_type_new() : c_ptr(GArrowInt64DataType);
  extern proc garrow_uint64_data_type_get_type() : GType;
  extern proc GARROW_UINT64_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowUInt64DataType);
  extern proc GARROW_UINT64_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowUInt64DataTypeClass);
  extern proc GARROW_IS_UINT64_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT64_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT64_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt64DataTypeClass);
  extern proc garrow_uint64_data_type_new() : c_ptr(GArrowUInt64DataType);
  extern proc garrow_floating_point_data_type_get_type() : GType;
  extern proc GARROW_FLOATING_POINT_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowFloatingPointDataType);
  extern proc GARROW_FLOATING_POINT_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowFloatingPointDataTypeClass);
  extern proc GARROW_IS_FLOATING_POINT_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FLOATING_POINT_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FLOATING_POINT_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFloatingPointDataTypeClass);
  extern proc garrow_float_data_type_get_type() : GType;
  extern proc GARROW_FLOAT_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowFloatDataType);
  extern proc GARROW_FLOAT_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowFloatDataTypeClass);
  extern proc GARROW_IS_FLOAT_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FLOAT_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FLOAT_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFloatDataTypeClass);
  extern proc garrow_float_data_type_new() : c_ptr(GArrowFloatDataType);
  extern proc garrow_double_data_type_get_type() : GType;
  extern proc GARROW_DOUBLE_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowDoubleDataType);
  extern proc GARROW_DOUBLE_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowDoubleDataTypeClass);
  extern proc GARROW_IS_DOUBLE_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DOUBLE_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DOUBLE_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDoubleDataTypeClass);
  extern proc garrow_double_data_type_new() : c_ptr(GArrowDoubleDataType);
  extern proc garrow_binary_data_type_get_type() : GType;
  extern proc GARROW_BINARY_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowBinaryDataType);
  extern proc GARROW_BINARY_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowBinaryDataTypeClass);
  extern proc GARROW_IS_BINARY_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_BINARY_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_BINARY_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowBinaryDataTypeClass);
  extern proc garrow_binary_data_type_new() : c_ptr(GArrowBinaryDataType);
  extern proc garrow_fixed_size_binary_data_type_get_type() : GType;
  extern proc GARROW_FIXED_SIZE_BINARY_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowFixedSizeBinaryDataType);
  extern proc GARROW_FIXED_SIZE_BINARY_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowFixedSizeBinaryDataTypeClass);
  extern proc GARROW_IS_FIXED_SIZE_BINARY_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FIXED_SIZE_BINARY_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FIXED_SIZE_BINARY_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFixedSizeBinaryDataTypeClass);
  extern proc garrow_fixed_size_binary_data_type_new(byte_width : gint32) : c_ptr(GArrowFixedSizeBinaryDataType);
  extern proc garrow_fixed_size_binary_data_type_get_byte_width(ref data_type : GArrowFixedSizeBinaryDataType) : gint32;
  extern proc garrow_fixed_size_binary_data_type_get_byte_width(data_type : c_ptr(GArrowFixedSizeBinaryDataType)) : gint32;
  extern proc garrow_large_binary_data_type_get_type() : GType;
  extern proc GARROW_LARGE_BINARY_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowLargeBinaryDataType);
  extern proc GARROW_LARGE_BINARY_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowLargeBinaryDataTypeClass);
  extern proc GARROW_IS_LARGE_BINARY_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LARGE_BINARY_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LARGE_BINARY_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowLargeBinaryDataTypeClass);
  extern proc garrow_large_binary_data_type_new() : c_ptr(GArrowLargeBinaryDataType);
  extern proc garrow_string_data_type_get_type() : GType;
  extern proc GARROW_STRING_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowStringDataType);
  extern proc GARROW_STRING_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowStringDataTypeClass);
  extern proc GARROW_IS_STRING_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_STRING_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_STRING_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowStringDataTypeClass);
  extern proc garrow_string_data_type_new() : c_ptr(GArrowStringDataType);
  extern proc garrow_large_string_data_type_get_type() : GType;
  extern proc GARROW_LARGE_STRING_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowLargeStringDataType);
  extern proc GARROW_LARGE_STRING_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowLargeStringDataTypeClass);
  extern proc GARROW_IS_LARGE_STRING_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LARGE_STRING_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LARGE_STRING_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowLargeStringDataTypeClass);
  extern proc garrow_large_string_data_type_new() : c_ptr(GArrowLargeStringDataType);
  extern proc garrow_date32_data_type_get_type() : GType;
  extern proc GARROW_DATE32_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowDate32DataType);
  extern proc GARROW_DATE32_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowDate32DataTypeClass);
  extern proc GARROW_IS_DATE32_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DATE32_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DATE32_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDate32DataTypeClass);
  extern proc garrow_date32_data_type_new() : c_ptr(GArrowDate32DataType);
  extern proc garrow_date64_data_type_get_type() : GType;
  extern proc GARROW_DATE64_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowDate64DataType);
  extern proc GARROW_DATE64_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowDate64DataTypeClass);
  extern proc GARROW_IS_DATE64_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DATE64_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DATE64_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDate64DataTypeClass);
  extern proc garrow_date64_data_type_new() : c_ptr(GArrowDate64DataType);
  extern proc garrow_timestamp_data_type_get_type() : GType;
  extern proc GARROW_TIMESTAMP_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowTimestampDataType);
  extern proc GARROW_TIMESTAMP_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowTimestampDataTypeClass);
  extern proc GARROW_IS_TIMESTAMP_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TIMESTAMP_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TIMESTAMP_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTimestampDataTypeClass);
  extern proc garrow_timestamp_data_type_new(unit : GArrowTimeUnit) : c_ptr(GArrowTimestampDataType);
  extern proc garrow_timestamp_data_type_get_unit(ref timestamp_data_type : GArrowTimestampDataType) : GArrowTimeUnit;
  extern proc garrow_timestamp_data_type_get_unit(timestamp_data_type : c_ptr(GArrowTimestampDataType)) : GArrowTimeUnit;
  extern proc garrow_time_data_type_get_type() : GType;
  extern proc GARROW_TIME_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowTimeDataType);
  extern proc GARROW_TIME_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowTimeDataTypeClass);
  extern proc GARROW_IS_TIME_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TIME_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TIME_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTimeDataTypeClass);
  extern proc garrow_time_data_type_get_unit(ref time_data_type : GArrowTimeDataType) : GArrowTimeUnit;
  extern proc garrow_time_data_type_get_unit(time_data_type : c_ptr(GArrowTimeDataType)) : GArrowTimeUnit;
  extern proc garrow_time32_data_type_get_type() : GType;
  extern proc GARROW_TIME32_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowTime32DataType);
  extern proc GARROW_TIME32_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowTime32DataTypeClass);
  extern proc GARROW_IS_TIME32_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TIME32_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TIME32_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTime32DataTypeClass);
  extern proc garrow_time32_data_type_new(unit : GArrowTimeUnit, ref error : c_ptr(GError)) : c_ptr(GArrowTime32DataType);
  extern proc garrow_time32_data_type_new(unit : GArrowTimeUnit, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTime32DataType);
  extern proc garrow_time64_data_type_get_type() : GType;
  extern proc GARROW_TIME64_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowTime64DataType);
  extern proc GARROW_TIME64_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowTime64DataTypeClass);
  extern proc GARROW_IS_TIME64_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TIME64_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TIME64_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTime64DataTypeClass);
  extern proc garrow_time64_data_type_new(unit : GArrowTimeUnit, ref error : c_ptr(GError)) : c_ptr(GArrowTime64DataType);
  extern proc garrow_time64_data_type_new(unit : GArrowTimeUnit, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTime64DataType);
  extern proc garrow_decimal_data_type_get_type() : GType;
  extern proc GARROW_DECIMAL_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowDecimalDataType);
  extern proc GARROW_DECIMAL_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowDecimalDataTypeClass);
  extern proc GARROW_IS_DECIMAL_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DECIMAL_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DECIMAL_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDecimalDataTypeClass);
  extern proc garrow_decimal_data_type_new(precision : gint32, scale : gint32) : c_ptr(GArrowDecimalDataType);
  extern proc garrow_decimal_data_type_get_precision(ref decimal_data_type : GArrowDecimalDataType) : gint32;
  extern proc garrow_decimal_data_type_get_precision(decimal_data_type : c_ptr(GArrowDecimalDataType)) : gint32;
  extern proc garrow_decimal_data_type_get_scale(ref decimal_data_type : GArrowDecimalDataType) : gint32;
  extern proc garrow_decimal_data_type_get_scale(decimal_data_type : c_ptr(GArrowDecimalDataType)) : gint32;
  extern proc garrow_decimal128_data_type_get_type() : GType;
  extern proc GARROW_DECIMAL128_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowDecimal128DataType);
  extern proc GARROW_DECIMAL128_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal128DataTypeClass);
  extern proc GARROW_IS_DECIMAL128_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DECIMAL128_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DECIMAL128_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal128DataTypeClass);
  extern proc garrow_decimal128_data_type_max_precision() : gint32;
  extern proc garrow_decimal128_data_type_new(precision : gint32, scale : gint32) : c_ptr(GArrowDecimal128DataType);
  extern proc garrow_decimal256_data_type_get_type() : GType;
  extern proc GARROW_DECIMAL256_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowDecimal256DataType);
  extern proc GARROW_DECIMAL256_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal256DataTypeClass);
  extern proc GARROW_IS_DECIMAL256_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DECIMAL256_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DECIMAL256_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal256DataTypeClass);
  extern proc garrow_decimal256_data_type_max_precision() : gint32;
  extern proc garrow_decimal256_data_type_new(precision : gint32, scale : gint32) : c_ptr(GArrowDecimal256DataType);
  extern proc garrow_extension_data_type_get_type() : GType;
  extern proc GARROW_EXTENSION_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowExtensionDataType);
  extern proc GARROW_EXTENSION_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowExtensionDataTypeClass);
  extern proc GARROW_IS_EXTENSION_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_EXTENSION_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_EXTENSION_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowExtensionDataTypeClass);
  extern proc garrow_extension_data_type_get_extension_name(ref data_type : GArrowExtensionDataType) : c_ptr(gchar);
  extern proc garrow_extension_data_type_get_extension_name(data_type : c_ptr(GArrowExtensionDataType)) : c_ptr(gchar);
  extern proc garrow_extension_data_type_wrap_array(ref data_type : GArrowExtensionDataType, ref storage : GArrowArray) : c_ptr(GArrowExtensionArray);
  extern proc garrow_extension_data_type_wrap_array(data_type : c_ptr(GArrowExtensionDataType), storage : c_ptr(GArrowArray)) : c_ptr(GArrowExtensionArray);
  extern proc garrow_extension_data_type_wrap_chunked_array(ref data_type : GArrowExtensionDataType, ref storage : GArrowChunkedArray) : c_ptr(GArrowChunkedArray);
  extern proc garrow_extension_data_type_wrap_chunked_array(data_type : c_ptr(GArrowExtensionDataType), storage : c_ptr(GArrowChunkedArray)) : c_ptr(GArrowChunkedArray);
  extern proc garrow_extension_data_type_registry_get_type() : GType;
  extern proc GARROW_EXTENSION_DATA_TYPE_REGISTRY(ptr : gpointer) : c_ptr(GArrowExtensionDataTypeRegistry);
  extern proc GARROW_EXTENSION_DATA_TYPE_REGISTRY_CLASS(ptr : gpointer) : c_ptr(GArrowExtensionDataTypeRegistryClass);
  extern proc GARROW_IS_EXTENSION_DATA_TYPE_REGISTRY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_EXTENSION_DATA_TYPE_REGISTRY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_EXTENSION_DATA_TYPE_REGISTRY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowExtensionDataTypeRegistryClass);
  extern proc garrow_extension_data_type_registry_default() : c_ptr(GArrowExtensionDataTypeRegistry);
  extern proc garrow_extension_data_type_registry_register(ref registry : GArrowExtensionDataTypeRegistry, ref data_type : GArrowExtensionDataType, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_extension_data_type_registry_register(registry : c_ptr(GArrowExtensionDataTypeRegistry), data_type : c_ptr(GArrowExtensionDataType), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_extension_data_type_registry_unregister(ref registry : GArrowExtensionDataTypeRegistry, ref name : gchar, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_extension_data_type_registry_unregister(registry : c_ptr(GArrowExtensionDataTypeRegistry), name : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_extension_data_type_registry_lookup(ref registry : GArrowExtensionDataTypeRegistry, ref name : gchar) : c_ptr(GArrowExtensionDataType);
  extern proc garrow_extension_data_type_registry_lookup(registry : c_ptr(GArrowExtensionDataTypeRegistry), name : c_ptr(gchar)) : c_ptr(GArrowExtensionDataType);
  extern proc garrow_buffer_get_type() : GType;
  extern proc GARROW_BUFFER(ptr : gpointer) : c_ptr(GArrowBuffer);
  extern proc GARROW_BUFFER_CLASS(ptr : gpointer) : c_ptr(GArrowBufferClass);
  extern proc GARROW_IS_BUFFER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_BUFFER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_BUFFER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowBufferClass);
  extern proc garrow_buffer_new(ref data : guint8, size : gint64) : c_ptr(GArrowBuffer);
  extern proc garrow_buffer_new(data : c_ptr(guint8), size : gint64) : c_ptr(GArrowBuffer);
  extern proc garrow_buffer_new_bytes(ref data : GBytes) : c_ptr(GArrowBuffer);
  extern proc garrow_buffer_new_bytes(data : c_ptr(GBytes)) : c_ptr(GArrowBuffer);
  extern proc garrow_buffer_equal(ref buffer : GArrowBuffer, ref other_buffer : GArrowBuffer) : gboolean;
  extern proc garrow_buffer_equal(buffer : c_ptr(GArrowBuffer), other_buffer : c_ptr(GArrowBuffer)) : gboolean;
  extern proc garrow_buffer_equal_n_bytes(ref buffer : GArrowBuffer, ref other_buffer : GArrowBuffer, n_bytes : gint64) : gboolean;
  extern proc garrow_buffer_equal_n_bytes(buffer : c_ptr(GArrowBuffer), other_buffer : c_ptr(GArrowBuffer), n_bytes : gint64) : gboolean;
  extern proc garrow_buffer_is_mutable(ref buffer : GArrowBuffer) : gboolean;
  extern proc garrow_buffer_is_mutable(buffer : c_ptr(GArrowBuffer)) : gboolean;
  extern proc garrow_buffer_get_capacity(ref buffer : GArrowBuffer) : gint64;
  extern proc garrow_buffer_get_capacity(buffer : c_ptr(GArrowBuffer)) : gint64;
  extern proc garrow_buffer_get_data(ref buffer : GArrowBuffer) : c_ptr(GBytes);
  extern proc garrow_buffer_get_data(buffer : c_ptr(GArrowBuffer)) : c_ptr(GBytes);
  extern proc garrow_buffer_get_mutable_data(ref buffer : GArrowBuffer) : c_ptr(GBytes);
  extern proc garrow_buffer_get_mutable_data(buffer : c_ptr(GArrowBuffer)) : c_ptr(GBytes);
  extern proc garrow_buffer_get_size(ref buffer : GArrowBuffer) : gint64;
  extern proc garrow_buffer_get_size(buffer : c_ptr(GArrowBuffer)) : gint64;
  extern proc garrow_buffer_get_parent(ref buffer : GArrowBuffer) : c_ptr(GArrowBuffer);
  extern proc garrow_buffer_get_parent(buffer : c_ptr(GArrowBuffer)) : c_ptr(GArrowBuffer);
  extern proc garrow_buffer_copy(ref buffer : GArrowBuffer, start : gint64, size : gint64, ref error : c_ptr(GError)) : c_ptr(GArrowBuffer);
  extern proc garrow_buffer_copy(buffer : c_ptr(GArrowBuffer), start : gint64, size : gint64, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowBuffer);
  extern proc garrow_buffer_slice(ref buffer : GArrowBuffer, offset : gint64, size : gint64) : c_ptr(GArrowBuffer);
  extern proc garrow_buffer_slice(buffer : c_ptr(GArrowBuffer), offset : gint64, size : gint64) : c_ptr(GArrowBuffer);
  extern proc garrow_mutable_buffer_get_type() : GType;
  extern proc GARROW_MUTABLE_BUFFER(ptr : gpointer) : c_ptr(GArrowMutableBuffer);
  extern proc GARROW_MUTABLE_BUFFER_CLASS(ptr : gpointer) : c_ptr(GArrowMutableBufferClass);
  extern proc GARROW_IS_MUTABLE_BUFFER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_MUTABLE_BUFFER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_MUTABLE_BUFFER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowMutableBufferClass);
  extern proc garrow_mutable_buffer_new(ref data : guint8, size : gint64) : c_ptr(GArrowMutableBuffer);
  extern proc garrow_mutable_buffer_new(data : c_ptr(guint8), size : gint64) : c_ptr(GArrowMutableBuffer);
  extern proc garrow_mutable_buffer_new_bytes(ref data : GBytes) : c_ptr(GArrowMutableBuffer);
  extern proc garrow_mutable_buffer_new_bytes(data : c_ptr(GBytes)) : c_ptr(GArrowMutableBuffer);
  extern proc garrow_mutable_buffer_slice(ref buffer : GArrowMutableBuffer, offset : gint64, size : gint64) : c_ptr(GArrowMutableBuffer);
  extern proc garrow_mutable_buffer_slice(buffer : c_ptr(GArrowMutableBuffer), offset : gint64, size : gint64) : c_ptr(GArrowMutableBuffer);
  extern proc garrow_mutable_buffer_set_data(ref buffer : GArrowMutableBuffer, offset : gint64, ref data : guint8, size : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_mutable_buffer_set_data(buffer : c_ptr(GArrowMutableBuffer), offset : gint64, data : c_ptr(guint8), size : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_resizable_buffer_get_type() : GType;
  extern proc GARROW_RESIZABLE_BUFFER(ptr : gpointer) : c_ptr(GArrowResizableBuffer);
  extern proc GARROW_RESIZABLE_BUFFER_CLASS(ptr : gpointer) : c_ptr(GArrowResizableBufferClass);
  extern proc GARROW_IS_RESIZABLE_BUFFER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_RESIZABLE_BUFFER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_RESIZABLE_BUFFER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowResizableBufferClass);
  extern proc garrow_resizable_buffer_new(initial_size : gint64, ref error : c_ptr(GError)) : c_ptr(GArrowResizableBuffer);
  extern proc garrow_resizable_buffer_new(initial_size : gint64, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowResizableBuffer);
  extern proc garrow_resizable_buffer_resize(ref buffer : GArrowResizableBuffer, new_size : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_resizable_buffer_resize(buffer : c_ptr(GArrowResizableBuffer), new_size : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_resizable_buffer_reserve(ref buffer : GArrowResizableBuffer, new_capacity : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_resizable_buffer_reserve(buffer : c_ptr(GArrowResizableBuffer), new_capacity : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_equal_options_get_type() : GType;
  extern proc GARROW_EQUAL_OPTIONS(ptr : gpointer) : c_ptr(GArrowEqualOptions);
  extern proc GARROW_EQUAL_OPTIONS_CLASS(ptr : gpointer) : c_ptr(GArrowEqualOptionsClass);
  extern proc GARROW_IS_EQUAL_OPTIONS(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_EQUAL_OPTIONS_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_EQUAL_OPTIONS_GET_CLASS(ptr : gpointer) : c_ptr(GArrowEqualOptionsClass);
  extern proc garrow_equal_options_new() : c_ptr(GArrowEqualOptions);
  extern proc garrow_equal_options_is_approx(ref options : GArrowEqualOptions) : gboolean;
  extern proc garrow_equal_options_is_approx(options : c_ptr(GArrowEqualOptions)) : gboolean;
  extern proc garrow_array_get_type() : GType;
  extern proc GARROW_ARRAY(ptr : gpointer) : c_ptr(GArrowArray);
  extern proc GARROW_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowArrayClass);
  extern proc GARROW_IS_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowArrayClass);
  extern proc garrow_array_equal(ref array : GArrowArray, ref other_array : GArrowArray) : gboolean;
  extern proc garrow_array_equal(array : c_ptr(GArrowArray), other_array : c_ptr(GArrowArray)) : gboolean;
  extern proc garrow_array_equal_options(ref array : GArrowArray, ref other_array : GArrowArray, ref options : GArrowEqualOptions) : gboolean;
  extern proc garrow_array_equal_options(array : c_ptr(GArrowArray), other_array : c_ptr(GArrowArray), options : c_ptr(GArrowEqualOptions)) : gboolean;
  extern proc garrow_array_equal_approx(ref array : GArrowArray, ref other_array : GArrowArray) : gboolean;
  extern proc garrow_array_equal_approx(array : c_ptr(GArrowArray), other_array : c_ptr(GArrowArray)) : gboolean;
  extern proc garrow_array_equal_range(ref array : GArrowArray, start_index : gint64, ref other_array : GArrowArray, other_start_index : gint64, end_index : gint64, ref options : GArrowEqualOptions) : gboolean;
  extern proc garrow_array_equal_range(array : c_ptr(GArrowArray), start_index : gint64, other_array : c_ptr(GArrowArray), other_start_index : gint64, end_index : gint64, options : c_ptr(GArrowEqualOptions)) : gboolean;
  extern proc garrow_array_is_null(ref array : GArrowArray, i : gint64) : gboolean;
  extern proc garrow_array_is_null(array : c_ptr(GArrowArray), i : gint64) : gboolean;
  extern proc garrow_array_is_valid(ref array : GArrowArray, i : gint64) : gboolean;
  extern proc garrow_array_is_valid(array : c_ptr(GArrowArray), i : gint64) : gboolean;
  extern proc garrow_array_get_length(ref array : GArrowArray) : gint64;
  extern proc garrow_array_get_length(array : c_ptr(GArrowArray)) : gint64;
  extern proc garrow_array_get_offset(ref array : GArrowArray) : gint64;
  extern proc garrow_array_get_offset(array : c_ptr(GArrowArray)) : gint64;
  extern proc garrow_array_get_n_nulls(ref array : GArrowArray) : gint64;
  extern proc garrow_array_get_n_nulls(array : c_ptr(GArrowArray)) : gint64;
  extern proc garrow_array_get_null_bitmap(ref array : GArrowArray) : c_ptr(GArrowBuffer);
  extern proc garrow_array_get_null_bitmap(array : c_ptr(GArrowArray)) : c_ptr(GArrowBuffer);
  extern proc garrow_array_get_value_data_type(ref array : GArrowArray) : c_ptr(GArrowDataType);
  extern proc garrow_array_get_value_data_type(array : c_ptr(GArrowArray)) : c_ptr(GArrowDataType);
  extern proc garrow_array_get_value_type(ref array : GArrowArray) : GArrowType;
  extern proc garrow_array_get_value_type(array : c_ptr(GArrowArray)) : GArrowType;
  extern proc garrow_array_slice(ref array : GArrowArray, offset : gint64, length : gint64) : c_ptr(GArrowArray);
  extern proc garrow_array_slice(array : c_ptr(GArrowArray), offset : gint64, length : gint64) : c_ptr(GArrowArray);
  extern proc garrow_array_to_string(ref array : GArrowArray, ref error : c_ptr(GError)) : c_ptr(gchar);
  extern proc garrow_array_to_string(array : c_ptr(GArrowArray), error : c_ptr(c_ptr(GError))) : c_ptr(gchar);
  extern proc garrow_array_view(ref array : GArrowArray, ref return_type : GArrowDataType, ref error : c_ptr(GError)) : c_ptr(GArrowArray);
  extern proc garrow_array_view(array : c_ptr(GArrowArray), return_type : c_ptr(GArrowDataType), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowArray);
  extern proc garrow_array_diff_unified(ref array : GArrowArray, ref other_array : GArrowArray) : c_ptr(gchar);
  extern proc garrow_array_diff_unified(array : c_ptr(GArrowArray), other_array : c_ptr(GArrowArray)) : c_ptr(gchar);
  extern proc garrow_array_concatenate(ref array : GArrowArray, ref other_arrays : GList, ref error : c_ptr(GError)) : c_ptr(GArrowArray);
  extern proc garrow_array_concatenate(array : c_ptr(GArrowArray), other_arrays : c_ptr(GList), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowArray);
  extern proc garrow_null_array_get_type() : GType;
  extern proc GARROW_NULL_ARRAY(ptr : gpointer) : c_ptr(GArrowNullArray);
  extern proc GARROW_NULL_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowNullArrayClass);
  extern proc GARROW_IS_NULL_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_NULL_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_NULL_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowNullArrayClass);
  extern proc garrow_null_array_new(length : gint64) : c_ptr(GArrowNullArray);
  extern proc garrow_primitive_array_get_type() : GType;
  extern proc GARROW_PRIMITIVE_ARRAY(ptr : gpointer) : c_ptr(GArrowPrimitiveArray);
  extern proc GARROW_PRIMITIVE_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowPrimitiveArrayClass);
  extern proc GARROW_IS_PRIMITIVE_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_PRIMITIVE_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_PRIMITIVE_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowPrimitiveArrayClass);
  extern proc garrow_primitive_array_get_buffer(ref array : GArrowPrimitiveArray) : c_ptr(GArrowBuffer);
  extern proc garrow_primitive_array_get_buffer(array : c_ptr(GArrowPrimitiveArray)) : c_ptr(GArrowBuffer);
  extern proc garrow_primitive_array_get_data_buffer(ref array : GArrowPrimitiveArray) : c_ptr(GArrowBuffer);
  extern proc garrow_primitive_array_get_data_buffer(array : c_ptr(GArrowPrimitiveArray)) : c_ptr(GArrowBuffer);
  extern proc garrow_boolean_array_get_type() : GType;
  extern proc GARROW_BOOLEAN_ARRAY(ptr : gpointer) : c_ptr(GArrowBooleanArray);
  extern proc GARROW_BOOLEAN_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowBooleanArrayClass);
  extern proc GARROW_IS_BOOLEAN_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_BOOLEAN_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_BOOLEAN_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowBooleanArrayClass);
  extern proc garrow_boolean_array_new(length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowBooleanArray);
  extern proc garrow_boolean_array_new(length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowBooleanArray);
  extern proc garrow_boolean_array_get_value(ref array : GArrowBooleanArray, i : gint64) : gboolean;
  extern proc garrow_boolean_array_get_value(array : c_ptr(GArrowBooleanArray), i : gint64) : gboolean;
  extern proc garrow_boolean_array_get_values(ref array : GArrowBooleanArray, ref length : gint64) : c_ptr(gboolean);
  extern proc garrow_boolean_array_get_values(array : c_ptr(GArrowBooleanArray), length : c_ptr(gint64)) : c_ptr(gboolean);
  extern proc garrow_numeric_array_get_type() : GType;
  extern proc GARROW_NUMERIC_ARRAY(ptr : gpointer) : c_ptr(GArrowNumericArray);
  extern proc GARROW_NUMERIC_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowNumericArrayClass);
  extern proc GARROW_IS_NUMERIC_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_NUMERIC_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_NUMERIC_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowNumericArrayClass);
  extern proc garrow_int8_array_get_type() : GType;
  extern proc GARROW_INT8_ARRAY(ptr : gpointer) : c_ptr(GArrowInt8Array);
  extern proc GARROW_INT8_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowInt8ArrayClass);
  extern proc GARROW_IS_INT8_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT8_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT8_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt8ArrayClass);
  extern proc garrow_int8_array_new(length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowInt8Array);
  extern proc garrow_int8_array_new(length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowInt8Array);
  extern proc garrow_int8_array_get_value(ref array : GArrowInt8Array, i : gint64) : gint8;
  extern proc garrow_int8_array_get_value(array : c_ptr(GArrowInt8Array), i : gint64) : gint8;
  extern proc garrow_int8_array_get_values(ref array : GArrowInt8Array, ref length : gint64) : c_ptr(gint8);
  extern proc garrow_int8_array_get_values(array : c_ptr(GArrowInt8Array), length : c_ptr(gint64)) : c_ptr(gint8);
  extern proc garrow_uint8_array_get_type() : GType;
  extern proc GARROW_UINT8_ARRAY(ptr : gpointer) : c_ptr(GArrowUInt8Array);
  extern proc GARROW_UINT8_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowUInt8ArrayClass);
  extern proc GARROW_IS_UINT8_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT8_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT8_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt8ArrayClass);
  extern proc garrow_uint8_array_new(length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowUInt8Array);
  extern proc garrow_uint8_array_new(length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowUInt8Array);
  extern proc garrow_uint8_array_get_value(ref array : GArrowUInt8Array, i : gint64) : guint8;
  extern proc garrow_uint8_array_get_value(array : c_ptr(GArrowUInt8Array), i : gint64) : guint8;
  extern proc garrow_uint8_array_get_values(ref array : GArrowUInt8Array, ref length : gint64) : c_ptr(guint8);
  extern proc garrow_uint8_array_get_values(array : c_ptr(GArrowUInt8Array), length : c_ptr(gint64)) : c_ptr(guint8);
  extern proc garrow_int16_array_get_type() : GType;
  extern proc GARROW_INT16_ARRAY(ptr : gpointer) : c_ptr(GArrowInt16Array);
  extern proc GARROW_INT16_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowInt16ArrayClass);
  extern proc GARROW_IS_INT16_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT16_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT16_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt16ArrayClass);
  extern proc garrow_int16_array_new(length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowInt16Array);
  extern proc garrow_int16_array_new(length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowInt16Array);
  extern proc garrow_int16_array_get_value(ref array : GArrowInt16Array, i : gint64) : gint16;
  extern proc garrow_int16_array_get_value(array : c_ptr(GArrowInt16Array), i : gint64) : gint16;
  extern proc garrow_int16_array_get_values(ref array : GArrowInt16Array, ref length : gint64) : c_ptr(gint16);
  extern proc garrow_int16_array_get_values(array : c_ptr(GArrowInt16Array), length : c_ptr(gint64)) : c_ptr(gint16);
  extern proc garrow_uint16_array_get_type() : GType;
  extern proc GARROW_UINT16_ARRAY(ptr : gpointer) : c_ptr(GArrowUInt16Array);
  extern proc GARROW_UINT16_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowUInt16ArrayClass);
  extern proc GARROW_IS_UINT16_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT16_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT16_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt16ArrayClass);
  extern proc garrow_uint16_array_new(length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowUInt16Array);
  extern proc garrow_uint16_array_new(length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowUInt16Array);
  extern proc garrow_uint16_array_get_value(ref array : GArrowUInt16Array, i : gint64) : guint16;
  extern proc garrow_uint16_array_get_value(array : c_ptr(GArrowUInt16Array), i : gint64) : guint16;
  extern proc garrow_uint16_array_get_values(ref array : GArrowUInt16Array, ref length : gint64) : c_ptr(guint16);
  extern proc garrow_uint16_array_get_values(array : c_ptr(GArrowUInt16Array), length : c_ptr(gint64)) : c_ptr(guint16);
  extern proc garrow_int32_array_get_type() : GType;
  extern proc GARROW_INT32_ARRAY(ptr : gpointer) : c_ptr(GArrowInt32Array);
  extern proc GARROW_INT32_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowInt32ArrayClass);
  extern proc GARROW_IS_INT32_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT32_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT32_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt32ArrayClass);
  extern proc garrow_int32_array_new(length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowInt32Array);
  extern proc garrow_int32_array_new(length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowInt32Array);
  extern proc garrow_int32_array_get_value(ref array : GArrowInt32Array, i : gint64) : gint32;
  extern proc garrow_int32_array_get_value(array : c_ptr(GArrowInt32Array), i : gint64) : gint32;
  extern proc garrow_int32_array_get_values(ref array : GArrowInt32Array, ref length : gint64) : c_ptr(gint32);
  extern proc garrow_int32_array_get_values(array : c_ptr(GArrowInt32Array), length : c_ptr(gint64)) : c_ptr(gint32);
  extern proc garrow_uint32_array_get_type() : GType;
  extern proc GARROW_UINT32_ARRAY(ptr : gpointer) : c_ptr(GArrowUInt32Array);
  extern proc GARROW_UINT32_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowUInt32ArrayClass);
  extern proc GARROW_IS_UINT32_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT32_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT32_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt32ArrayClass);
  extern proc garrow_uint32_array_new(length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowUInt32Array);
  extern proc garrow_uint32_array_new(length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowUInt32Array);
  extern proc garrow_uint32_array_get_value(ref array : GArrowUInt32Array, i : gint64) : guint32;
  extern proc garrow_uint32_array_get_value(array : c_ptr(GArrowUInt32Array), i : gint64) : guint32;
  extern proc garrow_uint32_array_get_values(ref array : GArrowUInt32Array, ref length : gint64) : c_ptr(guint32);
  extern proc garrow_uint32_array_get_values(array : c_ptr(GArrowUInt32Array), length : c_ptr(gint64)) : c_ptr(guint32);
  extern proc garrow_int64_array_get_type() : GType;
  extern proc GARROW_INT64_ARRAY(ptr : gpointer) : c_ptr(GArrowInt64Array);
  extern proc GARROW_INT64_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowInt64ArrayClass);
  extern proc GARROW_IS_INT64_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT64_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT64_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt64ArrayClass);
  extern proc garrow_int64_array_new(length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowInt64Array);
  extern proc garrow_int64_array_new(length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowInt64Array);
  extern proc garrow_int64_array_get_value(ref array : GArrowInt64Array, i : gint64) : gint64;
  extern proc garrow_int64_array_get_value(array : c_ptr(GArrowInt64Array), i : gint64) : gint64;
  extern proc garrow_int64_array_get_values(ref array : GArrowInt64Array, ref length : gint64) : c_ptr(gint64);
  extern proc garrow_int64_array_get_values(array : c_ptr(GArrowInt64Array), length : c_ptr(gint64)) : c_ptr(gint64);
  extern proc garrow_uint64_array_get_type() : GType;
  extern proc GARROW_UINT64_ARRAY(ptr : gpointer) : c_ptr(GArrowUInt64Array);
  extern proc GARROW_UINT64_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowUInt64ArrayClass);
  extern proc GARROW_IS_UINT64_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT64_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT64_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt64ArrayClass);
  extern proc garrow_uint64_array_new(length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowUInt64Array);
  extern proc garrow_uint64_array_new(length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowUInt64Array);
  extern proc garrow_uint64_array_get_value(ref array : GArrowUInt64Array, i : gint64) : guint64;
  extern proc garrow_uint64_array_get_value(array : c_ptr(GArrowUInt64Array), i : gint64) : guint64;
  extern proc garrow_uint64_array_get_values(ref array : GArrowUInt64Array, ref length : gint64) : c_ptr(guint64);
  extern proc garrow_uint64_array_get_values(array : c_ptr(GArrowUInt64Array), length : c_ptr(gint64)) : c_ptr(guint64);
  extern proc garrow_float_array_get_type() : GType;
  extern proc GARROW_FLOAT_ARRAY(ptr : gpointer) : c_ptr(GArrowFloatArray);
  extern proc GARROW_FLOAT_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowFloatArrayClass);
  extern proc GARROW_IS_FLOAT_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FLOAT_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FLOAT_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFloatArrayClass);
  extern proc garrow_float_array_new(length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowFloatArray);
  extern proc garrow_float_array_new(length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowFloatArray);
  extern proc garrow_float_array_get_value(ref array : GArrowFloatArray, i : gint64) : gfloat;
  extern proc garrow_float_array_get_value(array : c_ptr(GArrowFloatArray), i : gint64) : gfloat;
  extern proc garrow_float_array_get_values(ref array : GArrowFloatArray, ref length : gint64) : c_ptr(gfloat);
  extern proc garrow_float_array_get_values(array : c_ptr(GArrowFloatArray), length : c_ptr(gint64)) : c_ptr(gfloat);
  extern proc garrow_double_array_get_type() : GType;
  extern proc GARROW_DOUBLE_ARRAY(ptr : gpointer) : c_ptr(GArrowDoubleArray);
  extern proc GARROW_DOUBLE_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowDoubleArrayClass);
  extern proc GARROW_IS_DOUBLE_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DOUBLE_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DOUBLE_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDoubleArrayClass);
  extern proc garrow_double_array_new(length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowDoubleArray);
  extern proc garrow_double_array_new(length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowDoubleArray);
  extern proc garrow_double_array_get_value(ref array : GArrowDoubleArray, i : gint64) : gdouble;
  extern proc garrow_double_array_get_value(array : c_ptr(GArrowDoubleArray), i : gint64) : gdouble;
  extern proc garrow_double_array_get_values(ref array : GArrowDoubleArray, ref length : gint64) : c_ptr(gdouble);
  extern proc garrow_double_array_get_values(array : c_ptr(GArrowDoubleArray), length : c_ptr(gint64)) : c_ptr(gdouble);
  extern proc garrow_binary_array_get_type() : GType;
  extern proc GARROW_BINARY_ARRAY(ptr : gpointer) : c_ptr(GArrowBinaryArray);
  extern proc GARROW_BINARY_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowBinaryArrayClass);
  extern proc GARROW_IS_BINARY_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_BINARY_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_BINARY_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowBinaryArrayClass);
  extern proc garrow_binary_array_new(length : gint64, ref value_offsets : GArrowBuffer, ref value_data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowBinaryArray);
  extern proc garrow_binary_array_new(length : gint64, value_offsets : c_ptr(GArrowBuffer), value_data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowBinaryArray);
  extern proc garrow_binary_array_get_value(ref array : GArrowBinaryArray, i : gint64) : c_ptr(GBytes);
  extern proc garrow_binary_array_get_value(array : c_ptr(GArrowBinaryArray), i : gint64) : c_ptr(GBytes);
  extern proc garrow_binary_array_get_buffer(ref array : GArrowBinaryArray) : c_ptr(GArrowBuffer);
  extern proc garrow_binary_array_get_buffer(array : c_ptr(GArrowBinaryArray)) : c_ptr(GArrowBuffer);
  extern proc garrow_binary_array_get_data_buffer(ref array : GArrowBinaryArray) : c_ptr(GArrowBuffer);
  extern proc garrow_binary_array_get_data_buffer(array : c_ptr(GArrowBinaryArray)) : c_ptr(GArrowBuffer);
  extern proc garrow_binary_array_get_offsets_buffer(ref array : GArrowBinaryArray) : c_ptr(GArrowBuffer);
  extern proc garrow_binary_array_get_offsets_buffer(array : c_ptr(GArrowBinaryArray)) : c_ptr(GArrowBuffer);
  extern proc garrow_large_binary_array_get_type() : GType;
  extern proc GARROW_LARGE_BINARY_ARRAY(ptr : gpointer) : c_ptr(GArrowLargeBinaryArray);
  extern proc GARROW_LARGE_BINARY_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowLargeBinaryArrayClass);
  extern proc GARROW_IS_LARGE_BINARY_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LARGE_BINARY_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LARGE_BINARY_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowLargeBinaryArrayClass);
  extern proc garrow_large_binary_array_new(length : gint64, ref value_offsets : GArrowBuffer, ref value_data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowLargeBinaryArray);
  extern proc garrow_large_binary_array_new(length : gint64, value_offsets : c_ptr(GArrowBuffer), value_data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowLargeBinaryArray);
  extern proc garrow_large_binary_array_get_value(ref array : GArrowLargeBinaryArray, i : gint64) : c_ptr(GBytes);
  extern proc garrow_large_binary_array_get_value(array : c_ptr(GArrowLargeBinaryArray), i : gint64) : c_ptr(GBytes);
  extern proc garrow_large_binary_array_get_buffer(ref array : GArrowLargeBinaryArray) : c_ptr(GArrowBuffer);
  extern proc garrow_large_binary_array_get_buffer(array : c_ptr(GArrowLargeBinaryArray)) : c_ptr(GArrowBuffer);
  extern proc garrow_large_binary_array_get_data_buffer(ref array : GArrowLargeBinaryArray) : c_ptr(GArrowBuffer);
  extern proc garrow_large_binary_array_get_data_buffer(array : c_ptr(GArrowLargeBinaryArray)) : c_ptr(GArrowBuffer);
  extern proc garrow_large_binary_array_get_offsets_buffer(ref array : GArrowLargeBinaryArray) : c_ptr(GArrowBuffer);
  extern proc garrow_large_binary_array_get_offsets_buffer(array : c_ptr(GArrowLargeBinaryArray)) : c_ptr(GArrowBuffer);
  extern proc garrow_string_array_get_type() : GType;
  extern proc GARROW_STRING_ARRAY(ptr : gpointer) : c_ptr(GArrowStringArray);
  extern proc GARROW_STRING_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowStringArrayClass);
  extern proc GARROW_IS_STRING_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_STRING_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_STRING_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowStringArrayClass);
  extern proc garrow_string_array_new(length : gint64, ref value_offsets : GArrowBuffer, ref value_data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowStringArray);
  extern proc garrow_string_array_new(length : gint64, value_offsets : c_ptr(GArrowBuffer), value_data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowStringArray);
  extern proc garrow_string_array_get_string(ref array : GArrowStringArray, i : gint64) : c_ptr(gchar);
  extern proc garrow_string_array_get_string(array : c_ptr(GArrowStringArray), i : gint64) : c_ptr(gchar);
  extern proc garrow_large_string_array_get_type() : GType;
  extern proc GARROW_LARGE_STRING_ARRAY(ptr : gpointer) : c_ptr(GArrowLargeStringArray);
  extern proc GARROW_LARGE_STRING_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowLargeStringArrayClass);
  extern proc GARROW_IS_LARGE_STRING_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LARGE_STRING_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LARGE_STRING_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowLargeStringArrayClass);
  extern proc garrow_large_string_array_new(length : gint64, ref value_offsets : GArrowBuffer, ref value_data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowLargeStringArray);
  extern proc garrow_large_string_array_new(length : gint64, value_offsets : c_ptr(GArrowBuffer), value_data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowLargeStringArray);
  extern proc garrow_large_string_array_get_string(ref array : GArrowLargeStringArray, i : gint64) : c_ptr(gchar);
  extern proc garrow_large_string_array_get_string(array : c_ptr(GArrowLargeStringArray), i : gint64) : c_ptr(gchar);
  extern proc garrow_date32_array_get_type() : GType;
  extern proc GARROW_DATE32_ARRAY(ptr : gpointer) : c_ptr(GArrowDate32Array);
  extern proc GARROW_DATE32_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowDate32ArrayClass);
  extern proc GARROW_IS_DATE32_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DATE32_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DATE32_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDate32ArrayClass);
  extern proc garrow_date32_array_new(length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowDate32Array);
  extern proc garrow_date32_array_new(length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowDate32Array);
  extern proc garrow_date32_array_get_value(ref array : GArrowDate32Array, i : gint64) : gint32;
  extern proc garrow_date32_array_get_value(array : c_ptr(GArrowDate32Array), i : gint64) : gint32;
  extern proc garrow_date32_array_get_values(ref array : GArrowDate32Array, ref length : gint64) : c_ptr(gint32);
  extern proc garrow_date32_array_get_values(array : c_ptr(GArrowDate32Array), length : c_ptr(gint64)) : c_ptr(gint32);
  extern proc garrow_date64_array_get_type() : GType;
  extern proc GARROW_DATE64_ARRAY(ptr : gpointer) : c_ptr(GArrowDate64Array);
  extern proc GARROW_DATE64_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowDate64ArrayClass);
  extern proc GARROW_IS_DATE64_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DATE64_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DATE64_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDate64ArrayClass);
  extern proc garrow_date64_array_new(length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowDate64Array);
  extern proc garrow_date64_array_new(length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowDate64Array);
  extern proc garrow_date64_array_get_value(ref array : GArrowDate64Array, i : gint64) : gint64;
  extern proc garrow_date64_array_get_value(array : c_ptr(GArrowDate64Array), i : gint64) : gint64;
  extern proc garrow_date64_array_get_values(ref array : GArrowDate64Array, ref length : gint64) : c_ptr(gint64);
  extern proc garrow_date64_array_get_values(array : c_ptr(GArrowDate64Array), length : c_ptr(gint64)) : c_ptr(gint64);
  extern proc garrow_timestamp_array_get_type() : GType;
  extern proc GARROW_TIMESTAMP_ARRAY(ptr : gpointer) : c_ptr(GArrowTimestampArray);
  extern proc GARROW_TIMESTAMP_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowTimestampArrayClass);
  extern proc GARROW_IS_TIMESTAMP_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TIMESTAMP_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TIMESTAMP_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTimestampArrayClass);
  extern proc garrow_timestamp_array_new(ref data_type : GArrowTimestampDataType, length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowTimestampArray);
  extern proc garrow_timestamp_array_new(data_type : c_ptr(GArrowTimestampDataType), length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowTimestampArray);
  extern proc garrow_timestamp_array_get_value(ref array : GArrowTimestampArray, i : gint64) : gint64;
  extern proc garrow_timestamp_array_get_value(array : c_ptr(GArrowTimestampArray), i : gint64) : gint64;
  extern proc garrow_timestamp_array_get_values(ref array : GArrowTimestampArray, ref length : gint64) : c_ptr(gint64);
  extern proc garrow_timestamp_array_get_values(array : c_ptr(GArrowTimestampArray), length : c_ptr(gint64)) : c_ptr(gint64);
  extern proc garrow_time32_array_get_type() : GType;
  extern proc GARROW_TIME32_ARRAY(ptr : gpointer) : c_ptr(GArrowTime32Array);
  extern proc GARROW_TIME32_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowTime32ArrayClass);
  extern proc GARROW_IS_TIME32_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TIME32_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TIME32_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTime32ArrayClass);
  extern proc garrow_time32_array_new(ref data_type : GArrowTime32DataType, length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowTime32Array);
  extern proc garrow_time32_array_new(data_type : c_ptr(GArrowTime32DataType), length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowTime32Array);
  extern proc garrow_time32_array_get_value(ref array : GArrowTime32Array, i : gint64) : gint32;
  extern proc garrow_time32_array_get_value(array : c_ptr(GArrowTime32Array), i : gint64) : gint32;
  extern proc garrow_time32_array_get_values(ref array : GArrowTime32Array, ref length : gint64) : c_ptr(gint32);
  extern proc garrow_time32_array_get_values(array : c_ptr(GArrowTime32Array), length : c_ptr(gint64)) : c_ptr(gint32);
  extern proc garrow_time64_array_get_type() : GType;
  extern proc GARROW_TIME64_ARRAY(ptr : gpointer) : c_ptr(GArrowTime64Array);
  extern proc GARROW_TIME64_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowTime64ArrayClass);
  extern proc GARROW_IS_TIME64_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TIME64_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TIME64_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTime64ArrayClass);
  extern proc garrow_time64_array_new(ref data_type : GArrowTime64DataType, length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowTime64Array);
  extern proc garrow_time64_array_new(data_type : c_ptr(GArrowTime64DataType), length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowTime64Array);
  extern proc garrow_time64_array_get_value(ref array : GArrowTime64Array, i : gint64) : gint64;
  extern proc garrow_time64_array_get_value(array : c_ptr(GArrowTime64Array), i : gint64) : gint64;
  extern proc garrow_time64_array_get_values(ref array : GArrowTime64Array, ref length : gint64) : c_ptr(gint64);
  extern proc garrow_time64_array_get_values(array : c_ptr(GArrowTime64Array), length : c_ptr(gint64)) : c_ptr(gint64);
  extern proc garrow_fixed_size_binary_array_get_type() : GType;
  extern proc GARROW_FIXED_SIZE_BINARY_ARRAY(ptr : gpointer) : c_ptr(GArrowFixedSizeBinaryArray);
  extern proc GARROW_FIXED_SIZE_BINARY_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowFixedSizeBinaryArrayClass);
  extern proc GARROW_IS_FIXED_SIZE_BINARY_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FIXED_SIZE_BINARY_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FIXED_SIZE_BINARY_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFixedSizeBinaryArrayClass);
  extern proc garrow_fixed_size_binary_array_new(ref data_type : GArrowFixedSizeBinaryDataType, length : gint64, ref data : GArrowBuffer, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowFixedSizeBinaryArray);
  extern proc garrow_fixed_size_binary_array_new(data_type : c_ptr(GArrowFixedSizeBinaryDataType), length : gint64, data : c_ptr(GArrowBuffer), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowFixedSizeBinaryArray);
  extern proc garrow_fixed_size_binary_array_get_byte_width(ref array : GArrowFixedSizeBinaryArray) : gint32;
  extern proc garrow_fixed_size_binary_array_get_byte_width(array : c_ptr(GArrowFixedSizeBinaryArray)) : gint32;
  extern proc garrow_fixed_size_binary_array_get_value(ref array : GArrowFixedSizeBinaryArray, i : gint64) : c_ptr(GBytes);
  extern proc garrow_fixed_size_binary_array_get_value(array : c_ptr(GArrowFixedSizeBinaryArray), i : gint64) : c_ptr(GBytes);
  extern proc garrow_fixed_size_binary_array_get_values_bytes(ref array : GArrowFixedSizeBinaryArray) : c_ptr(GBytes);
  extern proc garrow_fixed_size_binary_array_get_values_bytes(array : c_ptr(GArrowFixedSizeBinaryArray)) : c_ptr(GBytes);
  extern proc garrow_decimal128_array_get_type() : GType;
  extern proc GARROW_DECIMAL128_ARRAY(ptr : gpointer) : c_ptr(GArrowDecimal128Array);
  extern proc GARROW_DECIMAL128_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal128ArrayClass);
  extern proc GARROW_IS_DECIMAL128_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DECIMAL128_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DECIMAL128_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal128ArrayClass);
  extern proc garrow_decimal128_array_format_value(ref array : GArrowDecimal128Array, i : gint64) : c_ptr(gchar);
  extern proc garrow_decimal128_array_format_value(array : c_ptr(GArrowDecimal128Array), i : gint64) : c_ptr(gchar);
  extern proc garrow_decimal128_array_get_value(ref array : GArrowDecimal128Array, i : gint64) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_array_get_value(array : c_ptr(GArrowDecimal128Array), i : gint64) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal256_array_get_type() : GType;
  extern proc GARROW_DECIMAL256_ARRAY(ptr : gpointer) : c_ptr(GArrowDecimal256Array);
  extern proc GARROW_DECIMAL256_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal256ArrayClass);
  extern proc GARROW_IS_DECIMAL256_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DECIMAL256_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DECIMAL256_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal256ArrayClass);
  extern proc garrow_decimal256_array_format_value(ref array : GArrowDecimal256Array, i : gint64) : c_ptr(gchar);
  extern proc garrow_decimal256_array_format_value(array : c_ptr(GArrowDecimal256Array), i : gint64) : c_ptr(gchar);
  extern proc garrow_decimal256_array_get_value(ref array : GArrowDecimal256Array, i : gint64) : c_ptr(GArrowDecimal256);
  extern proc garrow_decimal256_array_get_value(array : c_ptr(GArrowDecimal256Array), i : gint64) : c_ptr(GArrowDecimal256);
  extern proc garrow_extension_array_get_type() : GType;
  extern proc GARROW_EXTENSION_ARRAY(ptr : gpointer) : c_ptr(GArrowExtensionArray);
  extern proc GARROW_EXTENSION_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowExtensionArrayClass);
  extern proc GARROW_IS_EXTENSION_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_EXTENSION_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_EXTENSION_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowExtensionArrayClass);
  extern proc garrow_extension_array_get_storage(ref array : GArrowExtensionArray) : c_ptr(GArrowArray);
  extern proc garrow_extension_array_get_storage(array : c_ptr(GArrowExtensionArray)) : c_ptr(GArrowArray);
  extern proc garrow_field_get_type() : GType;
  extern proc GARROW_FIELD(ptr : gpointer) : c_ptr(GArrowField);
  extern proc GARROW_FIELD_CLASS(ptr : gpointer) : c_ptr(GArrowFieldClass);
  extern proc GARROW_IS_FIELD(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FIELD_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FIELD_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFieldClass);
  extern proc garrow_field_new(ref name : gchar, ref data_type : GArrowDataType) : c_ptr(GArrowField);
  extern proc garrow_field_new(name : c_ptr(gchar), data_type : c_ptr(GArrowDataType)) : c_ptr(GArrowField);
  extern proc garrow_field_new_full(ref name : gchar, ref data_type : GArrowDataType, nullable : gboolean) : c_ptr(GArrowField);
  extern proc garrow_field_new_full(name : c_ptr(gchar), data_type : c_ptr(GArrowDataType), nullable : gboolean) : c_ptr(GArrowField);
  extern proc garrow_field_get_name(ref field : GArrowField) : c_ptr(gchar);
  extern proc garrow_field_get_name(field : c_ptr(GArrowField)) : c_ptr(gchar);
  extern proc garrow_field_get_data_type(ref field : GArrowField) : c_ptr(GArrowDataType);
  extern proc garrow_field_get_data_type(field : c_ptr(GArrowField)) : c_ptr(GArrowDataType);
  extern proc garrow_field_is_nullable(ref field : GArrowField) : gboolean;
  extern proc garrow_field_is_nullable(field : c_ptr(GArrowField)) : gboolean;
  extern proc garrow_field_equal(ref field : GArrowField, ref other_field : GArrowField) : gboolean;
  extern proc garrow_field_equal(field : c_ptr(GArrowField), other_field : c_ptr(GArrowField)) : gboolean;
  extern proc garrow_field_to_string(ref field : GArrowField) : c_ptr(gchar);
  extern proc garrow_field_to_string(field : c_ptr(GArrowField)) : c_ptr(gchar);
  extern proc garrow_field_to_string_metadata(ref field : GArrowField, show_metadata : gboolean) : c_ptr(gchar);
  extern proc garrow_field_to_string_metadata(field : c_ptr(GArrowField), show_metadata : gboolean) : c_ptr(gchar);
  extern proc garrow_field_has_metadata(ref field : GArrowField) : gboolean;
  extern proc garrow_field_has_metadata(field : c_ptr(GArrowField)) : gboolean;
  extern proc garrow_field_get_metadata(ref field : GArrowField) : c_ptr(GHashTable);
  extern proc garrow_field_get_metadata(field : c_ptr(GArrowField)) : c_ptr(GHashTable);
  extern proc garrow_field_with_metadata(ref field : GArrowField, ref metadata : GHashTable) : c_ptr(GArrowField);
  extern proc garrow_field_with_metadata(field : c_ptr(GArrowField), metadata : c_ptr(GHashTable)) : c_ptr(GArrowField);
  extern proc garrow_field_with_merged_metadata(ref field : GArrowField, ref metadata : GHashTable) : c_ptr(GArrowField);
  extern proc garrow_field_with_merged_metadata(field : c_ptr(GArrowField), metadata : c_ptr(GHashTable)) : c_ptr(GArrowField);
  extern proc garrow_field_remove_metadata(ref field : GArrowField) : c_ptr(GArrowField);
  extern proc garrow_field_remove_metadata(field : c_ptr(GArrowField)) : c_ptr(GArrowField);
  extern proc garrow_list_data_type_get_type() : GType;
  extern proc GARROW_LIST_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowListDataType);
  extern proc GARROW_LIST_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowListDataTypeClass);
  extern proc GARROW_IS_LIST_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LIST_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LIST_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowListDataTypeClass);
  extern proc garrow_list_data_type_new(ref field : GArrowField) : c_ptr(GArrowListDataType);
  extern proc garrow_list_data_type_new(field : c_ptr(GArrowField)) : c_ptr(GArrowListDataType);
  extern proc garrow_list_data_type_get_value_field(ref list_data_type : GArrowListDataType) : c_ptr(GArrowField);
  extern proc garrow_list_data_type_get_value_field(list_data_type : c_ptr(GArrowListDataType)) : c_ptr(GArrowField);
  extern proc garrow_list_data_type_get_field(ref list_data_type : GArrowListDataType) : c_ptr(GArrowField);
  extern proc garrow_list_data_type_get_field(list_data_type : c_ptr(GArrowListDataType)) : c_ptr(GArrowField);
  extern proc garrow_large_list_data_type_get_type() : GType;
  extern proc GARROW_LARGE_LIST_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowLargeListDataType);
  extern proc GARROW_LARGE_LIST_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowLargeListDataTypeClass);
  extern proc GARROW_IS_LARGE_LIST_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LARGE_LIST_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LARGE_LIST_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowLargeListDataTypeClass);
  extern proc garrow_large_list_data_type_new(ref field : GArrowField) : c_ptr(GArrowLargeListDataType);
  extern proc garrow_large_list_data_type_new(field : c_ptr(GArrowField)) : c_ptr(GArrowLargeListDataType);
  extern proc garrow_large_list_data_type_get_field(ref large_list_data_type : GArrowLargeListDataType) : c_ptr(GArrowField);
  extern proc garrow_large_list_data_type_get_field(large_list_data_type : c_ptr(GArrowLargeListDataType)) : c_ptr(GArrowField);
  extern proc garrow_struct_data_type_get_type() : GType;
  extern proc GARROW_STRUCT_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowStructDataType);
  extern proc GARROW_STRUCT_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowStructDataTypeClass);
  extern proc GARROW_IS_STRUCT_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_STRUCT_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_STRUCT_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowStructDataTypeClass);
  extern proc garrow_struct_data_type_new(ref fields : GList) : c_ptr(GArrowStructDataType);
  extern proc garrow_struct_data_type_new(fields : c_ptr(GList)) : c_ptr(GArrowStructDataType);
  extern proc garrow_struct_data_type_get_n_fields(ref struct_data_type : GArrowStructDataType) : gint;
  extern proc garrow_struct_data_type_get_n_fields(struct_data_type : c_ptr(GArrowStructDataType)) : gint;
  extern proc garrow_struct_data_type_get_fields(ref struct_data_type : GArrowStructDataType) : c_ptr(GList);
  extern proc garrow_struct_data_type_get_fields(struct_data_type : c_ptr(GArrowStructDataType)) : c_ptr(GList);
  extern proc garrow_struct_data_type_get_field(ref struct_data_type : GArrowStructDataType, i : gint) : c_ptr(GArrowField);
  extern proc garrow_struct_data_type_get_field(struct_data_type : c_ptr(GArrowStructDataType), i : gint) : c_ptr(GArrowField);
  extern proc garrow_struct_data_type_get_field_by_name(ref struct_data_type : GArrowStructDataType, ref name : gchar) : c_ptr(GArrowField);
  extern proc garrow_struct_data_type_get_field_by_name(struct_data_type : c_ptr(GArrowStructDataType), name : c_ptr(gchar)) : c_ptr(GArrowField);
  extern proc garrow_struct_data_type_get_field_index(ref struct_data_type : GArrowStructDataType, ref name : gchar) : gint;
  extern proc garrow_struct_data_type_get_field_index(struct_data_type : c_ptr(GArrowStructDataType), name : c_ptr(gchar)) : gint;
  extern proc garrow_map_data_type_get_type() : GType;
  extern proc GARROW_MAP_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowMapDataType);
  extern proc GARROW_MAP_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowMapDataTypeClass);
  extern proc GARROW_IS_MAP_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_MAP_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_MAP_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowMapDataTypeClass);
  extern proc garrow_map_data_type_new(ref key_type : GArrowDataType, ref item_type : GArrowDataType) : c_ptr(GArrowMapDataType);
  extern proc garrow_map_data_type_new(key_type : c_ptr(GArrowDataType), item_type : c_ptr(GArrowDataType)) : c_ptr(GArrowMapDataType);
  extern proc garrow_map_data_type_get_key_type(ref map_data_type : GArrowMapDataType) : c_ptr(GArrowDataType);
  extern proc garrow_map_data_type_get_key_type(map_data_type : c_ptr(GArrowMapDataType)) : c_ptr(GArrowDataType);
  extern proc garrow_map_data_type_get_item_type(ref map_data_type : GArrowMapDataType) : c_ptr(GArrowDataType);
  extern proc garrow_map_data_type_get_item_type(map_data_type : c_ptr(GArrowMapDataType)) : c_ptr(GArrowDataType);
  extern proc garrow_union_data_type_get_type() : GType;
  extern proc GARROW_UNION_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowUnionDataType);
  extern proc GARROW_UNION_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowUnionDataTypeClass);
  extern proc GARROW_IS_UNION_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UNION_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UNION_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUnionDataTypeClass);
  extern proc garrow_union_data_type_get_n_fields(ref union_data_type : GArrowUnionDataType) : gint;
  extern proc garrow_union_data_type_get_n_fields(union_data_type : c_ptr(GArrowUnionDataType)) : gint;
  extern proc garrow_union_data_type_get_fields(ref union_data_type : GArrowUnionDataType) : c_ptr(GList);
  extern proc garrow_union_data_type_get_fields(union_data_type : c_ptr(GArrowUnionDataType)) : c_ptr(GList);
  extern proc garrow_union_data_type_get_field(ref union_data_type : GArrowUnionDataType, i : gint) : c_ptr(GArrowField);
  extern proc garrow_union_data_type_get_field(union_data_type : c_ptr(GArrowUnionDataType), i : gint) : c_ptr(GArrowField);
  extern proc garrow_union_data_type_get_type_codes(ref union_data_type : GArrowUnionDataType, ref n_type_codes : gsize) : c_ptr(gint8);
  extern proc garrow_union_data_type_get_type_codes(union_data_type : c_ptr(GArrowUnionDataType), n_type_codes : c_ptr(gsize)) : c_ptr(gint8);
  extern proc garrow_sparse_union_data_type_get_type() : GType;
  extern proc GARROW_SPARSE_UNION_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowSparseUnionDataType);
  extern proc GARROW_SPARSE_UNION_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowSparseUnionDataTypeClass);
  extern proc GARROW_IS_SPARSE_UNION_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_SPARSE_UNION_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_SPARSE_UNION_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowSparseUnionDataTypeClass);
  extern proc garrow_sparse_union_data_type_new(ref fields : GList, ref type_codes : gint8, n_type_codes : gsize) : c_ptr(GArrowSparseUnionDataType);
  extern proc garrow_sparse_union_data_type_new(fields : c_ptr(GList), type_codes : c_ptr(gint8), n_type_codes : gsize) : c_ptr(GArrowSparseUnionDataType);
  extern proc garrow_dense_union_data_type_get_type() : GType;
  extern proc GARROW_DENSE_UNION_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowDenseUnionDataType);
  extern proc GARROW_DENSE_UNION_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowDenseUnionDataTypeClass);
  extern proc GARROW_IS_DENSE_UNION_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DENSE_UNION_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DENSE_UNION_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDenseUnionDataTypeClass);
  extern proc garrow_dense_union_data_type_new(ref fields : GList, ref type_codes : gint8, n_type_codes : gsize) : c_ptr(GArrowDenseUnionDataType);
  extern proc garrow_dense_union_data_type_new(fields : c_ptr(GList), type_codes : c_ptr(gint8), n_type_codes : gsize) : c_ptr(GArrowDenseUnionDataType);
  extern proc garrow_dictionary_data_type_get_type() : GType;
  extern proc GARROW_DICTIONARY_DATA_TYPE(ptr : gpointer) : c_ptr(GArrowDictionaryDataType);
  extern proc GARROW_DICTIONARY_DATA_TYPE_CLASS(ptr : gpointer) : c_ptr(GArrowDictionaryDataTypeClass);
  extern proc GARROW_IS_DICTIONARY_DATA_TYPE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DICTIONARY_DATA_TYPE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DICTIONARY_DATA_TYPE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDictionaryDataTypeClass);
  extern proc garrow_dictionary_data_type_new(ref index_data_type : GArrowDataType, ref value_data_type : GArrowDataType, ordered : gboolean) : c_ptr(GArrowDictionaryDataType);
  extern proc garrow_dictionary_data_type_new(index_data_type : c_ptr(GArrowDataType), value_data_type : c_ptr(GArrowDataType), ordered : gboolean) : c_ptr(GArrowDictionaryDataType);
  extern proc garrow_dictionary_data_type_get_index_data_type(ref dictionary_data_type : GArrowDictionaryDataType) : c_ptr(GArrowDataType);
  extern proc garrow_dictionary_data_type_get_index_data_type(dictionary_data_type : c_ptr(GArrowDictionaryDataType)) : c_ptr(GArrowDataType);
  extern proc garrow_dictionary_data_type_get_value_data_type(ref dictionary_data_type : GArrowDictionaryDataType) : c_ptr(GArrowDataType);
  extern proc garrow_dictionary_data_type_get_value_data_type(dictionary_data_type : c_ptr(GArrowDictionaryDataType)) : c_ptr(GArrowDataType);
  extern proc garrow_dictionary_data_type_is_ordered(ref dictionary_data_type : GArrowDictionaryDataType) : gboolean;
  extern proc garrow_dictionary_data_type_is_ordered(dictionary_data_type : c_ptr(GArrowDictionaryDataType)) : gboolean;
  extern proc garrow_list_array_get_type() : GType;
  extern proc GARROW_LIST_ARRAY(ptr : gpointer) : c_ptr(GArrowListArray);
  extern proc GARROW_LIST_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowListArrayClass);
  extern proc GARROW_IS_LIST_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LIST_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LIST_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowListArrayClass);
  extern proc garrow_list_array_new(ref data_type : GArrowDataType, length : gint64, ref value_offsets : GArrowBuffer, ref values : GArrowArray, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowListArray);
  extern proc garrow_list_array_new(data_type : c_ptr(GArrowDataType), length : gint64, value_offsets : c_ptr(GArrowBuffer), values : c_ptr(GArrowArray), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowListArray);
  extern proc garrow_list_array_get_value_type(ref array : GArrowListArray) : c_ptr(GArrowDataType);
  extern proc garrow_list_array_get_value_type(array : c_ptr(GArrowListArray)) : c_ptr(GArrowDataType);
  extern proc garrow_list_array_get_value(ref array : GArrowListArray, i : gint64) : c_ptr(GArrowArray);
  extern proc garrow_list_array_get_value(array : c_ptr(GArrowListArray), i : gint64) : c_ptr(GArrowArray);
  extern proc garrow_list_array_get_values(ref array : GArrowListArray) : c_ptr(GArrowArray);
  extern proc garrow_list_array_get_values(array : c_ptr(GArrowListArray)) : c_ptr(GArrowArray);
  extern proc garrow_list_array_get_value_offset(ref array : GArrowListArray, i : gint64) : gint32;
  extern proc garrow_list_array_get_value_offset(array : c_ptr(GArrowListArray), i : gint64) : gint32;
  extern proc garrow_list_array_get_value_length(ref array : GArrowListArray, i : gint64) : gint32;
  extern proc garrow_list_array_get_value_length(array : c_ptr(GArrowListArray), i : gint64) : gint32;
  extern proc garrow_list_array_get_value_offsets(ref array : GArrowListArray, ref n_offsets : gint64) : c_ptr(gint32);
  extern proc garrow_list_array_get_value_offsets(array : c_ptr(GArrowListArray), n_offsets : c_ptr(gint64)) : c_ptr(gint32);
  extern proc garrow_large_list_array_get_type() : GType;
  extern proc GARROW_LARGE_LIST_ARRAY(ptr : gpointer) : c_ptr(GArrowLargeListArray);
  extern proc GARROW_LARGE_LIST_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowLargeListArrayClass);
  extern proc GARROW_IS_LARGE_LIST_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LARGE_LIST_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LARGE_LIST_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowLargeListArrayClass);
  extern proc garrow_large_list_array_new(ref data_type : GArrowDataType, length : gint64, ref value_offsets : GArrowBuffer, ref values : GArrowArray, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowLargeListArray);
  extern proc garrow_large_list_array_new(data_type : c_ptr(GArrowDataType), length : gint64, value_offsets : c_ptr(GArrowBuffer), values : c_ptr(GArrowArray), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowLargeListArray);
  extern proc garrow_large_list_array_get_value_type(ref array : GArrowLargeListArray) : c_ptr(GArrowDataType);
  extern proc garrow_large_list_array_get_value_type(array : c_ptr(GArrowLargeListArray)) : c_ptr(GArrowDataType);
  extern proc garrow_large_list_array_get_value(ref array : GArrowLargeListArray, i : gint64) : c_ptr(GArrowArray);
  extern proc garrow_large_list_array_get_value(array : c_ptr(GArrowLargeListArray), i : gint64) : c_ptr(GArrowArray);
  extern proc garrow_large_list_array_get_values(ref array : GArrowLargeListArray) : c_ptr(GArrowArray);
  extern proc garrow_large_list_array_get_values(array : c_ptr(GArrowLargeListArray)) : c_ptr(GArrowArray);
  extern proc garrow_large_list_array_get_value_offset(ref array : GArrowLargeListArray, i : gint64) : gint64;
  extern proc garrow_large_list_array_get_value_offset(array : c_ptr(GArrowLargeListArray), i : gint64) : gint64;
  extern proc garrow_large_list_array_get_value_length(ref array : GArrowLargeListArray, i : gint64) : gint64;
  extern proc garrow_large_list_array_get_value_length(array : c_ptr(GArrowLargeListArray), i : gint64) : gint64;
  extern proc garrow_large_list_array_get_value_offsets(ref array : GArrowLargeListArray, ref n_offsets : gint64) : c_ptr(gint64);
  extern proc garrow_large_list_array_get_value_offsets(array : c_ptr(GArrowLargeListArray), n_offsets : c_ptr(gint64)) : c_ptr(gint64);
  extern proc garrow_struct_array_get_type() : GType;
  extern proc GARROW_STRUCT_ARRAY(ptr : gpointer) : c_ptr(GArrowStructArray);
  extern proc GARROW_STRUCT_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowStructArrayClass);
  extern proc GARROW_IS_STRUCT_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_STRUCT_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_STRUCT_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowStructArrayClass);
  extern proc garrow_struct_array_new(ref data_type : GArrowDataType, length : gint64, ref fields : GList, ref null_bitmap : GArrowBuffer, n_nulls : gint64) : c_ptr(GArrowStructArray);
  extern proc garrow_struct_array_new(data_type : c_ptr(GArrowDataType), length : gint64, fields : c_ptr(GList), null_bitmap : c_ptr(GArrowBuffer), n_nulls : gint64) : c_ptr(GArrowStructArray);
  extern proc garrow_struct_array_get_field(ref array : GArrowStructArray, i : gint) : c_ptr(GArrowArray);
  extern proc garrow_struct_array_get_field(array : c_ptr(GArrowStructArray), i : gint) : c_ptr(GArrowArray);
  extern proc garrow_struct_array_get_fields(ref array : GArrowStructArray) : c_ptr(GList);
  extern proc garrow_struct_array_get_fields(array : c_ptr(GArrowStructArray)) : c_ptr(GList);
  extern proc garrow_struct_array_flatten(ref array : GArrowStructArray, ref error : c_ptr(GError)) : c_ptr(GList);
  extern proc garrow_struct_array_flatten(array : c_ptr(GArrowStructArray), error : c_ptr(c_ptr(GError))) : c_ptr(GList);
  extern proc garrow_map_array_get_type() : GType;
  extern proc GARROW_MAP_ARRAY(ptr : gpointer) : c_ptr(GArrowMapArray);
  extern proc GARROW_MAP_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowMapArrayClass);
  extern proc GARROW_IS_MAP_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_MAP_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_MAP_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowMapArrayClass);
  extern proc garrow_map_array_new(ref offsets : GArrowArray, ref keys : GArrowArray, ref items : GArrowArray, ref error : c_ptr(GError)) : c_ptr(GArrowMapArray);
  extern proc garrow_map_array_new(offsets : c_ptr(GArrowArray), keys : c_ptr(GArrowArray), items : c_ptr(GArrowArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowMapArray);
  extern proc garrow_map_array_get_keys(ref array : GArrowMapArray) : c_ptr(GArrowArray);
  extern proc garrow_map_array_get_keys(array : c_ptr(GArrowMapArray)) : c_ptr(GArrowArray);
  extern proc garrow_map_array_get_items(ref array : GArrowMapArray) : c_ptr(GArrowArray);
  extern proc garrow_map_array_get_items(array : c_ptr(GArrowMapArray)) : c_ptr(GArrowArray);
  extern proc garrow_union_array_get_type() : GType;
  extern proc GARROW_UNION_ARRAY(ptr : gpointer) : c_ptr(GArrowUnionArray);
  extern proc GARROW_UNION_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowUnionArrayClass);
  extern proc GARROW_IS_UNION_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UNION_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UNION_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUnionArrayClass);
  extern proc garrow_union_array_get_field(ref array : GArrowUnionArray, i : gint) : c_ptr(GArrowArray);
  extern proc garrow_union_array_get_field(array : c_ptr(GArrowUnionArray), i : gint) : c_ptr(GArrowArray);
  extern proc garrow_sparse_union_array_get_type() : GType;
  extern proc GARROW_SPARSE_UNION_ARRAY(ptr : gpointer) : c_ptr(GArrowSparseUnionArray);
  extern proc GARROW_SPARSE_UNION_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowSparseUnionArrayClass);
  extern proc GARROW_IS_SPARSE_UNION_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_SPARSE_UNION_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_SPARSE_UNION_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowSparseUnionArrayClass);
  extern proc garrow_sparse_union_array_new(ref type_ids : GArrowInt8Array, ref fields : GList, ref error : c_ptr(GError)) : c_ptr(GArrowSparseUnionArray);
  extern proc garrow_sparse_union_array_new(type_ids : c_ptr(GArrowInt8Array), fields : c_ptr(GList), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowSparseUnionArray);
  extern proc garrow_sparse_union_array_new_data_type(ref data_type : GArrowSparseUnionDataType, ref type_ids : GArrowInt8Array, ref fields : GList, ref error : c_ptr(GError)) : c_ptr(GArrowSparseUnionArray);
  extern proc garrow_sparse_union_array_new_data_type(data_type : c_ptr(GArrowSparseUnionDataType), type_ids : c_ptr(GArrowInt8Array), fields : c_ptr(GList), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowSparseUnionArray);
  extern proc garrow_dense_union_array_get_type() : GType;
  extern proc GARROW_DENSE_UNION_ARRAY(ptr : gpointer) : c_ptr(GArrowDenseUnionArray);
  extern proc GARROW_DENSE_UNION_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowDenseUnionArrayClass);
  extern proc GARROW_IS_DENSE_UNION_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DENSE_UNION_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DENSE_UNION_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDenseUnionArrayClass);
  extern proc garrow_dense_union_array_new(ref type_ids : GArrowInt8Array, ref value_offsets : GArrowInt32Array, ref fields : GList, ref error : c_ptr(GError)) : c_ptr(GArrowDenseUnionArray);
  extern proc garrow_dense_union_array_new(type_ids : c_ptr(GArrowInt8Array), value_offsets : c_ptr(GArrowInt32Array), fields : c_ptr(GList), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowDenseUnionArray);
  extern proc garrow_dense_union_array_new_data_type(ref data_type : GArrowDenseUnionDataType, ref type_ids : GArrowInt8Array, ref value_offsets : GArrowInt32Array, ref fields : GList, ref error : c_ptr(GError)) : c_ptr(GArrowDenseUnionArray);
  extern proc garrow_dense_union_array_new_data_type(data_type : c_ptr(GArrowDenseUnionDataType), type_ids : c_ptr(GArrowInt8Array), value_offsets : c_ptr(GArrowInt32Array), fields : c_ptr(GList), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowDenseUnionArray);
  extern proc garrow_dictionary_array_get_type() : GType;
  extern proc GARROW_DICTIONARY_ARRAY(ptr : gpointer) : c_ptr(GArrowDictionaryArray);
  extern proc GARROW_DICTIONARY_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowDictionaryArrayClass);
  extern proc GARROW_IS_DICTIONARY_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DICTIONARY_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DICTIONARY_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDictionaryArrayClass);
  extern proc garrow_dictionary_array_new(ref data_type : GArrowDataType, ref indices : GArrowArray, ref dictionary : GArrowArray, ref error : c_ptr(GError)) : c_ptr(GArrowDictionaryArray);
  extern proc garrow_dictionary_array_new(data_type : c_ptr(GArrowDataType), indices : c_ptr(GArrowArray), dictionary : c_ptr(GArrowArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowDictionaryArray);
  extern proc garrow_dictionary_array_get_indices(ref array : GArrowDictionaryArray) : c_ptr(GArrowArray);
  extern proc garrow_dictionary_array_get_indices(array : c_ptr(GArrowDictionaryArray)) : c_ptr(GArrowArray);
  extern proc garrow_dictionary_array_get_dictionary(ref array : GArrowDictionaryArray) : c_ptr(GArrowArray);
  extern proc garrow_dictionary_array_get_dictionary(array : c_ptr(GArrowDictionaryArray)) : c_ptr(GArrowArray);
  extern proc garrow_dictionary_array_get_dictionary_data_type(ref array : GArrowDictionaryArray) : c_ptr(GArrowDictionaryDataType);
  extern proc garrow_dictionary_array_get_dictionary_data_type(array : c_ptr(GArrowDictionaryArray)) : c_ptr(GArrowDictionaryDataType);
  extern proc garrow_array_builder_get_type() : GType;
  extern proc GARROW_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowArrayBuilder);
  extern proc GARROW_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowArrayBuilderClass);
  extern proc GARROW_IS_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowArrayBuilderClass);
  extern proc garrow_array_builder_release_ownership(ref builder : GArrowArrayBuilder) : void;
  extern proc garrow_array_builder_release_ownership(builder : c_ptr(GArrowArrayBuilder)) : void;
  extern proc garrow_array_builder_get_value_data_type(ref builder : GArrowArrayBuilder) : c_ptr(GArrowDataType);
  extern proc garrow_array_builder_get_value_data_type(builder : c_ptr(GArrowArrayBuilder)) : c_ptr(GArrowDataType);
  extern proc garrow_array_builder_get_value_type(ref builder : GArrowArrayBuilder) : GArrowType;
  extern proc garrow_array_builder_get_value_type(builder : c_ptr(GArrowArrayBuilder)) : GArrowType;
  extern proc garrow_array_builder_finish(ref builder : GArrowArrayBuilder, ref error : c_ptr(GError)) : c_ptr(GArrowArray);
  extern proc garrow_array_builder_finish(builder : c_ptr(GArrowArrayBuilder), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowArray);
  extern proc garrow_array_builder_reset(ref builder : GArrowArrayBuilder) : void;
  extern proc garrow_array_builder_reset(builder : c_ptr(GArrowArrayBuilder)) : void;
  extern proc garrow_array_builder_get_capacity(ref builder : GArrowArrayBuilder) : gint64;
  extern proc garrow_array_builder_get_capacity(builder : c_ptr(GArrowArrayBuilder)) : gint64;
  extern proc garrow_array_builder_get_length(ref builder : GArrowArrayBuilder) : gint64;
  extern proc garrow_array_builder_get_length(builder : c_ptr(GArrowArrayBuilder)) : gint64;
  extern proc garrow_array_builder_get_n_nulls(ref builder : GArrowArrayBuilder) : gint64;
  extern proc garrow_array_builder_get_n_nulls(builder : c_ptr(GArrowArrayBuilder)) : gint64;
  extern proc garrow_array_builder_resize(ref builder : GArrowArrayBuilder, capacity : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_array_builder_resize(builder : c_ptr(GArrowArrayBuilder), capacity : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_array_builder_reserve(ref builder : GArrowArrayBuilder, additional_capacity : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_array_builder_reserve(builder : c_ptr(GArrowArrayBuilder), additional_capacity : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_array_builder_append_null(ref builder : GArrowArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_array_builder_append_null(builder : c_ptr(GArrowArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_array_builder_append_nulls(ref builder : GArrowArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_array_builder_append_nulls(builder : c_ptr(GArrowArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_array_builder_append_empty_value(ref builder : GArrowArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_array_builder_append_empty_value(builder : c_ptr(GArrowArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_array_builder_append_empty_values(ref builder : GArrowArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_array_builder_append_empty_values(builder : c_ptr(GArrowArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_null_array_builder_get_type() : GType;
  extern proc GARROW_NULL_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowNullArrayBuilder);
  extern proc GARROW_NULL_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowNullArrayBuilderClass);
  extern proc GARROW_IS_NULL_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_NULL_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_NULL_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowNullArrayBuilderClass);
  extern proc garrow_null_array_builder_new() : c_ptr(GArrowNullArrayBuilder);
  extern proc garrow_null_array_builder_append_null(ref builder : GArrowNullArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_null_array_builder_append_null(builder : c_ptr(GArrowNullArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_null_array_builder_append_nulls(ref builder : GArrowNullArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_null_array_builder_append_nulls(builder : c_ptr(GArrowNullArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_boolean_array_builder_get_type() : GType;
  extern proc GARROW_BOOLEAN_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowBooleanArrayBuilder);
  extern proc GARROW_BOOLEAN_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowBooleanArrayBuilderClass);
  extern proc GARROW_IS_BOOLEAN_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_BOOLEAN_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_BOOLEAN_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowBooleanArrayBuilderClass);
  extern proc garrow_boolean_array_builder_new() : c_ptr(GArrowBooleanArrayBuilder);
  extern proc garrow_boolean_array_builder_append(ref builder : GArrowBooleanArrayBuilder, value : gboolean, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_boolean_array_builder_append(builder : c_ptr(GArrowBooleanArrayBuilder), value : gboolean, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_boolean_array_builder_append_value(ref builder : GArrowBooleanArrayBuilder, value : gboolean, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_boolean_array_builder_append_value(builder : c_ptr(GArrowBooleanArrayBuilder), value : gboolean, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_boolean_array_builder_append_values(ref builder : GArrowBooleanArrayBuilder, ref values : gboolean, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_boolean_array_builder_append_values(builder : c_ptr(GArrowBooleanArrayBuilder), values : c_ptr(gboolean), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_boolean_array_builder_append_null(ref builder : GArrowBooleanArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_boolean_array_builder_append_null(builder : c_ptr(GArrowBooleanArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_boolean_array_builder_append_nulls(ref builder : GArrowBooleanArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_boolean_array_builder_append_nulls(builder : c_ptr(GArrowBooleanArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int_array_builder_get_type() : GType;
  extern proc GARROW_INT_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowIntArrayBuilder);
  extern proc GARROW_INT_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowIntArrayBuilderClass);
  extern proc GARROW_IS_INT_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowIntArrayBuilderClass);
  extern proc garrow_int_array_builder_new() : c_ptr(GArrowIntArrayBuilder);
  extern proc garrow_int_array_builder_append(ref builder : GArrowIntArrayBuilder, value : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int_array_builder_append(builder : c_ptr(GArrowIntArrayBuilder), value : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int_array_builder_append_value(ref builder : GArrowIntArrayBuilder, value : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int_array_builder_append_value(builder : c_ptr(GArrowIntArrayBuilder), value : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int_array_builder_append_values(ref builder : GArrowIntArrayBuilder, ref values : gint64, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int_array_builder_append_values(builder : c_ptr(GArrowIntArrayBuilder), values : c_ptr(gint64), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int_array_builder_append_null(ref builder : GArrowIntArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int_array_builder_append_null(builder : c_ptr(GArrowIntArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int_array_builder_append_nulls(ref builder : GArrowIntArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int_array_builder_append_nulls(builder : c_ptr(GArrowIntArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint_array_builder_get_type() : GType;
  extern proc GARROW_UINT_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowUIntArrayBuilder);
  extern proc GARROW_UINT_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowUIntArrayBuilderClass);
  extern proc GARROW_IS_UINT_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUIntArrayBuilderClass);
  extern proc garrow_uint_array_builder_new() : c_ptr(GArrowUIntArrayBuilder);
  extern proc garrow_uint_array_builder_append(ref builder : GArrowUIntArrayBuilder, value : guint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint_array_builder_append(builder : c_ptr(GArrowUIntArrayBuilder), value : guint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint_array_builder_append_value(ref builder : GArrowUIntArrayBuilder, value : guint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint_array_builder_append_value(builder : c_ptr(GArrowUIntArrayBuilder), value : guint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint_array_builder_append_values(ref builder : GArrowUIntArrayBuilder, ref values : guint64, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint_array_builder_append_values(builder : c_ptr(GArrowUIntArrayBuilder), values : c_ptr(guint64), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint_array_builder_append_null(ref builder : GArrowUIntArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint_array_builder_append_null(builder : c_ptr(GArrowUIntArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint_array_builder_append_nulls(ref builder : GArrowUIntArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint_array_builder_append_nulls(builder : c_ptr(GArrowUIntArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int8_array_builder_get_type() : GType;
  extern proc GARROW_INT8_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowInt8ArrayBuilder);
  extern proc GARROW_INT8_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowInt8ArrayBuilderClass);
  extern proc GARROW_IS_INT8_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT8_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT8_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt8ArrayBuilderClass);
  extern proc garrow_int8_array_builder_new() : c_ptr(GArrowInt8ArrayBuilder);
  extern proc garrow_int8_array_builder_append(ref builder : GArrowInt8ArrayBuilder, value : gint8, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int8_array_builder_append(builder : c_ptr(GArrowInt8ArrayBuilder), value : gint8, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int8_array_builder_append_value(ref builder : GArrowInt8ArrayBuilder, value : gint8, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int8_array_builder_append_value(builder : c_ptr(GArrowInt8ArrayBuilder), value : gint8, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int8_array_builder_append_values(ref builder : GArrowInt8ArrayBuilder, ref values : gint8, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int8_array_builder_append_values(builder : c_ptr(GArrowInt8ArrayBuilder), values : c_ptr(gint8), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int8_array_builder_append_null(ref builder : GArrowInt8ArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int8_array_builder_append_null(builder : c_ptr(GArrowInt8ArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int8_array_builder_append_nulls(ref builder : GArrowInt8ArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int8_array_builder_append_nulls(builder : c_ptr(GArrowInt8ArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint8_array_builder_get_type() : GType;
  extern proc GARROW_UINT8_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowUInt8ArrayBuilder);
  extern proc GARROW_UINT8_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowUInt8ArrayBuilderClass);
  extern proc GARROW_IS_UINT8_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT8_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT8_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt8ArrayBuilderClass);
  extern proc garrow_uint8_array_builder_new() : c_ptr(GArrowUInt8ArrayBuilder);
  extern proc garrow_uint8_array_builder_append(ref builder : GArrowUInt8ArrayBuilder, value : guint8, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint8_array_builder_append(builder : c_ptr(GArrowUInt8ArrayBuilder), value : guint8, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint8_array_builder_append_value(ref builder : GArrowUInt8ArrayBuilder, value : guint8, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint8_array_builder_append_value(builder : c_ptr(GArrowUInt8ArrayBuilder), value : guint8, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint8_array_builder_append_values(ref builder : GArrowUInt8ArrayBuilder, ref values : guint8, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint8_array_builder_append_values(builder : c_ptr(GArrowUInt8ArrayBuilder), values : c_ptr(guint8), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint8_array_builder_append_null(ref builder : GArrowUInt8ArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint8_array_builder_append_null(builder : c_ptr(GArrowUInt8ArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint8_array_builder_append_nulls(ref builder : GArrowUInt8ArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint8_array_builder_append_nulls(builder : c_ptr(GArrowUInt8ArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int16_array_builder_get_type() : GType;
  extern proc GARROW_INT16_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowInt16ArrayBuilder);
  extern proc GARROW_INT16_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowInt16ArrayBuilderClass);
  extern proc GARROW_IS_INT16_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT16_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT16_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt16ArrayBuilderClass);
  extern proc garrow_int16_array_builder_new() : c_ptr(GArrowInt16ArrayBuilder);
  extern proc garrow_int16_array_builder_append(ref builder : GArrowInt16ArrayBuilder, value : gint16, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int16_array_builder_append(builder : c_ptr(GArrowInt16ArrayBuilder), value : gint16, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int16_array_builder_append_value(ref builder : GArrowInt16ArrayBuilder, value : gint16, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int16_array_builder_append_value(builder : c_ptr(GArrowInt16ArrayBuilder), value : gint16, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int16_array_builder_append_values(ref builder : GArrowInt16ArrayBuilder, ref values : gint16, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int16_array_builder_append_values(builder : c_ptr(GArrowInt16ArrayBuilder), values : c_ptr(gint16), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int16_array_builder_append_null(ref builder : GArrowInt16ArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int16_array_builder_append_null(builder : c_ptr(GArrowInt16ArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int16_array_builder_append_nulls(ref builder : GArrowInt16ArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int16_array_builder_append_nulls(builder : c_ptr(GArrowInt16ArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint16_array_builder_get_type() : GType;
  extern proc GARROW_UINT16_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowUInt16ArrayBuilder);
  extern proc GARROW_UINT16_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowUInt16ArrayBuilderClass);
  extern proc GARROW_IS_UINT16_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT16_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT16_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt16ArrayBuilderClass);
  extern proc garrow_uint16_array_builder_new() : c_ptr(GArrowUInt16ArrayBuilder);
  extern proc garrow_uint16_array_builder_append(ref builder : GArrowUInt16ArrayBuilder, value : guint16, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint16_array_builder_append(builder : c_ptr(GArrowUInt16ArrayBuilder), value : guint16, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint16_array_builder_append_value(ref builder : GArrowUInt16ArrayBuilder, value : guint16, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint16_array_builder_append_value(builder : c_ptr(GArrowUInt16ArrayBuilder), value : guint16, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint16_array_builder_append_values(ref builder : GArrowUInt16ArrayBuilder, ref values : guint16, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint16_array_builder_append_values(builder : c_ptr(GArrowUInt16ArrayBuilder), values : c_ptr(guint16), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint16_array_builder_append_null(ref builder : GArrowUInt16ArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint16_array_builder_append_null(builder : c_ptr(GArrowUInt16ArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint16_array_builder_append_nulls(ref builder : GArrowUInt16ArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint16_array_builder_append_nulls(builder : c_ptr(GArrowUInt16ArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int32_array_builder_get_type() : GType;
  extern proc GARROW_INT32_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowInt32ArrayBuilder);
  extern proc GARROW_INT32_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowInt32ArrayBuilderClass);
  extern proc GARROW_IS_INT32_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT32_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT32_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt32ArrayBuilderClass);
  extern proc garrow_int32_array_builder_new() : c_ptr(GArrowInt32ArrayBuilder);
  extern proc garrow_int32_array_builder_append(ref builder : GArrowInt32ArrayBuilder, value : gint32, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int32_array_builder_append(builder : c_ptr(GArrowInt32ArrayBuilder), value : gint32, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int32_array_builder_append_value(ref builder : GArrowInt32ArrayBuilder, value : gint32, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int32_array_builder_append_value(builder : c_ptr(GArrowInt32ArrayBuilder), value : gint32, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int32_array_builder_append_values(ref builder : GArrowInt32ArrayBuilder, ref values : gint32, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int32_array_builder_append_values(builder : c_ptr(GArrowInt32ArrayBuilder), values : c_ptr(gint32), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int32_array_builder_append_null(ref builder : GArrowInt32ArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int32_array_builder_append_null(builder : c_ptr(GArrowInt32ArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int32_array_builder_append_nulls(ref builder : GArrowInt32ArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int32_array_builder_append_nulls(builder : c_ptr(GArrowInt32ArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint32_array_builder_get_type() : GType;
  extern proc GARROW_UINT32_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowUInt32ArrayBuilder);
  extern proc GARROW_UINT32_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowUInt32ArrayBuilderClass);
  extern proc GARROW_IS_UINT32_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT32_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT32_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt32ArrayBuilderClass);
  extern proc garrow_uint32_array_builder_new() : c_ptr(GArrowUInt32ArrayBuilder);
  extern proc garrow_uint32_array_builder_append(ref builder : GArrowUInt32ArrayBuilder, value : guint32, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint32_array_builder_append(builder : c_ptr(GArrowUInt32ArrayBuilder), value : guint32, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint32_array_builder_append_value(ref builder : GArrowUInt32ArrayBuilder, value : guint32, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint32_array_builder_append_value(builder : c_ptr(GArrowUInt32ArrayBuilder), value : guint32, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint32_array_builder_append_values(ref builder : GArrowUInt32ArrayBuilder, ref values : guint32, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint32_array_builder_append_values(builder : c_ptr(GArrowUInt32ArrayBuilder), values : c_ptr(guint32), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint32_array_builder_append_null(ref builder : GArrowUInt32ArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint32_array_builder_append_null(builder : c_ptr(GArrowUInt32ArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint32_array_builder_append_nulls(ref builder : GArrowUInt32ArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint32_array_builder_append_nulls(builder : c_ptr(GArrowUInt32ArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int64_array_builder_get_type() : GType;
  extern proc GARROW_INT64_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowInt64ArrayBuilder);
  extern proc GARROW_INT64_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowInt64ArrayBuilderClass);
  extern proc GARROW_IS_INT64_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT64_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT64_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt64ArrayBuilderClass);
  extern proc garrow_int64_array_builder_new() : c_ptr(GArrowInt64ArrayBuilder);
  extern proc garrow_int64_array_builder_append(ref builder : GArrowInt64ArrayBuilder, value : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int64_array_builder_append(builder : c_ptr(GArrowInt64ArrayBuilder), value : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int64_array_builder_append_value(ref builder : GArrowInt64ArrayBuilder, value : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int64_array_builder_append_value(builder : c_ptr(GArrowInt64ArrayBuilder), value : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int64_array_builder_append_values(ref builder : GArrowInt64ArrayBuilder, ref values : gint64, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int64_array_builder_append_values(builder : c_ptr(GArrowInt64ArrayBuilder), values : c_ptr(gint64), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int64_array_builder_append_null(ref builder : GArrowInt64ArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int64_array_builder_append_null(builder : c_ptr(GArrowInt64ArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_int64_array_builder_append_nulls(ref builder : GArrowInt64ArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_int64_array_builder_append_nulls(builder : c_ptr(GArrowInt64ArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint64_array_builder_get_type() : GType;
  extern proc GARROW_UINT64_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowUInt64ArrayBuilder);
  extern proc GARROW_UINT64_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowUInt64ArrayBuilderClass);
  extern proc GARROW_IS_UINT64_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT64_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT64_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt64ArrayBuilderClass);
  extern proc garrow_uint64_array_builder_new() : c_ptr(GArrowUInt64ArrayBuilder);
  extern proc garrow_uint64_array_builder_append(ref builder : GArrowUInt64ArrayBuilder, value : guint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint64_array_builder_append(builder : c_ptr(GArrowUInt64ArrayBuilder), value : guint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint64_array_builder_append_value(ref builder : GArrowUInt64ArrayBuilder, value : guint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint64_array_builder_append_value(builder : c_ptr(GArrowUInt64ArrayBuilder), value : guint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint64_array_builder_append_values(ref builder : GArrowUInt64ArrayBuilder, ref values : guint64, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint64_array_builder_append_values(builder : c_ptr(GArrowUInt64ArrayBuilder), values : c_ptr(guint64), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint64_array_builder_append_null(ref builder : GArrowUInt64ArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint64_array_builder_append_null(builder : c_ptr(GArrowUInt64ArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_uint64_array_builder_append_nulls(ref builder : GArrowUInt64ArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_uint64_array_builder_append_nulls(builder : c_ptr(GArrowUInt64ArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_float_array_builder_get_type() : GType;
  extern proc GARROW_FLOAT_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowFloatArrayBuilder);
  extern proc GARROW_FLOAT_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowFloatArrayBuilderClass);
  extern proc GARROW_IS_FLOAT_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FLOAT_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FLOAT_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFloatArrayBuilderClass);
  extern proc garrow_float_array_builder_new() : c_ptr(GArrowFloatArrayBuilder);
  extern proc garrow_float_array_builder_append(ref builder : GArrowFloatArrayBuilder, value : gfloat, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_float_array_builder_append(builder : c_ptr(GArrowFloatArrayBuilder), value : gfloat, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_float_array_builder_append_value(ref builder : GArrowFloatArrayBuilder, value : gfloat, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_float_array_builder_append_value(builder : c_ptr(GArrowFloatArrayBuilder), value : gfloat, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_float_array_builder_append_values(ref builder : GArrowFloatArrayBuilder, ref values : gfloat, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_float_array_builder_append_values(builder : c_ptr(GArrowFloatArrayBuilder), values : c_ptr(gfloat), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_float_array_builder_append_null(ref builder : GArrowFloatArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_float_array_builder_append_null(builder : c_ptr(GArrowFloatArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_float_array_builder_append_nulls(ref builder : GArrowFloatArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_float_array_builder_append_nulls(builder : c_ptr(GArrowFloatArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_double_array_builder_get_type() : GType;
  extern proc GARROW_DOUBLE_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowDoubleArrayBuilder);
  extern proc GARROW_DOUBLE_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowDoubleArrayBuilderClass);
  extern proc GARROW_IS_DOUBLE_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DOUBLE_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DOUBLE_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDoubleArrayBuilderClass);
  extern proc garrow_double_array_builder_new() : c_ptr(GArrowDoubleArrayBuilder);
  extern proc garrow_double_array_builder_append(ref builder : GArrowDoubleArrayBuilder, value : gdouble, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_double_array_builder_append(builder : c_ptr(GArrowDoubleArrayBuilder), value : gdouble, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_double_array_builder_append_value(ref builder : GArrowDoubleArrayBuilder, value : gdouble, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_double_array_builder_append_value(builder : c_ptr(GArrowDoubleArrayBuilder), value : gdouble, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_double_array_builder_append_values(ref builder : GArrowDoubleArrayBuilder, ref values : gdouble, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_double_array_builder_append_values(builder : c_ptr(GArrowDoubleArrayBuilder), values : c_ptr(gdouble), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_double_array_builder_append_null(ref builder : GArrowDoubleArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_double_array_builder_append_null(builder : c_ptr(GArrowDoubleArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_double_array_builder_append_nulls(ref builder : GArrowDoubleArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_double_array_builder_append_nulls(builder : c_ptr(GArrowDoubleArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_binary_array_builder_get_type() : GType;
  extern proc GARROW_BINARY_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowBinaryArrayBuilder);
  extern proc GARROW_BINARY_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowBinaryArrayBuilderClass);
  extern proc GARROW_IS_BINARY_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_BINARY_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_BINARY_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowBinaryArrayBuilderClass);
  extern proc garrow_binary_array_builder_new() : c_ptr(GArrowBinaryArrayBuilder);
  extern proc garrow_binary_array_builder_append(ref builder : GArrowBinaryArrayBuilder, ref value : guint8, length : gint32, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_binary_array_builder_append(builder : c_ptr(GArrowBinaryArrayBuilder), value : c_ptr(guint8), length : gint32, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_binary_array_builder_append_value(ref builder : GArrowBinaryArrayBuilder, ref value : guint8, length : gint32, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_binary_array_builder_append_value(builder : c_ptr(GArrowBinaryArrayBuilder), value : c_ptr(guint8), length : gint32, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_binary_array_builder_append_value_bytes(ref builder : GArrowBinaryArrayBuilder, ref value : GBytes, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_binary_array_builder_append_value_bytes(builder : c_ptr(GArrowBinaryArrayBuilder), value : c_ptr(GBytes), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_binary_array_builder_append_values(ref builder : GArrowBinaryArrayBuilder, ref values : c_ptr(GBytes), values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_binary_array_builder_append_values(builder : c_ptr(GArrowBinaryArrayBuilder), values : c_ptr(c_ptr(GBytes)), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_binary_array_builder_append_null(ref builder : GArrowBinaryArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_binary_array_builder_append_null(builder : c_ptr(GArrowBinaryArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_binary_array_builder_append_nulls(ref builder : GArrowBinaryArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_binary_array_builder_append_nulls(builder : c_ptr(GArrowBinaryArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_large_binary_array_builder_get_type() : GType;
  extern proc GARROW_LARGE_BINARY_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowLargeBinaryArrayBuilder);
  extern proc GARROW_LARGE_BINARY_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowLargeBinaryArrayBuilderClass);
  extern proc GARROW_IS_LARGE_BINARY_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LARGE_BINARY_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LARGE_BINARY_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowLargeBinaryArrayBuilderClass);
  extern proc garrow_large_binary_array_builder_new() : c_ptr(GArrowLargeBinaryArrayBuilder);
  extern proc garrow_large_binary_array_builder_append_value(ref builder : GArrowLargeBinaryArrayBuilder, ref value : guint8, length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_large_binary_array_builder_append_value(builder : c_ptr(GArrowLargeBinaryArrayBuilder), value : c_ptr(guint8), length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_large_binary_array_builder_append_value_bytes(ref builder : GArrowLargeBinaryArrayBuilder, ref value : GBytes, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_large_binary_array_builder_append_value_bytes(builder : c_ptr(GArrowLargeBinaryArrayBuilder), value : c_ptr(GBytes), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_large_binary_array_builder_append_values(ref builder : GArrowLargeBinaryArrayBuilder, ref values : c_ptr(GBytes), values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_large_binary_array_builder_append_values(builder : c_ptr(GArrowLargeBinaryArrayBuilder), values : c_ptr(c_ptr(GBytes)), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_large_binary_array_builder_append_null(ref builder : GArrowLargeBinaryArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_large_binary_array_builder_append_null(builder : c_ptr(GArrowLargeBinaryArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_large_binary_array_builder_append_nulls(ref builder : GArrowLargeBinaryArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_large_binary_array_builder_append_nulls(builder : c_ptr(GArrowLargeBinaryArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_string_array_builder_get_type() : GType;
  extern proc GARROW_STRING_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowStringArrayBuilder);
  extern proc GARROW_STRING_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowStringArrayBuilderClass);
  extern proc GARROW_IS_STRING_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_STRING_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_STRING_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowStringArrayBuilderClass);
  extern proc garrow_string_array_builder_new() : c_ptr(GArrowStringArrayBuilder);
  extern proc garrow_string_array_builder_append(ref builder : GArrowStringArrayBuilder, ref value : gchar, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_string_array_builder_append(builder : c_ptr(GArrowStringArrayBuilder), value : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_string_array_builder_append_value(ref builder : GArrowStringArrayBuilder, ref value : gchar, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_string_array_builder_append_value(builder : c_ptr(GArrowStringArrayBuilder), value : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_string_array_builder_append_string(ref builder : GArrowStringArrayBuilder, ref value : gchar, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_string_array_builder_append_string(builder : c_ptr(GArrowStringArrayBuilder), value : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_string_array_builder_append_values(ref builder : GArrowStringArrayBuilder, ref values : c_ptr(gchar), values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_string_array_builder_append_values(builder : c_ptr(GArrowStringArrayBuilder), values : c_ptr(c_ptr(gchar)), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_string_array_builder_append_strings(ref builder : GArrowStringArrayBuilder, ref values : c_ptr(gchar), values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_string_array_builder_append_strings(builder : c_ptr(GArrowStringArrayBuilder), values : c_ptr(c_ptr(gchar)), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_large_string_array_builder_get_type() : GType;
  extern proc GARROW_LARGE_STRING_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowLargeStringArrayBuilder);
  extern proc GARROW_LARGE_STRING_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowLargeStringArrayBuilderClass);
  extern proc GARROW_IS_LARGE_STRING_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LARGE_STRING_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LARGE_STRING_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowLargeStringArrayBuilderClass);
  extern proc garrow_large_string_array_builder_new() : c_ptr(GArrowLargeStringArrayBuilder);
  extern proc garrow_large_string_array_builder_append_string(ref builder : GArrowLargeStringArrayBuilder, ref value : gchar, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_large_string_array_builder_append_string(builder : c_ptr(GArrowLargeStringArrayBuilder), value : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_large_string_array_builder_append_strings(ref builder : GArrowLargeStringArrayBuilder, ref values : c_ptr(gchar), values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_large_string_array_builder_append_strings(builder : c_ptr(GArrowLargeStringArrayBuilder), values : c_ptr(c_ptr(gchar)), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_fixed_size_binary_array_builder_get_type() : GType;
  extern proc GARROW_FIXED_SIZE_BINARY_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowFixedSizeBinaryArrayBuilder);
  extern proc GARROW_FIXED_SIZE_BINARY_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowFixedSizeBinaryArrayBuilderClass);
  extern proc GARROW_IS_FIXED_SIZE_BINARY_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FIXED_SIZE_BINARY_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FIXED_SIZE_BINARY_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFixedSizeBinaryArrayBuilderClass);
  extern proc garrow_fixed_size_binary_array_builder_new(ref data_type : GArrowFixedSizeBinaryDataType) : c_ptr(GArrowFixedSizeBinaryArrayBuilder);
  extern proc garrow_fixed_size_binary_array_builder_new(data_type : c_ptr(GArrowFixedSizeBinaryDataType)) : c_ptr(GArrowFixedSizeBinaryArrayBuilder);
  extern proc garrow_fixed_size_binary_array_builder_append_value(ref builder : GArrowFixedSizeBinaryArrayBuilder, ref value : guint8, length : gint32, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_fixed_size_binary_array_builder_append_value(builder : c_ptr(GArrowFixedSizeBinaryArrayBuilder), value : c_ptr(guint8), length : gint32, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_fixed_size_binary_array_builder_append_value_bytes(ref builder : GArrowFixedSizeBinaryArrayBuilder, ref value : GBytes, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_fixed_size_binary_array_builder_append_value_bytes(builder : c_ptr(GArrowFixedSizeBinaryArrayBuilder), value : c_ptr(GBytes), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_fixed_size_binary_array_builder_append_values(ref builder : GArrowFixedSizeBinaryArrayBuilder, ref values : c_ptr(GBytes), values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_fixed_size_binary_array_builder_append_values(builder : c_ptr(GArrowFixedSizeBinaryArrayBuilder), values : c_ptr(c_ptr(GBytes)), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_fixed_size_binary_array_builder_append_values_packed(ref builder : GArrowFixedSizeBinaryArrayBuilder, ref values : GBytes, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_fixed_size_binary_array_builder_append_values_packed(builder : c_ptr(GArrowFixedSizeBinaryArrayBuilder), values : c_ptr(GBytes), is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_date32_array_builder_get_type() : GType;
  extern proc GARROW_DATE32_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowDate32ArrayBuilder);
  extern proc GARROW_DATE32_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowDate32ArrayBuilderClass);
  extern proc GARROW_IS_DATE32_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DATE32_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DATE32_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDate32ArrayBuilderClass);
  extern proc garrow_date32_array_builder_new() : c_ptr(GArrowDate32ArrayBuilder);
  extern proc garrow_date32_array_builder_append(ref builder : GArrowDate32ArrayBuilder, value : gint32, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_date32_array_builder_append(builder : c_ptr(GArrowDate32ArrayBuilder), value : gint32, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_date32_array_builder_append_value(ref builder : GArrowDate32ArrayBuilder, value : gint32, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_date32_array_builder_append_value(builder : c_ptr(GArrowDate32ArrayBuilder), value : gint32, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_date32_array_builder_append_values(ref builder : GArrowDate32ArrayBuilder, ref values : gint32, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_date32_array_builder_append_values(builder : c_ptr(GArrowDate32ArrayBuilder), values : c_ptr(gint32), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_date32_array_builder_append_null(ref builder : GArrowDate32ArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_date32_array_builder_append_null(builder : c_ptr(GArrowDate32ArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_date32_array_builder_append_nulls(ref builder : GArrowDate32ArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_date32_array_builder_append_nulls(builder : c_ptr(GArrowDate32ArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_date64_array_builder_get_type() : GType;
  extern proc GARROW_DATE64_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowDate64ArrayBuilder);
  extern proc GARROW_DATE64_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowDate64ArrayBuilderClass);
  extern proc GARROW_IS_DATE64_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DATE64_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DATE64_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDate64ArrayBuilderClass);
  extern proc garrow_date64_array_builder_new() : c_ptr(GArrowDate64ArrayBuilder);
  extern proc garrow_date64_array_builder_append(ref builder : GArrowDate64ArrayBuilder, value : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_date64_array_builder_append(builder : c_ptr(GArrowDate64ArrayBuilder), value : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_date64_array_builder_append_value(ref builder : GArrowDate64ArrayBuilder, value : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_date64_array_builder_append_value(builder : c_ptr(GArrowDate64ArrayBuilder), value : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_date64_array_builder_append_values(ref builder : GArrowDate64ArrayBuilder, ref values : gint64, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_date64_array_builder_append_values(builder : c_ptr(GArrowDate64ArrayBuilder), values : c_ptr(gint64), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_date64_array_builder_append_null(ref builder : GArrowDate64ArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_date64_array_builder_append_null(builder : c_ptr(GArrowDate64ArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_date64_array_builder_append_nulls(ref builder : GArrowDate64ArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_date64_array_builder_append_nulls(builder : c_ptr(GArrowDate64ArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_timestamp_array_builder_get_type() : GType;
  extern proc GARROW_TIMESTAMP_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowTimestampArrayBuilder);
  extern proc GARROW_TIMESTAMP_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowTimestampArrayBuilderClass);
  extern proc GARROW_IS_TIMESTAMP_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TIMESTAMP_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TIMESTAMP_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTimestampArrayBuilderClass);
  extern proc garrow_timestamp_array_builder_new(ref data_type : GArrowTimestampDataType) : c_ptr(GArrowTimestampArrayBuilder);
  extern proc garrow_timestamp_array_builder_new(data_type : c_ptr(GArrowTimestampDataType)) : c_ptr(GArrowTimestampArrayBuilder);
  extern proc garrow_timestamp_array_builder_append(ref builder : GArrowTimestampArrayBuilder, value : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_timestamp_array_builder_append(builder : c_ptr(GArrowTimestampArrayBuilder), value : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_timestamp_array_builder_append_value(ref builder : GArrowTimestampArrayBuilder, value : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_timestamp_array_builder_append_value(builder : c_ptr(GArrowTimestampArrayBuilder), value : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_timestamp_array_builder_append_values(ref builder : GArrowTimestampArrayBuilder, ref values : gint64, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_timestamp_array_builder_append_values(builder : c_ptr(GArrowTimestampArrayBuilder), values : c_ptr(gint64), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_timestamp_array_builder_append_null(ref builder : GArrowTimestampArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_timestamp_array_builder_append_null(builder : c_ptr(GArrowTimestampArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_timestamp_array_builder_append_nulls(ref builder : GArrowTimestampArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_timestamp_array_builder_append_nulls(builder : c_ptr(GArrowTimestampArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_time32_array_builder_get_type() : GType;
  extern proc GARROW_TIME32_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowTime32ArrayBuilder);
  extern proc GARROW_TIME32_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowTime32ArrayBuilderClass);
  extern proc GARROW_IS_TIME32_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TIME32_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TIME32_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTime32ArrayBuilderClass);
  extern proc garrow_time32_array_builder_new(ref data_type : GArrowTime32DataType) : c_ptr(GArrowTime32ArrayBuilder);
  extern proc garrow_time32_array_builder_new(data_type : c_ptr(GArrowTime32DataType)) : c_ptr(GArrowTime32ArrayBuilder);
  extern proc garrow_time32_array_builder_append(ref builder : GArrowTime32ArrayBuilder, value : gint32, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_time32_array_builder_append(builder : c_ptr(GArrowTime32ArrayBuilder), value : gint32, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_time32_array_builder_append_value(ref builder : GArrowTime32ArrayBuilder, value : gint32, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_time32_array_builder_append_value(builder : c_ptr(GArrowTime32ArrayBuilder), value : gint32, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_time32_array_builder_append_values(ref builder : GArrowTime32ArrayBuilder, ref values : gint32, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_time32_array_builder_append_values(builder : c_ptr(GArrowTime32ArrayBuilder), values : c_ptr(gint32), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_time32_array_builder_append_null(ref builder : GArrowTime32ArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_time32_array_builder_append_null(builder : c_ptr(GArrowTime32ArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_time32_array_builder_append_nulls(ref builder : GArrowTime32ArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_time32_array_builder_append_nulls(builder : c_ptr(GArrowTime32ArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_time64_array_builder_get_type() : GType;
  extern proc GARROW_TIME64_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowTime64ArrayBuilder);
  extern proc GARROW_TIME64_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowTime64ArrayBuilderClass);
  extern proc GARROW_IS_TIME64_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TIME64_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TIME64_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTime64ArrayBuilderClass);
  extern proc garrow_time64_array_builder_new(ref data_type : GArrowTime64DataType) : c_ptr(GArrowTime64ArrayBuilder);
  extern proc garrow_time64_array_builder_new(data_type : c_ptr(GArrowTime64DataType)) : c_ptr(GArrowTime64ArrayBuilder);
  extern proc garrow_time64_array_builder_append(ref builder : GArrowTime64ArrayBuilder, value : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_time64_array_builder_append(builder : c_ptr(GArrowTime64ArrayBuilder), value : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_time64_array_builder_append_value(ref builder : GArrowTime64ArrayBuilder, value : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_time64_array_builder_append_value(builder : c_ptr(GArrowTime64ArrayBuilder), value : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_time64_array_builder_append_values(ref builder : GArrowTime64ArrayBuilder, ref values : gint64, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_time64_array_builder_append_values(builder : c_ptr(GArrowTime64ArrayBuilder), values : c_ptr(gint64), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_time64_array_builder_append_null(ref builder : GArrowTime64ArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_time64_array_builder_append_null(builder : c_ptr(GArrowTime64ArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_time64_array_builder_append_nulls(ref builder : GArrowTime64ArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_time64_array_builder_append_nulls(builder : c_ptr(GArrowTime64ArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_binary_dictionary_array_builder_get_type() : GType;
  extern proc GARROW_BINARY_DICTIONARY_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowBinaryDictionaryArrayBuilder);
  extern proc GARROW_BINARY_DICTIONARY_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowBinaryDictionaryArrayBuilderClass);
  extern proc GARROW_IS_BINARY_DICTIONARY_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_BINARY_DICTIONARY_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_BINARY_DICTIONARY_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowBinaryDictionaryArrayBuilderClass);
  extern proc garrow_binary_dictionary_array_builder_new() : c_ptr(GArrowBinaryDictionaryArrayBuilder);
  extern proc garrow_binary_dictionary_array_builder_append_null(ref builder : GArrowBinaryDictionaryArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_binary_dictionary_array_builder_append_null(builder : c_ptr(GArrowBinaryDictionaryArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_binary_dictionary_array_builder_append_value(ref builder : GArrowBinaryDictionaryArrayBuilder, ref value : guint8, length : gint32, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_binary_dictionary_array_builder_append_value(builder : c_ptr(GArrowBinaryDictionaryArrayBuilder), value : c_ptr(guint8), length : gint32, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_binary_dictionary_array_builder_append_value_bytes(ref builder : GArrowBinaryDictionaryArrayBuilder, ref value : GBytes, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_binary_dictionary_array_builder_append_value_bytes(builder : c_ptr(GArrowBinaryDictionaryArrayBuilder), value : c_ptr(GBytes), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_binary_dictionary_array_builder_append_array(ref builder : GArrowBinaryDictionaryArrayBuilder, ref array : GArrowBinaryArray, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_binary_dictionary_array_builder_append_array(builder : c_ptr(GArrowBinaryDictionaryArrayBuilder), array : c_ptr(GArrowBinaryArray), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_binary_dictionary_array_builder_append_indices(ref builder : GArrowBinaryDictionaryArrayBuilder, ref values : gint64, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_binary_dictionary_array_builder_append_indices(builder : c_ptr(GArrowBinaryDictionaryArrayBuilder), values : c_ptr(gint64), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_binary_dictionary_array_builder_get_dictionary_length(ref builder : GArrowBinaryDictionaryArrayBuilder) : gint64;
  extern proc garrow_binary_dictionary_array_builder_get_dictionary_length(builder : c_ptr(GArrowBinaryDictionaryArrayBuilder)) : gint64;
  extern proc garrow_binary_dictionary_array_builder_finish_delta(ref builder : GArrowBinaryDictionaryArrayBuilder, ref out_indices : c_ptr(GArrowArray), ref out_delta : c_ptr(GArrowArray), ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_binary_dictionary_array_builder_finish_delta(builder : c_ptr(GArrowBinaryDictionaryArrayBuilder), out_indices : c_ptr(c_ptr(GArrowArray)), out_delta : c_ptr(c_ptr(GArrowArray)), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_binary_dictionary_array_builder_insert_memo_values(ref builder : GArrowBinaryDictionaryArrayBuilder, ref values : GArrowBinaryArray, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_binary_dictionary_array_builder_insert_memo_values(builder : c_ptr(GArrowBinaryDictionaryArrayBuilder), values : c_ptr(GArrowBinaryArray), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_binary_dictionary_array_builder_reset_full(ref builder : GArrowBinaryDictionaryArrayBuilder) : void;
  extern proc garrow_binary_dictionary_array_builder_reset_full(builder : c_ptr(GArrowBinaryDictionaryArrayBuilder)) : void;
  extern proc garrow_string_dictionary_array_builder_get_type() : GType;
  extern proc GARROW_STRING_DICTIONARY_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowStringDictionaryArrayBuilder);
  extern proc GARROW_STRING_DICTIONARY_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowStringDictionaryArrayBuilderClass);
  extern proc GARROW_IS_STRING_DICTIONARY_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_STRING_DICTIONARY_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_STRING_DICTIONARY_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowStringDictionaryArrayBuilderClass);
  extern proc garrow_string_dictionary_array_builder_new() : c_ptr(GArrowStringDictionaryArrayBuilder);
  extern proc garrow_string_dictionary_array_builder_append_null(ref builder : GArrowStringDictionaryArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_string_dictionary_array_builder_append_null(builder : c_ptr(GArrowStringDictionaryArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_string_dictionary_array_builder_append_string(ref builder : GArrowStringDictionaryArrayBuilder, ref value : gchar, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_string_dictionary_array_builder_append_string(builder : c_ptr(GArrowStringDictionaryArrayBuilder), value : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_string_dictionary_array_builder_append_array(ref builder : GArrowStringDictionaryArrayBuilder, ref array : GArrowStringArray, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_string_dictionary_array_builder_append_array(builder : c_ptr(GArrowStringDictionaryArrayBuilder), array : c_ptr(GArrowStringArray), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_string_dictionary_array_builder_append_indices(ref builder : GArrowStringDictionaryArrayBuilder, ref values : gint64, values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_string_dictionary_array_builder_append_indices(builder : c_ptr(GArrowStringDictionaryArrayBuilder), values : c_ptr(gint64), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_string_dictionary_array_builder_get_dictionary_length(ref builder : GArrowStringDictionaryArrayBuilder) : gint64;
  extern proc garrow_string_dictionary_array_builder_get_dictionary_length(builder : c_ptr(GArrowStringDictionaryArrayBuilder)) : gint64;
  extern proc garrow_string_dictionary_array_builder_finish_delta(ref builder : GArrowStringDictionaryArrayBuilder, ref out_indices : c_ptr(GArrowArray), ref out_delta : c_ptr(GArrowArray), ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_string_dictionary_array_builder_finish_delta(builder : c_ptr(GArrowStringDictionaryArrayBuilder), out_indices : c_ptr(c_ptr(GArrowArray)), out_delta : c_ptr(c_ptr(GArrowArray)), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_string_dictionary_array_builder_insert_memo_values(ref builder : GArrowStringDictionaryArrayBuilder, ref values : GArrowStringArray, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_string_dictionary_array_builder_insert_memo_values(builder : c_ptr(GArrowStringDictionaryArrayBuilder), values : c_ptr(GArrowStringArray), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_string_dictionary_array_builder_reset_full(ref builder : GArrowStringDictionaryArrayBuilder) : void;
  extern proc garrow_string_dictionary_array_builder_reset_full(builder : c_ptr(GArrowStringDictionaryArrayBuilder)) : void;
  extern proc garrow_list_array_builder_get_type() : GType;
  extern proc GARROW_LIST_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowListArrayBuilder);
  extern proc GARROW_LIST_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowListArrayBuilderClass);
  extern proc GARROW_IS_LIST_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LIST_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LIST_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowListArrayBuilderClass);
  extern proc garrow_list_array_builder_new(ref data_type : GArrowListDataType, ref error : c_ptr(GError)) : c_ptr(GArrowListArrayBuilder);
  extern proc garrow_list_array_builder_new(data_type : c_ptr(GArrowListDataType), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowListArrayBuilder);
  extern proc garrow_list_array_builder_append(ref builder : GArrowListArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_list_array_builder_append(builder : c_ptr(GArrowListArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_list_array_builder_append_value(ref builder : GArrowListArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_list_array_builder_append_value(builder : c_ptr(GArrowListArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_list_array_builder_append_null(ref builder : GArrowListArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_list_array_builder_append_null(builder : c_ptr(GArrowListArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_list_array_builder_get_value_builder(ref builder : GArrowListArrayBuilder) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_list_array_builder_get_value_builder(builder : c_ptr(GArrowListArrayBuilder)) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_large_list_array_builder_get_type() : GType;
  extern proc GARROW_LARGE_LIST_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowLargeListArrayBuilder);
  extern proc GARROW_LARGE_LIST_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowLargeListArrayBuilderClass);
  extern proc GARROW_IS_LARGE_LIST_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LARGE_LIST_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LARGE_LIST_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowLargeListArrayBuilderClass);
  extern proc garrow_large_list_array_builder_new(ref data_type : GArrowLargeListDataType, ref error : c_ptr(GError)) : c_ptr(GArrowLargeListArrayBuilder);
  extern proc garrow_large_list_array_builder_new(data_type : c_ptr(GArrowLargeListDataType), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowLargeListArrayBuilder);
  extern proc garrow_large_list_array_builder_append_value(ref builder : GArrowLargeListArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_large_list_array_builder_append_value(builder : c_ptr(GArrowLargeListArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_large_list_array_builder_append_null(ref builder : GArrowLargeListArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_large_list_array_builder_append_null(builder : c_ptr(GArrowLargeListArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_large_list_array_builder_get_value_builder(ref builder : GArrowLargeListArrayBuilder) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_large_list_array_builder_get_value_builder(builder : c_ptr(GArrowLargeListArrayBuilder)) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_struct_array_builder_get_type() : GType;
  extern proc GARROW_STRUCT_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowStructArrayBuilder);
  extern proc GARROW_STRUCT_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowStructArrayBuilderClass);
  extern proc GARROW_IS_STRUCT_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_STRUCT_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_STRUCT_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowStructArrayBuilderClass);
  extern proc garrow_struct_array_builder_new(ref data_type : GArrowStructDataType, ref error : c_ptr(GError)) : c_ptr(GArrowStructArrayBuilder);
  extern proc garrow_struct_array_builder_new(data_type : c_ptr(GArrowStructDataType), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowStructArrayBuilder);
  extern proc garrow_struct_array_builder_append(ref builder : GArrowStructArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_struct_array_builder_append(builder : c_ptr(GArrowStructArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_struct_array_builder_append_value(ref builder : GArrowStructArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_struct_array_builder_append_value(builder : c_ptr(GArrowStructArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_struct_array_builder_append_null(ref builder : GArrowStructArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_struct_array_builder_append_null(builder : c_ptr(GArrowStructArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_struct_array_builder_get_field_builder(ref builder : GArrowStructArrayBuilder, i : gint) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_struct_array_builder_get_field_builder(builder : c_ptr(GArrowStructArrayBuilder), i : gint) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_struct_array_builder_get_field_builders(ref builder : GArrowStructArrayBuilder) : c_ptr(GList);
  extern proc garrow_struct_array_builder_get_field_builders(builder : c_ptr(GArrowStructArrayBuilder)) : c_ptr(GList);
  extern proc garrow_map_array_builder_get_type() : GType;
  extern proc GARROW_MAP_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowMapArrayBuilder);
  extern proc GARROW_MAP_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowMapArrayBuilderClass);
  extern proc GARROW_IS_MAP_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_MAP_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_MAP_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowMapArrayBuilderClass);
  extern proc garrow_map_array_builder_new(ref data_type : GArrowMapDataType, ref error : c_ptr(GError)) : c_ptr(GArrowMapArrayBuilder);
  extern proc garrow_map_array_builder_new(data_type : c_ptr(GArrowMapDataType), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowMapArrayBuilder);
  extern proc garrow_map_array_builder_append_value(ref builder : GArrowMapArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_map_array_builder_append_value(builder : c_ptr(GArrowMapArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_map_array_builder_append_values(ref builder : GArrowMapArrayBuilder, ref offsets : gint32, offsets_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_map_array_builder_append_values(builder : c_ptr(GArrowMapArrayBuilder), offsets : c_ptr(gint32), offsets_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_map_array_builder_append_null(ref builder : GArrowMapArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_map_array_builder_append_null(builder : c_ptr(GArrowMapArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_map_array_builder_append_nulls(ref builder : GArrowMapArrayBuilder, n : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_map_array_builder_append_nulls(builder : c_ptr(GArrowMapArrayBuilder), n : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_map_array_builder_get_key_builder(ref builder : GArrowMapArrayBuilder) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_map_array_builder_get_key_builder(builder : c_ptr(GArrowMapArrayBuilder)) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_map_array_builder_get_item_builder(ref builder : GArrowMapArrayBuilder) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_map_array_builder_get_item_builder(builder : c_ptr(GArrowMapArrayBuilder)) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_map_array_builder_get_value_builder(ref builder : GArrowMapArrayBuilder) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_map_array_builder_get_value_builder(builder : c_ptr(GArrowMapArrayBuilder)) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_decimal128_array_builder_get_type() : GType;
  extern proc GARROW_DECIMAL128_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowDecimal128ArrayBuilder);
  extern proc GARROW_DECIMAL128_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal128ArrayBuilderClass);
  extern proc GARROW_IS_DECIMAL128_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DECIMAL128_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DECIMAL128_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal128ArrayBuilderClass);
  extern proc garrow_decimal128_array_builder_new(ref data_type : GArrowDecimal128DataType) : c_ptr(GArrowDecimal128ArrayBuilder);
  extern proc garrow_decimal128_array_builder_new(data_type : c_ptr(GArrowDecimal128DataType)) : c_ptr(GArrowDecimal128ArrayBuilder);
  extern proc garrow_decimal128_array_builder_append(ref builder : GArrowDecimal128ArrayBuilder, ref value : GArrowDecimal128, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_decimal128_array_builder_append(builder : c_ptr(GArrowDecimal128ArrayBuilder), value : c_ptr(GArrowDecimal128), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_decimal128_array_builder_append_value(ref builder : GArrowDecimal128ArrayBuilder, ref value : GArrowDecimal128, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_decimal128_array_builder_append_value(builder : c_ptr(GArrowDecimal128ArrayBuilder), value : c_ptr(GArrowDecimal128), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_decimal128_array_builder_append_values(ref builder : GArrowDecimal128ArrayBuilder, ref values : c_ptr(GArrowDecimal128), values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_decimal128_array_builder_append_values(builder : c_ptr(GArrowDecimal128ArrayBuilder), values : c_ptr(c_ptr(GArrowDecimal128)), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_decimal128_array_builder_append_null(ref builder : GArrowDecimal128ArrayBuilder, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_decimal128_array_builder_append_null(builder : c_ptr(GArrowDecimal128ArrayBuilder), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_decimal256_array_builder_get_type() : GType;
  extern proc GARROW_DECIMAL256_ARRAY_BUILDER(ptr : gpointer) : c_ptr(GArrowDecimal256ArrayBuilder);
  extern proc GARROW_DECIMAL256_ARRAY_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal256ArrayBuilderClass);
  extern proc GARROW_IS_DECIMAL256_ARRAY_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DECIMAL256_ARRAY_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DECIMAL256_ARRAY_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal256ArrayBuilderClass);
  extern proc garrow_decimal256_array_builder_new(ref data_type : GArrowDecimal256DataType) : c_ptr(GArrowDecimal256ArrayBuilder);
  extern proc garrow_decimal256_array_builder_new(data_type : c_ptr(GArrowDecimal256DataType)) : c_ptr(GArrowDecimal256ArrayBuilder);
  extern proc garrow_decimal256_array_builder_append_value(ref builder : GArrowDecimal256ArrayBuilder, ref value : GArrowDecimal256, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_decimal256_array_builder_append_value(builder : c_ptr(GArrowDecimal256ArrayBuilder), value : c_ptr(GArrowDecimal256), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_decimal256_array_builder_append_values(ref builder : GArrowDecimal256ArrayBuilder, ref values : c_ptr(GArrowDecimal256), values_length : gint64, ref is_valids : gboolean, is_valids_length : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_decimal256_array_builder_append_values(builder : c_ptr(GArrowDecimal256ArrayBuilder), values : c_ptr(c_ptr(GArrowDecimal256)), values_length : gint64, is_valids : c_ptr(gboolean), is_valids_length : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_chunked_array_get_type() : GType;
  extern proc GARROW_CHUNKED_ARRAY(ptr : gpointer) : c_ptr(GArrowChunkedArray);
  extern proc GARROW_CHUNKED_ARRAY_CLASS(ptr : gpointer) : c_ptr(GArrowChunkedArrayClass);
  extern proc GARROW_IS_CHUNKED_ARRAY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_CHUNKED_ARRAY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_CHUNKED_ARRAY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowChunkedArrayClass);
  extern proc garrow_chunked_array_new(ref chunks : GList) : c_ptr(GArrowChunkedArray);
  extern proc garrow_chunked_array_new(chunks : c_ptr(GList)) : c_ptr(GArrowChunkedArray);
  extern proc garrow_chunked_array_equal(ref chunked_array : GArrowChunkedArray, ref other_chunked_array : GArrowChunkedArray) : gboolean;
  extern proc garrow_chunked_array_equal(chunked_array : c_ptr(GArrowChunkedArray), other_chunked_array : c_ptr(GArrowChunkedArray)) : gboolean;
  extern proc garrow_chunked_array_get_value_data_type(ref chunked_array : GArrowChunkedArray) : c_ptr(GArrowDataType);
  extern proc garrow_chunked_array_get_value_data_type(chunked_array : c_ptr(GArrowChunkedArray)) : c_ptr(GArrowDataType);
  extern proc garrow_chunked_array_get_value_type(ref chunked_array : GArrowChunkedArray) : GArrowType;
  extern proc garrow_chunked_array_get_value_type(chunked_array : c_ptr(GArrowChunkedArray)) : GArrowType;
  extern proc garrow_chunked_array_get_length(ref chunked_array : GArrowChunkedArray) : guint64;
  extern proc garrow_chunked_array_get_length(chunked_array : c_ptr(GArrowChunkedArray)) : guint64;
  extern proc garrow_chunked_array_get_n_rows(ref chunked_array : GArrowChunkedArray) : guint64;
  extern proc garrow_chunked_array_get_n_rows(chunked_array : c_ptr(GArrowChunkedArray)) : guint64;
  extern proc garrow_chunked_array_get_n_nulls(ref chunked_array : GArrowChunkedArray) : guint64;
  extern proc garrow_chunked_array_get_n_nulls(chunked_array : c_ptr(GArrowChunkedArray)) : guint64;
  extern proc garrow_chunked_array_get_n_chunks(ref chunked_array : GArrowChunkedArray) : guint;
  extern proc garrow_chunked_array_get_n_chunks(chunked_array : c_ptr(GArrowChunkedArray)) : guint;
  extern proc garrow_chunked_array_get_chunk(ref chunked_array : GArrowChunkedArray, i : guint) : c_ptr(GArrowArray);
  extern proc garrow_chunked_array_get_chunk(chunked_array : c_ptr(GArrowChunkedArray), i : guint) : c_ptr(GArrowArray);
  extern proc garrow_chunked_array_get_chunks(ref chunked_array : GArrowChunkedArray) : c_ptr(GList);
  extern proc garrow_chunked_array_get_chunks(chunked_array : c_ptr(GArrowChunkedArray)) : c_ptr(GList);
  extern proc garrow_chunked_array_slice(ref chunked_array : GArrowChunkedArray, offset : guint64, length : guint64) : c_ptr(GArrowChunkedArray);
  extern proc garrow_chunked_array_slice(chunked_array : c_ptr(GArrowChunkedArray), offset : guint64, length : guint64) : c_ptr(GArrowChunkedArray);
  extern proc garrow_chunked_array_to_string(ref chunked_array : GArrowChunkedArray, ref error : c_ptr(GError)) : c_ptr(gchar);
  extern proc garrow_chunked_array_to_string(chunked_array : c_ptr(GArrowChunkedArray), error : c_ptr(c_ptr(GError))) : c_ptr(gchar);
  extern proc garrow_chunked_array_combine(ref chunked_array : GArrowChunkedArray, ref error : c_ptr(GError)) : c_ptr(GArrowArray);
  extern proc garrow_chunked_array_combine(chunked_array : c_ptr(GArrowChunkedArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowArray);
  extern proc garrow_codec_get_type() : GType;
  extern proc GARROW_CODEC(ptr : gpointer) : c_ptr(GArrowCodec);
  extern proc GARROW_CODEC_CLASS(ptr : gpointer) : c_ptr(GArrowCodecClass);
  extern proc GARROW_IS_CODEC(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_CODEC_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_CODEC_GET_CLASS(ptr : gpointer) : c_ptr(GArrowCodecClass);
  extern proc garrow_codec_new(type_arg : GArrowCompressionType, ref error : c_ptr(GError)) : c_ptr(GArrowCodec);
  extern proc garrow_codec_new(type_arg : GArrowCompressionType, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowCodec);
  extern proc garrow_codec_get_name(ref codec : GArrowCodec) : c_ptr(gchar);
  extern proc garrow_codec_get_name(codec : c_ptr(GArrowCodec)) : c_ptr(gchar);
  extern proc garrow_codec_get_compression_type(ref codec : GArrowCodec) : GArrowCompressionType;
  extern proc garrow_codec_get_compression_type(codec : c_ptr(GArrowCodec)) : GArrowCompressionType;
  extern proc garrow_codec_get_compression_level(ref codec : GArrowCodec) : gint;
  extern proc garrow_codec_get_compression_level(codec : c_ptr(GArrowCodec)) : gint;
  extern proc garrow_read_options_get_type() : GType;
  extern proc GARROW_READ_OPTIONS(ptr : gpointer) : c_ptr(GArrowReadOptions);
  extern proc GARROW_READ_OPTIONS_CLASS(ptr : gpointer) : c_ptr(GArrowReadOptionsClass);
  extern proc GARROW_IS_READ_OPTIONS(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_READ_OPTIONS_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_READ_OPTIONS_GET_CLASS(ptr : gpointer) : c_ptr(GArrowReadOptionsClass);
  extern proc garrow_read_options_new() : c_ptr(GArrowReadOptions);
  extern proc garrow_read_options_get_included_fields(ref options : GArrowReadOptions, ref n_fields : gsize) : c_ptr(c_int);
  extern proc garrow_read_options_get_included_fields(options : c_ptr(GArrowReadOptions), n_fields : c_ptr(gsize)) : c_ptr(c_int);
  extern proc garrow_read_options_set_included_fields(ref options : GArrowReadOptions, ref fields : c_int, n_fields : gsize) : void;
  extern proc garrow_read_options_set_included_fields(options : c_ptr(GArrowReadOptions), fields : c_ptr(c_int), n_fields : gsize) : void;
  extern proc garrow_write_options_get_type() : GType;
  extern proc GARROW_WRITE_OPTIONS(ptr : gpointer) : c_ptr(GArrowWriteOptions);
  extern proc GARROW_WRITE_OPTIONS_CLASS(ptr : gpointer) : c_ptr(GArrowWriteOptionsClass);
  extern proc GARROW_IS_WRITE_OPTIONS(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_WRITE_OPTIONS_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_WRITE_OPTIONS_GET_CLASS(ptr : gpointer) : c_ptr(GArrowWriteOptionsClass);
  extern proc garrow_write_options_new() : c_ptr(GArrowWriteOptions);
  extern proc garrow_schema_get_type() : GType;
  extern proc GARROW_SCHEMA(ptr : gpointer) : c_ptr(GArrowSchema);
  extern proc GARROW_SCHEMA_CLASS(ptr : gpointer) : c_ptr(GArrowSchemaClass);
  extern proc GARROW_IS_SCHEMA(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_SCHEMA_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_SCHEMA_GET_CLASS(ptr : gpointer) : c_ptr(GArrowSchemaClass);
  extern proc garrow_schema_new(ref fields : GList) : c_ptr(GArrowSchema);
  extern proc garrow_schema_new(fields : c_ptr(GList)) : c_ptr(GArrowSchema);
  extern proc garrow_schema_equal(ref schema : GArrowSchema, ref other_schema : GArrowSchema) : gboolean;
  extern proc garrow_schema_equal(schema : c_ptr(GArrowSchema), other_schema : c_ptr(GArrowSchema)) : gboolean;
  extern proc garrow_schema_get_field(ref schema : GArrowSchema, i : guint) : c_ptr(GArrowField);
  extern proc garrow_schema_get_field(schema : c_ptr(GArrowSchema), i : guint) : c_ptr(GArrowField);
  extern proc garrow_schema_get_field_by_name(ref schema : GArrowSchema, ref name : gchar) : c_ptr(GArrowField);
  extern proc garrow_schema_get_field_by_name(schema : c_ptr(GArrowSchema), name : c_ptr(gchar)) : c_ptr(GArrowField);
  extern proc garrow_schema_get_field_index(ref schema : GArrowSchema, ref name : gchar) : gint;
  extern proc garrow_schema_get_field_index(schema : c_ptr(GArrowSchema), name : c_ptr(gchar)) : gint;
  extern proc garrow_schema_n_fields(ref schema : GArrowSchema) : guint;
  extern proc garrow_schema_n_fields(schema : c_ptr(GArrowSchema)) : guint;
  extern proc garrow_schema_get_fields(ref schema : GArrowSchema) : c_ptr(GList);
  extern proc garrow_schema_get_fields(schema : c_ptr(GArrowSchema)) : c_ptr(GList);
  extern proc garrow_schema_to_string(ref schema : GArrowSchema) : c_ptr(gchar);
  extern proc garrow_schema_to_string(schema : c_ptr(GArrowSchema)) : c_ptr(gchar);
  extern proc garrow_schema_to_string_metadata(ref schema : GArrowSchema, show_metadata : gboolean) : c_ptr(gchar);
  extern proc garrow_schema_to_string_metadata(schema : c_ptr(GArrowSchema), show_metadata : gboolean) : c_ptr(gchar);
  extern proc garrow_schema_add_field(ref schema : GArrowSchema, i : guint, ref field : GArrowField, ref error : c_ptr(GError)) : c_ptr(GArrowSchema);
  extern proc garrow_schema_add_field(schema : c_ptr(GArrowSchema), i : guint, field : c_ptr(GArrowField), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowSchema);
  extern proc garrow_schema_remove_field(ref schema : GArrowSchema, i : guint, ref error : c_ptr(GError)) : c_ptr(GArrowSchema);
  extern proc garrow_schema_remove_field(schema : c_ptr(GArrowSchema), i : guint, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowSchema);
  extern proc garrow_schema_replace_field(ref schema : GArrowSchema, i : guint, ref field : GArrowField, ref error : c_ptr(GError)) : c_ptr(GArrowSchema);
  extern proc garrow_schema_replace_field(schema : c_ptr(GArrowSchema), i : guint, field : c_ptr(GArrowField), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowSchema);
  extern proc garrow_schema_has_metadata(ref schema : GArrowSchema) : gboolean;
  extern proc garrow_schema_has_metadata(schema : c_ptr(GArrowSchema)) : gboolean;
  extern proc garrow_schema_get_metadata(ref schema : GArrowSchema) : c_ptr(GHashTable);
  extern proc garrow_schema_get_metadata(schema : c_ptr(GArrowSchema)) : c_ptr(GHashTable);
  extern proc garrow_schema_with_metadata(ref schema : GArrowSchema, ref metadata : GHashTable) : c_ptr(GArrowSchema);
  extern proc garrow_schema_with_metadata(schema : c_ptr(GArrowSchema), metadata : c_ptr(GHashTable)) : c_ptr(GArrowSchema);
  extern proc garrow_record_batch_get_type() : GType;
  extern proc GARROW_RECORD_BATCH(ptr : gpointer) : c_ptr(GArrowRecordBatch);
  extern proc GARROW_RECORD_BATCH_CLASS(ptr : gpointer) : c_ptr(GArrowRecordBatchClass);
  extern proc GARROW_IS_RECORD_BATCH(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_RECORD_BATCH_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_RECORD_BATCH_GET_CLASS(ptr : gpointer) : c_ptr(GArrowRecordBatchClass);
  extern proc garrow_record_batch_new(ref schema : GArrowSchema, n_rows : guint32, ref columns : GList, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_new(schema : c_ptr(GArrowSchema), n_rows : guint32, columns : c_ptr(GList), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_equal(ref record_batch : GArrowRecordBatch, ref other_record_batch : GArrowRecordBatch) : gboolean;
  extern proc garrow_record_batch_equal(record_batch : c_ptr(GArrowRecordBatch), other_record_batch : c_ptr(GArrowRecordBatch)) : gboolean;
  extern proc garrow_record_batch_equal_metadata(ref record_batch : GArrowRecordBatch, ref other_record_batch : GArrowRecordBatch, check_metadata : gboolean) : gboolean;
  extern proc garrow_record_batch_equal_metadata(record_batch : c_ptr(GArrowRecordBatch), other_record_batch : c_ptr(GArrowRecordBatch), check_metadata : gboolean) : gboolean;
  extern proc garrow_record_batch_get_schema(ref record_batch : GArrowRecordBatch) : c_ptr(GArrowSchema);
  extern proc garrow_record_batch_get_schema(record_batch : c_ptr(GArrowRecordBatch)) : c_ptr(GArrowSchema);
  extern proc garrow_record_batch_get_column_data(ref record_batch : GArrowRecordBatch, i : gint) : c_ptr(GArrowArray);
  extern proc garrow_record_batch_get_column_data(record_batch : c_ptr(GArrowRecordBatch), i : gint) : c_ptr(GArrowArray);
  extern proc garrow_record_batch_get_column_name(ref record_batch : GArrowRecordBatch, i : gint) : c_ptr(gchar);
  extern proc garrow_record_batch_get_column_name(record_batch : c_ptr(GArrowRecordBatch), i : gint) : c_ptr(gchar);
  extern proc garrow_record_batch_get_n_columns(ref record_batch : GArrowRecordBatch) : guint;
  extern proc garrow_record_batch_get_n_columns(record_batch : c_ptr(GArrowRecordBatch)) : guint;
  extern proc garrow_record_batch_get_n_rows(ref record_batch : GArrowRecordBatch) : gint64;
  extern proc garrow_record_batch_get_n_rows(record_batch : c_ptr(GArrowRecordBatch)) : gint64;
  extern proc garrow_record_batch_slice(ref record_batch : GArrowRecordBatch, offset : gint64, length : gint64) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_slice(record_batch : c_ptr(GArrowRecordBatch), offset : gint64, length : gint64) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_to_string(ref record_batch : GArrowRecordBatch, ref error : c_ptr(GError)) : c_ptr(gchar);
  extern proc garrow_record_batch_to_string(record_batch : c_ptr(GArrowRecordBatch), error : c_ptr(c_ptr(GError))) : c_ptr(gchar);
  extern proc garrow_record_batch_add_column(ref record_batch : GArrowRecordBatch, i : guint, ref field : GArrowField, ref column : GArrowArray, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_add_column(record_batch : c_ptr(GArrowRecordBatch), i : guint, field : c_ptr(GArrowField), column : c_ptr(GArrowArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_remove_column(ref record_batch : GArrowRecordBatch, i : guint, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_remove_column(record_batch : c_ptr(GArrowRecordBatch), i : guint, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_serialize(ref record_batch : GArrowRecordBatch, ref options : GArrowWriteOptions, ref error : c_ptr(GError)) : c_ptr(GArrowBuffer);
  extern proc garrow_record_batch_serialize(record_batch : c_ptr(GArrowRecordBatch), options : c_ptr(GArrowWriteOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowBuffer);
  extern proc garrow_record_batch_iterator_get_type() : GType;
  extern proc GARROW_RECORD_BATCH_ITERATOR(ptr : gpointer) : c_ptr(GArrowRecordBatchIterator);
  extern proc GARROW_RECORD_BATCH_ITERATOR_CLASS(ptr : gpointer) : c_ptr(GArrowRecordBatchIteratorClass);
  extern proc GARROW_IS_RECORD_BATCH_ITERATOR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_RECORD_BATCH_ITERATOR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_RECORD_BATCH_ITERATOR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowRecordBatchIteratorClass);
  extern proc garrow_record_batch_iterator_new(ref record_batches : GList) : c_ptr(GArrowRecordBatchIterator);
  extern proc garrow_record_batch_iterator_new(record_batches : c_ptr(GList)) : c_ptr(GArrowRecordBatchIterator);
  extern proc garrow_record_batch_iterator_next(ref iterator : GArrowRecordBatchIterator, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_iterator_next(iterator : c_ptr(GArrowRecordBatchIterator), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_iterator_equal(ref iterator : GArrowRecordBatchIterator, ref other_iterator : GArrowRecordBatchIterator) : gboolean;
  extern proc garrow_record_batch_iterator_equal(iterator : c_ptr(GArrowRecordBatchIterator), other_iterator : c_ptr(GArrowRecordBatchIterator)) : gboolean;
  extern proc garrow_record_batch_iterator_to_list(ref iterator : GArrowRecordBatchIterator, ref error : c_ptr(GError)) : c_ptr(GList);
  extern proc garrow_record_batch_iterator_to_list(iterator : c_ptr(GArrowRecordBatchIterator), error : c_ptr(c_ptr(GError))) : c_ptr(GList);
  extern proc garrow_scalar_get_type() : GType;
  extern proc GARROW_SCALAR(ptr : gpointer) : c_ptr(GArrowScalar);
  extern proc GARROW_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowScalarClass);
  extern proc GARROW_IS_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowScalarClass);
  extern proc garrow_scalar_parse(ref data_type : GArrowDataType, ref data : guint8, size : gsize, ref error : c_ptr(GError)) : c_ptr(GArrowScalar);
  extern proc garrow_scalar_parse(data_type : c_ptr(GArrowDataType), data : c_ptr(guint8), size : gsize, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowScalar);
  extern proc garrow_scalar_get_data_type(ref scalar : GArrowScalar) : c_ptr(GArrowDataType);
  extern proc garrow_scalar_get_data_type(scalar : c_ptr(GArrowScalar)) : c_ptr(GArrowDataType);
  extern proc garrow_scalar_is_valid(ref scalar : GArrowScalar) : gboolean;
  extern proc garrow_scalar_is_valid(scalar : c_ptr(GArrowScalar)) : gboolean;
  extern proc garrow_scalar_equal(ref scalar : GArrowScalar, ref other_scalar : GArrowScalar) : gboolean;
  extern proc garrow_scalar_equal(scalar : c_ptr(GArrowScalar), other_scalar : c_ptr(GArrowScalar)) : gboolean;
  extern proc garrow_scalar_equal_options(ref scalar : GArrowScalar, ref other_scalar : GArrowScalar, ref options : GArrowEqualOptions) : gboolean;
  extern proc garrow_scalar_equal_options(scalar : c_ptr(GArrowScalar), other_scalar : c_ptr(GArrowScalar), options : c_ptr(GArrowEqualOptions)) : gboolean;
  extern proc garrow_scalar_to_string(ref scalar : GArrowScalar) : c_ptr(gchar);
  extern proc garrow_scalar_to_string(scalar : c_ptr(GArrowScalar)) : c_ptr(gchar);
  extern proc garrow_scalar_cast(ref scalar : GArrowScalar, ref data_type : GArrowDataType, ref options : GArrowCastOptions, ref error : c_ptr(GError)) : c_ptr(GArrowScalar);
  extern proc garrow_scalar_cast(scalar : c_ptr(GArrowScalar), data_type : c_ptr(GArrowDataType), options : c_ptr(GArrowCastOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowScalar);
  extern proc garrow_null_scalar_get_type() : GType;
  extern proc GARROW_NULL_SCALAR(ptr : gpointer) : c_ptr(GArrowNullScalar);
  extern proc GARROW_NULL_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowNullScalarClass);
  extern proc GARROW_IS_NULL_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_NULL_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_NULL_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowNullScalarClass);
  extern proc garrow_null_scalar_new() : c_ptr(GArrowNullScalar);
  extern proc garrow_boolean_scalar_get_type() : GType;
  extern proc GARROW_BOOLEAN_SCALAR(ptr : gpointer) : c_ptr(GArrowBooleanScalar);
  extern proc GARROW_BOOLEAN_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowBooleanScalarClass);
  extern proc GARROW_IS_BOOLEAN_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_BOOLEAN_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_BOOLEAN_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowBooleanScalarClass);
  extern proc garrow_boolean_scalar_new(value : gboolean) : c_ptr(GArrowBooleanScalar);
  extern proc garrow_boolean_scalar_get_value(ref scalar : GArrowBooleanScalar) : gboolean;
  extern proc garrow_boolean_scalar_get_value(scalar : c_ptr(GArrowBooleanScalar)) : gboolean;
  extern proc garrow_int8_scalar_get_type() : GType;
  extern proc GARROW_INT8_SCALAR(ptr : gpointer) : c_ptr(GArrowInt8Scalar);
  extern proc GARROW_INT8_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowInt8ScalarClass);
  extern proc GARROW_IS_INT8_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT8_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT8_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt8ScalarClass);
  extern proc garrow_int8_scalar_new(value : gint8) : c_ptr(GArrowInt8Scalar);
  extern proc garrow_int8_scalar_get_value(ref scalar : GArrowInt8Scalar) : gint8;
  extern proc garrow_int8_scalar_get_value(scalar : c_ptr(GArrowInt8Scalar)) : gint8;
  extern proc garrow_int16_scalar_get_type() : GType;
  extern proc GARROW_INT16_SCALAR(ptr : gpointer) : c_ptr(GArrowInt16Scalar);
  extern proc GARROW_INT16_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowInt16ScalarClass);
  extern proc GARROW_IS_INT16_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT16_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT16_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt16ScalarClass);
  extern proc garrow_int16_scalar_new(value : gint16) : c_ptr(GArrowInt16Scalar);
  extern proc garrow_int16_scalar_get_value(ref scalar : GArrowInt16Scalar) : gint16;
  extern proc garrow_int16_scalar_get_value(scalar : c_ptr(GArrowInt16Scalar)) : gint16;
  extern proc garrow_int32_scalar_get_type() : GType;
  extern proc GARROW_INT32_SCALAR(ptr : gpointer) : c_ptr(GArrowInt32Scalar);
  extern proc GARROW_INT32_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowInt32ScalarClass);
  extern proc GARROW_IS_INT32_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT32_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT32_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt32ScalarClass);
  extern proc garrow_int32_scalar_new(value : gint32) : c_ptr(GArrowInt32Scalar);
  extern proc garrow_int32_scalar_get_value(ref scalar : GArrowInt32Scalar) : gint32;
  extern proc garrow_int32_scalar_get_value(scalar : c_ptr(GArrowInt32Scalar)) : gint32;
  extern proc garrow_int64_scalar_get_type() : GType;
  extern proc GARROW_INT64_SCALAR(ptr : gpointer) : c_ptr(GArrowInt64Scalar);
  extern proc GARROW_INT64_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowInt64ScalarClass);
  extern proc GARROW_IS_INT64_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INT64_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INT64_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInt64ScalarClass);
  extern proc garrow_int64_scalar_new(value : gint64) : c_ptr(GArrowInt64Scalar);
  extern proc garrow_int64_scalar_get_value(ref scalar : GArrowInt64Scalar) : gint64;
  extern proc garrow_int64_scalar_get_value(scalar : c_ptr(GArrowInt64Scalar)) : gint64;
  extern proc garrow_uint8_scalar_get_type() : GType;
  extern proc GARROW_UINT8_SCALAR(ptr : gpointer) : c_ptr(GArrowUInt8Scalar);
  extern proc GARROW_UINT8_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowUInt8ScalarClass);
  extern proc GARROW_IS_UINT8_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT8_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT8_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt8ScalarClass);
  extern proc garrow_uint8_scalar_new(value : guint8) : c_ptr(GArrowUInt8Scalar);
  extern proc garrow_uint8_scalar_get_value(ref scalar : GArrowUInt8Scalar) : guint8;
  extern proc garrow_uint8_scalar_get_value(scalar : c_ptr(GArrowUInt8Scalar)) : guint8;
  extern proc garrow_uint16_scalar_get_type() : GType;
  extern proc GARROW_UINT16_SCALAR(ptr : gpointer) : c_ptr(GArrowUInt16Scalar);
  extern proc GARROW_UINT16_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowUInt16ScalarClass);
  extern proc GARROW_IS_UINT16_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT16_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT16_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt16ScalarClass);
  extern proc garrow_uint16_scalar_new(value : guint16) : c_ptr(GArrowUInt16Scalar);
  extern proc garrow_uint16_scalar_get_value(ref scalar : GArrowUInt16Scalar) : guint16;
  extern proc garrow_uint16_scalar_get_value(scalar : c_ptr(GArrowUInt16Scalar)) : guint16;
  extern proc garrow_uint32_scalar_get_type() : GType;
  extern proc GARROW_UINT32_SCALAR(ptr : gpointer) : c_ptr(GArrowUInt32Scalar);
  extern proc GARROW_UINT32_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowUInt32ScalarClass);
  extern proc GARROW_IS_UINT32_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT32_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT32_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt32ScalarClass);
  extern proc garrow_uint32_scalar_new(value : guint32) : c_ptr(GArrowUInt32Scalar);
  extern proc garrow_uint32_scalar_get_value(ref scalar : GArrowUInt32Scalar) : guint32;
  extern proc garrow_uint32_scalar_get_value(scalar : c_ptr(GArrowUInt32Scalar)) : guint32;
  extern proc garrow_uint64_scalar_get_type() : GType;
  extern proc GARROW_UINT64_SCALAR(ptr : gpointer) : c_ptr(GArrowUInt64Scalar);
  extern proc GARROW_UINT64_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowUInt64ScalarClass);
  extern proc GARROW_IS_UINT64_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UINT64_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UINT64_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUInt64ScalarClass);
  extern proc garrow_uint64_scalar_new(value : guint64) : c_ptr(GArrowUInt64Scalar);
  extern proc garrow_uint64_scalar_get_value(ref scalar : GArrowUInt64Scalar) : guint64;
  extern proc garrow_uint64_scalar_get_value(scalar : c_ptr(GArrowUInt64Scalar)) : guint64;
  extern proc garrow_float_scalar_get_type() : GType;
  extern proc GARROW_FLOAT_SCALAR(ptr : gpointer) : c_ptr(GArrowFloatScalar);
  extern proc GARROW_FLOAT_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowFloatScalarClass);
  extern proc GARROW_IS_FLOAT_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FLOAT_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FLOAT_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFloatScalarClass);
  extern proc garrow_float_scalar_new(value : gfloat) : c_ptr(GArrowFloatScalar);
  extern proc garrow_float_scalar_get_value(ref scalar : GArrowFloatScalar) : gfloat;
  extern proc garrow_float_scalar_get_value(scalar : c_ptr(GArrowFloatScalar)) : gfloat;
  extern proc garrow_double_scalar_get_type() : GType;
  extern proc GARROW_DOUBLE_SCALAR(ptr : gpointer) : c_ptr(GArrowDoubleScalar);
  extern proc GARROW_DOUBLE_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowDoubleScalarClass);
  extern proc GARROW_IS_DOUBLE_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DOUBLE_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DOUBLE_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDoubleScalarClass);
  extern proc garrow_double_scalar_new(value : gdouble) : c_ptr(GArrowDoubleScalar);
  extern proc garrow_double_scalar_get_value(ref scalar : GArrowDoubleScalar) : gdouble;
  extern proc garrow_double_scalar_get_value(scalar : c_ptr(GArrowDoubleScalar)) : gdouble;
  extern proc garrow_base_binary_scalar_get_type() : GType;
  extern proc GARROW_BASE_BINARY_SCALAR(ptr : gpointer) : c_ptr(GArrowBaseBinaryScalar);
  extern proc GARROW_BASE_BINARY_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowBaseBinaryScalarClass);
  extern proc GARROW_IS_BASE_BINARY_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_BASE_BINARY_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_BASE_BINARY_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowBaseBinaryScalarClass);
  extern proc garrow_base_binary_scalar_get_value(ref scalar : GArrowBaseBinaryScalar) : c_ptr(GArrowBuffer);
  extern proc garrow_base_binary_scalar_get_value(scalar : c_ptr(GArrowBaseBinaryScalar)) : c_ptr(GArrowBuffer);
  extern proc garrow_binary_scalar_get_type() : GType;
  extern proc GARROW_BINARY_SCALAR(ptr : gpointer) : c_ptr(GArrowBinaryScalar);
  extern proc GARROW_BINARY_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowBinaryScalarClass);
  extern proc GARROW_IS_BINARY_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_BINARY_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_BINARY_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowBinaryScalarClass);
  extern proc garrow_binary_scalar_new(ref value : GArrowBuffer) : c_ptr(GArrowBinaryScalar);
  extern proc garrow_binary_scalar_new(value : c_ptr(GArrowBuffer)) : c_ptr(GArrowBinaryScalar);
  extern proc garrow_string_scalar_get_type() : GType;
  extern proc GARROW_STRING_SCALAR(ptr : gpointer) : c_ptr(GArrowStringScalar);
  extern proc GARROW_STRING_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowStringScalarClass);
  extern proc GARROW_IS_STRING_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_STRING_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_STRING_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowStringScalarClass);
  extern proc garrow_string_scalar_new(ref value : GArrowBuffer) : c_ptr(GArrowStringScalar);
  extern proc garrow_string_scalar_new(value : c_ptr(GArrowBuffer)) : c_ptr(GArrowStringScalar);
  extern proc garrow_large_binary_scalar_get_type() : GType;
  extern proc GARROW_LARGE_BINARY_SCALAR(ptr : gpointer) : c_ptr(GArrowLargeBinaryScalar);
  extern proc GARROW_LARGE_BINARY_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowLargeBinaryScalarClass);
  extern proc GARROW_IS_LARGE_BINARY_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LARGE_BINARY_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LARGE_BINARY_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowLargeBinaryScalarClass);
  extern proc garrow_large_binary_scalar_new(ref value : GArrowBuffer) : c_ptr(GArrowLargeBinaryScalar);
  extern proc garrow_large_binary_scalar_new(value : c_ptr(GArrowBuffer)) : c_ptr(GArrowLargeBinaryScalar);
  extern proc garrow_large_string_scalar_get_type() : GType;
  extern proc GARROW_LARGE_STRING_SCALAR(ptr : gpointer) : c_ptr(GArrowLargeStringScalar);
  extern proc GARROW_LARGE_STRING_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowLargeStringScalarClass);
  extern proc GARROW_IS_LARGE_STRING_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LARGE_STRING_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LARGE_STRING_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowLargeStringScalarClass);
  extern proc garrow_large_string_scalar_new(ref value : GArrowBuffer) : c_ptr(GArrowLargeStringScalar);
  extern proc garrow_large_string_scalar_new(value : c_ptr(GArrowBuffer)) : c_ptr(GArrowLargeStringScalar);
  extern proc garrow_fixed_size_binary_scalar_get_type() : GType;
  extern proc GARROW_FIXED_SIZE_BINARY_SCALAR(ptr : gpointer) : c_ptr(GArrowFixedSizeBinaryScalar);
  extern proc GARROW_FIXED_SIZE_BINARY_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowFixedSizeBinaryScalarClass);
  extern proc GARROW_IS_FIXED_SIZE_BINARY_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FIXED_SIZE_BINARY_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FIXED_SIZE_BINARY_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFixedSizeBinaryScalarClass);
  extern proc garrow_fixed_size_binary_scalar_new(ref data_type : GArrowFixedSizeBinaryDataType, ref value : GArrowBuffer) : c_ptr(GArrowFixedSizeBinaryScalar);
  extern proc garrow_fixed_size_binary_scalar_new(data_type : c_ptr(GArrowFixedSizeBinaryDataType), value : c_ptr(GArrowBuffer)) : c_ptr(GArrowFixedSizeBinaryScalar);
  extern proc garrow_date32_scalar_get_type() : GType;
  extern proc GARROW_DATE32_SCALAR(ptr : gpointer) : c_ptr(GArrowDate32Scalar);
  extern proc GARROW_DATE32_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowDate32ScalarClass);
  extern proc GARROW_IS_DATE32_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DATE32_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DATE32_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDate32ScalarClass);
  extern proc garrow_date32_scalar_new(value : gint32) : c_ptr(GArrowDate32Scalar);
  extern proc garrow_date32_scalar_get_value(ref scalar : GArrowDate32Scalar) : gint32;
  extern proc garrow_date32_scalar_get_value(scalar : c_ptr(GArrowDate32Scalar)) : gint32;
  extern proc garrow_date64_scalar_get_type() : GType;
  extern proc GARROW_DATE64_SCALAR(ptr : gpointer) : c_ptr(GArrowDate64Scalar);
  extern proc GARROW_DATE64_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowDate64ScalarClass);
  extern proc GARROW_IS_DATE64_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DATE64_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DATE64_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDate64ScalarClass);
  extern proc garrow_date64_scalar_new(value : gint64) : c_ptr(GArrowDate64Scalar);
  extern proc garrow_date64_scalar_get_value(ref scalar : GArrowDate64Scalar) : gint64;
  extern proc garrow_date64_scalar_get_value(scalar : c_ptr(GArrowDate64Scalar)) : gint64;
  extern proc garrow_time32_scalar_get_type() : GType;
  extern proc GARROW_TIME32_SCALAR(ptr : gpointer) : c_ptr(GArrowTime32Scalar);
  extern proc GARROW_TIME32_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowTime32ScalarClass);
  extern proc GARROW_IS_TIME32_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TIME32_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TIME32_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTime32ScalarClass);
  extern proc garrow_time32_scalar_new(ref data_type : GArrowTime32DataType, value : gint32) : c_ptr(GArrowTime32Scalar);
  extern proc garrow_time32_scalar_new(data_type : c_ptr(GArrowTime32DataType), value : gint32) : c_ptr(GArrowTime32Scalar);
  extern proc garrow_time32_scalar_get_value(ref scalar : GArrowTime32Scalar) : gint32;
  extern proc garrow_time32_scalar_get_value(scalar : c_ptr(GArrowTime32Scalar)) : gint32;
  extern proc garrow_time64_scalar_get_type() : GType;
  extern proc GARROW_TIME64_SCALAR(ptr : gpointer) : c_ptr(GArrowTime64Scalar);
  extern proc GARROW_TIME64_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowTime64ScalarClass);
  extern proc GARROW_IS_TIME64_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TIME64_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TIME64_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTime64ScalarClass);
  extern proc garrow_time64_scalar_new(ref data_type : GArrowTime64DataType, value : gint64) : c_ptr(GArrowTime64Scalar);
  extern proc garrow_time64_scalar_new(data_type : c_ptr(GArrowTime64DataType), value : gint64) : c_ptr(GArrowTime64Scalar);
  extern proc garrow_time64_scalar_get_value(ref scalar : GArrowTime64Scalar) : gint64;
  extern proc garrow_time64_scalar_get_value(scalar : c_ptr(GArrowTime64Scalar)) : gint64;
  extern proc garrow_timestamp_scalar_get_type() : GType;
  extern proc GARROW_TIMESTAMP_SCALAR(ptr : gpointer) : c_ptr(GArrowTimestampScalar);
  extern proc GARROW_TIMESTAMP_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowTimestampScalarClass);
  extern proc GARROW_IS_TIMESTAMP_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TIMESTAMP_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TIMESTAMP_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTimestampScalarClass);
  extern proc garrow_timestamp_scalar_new(ref data_type : GArrowTimestampDataType, value : gint64) : c_ptr(GArrowTimestampScalar);
  extern proc garrow_timestamp_scalar_new(data_type : c_ptr(GArrowTimestampDataType), value : gint64) : c_ptr(GArrowTimestampScalar);
  extern proc garrow_timestamp_scalar_get_value(ref scalar : GArrowTimestampScalar) : gint64;
  extern proc garrow_timestamp_scalar_get_value(scalar : c_ptr(GArrowTimestampScalar)) : gint64;
  extern proc garrow_decimal128_scalar_get_type() : GType;
  extern proc GARROW_DECIMAL128_SCALAR(ptr : gpointer) : c_ptr(GArrowDecimal128Scalar);
  extern proc GARROW_DECIMAL128_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal128ScalarClass);
  extern proc GARROW_IS_DECIMAL128_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DECIMAL128_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DECIMAL128_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal128ScalarClass);
  extern proc garrow_decimal128_scalar_new(ref data_type : GArrowDecimal128DataType, ref value : GArrowDecimal128) : c_ptr(GArrowDecimal128Scalar);
  extern proc garrow_decimal128_scalar_new(data_type : c_ptr(GArrowDecimal128DataType), value : c_ptr(GArrowDecimal128)) : c_ptr(GArrowDecimal128Scalar);
  extern proc garrow_decimal128_scalar_get_value(ref scalar : GArrowDecimal128Scalar) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal128_scalar_get_value(scalar : c_ptr(GArrowDecimal128Scalar)) : c_ptr(GArrowDecimal128);
  extern proc garrow_decimal256_scalar_get_type() : GType;
  extern proc GARROW_DECIMAL256_SCALAR(ptr : gpointer) : c_ptr(GArrowDecimal256Scalar);
  extern proc GARROW_DECIMAL256_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal256ScalarClass);
  extern proc GARROW_IS_DECIMAL256_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DECIMAL256_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DECIMAL256_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDecimal256ScalarClass);
  extern proc garrow_decimal256_scalar_new(ref data_type : GArrowDecimal256DataType, ref value : GArrowDecimal256) : c_ptr(GArrowDecimal256Scalar);
  extern proc garrow_decimal256_scalar_new(data_type : c_ptr(GArrowDecimal256DataType), value : c_ptr(GArrowDecimal256)) : c_ptr(GArrowDecimal256Scalar);
  extern proc garrow_decimal256_scalar_get_value(ref scalar : GArrowDecimal256Scalar) : c_ptr(GArrowDecimal256);
  extern proc garrow_decimal256_scalar_get_value(scalar : c_ptr(GArrowDecimal256Scalar)) : c_ptr(GArrowDecimal256);
  extern proc garrow_base_list_scalar_get_type() : GType;
  extern proc GARROW_BASE_LIST_SCALAR(ptr : gpointer) : c_ptr(GArrowBaseListScalar);
  extern proc GARROW_BASE_LIST_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowBaseListScalarClass);
  extern proc GARROW_IS_BASE_LIST_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_BASE_LIST_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_BASE_LIST_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowBaseListScalarClass);
  extern proc garrow_base_list_scalar_get_value(ref scalar : GArrowBaseListScalar) : c_ptr(GArrowArray);
  extern proc garrow_base_list_scalar_get_value(scalar : c_ptr(GArrowBaseListScalar)) : c_ptr(GArrowArray);
  extern proc garrow_list_scalar_get_type() : GType;
  extern proc GARROW_LIST_SCALAR(ptr : gpointer) : c_ptr(GArrowListScalar);
  extern proc GARROW_LIST_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowListScalarClass);
  extern proc GARROW_IS_LIST_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LIST_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LIST_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowListScalarClass);
  extern proc garrow_list_scalar_new(ref value : GArrowListArray) : c_ptr(GArrowListScalar);
  extern proc garrow_list_scalar_new(value : c_ptr(GArrowListArray)) : c_ptr(GArrowListScalar);
  extern proc garrow_large_list_scalar_get_type() : GType;
  extern proc GARROW_LARGE_LIST_SCALAR(ptr : gpointer) : c_ptr(GArrowLargeListScalar);
  extern proc GARROW_LARGE_LIST_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowLargeListScalarClass);
  extern proc GARROW_IS_LARGE_LIST_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LARGE_LIST_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LARGE_LIST_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowLargeListScalarClass);
  extern proc garrow_large_list_scalar_new(ref value : GArrowLargeListArray) : c_ptr(GArrowLargeListScalar);
  extern proc garrow_large_list_scalar_new(value : c_ptr(GArrowLargeListArray)) : c_ptr(GArrowLargeListScalar);
  extern proc garrow_map_scalar_get_type() : GType;
  extern proc GARROW_MAP_SCALAR(ptr : gpointer) : c_ptr(GArrowMapScalar);
  extern proc GARROW_MAP_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowMapScalarClass);
  extern proc GARROW_IS_MAP_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_MAP_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_MAP_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowMapScalarClass);
  extern proc garrow_map_scalar_new(ref value : GArrowStructArray) : c_ptr(GArrowMapScalar);
  extern proc garrow_map_scalar_new(value : c_ptr(GArrowStructArray)) : c_ptr(GArrowMapScalar);
  extern proc garrow_struct_scalar_get_type() : GType;
  extern proc GARROW_STRUCT_SCALAR(ptr : gpointer) : c_ptr(GArrowStructScalar);
  extern proc GARROW_STRUCT_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowStructScalarClass);
  extern proc GARROW_IS_STRUCT_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_STRUCT_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_STRUCT_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowStructScalarClass);
  extern proc garrow_struct_scalar_new(ref data_type : GArrowStructDataType, ref value : GList) : c_ptr(GArrowStructScalar);
  extern proc garrow_struct_scalar_new(data_type : c_ptr(GArrowStructDataType), value : c_ptr(GList)) : c_ptr(GArrowStructScalar);
  extern proc garrow_struct_scalar_get_value(ref scalar : GArrowStructScalar) : c_ptr(GList);
  extern proc garrow_struct_scalar_get_value(scalar : c_ptr(GArrowStructScalar)) : c_ptr(GList);
  extern proc garrow_union_scalar_get_type() : GType;
  extern proc GARROW_UNION_SCALAR(ptr : gpointer) : c_ptr(GArrowUnionScalar);
  extern proc GARROW_UNION_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowUnionScalarClass);
  extern proc GARROW_IS_UNION_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_UNION_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_UNION_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowUnionScalarClass);
  extern proc garrow_union_scalar_get_value(ref scalar : GArrowUnionScalar) : c_ptr(GArrowScalar);
  extern proc garrow_union_scalar_get_value(scalar : c_ptr(GArrowUnionScalar)) : c_ptr(GArrowScalar);
  extern proc garrow_sparse_union_scalar_get_type() : GType;
  extern proc GARROW_SPARSE_UNION_SCALAR(ptr : gpointer) : c_ptr(GArrowSparseUnionScalar);
  extern proc GARROW_SPARSE_UNION_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowSparseUnionScalarClass);
  extern proc GARROW_IS_SPARSE_UNION_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_SPARSE_UNION_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_SPARSE_UNION_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowSparseUnionScalarClass);
  extern proc garrow_sparse_union_scalar_new(ref data_type : GArrowSparseUnionDataType, ref value : GArrowScalar) : c_ptr(GArrowSparseUnionScalar);
  extern proc garrow_sparse_union_scalar_new(data_type : c_ptr(GArrowSparseUnionDataType), value : c_ptr(GArrowScalar)) : c_ptr(GArrowSparseUnionScalar);
  extern proc garrow_dense_union_scalar_get_type() : GType;
  extern proc GARROW_DENSE_UNION_SCALAR(ptr : gpointer) : c_ptr(GArrowDenseUnionScalar);
  extern proc GARROW_DENSE_UNION_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowDenseUnionScalarClass);
  extern proc GARROW_IS_DENSE_UNION_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DENSE_UNION_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DENSE_UNION_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDenseUnionScalarClass);
  extern proc garrow_dense_union_scalar_new(ref data_type : GArrowDenseUnionDataType, ref value : GArrowScalar) : c_ptr(GArrowDenseUnionScalar);
  extern proc garrow_dense_union_scalar_new(data_type : c_ptr(GArrowDenseUnionDataType), value : c_ptr(GArrowScalar)) : c_ptr(GArrowDenseUnionScalar);
  extern proc garrow_extension_scalar_get_type() : GType;
  extern proc GARROW_EXTENSION_SCALAR(ptr : gpointer) : c_ptr(GArrowExtensionScalar);
  extern proc GARROW_EXTENSION_SCALAR_CLASS(ptr : gpointer) : c_ptr(GArrowExtensionScalarClass);
  extern proc GARROW_IS_EXTENSION_SCALAR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_EXTENSION_SCALAR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_EXTENSION_SCALAR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowExtensionScalarClass);
  // Overload for empty varargs
  // Overload for empty varargs
  // Overload for empty varargs
  // Unable to generate function 'g_initable_new_valist' due to va_list argument
  // Overload for empty varargs
  // Unable to generate function 'g_async_initable_new_valist_async' due to va_list argument
  // Overload for empty varargs
  // Unable to generate function 'g_output_stream_vprintf' due to va_list argument
  // Overload for empty varargs
  // Unable to generate function 'g_dbus_error_set_dbus_error_valist' due to va_list argument
  // Overload for empty varargs
  // Unable to generate function 'g_dbus_message_new_method_error_valist' due to va_list argument
  // Overload for empty varargs
  // Unable to generate function 'g_dbus_method_invocation_return_error_valist' due to va_list argument
  // Overload for empty varargs
  // Fields omitted because one or more of the identifiers is a Chapel keyword
  // Overload for empty varargs
  // Overload for empty varargs
  // Overload for empty varargs
  // Overload for empty varargs
  // Overload for empty varargs
  // Overload for empty varargs
  // Overload for empty varargs
  // Overload for empty varargs
  // Overload for empty varargs
  // Overload for empty varargs
  // Unable to generate function 'g_simple_async_result_set_error_va' due to va_list argument
  // Overload for empty varargs
  // Overload for empty varargs
  // Overload for empty varargs
  // Overload for empty varargs
  // Overload for empty varargs
  extern proc garrow_tensor_get_type() : GType;
  extern proc GARROW_TENSOR(ptr : gpointer) : c_ptr(GArrowTensor);
  extern proc GARROW_TENSOR_CLASS(ptr : gpointer) : c_ptr(GArrowTensorClass);
  extern proc GARROW_IS_TENSOR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TENSOR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TENSOR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTensorClass);
  extern proc garrow_tensor_new(ref data_type : GArrowDataType, ref data : GArrowBuffer, ref shape : gint64, n_dimensions : gsize, ref strides : gint64, n_strides : gsize, ref dimension_names : c_ptr(gchar), n_dimension_names : gsize) : c_ptr(GArrowTensor);
  extern proc garrow_tensor_new(data_type : c_ptr(GArrowDataType), data : c_ptr(GArrowBuffer), shape : c_ptr(gint64), n_dimensions : gsize, strides : c_ptr(gint64), n_strides : gsize, dimension_names : c_ptr(c_ptr(gchar)), n_dimension_names : gsize) : c_ptr(GArrowTensor);
  extern proc garrow_tensor_equal(ref tensor : GArrowTensor, ref other_tensor : GArrowTensor) : gboolean;
  extern proc garrow_tensor_equal(tensor : c_ptr(GArrowTensor), other_tensor : c_ptr(GArrowTensor)) : gboolean;
  extern proc garrow_tensor_get_value_data_type(ref tensor : GArrowTensor) : c_ptr(GArrowDataType);
  extern proc garrow_tensor_get_value_data_type(tensor : c_ptr(GArrowTensor)) : c_ptr(GArrowDataType);
  extern proc garrow_tensor_get_value_type(ref tensor : GArrowTensor) : GArrowType;
  extern proc garrow_tensor_get_value_type(tensor : c_ptr(GArrowTensor)) : GArrowType;
  extern proc garrow_tensor_get_buffer(ref tensor : GArrowTensor) : c_ptr(GArrowBuffer);
  extern proc garrow_tensor_get_buffer(tensor : c_ptr(GArrowTensor)) : c_ptr(GArrowBuffer);
  extern proc garrow_tensor_get_shape(ref tensor : GArrowTensor, ref n_dimensions : gint) : c_ptr(gint64);
  extern proc garrow_tensor_get_shape(tensor : c_ptr(GArrowTensor), n_dimensions : c_ptr(gint)) : c_ptr(gint64);
  extern proc garrow_tensor_get_strides(ref tensor : GArrowTensor, ref n_strides : gint) : c_ptr(gint64);
  extern proc garrow_tensor_get_strides(tensor : c_ptr(GArrowTensor), n_strides : c_ptr(gint)) : c_ptr(gint64);
  extern proc garrow_tensor_get_n_dimensions(ref tensor : GArrowTensor) : gint;
  extern proc garrow_tensor_get_n_dimensions(tensor : c_ptr(GArrowTensor)) : gint;
  extern proc garrow_tensor_get_dimension_name(ref tensor : GArrowTensor, i : gint) : c_ptr(gchar);
  extern proc garrow_tensor_get_dimension_name(tensor : c_ptr(GArrowTensor), i : gint) : c_ptr(gchar);
  extern proc garrow_tensor_get_size(ref tensor : GArrowTensor) : gint64;
  extern proc garrow_tensor_get_size(tensor : c_ptr(GArrowTensor)) : gint64;
  extern proc garrow_tensor_is_mutable(ref tensor : GArrowTensor) : gboolean;
  extern proc garrow_tensor_is_mutable(tensor : c_ptr(GArrowTensor)) : gboolean;
  extern proc garrow_tensor_is_contiguous(ref tensor : GArrowTensor) : gboolean;
  extern proc garrow_tensor_is_contiguous(tensor : c_ptr(GArrowTensor)) : gboolean;
  extern proc garrow_tensor_is_row_major(ref tensor : GArrowTensor) : gboolean;
  extern proc garrow_tensor_is_row_major(tensor : c_ptr(GArrowTensor)) : gboolean;
  extern proc garrow_tensor_is_column_major(ref tensor : GArrowTensor) : gboolean;
  extern proc garrow_tensor_is_column_major(tensor : c_ptr(GArrowTensor)) : gboolean;
  extern proc garrow_output_stream_get_type() : GType;
  extern proc GARROW_OUTPUT_STREAM(ptr : gpointer) : c_ptr(GArrowOutputStream);
  extern proc GARROW_OUTPUT_STREAM_CLASS(ptr : gpointer) : c_ptr(GArrowOutputStreamClass);
  extern proc GARROW_IS_OUTPUT_STREAM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_OUTPUT_STREAM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_OUTPUT_STREAM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowOutputStreamClass);
  extern proc garrow_output_stream_align(ref stream : GArrowOutputStream, alignment : gint32, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_output_stream_align(stream : c_ptr(GArrowOutputStream), alignment : gint32, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_output_stream_write_tensor(ref stream : GArrowOutputStream, ref tensor : GArrowTensor, ref error : c_ptr(GError)) : gint64;
  extern proc garrow_output_stream_write_tensor(stream : c_ptr(GArrowOutputStream), tensor : c_ptr(GArrowTensor), error : c_ptr(c_ptr(GError))) : gint64;
  extern proc garrow_output_stream_write_record_batch(ref stream : GArrowOutputStream, ref record_batch : GArrowRecordBatch, ref options : GArrowWriteOptions, ref error : c_ptr(GError)) : gint64;
  extern proc garrow_output_stream_write_record_batch(stream : c_ptr(GArrowOutputStream), record_batch : c_ptr(GArrowRecordBatch), options : c_ptr(GArrowWriteOptions), error : c_ptr(c_ptr(GError))) : gint64;
  extern proc garrow_file_output_stream_get_type() : GType;
  extern proc garrow_file_output_stream_new(ref path : gchar, append : gboolean, ref error : c_ptr(GError)) : c_ptr(GArrowFileOutputStream);
  extern proc garrow_file_output_stream_new(path : c_ptr(gchar), append : gboolean, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowFileOutputStream);
  extern proc garrow_buffer_output_stream_get_type() : GType;
  extern proc garrow_buffer_output_stream_new(ref buffer : GArrowResizableBuffer) : c_ptr(GArrowBufferOutputStream);
  extern proc garrow_buffer_output_stream_new(buffer : c_ptr(GArrowResizableBuffer)) : c_ptr(GArrowBufferOutputStream);
  extern proc garrow_gio_output_stream_get_type() : GType;
  extern proc garrow_gio_output_stream_new(ref gio_output_stream : GOutputStream) : c_ptr(GArrowGIOOutputStream);
  extern proc garrow_gio_output_stream_new(gio_output_stream : c_ptr(GOutputStream)) : c_ptr(GArrowGIOOutputStream);
  extern proc garrow_gio_output_stream_get_raw(ref output_stream : GArrowGIOOutputStream) : c_ptr(GOutputStream);
  extern proc garrow_gio_output_stream_get_raw(output_stream : c_ptr(GArrowGIOOutputStream)) : c_ptr(GOutputStream);
  extern proc garrow_compressed_output_stream_get_type() : GType;
  extern proc GARROW_COMPRESSED_OUTPUT_STREAM(ptr : gpointer) : c_ptr(GArrowCompressedOutputStream);
  extern proc GARROW_COMPRESSED_OUTPUT_STREAM_CLASS(ptr : gpointer) : c_ptr(GArrowCompressedOutputStreamClass);
  extern proc GARROW_IS_COMPRESSED_OUTPUT_STREAM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_COMPRESSED_OUTPUT_STREAM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_COMPRESSED_OUTPUT_STREAM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowCompressedOutputStreamClass);
  extern proc garrow_compressed_output_stream_new(ref codec : GArrowCodec, ref raw : GArrowOutputStream, ref error : c_ptr(GError)) : c_ptr(GArrowCompressedOutputStream);
  extern proc garrow_compressed_output_stream_new(codec : c_ptr(GArrowCodec), raw : c_ptr(GArrowOutputStream), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowCompressedOutputStream);
  extern proc garrow_table_get_type() : GType;
  extern proc GARROW_TABLE(ptr : gpointer) : c_ptr(GArrowTable);
  extern proc GARROW_TABLE_CLASS(ptr : gpointer) : c_ptr(GArrowTableClass);
  extern proc GARROW_IS_TABLE(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TABLE_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TABLE_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTableClass);
  extern proc garrow_table_new_values(ref schema : GArrowSchema, ref values : GList, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_table_new_values(schema : c_ptr(GArrowSchema), values : c_ptr(GList), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_table_new_chunked_arrays(ref schema : GArrowSchema, ref chunked_arrays : c_ptr(GArrowChunkedArray), n_chunked_arrays : gsize, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_table_new_chunked_arrays(schema : c_ptr(GArrowSchema), chunked_arrays : c_ptr(c_ptr(GArrowChunkedArray)), n_chunked_arrays : gsize, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_table_new_arrays(ref schema : GArrowSchema, ref arrays : c_ptr(GArrowArray), n_arrays : gsize, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_table_new_arrays(schema : c_ptr(GArrowSchema), arrays : c_ptr(c_ptr(GArrowArray)), n_arrays : gsize, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_table_new_record_batches(ref schema : GArrowSchema, ref record_batches : c_ptr(GArrowRecordBatch), n_record_batches : gsize, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_table_new_record_batches(schema : c_ptr(GArrowSchema), record_batches : c_ptr(c_ptr(GArrowRecordBatch)), n_record_batches : gsize, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_table_equal(ref table : GArrowTable, ref other_table : GArrowTable) : gboolean;
  extern proc garrow_table_equal(table : c_ptr(GArrowTable), other_table : c_ptr(GArrowTable)) : gboolean;
  extern proc garrow_table_equal_metadata(ref table : GArrowTable, ref other_table : GArrowTable, check_metadata : gboolean) : gboolean;
  extern proc garrow_table_equal_metadata(table : c_ptr(GArrowTable), other_table : c_ptr(GArrowTable), check_metadata : gboolean) : gboolean;
  extern proc garrow_table_get_schema(ref table : GArrowTable) : c_ptr(GArrowSchema);
  extern proc garrow_table_get_schema(table : c_ptr(GArrowTable)) : c_ptr(GArrowSchema);
  extern proc garrow_table_get_column_data(ref table : GArrowTable, i : gint) : c_ptr(GArrowChunkedArray);
  extern proc garrow_table_get_column_data(table : c_ptr(GArrowTable), i : gint) : c_ptr(GArrowChunkedArray);
  extern proc garrow_table_get_n_columns(ref table : GArrowTable) : guint;
  extern proc garrow_table_get_n_columns(table : c_ptr(GArrowTable)) : guint;
  extern proc garrow_table_get_n_rows(ref table : GArrowTable) : guint64;
  extern proc garrow_table_get_n_rows(table : c_ptr(GArrowTable)) : guint64;
  extern proc garrow_table_add_column(ref table : GArrowTable, i : guint, ref field : GArrowField, ref chunked_array : GArrowChunkedArray, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_table_add_column(table : c_ptr(GArrowTable), i : guint, field : c_ptr(GArrowField), chunked_array : c_ptr(GArrowChunkedArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_table_remove_column(ref table : GArrowTable, i : guint, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_table_remove_column(table : c_ptr(GArrowTable), i : guint, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_table_replace_column(ref table : GArrowTable, i : guint, ref field : GArrowField, ref chunked_array : GArrowChunkedArray, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_table_replace_column(table : c_ptr(GArrowTable), i : guint, field : c_ptr(GArrowField), chunked_array : c_ptr(GArrowChunkedArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_table_to_string(ref table : GArrowTable, ref error : c_ptr(GError)) : c_ptr(gchar);
  extern proc garrow_table_to_string(table : c_ptr(GArrowTable), error : c_ptr(c_ptr(GError))) : c_ptr(gchar);
  extern proc garrow_table_concatenate(ref table : GArrowTable, ref other_tables : GList, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_table_concatenate(table : c_ptr(GArrowTable), other_tables : c_ptr(GList), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_table_slice(ref table : GArrowTable, offset : gint64, length : gint64) : c_ptr(GArrowTable);
  extern proc garrow_table_slice(table : c_ptr(GArrowTable), offset : gint64, length : gint64) : c_ptr(GArrowTable);
  extern proc garrow_table_combine_chunks(ref table : GArrowTable, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_table_combine_chunks(table : c_ptr(GArrowTable), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_feather_write_properties_get_type() : GType;
  extern proc GARROW_FEATHER_WRITE_PROPERTIES(ptr : gpointer) : c_ptr(GArrowFeatherWriteProperties);
  extern proc GARROW_FEATHER_WRITE_PROPERTIES_CLASS(ptr : gpointer) : c_ptr(GArrowFeatherWritePropertiesClass);
  extern proc GARROW_IS_FEATHER_WRITE_PROPERTIES(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FEATHER_WRITE_PROPERTIES_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FEATHER_WRITE_PROPERTIES_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFeatherWritePropertiesClass);
  extern proc garrow_feather_write_properties_new() : c_ptr(GArrowFeatherWriteProperties);
  extern proc garrow_table_write_as_feather(ref table : GArrowTable, ref sink : GArrowOutputStream, ref properties : GArrowFeatherWriteProperties, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_table_write_as_feather(table : c_ptr(GArrowTable), sink : c_ptr(GArrowOutputStream), properties : c_ptr(GArrowFeatherWriteProperties), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_datum_get_type() : GType;
  extern proc GARROW_DATUM(ptr : gpointer) : c_ptr(GArrowDatum);
  extern proc GARROW_DATUM_CLASS(ptr : gpointer) : c_ptr(GArrowDatumClass);
  extern proc GARROW_IS_DATUM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_DATUM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_DATUM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowDatumClass);
  extern proc garrow_datum_is_array(ref datum : GArrowDatum) : gboolean;
  extern proc garrow_datum_is_array(datum : c_ptr(GArrowDatum)) : gboolean;
  extern proc garrow_datum_is_array_like(ref datum : GArrowDatum) : gboolean;
  extern proc garrow_datum_is_array_like(datum : c_ptr(GArrowDatum)) : gboolean;
  extern proc garrow_datum_is_scalar(ref datum : GArrowDatum) : gboolean;
  extern proc garrow_datum_is_scalar(datum : c_ptr(GArrowDatum)) : gboolean;
  extern proc garrow_datum_is_value(ref datum : GArrowDatum) : gboolean;
  extern proc garrow_datum_is_value(datum : c_ptr(GArrowDatum)) : gboolean;
  extern proc garrow_datum_equal(ref datum : GArrowDatum, ref other_datum : GArrowDatum) : gboolean;
  extern proc garrow_datum_equal(datum : c_ptr(GArrowDatum), other_datum : c_ptr(GArrowDatum)) : gboolean;
  extern proc garrow_datum_to_string(ref datum : GArrowDatum) : c_ptr(gchar);
  extern proc garrow_datum_to_string(datum : c_ptr(GArrowDatum)) : c_ptr(gchar);
  extern proc garrow_scalar_datum_get_type() : GType;
  extern proc GARROW_SCALAR_DATUM(ptr : gpointer) : c_ptr(GArrowScalarDatum);
  extern proc GARROW_SCALAR_DATUM_CLASS(ptr : gpointer) : c_ptr(GArrowScalarDatumClass);
  extern proc GARROW_IS_SCALAR_DATUM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_SCALAR_DATUM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_SCALAR_DATUM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowScalarDatumClass);
  extern proc garrow_scalar_datum_new(ref value : GArrowScalar) : c_ptr(GArrowScalarDatum);
  extern proc garrow_scalar_datum_new(value : c_ptr(GArrowScalar)) : c_ptr(GArrowScalarDatum);
  extern proc garrow_array_datum_get_type() : GType;
  extern proc GARROW_ARRAY_DATUM(ptr : gpointer) : c_ptr(GArrowArrayDatum);
  extern proc GARROW_ARRAY_DATUM_CLASS(ptr : gpointer) : c_ptr(GArrowArrayDatumClass);
  extern proc GARROW_IS_ARRAY_DATUM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_ARRAY_DATUM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_ARRAY_DATUM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowArrayDatumClass);
  extern proc garrow_array_datum_new(ref value : GArrowArray) : c_ptr(GArrowArrayDatum);
  extern proc garrow_array_datum_new(value : c_ptr(GArrowArray)) : c_ptr(GArrowArrayDatum);
  extern proc garrow_chunked_array_datum_get_type() : GType;
  extern proc GARROW_CHUNKED_ARRAY_DATUM(ptr : gpointer) : c_ptr(GArrowChunkedArrayDatum);
  extern proc GARROW_CHUNKED_ARRAY_DATUM_CLASS(ptr : gpointer) : c_ptr(GArrowChunkedArrayDatumClass);
  extern proc GARROW_IS_CHUNKED_ARRAY_DATUM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_CHUNKED_ARRAY_DATUM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_CHUNKED_ARRAY_DATUM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowChunkedArrayDatumClass);
  extern proc garrow_chunked_array_datum_new(ref value : GArrowChunkedArray) : c_ptr(GArrowChunkedArrayDatum);
  extern proc garrow_chunked_array_datum_new(value : c_ptr(GArrowChunkedArray)) : c_ptr(GArrowChunkedArrayDatum);
  extern proc garrow_record_batch_datum_get_type() : GType;
  extern proc GARROW_RECORD_BATCH_DATUM(ptr : gpointer) : c_ptr(GArrowRecordBatchDatum);
  extern proc GARROW_RECORD_BATCH_DATUM_CLASS(ptr : gpointer) : c_ptr(GArrowRecordBatchDatumClass);
  extern proc GARROW_IS_RECORD_BATCH_DATUM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_RECORD_BATCH_DATUM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_RECORD_BATCH_DATUM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowRecordBatchDatumClass);
  extern proc garrow_record_batch_datum_new(ref value : GArrowRecordBatch) : c_ptr(GArrowRecordBatchDatum);
  extern proc garrow_record_batch_datum_new(value : c_ptr(GArrowRecordBatch)) : c_ptr(GArrowRecordBatchDatum);
  extern proc garrow_table_datum_get_type() : GType;
  extern proc GARROW_TABLE_DATUM(ptr : gpointer) : c_ptr(GArrowTableDatum);
  extern proc GARROW_TABLE_DATUM_CLASS(ptr : gpointer) : c_ptr(GArrowTableDatumClass);
  extern proc GARROW_IS_TABLE_DATUM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TABLE_DATUM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TABLE_DATUM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTableDatumClass);
  extern proc garrow_table_datum_new(ref value : GArrowTable) : c_ptr(GArrowTableDatum);
  extern proc garrow_table_datum_new(value : c_ptr(GArrowTable)) : c_ptr(GArrowTableDatum);
  extern proc garrow_execute_context_get_type() : GType;
  extern proc GARROW_EXECUTE_CONTEXT(ptr : gpointer) : c_ptr(GArrowExecuteContext);
  extern proc GARROW_EXECUTE_CONTEXT_CLASS(ptr : gpointer) : c_ptr(GArrowExecuteContextClass);
  extern proc GARROW_IS_EXECUTE_CONTEXT(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_EXECUTE_CONTEXT_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_EXECUTE_CONTEXT_GET_CLASS(ptr : gpointer) : c_ptr(GArrowExecuteContextClass);
  extern proc garrow_execute_context_new() : c_ptr(GArrowExecuteContext);
  extern proc garrow_function_options_get_type() : GType;
  extern proc GARROW_FUNCTION_OPTIONS(ptr : gpointer) : c_ptr(GArrowFunctionOptions);
  extern proc GARROW_IS_FUNCTION_OPTIONS(ptr : gpointer) : gboolean;
  extern proc GARROW_FUNCTION_OPTIONS_GET_IFACE(ptr : gpointer) : c_ptr(GArrowFunctionOptionsInterface);
  extern proc garrow_function_get_type() : GType;
  extern proc GARROW_FUNCTION(ptr : gpointer) : c_ptr(GArrowFunction);
  extern proc GARROW_FUNCTION_CLASS(ptr : gpointer) : c_ptr(GArrowFunctionClass);
  extern proc GARROW_IS_FUNCTION(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FUNCTION_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FUNCTION_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFunctionClass);
  extern proc garrow_function_find(ref name : gchar) : c_ptr(GArrowFunction);
  extern proc garrow_function_find(name : c_ptr(gchar)) : c_ptr(GArrowFunction);
  extern proc garrow_function_execute(ref function : GArrowFunction, ref args : GList, ref options : GArrowFunctionOptions, ref context : GArrowExecuteContext, ref error : c_ptr(GError)) : c_ptr(GArrowDatum);
  extern proc garrow_function_execute(function : c_ptr(GArrowFunction), args : c_ptr(GList), options : c_ptr(GArrowFunctionOptions), context : c_ptr(GArrowExecuteContext), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowDatum);
  extern proc garrow_cast_options_get_type() : GType;
  extern proc GARROW_CAST_OPTIONS(ptr : gpointer) : c_ptr(GArrowCastOptions);
  extern proc GARROW_CAST_OPTIONS_CLASS(ptr : gpointer) : c_ptr(GArrowCastOptionsClass);
  extern proc GARROW_IS_CAST_OPTIONS(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_CAST_OPTIONS_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_CAST_OPTIONS_GET_CLASS(ptr : gpointer) : c_ptr(GArrowCastOptionsClass);
  extern proc garrow_cast_options_new() : c_ptr(GArrowCastOptions);
  extern proc garrow_scalar_aggregate_options_get_type() : GType;
  extern proc GARROW_SCALAR_AGGREGATE_OPTIONS(ptr : gpointer) : c_ptr(GArrowScalarAggregateOptions);
  extern proc GARROW_SCALAR_AGGREGATE_OPTIONS_CLASS(ptr : gpointer) : c_ptr(GArrowScalarAggregateOptionsClass);
  extern proc GARROW_IS_SCALAR_AGGREGATE_OPTIONS(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_SCALAR_AGGREGATE_OPTIONS_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_SCALAR_AGGREGATE_OPTIONS_GET_CLASS(ptr : gpointer) : c_ptr(GArrowScalarAggregateOptionsClass);
  extern proc garrow_scalar_aggregate_options_new() : c_ptr(GArrowScalarAggregateOptions);
  extern proc garrow_filter_options_get_type() : GType;
  extern proc GARROW_FILTER_OPTIONS(ptr : gpointer) : c_ptr(GArrowFilterOptions);
  extern proc GARROW_FILTER_OPTIONS_CLASS(ptr : gpointer) : c_ptr(GArrowFilterOptionsClass);
  extern proc GARROW_IS_FILTER_OPTIONS(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FILTER_OPTIONS_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FILTER_OPTIONS_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFilterOptionsClass);
  extern proc garrow_filter_options_new() : c_ptr(GArrowFilterOptions);
  extern proc garrow_take_options_get_type() : GType;
  extern proc GARROW_TAKE_OPTIONS(ptr : gpointer) : c_ptr(GArrowTakeOptions);
  extern proc GARROW_TAKE_OPTIONS_CLASS(ptr : gpointer) : c_ptr(GArrowTakeOptionsClass);
  extern proc GARROW_IS_TAKE_OPTIONS(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TAKE_OPTIONS_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TAKE_OPTIONS_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTakeOptionsClass);
  extern proc garrow_take_options_new() : c_ptr(GArrowTakeOptions);
  extern proc garrow_array_sort_options_get_type() : GType;
  extern proc GARROW_ARRAY_SORT_OPTIONS(ptr : gpointer) : c_ptr(GArrowArraySortOptions);
  extern proc GARROW_ARRAY_SORT_OPTIONS_CLASS(ptr : gpointer) : c_ptr(GArrowArraySortOptionsClass);
  extern proc GARROW_IS_ARRAY_SORT_OPTIONS(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_ARRAY_SORT_OPTIONS_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_ARRAY_SORT_OPTIONS_GET_CLASS(ptr : gpointer) : c_ptr(GArrowArraySortOptionsClass);
  extern proc garrow_array_sort_options_new(order : GArrowSortOrder) : c_ptr(GArrowArraySortOptions);
  extern proc garrow_array_sort_options_equal(ref options : GArrowArraySortOptions, ref other_options : GArrowArraySortOptions) : gboolean;
  extern proc garrow_array_sort_options_equal(options : c_ptr(GArrowArraySortOptions), other_options : c_ptr(GArrowArraySortOptions)) : gboolean;
  extern proc garrow_sort_key_get_type() : GType;
  extern proc GARROW_SORT_KEY(ptr : gpointer) : c_ptr(GArrowSortKey);
  extern proc GARROW_SORT_KEY_CLASS(ptr : gpointer) : c_ptr(GArrowSortKeyClass);
  extern proc GARROW_IS_SORT_KEY(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_SORT_KEY_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_SORT_KEY_GET_CLASS(ptr : gpointer) : c_ptr(GArrowSortKeyClass);
  extern proc garrow_sort_key_new(ref name : gchar, order : GArrowSortOrder) : c_ptr(GArrowSortKey);
  extern proc garrow_sort_key_new(name : c_ptr(gchar), order : GArrowSortOrder) : c_ptr(GArrowSortKey);
  extern proc garrow_sort_key_equal(ref sort_key : GArrowSortKey, ref other_sort_key : GArrowSortKey) : gboolean;
  extern proc garrow_sort_key_equal(sort_key : c_ptr(GArrowSortKey), other_sort_key : c_ptr(GArrowSortKey)) : gboolean;
  extern proc garrow_sort_options_get_type() : GType;
  extern proc GARROW_SORT_OPTIONS(ptr : gpointer) : c_ptr(GArrowSortOptions);
  extern proc GARROW_SORT_OPTIONS_CLASS(ptr : gpointer) : c_ptr(GArrowSortOptionsClass);
  extern proc GARROW_IS_SORT_OPTIONS(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_SORT_OPTIONS_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_SORT_OPTIONS_GET_CLASS(ptr : gpointer) : c_ptr(GArrowSortOptionsClass);
  extern proc garrow_sort_options_new(ref sort_keys : GList) : c_ptr(GArrowSortOptions);
  extern proc garrow_sort_options_new(sort_keys : c_ptr(GList)) : c_ptr(GArrowSortOptions);
  extern proc garrow_sort_options_equal(ref options : GArrowSortOptions, ref other_options : GArrowSortOptions) : gboolean;
  extern proc garrow_sort_options_equal(options : c_ptr(GArrowSortOptions), other_options : c_ptr(GArrowSortOptions)) : gboolean;
  extern proc garrow_sort_options_get_sort_keys(ref options : GArrowSortOptions) : c_ptr(GList);
  extern proc garrow_sort_options_get_sort_keys(options : c_ptr(GArrowSortOptions)) : c_ptr(GList);
  extern proc garrow_sort_options_set_sort_keys(ref options : GArrowSortOptions, ref sort_keys : GList) : void;
  extern proc garrow_sort_options_set_sort_keys(options : c_ptr(GArrowSortOptions), sort_keys : c_ptr(GList)) : void;
  extern proc garrow_sort_options_add_sort_key(ref options : GArrowSortOptions, ref sort_key : GArrowSortKey) : void;
  extern proc garrow_sort_options_add_sort_key(options : c_ptr(GArrowSortOptions), sort_key : c_ptr(GArrowSortKey)) : void;
  extern proc garrow_array_cast(ref array : GArrowArray, ref target_data_type : GArrowDataType, ref options : GArrowCastOptions, ref error : c_ptr(GError)) : c_ptr(GArrowArray);
  extern proc garrow_array_cast(array : c_ptr(GArrowArray), target_data_type : c_ptr(GArrowDataType), options : c_ptr(GArrowCastOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowArray);
  extern proc garrow_array_unique(ref array : GArrowArray, ref error : c_ptr(GError)) : c_ptr(GArrowArray);
  extern proc garrow_array_unique(array : c_ptr(GArrowArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowArray);
  extern proc garrow_array_dictionary_encode(ref array : GArrowArray, ref error : c_ptr(GError)) : c_ptr(GArrowDictionaryArray);
  extern proc garrow_array_dictionary_encode(array : c_ptr(GArrowArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowDictionaryArray);
  extern proc garrow_array_count(ref array : GArrowArray, ref options : GArrowScalarAggregateOptions, ref error : c_ptr(GError)) : gint64;
  extern proc garrow_array_count(array : c_ptr(GArrowArray), options : c_ptr(GArrowScalarAggregateOptions), error : c_ptr(c_ptr(GError))) : gint64;
  extern proc garrow_array_count_values(ref array : GArrowArray, ref error : c_ptr(GError)) : c_ptr(GArrowStructArray);
  extern proc garrow_array_count_values(array : c_ptr(GArrowArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowStructArray);
  extern proc garrow_boolean_array_invert(ref array : GArrowBooleanArray, ref error : c_ptr(GError)) : c_ptr(GArrowBooleanArray);
  extern proc garrow_boolean_array_invert(array : c_ptr(GArrowBooleanArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowBooleanArray);
  extern proc garrow_boolean_array_and(ref left : GArrowBooleanArray, ref right : GArrowBooleanArray, ref error : c_ptr(GError)) : c_ptr(GArrowBooleanArray);
  extern proc garrow_boolean_array_and(left : c_ptr(GArrowBooleanArray), right : c_ptr(GArrowBooleanArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowBooleanArray);
  extern proc garrow_boolean_array_or(ref left : GArrowBooleanArray, ref right : GArrowBooleanArray, ref error : c_ptr(GError)) : c_ptr(GArrowBooleanArray);
  extern proc garrow_boolean_array_or(left : c_ptr(GArrowBooleanArray), right : c_ptr(GArrowBooleanArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowBooleanArray);
  extern proc garrow_boolean_array_xor(ref left : GArrowBooleanArray, ref right : GArrowBooleanArray, ref error : c_ptr(GError)) : c_ptr(GArrowBooleanArray);
  extern proc garrow_boolean_array_xor(left : c_ptr(GArrowBooleanArray), right : c_ptr(GArrowBooleanArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowBooleanArray);
  extern proc garrow_numeric_array_mean(ref array : GArrowNumericArray, ref error : c_ptr(GError)) : gdouble;
  extern proc garrow_numeric_array_mean(array : c_ptr(GArrowNumericArray), error : c_ptr(c_ptr(GError))) : gdouble;
  extern proc garrow_int8_array_sum(ref array : GArrowInt8Array, ref error : c_ptr(GError)) : gint64;
  extern proc garrow_int8_array_sum(array : c_ptr(GArrowInt8Array), error : c_ptr(c_ptr(GError))) : gint64;
  extern proc garrow_uint8_array_sum(ref array : GArrowUInt8Array, ref error : c_ptr(GError)) : guint64;
  extern proc garrow_uint8_array_sum(array : c_ptr(GArrowUInt8Array), error : c_ptr(c_ptr(GError))) : guint64;
  extern proc garrow_int16_array_sum(ref array : GArrowInt16Array, ref error : c_ptr(GError)) : gint64;
  extern proc garrow_int16_array_sum(array : c_ptr(GArrowInt16Array), error : c_ptr(c_ptr(GError))) : gint64;
  extern proc garrow_uint16_array_sum(ref array : GArrowUInt16Array, ref error : c_ptr(GError)) : guint64;
  extern proc garrow_uint16_array_sum(array : c_ptr(GArrowUInt16Array), error : c_ptr(c_ptr(GError))) : guint64;
  extern proc garrow_int32_array_sum(ref array : GArrowInt32Array, ref error : c_ptr(GError)) : gint64;
  extern proc garrow_int32_array_sum(array : c_ptr(GArrowInt32Array), error : c_ptr(c_ptr(GError))) : gint64;
  extern proc garrow_uint32_array_sum(ref array : GArrowUInt32Array, ref error : c_ptr(GError)) : guint64;
  extern proc garrow_uint32_array_sum(array : c_ptr(GArrowUInt32Array), error : c_ptr(c_ptr(GError))) : guint64;
  extern proc garrow_int64_array_sum(ref array : GArrowInt64Array, ref error : c_ptr(GError)) : gint64;
  extern proc garrow_int64_array_sum(array : c_ptr(GArrowInt64Array), error : c_ptr(c_ptr(GError))) : gint64;
  extern proc garrow_uint64_array_sum(ref array : GArrowUInt64Array, ref error : c_ptr(GError)) : guint64;
  extern proc garrow_uint64_array_sum(array : c_ptr(GArrowUInt64Array), error : c_ptr(c_ptr(GError))) : guint64;
  extern proc garrow_float_array_sum(ref array : GArrowFloatArray, ref error : c_ptr(GError)) : gdouble;
  extern proc garrow_float_array_sum(array : c_ptr(GArrowFloatArray), error : c_ptr(c_ptr(GError))) : gdouble;
  extern proc garrow_double_array_sum(ref array : GArrowDoubleArray, ref error : c_ptr(GError)) : gdouble;
  extern proc garrow_double_array_sum(array : c_ptr(GArrowDoubleArray), error : c_ptr(c_ptr(GError))) : gdouble;
  extern proc garrow_array_take(ref array : GArrowArray, ref indices : GArrowArray, ref options : GArrowTakeOptions, ref error : c_ptr(GError)) : c_ptr(GArrowArray);
  extern proc garrow_array_take(array : c_ptr(GArrowArray), indices : c_ptr(GArrowArray), options : c_ptr(GArrowTakeOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowArray);
  extern proc garrow_array_take_chunked_array(ref array : GArrowArray, ref indices : GArrowChunkedArray, ref options : GArrowTakeOptions, ref error : c_ptr(GError)) : c_ptr(GArrowChunkedArray);
  extern proc garrow_array_take_chunked_array(array : c_ptr(GArrowArray), indices : c_ptr(GArrowChunkedArray), options : c_ptr(GArrowTakeOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowChunkedArray);
  extern proc garrow_table_take(ref table : GArrowTable, ref indices : GArrowArray, ref options : GArrowTakeOptions, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_table_take(table : c_ptr(GArrowTable), indices : c_ptr(GArrowArray), options : c_ptr(GArrowTakeOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_table_take_chunked_array(ref table : GArrowTable, ref indices : GArrowChunkedArray, ref options : GArrowTakeOptions, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_table_take_chunked_array(table : c_ptr(GArrowTable), indices : c_ptr(GArrowChunkedArray), options : c_ptr(GArrowTakeOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_chunked_array_take(ref chunked_array : GArrowChunkedArray, ref indices : GArrowArray, ref options : GArrowTakeOptions, ref error : c_ptr(GError)) : c_ptr(GArrowChunkedArray);
  extern proc garrow_chunked_array_take(chunked_array : c_ptr(GArrowChunkedArray), indices : c_ptr(GArrowArray), options : c_ptr(GArrowTakeOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowChunkedArray);
  extern proc garrow_chunked_array_take_chunked_array(ref chunked_array : GArrowChunkedArray, ref indices : GArrowChunkedArray, ref options : GArrowTakeOptions, ref error : c_ptr(GError)) : c_ptr(GArrowChunkedArray);
  extern proc garrow_chunked_array_take_chunked_array(chunked_array : c_ptr(GArrowChunkedArray), indices : c_ptr(GArrowChunkedArray), options : c_ptr(GArrowTakeOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowChunkedArray);
  extern proc garrow_record_batch_take(ref record_batch : GArrowRecordBatch, ref indices : GArrowArray, ref options : GArrowTakeOptions, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_take(record_batch : c_ptr(GArrowRecordBatch), indices : c_ptr(GArrowArray), options : c_ptr(GArrowTakeOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatch);
  extern proc garrow_array_filter(ref array : GArrowArray, ref filter : GArrowBooleanArray, ref options : GArrowFilterOptions, ref error : c_ptr(GError)) : c_ptr(GArrowArray);
  extern proc garrow_array_filter(array : c_ptr(GArrowArray), filter : c_ptr(GArrowBooleanArray), options : c_ptr(GArrowFilterOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowArray);
  extern proc garrow_array_is_in(ref left : GArrowArray, ref right : GArrowArray, ref error : c_ptr(GError)) : c_ptr(GArrowBooleanArray);
  extern proc garrow_array_is_in(left : c_ptr(GArrowArray), right : c_ptr(GArrowArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowBooleanArray);
  extern proc garrow_array_is_in_chunked_array(ref left : GArrowArray, ref right : GArrowChunkedArray, ref error : c_ptr(GError)) : c_ptr(GArrowBooleanArray);
  extern proc garrow_array_is_in_chunked_array(left : c_ptr(GArrowArray), right : c_ptr(GArrowChunkedArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowBooleanArray);
  extern proc garrow_array_sort_indices(ref array : GArrowArray, order : GArrowSortOrder, ref error : c_ptr(GError)) : c_ptr(GArrowUInt64Array);
  extern proc garrow_array_sort_indices(array : c_ptr(GArrowArray), order : GArrowSortOrder, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowUInt64Array);
  extern proc garrow_array_sort_to_indices(ref array : GArrowArray, ref error : c_ptr(GError)) : c_ptr(GArrowUInt64Array);
  extern proc garrow_array_sort_to_indices(array : c_ptr(GArrowArray), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowUInt64Array);
  extern proc garrow_chunked_array_sort_indices(ref chunked_array : GArrowChunkedArray, order : GArrowSortOrder, ref error : c_ptr(GError)) : c_ptr(GArrowUInt64Array);
  extern proc garrow_chunked_array_sort_indices(chunked_array : c_ptr(GArrowChunkedArray), order : GArrowSortOrder, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowUInt64Array);
  extern proc garrow_record_batch_sort_indices(ref record_batch : GArrowRecordBatch, ref options : GArrowSortOptions, ref error : c_ptr(GError)) : c_ptr(GArrowUInt64Array);
  extern proc garrow_record_batch_sort_indices(record_batch : c_ptr(GArrowRecordBatch), options : c_ptr(GArrowSortOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowUInt64Array);
  extern proc garrow_table_sort_indices(ref table : GArrowTable, ref options : GArrowSortOptions, ref error : c_ptr(GError)) : c_ptr(GArrowUInt64Array);
  extern proc garrow_table_sort_indices(table : c_ptr(GArrowTable), options : c_ptr(GArrowSortOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowUInt64Array);
  extern proc garrow_table_filter(ref table : GArrowTable, ref filter : GArrowBooleanArray, ref options : GArrowFilterOptions, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_table_filter(table : c_ptr(GArrowTable), filter : c_ptr(GArrowBooleanArray), options : c_ptr(GArrowFilterOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_table_filter_chunked_array(ref table : GArrowTable, ref filter : GArrowChunkedArray, ref options : GArrowFilterOptions, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_table_filter_chunked_array(table : c_ptr(GArrowTable), filter : c_ptr(GArrowChunkedArray), options : c_ptr(GArrowFilterOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_chunked_array_filter(ref chunked_array : GArrowChunkedArray, ref filter : GArrowBooleanArray, ref options : GArrowFilterOptions, ref error : c_ptr(GError)) : c_ptr(GArrowChunkedArray);
  extern proc garrow_chunked_array_filter(chunked_array : c_ptr(GArrowChunkedArray), filter : c_ptr(GArrowBooleanArray), options : c_ptr(GArrowFilterOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowChunkedArray);
  extern proc garrow_chunked_array_filter_chunked_array(ref chunked_array : GArrowChunkedArray, ref filter : GArrowChunkedArray, ref options : GArrowFilterOptions, ref error : c_ptr(GError)) : c_ptr(GArrowChunkedArray);
  extern proc garrow_chunked_array_filter_chunked_array(chunked_array : c_ptr(GArrowChunkedArray), filter : c_ptr(GArrowChunkedArray), options : c_ptr(GArrowFilterOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowChunkedArray);
  extern proc garrow_record_batch_filter(ref record_batch : GArrowRecordBatch, ref filter : GArrowBooleanArray, ref options : GArrowFilterOptions, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_filter(record_batch : c_ptr(GArrowRecordBatch), filter : c_ptr(GArrowBooleanArray), options : c_ptr(GArrowFilterOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatch);
  extern proc ggandiva_result_nullable_type_get_type() : GType;
  extern proc ggandiva_selection_vector_mode_get_type() : GType;
  extern proc garrow_error_quark() : GQuark;
  extern proc garrow_record_batch_builder_get_type() : GType;
  extern proc GARROW_RECORD_BATCH_BUILDER(ptr : gpointer) : c_ptr(GArrowRecordBatchBuilder);
  extern proc GARROW_RECORD_BATCH_BUILDER_CLASS(ptr : gpointer) : c_ptr(GArrowRecordBatchBuilderClass);
  extern proc GARROW_IS_RECORD_BATCH_BUILDER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_RECORD_BATCH_BUILDER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_RECORD_BATCH_BUILDER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowRecordBatchBuilderClass);
  extern proc garrow_record_batch_builder_new(ref schema : GArrowSchema, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatchBuilder);
  extern proc garrow_record_batch_builder_new(schema : c_ptr(GArrowSchema), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatchBuilder);
  extern proc garrow_record_batch_builder_get_initial_capacity(ref builder : GArrowRecordBatchBuilder) : gint64;
  extern proc garrow_record_batch_builder_get_initial_capacity(builder : c_ptr(GArrowRecordBatchBuilder)) : gint64;
  extern proc garrow_record_batch_builder_set_initial_capacity(ref builder : GArrowRecordBatchBuilder, capacity : gint64) : void;
  extern proc garrow_record_batch_builder_set_initial_capacity(builder : c_ptr(GArrowRecordBatchBuilder), capacity : gint64) : void;
  extern proc garrow_record_batch_builder_get_schema(ref builder : GArrowRecordBatchBuilder) : c_ptr(GArrowSchema);
  extern proc garrow_record_batch_builder_get_schema(builder : c_ptr(GArrowRecordBatchBuilder)) : c_ptr(GArrowSchema);
  extern proc garrow_record_batch_builder_get_n_fields(ref builder : GArrowRecordBatchBuilder) : gint;
  extern proc garrow_record_batch_builder_get_n_fields(builder : c_ptr(GArrowRecordBatchBuilder)) : gint;
  extern proc garrow_record_batch_builder_get_n_columns(ref builder : GArrowRecordBatchBuilder) : gint;
  extern proc garrow_record_batch_builder_get_n_columns(builder : c_ptr(GArrowRecordBatchBuilder)) : gint;
  extern proc garrow_record_batch_builder_get_field(ref builder : GArrowRecordBatchBuilder, i : gint) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_record_batch_builder_get_field(builder : c_ptr(GArrowRecordBatchBuilder), i : gint) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_record_batch_builder_get_column_builder(ref builder : GArrowRecordBatchBuilder, i : gint) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_record_batch_builder_get_column_builder(builder : c_ptr(GArrowRecordBatchBuilder), i : gint) : c_ptr(GArrowArrayBuilder);
  extern proc garrow_record_batch_builder_flush(ref builder : GArrowRecordBatchBuilder, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_builder_flush(builder : c_ptr(GArrowRecordBatchBuilder), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatch);
  extern proc garrow_file_get_type() : GType;
  extern proc GARROW_FILE(ptr : gpointer) : c_ptr(GArrowFile);
  extern proc GARROW_IS_FILE(ptr : gpointer) : gboolean;
  extern proc GARROW_FILE_GET_IFACE(ptr : gpointer) : c_ptr(GArrowFileInterface);
  extern proc garrow_file_close(ref file : GArrowFile, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_file_close(file : c_ptr(GArrowFile), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_file_is_closed(ref file : GArrowFile) : gboolean;
  extern proc garrow_file_is_closed(file : c_ptr(GArrowFile)) : gboolean;
  extern proc garrow_file_tell(ref file : GArrowFile, ref error : c_ptr(GError)) : gint64;
  extern proc garrow_file_tell(file : c_ptr(GArrowFile), error : c_ptr(c_ptr(GError))) : gint64;
  extern proc garrow_file_get_mode(ref file : GArrowFile) : GArrowFileMode;
  extern proc garrow_file_get_mode(file : c_ptr(GArrowFile)) : GArrowFileMode;
  extern proc garrow_input_stream_get_type() : GType;
  extern proc GARROW_INPUT_STREAM(ptr : gpointer) : c_ptr(GArrowInputStream);
  extern proc GARROW_INPUT_STREAM_CLASS(ptr : gpointer) : c_ptr(GArrowInputStreamClass);
  extern proc GARROW_IS_INPUT_STREAM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_INPUT_STREAM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_INPUT_STREAM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowInputStreamClass);
  extern proc garrow_input_stream_advance(ref input_stream : GArrowInputStream, n_bytes : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_input_stream_advance(input_stream : c_ptr(GArrowInputStream), n_bytes : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_input_stream_align(ref input_stream : GArrowInputStream, alignment : gint32, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_input_stream_align(input_stream : c_ptr(GArrowInputStream), alignment : gint32, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_input_stream_read_tensor(ref input_stream : GArrowInputStream, ref error : c_ptr(GError)) : c_ptr(GArrowTensor);
  extern proc garrow_input_stream_read_tensor(input_stream : c_ptr(GArrowInputStream), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTensor);
  extern proc garrow_input_stream_read_record_batch(ref input_stream : GArrowInputStream, ref schema : GArrowSchema, ref options : GArrowReadOptions, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatch);
  extern proc garrow_input_stream_read_record_batch(input_stream : c_ptr(GArrowInputStream), schema : c_ptr(GArrowSchema), options : c_ptr(GArrowReadOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatch);
  extern proc garrow_seekable_input_stream_get_type() : GType;
  extern proc GARROW_SEEKABLE_INPUT_STREAM(ptr : gpointer) : c_ptr(GArrowSeekableInputStream);
  extern proc GARROW_SEEKABLE_INPUT_STREAM_CLASS(ptr : gpointer) : c_ptr(GArrowSeekableInputStreamClass);
  extern proc GARROW_IS_SEEKABLE_INPUT_STREAM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_SEEKABLE_INPUT_STREAM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_SEEKABLE_INPUT_STREAM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowSeekableInputStreamClass);
  extern proc garrow_seekable_input_stream_get_size(ref input_stream : GArrowSeekableInputStream, ref error : c_ptr(GError)) : guint64;
  extern proc garrow_seekable_input_stream_get_size(input_stream : c_ptr(GArrowSeekableInputStream), error : c_ptr(c_ptr(GError))) : guint64;
  extern proc garrow_seekable_input_stream_get_support_zero_copy(ref input_stream : GArrowSeekableInputStream) : gboolean;
  extern proc garrow_seekable_input_stream_get_support_zero_copy(input_stream : c_ptr(GArrowSeekableInputStream)) : gboolean;
  extern proc garrow_seekable_input_stream_read_at(ref input_stream : GArrowSeekableInputStream, position : gint64, n_bytes : gint64, ref error : c_ptr(GError)) : c_ptr(GArrowBuffer);
  extern proc garrow_seekable_input_stream_read_at(input_stream : c_ptr(GArrowSeekableInputStream), position : gint64, n_bytes : gint64, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowBuffer);
  extern proc garrow_seekable_input_stream_read_at_bytes(ref input_stream : GArrowSeekableInputStream, position : gint64, n_bytes : gint64, ref error : c_ptr(GError)) : c_ptr(GBytes);
  extern proc garrow_seekable_input_stream_read_at_bytes(input_stream : c_ptr(GArrowSeekableInputStream), position : gint64, n_bytes : gint64, error : c_ptr(c_ptr(GError))) : c_ptr(GBytes);
  extern proc garrow_seekable_input_stream_peek(ref input_stream : GArrowSeekableInputStream, n_bytes : gint64, ref error : c_ptr(GError)) : c_ptr(GBytes);
  extern proc garrow_seekable_input_stream_peek(input_stream : c_ptr(GArrowSeekableInputStream), n_bytes : gint64, error : c_ptr(c_ptr(GError))) : c_ptr(GBytes);
  extern proc garrow_buffer_input_stream_get_type() : GType;
  extern proc GARROW_BUFFER_INPUT_STREAM(ptr : gpointer) : c_ptr(GArrowBufferInputStream);
  extern proc GARROW_BUFFER_INPUT_STREAM_CLASS(ptr : gpointer) : c_ptr(GArrowBufferInputStreamClass);
  extern proc GARROW_IS_BUFFER_INPUT_STREAM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_BUFFER_INPUT_STREAM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_BUFFER_INPUT_STREAM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowBufferInputStreamClass);
  extern proc garrow_buffer_input_stream_new(ref buffer : GArrowBuffer) : c_ptr(GArrowBufferInputStream);
  extern proc garrow_buffer_input_stream_new(buffer : c_ptr(GArrowBuffer)) : c_ptr(GArrowBufferInputStream);
  extern proc garrow_buffer_input_stream_get_buffer(ref input_stream : GArrowBufferInputStream) : c_ptr(GArrowBuffer);
  extern proc garrow_buffer_input_stream_get_buffer(input_stream : c_ptr(GArrowBufferInputStream)) : c_ptr(GArrowBuffer);
  extern proc garrow_memory_mapped_input_stream_get_type() : GType;
  extern proc garrow_memory_mapped_input_stream_new(ref path : gchar, ref error : c_ptr(GError)) : c_ptr(GArrowMemoryMappedInputStream);
  extern proc garrow_memory_mapped_input_stream_new(path : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowMemoryMappedInputStream);
  extern proc garrow_gio_input_stream_get_type() : GType;
  extern proc garrow_gio_input_stream_new(ref gio_input_stream : GInputStream) : c_ptr(GArrowGIOInputStream);
  extern proc garrow_gio_input_stream_new(gio_input_stream : c_ptr(GInputStream)) : c_ptr(GArrowGIOInputStream);
  extern proc garrow_gio_input_stream_get_raw(ref input_stream : GArrowGIOInputStream) : c_ptr(GInputStream);
  extern proc garrow_gio_input_stream_get_raw(input_stream : c_ptr(GArrowGIOInputStream)) : c_ptr(GInputStream);
  extern proc garrow_compressed_input_stream_get_type() : GType;
  extern proc GARROW_COMPRESSED_INPUT_STREAM(ptr : gpointer) : c_ptr(GArrowCompressedInputStream);
  extern proc GARROW_COMPRESSED_INPUT_STREAM_CLASS(ptr : gpointer) : c_ptr(GArrowCompressedInputStreamClass);
  extern proc GARROW_IS_COMPRESSED_INPUT_STREAM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_COMPRESSED_INPUT_STREAM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_COMPRESSED_INPUT_STREAM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowCompressedInputStreamClass);
  extern proc garrow_compressed_input_stream_new(ref codec : GArrowCodec, ref raw : GArrowInputStream, ref error : c_ptr(GError)) : c_ptr(GArrowCompressedInputStream);
  extern proc garrow_compressed_input_stream_new(codec : c_ptr(GArrowCodec), raw : c_ptr(GArrowInputStream), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowCompressedInputStream);
  extern proc garrow_readable_get_type() : GType;
  extern proc GARROW_READABLE(ptr : gpointer) : c_ptr(GArrowReadable);
  extern proc GARROW_IS_READABLE(ptr : gpointer) : gboolean;
  extern proc GARROW_READABLE_GET_IFACE(ptr : gpointer) : c_ptr(GArrowReadableInterface);
  extern proc garrow_readable_read(ref readable : GArrowReadable, n_bytes : gint64, ref error : c_ptr(GError)) : c_ptr(GArrowBuffer);
  extern proc garrow_readable_read(readable : c_ptr(GArrowReadable), n_bytes : gint64, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowBuffer);
  extern proc garrow_readable_read_bytes(ref readable : GArrowReadable, n_bytes : gint64, ref error : c_ptr(GError)) : c_ptr(GBytes);
  extern proc garrow_readable_read_bytes(readable : c_ptr(GArrowReadable), n_bytes : gint64, error : c_ptr(c_ptr(GError))) : c_ptr(GBytes);
  extern proc garrow_writable_get_type() : GType;
  extern proc GARROW_WRITABLE(ptr : gpointer) : c_ptr(GArrowWritable);
  extern proc GARROW_IS_WRITABLE(ptr : gpointer) : gboolean;
  extern proc GARROW_WRITABLE_GET_IFACE(ptr : gpointer) : c_ptr(GArrowWritableInterface);
  extern proc garrow_writable_write(ref writable : GArrowWritable, ref data : guint8, n_bytes : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_writable_write(writable : c_ptr(GArrowWritable), data : c_ptr(guint8), n_bytes : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_writable_flush(ref writable : GArrowWritable, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_writable_flush(writable : c_ptr(GArrowWritable), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_writable_file_get_type() : GType;
  extern proc GARROW_WRITABLE_FILE(ptr : gpointer) : c_ptr(GArrowWritableFile);
  extern proc GARROW_IS_WRITABLE_FILE(ptr : gpointer) : gboolean;
  extern proc GARROW_WRITABLE_FILE_GET_IFACE(ptr : gpointer) : c_ptr(GArrowWritableFileInterface);
  extern proc garrow_writable_file_write_at(ref writable_file : GArrowWritableFile, position : gint64, ref data : guint8, n_bytes : gint64, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_writable_file_write_at(writable_file : c_ptr(GArrowWritableFile), position : gint64, data : c_ptr(guint8), n_bytes : gint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_record_batch_reader_get_type() : GType;
  extern proc GARROW_RECORD_BATCH_READER(ptr : gpointer) : c_ptr(GArrowRecordBatchReader);
  extern proc GARROW_RECORD_BATCH_READER_CLASS(ptr : gpointer) : c_ptr(GArrowRecordBatchReaderClass);
  extern proc GARROW_IS_RECORD_BATCH_READER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_RECORD_BATCH_READER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_RECORD_BATCH_READER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowRecordBatchReaderClass);
  extern proc garrow_record_batch_reader_get_schema(ref reader : GArrowRecordBatchReader) : c_ptr(GArrowSchema);
  extern proc garrow_record_batch_reader_get_schema(reader : c_ptr(GArrowRecordBatchReader)) : c_ptr(GArrowSchema);
  extern proc garrow_record_batch_reader_get_next_record_batch(ref reader : GArrowRecordBatchReader, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_reader_get_next_record_batch(reader : c_ptr(GArrowRecordBatchReader), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_reader_read_next_record_batch(ref reader : GArrowRecordBatchReader, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_reader_read_next_record_batch(reader : c_ptr(GArrowRecordBatchReader), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_reader_read_next(ref reader : GArrowRecordBatchReader, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_reader_read_next(reader : c_ptr(GArrowRecordBatchReader), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatch);
  extern proc garrow_table_batch_reader_get_type() : GType;
  extern proc GARROW_TABLE_BATCH_READER(ptr : gpointer) : c_ptr(GArrowTableBatchReader);
  extern proc GARROW_TABLE_BATCH_READER_CLASS(ptr : gpointer) : c_ptr(GArrowTableBatchReaderClass);
  extern proc GARROW_IS_TABLE_BATCH_READER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_TABLE_BATCH_READER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_TABLE_BATCH_READER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowTableBatchReaderClass);
  extern proc garrow_table_batch_reader_new(ref table : GArrowTable) : c_ptr(GArrowTableBatchReader);
  extern proc garrow_table_batch_reader_new(table : c_ptr(GArrowTable)) : c_ptr(GArrowTableBatchReader);
  extern proc garrow_record_batch_stream_reader_get_type() : GType;
  extern proc garrow_record_batch_stream_reader_new(ref stream : GArrowInputStream, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatchStreamReader);
  extern proc garrow_record_batch_stream_reader_new(stream : c_ptr(GArrowInputStream), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatchStreamReader);
  extern proc garrow_record_batch_file_reader_get_type() : GType;
  extern proc garrow_record_batch_file_reader_new(ref file : GArrowSeekableInputStream, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatchFileReader);
  extern proc garrow_record_batch_file_reader_new(file : c_ptr(GArrowSeekableInputStream), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatchFileReader);
  extern proc garrow_record_batch_file_reader_get_schema(ref reader : GArrowRecordBatchFileReader) : c_ptr(GArrowSchema);
  extern proc garrow_record_batch_file_reader_get_schema(reader : c_ptr(GArrowRecordBatchFileReader)) : c_ptr(GArrowSchema);
  extern proc garrow_record_batch_file_reader_get_n_record_batches(ref reader : GArrowRecordBatchFileReader) : guint;
  extern proc garrow_record_batch_file_reader_get_n_record_batches(reader : c_ptr(GArrowRecordBatchFileReader)) : guint;
  extern proc garrow_record_batch_file_reader_get_version(ref reader : GArrowRecordBatchFileReader) : GArrowMetadataVersion;
  extern proc garrow_record_batch_file_reader_get_version(reader : c_ptr(GArrowRecordBatchFileReader)) : GArrowMetadataVersion;
  extern proc garrow_record_batch_file_reader_get_record_batch(ref reader : GArrowRecordBatchFileReader, i : guint, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_file_reader_get_record_batch(reader : c_ptr(GArrowRecordBatchFileReader), i : guint, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_file_reader_read_record_batch(ref reader : GArrowRecordBatchFileReader, i : guint, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatch);
  extern proc garrow_record_batch_file_reader_read_record_batch(reader : c_ptr(GArrowRecordBatchFileReader), i : guint, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatch);
  extern proc garrow_feather_file_reader_get_type() : GType;
  extern proc GARROW_FEATHER_FILE_READER(ptr : gpointer) : c_ptr(GArrowFeatherFileReader);
  extern proc GARROW_FEATHER_FILE_READER_CLASS(ptr : gpointer) : c_ptr(GArrowFeatherFileReaderClass);
  extern proc GARROW_IS_FEATHER_FILE_READER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FEATHER_FILE_READER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FEATHER_FILE_READER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFeatherFileReaderClass);
  extern proc garrow_feather_file_reader_new(ref file : GArrowSeekableInputStream, ref error : c_ptr(GError)) : c_ptr(GArrowFeatherFileReader);
  extern proc garrow_feather_file_reader_new(file : c_ptr(GArrowSeekableInputStream), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowFeatherFileReader);
  extern proc garrow_feather_file_reader_get_version(ref reader : GArrowFeatherFileReader) : gint;
  extern proc garrow_feather_file_reader_get_version(reader : c_ptr(GArrowFeatherFileReader)) : gint;
  extern proc garrow_feather_file_reader_read(ref reader : GArrowFeatherFileReader, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_feather_file_reader_read(reader : c_ptr(GArrowFeatherFileReader), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_feather_file_reader_read_indices(ref reader : GArrowFeatherFileReader, ref indices : gint, n_indices : guint, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_feather_file_reader_read_indices(reader : c_ptr(GArrowFeatherFileReader), indices : c_ptr(gint), n_indices : guint, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_feather_file_reader_read_names(ref reader : GArrowFeatherFileReader, ref names : c_ptr(gchar), n_names : guint, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_feather_file_reader_read_names(reader : c_ptr(GArrowFeatherFileReader), names : c_ptr(c_ptr(gchar)), n_names : guint, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_csv_read_options_get_type() : GType;
  extern proc GARROW_CSV_READ_OPTIONS(ptr : gpointer) : c_ptr(GArrowCSVReadOptions);
  extern proc GARROW_CSV_READ_OPTIONS_CLASS(ptr : gpointer) : c_ptr(GArrowCSVReadOptionsClass);
  extern proc GARROW_IS_CSV_READ_OPTIONS(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_CSV_READ_OPTIONS_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_CSV_READ_OPTIONS_GET_CLASS(ptr : gpointer) : c_ptr(GArrowCSVReadOptionsClass);
  extern proc garrow_csv_read_options_new() : c_ptr(GArrowCSVReadOptions);
  extern proc garrow_csv_read_options_add_column_type(ref options : GArrowCSVReadOptions, ref name : gchar, ref data_type : GArrowDataType) : void;
  extern proc garrow_csv_read_options_add_column_type(options : c_ptr(GArrowCSVReadOptions), name : c_ptr(gchar), data_type : c_ptr(GArrowDataType)) : void;
  extern proc garrow_csv_read_options_add_schema(ref options : GArrowCSVReadOptions, ref schema : GArrowSchema) : void;
  extern proc garrow_csv_read_options_add_schema(options : c_ptr(GArrowCSVReadOptions), schema : c_ptr(GArrowSchema)) : void;
  extern proc garrow_csv_read_options_get_column_types(ref options : GArrowCSVReadOptions) : c_ptr(GHashTable);
  extern proc garrow_csv_read_options_get_column_types(options : c_ptr(GArrowCSVReadOptions)) : c_ptr(GHashTable);
  extern proc garrow_csv_read_options_set_null_values(ref options : GArrowCSVReadOptions, ref null_values : c_ptr(gchar), n_null_values : gsize) : void;
  extern proc garrow_csv_read_options_set_null_values(options : c_ptr(GArrowCSVReadOptions), null_values : c_ptr(c_ptr(gchar)), n_null_values : gsize) : void;
  extern proc garrow_csv_read_options_get_null_values(ref options : GArrowCSVReadOptions) : c_ptr(c_ptr(gchar));
  extern proc garrow_csv_read_options_get_null_values(options : c_ptr(GArrowCSVReadOptions)) : c_ptr(c_ptr(gchar));
  extern proc garrow_csv_read_options_add_null_value(ref options : GArrowCSVReadOptions, ref null_value : gchar) : void;
  extern proc garrow_csv_read_options_add_null_value(options : c_ptr(GArrowCSVReadOptions), null_value : c_ptr(gchar)) : void;
  extern proc garrow_csv_read_options_set_true_values(ref options : GArrowCSVReadOptions, ref true_values : c_ptr(gchar), n_true_values : gsize) : void;
  extern proc garrow_csv_read_options_set_true_values(options : c_ptr(GArrowCSVReadOptions), true_values : c_ptr(c_ptr(gchar)), n_true_values : gsize) : void;
  extern proc garrow_csv_read_options_get_true_values(ref options : GArrowCSVReadOptions) : c_ptr(c_ptr(gchar));
  extern proc garrow_csv_read_options_get_true_values(options : c_ptr(GArrowCSVReadOptions)) : c_ptr(c_ptr(gchar));
  extern proc garrow_csv_read_options_add_true_value(ref options : GArrowCSVReadOptions, ref true_value : gchar) : void;
  extern proc garrow_csv_read_options_add_true_value(options : c_ptr(GArrowCSVReadOptions), true_value : c_ptr(gchar)) : void;
  extern proc garrow_csv_read_options_set_false_values(ref options : GArrowCSVReadOptions, ref false_values : c_ptr(gchar), n_false_values : gsize) : void;
  extern proc garrow_csv_read_options_set_false_values(options : c_ptr(GArrowCSVReadOptions), false_values : c_ptr(c_ptr(gchar)), n_false_values : gsize) : void;
  extern proc garrow_csv_read_options_get_false_values(ref options : GArrowCSVReadOptions) : c_ptr(c_ptr(gchar));
  extern proc garrow_csv_read_options_get_false_values(options : c_ptr(GArrowCSVReadOptions)) : c_ptr(c_ptr(gchar));
  extern proc garrow_csv_read_options_add_false_value(ref options : GArrowCSVReadOptions, ref false_value : gchar) : void;
  extern proc garrow_csv_read_options_add_false_value(options : c_ptr(GArrowCSVReadOptions), false_value : c_ptr(gchar)) : void;
  extern proc garrow_csv_read_options_set_column_names(ref options : GArrowCSVReadOptions, ref column_names : c_ptr(gchar), n_column_names : gsize) : void;
  extern proc garrow_csv_read_options_set_column_names(options : c_ptr(GArrowCSVReadOptions), column_names : c_ptr(c_ptr(gchar)), n_column_names : gsize) : void;
  extern proc garrow_csv_read_options_get_column_names(ref options : GArrowCSVReadOptions) : c_ptr(c_ptr(gchar));
  extern proc garrow_csv_read_options_get_column_names(options : c_ptr(GArrowCSVReadOptions)) : c_ptr(c_ptr(gchar));
  extern proc garrow_csv_read_options_add_column_name(ref options : GArrowCSVReadOptions, ref column_name : gchar) : void;
  extern proc garrow_csv_read_options_add_column_name(options : c_ptr(GArrowCSVReadOptions), column_name : c_ptr(gchar)) : void;
  extern proc garrow_csv_reader_get_type() : GType;
  extern proc GARROW_CSV_READER(ptr : gpointer) : c_ptr(GArrowCSVReader);
  extern proc GARROW_CSV_READER_CLASS(ptr : gpointer) : c_ptr(GArrowCSVReaderClass);
  extern proc GARROW_IS_CSV_READER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_CSV_READER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_CSV_READER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowCSVReaderClass);
  extern proc garrow_csv_reader_new(ref input : GArrowInputStream, ref options : GArrowCSVReadOptions, ref error : c_ptr(GError)) : c_ptr(GArrowCSVReader);
  extern proc garrow_csv_reader_new(input : c_ptr(GArrowInputStream), options : c_ptr(GArrowCSVReadOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowCSVReader);
  extern proc garrow_csv_reader_read(ref reader : GArrowCSVReader, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_csv_reader_read(reader : c_ptr(GArrowCSVReader), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_json_read_options_get_type() : GType;
  extern proc GARROW_JSON_READ_OPTIONS(ptr : gpointer) : c_ptr(GArrowJSONReadOptions);
  extern proc GARROW_JSON_READ_OPTIONS_CLASS(ptr : gpointer) : c_ptr(GArrowJSONReadOptionsClass);
  extern proc GARROW_IS_JSON_READ_OPTIONS(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_JSON_READ_OPTIONS_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_JSON_READ_OPTIONS_GET_CLASS(ptr : gpointer) : c_ptr(GArrowJSONReadOptionsClass);
  extern proc garrow_json_read_options_new() : c_ptr(GArrowJSONReadOptions);
  extern proc garrow_json_reader_get_type() : GType;
  extern proc GARROW_JSON_READER(ptr : gpointer) : c_ptr(GArrowJSONReader);
  extern proc GARROW_JSON_READER_CLASS(ptr : gpointer) : c_ptr(GArrowJSONReaderClass);
  extern proc GARROW_IS_JSON_READER(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_JSON_READER_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_JSON_READER_GET_CLASS(ptr : gpointer) : c_ptr(GArrowJSONReaderClass);
  extern proc garrow_json_reader_new(ref input : GArrowInputStream, ref options : GArrowJSONReadOptions, ref error : c_ptr(GError)) : c_ptr(GArrowJSONReader);
  extern proc garrow_json_reader_new(input : c_ptr(GArrowInputStream), options : c_ptr(GArrowJSONReadOptions), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowJSONReader);
  extern proc garrow_json_reader_read(ref reader : GArrowJSONReader, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc garrow_json_reader_read(reader : c_ptr(GArrowJSONReader), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc garrow_record_batch_writer_get_type() : GType;
  extern proc garrow_record_batch_writer_write_record_batch(ref writer : GArrowRecordBatchWriter, ref record_batch : GArrowRecordBatch, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_record_batch_writer_write_record_batch(writer : c_ptr(GArrowRecordBatchWriter), record_batch : c_ptr(GArrowRecordBatch), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_record_batch_writer_write_table(ref writer : GArrowRecordBatchWriter, ref table : GArrowTable, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_record_batch_writer_write_table(writer : c_ptr(GArrowRecordBatchWriter), table : c_ptr(GArrowTable), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_record_batch_writer_close(ref writer : GArrowRecordBatchWriter, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_record_batch_writer_close(writer : c_ptr(GArrowRecordBatchWriter), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_record_batch_stream_writer_get_type() : GType;
  extern proc garrow_record_batch_stream_writer_new(ref sink : GArrowOutputStream, ref schema : GArrowSchema, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatchStreamWriter);
  extern proc garrow_record_batch_stream_writer_new(sink : c_ptr(GArrowOutputStream), schema : c_ptr(GArrowSchema), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatchStreamWriter);
  extern proc garrow_record_batch_file_writer_get_type() : GType;
  extern proc garrow_record_batch_file_writer_new(ref sink : GArrowOutputStream, ref schema : GArrowSchema, ref error : c_ptr(GError)) : c_ptr(GArrowRecordBatchFileWriter);
  extern proc garrow_record_batch_file_writer_new(sink : c_ptr(GArrowOutputStream), schema : c_ptr(GArrowSchema), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowRecordBatchFileWriter);
  extern proc garrow_file_info_get_type() : GType;
  extern proc GARROW_FILE_INFO(ptr : gpointer) : c_ptr(GArrowFileInfo);
  extern proc GARROW_FILE_INFO_CLASS(ptr : gpointer) : c_ptr(GArrowFileInfoClass);
  extern proc GARROW_IS_FILE_INFO(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FILE_INFO_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FILE_INFO_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFileInfoClass);
  extern proc garrow_file_info_new() : c_ptr(GArrowFileInfo);
  extern proc garrow_file_info_equal(ref file_info : GArrowFileInfo, ref other_file_info : GArrowFileInfo) : gboolean;
  extern proc garrow_file_info_equal(file_info : c_ptr(GArrowFileInfo), other_file_info : c_ptr(GArrowFileInfo)) : gboolean;
  extern proc garrow_file_info_is_file(ref file_info : GArrowFileInfo) : gboolean;
  extern proc garrow_file_info_is_file(file_info : c_ptr(GArrowFileInfo)) : gboolean;
  extern proc garrow_file_info_is_dir(ref file_info : GArrowFileInfo) : gboolean;
  extern proc garrow_file_info_is_dir(file_info : c_ptr(GArrowFileInfo)) : gboolean;
  extern proc garrow_file_info_to_string(ref file_info : GArrowFileInfo) : c_ptr(gchar);
  extern proc garrow_file_info_to_string(file_info : c_ptr(GArrowFileInfo)) : c_ptr(gchar);
  extern proc garrow_file_selector_get_type() : GType;
  extern proc GARROW_FILE_SELECTOR(ptr : gpointer) : c_ptr(GArrowFileSelector);
  extern proc GARROW_FILE_SELECTOR_CLASS(ptr : gpointer) : c_ptr(GArrowFileSelectorClass);
  extern proc GARROW_IS_FILE_SELECTOR(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FILE_SELECTOR_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FILE_SELECTOR_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFileSelectorClass);
  extern proc garrow_file_system_get_type() : GType;
  extern proc GARROW_FILE_SYSTEM(ptr : gpointer) : c_ptr(GArrowFileSystem);
  extern proc GARROW_FILE_SYSTEM_CLASS(ptr : gpointer) : c_ptr(GArrowFileSystemClass);
  extern proc GARROW_IS_FILE_SYSTEM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_FILE_SYSTEM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_FILE_SYSTEM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowFileSystemClass);
  extern proc garrow_file_system_create(ref uri : gchar, ref error : c_ptr(GError)) : c_ptr(GArrowFileSystem);
  extern proc garrow_file_system_create(uri : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowFileSystem);
  extern proc garrow_file_system_get_type_name(ref file_system : GArrowFileSystem) : c_ptr(gchar);
  extern proc garrow_file_system_get_type_name(file_system : c_ptr(GArrowFileSystem)) : c_ptr(gchar);
  extern proc garrow_file_system_get_file_info(ref file_system : GArrowFileSystem, ref path : gchar, ref error : c_ptr(GError)) : c_ptr(GArrowFileInfo);
  extern proc garrow_file_system_get_file_info(file_system : c_ptr(GArrowFileSystem), path : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowFileInfo);
  extern proc garrow_file_system_get_file_infos_paths(ref file_system : GArrowFileSystem, ref paths : c_ptr(gchar), n_paths : gsize, ref error : c_ptr(GError)) : c_ptr(GList);
  extern proc garrow_file_system_get_file_infos_paths(file_system : c_ptr(GArrowFileSystem), paths : c_ptr(c_ptr(gchar)), n_paths : gsize, error : c_ptr(c_ptr(GError))) : c_ptr(GList);
  extern proc garrow_file_system_get_file_infos_selector(ref file_system : GArrowFileSystem, ref file_selector : GArrowFileSelector, ref error : c_ptr(GError)) : c_ptr(GList);
  extern proc garrow_file_system_get_file_infos_selector(file_system : c_ptr(GArrowFileSystem), file_selector : c_ptr(GArrowFileSelector), error : c_ptr(c_ptr(GError))) : c_ptr(GList);
  extern proc garrow_file_system_create_dir(ref file_system : GArrowFileSystem, ref path : gchar, recursive : gboolean, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_file_system_create_dir(file_system : c_ptr(GArrowFileSystem), path : c_ptr(gchar), recursive : gboolean, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_file_system_delete_dir(ref file_system : GArrowFileSystem, ref path : gchar, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_file_system_delete_dir(file_system : c_ptr(GArrowFileSystem), path : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_file_system_delete_dir_contents(ref file_system : GArrowFileSystem, ref path : gchar, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_file_system_delete_dir_contents(file_system : c_ptr(GArrowFileSystem), path : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_file_system_delete_file(ref file_system : GArrowFileSystem, ref path : gchar, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_file_system_delete_file(file_system : c_ptr(GArrowFileSystem), path : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_file_system_delete_files(ref file_system : GArrowFileSystem, ref paths : c_ptr(gchar), n_paths : gsize, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_file_system_delete_files(file_system : c_ptr(GArrowFileSystem), paths : c_ptr(c_ptr(gchar)), n_paths : gsize, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_file_system_move(ref file_system : GArrowFileSystem, ref src : gchar, ref dest : gchar, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_file_system_move(file_system : c_ptr(GArrowFileSystem), src : c_ptr(gchar), dest : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_file_system_copy_file(ref file_system : GArrowFileSystem, ref src : gchar, ref dest : gchar, ref error : c_ptr(GError)) : gboolean;
  extern proc garrow_file_system_copy_file(file_system : c_ptr(GArrowFileSystem), src : c_ptr(gchar), dest : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc garrow_file_system_open_input_stream(ref file_system : GArrowFileSystem, ref path : gchar, ref error : c_ptr(GError)) : c_ptr(GArrowInputStream);
  extern proc garrow_file_system_open_input_stream(file_system : c_ptr(GArrowFileSystem), path : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowInputStream);
  extern proc garrow_file_system_open_input_file(ref file_system : GArrowFileSystem, ref path : gchar, ref error : c_ptr(GError)) : c_ptr(GArrowSeekableInputStream);
  extern proc garrow_file_system_open_input_file(file_system : c_ptr(GArrowFileSystem), path : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowSeekableInputStream);
  extern proc garrow_file_system_open_output_stream(ref file_system : GArrowFileSystem, ref path : gchar, ref error : c_ptr(GError)) : c_ptr(GArrowOutputStream);
  extern proc garrow_file_system_open_output_stream(file_system : c_ptr(GArrowFileSystem), path : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowOutputStream);
  extern proc garrow_file_system_open_append_stream(ref file_system : GArrowFileSystem, ref path : gchar, ref error : c_ptr(GError)) : c_ptr(GArrowOutputStream);
  extern proc garrow_file_system_open_append_stream(file_system : c_ptr(GArrowFileSystem), path : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowOutputStream);
  extern proc garrow_sub_tree_file_system_get_type() : GType;
  extern proc GARROW_SUB_TREE_FILE_SYSTEM(ptr : gpointer) : c_ptr(GArrowSubTreeFileSystem);
  extern proc GARROW_SUB_TREE_FILE_SYSTEM_CLASS(ptr : gpointer) : c_ptr(GArrowSubTreeFileSystemClass);
  extern proc GARROW_IS_SUB_TREE_FILE_SYSTEM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_SUB_TREE_FILE_SYSTEM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_SUB_TREE_FILE_SYSTEM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowSubTreeFileSystemClass);
  extern proc garrow_sub_tree_file_system_new(ref base_path : gchar, ref base_file_system : GArrowFileSystem) : c_ptr(GArrowSubTreeFileSystem);
  extern proc garrow_sub_tree_file_system_new(base_path : c_ptr(gchar), base_file_system : c_ptr(GArrowFileSystem)) : c_ptr(GArrowSubTreeFileSystem);
  extern proc garrow_slow_file_system_get_type() : GType;
  extern proc GARROW_SLOW_FILE_SYSTEM(ptr : gpointer) : c_ptr(GArrowSlowFileSystem);
  extern proc GARROW_SLOW_FILE_SYSTEM_CLASS(ptr : gpointer) : c_ptr(GArrowSlowFileSystemClass);
  extern proc GARROW_IS_SLOW_FILE_SYSTEM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_SLOW_FILE_SYSTEM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_SLOW_FILE_SYSTEM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowSlowFileSystemClass);
  extern proc garrow_slow_file_system_new_average_latency(ref base_file_system : GArrowFileSystem, average_latency : gdouble) : c_ptr(GArrowSlowFileSystem);
  extern proc garrow_slow_file_system_new_average_latency(base_file_system : c_ptr(GArrowFileSystem), average_latency : gdouble) : c_ptr(GArrowSlowFileSystem);
  extern proc garrow_slow_file_system_new_average_latency_and_seed(ref base_file_system : GArrowFileSystem, average_latency : gdouble, seed : gint32) : c_ptr(GArrowSlowFileSystem);
  extern proc garrow_slow_file_system_new_average_latency_and_seed(base_file_system : c_ptr(GArrowFileSystem), average_latency : gdouble, seed : gint32) : c_ptr(GArrowSlowFileSystem);
  extern proc garrow_mock_file_system_get_type() : GType;
  extern proc GARROW_MOCK_FILE_SYSTEM(ptr : gpointer) : c_ptr(GArrowMockFileSystem);
  extern proc GARROW_MOCK_FILE_SYSTEM_CLASS(ptr : gpointer) : c_ptr(GArrowMockFileSystemClass);
  extern proc GARROW_IS_MOCK_FILE_SYSTEM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_MOCK_FILE_SYSTEM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_MOCK_FILE_SYSTEM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowMockFileSystemClass);
  extern proc garrow_hdfs_file_system_get_type() : GType;
  extern proc GARROW_HDFS_FILE_SYSTEM(ptr : gpointer) : c_ptr(GArrowHDFSFileSystem);
  extern proc GARROW_HDFS_FILE_SYSTEM_CLASS(ptr : gpointer) : c_ptr(GArrowHDFSFileSystemClass);
  extern proc GARROW_IS_HDFS_FILE_SYSTEM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_HDFS_FILE_SYSTEM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_HDFS_FILE_SYSTEM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowHDFSFileSystemClass);
  extern proc garrow_s3_file_system_get_type() : GType;
  extern proc GARROW_S3_FILE_SYSTEM(ptr : gpointer) : c_ptr(GArrowS3FileSystem);
  extern proc GARROW_S3_FILE_SYSTEM_CLASS(ptr : gpointer) : c_ptr(GArrowS3FileSystemClass);
  extern proc GARROW_IS_S3_FILE_SYSTEM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_S3_FILE_SYSTEM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_S3_FILE_SYSTEM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowS3FileSystemClass);
  extern proc garrow_local_file_system_options_get_type() : GType;
  extern proc GARROW_LOCAL_FILE_SYSTEM_OPTIONS(ptr : gpointer) : c_ptr(GArrowLocalFileSystemOptions);
  extern proc GARROW_LOCAL_FILE_SYSTEM_OPTIONS_CLASS(ptr : gpointer) : c_ptr(GArrowLocalFileSystemOptionsClass);
  extern proc GARROW_IS_LOCAL_FILE_SYSTEM_OPTIONS(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LOCAL_FILE_SYSTEM_OPTIONS_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LOCAL_FILE_SYSTEM_OPTIONS_GET_CLASS(ptr : gpointer) : c_ptr(GArrowLocalFileSystemOptionsClass);
  extern proc garrow_local_file_system_options_new() : c_ptr(GArrowLocalFileSystemOptions);
  extern proc garrow_local_file_system_get_type() : GType;
  extern proc GARROW_LOCAL_FILE_SYSTEM(ptr : gpointer) : c_ptr(GArrowLocalFileSystem);
  extern proc GARROW_LOCAL_FILE_SYSTEM_CLASS(ptr : gpointer) : c_ptr(GArrowLocalFileSystemClass);
  extern proc GARROW_IS_LOCAL_FILE_SYSTEM(ptr : gpointer) : gboolean;
  extern proc GARROW_IS_LOCAL_FILE_SYSTEM_CLASS(ptr : gpointer) : gboolean;
  extern proc GARROW_LOCAL_FILE_SYSTEM_GET_CLASS(ptr : gpointer) : c_ptr(GArrowLocalFileSystemClass);
  extern proc garrow_local_file_system_new(ref options : GArrowLocalFileSystemOptions) : c_ptr(GArrowLocalFileSystem);
  extern proc garrow_local_file_system_new(options : c_ptr(GArrowLocalFileSystemOptions)) : c_ptr(GArrowLocalFileSystem);
  extern proc gparquet_arrow_file_reader_get_type() : GType;
  extern proc GPARQUET_ARROW_FILE_READER(ptr : gpointer) : c_ptr(GParquetArrowFileReader);
  extern proc GPARQUET_ARROW_FILE_READER_CLASS(ptr : gpointer) : c_ptr(GParquetArrowFileReaderClass);
  extern proc GPARQUET_IS_ARROW_FILE_READER(ptr : gpointer) : gboolean;
  extern proc GPARQUET_IS_ARROW_FILE_READER_CLASS(ptr : gpointer) : gboolean;
  extern proc GPARQUET_ARROW_FILE_READER_GET_CLASS(ptr : gpointer) : c_ptr(GParquetArrowFileReaderClass);
  extern proc gparquet_arrow_file_reader_new_arrow(ref source : GArrowSeekableInputStream, ref error : c_ptr(GError)) : c_ptr(GParquetArrowFileReader);
  extern proc gparquet_arrow_file_reader_new_arrow(source : c_ptr(GArrowSeekableInputStream), error : c_ptr(c_ptr(GError))) : c_ptr(GParquetArrowFileReader);
  extern proc gparquet_arrow_file_reader_new_path(ref path : gchar, ref error : c_ptr(GError)) : c_ptr(GParquetArrowFileReader);
  extern proc gparquet_arrow_file_reader_new_path(path : c_ptr(gchar), error : c_ptr(c_ptr(GError))) : c_ptr(GParquetArrowFileReader);
  extern proc gparquet_arrow_file_reader_read_table(ref reader : GParquetArrowFileReader, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc gparquet_arrow_file_reader_read_table(reader : c_ptr(GParquetArrowFileReader), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc gparquet_arrow_file_reader_read_row_group(ref reader : GParquetArrowFileReader, row_group_index : gint, ref column_indices : gint, n_column_indices : gsize, ref error : c_ptr(GError)) : c_ptr(GArrowTable);
  extern proc gparquet_arrow_file_reader_read_row_group(reader : c_ptr(GParquetArrowFileReader), row_group_index : gint, column_indices : c_ptr(gint), n_column_indices : gsize, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowTable);
  extern proc gparquet_arrow_file_reader_get_schema(ref reader : GParquetArrowFileReader, ref error : c_ptr(GError)) : c_ptr(GArrowSchema);
  extern proc gparquet_arrow_file_reader_get_schema(reader : c_ptr(GParquetArrowFileReader), error : c_ptr(c_ptr(GError))) : c_ptr(GArrowSchema);
  extern proc gparquet_arrow_file_reader_read_column_data(ref reader : GParquetArrowFileReader, i : gint, ref error : c_ptr(GError)) : c_ptr(GArrowChunkedArray);
  extern proc gparquet_arrow_file_reader_read_column_data(reader : c_ptr(GParquetArrowFileReader), i : gint, error : c_ptr(c_ptr(GError))) : c_ptr(GArrowChunkedArray);
  extern proc gparquet_arrow_file_reader_get_n_row_groups(ref reader : GParquetArrowFileReader) : gint;
  extern proc gparquet_arrow_file_reader_get_n_row_groups(reader : c_ptr(GParquetArrowFileReader)) : gint;
  extern proc gparquet_arrow_file_reader_set_use_threads(ref reader : GParquetArrowFileReader, use_threads : gboolean) : void;
  extern proc gparquet_arrow_file_reader_set_use_threads(reader : c_ptr(GParquetArrowFileReader), use_threads : gboolean) : void;
  extern proc gparquet_writer_properties_get_type() : GType;
  extern proc GPARQUET_WRITER_PROPERTIES(ptr : gpointer) : c_ptr(GParquetWriterProperties);
  extern proc GPARQUET_WRITER_PROPERTIES_CLASS(ptr : gpointer) : c_ptr(GParquetWriterPropertiesClass);
  extern proc GPARQUET_IS_WRITER_PROPERTIES(ptr : gpointer) : gboolean;
  extern proc GPARQUET_IS_WRITER_PROPERTIES_CLASS(ptr : gpointer) : gboolean;
  extern proc GPARQUET_WRITER_PROPERTIES_GET_CLASS(ptr : gpointer) : c_ptr(GParquetWriterPropertiesClass);
  extern proc gparquet_writer_properties_new() : c_ptr(GParquetWriterProperties);
  extern proc gparquet_writer_properties_set_compression(ref properties : GParquetWriterProperties, compression_type : GArrowCompressionType, ref path : gchar) : void;
  extern proc gparquet_writer_properties_set_compression(properties : c_ptr(GParquetWriterProperties), compression_type : GArrowCompressionType, path : c_ptr(gchar)) : void;
  extern proc gparquet_writer_properties_get_compression_path(ref properties : GParquetWriterProperties, ref path : gchar) : GArrowCompressionType;
  extern proc gparquet_writer_properties_get_compression_path(properties : c_ptr(GParquetWriterProperties), path : c_ptr(gchar)) : GArrowCompressionType;
  extern proc gparquet_writer_properties_enable_dictionary(ref properties : GParquetWriterProperties, ref path : gchar) : void;
  extern proc gparquet_writer_properties_enable_dictionary(properties : c_ptr(GParquetWriterProperties), path : c_ptr(gchar)) : void;
  extern proc gparquet_writer_properties_disable_dictionary(ref properties : GParquetWriterProperties, ref path : gchar) : void;
  extern proc gparquet_writer_properties_disable_dictionary(properties : c_ptr(GParquetWriterProperties), path : c_ptr(gchar)) : void;
  extern proc gparquet_writer_properties_is_dictionary_enabled(ref properties : GParquetWriterProperties, ref path : gchar) : gboolean;
  extern proc gparquet_writer_properties_is_dictionary_enabled(properties : c_ptr(GParquetWriterProperties), path : c_ptr(gchar)) : gboolean;
  extern proc gparquet_writer_properties_set_dictionary_page_size_limit(ref properties : GParquetWriterProperties, limit : gint64) : void;
  extern proc gparquet_writer_properties_set_dictionary_page_size_limit(properties : c_ptr(GParquetWriterProperties), limit : gint64) : void;
  extern proc gparquet_writer_properties_get_dictionary_page_size_limit(ref properties : GParquetWriterProperties) : gint64;
  extern proc gparquet_writer_properties_get_dictionary_page_size_limit(properties : c_ptr(GParquetWriterProperties)) : gint64;
  extern proc gparquet_writer_properties_set_batch_size(ref properties : GParquetWriterProperties, batch_size : gint64) : void;
  extern proc gparquet_writer_properties_set_batch_size(properties : c_ptr(GParquetWriterProperties), batch_size : gint64) : void;
  extern proc gparquet_writer_properties_get_batch_size(ref properties : GParquetWriterProperties) : gint64;
  extern proc gparquet_writer_properties_get_batch_size(properties : c_ptr(GParquetWriterProperties)) : gint64;
  extern proc gparquet_writer_properties_set_max_row_group_length(ref properties : GParquetWriterProperties, length : gint64) : void;
  extern proc gparquet_writer_properties_set_max_row_group_length(properties : c_ptr(GParquetWriterProperties), length : gint64) : void;
  extern proc gparquet_writer_properties_get_max_row_group_length(ref properties : GParquetWriterProperties) : gint64;
  extern proc gparquet_writer_properties_get_max_row_group_length(properties : c_ptr(GParquetWriterProperties)) : gint64;
  extern proc gparquet_writer_properties_set_data_page_size(ref properties : GParquetWriterProperties, data_page_size : gint64) : void;
  extern proc gparquet_writer_properties_set_data_page_size(properties : c_ptr(GParquetWriterProperties), data_page_size : gint64) : void;
  extern proc gparquet_writer_properties_get_data_page_size(ref properties : GParquetWriterProperties) : gint64;
  extern proc gparquet_writer_properties_get_data_page_size(properties : c_ptr(GParquetWriterProperties)) : gint64;
  extern proc gparquet_arrow_file_writer_get_type() : GType;
  extern proc GPARQUET_ARROW_FILE_WRITER(ptr : gpointer) : c_ptr(GParquetArrowFileWriter);
  extern proc GPARQUET_ARROW_FILE_WRITER_CLASS(ptr : gpointer) : c_ptr(GParquetArrowFileWriterClass);
  extern proc GPARQUET_IS_ARROW_FILE_WRITER(ptr : gpointer) : gboolean;
  extern proc GPARQUET_IS_ARROW_FILE_WRITER_CLASS(ptr : gpointer) : gboolean;
  extern proc GPARQUET_ARROW_FILE_WRITER_GET_CLASS(ptr : gpointer) : c_ptr(GParquetArrowFileWriterClass);
  extern proc gparquet_arrow_file_writer_new_arrow(ref schema : GArrowSchema, ref sink : GArrowOutputStream, ref writer_properties : GParquetWriterProperties, ref error : c_ptr(GError)) : c_ptr(GParquetArrowFileWriter);
  extern proc gparquet_arrow_file_writer_new_arrow(schema : c_ptr(GArrowSchema), sink : c_ptr(GArrowOutputStream), writer_properties : c_ptr(GParquetWriterProperties), error : c_ptr(c_ptr(GError))) : c_ptr(GParquetArrowFileWriter);
  extern proc gparquet_arrow_file_writer_new_path(ref schema : GArrowSchema, ref path : gchar, ref writer_properties : GParquetWriterProperties, ref error : c_ptr(GError)) : c_ptr(GParquetArrowFileWriter);
  extern proc gparquet_arrow_file_writer_new_path(schema : c_ptr(GArrowSchema), path : c_ptr(gchar), writer_properties : c_ptr(GParquetWriterProperties), error : c_ptr(c_ptr(GError))) : c_ptr(GParquetArrowFileWriter);
  extern proc gparquet_arrow_file_writer_write_table(ref writer : GParquetArrowFileWriter, ref table : GArrowTable, chunk_size : guint64, ref error : c_ptr(GError)) : gboolean;
  extern proc gparquet_arrow_file_writer_write_table(writer : c_ptr(GParquetArrowFileWriter), table : c_ptr(GArrowTable), chunk_size : guint64, error : c_ptr(c_ptr(GError))) : gboolean;
  extern proc gparquet_arrow_file_writer_close(ref writer : GParquetArrowFileWriter, ref error : c_ptr(GError)) : gboolean;
  extern proc gparquet_arrow_file_writer_close(writer : c_ptr(GParquetArrowFileWriter), error : c_ptr(c_ptr(GError))) : gboolean;
  // ==== c2chapel typedefs ====
  // Opaque struct?
  extern type GActionEntry = _GActionEntry;
  // Opaque struct?
  extern type GActionGroupInterface = _GActionGroupInterface;
  extern type GActionInterface = _GActionInterface;
  // Opaque struct?
  extern type GActionMapInterface = _GActionMapInterface;
  extern type GActionMap_autoptr = c_ptr(GActionMap);
  extern type GActionMap_listautoptr = c_ptr(GList);
  extern type GActionMap_queueautoptr = c_ptr(GQueue);
  extern type GActionMap_slistautoptr = c_ptr(GSList);
  extern type GAction_autoptr = c_ptr(GAction);
  extern type GAction_listautoptr = c_ptr(GList);
  extern type GAction_queueautoptr = c_ptr(GQueue);
  extern type GAction_slistautoptr = c_ptr(GSList);
  // Opaque struct?
  // Opaque struct?
  // GAppInfoCreateFlags enum
  extern type GAppInfoCreateFlags = c_int;
  extern const G_APP_INFO_CREATE_NONE :GAppInfoCreateFlags;
  extern const G_APP_INFO_CREATE_NEEDS_TERMINAL :GAppInfoCreateFlags;
  extern const G_APP_INFO_CREATE_SUPPORTS_URIS :GAppInfoCreateFlags;
  extern const G_APP_INFO_CREATE_SUPPORTS_STARTUP_NOTIFICATION :GAppInfoCreateFlags;
  extern type GAppInfoIface = _GAppInfoIface;
  // Opaque struct?
  extern type GAppInfoMonitor_autoptr = c_ptr(GAppInfoMonitor);
  extern type GAppInfoMonitor_listautoptr = c_ptr(GList);
  extern type GAppInfoMonitor_queueautoptr = c_ptr(GQueue);
  extern type GAppInfoMonitor_slistautoptr = c_ptr(GSList);
  extern type GAppInfo_autoptr = c_ptr(GAppInfo);
  extern type GAppInfo_listautoptr = c_ptr(GList);
  extern type GAppInfo_queueautoptr = c_ptr(GQueue);
  extern type GAppInfo_slistautoptr = c_ptr(GSList);
  extern type GAppLaunchContext = _GAppLaunchContext;
  extern type GAppLaunchContextClass = _GAppLaunchContextClass;
  // Opaque struct?
  extern type GAppLaunchContext_autoptr = c_ptr(GAppLaunchContext);
  extern type GAppLaunchContext_listautoptr = c_ptr(GList);
  extern type GAppLaunchContext_queueautoptr = c_ptr(GQueue);
  extern type GAppLaunchContext_slistautoptr = c_ptr(GSList);
  extern type GApplication = _GApplication;
  extern type GApplicationClass = _GApplicationClass;
  extern type GApplicationCommandLine = _GApplicationCommandLine;
  extern type GApplicationCommandLineClass = _GApplicationCommandLineClass;
  // Opaque struct?
  extern type GApplicationCommandLine_autoptr = c_ptr(GApplicationCommandLine);
  extern type GApplicationCommandLine_listautoptr = c_ptr(GList);
  extern type GApplicationCommandLine_queueautoptr = c_ptr(GQueue);
  extern type GApplicationCommandLine_slistautoptr = c_ptr(GSList);
  // GApplicationFlags enum
  extern type GApplicationFlags = c_int;
  extern const G_APPLICATION_FLAGS_NONE :GApplicationFlags;
  extern const G_APPLICATION_IS_SERVICE :GApplicationFlags;
  extern const G_APPLICATION_IS_LAUNCHER :GApplicationFlags;
  extern const G_APPLICATION_HANDLES_OPEN :GApplicationFlags;
  extern const G_APPLICATION_HANDLES_COMMAND_LINE :GApplicationFlags;
  extern const G_APPLICATION_SEND_ENVIRONMENT :GApplicationFlags;
  extern const G_APPLICATION_NON_UNIQUE :GApplicationFlags;
  extern const G_APPLICATION_CAN_OVERRIDE_APP_ID :GApplicationFlags;
  extern const G_APPLICATION_ALLOW_REPLACEMENT :GApplicationFlags;
  extern const G_APPLICATION_REPLACE :GApplicationFlags;
  // Opaque struct?
  extern type GApplication_autoptr = c_ptr(GApplication);
  extern type GApplication_listautoptr = c_ptr(GList);
  extern type GApplication_queueautoptr = c_ptr(GQueue);
  extern type GApplication_slistautoptr = c_ptr(GSList);
  extern type GArray = _GArray;
  extern type GArray_autoptr = c_ptr(GArray);
  extern type GArray_listautoptr = c_ptr(GList);
  extern type GArray_queueautoptr = c_ptr(GQueue);
  extern type GArray_slistautoptr = c_ptr(GSList);
  extern type GArrowArray = _GArrowArray;
  extern type GArrowArrayBuilder = _GArrowArrayBuilder;
  extern type GArrowArrayBuilderClass = _GArrowArrayBuilderClass;
  extern type GArrowArrayBuilderClass_autoptr = c_ptr(GArrowArrayBuilderClass);
  extern type GArrowArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowArrayBuilder_autoptr = c_ptr(GArrowArrayBuilder);
  extern type GArrowArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowArrayClass = _GArrowArrayClass;
  extern type GArrowArrayClass_autoptr = c_ptr(GArrowArrayClass);
  extern type GArrowArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowArrayDatum = _GArrowArrayDatum;
  extern type GArrowArrayDatumClass = _GArrowArrayDatumClass;
  extern type GArrowArrayDatumClass_autoptr = c_ptr(GArrowArrayDatumClass);
  extern type GArrowArrayDatumClass_listautoptr = c_ptr(GList);
  extern type GArrowArrayDatumClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowArrayDatumClass_slistautoptr = c_ptr(GSList);
  extern type GArrowArrayDatum_autoptr = c_ptr(GArrowArrayDatum);
  extern type GArrowArrayDatum_listautoptr = c_ptr(GList);
  extern type GArrowArrayDatum_queueautoptr = c_ptr(GQueue);
  extern type GArrowArrayDatum_slistautoptr = c_ptr(GSList);
  extern type GArrowArraySortOptions = _GArrowArraySortOptions;
  extern type GArrowArraySortOptionsClass = _GArrowArraySortOptionsClass;
  extern type GArrowArraySortOptionsClass_autoptr = c_ptr(GArrowArraySortOptionsClass);
  extern type GArrowArraySortOptionsClass_listautoptr = c_ptr(GList);
  extern type GArrowArraySortOptionsClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowArraySortOptionsClass_slistautoptr = c_ptr(GSList);
  extern type GArrowArraySortOptions_autoptr = c_ptr(GArrowArraySortOptions);
  extern type GArrowArraySortOptions_listautoptr = c_ptr(GList);
  extern type GArrowArraySortOptions_queueautoptr = c_ptr(GQueue);
  extern type GArrowArraySortOptions_slistautoptr = c_ptr(GSList);
  extern type GArrowArray_autoptr = c_ptr(GArrowArray);
  extern type GArrowArray_listautoptr = c_ptr(GList);
  extern type GArrowArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowArray_slistautoptr = c_ptr(GSList);
  extern type GArrowBaseBinaryScalar = _GArrowBaseBinaryScalar;
  extern type GArrowBaseBinaryScalarClass = _GArrowBaseBinaryScalarClass;
  extern type GArrowBaseBinaryScalarClass_autoptr = c_ptr(GArrowBaseBinaryScalarClass);
  extern type GArrowBaseBinaryScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowBaseBinaryScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowBaseBinaryScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowBaseBinaryScalar_autoptr = c_ptr(GArrowBaseBinaryScalar);
  extern type GArrowBaseBinaryScalar_listautoptr = c_ptr(GList);
  extern type GArrowBaseBinaryScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowBaseBinaryScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowBaseListScalar = _GArrowBaseListScalar;
  extern type GArrowBaseListScalarClass = _GArrowBaseListScalarClass;
  extern type GArrowBaseListScalarClass_autoptr = c_ptr(GArrowBaseListScalarClass);
  extern type GArrowBaseListScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowBaseListScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowBaseListScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowBaseListScalar_autoptr = c_ptr(GArrowBaseListScalar);
  extern type GArrowBaseListScalar_listautoptr = c_ptr(GList);
  extern type GArrowBaseListScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowBaseListScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowBinaryArray = _GArrowBinaryArray;
  extern type GArrowBinaryArrayBuilder = _GArrowBinaryArrayBuilder;
  extern type GArrowBinaryArrayBuilderClass = _GArrowBinaryArrayBuilderClass;
  extern type GArrowBinaryArrayBuilderClass_autoptr = c_ptr(GArrowBinaryArrayBuilderClass);
  extern type GArrowBinaryArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowBinaryArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowBinaryArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowBinaryArrayBuilder_autoptr = c_ptr(GArrowBinaryArrayBuilder);
  extern type GArrowBinaryArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowBinaryArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowBinaryArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowBinaryArrayClass = _GArrowBinaryArrayClass;
  extern type GArrowBinaryArrayClass_autoptr = c_ptr(GArrowBinaryArrayClass);
  extern type GArrowBinaryArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowBinaryArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowBinaryArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowBinaryArray_autoptr = c_ptr(GArrowBinaryArray);
  extern type GArrowBinaryArray_listautoptr = c_ptr(GList);
  extern type GArrowBinaryArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowBinaryArray_slistautoptr = c_ptr(GSList);
  extern type GArrowBinaryDataType = _GArrowBinaryDataType;
  extern type GArrowBinaryDataTypeClass = _GArrowBinaryDataTypeClass;
  extern type GArrowBinaryDataTypeClass_autoptr = c_ptr(GArrowBinaryDataTypeClass);
  extern type GArrowBinaryDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowBinaryDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowBinaryDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowBinaryDataType_autoptr = c_ptr(GArrowBinaryDataType);
  extern type GArrowBinaryDataType_listautoptr = c_ptr(GList);
  extern type GArrowBinaryDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowBinaryDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowBinaryDictionaryArrayBuilder = _GArrowBinaryDictionaryArrayBuilder;
  extern type GArrowBinaryDictionaryArrayBuilderClass = _GArrowBinaryDictionaryArrayBuilderClass;
  extern type GArrowBinaryDictionaryArrayBuilderClass_autoptr = c_ptr(GArrowBinaryDictionaryArrayBuilderClass);
  extern type GArrowBinaryDictionaryArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowBinaryDictionaryArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowBinaryDictionaryArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowBinaryDictionaryArrayBuilder_autoptr = c_ptr(GArrowBinaryDictionaryArrayBuilder);
  extern type GArrowBinaryDictionaryArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowBinaryDictionaryArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowBinaryDictionaryArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowBinaryScalar = _GArrowBinaryScalar;
  extern type GArrowBinaryScalarClass = _GArrowBinaryScalarClass;
  extern type GArrowBinaryScalarClass_autoptr = c_ptr(GArrowBinaryScalarClass);
  extern type GArrowBinaryScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowBinaryScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowBinaryScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowBinaryScalar_autoptr = c_ptr(GArrowBinaryScalar);
  extern type GArrowBinaryScalar_listautoptr = c_ptr(GList);
  extern type GArrowBinaryScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowBinaryScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowBooleanArray = _GArrowBooleanArray;
  extern type GArrowBooleanArrayBuilder = _GArrowBooleanArrayBuilder;
  extern type GArrowBooleanArrayBuilderClass = _GArrowBooleanArrayBuilderClass;
  extern type GArrowBooleanArrayBuilderClass_autoptr = c_ptr(GArrowBooleanArrayBuilderClass);
  extern type GArrowBooleanArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowBooleanArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowBooleanArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowBooleanArrayBuilder_autoptr = c_ptr(GArrowBooleanArrayBuilder);
  extern type GArrowBooleanArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowBooleanArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowBooleanArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowBooleanArrayClass = _GArrowBooleanArrayClass;
  extern type GArrowBooleanArrayClass_autoptr = c_ptr(GArrowBooleanArrayClass);
  extern type GArrowBooleanArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowBooleanArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowBooleanArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowBooleanArray_autoptr = c_ptr(GArrowBooleanArray);
  extern type GArrowBooleanArray_listautoptr = c_ptr(GList);
  extern type GArrowBooleanArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowBooleanArray_slistautoptr = c_ptr(GSList);
  extern type GArrowBooleanDataType = _GArrowBooleanDataType;
  extern type GArrowBooleanDataTypeClass = _GArrowBooleanDataTypeClass;
  extern type GArrowBooleanDataTypeClass_autoptr = c_ptr(GArrowBooleanDataTypeClass);
  extern type GArrowBooleanDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowBooleanDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowBooleanDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowBooleanDataType_autoptr = c_ptr(GArrowBooleanDataType);
  extern type GArrowBooleanDataType_listautoptr = c_ptr(GList);
  extern type GArrowBooleanDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowBooleanDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowBooleanScalar = _GArrowBooleanScalar;
  extern type GArrowBooleanScalarClass = _GArrowBooleanScalarClass;
  extern type GArrowBooleanScalarClass_autoptr = c_ptr(GArrowBooleanScalarClass);
  extern type GArrowBooleanScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowBooleanScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowBooleanScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowBooleanScalar_autoptr = c_ptr(GArrowBooleanScalar);
  extern type GArrowBooleanScalar_listautoptr = c_ptr(GList);
  extern type GArrowBooleanScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowBooleanScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowBuffer = _GArrowBuffer;
  extern type GArrowBufferClass = _GArrowBufferClass;
  extern type GArrowBufferClass_autoptr = c_ptr(GArrowBufferClass);
  extern type GArrowBufferClass_listautoptr = c_ptr(GList);
  extern type GArrowBufferClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowBufferClass_slistautoptr = c_ptr(GSList);
  extern type GArrowBufferInputStream = _GArrowBufferInputStream;
  extern type GArrowBufferInputStreamClass = _GArrowBufferInputStreamClass;
  extern type GArrowBufferInputStreamClass_autoptr = c_ptr(GArrowBufferInputStreamClass);
  extern type GArrowBufferInputStreamClass_listautoptr = c_ptr(GList);
  extern type GArrowBufferInputStreamClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowBufferInputStreamClass_slistautoptr = c_ptr(GSList);
  extern type GArrowBufferInputStream_autoptr = c_ptr(GArrowBufferInputStream);
  extern type GArrowBufferInputStream_listautoptr = c_ptr(GList);
  extern type GArrowBufferInputStream_queueautoptr = c_ptr(GQueue);
  extern type GArrowBufferInputStream_slistautoptr = c_ptr(GSList);
  extern type GArrowBufferOutputStream = _GArrowBufferOutputStream;
  extern type GArrowBufferOutputStreamClass = _GArrowBufferOutputStreamClass;
  extern type GArrowBuffer_autoptr = c_ptr(GArrowBuffer);
  extern type GArrowBuffer_listautoptr = c_ptr(GList);
  extern type GArrowBuffer_queueautoptr = c_ptr(GQueue);
  extern type GArrowBuffer_slistautoptr = c_ptr(GSList);
  extern type GArrowCSVReadOptions = _GArrowCSVReadOptions;
  extern type GArrowCSVReadOptionsClass = _GArrowCSVReadOptionsClass;
  extern type GArrowCSVReadOptionsClass_autoptr = c_ptr(GArrowCSVReadOptionsClass);
  extern type GArrowCSVReadOptionsClass_listautoptr = c_ptr(GList);
  extern type GArrowCSVReadOptionsClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowCSVReadOptionsClass_slistautoptr = c_ptr(GSList);
  extern type GArrowCSVReadOptions_autoptr = c_ptr(GArrowCSVReadOptions);
  extern type GArrowCSVReadOptions_listautoptr = c_ptr(GList);
  extern type GArrowCSVReadOptions_queueautoptr = c_ptr(GQueue);
  extern type GArrowCSVReadOptions_slistautoptr = c_ptr(GSList);
  extern type GArrowCSVReader = _GArrowCSVReader;
  extern type GArrowCSVReaderClass = _GArrowCSVReaderClass;
  extern type GArrowCSVReaderClass_autoptr = c_ptr(GArrowCSVReaderClass);
  extern type GArrowCSVReaderClass_listautoptr = c_ptr(GList);
  extern type GArrowCSVReaderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowCSVReaderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowCSVReader_autoptr = c_ptr(GArrowCSVReader);
  extern type GArrowCSVReader_listautoptr = c_ptr(GList);
  extern type GArrowCSVReader_queueautoptr = c_ptr(GQueue);
  extern type GArrowCSVReader_slistautoptr = c_ptr(GSList);
  extern type GArrowCastOptions = _GArrowCastOptions;
  extern type GArrowCastOptionsClass = _GArrowCastOptionsClass;
  extern type GArrowCastOptionsClass_autoptr = c_ptr(GArrowCastOptionsClass);
  extern type GArrowCastOptionsClass_listautoptr = c_ptr(GList);
  extern type GArrowCastOptionsClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowCastOptionsClass_slistautoptr = c_ptr(GSList);
  extern type GArrowCastOptions_autoptr = c_ptr(GArrowCastOptions);
  extern type GArrowCastOptions_listautoptr = c_ptr(GList);
  extern type GArrowCastOptions_queueautoptr = c_ptr(GQueue);
  extern type GArrowCastOptions_slistautoptr = c_ptr(GSList);
  extern type GArrowChunkedArray = _GArrowChunkedArray;
  extern type GArrowChunkedArrayClass = _GArrowChunkedArrayClass;
  extern type GArrowChunkedArrayClass_autoptr = c_ptr(GArrowChunkedArrayClass);
  extern type GArrowChunkedArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowChunkedArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowChunkedArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowChunkedArrayDatum = _GArrowChunkedArrayDatum;
  extern type GArrowChunkedArrayDatumClass = _GArrowChunkedArrayDatumClass;
  extern type GArrowChunkedArrayDatumClass_autoptr = c_ptr(GArrowChunkedArrayDatumClass);
  extern type GArrowChunkedArrayDatumClass_listautoptr = c_ptr(GList);
  extern type GArrowChunkedArrayDatumClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowChunkedArrayDatumClass_slistautoptr = c_ptr(GSList);
  extern type GArrowChunkedArrayDatum_autoptr = c_ptr(GArrowChunkedArrayDatum);
  extern type GArrowChunkedArrayDatum_listautoptr = c_ptr(GList);
  extern type GArrowChunkedArrayDatum_queueautoptr = c_ptr(GQueue);
  extern type GArrowChunkedArrayDatum_slistautoptr = c_ptr(GSList);
  extern type GArrowChunkedArray_autoptr = c_ptr(GArrowChunkedArray);
  extern type GArrowChunkedArray_listautoptr = c_ptr(GList);
  extern type GArrowChunkedArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowChunkedArray_slistautoptr = c_ptr(GSList);
  extern type GArrowCodec = _GArrowCodec;
  extern type GArrowCodecClass = _GArrowCodecClass;
  extern type GArrowCodecClass_autoptr = c_ptr(GArrowCodecClass);
  extern type GArrowCodecClass_listautoptr = c_ptr(GList);
  extern type GArrowCodecClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowCodecClass_slistautoptr = c_ptr(GSList);
  extern type GArrowCodec_autoptr = c_ptr(GArrowCodec);
  extern type GArrowCodec_listautoptr = c_ptr(GList);
  extern type GArrowCodec_queueautoptr = c_ptr(GQueue);
  extern type GArrowCodec_slistautoptr = c_ptr(GSList);
  extern type GArrowCompressedInputStream = _GArrowCompressedInputStream;
  extern type GArrowCompressedInputStreamClass = _GArrowCompressedInputStreamClass;
  extern type GArrowCompressedInputStreamClass_autoptr = c_ptr(GArrowCompressedInputStreamClass);
  extern type GArrowCompressedInputStreamClass_listautoptr = c_ptr(GList);
  extern type GArrowCompressedInputStreamClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowCompressedInputStreamClass_slistautoptr = c_ptr(GSList);
  extern type GArrowCompressedInputStream_autoptr = c_ptr(GArrowCompressedInputStream);
  extern type GArrowCompressedInputStream_listautoptr = c_ptr(GList);
  extern type GArrowCompressedInputStream_queueautoptr = c_ptr(GQueue);
  extern type GArrowCompressedInputStream_slistautoptr = c_ptr(GSList);
  extern type GArrowCompressedOutputStream = _GArrowCompressedOutputStream;
  extern type GArrowCompressedOutputStreamClass = _GArrowCompressedOutputStreamClass;
  extern type GArrowCompressedOutputStreamClass_autoptr = c_ptr(GArrowCompressedOutputStreamClass);
  extern type GArrowCompressedOutputStreamClass_listautoptr = c_ptr(GList);
  extern type GArrowCompressedOutputStreamClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowCompressedOutputStreamClass_slistautoptr = c_ptr(GSList);
  extern type GArrowCompressedOutputStream_autoptr = c_ptr(GArrowCompressedOutputStream);
  extern type GArrowCompressedOutputStream_listautoptr = c_ptr(GList);
  extern type GArrowCompressedOutputStream_queueautoptr = c_ptr(GQueue);
  extern type GArrowCompressedOutputStream_slistautoptr = c_ptr(GSList);
  // GArrowCompressionType enum
  extern type GArrowCompressionType = c_int;
  extern const GARROW_COMPRESSION_TYPE_UNCOMPRESSED :GArrowCompressionType;
  extern const GARROW_COMPRESSION_TYPE_SNAPPY :GArrowCompressionType;
  extern const GARROW_COMPRESSION_TYPE_GZIP :GArrowCompressionType;
  extern const GARROW_COMPRESSION_TYPE_BROTLI :GArrowCompressionType;
  extern const GARROW_COMPRESSION_TYPE_ZSTD :GArrowCompressionType;
  extern const GARROW_COMPRESSION_TYPE_LZ4 :GArrowCompressionType;
  extern const GARROW_COMPRESSION_TYPE_LZO :GArrowCompressionType;
  extern const GARROW_COMPRESSION_TYPE_BZ2 :GArrowCompressionType;
  extern type GArrowDataType = _GArrowDataType;
  extern type GArrowDataTypeClass = _GArrowDataTypeClass;
  extern type GArrowDataTypeClass_autoptr = c_ptr(GArrowDataTypeClass);
  extern type GArrowDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDataType_autoptr = c_ptr(GArrowDataType);
  extern type GArrowDataType_listautoptr = c_ptr(GList);
  extern type GArrowDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowDate32Array = _GArrowDate32Array;
  extern type GArrowDate32ArrayBuilder = _GArrowDate32ArrayBuilder;
  extern type GArrowDate32ArrayBuilderClass = _GArrowDate32ArrayBuilderClass;
  extern type GArrowDate32ArrayBuilderClass_autoptr = c_ptr(GArrowDate32ArrayBuilderClass);
  extern type GArrowDate32ArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowDate32ArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate32ArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDate32ArrayBuilder_autoptr = c_ptr(GArrowDate32ArrayBuilder);
  extern type GArrowDate32ArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowDate32ArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate32ArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowDate32ArrayClass = _GArrowDate32ArrayClass;
  extern type GArrowDate32ArrayClass_autoptr = c_ptr(GArrowDate32ArrayClass);
  extern type GArrowDate32ArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowDate32ArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate32ArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDate32Array_autoptr = c_ptr(GArrowDate32Array);
  extern type GArrowDate32Array_listautoptr = c_ptr(GList);
  extern type GArrowDate32Array_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate32Array_slistautoptr = c_ptr(GSList);
  extern type GArrowDate32DataType = _GArrowDate32DataType;
  extern type GArrowDate32DataTypeClass = _GArrowDate32DataTypeClass;
  extern type GArrowDate32DataTypeClass_autoptr = c_ptr(GArrowDate32DataTypeClass);
  extern type GArrowDate32DataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowDate32DataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate32DataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDate32DataType_autoptr = c_ptr(GArrowDate32DataType);
  extern type GArrowDate32DataType_listautoptr = c_ptr(GList);
  extern type GArrowDate32DataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate32DataType_slistautoptr = c_ptr(GSList);
  extern type GArrowDate32Scalar = _GArrowDate32Scalar;
  extern type GArrowDate32ScalarClass = _GArrowDate32ScalarClass;
  extern type GArrowDate32ScalarClass_autoptr = c_ptr(GArrowDate32ScalarClass);
  extern type GArrowDate32ScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowDate32ScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate32ScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDate32Scalar_autoptr = c_ptr(GArrowDate32Scalar);
  extern type GArrowDate32Scalar_listautoptr = c_ptr(GList);
  extern type GArrowDate32Scalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate32Scalar_slistautoptr = c_ptr(GSList);
  extern type GArrowDate64Array = _GArrowDate64Array;
  extern type GArrowDate64ArrayBuilder = _GArrowDate64ArrayBuilder;
  extern type GArrowDate64ArrayBuilderClass = _GArrowDate64ArrayBuilderClass;
  extern type GArrowDate64ArrayBuilderClass_autoptr = c_ptr(GArrowDate64ArrayBuilderClass);
  extern type GArrowDate64ArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowDate64ArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate64ArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDate64ArrayBuilder_autoptr = c_ptr(GArrowDate64ArrayBuilder);
  extern type GArrowDate64ArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowDate64ArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate64ArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowDate64ArrayClass = _GArrowDate64ArrayClass;
  extern type GArrowDate64ArrayClass_autoptr = c_ptr(GArrowDate64ArrayClass);
  extern type GArrowDate64ArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowDate64ArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate64ArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDate64Array_autoptr = c_ptr(GArrowDate64Array);
  extern type GArrowDate64Array_listautoptr = c_ptr(GList);
  extern type GArrowDate64Array_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate64Array_slistautoptr = c_ptr(GSList);
  extern type GArrowDate64DataType = _GArrowDate64DataType;
  extern type GArrowDate64DataTypeClass = _GArrowDate64DataTypeClass;
  extern type GArrowDate64DataTypeClass_autoptr = c_ptr(GArrowDate64DataTypeClass);
  extern type GArrowDate64DataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowDate64DataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate64DataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDate64DataType_autoptr = c_ptr(GArrowDate64DataType);
  extern type GArrowDate64DataType_listautoptr = c_ptr(GList);
  extern type GArrowDate64DataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate64DataType_slistautoptr = c_ptr(GSList);
  extern type GArrowDate64Scalar = _GArrowDate64Scalar;
  extern type GArrowDate64ScalarClass = _GArrowDate64ScalarClass;
  extern type GArrowDate64ScalarClass_autoptr = c_ptr(GArrowDate64ScalarClass);
  extern type GArrowDate64ScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowDate64ScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate64ScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDate64Scalar_autoptr = c_ptr(GArrowDate64Scalar);
  extern type GArrowDate64Scalar_listautoptr = c_ptr(GList);
  extern type GArrowDate64Scalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowDate64Scalar_slistautoptr = c_ptr(GSList);
  extern type GArrowDatum = _GArrowDatum;
  extern type GArrowDatumClass = _GArrowDatumClass;
  extern type GArrowDatumClass_autoptr = c_ptr(GArrowDatumClass);
  extern type GArrowDatumClass_listautoptr = c_ptr(GList);
  extern type GArrowDatumClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDatumClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDatum_autoptr = c_ptr(GArrowDatum);
  extern type GArrowDatum_listautoptr = c_ptr(GList);
  extern type GArrowDatum_queueautoptr = c_ptr(GQueue);
  extern type GArrowDatum_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal128 = _GArrowDecimal128;
  extern type GArrowDecimal128Array = _GArrowDecimal128Array;
  extern type GArrowDecimal128ArrayBuilder = _GArrowDecimal128ArrayBuilder;
  extern type GArrowDecimal128ArrayBuilderClass = _GArrowDecimal128ArrayBuilderClass;
  extern type GArrowDecimal128ArrayBuilderClass_autoptr = c_ptr(GArrowDecimal128ArrayBuilderClass);
  extern type GArrowDecimal128ArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowDecimal128ArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal128ArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal128ArrayBuilder_autoptr = c_ptr(GArrowDecimal128ArrayBuilder);
  extern type GArrowDecimal128ArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowDecimal128ArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal128ArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal128ArrayClass = _GArrowDecimal128ArrayClass;
  extern type GArrowDecimal128ArrayClass_autoptr = c_ptr(GArrowDecimal128ArrayClass);
  extern type GArrowDecimal128ArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowDecimal128ArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal128ArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal128Array_autoptr = c_ptr(GArrowDecimal128Array);
  extern type GArrowDecimal128Array_listautoptr = c_ptr(GList);
  extern type GArrowDecimal128Array_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal128Array_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal128Class = _GArrowDecimal128Class;
  extern type GArrowDecimal128Class_autoptr = c_ptr(GArrowDecimal128Class);
  extern type GArrowDecimal128Class_listautoptr = c_ptr(GList);
  extern type GArrowDecimal128Class_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal128Class_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal128DataType = _GArrowDecimal128DataType;
  extern type GArrowDecimal128DataTypeClass = _GArrowDecimal128DataTypeClass;
  extern type GArrowDecimal128DataTypeClass_autoptr = c_ptr(GArrowDecimal128DataTypeClass);
  extern type GArrowDecimal128DataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowDecimal128DataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal128DataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal128DataType_autoptr = c_ptr(GArrowDecimal128DataType);
  extern type GArrowDecimal128DataType_listautoptr = c_ptr(GList);
  extern type GArrowDecimal128DataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal128DataType_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal128Scalar = _GArrowDecimal128Scalar;
  extern type GArrowDecimal128ScalarClass = _GArrowDecimal128ScalarClass;
  extern type GArrowDecimal128ScalarClass_autoptr = c_ptr(GArrowDecimal128ScalarClass);
  extern type GArrowDecimal128ScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowDecimal128ScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal128ScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal128Scalar_autoptr = c_ptr(GArrowDecimal128Scalar);
  extern type GArrowDecimal128Scalar_listautoptr = c_ptr(GList);
  extern type GArrowDecimal128Scalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal128Scalar_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal128_autoptr = c_ptr(GArrowDecimal128);
  extern type GArrowDecimal128_listautoptr = c_ptr(GList);
  extern type GArrowDecimal128_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal128_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal256 = _GArrowDecimal256;
  extern type GArrowDecimal256Array = _GArrowDecimal256Array;
  extern type GArrowDecimal256ArrayBuilder = _GArrowDecimal256ArrayBuilder;
  extern type GArrowDecimal256ArrayBuilderClass = _GArrowDecimal256ArrayBuilderClass;
  extern type GArrowDecimal256ArrayBuilderClass_autoptr = c_ptr(GArrowDecimal256ArrayBuilderClass);
  extern type GArrowDecimal256ArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowDecimal256ArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal256ArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal256ArrayBuilder_autoptr = c_ptr(GArrowDecimal256ArrayBuilder);
  extern type GArrowDecimal256ArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowDecimal256ArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal256ArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal256ArrayClass = _GArrowDecimal256ArrayClass;
  extern type GArrowDecimal256ArrayClass_autoptr = c_ptr(GArrowDecimal256ArrayClass);
  extern type GArrowDecimal256ArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowDecimal256ArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal256ArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal256Array_autoptr = c_ptr(GArrowDecimal256Array);
  extern type GArrowDecimal256Array_listautoptr = c_ptr(GList);
  extern type GArrowDecimal256Array_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal256Array_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal256Class = _GArrowDecimal256Class;
  extern type GArrowDecimal256Class_autoptr = c_ptr(GArrowDecimal256Class);
  extern type GArrowDecimal256Class_listautoptr = c_ptr(GList);
  extern type GArrowDecimal256Class_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal256Class_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal256DataType = _GArrowDecimal256DataType;
  extern type GArrowDecimal256DataTypeClass = _GArrowDecimal256DataTypeClass;
  extern type GArrowDecimal256DataTypeClass_autoptr = c_ptr(GArrowDecimal256DataTypeClass);
  extern type GArrowDecimal256DataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowDecimal256DataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal256DataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal256DataType_autoptr = c_ptr(GArrowDecimal256DataType);
  extern type GArrowDecimal256DataType_listautoptr = c_ptr(GList);
  extern type GArrowDecimal256DataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal256DataType_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal256Scalar = _GArrowDecimal256Scalar;
  extern type GArrowDecimal256ScalarClass = _GArrowDecimal256ScalarClass;
  extern type GArrowDecimal256ScalarClass_autoptr = c_ptr(GArrowDecimal256ScalarClass);
  extern type GArrowDecimal256ScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowDecimal256ScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal256ScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal256Scalar_autoptr = c_ptr(GArrowDecimal256Scalar);
  extern type GArrowDecimal256Scalar_listautoptr = c_ptr(GList);
  extern type GArrowDecimal256Scalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal256Scalar_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimal256_autoptr = c_ptr(GArrowDecimal256);
  extern type GArrowDecimal256_listautoptr = c_ptr(GList);
  extern type GArrowDecimal256_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimal256_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimalDataType = _GArrowDecimalDataType;
  extern type GArrowDecimalDataTypeClass = _GArrowDecimalDataTypeClass;
  extern type GArrowDecimalDataTypeClass_autoptr = c_ptr(GArrowDecimalDataTypeClass);
  extern type GArrowDecimalDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowDecimalDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimalDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDecimalDataType_autoptr = c_ptr(GArrowDecimalDataType);
  extern type GArrowDecimalDataType_listautoptr = c_ptr(GList);
  extern type GArrowDecimalDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowDecimalDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowDenseUnionArray = _GArrowDenseUnionArray;
  extern type GArrowDenseUnionArrayClass = _GArrowDenseUnionArrayClass;
  extern type GArrowDenseUnionArrayClass_autoptr = c_ptr(GArrowDenseUnionArrayClass);
  extern type GArrowDenseUnionArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowDenseUnionArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDenseUnionArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDenseUnionArray_autoptr = c_ptr(GArrowDenseUnionArray);
  extern type GArrowDenseUnionArray_listautoptr = c_ptr(GList);
  extern type GArrowDenseUnionArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowDenseUnionArray_slistautoptr = c_ptr(GSList);
  extern type GArrowDenseUnionDataType = _GArrowDenseUnionDataType;
  extern type GArrowDenseUnionDataTypeClass = _GArrowDenseUnionDataTypeClass;
  extern type GArrowDenseUnionDataTypeClass_autoptr = c_ptr(GArrowDenseUnionDataTypeClass);
  extern type GArrowDenseUnionDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowDenseUnionDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDenseUnionDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDenseUnionDataType_autoptr = c_ptr(GArrowDenseUnionDataType);
  extern type GArrowDenseUnionDataType_listautoptr = c_ptr(GList);
  extern type GArrowDenseUnionDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowDenseUnionDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowDenseUnionScalar = _GArrowDenseUnionScalar;
  extern type GArrowDenseUnionScalarClass = _GArrowDenseUnionScalarClass;
  extern type GArrowDenseUnionScalarClass_autoptr = c_ptr(GArrowDenseUnionScalarClass);
  extern type GArrowDenseUnionScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowDenseUnionScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDenseUnionScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDenseUnionScalar_autoptr = c_ptr(GArrowDenseUnionScalar);
  extern type GArrowDenseUnionScalar_listautoptr = c_ptr(GList);
  extern type GArrowDenseUnionScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowDenseUnionScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowDictionaryArray = _GArrowDictionaryArray;
  extern type GArrowDictionaryArrayClass = _GArrowDictionaryArrayClass;
  extern type GArrowDictionaryArrayClass_autoptr = c_ptr(GArrowDictionaryArrayClass);
  extern type GArrowDictionaryArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowDictionaryArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDictionaryArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDictionaryArray_autoptr = c_ptr(GArrowDictionaryArray);
  extern type GArrowDictionaryArray_listautoptr = c_ptr(GList);
  extern type GArrowDictionaryArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowDictionaryArray_slistautoptr = c_ptr(GSList);
  extern type GArrowDictionaryDataType = _GArrowDictionaryDataType;
  extern type GArrowDictionaryDataTypeClass = _GArrowDictionaryDataTypeClass;
  extern type GArrowDictionaryDataTypeClass_autoptr = c_ptr(GArrowDictionaryDataTypeClass);
  extern type GArrowDictionaryDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowDictionaryDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDictionaryDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDictionaryDataType_autoptr = c_ptr(GArrowDictionaryDataType);
  extern type GArrowDictionaryDataType_listautoptr = c_ptr(GList);
  extern type GArrowDictionaryDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowDictionaryDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowDoubleArray = _GArrowDoubleArray;
  extern type GArrowDoubleArrayBuilder = _GArrowDoubleArrayBuilder;
  extern type GArrowDoubleArrayBuilderClass = _GArrowDoubleArrayBuilderClass;
  extern type GArrowDoubleArrayBuilderClass_autoptr = c_ptr(GArrowDoubleArrayBuilderClass);
  extern type GArrowDoubleArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowDoubleArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDoubleArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDoubleArrayBuilder_autoptr = c_ptr(GArrowDoubleArrayBuilder);
  extern type GArrowDoubleArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowDoubleArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowDoubleArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowDoubleArrayClass = _GArrowDoubleArrayClass;
  extern type GArrowDoubleArrayClass_autoptr = c_ptr(GArrowDoubleArrayClass);
  extern type GArrowDoubleArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowDoubleArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDoubleArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDoubleArray_autoptr = c_ptr(GArrowDoubleArray);
  extern type GArrowDoubleArray_listautoptr = c_ptr(GList);
  extern type GArrowDoubleArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowDoubleArray_slistautoptr = c_ptr(GSList);
  extern type GArrowDoubleDataType = _GArrowDoubleDataType;
  extern type GArrowDoubleDataTypeClass = _GArrowDoubleDataTypeClass;
  extern type GArrowDoubleDataTypeClass_autoptr = c_ptr(GArrowDoubleDataTypeClass);
  extern type GArrowDoubleDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowDoubleDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDoubleDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDoubleDataType_autoptr = c_ptr(GArrowDoubleDataType);
  extern type GArrowDoubleDataType_listautoptr = c_ptr(GList);
  extern type GArrowDoubleDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowDoubleDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowDoubleScalar = _GArrowDoubleScalar;
  extern type GArrowDoubleScalarClass = _GArrowDoubleScalarClass;
  extern type GArrowDoubleScalarClass_autoptr = c_ptr(GArrowDoubleScalarClass);
  extern type GArrowDoubleScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowDoubleScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowDoubleScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowDoubleScalar_autoptr = c_ptr(GArrowDoubleScalar);
  extern type GArrowDoubleScalar_listautoptr = c_ptr(GList);
  extern type GArrowDoubleScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowDoubleScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowEqualOptions = _GArrowEqualOptions;
  extern type GArrowEqualOptionsClass = _GArrowEqualOptionsClass;
  extern type GArrowEqualOptionsClass_autoptr = c_ptr(GArrowEqualOptionsClass);
  extern type GArrowEqualOptionsClass_listautoptr = c_ptr(GList);
  extern type GArrowEqualOptionsClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowEqualOptionsClass_slistautoptr = c_ptr(GSList);
  extern type GArrowEqualOptions_autoptr = c_ptr(GArrowEqualOptions);
  extern type GArrowEqualOptions_listautoptr = c_ptr(GList);
  extern type GArrowEqualOptions_queueautoptr = c_ptr(GQueue);
  extern type GArrowEqualOptions_slistautoptr = c_ptr(GSList);
  // GArrowError enum
  extern type GArrowError = c_int;
  extern const GARROW_ERROR_OUT_OF_MEMORY :GArrowError;
  extern const GARROW_ERROR_KEY :GArrowError;
  extern const GARROW_ERROR_TYPE :GArrowError;
  extern const GARROW_ERROR_INVALID :GArrowError;
  extern const GARROW_ERROR_IO :GArrowError;
  extern const GARROW_ERROR_CAPACITY :GArrowError;
  extern const GARROW_ERROR_INDEX :GArrowError;
  extern const GARROW_ERROR_UNKNOWN :GArrowError;
  extern const GARROW_ERROR_NOT_IMPLEMENTED :GArrowError;
  extern const GARROW_ERROR_SERIALIZATION :GArrowError;
  extern const GARROW_ERROR_CODE_GENERATION :GArrowError;
  extern const GARROW_ERROR_EXPRESSION_VALIDATION :GArrowError;
  extern const GARROW_ERROR_EXECUTION :GArrowError;
  extern const GARROW_ERROR_ALREADY_EXISTS :GArrowError;
  extern type GArrowExecuteContext = _GArrowExecuteContext;
  extern type GArrowExecuteContextClass = _GArrowExecuteContextClass;
  extern type GArrowExecuteContextClass_autoptr = c_ptr(GArrowExecuteContextClass);
  extern type GArrowExecuteContextClass_listautoptr = c_ptr(GList);
  extern type GArrowExecuteContextClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowExecuteContextClass_slistautoptr = c_ptr(GSList);
  extern type GArrowExecuteContext_autoptr = c_ptr(GArrowExecuteContext);
  extern type GArrowExecuteContext_listautoptr = c_ptr(GList);
  extern type GArrowExecuteContext_queueautoptr = c_ptr(GQueue);
  extern type GArrowExecuteContext_slistautoptr = c_ptr(GSList);
  extern type GArrowExtensionArray = _GArrowExtensionArray;
  extern type GArrowExtensionArrayClass = _GArrowExtensionArrayClass;
  extern type GArrowExtensionArrayClass_autoptr = c_ptr(GArrowExtensionArrayClass);
  extern type GArrowExtensionArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowExtensionArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowExtensionArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowExtensionArray_autoptr = c_ptr(GArrowExtensionArray);
  extern type GArrowExtensionArray_listautoptr = c_ptr(GList);
  extern type GArrowExtensionArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowExtensionArray_slistautoptr = c_ptr(GSList);
  extern type GArrowExtensionDataType = _GArrowExtensionDataType;
  extern type GArrowExtensionDataTypeClass = _GArrowExtensionDataTypeClass;
  extern type GArrowExtensionDataTypeClass_autoptr = c_ptr(GArrowExtensionDataTypeClass);
  extern type GArrowExtensionDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowExtensionDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowExtensionDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowExtensionDataTypeRegistry = _GArrowExtensionDataTypeRegistry;
  extern type GArrowExtensionDataTypeRegistryClass = _GArrowExtensionDataTypeRegistryClass;
  extern type GArrowExtensionDataTypeRegistryClass_autoptr = c_ptr(GArrowExtensionDataTypeRegistryClass);
  extern type GArrowExtensionDataTypeRegistryClass_listautoptr = c_ptr(GList);
  extern type GArrowExtensionDataTypeRegistryClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowExtensionDataTypeRegistryClass_slistautoptr = c_ptr(GSList);
  extern type GArrowExtensionDataTypeRegistry_autoptr = c_ptr(GArrowExtensionDataTypeRegistry);
  extern type GArrowExtensionDataTypeRegistry_listautoptr = c_ptr(GList);
  extern type GArrowExtensionDataTypeRegistry_queueautoptr = c_ptr(GQueue);
  extern type GArrowExtensionDataTypeRegistry_slistautoptr = c_ptr(GSList);
  extern type GArrowExtensionDataType_autoptr = c_ptr(GArrowExtensionDataType);
  extern type GArrowExtensionDataType_listautoptr = c_ptr(GList);
  extern type GArrowExtensionDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowExtensionDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowExtensionScalar = _GArrowExtensionScalar;
  extern type GArrowExtensionScalarClass = _GArrowExtensionScalarClass;
  extern type GArrowExtensionScalarClass_autoptr = c_ptr(GArrowExtensionScalarClass);
  extern type GArrowExtensionScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowExtensionScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowExtensionScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowExtensionScalar_autoptr = c_ptr(GArrowExtensionScalar);
  extern type GArrowExtensionScalar_listautoptr = c_ptr(GList);
  extern type GArrowExtensionScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowExtensionScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowFeatherFileReader = _GArrowFeatherFileReader;
  extern type GArrowFeatherFileReaderClass = _GArrowFeatherFileReaderClass;
  extern type GArrowFeatherFileReaderClass_autoptr = c_ptr(GArrowFeatherFileReaderClass);
  extern type GArrowFeatherFileReaderClass_listautoptr = c_ptr(GList);
  extern type GArrowFeatherFileReaderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFeatherFileReaderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFeatherFileReader_autoptr = c_ptr(GArrowFeatherFileReader);
  extern type GArrowFeatherFileReader_listautoptr = c_ptr(GList);
  extern type GArrowFeatherFileReader_queueautoptr = c_ptr(GQueue);
  extern type GArrowFeatherFileReader_slistautoptr = c_ptr(GSList);
  extern type GArrowFeatherWriteProperties = _GArrowFeatherWriteProperties;
  extern type GArrowFeatherWritePropertiesClass = _GArrowFeatherWritePropertiesClass;
  extern type GArrowFeatherWritePropertiesClass_autoptr = c_ptr(GArrowFeatherWritePropertiesClass);
  extern type GArrowFeatherWritePropertiesClass_listautoptr = c_ptr(GList);
  extern type GArrowFeatherWritePropertiesClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFeatherWritePropertiesClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFeatherWriteProperties_autoptr = c_ptr(GArrowFeatherWriteProperties);
  extern type GArrowFeatherWriteProperties_listautoptr = c_ptr(GList);
  extern type GArrowFeatherWriteProperties_queueautoptr = c_ptr(GQueue);
  extern type GArrowFeatherWriteProperties_slistautoptr = c_ptr(GSList);
  extern type GArrowField = _GArrowField;
  extern type GArrowFieldClass = _GArrowFieldClass;
  extern type GArrowFieldClass_autoptr = c_ptr(GArrowFieldClass);
  extern type GArrowFieldClass_listautoptr = c_ptr(GList);
  extern type GArrowFieldClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFieldClass_slistautoptr = c_ptr(GSList);
  extern type GArrowField_autoptr = c_ptr(GArrowField);
  extern type GArrowField_listautoptr = c_ptr(GList);
  extern type GArrowField_queueautoptr = c_ptr(GQueue);
  extern type GArrowField_slistautoptr = c_ptr(GSList);
  // Opaque struct?
  extern type GArrowFileInfo = _GArrowFileInfo;
  extern type GArrowFileInfoClass = _GArrowFileInfoClass;
  extern type GArrowFileInfoClass_autoptr = c_ptr(GArrowFileInfoClass);
  extern type GArrowFileInfoClass_listautoptr = c_ptr(GList);
  extern type GArrowFileInfoClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFileInfoClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFileInfo_autoptr = c_ptr(GArrowFileInfo);
  extern type GArrowFileInfo_listautoptr = c_ptr(GList);
  extern type GArrowFileInfo_queueautoptr = c_ptr(GQueue);
  extern type GArrowFileInfo_slistautoptr = c_ptr(GSList);
  // Opaque struct?
  // GArrowFileMode enum
  extern type GArrowFileMode = c_int;
  extern const GARROW_FILE_MODE_READ :GArrowFileMode;
  extern const GARROW_FILE_MODE_WRITE :GArrowFileMode;
  extern const GARROW_FILE_MODE_READWRITE :GArrowFileMode;
  extern type GArrowFileOutputStream = _GArrowFileOutputStream;
  extern type GArrowFileOutputStreamClass = _GArrowFileOutputStreamClass;
  extern type GArrowFileSelector = _GArrowFileSelector;
  extern type GArrowFileSelectorClass = _GArrowFileSelectorClass;
  extern type GArrowFileSelectorClass_autoptr = c_ptr(GArrowFileSelectorClass);
  extern type GArrowFileSelectorClass_listautoptr = c_ptr(GList);
  extern type GArrowFileSelectorClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFileSelectorClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFileSelector_autoptr = c_ptr(GArrowFileSelector);
  extern type GArrowFileSelector_listautoptr = c_ptr(GList);
  extern type GArrowFileSelector_queueautoptr = c_ptr(GQueue);
  extern type GArrowFileSelector_slistautoptr = c_ptr(GSList);
  extern type GArrowFileSystem = _GArrowFileSystem;
  extern type GArrowFileSystemClass = _GArrowFileSystemClass;
  extern type GArrowFileSystemClass_autoptr = c_ptr(GArrowFileSystemClass);
  extern type GArrowFileSystemClass_listautoptr = c_ptr(GList);
  extern type GArrowFileSystemClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFileSystemClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFileSystem_autoptr = c_ptr(GArrowFileSystem);
  extern type GArrowFileSystem_listautoptr = c_ptr(GList);
  extern type GArrowFileSystem_queueautoptr = c_ptr(GQueue);
  extern type GArrowFileSystem_slistautoptr = c_ptr(GSList);
  // GArrowFileType enum
  extern type GArrowFileType = c_int;
  extern const GARROW_FILE_TYPE_NOT_FOUND :GArrowFileType;
  extern const GARROW_FILE_TYPE_UNKNOWN :GArrowFileType;
  extern const GARROW_FILE_TYPE_FILE :GArrowFileType;
  extern const GARROW_FILE_TYPE_DIR :GArrowFileType;
  extern type GArrowFile_autoptr = c_ptr(GArrowFile);
  extern type GArrowFile_listautoptr = c_ptr(GList);
  extern type GArrowFile_queueautoptr = c_ptr(GQueue);
  extern type GArrowFile_slistautoptr = c_ptr(GSList);
  // GArrowFilterNullSelectionBehavior enum
  extern type GArrowFilterNullSelectionBehavior = c_int;
  extern const GARROW_FILTER_NULL_SELECTION_DROP :GArrowFilterNullSelectionBehavior;
  extern const GARROW_FILTER_NULL_SELECTION_EMIT_NULL :GArrowFilterNullSelectionBehavior;
  extern type GArrowFilterOptions = _GArrowFilterOptions;
  extern type GArrowFilterOptionsClass = _GArrowFilterOptionsClass;
  extern type GArrowFilterOptionsClass_autoptr = c_ptr(GArrowFilterOptionsClass);
  extern type GArrowFilterOptionsClass_listautoptr = c_ptr(GList);
  extern type GArrowFilterOptionsClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFilterOptionsClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFilterOptions_autoptr = c_ptr(GArrowFilterOptions);
  extern type GArrowFilterOptions_listautoptr = c_ptr(GList);
  extern type GArrowFilterOptions_queueautoptr = c_ptr(GQueue);
  extern type GArrowFilterOptions_slistautoptr = c_ptr(GSList);
  extern type GArrowFixedSizeBinaryArray = _GArrowFixedSizeBinaryArray;
  extern type GArrowFixedSizeBinaryArrayBuilder = _GArrowFixedSizeBinaryArrayBuilder;
  extern type GArrowFixedSizeBinaryArrayBuilderClass = _GArrowFixedSizeBinaryArrayBuilderClass;
  extern type GArrowFixedSizeBinaryArrayBuilderClass_autoptr = c_ptr(GArrowFixedSizeBinaryArrayBuilderClass);
  extern type GArrowFixedSizeBinaryArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowFixedSizeBinaryArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFixedSizeBinaryArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFixedSizeBinaryArrayBuilder_autoptr = c_ptr(GArrowFixedSizeBinaryArrayBuilder);
  extern type GArrowFixedSizeBinaryArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowFixedSizeBinaryArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowFixedSizeBinaryArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowFixedSizeBinaryArrayClass = _GArrowFixedSizeBinaryArrayClass;
  extern type GArrowFixedSizeBinaryArrayClass_autoptr = c_ptr(GArrowFixedSizeBinaryArrayClass);
  extern type GArrowFixedSizeBinaryArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowFixedSizeBinaryArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFixedSizeBinaryArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFixedSizeBinaryArray_autoptr = c_ptr(GArrowFixedSizeBinaryArray);
  extern type GArrowFixedSizeBinaryArray_listautoptr = c_ptr(GList);
  extern type GArrowFixedSizeBinaryArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowFixedSizeBinaryArray_slistautoptr = c_ptr(GSList);
  extern type GArrowFixedSizeBinaryDataType = _GArrowFixedSizeBinaryDataType;
  extern type GArrowFixedSizeBinaryDataTypeClass = _GArrowFixedSizeBinaryDataTypeClass;
  extern type GArrowFixedSizeBinaryDataTypeClass_autoptr = c_ptr(GArrowFixedSizeBinaryDataTypeClass);
  extern type GArrowFixedSizeBinaryDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowFixedSizeBinaryDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFixedSizeBinaryDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFixedSizeBinaryDataType_autoptr = c_ptr(GArrowFixedSizeBinaryDataType);
  extern type GArrowFixedSizeBinaryDataType_listautoptr = c_ptr(GList);
  extern type GArrowFixedSizeBinaryDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowFixedSizeBinaryDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowFixedSizeBinaryScalar = _GArrowFixedSizeBinaryScalar;
  extern type GArrowFixedSizeBinaryScalarClass = _GArrowFixedSizeBinaryScalarClass;
  extern type GArrowFixedSizeBinaryScalarClass_autoptr = c_ptr(GArrowFixedSizeBinaryScalarClass);
  extern type GArrowFixedSizeBinaryScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowFixedSizeBinaryScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFixedSizeBinaryScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFixedSizeBinaryScalar_autoptr = c_ptr(GArrowFixedSizeBinaryScalar);
  extern type GArrowFixedSizeBinaryScalar_listautoptr = c_ptr(GList);
  extern type GArrowFixedSizeBinaryScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowFixedSizeBinaryScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowFixedWidthDataType = _GArrowFixedWidthDataType;
  extern type GArrowFixedWidthDataTypeClass = _GArrowFixedWidthDataTypeClass;
  extern type GArrowFixedWidthDataTypeClass_autoptr = c_ptr(GArrowFixedWidthDataTypeClass);
  extern type GArrowFixedWidthDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowFixedWidthDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFixedWidthDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFixedWidthDataType_autoptr = c_ptr(GArrowFixedWidthDataType);
  extern type GArrowFixedWidthDataType_listautoptr = c_ptr(GList);
  extern type GArrowFixedWidthDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowFixedWidthDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowFloatArray = _GArrowFloatArray;
  extern type GArrowFloatArrayBuilder = _GArrowFloatArrayBuilder;
  extern type GArrowFloatArrayBuilderClass = _GArrowFloatArrayBuilderClass;
  extern type GArrowFloatArrayBuilderClass_autoptr = c_ptr(GArrowFloatArrayBuilderClass);
  extern type GArrowFloatArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowFloatArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFloatArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFloatArrayBuilder_autoptr = c_ptr(GArrowFloatArrayBuilder);
  extern type GArrowFloatArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowFloatArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowFloatArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowFloatArrayClass = _GArrowFloatArrayClass;
  extern type GArrowFloatArrayClass_autoptr = c_ptr(GArrowFloatArrayClass);
  extern type GArrowFloatArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowFloatArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFloatArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFloatArray_autoptr = c_ptr(GArrowFloatArray);
  extern type GArrowFloatArray_listautoptr = c_ptr(GList);
  extern type GArrowFloatArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowFloatArray_slistautoptr = c_ptr(GSList);
  extern type GArrowFloatDataType = _GArrowFloatDataType;
  extern type GArrowFloatDataTypeClass = _GArrowFloatDataTypeClass;
  extern type GArrowFloatDataTypeClass_autoptr = c_ptr(GArrowFloatDataTypeClass);
  extern type GArrowFloatDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowFloatDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFloatDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFloatDataType_autoptr = c_ptr(GArrowFloatDataType);
  extern type GArrowFloatDataType_listautoptr = c_ptr(GList);
  extern type GArrowFloatDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowFloatDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowFloatScalar = _GArrowFloatScalar;
  extern type GArrowFloatScalarClass = _GArrowFloatScalarClass;
  extern type GArrowFloatScalarClass_autoptr = c_ptr(GArrowFloatScalarClass);
  extern type GArrowFloatScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowFloatScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFloatScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFloatScalar_autoptr = c_ptr(GArrowFloatScalar);
  extern type GArrowFloatScalar_listautoptr = c_ptr(GList);
  extern type GArrowFloatScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowFloatScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowFloatingPointDataType = _GArrowFloatingPointDataType;
  extern type GArrowFloatingPointDataTypeClass = _GArrowFloatingPointDataTypeClass;
  extern type GArrowFloatingPointDataTypeClass_autoptr = c_ptr(GArrowFloatingPointDataTypeClass);
  extern type GArrowFloatingPointDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowFloatingPointDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFloatingPointDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowFloatingPointDataType_autoptr = c_ptr(GArrowFloatingPointDataType);
  extern type GArrowFloatingPointDataType_listautoptr = c_ptr(GList);
  extern type GArrowFloatingPointDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowFloatingPointDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowFunction = _GArrowFunction;
  extern type GArrowFunctionClass = _GArrowFunctionClass;
  extern type GArrowFunctionClass_autoptr = c_ptr(GArrowFunctionClass);
  extern type GArrowFunctionClass_listautoptr = c_ptr(GList);
  extern type GArrowFunctionClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowFunctionClass_slistautoptr = c_ptr(GSList);
  // Opaque struct?
  // Opaque struct?
  extern type GArrowFunctionOptions_autoptr = c_ptr(GArrowFunctionOptions);
  extern type GArrowFunctionOptions_listautoptr = c_ptr(GList);
  extern type GArrowFunctionOptions_queueautoptr = c_ptr(GQueue);
  extern type GArrowFunctionOptions_slistautoptr = c_ptr(GSList);
  extern type GArrowFunction_autoptr = c_ptr(GArrowFunction);
  extern type GArrowFunction_listautoptr = c_ptr(GList);
  extern type GArrowFunction_queueautoptr = c_ptr(GQueue);
  extern type GArrowFunction_slistautoptr = c_ptr(GSList);
  extern type GArrowGIOInputStream = _GArrowGIOInputStream;
  extern type GArrowGIOInputStreamClass = _GArrowGIOInputStreamClass;
  extern type GArrowGIOOutputStream = _GArrowGIOOutputStream;
  extern type GArrowGIOOutputStreamClass = _GArrowGIOOutputStreamClass;
  extern type GArrowHDFSFileSystem = _GArrowHDFSFileSystem;
  extern type GArrowHDFSFileSystemClass = _GArrowHDFSFileSystemClass;
  extern type GArrowHDFSFileSystemClass_autoptr = c_ptr(GArrowHDFSFileSystemClass);
  extern type GArrowHDFSFileSystemClass_listautoptr = c_ptr(GList);
  extern type GArrowHDFSFileSystemClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowHDFSFileSystemClass_slistautoptr = c_ptr(GSList);
  extern type GArrowHDFSFileSystem_autoptr = c_ptr(GArrowHDFSFileSystem);
  extern type GArrowHDFSFileSystem_listautoptr = c_ptr(GList);
  extern type GArrowHDFSFileSystem_queueautoptr = c_ptr(GQueue);
  extern type GArrowHDFSFileSystem_slistautoptr = c_ptr(GSList);
  extern type GArrowInputStream = _GArrowInputStream;
  extern type GArrowInputStreamClass = _GArrowInputStreamClass;
  extern type GArrowInputStreamClass_autoptr = c_ptr(GArrowInputStreamClass);
  extern type GArrowInputStreamClass_listautoptr = c_ptr(GList);
  extern type GArrowInputStreamClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInputStreamClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInputStream_autoptr = c_ptr(GArrowInputStream);
  extern type GArrowInputStream_listautoptr = c_ptr(GList);
  extern type GArrowInputStream_queueautoptr = c_ptr(GQueue);
  extern type GArrowInputStream_slistautoptr = c_ptr(GSList);
  extern type GArrowInt16Array = _GArrowInt16Array;
  extern type GArrowInt16ArrayBuilder = _GArrowInt16ArrayBuilder;
  extern type GArrowInt16ArrayBuilderClass = _GArrowInt16ArrayBuilderClass;
  extern type GArrowInt16ArrayBuilderClass_autoptr = c_ptr(GArrowInt16ArrayBuilderClass);
  extern type GArrowInt16ArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowInt16ArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt16ArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt16ArrayBuilder_autoptr = c_ptr(GArrowInt16ArrayBuilder);
  extern type GArrowInt16ArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowInt16ArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt16ArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowInt16ArrayClass = _GArrowInt16ArrayClass;
  extern type GArrowInt16ArrayClass_autoptr = c_ptr(GArrowInt16ArrayClass);
  extern type GArrowInt16ArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowInt16ArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt16ArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt16Array_autoptr = c_ptr(GArrowInt16Array);
  extern type GArrowInt16Array_listautoptr = c_ptr(GList);
  extern type GArrowInt16Array_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt16Array_slistautoptr = c_ptr(GSList);
  extern type GArrowInt16DataType = _GArrowInt16DataType;
  extern type GArrowInt16DataTypeClass = _GArrowInt16DataTypeClass;
  extern type GArrowInt16DataTypeClass_autoptr = c_ptr(GArrowInt16DataTypeClass);
  extern type GArrowInt16DataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowInt16DataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt16DataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt16DataType_autoptr = c_ptr(GArrowInt16DataType);
  extern type GArrowInt16DataType_listautoptr = c_ptr(GList);
  extern type GArrowInt16DataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt16DataType_slistautoptr = c_ptr(GSList);
  extern type GArrowInt16Scalar = _GArrowInt16Scalar;
  extern type GArrowInt16ScalarClass = _GArrowInt16ScalarClass;
  extern type GArrowInt16ScalarClass_autoptr = c_ptr(GArrowInt16ScalarClass);
  extern type GArrowInt16ScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowInt16ScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt16ScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt16Scalar_autoptr = c_ptr(GArrowInt16Scalar);
  extern type GArrowInt16Scalar_listautoptr = c_ptr(GList);
  extern type GArrowInt16Scalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt16Scalar_slistautoptr = c_ptr(GSList);
  extern type GArrowInt32Array = _GArrowInt32Array;
  extern type GArrowInt32ArrayBuilder = _GArrowInt32ArrayBuilder;
  extern type GArrowInt32ArrayBuilderClass = _GArrowInt32ArrayBuilderClass;
  extern type GArrowInt32ArrayBuilderClass_autoptr = c_ptr(GArrowInt32ArrayBuilderClass);
  extern type GArrowInt32ArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowInt32ArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt32ArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt32ArrayBuilder_autoptr = c_ptr(GArrowInt32ArrayBuilder);
  extern type GArrowInt32ArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowInt32ArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt32ArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowInt32ArrayClass = _GArrowInt32ArrayClass;
  extern type GArrowInt32ArrayClass_autoptr = c_ptr(GArrowInt32ArrayClass);
  extern type GArrowInt32ArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowInt32ArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt32ArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt32Array_autoptr = c_ptr(GArrowInt32Array);
  extern type GArrowInt32Array_listautoptr = c_ptr(GList);
  extern type GArrowInt32Array_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt32Array_slistautoptr = c_ptr(GSList);
  extern type GArrowInt32DataType = _GArrowInt32DataType;
  extern type GArrowInt32DataTypeClass = _GArrowInt32DataTypeClass;
  extern type GArrowInt32DataTypeClass_autoptr = c_ptr(GArrowInt32DataTypeClass);
  extern type GArrowInt32DataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowInt32DataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt32DataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt32DataType_autoptr = c_ptr(GArrowInt32DataType);
  extern type GArrowInt32DataType_listautoptr = c_ptr(GList);
  extern type GArrowInt32DataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt32DataType_slistautoptr = c_ptr(GSList);
  extern type GArrowInt32Scalar = _GArrowInt32Scalar;
  extern type GArrowInt32ScalarClass = _GArrowInt32ScalarClass;
  extern type GArrowInt32ScalarClass_autoptr = c_ptr(GArrowInt32ScalarClass);
  extern type GArrowInt32ScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowInt32ScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt32ScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt32Scalar_autoptr = c_ptr(GArrowInt32Scalar);
  extern type GArrowInt32Scalar_listautoptr = c_ptr(GList);
  extern type GArrowInt32Scalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt32Scalar_slistautoptr = c_ptr(GSList);
  extern type GArrowInt64Array = _GArrowInt64Array;
  extern type GArrowInt64ArrayBuilder = _GArrowInt64ArrayBuilder;
  extern type GArrowInt64ArrayBuilderClass = _GArrowInt64ArrayBuilderClass;
  extern type GArrowInt64ArrayBuilderClass_autoptr = c_ptr(GArrowInt64ArrayBuilderClass);
  extern type GArrowInt64ArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowInt64ArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt64ArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt64ArrayBuilder_autoptr = c_ptr(GArrowInt64ArrayBuilder);
  extern type GArrowInt64ArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowInt64ArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt64ArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowInt64ArrayClass = _GArrowInt64ArrayClass;
  extern type GArrowInt64ArrayClass_autoptr = c_ptr(GArrowInt64ArrayClass);
  extern type GArrowInt64ArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowInt64ArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt64ArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt64Array_autoptr = c_ptr(GArrowInt64Array);
  extern type GArrowInt64Array_listautoptr = c_ptr(GList);
  extern type GArrowInt64Array_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt64Array_slistautoptr = c_ptr(GSList);
  extern type GArrowInt64DataType = _GArrowInt64DataType;
  extern type GArrowInt64DataTypeClass = _GArrowInt64DataTypeClass;
  extern type GArrowInt64DataTypeClass_autoptr = c_ptr(GArrowInt64DataTypeClass);
  extern type GArrowInt64DataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowInt64DataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt64DataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt64DataType_autoptr = c_ptr(GArrowInt64DataType);
  extern type GArrowInt64DataType_listautoptr = c_ptr(GList);
  extern type GArrowInt64DataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt64DataType_slistautoptr = c_ptr(GSList);
  extern type GArrowInt64Scalar = _GArrowInt64Scalar;
  extern type GArrowInt64ScalarClass = _GArrowInt64ScalarClass;
  extern type GArrowInt64ScalarClass_autoptr = c_ptr(GArrowInt64ScalarClass);
  extern type GArrowInt64ScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowInt64ScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt64ScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt64Scalar_autoptr = c_ptr(GArrowInt64Scalar);
  extern type GArrowInt64Scalar_listautoptr = c_ptr(GList);
  extern type GArrowInt64Scalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt64Scalar_slistautoptr = c_ptr(GSList);
  extern type GArrowInt8Array = _GArrowInt8Array;
  extern type GArrowInt8ArrayBuilder = _GArrowInt8ArrayBuilder;
  extern type GArrowInt8ArrayBuilderClass = _GArrowInt8ArrayBuilderClass;
  extern type GArrowInt8ArrayBuilderClass_autoptr = c_ptr(GArrowInt8ArrayBuilderClass);
  extern type GArrowInt8ArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowInt8ArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt8ArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt8ArrayBuilder_autoptr = c_ptr(GArrowInt8ArrayBuilder);
  extern type GArrowInt8ArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowInt8ArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt8ArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowInt8ArrayClass = _GArrowInt8ArrayClass;
  extern type GArrowInt8ArrayClass_autoptr = c_ptr(GArrowInt8ArrayClass);
  extern type GArrowInt8ArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowInt8ArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt8ArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt8Array_autoptr = c_ptr(GArrowInt8Array);
  extern type GArrowInt8Array_listautoptr = c_ptr(GList);
  extern type GArrowInt8Array_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt8Array_slistautoptr = c_ptr(GSList);
  extern type GArrowInt8DataType = _GArrowInt8DataType;
  extern type GArrowInt8DataTypeClass = _GArrowInt8DataTypeClass;
  extern type GArrowInt8DataTypeClass_autoptr = c_ptr(GArrowInt8DataTypeClass);
  extern type GArrowInt8DataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowInt8DataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt8DataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt8DataType_autoptr = c_ptr(GArrowInt8DataType);
  extern type GArrowInt8DataType_listautoptr = c_ptr(GList);
  extern type GArrowInt8DataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt8DataType_slistautoptr = c_ptr(GSList);
  extern type GArrowInt8Scalar = _GArrowInt8Scalar;
  extern type GArrowInt8ScalarClass = _GArrowInt8ScalarClass;
  extern type GArrowInt8ScalarClass_autoptr = c_ptr(GArrowInt8ScalarClass);
  extern type GArrowInt8ScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowInt8ScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt8ScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowInt8Scalar_autoptr = c_ptr(GArrowInt8Scalar);
  extern type GArrowInt8Scalar_listautoptr = c_ptr(GList);
  extern type GArrowInt8Scalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowInt8Scalar_slistautoptr = c_ptr(GSList);
  extern type GArrowIntArrayBuilder = _GArrowIntArrayBuilder;
  extern type GArrowIntArrayBuilderClass = _GArrowIntArrayBuilderClass;
  extern type GArrowIntArrayBuilderClass_autoptr = c_ptr(GArrowIntArrayBuilderClass);
  extern type GArrowIntArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowIntArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowIntArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowIntArrayBuilder_autoptr = c_ptr(GArrowIntArrayBuilder);
  extern type GArrowIntArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowIntArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowIntArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowIntegerDataType = _GArrowIntegerDataType;
  extern type GArrowIntegerDataTypeClass = _GArrowIntegerDataTypeClass;
  extern type GArrowIntegerDataTypeClass_autoptr = c_ptr(GArrowIntegerDataTypeClass);
  extern type GArrowIntegerDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowIntegerDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowIntegerDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowIntegerDataType_autoptr = c_ptr(GArrowIntegerDataType);
  extern type GArrowIntegerDataType_listautoptr = c_ptr(GList);
  extern type GArrowIntegerDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowIntegerDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowJSONReadOptions = _GArrowJSONReadOptions;
  extern type GArrowJSONReadOptionsClass = _GArrowJSONReadOptionsClass;
  extern type GArrowJSONReadOptionsClass_autoptr = c_ptr(GArrowJSONReadOptionsClass);
  extern type GArrowJSONReadOptionsClass_listautoptr = c_ptr(GList);
  extern type GArrowJSONReadOptionsClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowJSONReadOptionsClass_slistautoptr = c_ptr(GSList);
  extern type GArrowJSONReadOptions_autoptr = c_ptr(GArrowJSONReadOptions);
  extern type GArrowJSONReadOptions_listautoptr = c_ptr(GList);
  extern type GArrowJSONReadOptions_queueautoptr = c_ptr(GQueue);
  extern type GArrowJSONReadOptions_slistautoptr = c_ptr(GSList);
  // GArrowJSONReadUnexpectedFieldBehavior enum
  extern type GArrowJSONReadUnexpectedFieldBehavior = c_int;
  extern const GARROW_JSON_READ_IGNORE :GArrowJSONReadUnexpectedFieldBehavior;
  extern const GARROW_JSON_READ_ERROR :GArrowJSONReadUnexpectedFieldBehavior;
  extern const GARROW_JSON_READ_INFER_TYPE :GArrowJSONReadUnexpectedFieldBehavior;
  extern type GArrowJSONReader = _GArrowJSONReader;
  extern type GArrowJSONReaderClass = _GArrowJSONReaderClass;
  extern type GArrowJSONReaderClass_autoptr = c_ptr(GArrowJSONReaderClass);
  extern type GArrowJSONReaderClass_listautoptr = c_ptr(GList);
  extern type GArrowJSONReaderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowJSONReaderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowJSONReader_autoptr = c_ptr(GArrowJSONReader);
  extern type GArrowJSONReader_listautoptr = c_ptr(GList);
  extern type GArrowJSONReader_queueautoptr = c_ptr(GQueue);
  extern type GArrowJSONReader_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeBinaryArray = _GArrowLargeBinaryArray;
  extern type GArrowLargeBinaryArrayBuilder = _GArrowLargeBinaryArrayBuilder;
  extern type GArrowLargeBinaryArrayBuilderClass = _GArrowLargeBinaryArrayBuilderClass;
  extern type GArrowLargeBinaryArrayBuilderClass_autoptr = c_ptr(GArrowLargeBinaryArrayBuilderClass);
  extern type GArrowLargeBinaryArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowLargeBinaryArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeBinaryArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeBinaryArrayBuilder_autoptr = c_ptr(GArrowLargeBinaryArrayBuilder);
  extern type GArrowLargeBinaryArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowLargeBinaryArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeBinaryArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeBinaryArrayClass = _GArrowLargeBinaryArrayClass;
  extern type GArrowLargeBinaryArrayClass_autoptr = c_ptr(GArrowLargeBinaryArrayClass);
  extern type GArrowLargeBinaryArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowLargeBinaryArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeBinaryArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeBinaryArray_autoptr = c_ptr(GArrowLargeBinaryArray);
  extern type GArrowLargeBinaryArray_listautoptr = c_ptr(GList);
  extern type GArrowLargeBinaryArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeBinaryArray_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeBinaryDataType = _GArrowLargeBinaryDataType;
  extern type GArrowLargeBinaryDataTypeClass = _GArrowLargeBinaryDataTypeClass;
  extern type GArrowLargeBinaryDataTypeClass_autoptr = c_ptr(GArrowLargeBinaryDataTypeClass);
  extern type GArrowLargeBinaryDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowLargeBinaryDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeBinaryDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeBinaryDataType_autoptr = c_ptr(GArrowLargeBinaryDataType);
  extern type GArrowLargeBinaryDataType_listautoptr = c_ptr(GList);
  extern type GArrowLargeBinaryDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeBinaryDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeBinaryScalar = _GArrowLargeBinaryScalar;
  extern type GArrowLargeBinaryScalarClass = _GArrowLargeBinaryScalarClass;
  extern type GArrowLargeBinaryScalarClass_autoptr = c_ptr(GArrowLargeBinaryScalarClass);
  extern type GArrowLargeBinaryScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowLargeBinaryScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeBinaryScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeBinaryScalar_autoptr = c_ptr(GArrowLargeBinaryScalar);
  extern type GArrowLargeBinaryScalar_listautoptr = c_ptr(GList);
  extern type GArrowLargeBinaryScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeBinaryScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeListArray = _GArrowLargeListArray;
  extern type GArrowLargeListArrayBuilder = _GArrowLargeListArrayBuilder;
  extern type GArrowLargeListArrayBuilderClass = _GArrowLargeListArrayBuilderClass;
  extern type GArrowLargeListArrayBuilderClass_autoptr = c_ptr(GArrowLargeListArrayBuilderClass);
  extern type GArrowLargeListArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowLargeListArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeListArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeListArrayBuilder_autoptr = c_ptr(GArrowLargeListArrayBuilder);
  extern type GArrowLargeListArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowLargeListArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeListArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeListArrayClass = _GArrowLargeListArrayClass;
  extern type GArrowLargeListArrayClass_autoptr = c_ptr(GArrowLargeListArrayClass);
  extern type GArrowLargeListArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowLargeListArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeListArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeListArray_autoptr = c_ptr(GArrowLargeListArray);
  extern type GArrowLargeListArray_listautoptr = c_ptr(GList);
  extern type GArrowLargeListArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeListArray_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeListDataType = _GArrowLargeListDataType;
  extern type GArrowLargeListDataTypeClass = _GArrowLargeListDataTypeClass;
  extern type GArrowLargeListDataTypeClass_autoptr = c_ptr(GArrowLargeListDataTypeClass);
  extern type GArrowLargeListDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowLargeListDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeListDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeListDataType_autoptr = c_ptr(GArrowLargeListDataType);
  extern type GArrowLargeListDataType_listautoptr = c_ptr(GList);
  extern type GArrowLargeListDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeListDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeListScalar = _GArrowLargeListScalar;
  extern type GArrowLargeListScalarClass = _GArrowLargeListScalarClass;
  extern type GArrowLargeListScalarClass_autoptr = c_ptr(GArrowLargeListScalarClass);
  extern type GArrowLargeListScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowLargeListScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeListScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeListScalar_autoptr = c_ptr(GArrowLargeListScalar);
  extern type GArrowLargeListScalar_listautoptr = c_ptr(GList);
  extern type GArrowLargeListScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeListScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeStringArray = _GArrowLargeStringArray;
  extern type GArrowLargeStringArrayBuilder = _GArrowLargeStringArrayBuilder;
  extern type GArrowLargeStringArrayBuilderClass = _GArrowLargeStringArrayBuilderClass;
  extern type GArrowLargeStringArrayBuilderClass_autoptr = c_ptr(GArrowLargeStringArrayBuilderClass);
  extern type GArrowLargeStringArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowLargeStringArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeStringArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeStringArrayBuilder_autoptr = c_ptr(GArrowLargeStringArrayBuilder);
  extern type GArrowLargeStringArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowLargeStringArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeStringArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeStringArrayClass = _GArrowLargeStringArrayClass;
  extern type GArrowLargeStringArrayClass_autoptr = c_ptr(GArrowLargeStringArrayClass);
  extern type GArrowLargeStringArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowLargeStringArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeStringArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeStringArray_autoptr = c_ptr(GArrowLargeStringArray);
  extern type GArrowLargeStringArray_listautoptr = c_ptr(GList);
  extern type GArrowLargeStringArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeStringArray_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeStringDataType = _GArrowLargeStringDataType;
  extern type GArrowLargeStringDataTypeClass = _GArrowLargeStringDataTypeClass;
  extern type GArrowLargeStringDataTypeClass_autoptr = c_ptr(GArrowLargeStringDataTypeClass);
  extern type GArrowLargeStringDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowLargeStringDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeStringDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeStringDataType_autoptr = c_ptr(GArrowLargeStringDataType);
  extern type GArrowLargeStringDataType_listautoptr = c_ptr(GList);
  extern type GArrowLargeStringDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeStringDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeStringScalar = _GArrowLargeStringScalar;
  extern type GArrowLargeStringScalarClass = _GArrowLargeStringScalarClass;
  extern type GArrowLargeStringScalarClass_autoptr = c_ptr(GArrowLargeStringScalarClass);
  extern type GArrowLargeStringScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowLargeStringScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeStringScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowLargeStringScalar_autoptr = c_ptr(GArrowLargeStringScalar);
  extern type GArrowLargeStringScalar_listautoptr = c_ptr(GList);
  extern type GArrowLargeStringScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowLargeStringScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowListArray = _GArrowListArray;
  extern type GArrowListArrayBuilder = _GArrowListArrayBuilder;
  extern type GArrowListArrayBuilderClass = _GArrowListArrayBuilderClass;
  extern type GArrowListArrayBuilderClass_autoptr = c_ptr(GArrowListArrayBuilderClass);
  extern type GArrowListArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowListArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowListArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowListArrayBuilder_autoptr = c_ptr(GArrowListArrayBuilder);
  extern type GArrowListArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowListArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowListArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowListArrayClass = _GArrowListArrayClass;
  extern type GArrowListArrayClass_autoptr = c_ptr(GArrowListArrayClass);
  extern type GArrowListArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowListArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowListArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowListArray_autoptr = c_ptr(GArrowListArray);
  extern type GArrowListArray_listautoptr = c_ptr(GList);
  extern type GArrowListArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowListArray_slistautoptr = c_ptr(GSList);
  extern type GArrowListDataType = _GArrowListDataType;
  extern type GArrowListDataTypeClass = _GArrowListDataTypeClass;
  extern type GArrowListDataTypeClass_autoptr = c_ptr(GArrowListDataTypeClass);
  extern type GArrowListDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowListDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowListDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowListDataType_autoptr = c_ptr(GArrowListDataType);
  extern type GArrowListDataType_listautoptr = c_ptr(GList);
  extern type GArrowListDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowListDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowListScalar = _GArrowListScalar;
  extern type GArrowListScalarClass = _GArrowListScalarClass;
  extern type GArrowListScalarClass_autoptr = c_ptr(GArrowListScalarClass);
  extern type GArrowListScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowListScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowListScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowListScalar_autoptr = c_ptr(GArrowListScalar);
  extern type GArrowListScalar_listautoptr = c_ptr(GList);
  extern type GArrowListScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowListScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowLocalFileSystem = _GArrowLocalFileSystem;
  extern type GArrowLocalFileSystemClass = _GArrowLocalFileSystemClass;
  extern type GArrowLocalFileSystemClass_autoptr = c_ptr(GArrowLocalFileSystemClass);
  extern type GArrowLocalFileSystemClass_listautoptr = c_ptr(GList);
  extern type GArrowLocalFileSystemClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowLocalFileSystemClass_slistautoptr = c_ptr(GSList);
  extern type GArrowLocalFileSystemOptions = _GArrowLocalFileSystemOptions;
  extern type GArrowLocalFileSystemOptionsClass = _GArrowLocalFileSystemOptionsClass;
  extern type GArrowLocalFileSystemOptionsClass_autoptr = c_ptr(GArrowLocalFileSystemOptionsClass);
  extern type GArrowLocalFileSystemOptionsClass_listautoptr = c_ptr(GList);
  extern type GArrowLocalFileSystemOptionsClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowLocalFileSystemOptionsClass_slistautoptr = c_ptr(GSList);
  extern type GArrowLocalFileSystemOptions_autoptr = c_ptr(GArrowLocalFileSystemOptions);
  extern type GArrowLocalFileSystemOptions_listautoptr = c_ptr(GList);
  extern type GArrowLocalFileSystemOptions_queueautoptr = c_ptr(GQueue);
  extern type GArrowLocalFileSystemOptions_slistautoptr = c_ptr(GSList);
  extern type GArrowLocalFileSystem_autoptr = c_ptr(GArrowLocalFileSystem);
  extern type GArrowLocalFileSystem_listautoptr = c_ptr(GList);
  extern type GArrowLocalFileSystem_queueautoptr = c_ptr(GQueue);
  extern type GArrowLocalFileSystem_slistautoptr = c_ptr(GSList);
  extern type GArrowMapArray = _GArrowMapArray;
  extern type GArrowMapArrayBuilder = _GArrowMapArrayBuilder;
  extern type GArrowMapArrayBuilderClass = _GArrowMapArrayBuilderClass;
  extern type GArrowMapArrayBuilderClass_autoptr = c_ptr(GArrowMapArrayBuilderClass);
  extern type GArrowMapArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowMapArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowMapArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowMapArrayBuilder_autoptr = c_ptr(GArrowMapArrayBuilder);
  extern type GArrowMapArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowMapArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowMapArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowMapArrayClass = _GArrowMapArrayClass;
  extern type GArrowMapArrayClass_autoptr = c_ptr(GArrowMapArrayClass);
  extern type GArrowMapArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowMapArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowMapArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowMapArray_autoptr = c_ptr(GArrowMapArray);
  extern type GArrowMapArray_listautoptr = c_ptr(GList);
  extern type GArrowMapArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowMapArray_slistautoptr = c_ptr(GSList);
  extern type GArrowMapDataType = _GArrowMapDataType;
  extern type GArrowMapDataTypeClass = _GArrowMapDataTypeClass;
  extern type GArrowMapDataTypeClass_autoptr = c_ptr(GArrowMapDataTypeClass);
  extern type GArrowMapDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowMapDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowMapDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowMapDataType_autoptr = c_ptr(GArrowMapDataType);
  extern type GArrowMapDataType_listautoptr = c_ptr(GList);
  extern type GArrowMapDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowMapDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowMapScalar = _GArrowMapScalar;
  extern type GArrowMapScalarClass = _GArrowMapScalarClass;
  extern type GArrowMapScalarClass_autoptr = c_ptr(GArrowMapScalarClass);
  extern type GArrowMapScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowMapScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowMapScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowMapScalar_autoptr = c_ptr(GArrowMapScalar);
  extern type GArrowMapScalar_listautoptr = c_ptr(GList);
  extern type GArrowMapScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowMapScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowMemoryMappedInputStream = _GArrowMemoryMappedInputStream;
  extern type GArrowMemoryMappedInputStreamClass = _GArrowMemoryMappedInputStreamClass;
  // GArrowMetadataVersion enum
  extern type GArrowMetadataVersion = c_int;
  extern const GARROW_METADATA_VERSION_V1 :GArrowMetadataVersion;
  extern const GARROW_METADATA_VERSION_V2 :GArrowMetadataVersion;
  extern const GARROW_METADATA_VERSION_V3 :GArrowMetadataVersion;
  extern type GArrowMockFileSystem = _GArrowMockFileSystem;
  extern type GArrowMockFileSystemClass = _GArrowMockFileSystemClass;
  extern type GArrowMockFileSystemClass_autoptr = c_ptr(GArrowMockFileSystemClass);
  extern type GArrowMockFileSystemClass_listautoptr = c_ptr(GList);
  extern type GArrowMockFileSystemClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowMockFileSystemClass_slistautoptr = c_ptr(GSList);
  extern type GArrowMockFileSystem_autoptr = c_ptr(GArrowMockFileSystem);
  extern type GArrowMockFileSystem_listautoptr = c_ptr(GList);
  extern type GArrowMockFileSystem_queueautoptr = c_ptr(GQueue);
  extern type GArrowMockFileSystem_slistautoptr = c_ptr(GSList);
  extern type GArrowMutableBuffer = _GArrowMutableBuffer;
  extern type GArrowMutableBufferClass = _GArrowMutableBufferClass;
  extern type GArrowMutableBufferClass_autoptr = c_ptr(GArrowMutableBufferClass);
  extern type GArrowMutableBufferClass_listautoptr = c_ptr(GList);
  extern type GArrowMutableBufferClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowMutableBufferClass_slistautoptr = c_ptr(GSList);
  extern type GArrowMutableBuffer_autoptr = c_ptr(GArrowMutableBuffer);
  extern type GArrowMutableBuffer_listautoptr = c_ptr(GList);
  extern type GArrowMutableBuffer_queueautoptr = c_ptr(GQueue);
  extern type GArrowMutableBuffer_slistautoptr = c_ptr(GSList);
  extern type GArrowNullArray = _GArrowNullArray;
  extern type GArrowNullArrayBuilder = _GArrowNullArrayBuilder;
  extern type GArrowNullArrayBuilderClass = _GArrowNullArrayBuilderClass;
  extern type GArrowNullArrayBuilderClass_autoptr = c_ptr(GArrowNullArrayBuilderClass);
  extern type GArrowNullArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowNullArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowNullArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowNullArrayBuilder_autoptr = c_ptr(GArrowNullArrayBuilder);
  extern type GArrowNullArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowNullArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowNullArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowNullArrayClass = _GArrowNullArrayClass;
  extern type GArrowNullArrayClass_autoptr = c_ptr(GArrowNullArrayClass);
  extern type GArrowNullArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowNullArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowNullArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowNullArray_autoptr = c_ptr(GArrowNullArray);
  extern type GArrowNullArray_listautoptr = c_ptr(GList);
  extern type GArrowNullArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowNullArray_slistautoptr = c_ptr(GSList);
  extern type GArrowNullDataType = _GArrowNullDataType;
  extern type GArrowNullDataTypeClass = _GArrowNullDataTypeClass;
  extern type GArrowNullDataTypeClass_autoptr = c_ptr(GArrowNullDataTypeClass);
  extern type GArrowNullDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowNullDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowNullDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowNullDataType_autoptr = c_ptr(GArrowNullDataType);
  extern type GArrowNullDataType_listautoptr = c_ptr(GList);
  extern type GArrowNullDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowNullDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowNullScalar = _GArrowNullScalar;
  extern type GArrowNullScalarClass = _GArrowNullScalarClass;
  extern type GArrowNullScalarClass_autoptr = c_ptr(GArrowNullScalarClass);
  extern type GArrowNullScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowNullScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowNullScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowNullScalar_autoptr = c_ptr(GArrowNullScalar);
  extern type GArrowNullScalar_listautoptr = c_ptr(GList);
  extern type GArrowNullScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowNullScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowNumericArray = _GArrowNumericArray;
  extern type GArrowNumericArrayClass = _GArrowNumericArrayClass;
  extern type GArrowNumericArrayClass_autoptr = c_ptr(GArrowNumericArrayClass);
  extern type GArrowNumericArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowNumericArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowNumericArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowNumericArray_autoptr = c_ptr(GArrowNumericArray);
  extern type GArrowNumericArray_listautoptr = c_ptr(GList);
  extern type GArrowNumericArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowNumericArray_slistautoptr = c_ptr(GSList);
  extern type GArrowNumericDataType = _GArrowNumericDataType;
  extern type GArrowNumericDataTypeClass = _GArrowNumericDataTypeClass;
  extern type GArrowNumericDataTypeClass_autoptr = c_ptr(GArrowNumericDataTypeClass);
  extern type GArrowNumericDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowNumericDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowNumericDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowNumericDataType_autoptr = c_ptr(GArrowNumericDataType);
  extern type GArrowNumericDataType_listautoptr = c_ptr(GList);
  extern type GArrowNumericDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowNumericDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowOutputStream = _GArrowOutputStream;
  extern type GArrowOutputStreamClass = _GArrowOutputStreamClass;
  extern type GArrowOutputStreamClass_autoptr = c_ptr(GArrowOutputStreamClass);
  extern type GArrowOutputStreamClass_listautoptr = c_ptr(GList);
  extern type GArrowOutputStreamClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowOutputStreamClass_slistautoptr = c_ptr(GSList);
  extern type GArrowOutputStream_autoptr = c_ptr(GArrowOutputStream);
  extern type GArrowOutputStream_listautoptr = c_ptr(GList);
  extern type GArrowOutputStream_queueautoptr = c_ptr(GQueue);
  extern type GArrowOutputStream_slistautoptr = c_ptr(GSList);
  extern type GArrowPrimitiveArray = _GArrowPrimitiveArray;
  extern type GArrowPrimitiveArrayClass = _GArrowPrimitiveArrayClass;
  extern type GArrowPrimitiveArrayClass_autoptr = c_ptr(GArrowPrimitiveArrayClass);
  extern type GArrowPrimitiveArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowPrimitiveArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowPrimitiveArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowPrimitiveArray_autoptr = c_ptr(GArrowPrimitiveArray);
  extern type GArrowPrimitiveArray_listautoptr = c_ptr(GList);
  extern type GArrowPrimitiveArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowPrimitiveArray_slistautoptr = c_ptr(GSList);
  extern type GArrowReadOptions = _GArrowReadOptions;
  extern type GArrowReadOptionsClass = _GArrowReadOptionsClass;
  extern type GArrowReadOptionsClass_autoptr = c_ptr(GArrowReadOptionsClass);
  extern type GArrowReadOptionsClass_listautoptr = c_ptr(GList);
  extern type GArrowReadOptionsClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowReadOptionsClass_slistautoptr = c_ptr(GSList);
  extern type GArrowReadOptions_autoptr = c_ptr(GArrowReadOptions);
  extern type GArrowReadOptions_listautoptr = c_ptr(GList);
  extern type GArrowReadOptions_queueautoptr = c_ptr(GQueue);
  extern type GArrowReadOptions_slistautoptr = c_ptr(GSList);
  // Opaque struct?
  // Opaque struct?
  extern type GArrowReadable_autoptr = c_ptr(GArrowReadable);
  extern type GArrowReadable_listautoptr = c_ptr(GList);
  extern type GArrowReadable_queueautoptr = c_ptr(GQueue);
  extern type GArrowReadable_slistautoptr = c_ptr(GSList);
  extern type GArrowRecordBatch = _GArrowRecordBatch;
  extern type GArrowRecordBatchBuilder = _GArrowRecordBatchBuilder;
  extern type GArrowRecordBatchBuilderClass = _GArrowRecordBatchBuilderClass;
  extern type GArrowRecordBatchBuilderClass_autoptr = c_ptr(GArrowRecordBatchBuilderClass);
  extern type GArrowRecordBatchBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowRecordBatchBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowRecordBatchBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowRecordBatchBuilder_autoptr = c_ptr(GArrowRecordBatchBuilder);
  extern type GArrowRecordBatchBuilder_listautoptr = c_ptr(GList);
  extern type GArrowRecordBatchBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowRecordBatchBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowRecordBatchClass = _GArrowRecordBatchClass;
  extern type GArrowRecordBatchClass_autoptr = c_ptr(GArrowRecordBatchClass);
  extern type GArrowRecordBatchClass_listautoptr = c_ptr(GList);
  extern type GArrowRecordBatchClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowRecordBatchClass_slistautoptr = c_ptr(GSList);
  extern type GArrowRecordBatchDatum = _GArrowRecordBatchDatum;
  extern type GArrowRecordBatchDatumClass = _GArrowRecordBatchDatumClass;
  extern type GArrowRecordBatchDatumClass_autoptr = c_ptr(GArrowRecordBatchDatumClass);
  extern type GArrowRecordBatchDatumClass_listautoptr = c_ptr(GList);
  extern type GArrowRecordBatchDatumClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowRecordBatchDatumClass_slistautoptr = c_ptr(GSList);
  extern type GArrowRecordBatchDatum_autoptr = c_ptr(GArrowRecordBatchDatum);
  extern type GArrowRecordBatchDatum_listautoptr = c_ptr(GList);
  extern type GArrowRecordBatchDatum_queueautoptr = c_ptr(GQueue);
  extern type GArrowRecordBatchDatum_slistautoptr = c_ptr(GSList);
  extern type GArrowRecordBatchFileReader = _GArrowRecordBatchFileReader;
  extern type GArrowRecordBatchFileReaderClass = _GArrowRecordBatchFileReaderClass;
  extern type GArrowRecordBatchFileWriter = _GArrowRecordBatchFileWriter;
  extern type GArrowRecordBatchFileWriterClass = _GArrowRecordBatchFileWriterClass;
  extern type GArrowRecordBatchIterator = _GArrowRecordBatchIterator;
  extern type GArrowRecordBatchIteratorClass = _GArrowRecordBatchIteratorClass;
  extern type GArrowRecordBatchIteratorClass_autoptr = c_ptr(GArrowRecordBatchIteratorClass);
  extern type GArrowRecordBatchIteratorClass_listautoptr = c_ptr(GList);
  extern type GArrowRecordBatchIteratorClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowRecordBatchIteratorClass_slistautoptr = c_ptr(GSList);
  extern type GArrowRecordBatchIterator_autoptr = c_ptr(GArrowRecordBatchIterator);
  extern type GArrowRecordBatchIterator_listautoptr = c_ptr(GList);
  extern type GArrowRecordBatchIterator_queueautoptr = c_ptr(GQueue);
  extern type GArrowRecordBatchIterator_slistautoptr = c_ptr(GSList);
  extern type GArrowRecordBatchReader = _GArrowRecordBatchReader;
  extern type GArrowRecordBatchReaderClass = _GArrowRecordBatchReaderClass;
  extern type GArrowRecordBatchReaderClass_autoptr = c_ptr(GArrowRecordBatchReaderClass);
  extern type GArrowRecordBatchReaderClass_listautoptr = c_ptr(GList);
  extern type GArrowRecordBatchReaderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowRecordBatchReaderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowRecordBatchReader_autoptr = c_ptr(GArrowRecordBatchReader);
  extern type GArrowRecordBatchReader_listautoptr = c_ptr(GList);
  extern type GArrowRecordBatchReader_queueautoptr = c_ptr(GQueue);
  extern type GArrowRecordBatchReader_slistautoptr = c_ptr(GSList);
  extern type GArrowRecordBatchStreamReader = _GArrowRecordBatchStreamReader;
  extern type GArrowRecordBatchStreamReaderClass = _GArrowRecordBatchStreamReaderClass;
  extern type GArrowRecordBatchStreamWriter = _GArrowRecordBatchStreamWriter;
  extern type GArrowRecordBatchStreamWriterClass = _GArrowRecordBatchStreamWriterClass;
  extern type GArrowRecordBatchWriter = _GArrowRecordBatchWriter;
  extern type GArrowRecordBatchWriterClass = _GArrowRecordBatchWriterClass;
  extern type GArrowRecordBatch_autoptr = c_ptr(GArrowRecordBatch);
  extern type GArrowRecordBatch_listautoptr = c_ptr(GList);
  extern type GArrowRecordBatch_queueautoptr = c_ptr(GQueue);
  extern type GArrowRecordBatch_slistautoptr = c_ptr(GSList);
  extern type GArrowResizableBuffer = _GArrowResizableBuffer;
  extern type GArrowResizableBufferClass = _GArrowResizableBufferClass;
  extern type GArrowResizableBufferClass_autoptr = c_ptr(GArrowResizableBufferClass);
  extern type GArrowResizableBufferClass_listautoptr = c_ptr(GList);
  extern type GArrowResizableBufferClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowResizableBufferClass_slistautoptr = c_ptr(GSList);
  extern type GArrowResizableBuffer_autoptr = c_ptr(GArrowResizableBuffer);
  extern type GArrowResizableBuffer_listautoptr = c_ptr(GList);
  extern type GArrowResizableBuffer_queueautoptr = c_ptr(GQueue);
  extern type GArrowResizableBuffer_slistautoptr = c_ptr(GSList);
  extern type GArrowS3FileSystem = _GArrowS3FileSystem;
  extern type GArrowS3FileSystemClass = _GArrowS3FileSystemClass;
  extern type GArrowS3FileSystemClass_autoptr = c_ptr(GArrowS3FileSystemClass);
  extern type GArrowS3FileSystemClass_listautoptr = c_ptr(GList);
  extern type GArrowS3FileSystemClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowS3FileSystemClass_slistautoptr = c_ptr(GSList);
  extern type GArrowS3FileSystem_autoptr = c_ptr(GArrowS3FileSystem);
  extern type GArrowS3FileSystem_listautoptr = c_ptr(GList);
  extern type GArrowS3FileSystem_queueautoptr = c_ptr(GQueue);
  extern type GArrowS3FileSystem_slistautoptr = c_ptr(GSList);
  extern type GArrowScalar = _GArrowScalar;
  extern type GArrowScalarAggregateOptions = _GArrowScalarAggregateOptions;
  extern type GArrowScalarAggregateOptionsClass = _GArrowScalarAggregateOptionsClass;
  extern type GArrowScalarAggregateOptionsClass_autoptr = c_ptr(GArrowScalarAggregateOptionsClass);
  extern type GArrowScalarAggregateOptionsClass_listautoptr = c_ptr(GList);
  extern type GArrowScalarAggregateOptionsClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowScalarAggregateOptionsClass_slistautoptr = c_ptr(GSList);
  extern type GArrowScalarAggregateOptions_autoptr = c_ptr(GArrowScalarAggregateOptions);
  extern type GArrowScalarAggregateOptions_listautoptr = c_ptr(GList);
  extern type GArrowScalarAggregateOptions_queueautoptr = c_ptr(GQueue);
  extern type GArrowScalarAggregateOptions_slistautoptr = c_ptr(GSList);
  extern type GArrowScalarClass = _GArrowScalarClass;
  extern type GArrowScalarClass_autoptr = c_ptr(GArrowScalarClass);
  extern type GArrowScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowScalarDatum = _GArrowScalarDatum;
  extern type GArrowScalarDatumClass = _GArrowScalarDatumClass;
  extern type GArrowScalarDatumClass_autoptr = c_ptr(GArrowScalarDatumClass);
  extern type GArrowScalarDatumClass_listautoptr = c_ptr(GList);
  extern type GArrowScalarDatumClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowScalarDatumClass_slistautoptr = c_ptr(GSList);
  extern type GArrowScalarDatum_autoptr = c_ptr(GArrowScalarDatum);
  extern type GArrowScalarDatum_listautoptr = c_ptr(GList);
  extern type GArrowScalarDatum_queueautoptr = c_ptr(GQueue);
  extern type GArrowScalarDatum_slistautoptr = c_ptr(GSList);
  extern type GArrowScalar_autoptr = c_ptr(GArrowScalar);
  extern type GArrowScalar_listautoptr = c_ptr(GList);
  extern type GArrowScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowSchema = _GArrowSchema;
  extern type GArrowSchemaClass = _GArrowSchemaClass;
  extern type GArrowSchemaClass_autoptr = c_ptr(GArrowSchemaClass);
  extern type GArrowSchemaClass_listautoptr = c_ptr(GList);
  extern type GArrowSchemaClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowSchemaClass_slistautoptr = c_ptr(GSList);
  extern type GArrowSchema_autoptr = c_ptr(GArrowSchema);
  extern type GArrowSchema_listautoptr = c_ptr(GList);
  extern type GArrowSchema_queueautoptr = c_ptr(GQueue);
  extern type GArrowSchema_slistautoptr = c_ptr(GSList);
  extern type GArrowSeekableInputStream = _GArrowSeekableInputStream;
  extern type GArrowSeekableInputStreamClass = _GArrowSeekableInputStreamClass;
  extern type GArrowSeekableInputStreamClass_autoptr = c_ptr(GArrowSeekableInputStreamClass);
  extern type GArrowSeekableInputStreamClass_listautoptr = c_ptr(GList);
  extern type GArrowSeekableInputStreamClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowSeekableInputStreamClass_slistautoptr = c_ptr(GSList);
  extern type GArrowSeekableInputStream_autoptr = c_ptr(GArrowSeekableInputStream);
  extern type GArrowSeekableInputStream_listautoptr = c_ptr(GList);
  extern type GArrowSeekableInputStream_queueautoptr = c_ptr(GQueue);
  extern type GArrowSeekableInputStream_slistautoptr = c_ptr(GSList);
  extern type GArrowSlowFileSystem = _GArrowSlowFileSystem;
  extern type GArrowSlowFileSystemClass = _GArrowSlowFileSystemClass;
  extern type GArrowSlowFileSystemClass_autoptr = c_ptr(GArrowSlowFileSystemClass);
  extern type GArrowSlowFileSystemClass_listautoptr = c_ptr(GList);
  extern type GArrowSlowFileSystemClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowSlowFileSystemClass_slistautoptr = c_ptr(GSList);
  extern type GArrowSlowFileSystem_autoptr = c_ptr(GArrowSlowFileSystem);
  extern type GArrowSlowFileSystem_listautoptr = c_ptr(GList);
  extern type GArrowSlowFileSystem_queueautoptr = c_ptr(GQueue);
  extern type GArrowSlowFileSystem_slistautoptr = c_ptr(GSList);
  extern type GArrowSortKey = _GArrowSortKey;
  extern type GArrowSortKeyClass = _GArrowSortKeyClass;
  extern type GArrowSortKeyClass_autoptr = c_ptr(GArrowSortKeyClass);
  extern type GArrowSortKeyClass_listautoptr = c_ptr(GList);
  extern type GArrowSortKeyClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowSortKeyClass_slistautoptr = c_ptr(GSList);
  extern type GArrowSortKey_autoptr = c_ptr(GArrowSortKey);
  extern type GArrowSortKey_listautoptr = c_ptr(GList);
  extern type GArrowSortKey_queueautoptr = c_ptr(GQueue);
  extern type GArrowSortKey_slistautoptr = c_ptr(GSList);
  extern type GArrowSortOptions = _GArrowSortOptions;
  extern type GArrowSortOptionsClass = _GArrowSortOptionsClass;
  extern type GArrowSortOptionsClass_autoptr = c_ptr(GArrowSortOptionsClass);
  extern type GArrowSortOptionsClass_listautoptr = c_ptr(GList);
  extern type GArrowSortOptionsClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowSortOptionsClass_slistautoptr = c_ptr(GSList);
  extern type GArrowSortOptions_autoptr = c_ptr(GArrowSortOptions);
  extern type GArrowSortOptions_listautoptr = c_ptr(GList);
  extern type GArrowSortOptions_queueautoptr = c_ptr(GQueue);
  extern type GArrowSortOptions_slistautoptr = c_ptr(GSList);
  // GArrowSortOrder enum
  extern type GArrowSortOrder = c_int;
  extern const GARROW_SORT_ORDER_ASCENDING :GArrowSortOrder;
  extern const GARROW_SORT_ORDER_DESCENDING :GArrowSortOrder;
  extern type GArrowSparseUnionArray = _GArrowSparseUnionArray;
  extern type GArrowSparseUnionArrayClass = _GArrowSparseUnionArrayClass;
  extern type GArrowSparseUnionArrayClass_autoptr = c_ptr(GArrowSparseUnionArrayClass);
  extern type GArrowSparseUnionArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowSparseUnionArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowSparseUnionArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowSparseUnionArray_autoptr = c_ptr(GArrowSparseUnionArray);
  extern type GArrowSparseUnionArray_listautoptr = c_ptr(GList);
  extern type GArrowSparseUnionArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowSparseUnionArray_slistautoptr = c_ptr(GSList);
  extern type GArrowSparseUnionDataType = _GArrowSparseUnionDataType;
  extern type GArrowSparseUnionDataTypeClass = _GArrowSparseUnionDataTypeClass;
  extern type GArrowSparseUnionDataTypeClass_autoptr = c_ptr(GArrowSparseUnionDataTypeClass);
  extern type GArrowSparseUnionDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowSparseUnionDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowSparseUnionDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowSparseUnionDataType_autoptr = c_ptr(GArrowSparseUnionDataType);
  extern type GArrowSparseUnionDataType_listautoptr = c_ptr(GList);
  extern type GArrowSparseUnionDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowSparseUnionDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowSparseUnionScalar = _GArrowSparseUnionScalar;
  extern type GArrowSparseUnionScalarClass = _GArrowSparseUnionScalarClass;
  extern type GArrowSparseUnionScalarClass_autoptr = c_ptr(GArrowSparseUnionScalarClass);
  extern type GArrowSparseUnionScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowSparseUnionScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowSparseUnionScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowSparseUnionScalar_autoptr = c_ptr(GArrowSparseUnionScalar);
  extern type GArrowSparseUnionScalar_listautoptr = c_ptr(GList);
  extern type GArrowSparseUnionScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowSparseUnionScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowStringArray = _GArrowStringArray;
  extern type GArrowStringArrayBuilder = _GArrowStringArrayBuilder;
  extern type GArrowStringArrayBuilderClass = _GArrowStringArrayBuilderClass;
  extern type GArrowStringArrayBuilderClass_autoptr = c_ptr(GArrowStringArrayBuilderClass);
  extern type GArrowStringArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowStringArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowStringArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowStringArrayBuilder_autoptr = c_ptr(GArrowStringArrayBuilder);
  extern type GArrowStringArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowStringArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowStringArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowStringArrayClass = _GArrowStringArrayClass;
  extern type GArrowStringArrayClass_autoptr = c_ptr(GArrowStringArrayClass);
  extern type GArrowStringArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowStringArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowStringArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowStringArray_autoptr = c_ptr(GArrowStringArray);
  extern type GArrowStringArray_listautoptr = c_ptr(GList);
  extern type GArrowStringArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowStringArray_slistautoptr = c_ptr(GSList);
  extern type GArrowStringDataType = _GArrowStringDataType;
  extern type GArrowStringDataTypeClass = _GArrowStringDataTypeClass;
  extern type GArrowStringDataTypeClass_autoptr = c_ptr(GArrowStringDataTypeClass);
  extern type GArrowStringDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowStringDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowStringDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowStringDataType_autoptr = c_ptr(GArrowStringDataType);
  extern type GArrowStringDataType_listautoptr = c_ptr(GList);
  extern type GArrowStringDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowStringDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowStringDictionaryArrayBuilder = _GArrowStringDictionaryArrayBuilder;
  extern type GArrowStringDictionaryArrayBuilderClass = _GArrowStringDictionaryArrayBuilderClass;
  extern type GArrowStringDictionaryArrayBuilderClass_autoptr = c_ptr(GArrowStringDictionaryArrayBuilderClass);
  extern type GArrowStringDictionaryArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowStringDictionaryArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowStringDictionaryArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowStringDictionaryArrayBuilder_autoptr = c_ptr(GArrowStringDictionaryArrayBuilder);
  extern type GArrowStringDictionaryArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowStringDictionaryArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowStringDictionaryArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowStringScalar = _GArrowStringScalar;
  extern type GArrowStringScalarClass = _GArrowStringScalarClass;
  extern type GArrowStringScalarClass_autoptr = c_ptr(GArrowStringScalarClass);
  extern type GArrowStringScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowStringScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowStringScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowStringScalar_autoptr = c_ptr(GArrowStringScalar);
  extern type GArrowStringScalar_listautoptr = c_ptr(GList);
  extern type GArrowStringScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowStringScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowStructArray = _GArrowStructArray;
  extern type GArrowStructArrayBuilder = _GArrowStructArrayBuilder;
  extern type GArrowStructArrayBuilderClass = _GArrowStructArrayBuilderClass;
  extern type GArrowStructArrayBuilderClass_autoptr = c_ptr(GArrowStructArrayBuilderClass);
  extern type GArrowStructArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowStructArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowStructArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowStructArrayBuilder_autoptr = c_ptr(GArrowStructArrayBuilder);
  extern type GArrowStructArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowStructArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowStructArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowStructArrayClass = _GArrowStructArrayClass;
  extern type GArrowStructArrayClass_autoptr = c_ptr(GArrowStructArrayClass);
  extern type GArrowStructArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowStructArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowStructArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowStructArray_autoptr = c_ptr(GArrowStructArray);
  extern type GArrowStructArray_listautoptr = c_ptr(GList);
  extern type GArrowStructArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowStructArray_slistautoptr = c_ptr(GSList);
  extern type GArrowStructDataType = _GArrowStructDataType;
  extern type GArrowStructDataTypeClass = _GArrowStructDataTypeClass;
  extern type GArrowStructDataTypeClass_autoptr = c_ptr(GArrowStructDataTypeClass);
  extern type GArrowStructDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowStructDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowStructDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowStructDataType_autoptr = c_ptr(GArrowStructDataType);
  extern type GArrowStructDataType_listautoptr = c_ptr(GList);
  extern type GArrowStructDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowStructDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowStructScalar = _GArrowStructScalar;
  extern type GArrowStructScalarClass = _GArrowStructScalarClass;
  extern type GArrowStructScalarClass_autoptr = c_ptr(GArrowStructScalarClass);
  extern type GArrowStructScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowStructScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowStructScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowStructScalar_autoptr = c_ptr(GArrowStructScalar);
  extern type GArrowStructScalar_listautoptr = c_ptr(GList);
  extern type GArrowStructScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowStructScalar_slistautoptr = c_ptr(GSList);
  extern type GArrowSubTreeFileSystem = _GArrowSubTreeFileSystem;
  extern type GArrowSubTreeFileSystemClass = _GArrowSubTreeFileSystemClass;
  extern type GArrowSubTreeFileSystemClass_autoptr = c_ptr(GArrowSubTreeFileSystemClass);
  extern type GArrowSubTreeFileSystemClass_listautoptr = c_ptr(GList);
  extern type GArrowSubTreeFileSystemClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowSubTreeFileSystemClass_slistautoptr = c_ptr(GSList);
  extern type GArrowSubTreeFileSystem_autoptr = c_ptr(GArrowSubTreeFileSystem);
  extern type GArrowSubTreeFileSystem_listautoptr = c_ptr(GList);
  extern type GArrowSubTreeFileSystem_queueautoptr = c_ptr(GQueue);
  extern type GArrowSubTreeFileSystem_slistautoptr = c_ptr(GSList);
  extern type GArrowTable = _GArrowTable;
  extern type GArrowTableBatchReader = _GArrowTableBatchReader;
  extern type GArrowTableBatchReaderClass = _GArrowTableBatchReaderClass;
  extern type GArrowTableBatchReaderClass_autoptr = c_ptr(GArrowTableBatchReaderClass);
  extern type GArrowTableBatchReaderClass_listautoptr = c_ptr(GList);
  extern type GArrowTableBatchReaderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTableBatchReaderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTableBatchReader_autoptr = c_ptr(GArrowTableBatchReader);
  extern type GArrowTableBatchReader_listautoptr = c_ptr(GList);
  extern type GArrowTableBatchReader_queueautoptr = c_ptr(GQueue);
  extern type GArrowTableBatchReader_slistautoptr = c_ptr(GSList);
  extern type GArrowTableClass = _GArrowTableClass;
  extern type GArrowTableClass_autoptr = c_ptr(GArrowTableClass);
  extern type GArrowTableClass_listautoptr = c_ptr(GList);
  extern type GArrowTableClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTableClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTableDatum = _GArrowTableDatum;
  extern type GArrowTableDatumClass = _GArrowTableDatumClass;
  extern type GArrowTableDatumClass_autoptr = c_ptr(GArrowTableDatumClass);
  extern type GArrowTableDatumClass_listautoptr = c_ptr(GList);
  extern type GArrowTableDatumClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTableDatumClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTableDatum_autoptr = c_ptr(GArrowTableDatum);
  extern type GArrowTableDatum_listautoptr = c_ptr(GList);
  extern type GArrowTableDatum_queueautoptr = c_ptr(GQueue);
  extern type GArrowTableDatum_slistautoptr = c_ptr(GSList);
  extern type GArrowTable_autoptr = c_ptr(GArrowTable);
  extern type GArrowTable_listautoptr = c_ptr(GList);
  extern type GArrowTable_queueautoptr = c_ptr(GQueue);
  extern type GArrowTable_slistautoptr = c_ptr(GSList);
  extern type GArrowTakeOptions = _GArrowTakeOptions;
  extern type GArrowTakeOptionsClass = _GArrowTakeOptionsClass;
  extern type GArrowTakeOptionsClass_autoptr = c_ptr(GArrowTakeOptionsClass);
  extern type GArrowTakeOptionsClass_listautoptr = c_ptr(GList);
  extern type GArrowTakeOptionsClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTakeOptionsClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTakeOptions_autoptr = c_ptr(GArrowTakeOptions);
  extern type GArrowTakeOptions_listautoptr = c_ptr(GList);
  extern type GArrowTakeOptions_queueautoptr = c_ptr(GQueue);
  extern type GArrowTakeOptions_slistautoptr = c_ptr(GSList);
  extern type GArrowTensor = _GArrowTensor;
  extern type GArrowTensorClass = _GArrowTensorClass;
  extern type GArrowTensorClass_autoptr = c_ptr(GArrowTensorClass);
  extern type GArrowTensorClass_listautoptr = c_ptr(GList);
  extern type GArrowTensorClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTensorClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTensor_autoptr = c_ptr(GArrowTensor);
  extern type GArrowTensor_listautoptr = c_ptr(GList);
  extern type GArrowTensor_queueautoptr = c_ptr(GQueue);
  extern type GArrowTensor_slistautoptr = c_ptr(GSList);
  extern type GArrowTime32Array = _GArrowTime32Array;
  extern type GArrowTime32ArrayBuilder = _GArrowTime32ArrayBuilder;
  extern type GArrowTime32ArrayBuilderClass = _GArrowTime32ArrayBuilderClass;
  extern type GArrowTime32ArrayBuilderClass_autoptr = c_ptr(GArrowTime32ArrayBuilderClass);
  extern type GArrowTime32ArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowTime32ArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime32ArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTime32ArrayBuilder_autoptr = c_ptr(GArrowTime32ArrayBuilder);
  extern type GArrowTime32ArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowTime32ArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime32ArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowTime32ArrayClass = _GArrowTime32ArrayClass;
  extern type GArrowTime32ArrayClass_autoptr = c_ptr(GArrowTime32ArrayClass);
  extern type GArrowTime32ArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowTime32ArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime32ArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTime32Array_autoptr = c_ptr(GArrowTime32Array);
  extern type GArrowTime32Array_listautoptr = c_ptr(GList);
  extern type GArrowTime32Array_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime32Array_slistautoptr = c_ptr(GSList);
  extern type GArrowTime32DataType = _GArrowTime32DataType;
  extern type GArrowTime32DataTypeClass = _GArrowTime32DataTypeClass;
  extern type GArrowTime32DataTypeClass_autoptr = c_ptr(GArrowTime32DataTypeClass);
  extern type GArrowTime32DataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowTime32DataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime32DataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTime32DataType_autoptr = c_ptr(GArrowTime32DataType);
  extern type GArrowTime32DataType_listautoptr = c_ptr(GList);
  extern type GArrowTime32DataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime32DataType_slistautoptr = c_ptr(GSList);
  extern type GArrowTime32Scalar = _GArrowTime32Scalar;
  extern type GArrowTime32ScalarClass = _GArrowTime32ScalarClass;
  extern type GArrowTime32ScalarClass_autoptr = c_ptr(GArrowTime32ScalarClass);
  extern type GArrowTime32ScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowTime32ScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime32ScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTime32Scalar_autoptr = c_ptr(GArrowTime32Scalar);
  extern type GArrowTime32Scalar_listautoptr = c_ptr(GList);
  extern type GArrowTime32Scalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime32Scalar_slistautoptr = c_ptr(GSList);
  extern type GArrowTime64Array = _GArrowTime64Array;
  extern type GArrowTime64ArrayBuilder = _GArrowTime64ArrayBuilder;
  extern type GArrowTime64ArrayBuilderClass = _GArrowTime64ArrayBuilderClass;
  extern type GArrowTime64ArrayBuilderClass_autoptr = c_ptr(GArrowTime64ArrayBuilderClass);
  extern type GArrowTime64ArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowTime64ArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime64ArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTime64ArrayBuilder_autoptr = c_ptr(GArrowTime64ArrayBuilder);
  extern type GArrowTime64ArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowTime64ArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime64ArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowTime64ArrayClass = _GArrowTime64ArrayClass;
  extern type GArrowTime64ArrayClass_autoptr = c_ptr(GArrowTime64ArrayClass);
  extern type GArrowTime64ArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowTime64ArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime64ArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTime64Array_autoptr = c_ptr(GArrowTime64Array);
  extern type GArrowTime64Array_listautoptr = c_ptr(GList);
  extern type GArrowTime64Array_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime64Array_slistautoptr = c_ptr(GSList);
  extern type GArrowTime64DataType = _GArrowTime64DataType;
  extern type GArrowTime64DataTypeClass = _GArrowTime64DataTypeClass;
  extern type GArrowTime64DataTypeClass_autoptr = c_ptr(GArrowTime64DataTypeClass);
  extern type GArrowTime64DataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowTime64DataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime64DataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTime64DataType_autoptr = c_ptr(GArrowTime64DataType);
  extern type GArrowTime64DataType_listautoptr = c_ptr(GList);
  extern type GArrowTime64DataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime64DataType_slistautoptr = c_ptr(GSList);
  extern type GArrowTime64Scalar = _GArrowTime64Scalar;
  extern type GArrowTime64ScalarClass = _GArrowTime64ScalarClass;
  extern type GArrowTime64ScalarClass_autoptr = c_ptr(GArrowTime64ScalarClass);
  extern type GArrowTime64ScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowTime64ScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime64ScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTime64Scalar_autoptr = c_ptr(GArrowTime64Scalar);
  extern type GArrowTime64Scalar_listautoptr = c_ptr(GList);
  extern type GArrowTime64Scalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowTime64Scalar_slistautoptr = c_ptr(GSList);
  extern type GArrowTimeDataType = _GArrowTimeDataType;
  extern type GArrowTimeDataTypeClass = _GArrowTimeDataTypeClass;
  extern type GArrowTimeDataTypeClass_autoptr = c_ptr(GArrowTimeDataTypeClass);
  extern type GArrowTimeDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowTimeDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTimeDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTimeDataType_autoptr = c_ptr(GArrowTimeDataType);
  extern type GArrowTimeDataType_listautoptr = c_ptr(GList);
  extern type GArrowTimeDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowTimeDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowTimePoint = gint64;
  // GArrowTimeUnit enum
  extern type GArrowTimeUnit = c_int;
  extern const GARROW_TIME_UNIT_SECOND :GArrowTimeUnit;
  extern const GARROW_TIME_UNIT_MILLI :GArrowTimeUnit;
  extern const GARROW_TIME_UNIT_MICRO :GArrowTimeUnit;
  extern const GARROW_TIME_UNIT_NANO :GArrowTimeUnit;
  extern type GArrowTimestampArray = _GArrowTimestampArray;
  extern type GArrowTimestampArrayBuilder = _GArrowTimestampArrayBuilder;
  extern type GArrowTimestampArrayBuilderClass = _GArrowTimestampArrayBuilderClass;
  extern type GArrowTimestampArrayBuilderClass_autoptr = c_ptr(GArrowTimestampArrayBuilderClass);
  extern type GArrowTimestampArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowTimestampArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTimestampArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTimestampArrayBuilder_autoptr = c_ptr(GArrowTimestampArrayBuilder);
  extern type GArrowTimestampArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowTimestampArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowTimestampArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowTimestampArrayClass = _GArrowTimestampArrayClass;
  extern type GArrowTimestampArrayClass_autoptr = c_ptr(GArrowTimestampArrayClass);
  extern type GArrowTimestampArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowTimestampArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTimestampArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTimestampArray_autoptr = c_ptr(GArrowTimestampArray);
  extern type GArrowTimestampArray_listautoptr = c_ptr(GList);
  extern type GArrowTimestampArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowTimestampArray_slistautoptr = c_ptr(GSList);
  extern type GArrowTimestampDataType = _GArrowTimestampDataType;
  extern type GArrowTimestampDataTypeClass = _GArrowTimestampDataTypeClass;
  extern type GArrowTimestampDataTypeClass_autoptr = c_ptr(GArrowTimestampDataTypeClass);
  extern type GArrowTimestampDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowTimestampDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTimestampDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTimestampDataType_autoptr = c_ptr(GArrowTimestampDataType);
  extern type GArrowTimestampDataType_listautoptr = c_ptr(GList);
  extern type GArrowTimestampDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowTimestampDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowTimestampScalar = _GArrowTimestampScalar;
  extern type GArrowTimestampScalarClass = _GArrowTimestampScalarClass;
  extern type GArrowTimestampScalarClass_autoptr = c_ptr(GArrowTimestampScalarClass);
  extern type GArrowTimestampScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowTimestampScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowTimestampScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowTimestampScalar_autoptr = c_ptr(GArrowTimestampScalar);
  extern type GArrowTimestampScalar_listautoptr = c_ptr(GList);
  extern type GArrowTimestampScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowTimestampScalar_slistautoptr = c_ptr(GSList);
  // GArrowType enum
  extern type GArrowType = c_int;
  extern const GARROW_TYPE_NA :GArrowType;
  extern const GARROW_TYPE_BOOLEAN :GArrowType;
  extern const GARROW_TYPE_UINT8 :GArrowType;
  extern const GARROW_TYPE_INT8 :GArrowType;
  extern const GARROW_TYPE_UINT16 :GArrowType;
  extern const GARROW_TYPE_INT16 :GArrowType;
  extern const GARROW_TYPE_UINT32 :GArrowType;
  extern const GARROW_TYPE_INT32 :GArrowType;
  extern const GARROW_TYPE_UINT64 :GArrowType;
  extern const GARROW_TYPE_INT64 :GArrowType;
  extern const GARROW_TYPE_HALF_FLOAT :GArrowType;
  extern const GARROW_TYPE_FLOAT :GArrowType;
  extern const GARROW_TYPE_DOUBLE :GArrowType;
  extern const GARROW_TYPE_STRING :GArrowType;
  extern const GARROW_TYPE_BINARY :GArrowType;
  extern const GARROW_TYPE_FIXED_SIZE_BINARY :GArrowType;
  extern const GARROW_TYPE_DATE32 :GArrowType;
  extern const GARROW_TYPE_DATE64 :GArrowType;
  extern const GARROW_TYPE_TIMESTAMP :GArrowType;
  extern const GARROW_TYPE_TIME32 :GArrowType;
  extern const GARROW_TYPE_TIME64 :GArrowType;
  extern const GARROW_TYPE_INTERVAL_MONTHS :GArrowType;
  extern const GARROW_TYPE_INTERVAL_DAY_TIME :GArrowType;
  extern const GARROW_TYPE_DECIMAL128 :GArrowType;
  extern const GARROW_TYPE_DECIMAL256 :GArrowType;
  extern const GARROW_TYPE_LIST :GArrowType;
  extern const GARROW_TYPE_STRUCT :GArrowType;
  extern const GARROW_TYPE_SPARSE_UNION :GArrowType;
  extern const GARROW_TYPE_DENSE_UNION :GArrowType;
  extern const GARROW_TYPE_DICTIONARY :GArrowType;
  extern const GARROW_TYPE_MAP :GArrowType;
  extern const GARROW_TYPE_EXTENSION :GArrowType;
  extern const GARROW_TYPE_FIXED_SIZE_LIST :GArrowType;
  extern const GARROW_TYPE_DURATION :GArrowType;
  extern const GARROW_TYPE_LARGE_STRING :GArrowType;
  extern const GARROW_TYPE_LARGE_BINARY :GArrowType;
  extern const GARROW_TYPE_LARGE_LIST :GArrowType;
  extern type GArrowUInt16Array = _GArrowUInt16Array;
  extern type GArrowUInt16ArrayBuilder = _GArrowUInt16ArrayBuilder;
  extern type GArrowUInt16ArrayBuilderClass = _GArrowUInt16ArrayBuilderClass;
  extern type GArrowUInt16ArrayBuilderClass_autoptr = c_ptr(GArrowUInt16ArrayBuilderClass);
  extern type GArrowUInt16ArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt16ArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt16ArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt16ArrayBuilder_autoptr = c_ptr(GArrowUInt16ArrayBuilder);
  extern type GArrowUInt16ArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowUInt16ArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt16ArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt16ArrayClass = _GArrowUInt16ArrayClass;
  extern type GArrowUInt16ArrayClass_autoptr = c_ptr(GArrowUInt16ArrayClass);
  extern type GArrowUInt16ArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt16ArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt16ArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt16Array_autoptr = c_ptr(GArrowUInt16Array);
  extern type GArrowUInt16Array_listautoptr = c_ptr(GList);
  extern type GArrowUInt16Array_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt16Array_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt16DataType = _GArrowUInt16DataType;
  extern type GArrowUInt16DataTypeClass = _GArrowUInt16DataTypeClass;
  extern type GArrowUInt16DataTypeClass_autoptr = c_ptr(GArrowUInt16DataTypeClass);
  extern type GArrowUInt16DataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt16DataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt16DataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt16DataType_autoptr = c_ptr(GArrowUInt16DataType);
  extern type GArrowUInt16DataType_listautoptr = c_ptr(GList);
  extern type GArrowUInt16DataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt16DataType_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt16Scalar = _GArrowUInt16Scalar;
  extern type GArrowUInt16ScalarClass = _GArrowUInt16ScalarClass;
  extern type GArrowUInt16ScalarClass_autoptr = c_ptr(GArrowUInt16ScalarClass);
  extern type GArrowUInt16ScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt16ScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt16ScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt16Scalar_autoptr = c_ptr(GArrowUInt16Scalar);
  extern type GArrowUInt16Scalar_listautoptr = c_ptr(GList);
  extern type GArrowUInt16Scalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt16Scalar_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt32Array = _GArrowUInt32Array;
  extern type GArrowUInt32ArrayBuilder = _GArrowUInt32ArrayBuilder;
  extern type GArrowUInt32ArrayBuilderClass = _GArrowUInt32ArrayBuilderClass;
  extern type GArrowUInt32ArrayBuilderClass_autoptr = c_ptr(GArrowUInt32ArrayBuilderClass);
  extern type GArrowUInt32ArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt32ArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt32ArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt32ArrayBuilder_autoptr = c_ptr(GArrowUInt32ArrayBuilder);
  extern type GArrowUInt32ArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowUInt32ArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt32ArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt32ArrayClass = _GArrowUInt32ArrayClass;
  extern type GArrowUInt32ArrayClass_autoptr = c_ptr(GArrowUInt32ArrayClass);
  extern type GArrowUInt32ArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt32ArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt32ArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt32Array_autoptr = c_ptr(GArrowUInt32Array);
  extern type GArrowUInt32Array_listautoptr = c_ptr(GList);
  extern type GArrowUInt32Array_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt32Array_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt32DataType = _GArrowUInt32DataType;
  extern type GArrowUInt32DataTypeClass = _GArrowUInt32DataTypeClass;
  extern type GArrowUInt32DataTypeClass_autoptr = c_ptr(GArrowUInt32DataTypeClass);
  extern type GArrowUInt32DataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt32DataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt32DataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt32DataType_autoptr = c_ptr(GArrowUInt32DataType);
  extern type GArrowUInt32DataType_listautoptr = c_ptr(GList);
  extern type GArrowUInt32DataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt32DataType_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt32Scalar = _GArrowUInt32Scalar;
  extern type GArrowUInt32ScalarClass = _GArrowUInt32ScalarClass;
  extern type GArrowUInt32ScalarClass_autoptr = c_ptr(GArrowUInt32ScalarClass);
  extern type GArrowUInt32ScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt32ScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt32ScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt32Scalar_autoptr = c_ptr(GArrowUInt32Scalar);
  extern type GArrowUInt32Scalar_listautoptr = c_ptr(GList);
  extern type GArrowUInt32Scalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt32Scalar_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt64Array = _GArrowUInt64Array;
  extern type GArrowUInt64ArrayBuilder = _GArrowUInt64ArrayBuilder;
  extern type GArrowUInt64ArrayBuilderClass = _GArrowUInt64ArrayBuilderClass;
  extern type GArrowUInt64ArrayBuilderClass_autoptr = c_ptr(GArrowUInt64ArrayBuilderClass);
  extern type GArrowUInt64ArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt64ArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt64ArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt64ArrayBuilder_autoptr = c_ptr(GArrowUInt64ArrayBuilder);
  extern type GArrowUInt64ArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowUInt64ArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt64ArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt64ArrayClass = _GArrowUInt64ArrayClass;
  extern type GArrowUInt64ArrayClass_autoptr = c_ptr(GArrowUInt64ArrayClass);
  extern type GArrowUInt64ArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt64ArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt64ArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt64Array_autoptr = c_ptr(GArrowUInt64Array);
  extern type GArrowUInt64Array_listautoptr = c_ptr(GList);
  extern type GArrowUInt64Array_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt64Array_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt64DataType = _GArrowUInt64DataType;
  extern type GArrowUInt64DataTypeClass = _GArrowUInt64DataTypeClass;
  extern type GArrowUInt64DataTypeClass_autoptr = c_ptr(GArrowUInt64DataTypeClass);
  extern type GArrowUInt64DataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt64DataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt64DataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt64DataType_autoptr = c_ptr(GArrowUInt64DataType);
  extern type GArrowUInt64DataType_listautoptr = c_ptr(GList);
  extern type GArrowUInt64DataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt64DataType_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt64Scalar = _GArrowUInt64Scalar;
  extern type GArrowUInt64ScalarClass = _GArrowUInt64ScalarClass;
  extern type GArrowUInt64ScalarClass_autoptr = c_ptr(GArrowUInt64ScalarClass);
  extern type GArrowUInt64ScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt64ScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt64ScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt64Scalar_autoptr = c_ptr(GArrowUInt64Scalar);
  extern type GArrowUInt64Scalar_listautoptr = c_ptr(GList);
  extern type GArrowUInt64Scalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt64Scalar_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt8Array = _GArrowUInt8Array;
  extern type GArrowUInt8ArrayBuilder = _GArrowUInt8ArrayBuilder;
  extern type GArrowUInt8ArrayBuilderClass = _GArrowUInt8ArrayBuilderClass;
  extern type GArrowUInt8ArrayBuilderClass_autoptr = c_ptr(GArrowUInt8ArrayBuilderClass);
  extern type GArrowUInt8ArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt8ArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt8ArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt8ArrayBuilder_autoptr = c_ptr(GArrowUInt8ArrayBuilder);
  extern type GArrowUInt8ArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowUInt8ArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt8ArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt8ArrayClass = _GArrowUInt8ArrayClass;
  extern type GArrowUInt8ArrayClass_autoptr = c_ptr(GArrowUInt8ArrayClass);
  extern type GArrowUInt8ArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt8ArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt8ArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt8Array_autoptr = c_ptr(GArrowUInt8Array);
  extern type GArrowUInt8Array_listautoptr = c_ptr(GList);
  extern type GArrowUInt8Array_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt8Array_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt8DataType = _GArrowUInt8DataType;
  extern type GArrowUInt8DataTypeClass = _GArrowUInt8DataTypeClass;
  extern type GArrowUInt8DataTypeClass_autoptr = c_ptr(GArrowUInt8DataTypeClass);
  extern type GArrowUInt8DataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt8DataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt8DataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt8DataType_autoptr = c_ptr(GArrowUInt8DataType);
  extern type GArrowUInt8DataType_listautoptr = c_ptr(GList);
  extern type GArrowUInt8DataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt8DataType_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt8Scalar = _GArrowUInt8Scalar;
  extern type GArrowUInt8ScalarClass = _GArrowUInt8ScalarClass;
  extern type GArrowUInt8ScalarClass_autoptr = c_ptr(GArrowUInt8ScalarClass);
  extern type GArrowUInt8ScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowUInt8ScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt8ScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUInt8Scalar_autoptr = c_ptr(GArrowUInt8Scalar);
  extern type GArrowUInt8Scalar_listautoptr = c_ptr(GList);
  extern type GArrowUInt8Scalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowUInt8Scalar_slistautoptr = c_ptr(GSList);
  extern type GArrowUIntArrayBuilder = _GArrowUIntArrayBuilder;
  extern type GArrowUIntArrayBuilderClass = _GArrowUIntArrayBuilderClass;
  extern type GArrowUIntArrayBuilderClass_autoptr = c_ptr(GArrowUIntArrayBuilderClass);
  extern type GArrowUIntArrayBuilderClass_listautoptr = c_ptr(GList);
  extern type GArrowUIntArrayBuilderClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUIntArrayBuilderClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUIntArrayBuilder_autoptr = c_ptr(GArrowUIntArrayBuilder);
  extern type GArrowUIntArrayBuilder_listautoptr = c_ptr(GList);
  extern type GArrowUIntArrayBuilder_queueautoptr = c_ptr(GQueue);
  extern type GArrowUIntArrayBuilder_slistautoptr = c_ptr(GSList);
  extern type GArrowUnionArray = _GArrowUnionArray;
  extern type GArrowUnionArrayClass = _GArrowUnionArrayClass;
  extern type GArrowUnionArrayClass_autoptr = c_ptr(GArrowUnionArrayClass);
  extern type GArrowUnionArrayClass_listautoptr = c_ptr(GList);
  extern type GArrowUnionArrayClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUnionArrayClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUnionArray_autoptr = c_ptr(GArrowUnionArray);
  extern type GArrowUnionArray_listautoptr = c_ptr(GList);
  extern type GArrowUnionArray_queueautoptr = c_ptr(GQueue);
  extern type GArrowUnionArray_slistautoptr = c_ptr(GSList);
  extern type GArrowUnionDataType = _GArrowUnionDataType;
  extern type GArrowUnionDataTypeClass = _GArrowUnionDataTypeClass;
  extern type GArrowUnionDataTypeClass_autoptr = c_ptr(GArrowUnionDataTypeClass);
  extern type GArrowUnionDataTypeClass_listautoptr = c_ptr(GList);
  extern type GArrowUnionDataTypeClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUnionDataTypeClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUnionDataType_autoptr = c_ptr(GArrowUnionDataType);
  extern type GArrowUnionDataType_listautoptr = c_ptr(GList);
  extern type GArrowUnionDataType_queueautoptr = c_ptr(GQueue);
  extern type GArrowUnionDataType_slistautoptr = c_ptr(GSList);
  extern type GArrowUnionScalar = _GArrowUnionScalar;
  extern type GArrowUnionScalarClass = _GArrowUnionScalarClass;
  extern type GArrowUnionScalarClass_autoptr = c_ptr(GArrowUnionScalarClass);
  extern type GArrowUnionScalarClass_listautoptr = c_ptr(GList);
  extern type GArrowUnionScalarClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowUnionScalarClass_slistautoptr = c_ptr(GSList);
  extern type GArrowUnionScalar_autoptr = c_ptr(GArrowUnionScalar);
  extern type GArrowUnionScalar_listautoptr = c_ptr(GList);
  extern type GArrowUnionScalar_queueautoptr = c_ptr(GQueue);
  extern type GArrowUnionScalar_slistautoptr = c_ptr(GSList);
  // Opaque struct?
  // Opaque struct?
  // Opaque struct?
  extern type GArrowWritableFile_autoptr = c_ptr(GArrowWritableFile);
  extern type GArrowWritableFile_listautoptr = c_ptr(GList);
  extern type GArrowWritableFile_queueautoptr = c_ptr(GQueue);
  extern type GArrowWritableFile_slistautoptr = c_ptr(GSList);
  // Opaque struct?
  extern type GArrowWritable_autoptr = c_ptr(GArrowWritable);
  extern type GArrowWritable_listautoptr = c_ptr(GList);
  extern type GArrowWritable_queueautoptr = c_ptr(GQueue);
  extern type GArrowWritable_slistautoptr = c_ptr(GSList);
  extern type GArrowWriteOptions = _GArrowWriteOptions;
  extern type GArrowWriteOptionsClass = _GArrowWriteOptionsClass;
  extern type GArrowWriteOptionsClass_autoptr = c_ptr(GArrowWriteOptionsClass);
  extern type GArrowWriteOptionsClass_listautoptr = c_ptr(GList);
  extern type GArrowWriteOptionsClass_queueautoptr = c_ptr(GQueue);
  extern type GArrowWriteOptionsClass_slistautoptr = c_ptr(GSList);
  extern type GArrowWriteOptions_autoptr = c_ptr(GArrowWriteOptions);
  extern type GArrowWriteOptions_listautoptr = c_ptr(GList);
  extern type GArrowWriteOptions_queueautoptr = c_ptr(GQueue);
  extern type GArrowWriteOptions_slistautoptr = c_ptr(GSList);

  extern type gatomicrefcount = gint;
  extern type gboolean = gint;
  extern type gchar = c_char;
  extern type gchararray = c_ptr(gchar);
  extern type gconstpointer = c_void_ptr;
  extern type gdouble = real(64);
  extern type gfloat = real(32);
  extern type gint = c_int;
  extern type gint16 = c_short;
  extern type gint32 = c_int;
  extern type gint64 = c_longlong;
  extern type gint8 = c_schar;
  extern type gintptr = c_long;
  extern type glong = c_long;
  extern type goffset = gint64;
  extern type gpointer = c_void_ptr;
  extern type grefcount = gint;
  extern type gshort = c_short;
  extern type gsize = c_ulong;
  extern type gssize = c_long;
  extern type guchar = c_uchar;
  extern type guint = c_uint;
  extern type guint16 = c_ushort;
  extern type guint32 = c_uint;
  extern type guint64 = c_ulonglong;
  extern type guint8 = c_uchar;
  extern type guintptr = c_ulong;
  extern type gulong = c_ulong;
  extern type gunichar = guint32;
  extern type gunichar2 = guint16;
  extern type gushort = c_ushort;
  // START MINE
  extern proc g_print(a);
  extern proc g_print(a,b);
  extern proc g_print(a,b,c);
  extern proc g_error_free(a);
  extern proc g_object_unref(a);
  extern proc g_list_append(a,b): c_ptr(GList);
  // Temporary Error caller for convenience.
  inline proc printGError(msg: string, error: GErrorPtr){
    g_print("%s %s\n".c_str(): c_ptr(gchar), msg.c_str(), error.deref().message);
    g_error_free(error);
  }
  // -------------------------- Type Declarations and Functions -------------
  record arrowArray{
    var val: c_ptr(GArrowArray);
    proc init(arr: [] ?arrayType, validIndices: [] int = [-1], 
              invalidIndices: [] int = [-1]){
      this.val = buildArrowArray(arr, validIndices, invalidIndices);
    }
  }
  proc arrowInt32(): GArrowInt32DataType {
    return garrow_int32_data_type_new();
  }
  proc arrowInt64(): GArrowInt32DataType {
    return garrow_int64_data_type_new();
  }
  proc arrowString(): GArrowStringDataType {
    return garrow_string_data_type_new();
  }
  proc arrowBool(): GArrowBooleanDataType {
    return garrow_boolean_data_type_new();
  }
  //--------------------------- Array building functions --------------------
  
  proc buildArrowArray(arr: [] ?arrayType, validIndices: [] int = [-1], 
             invalidIndices: [] int = [-1]): c_ptr(GArrowArray) {
    // Build full validity array here since each function needs it anyways
    var validityArr: [0..#arr.size] gboolean;
    if(validIndices[0] != -1 && invalidIndices[0] != -1){
      // Both valid indices and invalid indices was passed which is not good
      // Maybe raise a compilerError here instead of the normal error handling
      writeln("Only one of two possible validity arguments may be passed.\
      [both validIndices and invalidIndices were passed]");
      var fakeReturn: c_ptr(GArrowArray);
      return fakeReturn;
    } else if(validIndices[0] != -1){
      validityArr = [i in 0..#arr.size] 0: gboolean;
      forall i in 0..#validIndices.size {
        validityArr[validIndices[i]] = 1: gboolean;
      }
    } else if(invalidIndices[0] != -1){
      validityArr = [i in 0..#arr.size] 1: gboolean;
      forall i in 0..#invalidIndices.size {
        validityArr[invalidIndices[i]] = 0: gboolean;
      }
    } else {
      validityArr = [i in 0..#arr.size] 1: gboolean;
    }
    select arrayType {
        when int do return int64Array(arr, validityArr) : c_ptr(GArrowArray);
        when string do return stringArray(arr, validityArr) : c_ptr(GArrowArray);
        when bool do return boolArray(arr, validityArr) : c_ptr(GArrowArray);
        otherwise {
          writeln("Unsupported type, \nreturning nil"); 
          var fakeReturn: c_ptr(GArrowArray);
          return fakeReturn;
        }
      }
  }
  proc int64Array(arr: [] int, validity: [] gboolean) : c_ptr(GArrowInt64Array) {
    var success: gboolean = 1;
    var error: GErrorPtr;
    var builder: c_ptr(GArrowInt64ArrayBuilder) = garrow_int64_array_builder_new();
    var retval: c_ptr(GArrowInt64Array);
    if (success) {
      var intArrLen: gint64 = arr.size;
      var intValArr = [val in arr] val;
      var intValidityArrLen = intArrLen;
      success = garrow_int64_array_builder_append_values(
                                                         builder, c_ptrTo(intValArr), intArrLen, c_ptrTo(validity), intValidityArrLen, c_ptrTo(error));
    }
    if (!success) {
      printGError("failed to append:", error);
      g_object_unref(builder);
      return retval;
    }
    retval = garrow_array_builder_finish(
                                         GARROW_ARRAY_BUILDER(builder), c_ptrTo(error)) : c_ptr(GArrowInt64Array);
    if (isNull(retval)) {
      printGError("failed to finish:", error);
      g_object_unref(builder);
      return retval;
    }
    g_object_unref(builder);
    return retval;
  }
  proc int32Array(arr: [] int(32), validity: [] gboolean) : c_ptr(GArrowInt32Array) {
    var success: gboolean = 1;
    var error: GErrorPtr;
    var builder: c_ptr(GArrowInt32ArrayBuilder) = garrow_int32_array_builder_new();
    var retval: c_ptr(GArrowInt32Array);
    if (success) {
      var intArrLen: gint64 = arr.size;
      var intValArr = [val in arr] val: gint32;
      var intValidityArrLen: gint64 = intArrLen;
      success = garrow_int32_array_builder_append_values(
                                                         builder, c_ptrTo(intValArr), intArrLen, c_ptrTo(validity), intValidityArrLen, c_ptrTo(error));
    }
    if (!success) {
      printGError("failed to append:", error);
      g_object_unref(builder);
      return retval;
    }
    retval = garrow_array_builder_finish(
                                         GARROW_ARRAY_BUILDER(builder), c_ptrTo(error)) : c_ptr(GArrowInt32Array);
    if (isNull(retval)) {
      printGError("failed to finish:", error);
      g_object_unref(builder);
      return retval;
    }
    g_object_unref(builder); 
    return retval;
  }
  proc stringArray(arr: [] string, validity: [] gboolean) : c_ptr(GArrowStringArray) {
    var success: gboolean = 1;
    var error: GErrorPtr;
    var builder: c_ptr(GArrowStringArrayBuilder) = garrow_string_array_builder_new();
    var retval: c_ptr(GArrowStringArray);
    if (success) {
      var strArrLen: gint64 = arr.size: gint64;
      var strValArr = [val in arr] val.c_str() : c_ptr(gchar);
      var strValidityArrLen: gint64 = strArrLen;
      success = garrow_string_array_builder_append_strings(
                                                           builder, c_ptrTo(strValArr), strArrLen, c_ptrTo(validity), strValidityArrLen, c_ptrTo(error));
    }
    if (!success) {
      printGError("failed to append:", error);
      g_object_unref(builder);
      return retval;
    }
    retval = garrow_array_builder_finish(
                                         GARROW_ARRAY_BUILDER(builder), c_ptrTo(error)) : c_ptr(GArrowStringArray);
    if (isNull(retval)) {
      printGError("failed to finish:", error);
      g_object_unref(builder);
      return retval;
    }
    g_object_unref(builder); 
    return retval;
  }
  proc boolArray(arr: [] bool, validity: [] gboolean) : c_ptr(GArrowBooleanArray) {
    var success: gboolean = 1;
    var error: GErrorPtr;
    var builder: c_ptr(GArrowBooleanArrayBuilder) = garrow_boolean_array_builder_new();
    var retval: c_ptr(GArrowBooleanArray);
    if (success) {
      var boolArrLen: gint64 = arr.size: gint64;
      var boolValArr = [val in arr] val: gboolean;
      var boolValidityArrLen: gint64 = boolArrLen;
      success = garrow_boolean_array_builder_append_values(
                                                           builder, c_ptrTo(boolValArr), boolArrLen, c_ptrTo(validity), boolValidityArrLen, c_ptrTo(error));
    }
    if (!success) {
      printGError("failed to append:", error);
      g_object_unref(builder);
      return retval;
    }
    retval = garrow_array_builder_finish(
                                         GARROW_ARRAY_BUILDER(builder), c_ptrTo(error)) : c_ptr(GArrowBooleanArray);
    if (isNull(retval)) {
      printGError("failed to finish:", error);
      g_object_unref(builder);
      return retval;
    }
    g_object_unref(builder); 
    return retval;
  }
  // ---------------------- Record Batches and schemas ----------------------
  record arrowRecordBatch {
    var rcbatch: c_ptr(GArrowRecordBatch);
    proc init(args ...?n){
      this.rcbatch = recordBatch( (...args) ); // Unpacking the tuple using ...
    }
  }
  proc recordBatch (args ...?n): c_ptr(GArrowRecordBatch) {
    
    // Verifying the Integrity of the arguments
    if(n%2!=0) then
      compilerError("Mismatched arguments");
    for param i in 0..#n {
      if i%2 == 0 {
        if args[i].type != string then
          compilerError("Wrong even argument type");
      } else {
        if args[i].type != arrowArray then
          compilerError("Wrong odd argument type");
      }
    }
    var fields: c_ptr(GList) = nil;
    for param i in 1..n by 2{
      // Building the (column)
      var col: c_ptr(GArrowField) = garrow_field_new(
                                                     args[i-1].c_str(): c_ptr(gchar), 
                                                     garrow_array_get_value_data_type(args[i].val: c_ptr(GArrowArray)));
      // Adding the column to the list
      fields = g_list_append(fields, col);
      // Moving on the the next pair of arguments
    }
    // Gotta build this schema now
    var schema: c_ptr(GArrowSchema) = garrow_schema_new(fields);
    // We might want to check the equality of the arrays length but the error will give it to us
    // anyway if they are not equal
    var n_rows: guint32 = garrow_array_get_length(args[1].val): guint32;
    var arrays: c_ptr(GList) = nil;
    for param j in 1..n by 2 {
      // Adding the array to the list
      arrays = g_list_append(arrays, args[j].val);
    }
    var error: GErrorPtr;
    var record_batch: c_ptr(GArrowRecordBatch) = garrow_record_batch_new(schema, n_rows, arrays, c_ptrTo(error));
    if(isNull(record_batch)){
      g_print("%s\n".c_str(): c_ptr(gchar), error.deref().message);
    }
    // And after a lot of lines of code we have created the record batch.
    // The last part can also be done using a record batch builder class.
    //print_record_batch(record_batch);
    return record_batch;
  }
  record arrowTable {
    var tbl: c_ptr(GArrowTable);
  
    proc init(args: arrowRecordBatch ...?n){
      this.tbl = table( (...args) ); // Unpacking the tuple using ...
    }
    proc init(recordBatches: [] arrowRecordBatch){
      this.tbl = table(recordBatches);
    }
    proc init(table: c_ptr(GArrowTable)){
      this.tbl = table;
    }
  }
  proc table(recordBatches: [] arrowRecordBatch): c_ptr(GArrowTable) {
    var error: GErrorPtr;
    var schema: c_ptr(GArrowSchema) = garrow_record_batch_get_schema(recordBatches[0].rcbatch);
    var rbArray = [rb in recordBatches] rb.rcbatch;
    var retval: c_ptr(GArrowTable) = garrow_table_new_record_batches(
                                                                     schema, c_ptrTo(rbArray), recordBatches.size : guint64, c_ptrTo(error));
    
    if(isNull(retval)){
      g_print("Error creating table: %s\n", error.deref().message);
    }
    return retval;
  }
  proc table(recordBatches: arrowRecordBatch ...?n){
    var error: GErrorPtr;
    var schema: c_ptr(GArrowSchema) = garrow_record_batch_get_schema(recordBatches[0].rcbatch: c_ptr(GArrowRecordBatch));
    var rbArray = [rb in recordBatches] rb.rcbatch;
    var retval: c_ptr(GArrowTable) = garrow_table_new_record_batches(
                                                                     schema, c_ptrTo(rbArray), recordBatches.size : guint64, c_ptrTo(error));
    
    if(isNull(retval)){
      printGError("Error creating table:", error);
    }
    return retval;
  }
  
  proc writeTableToParquetFile(table: arrowTable, path: string) {
    var error: GErrorPtr;
    var writer_properties: c_ptr(GParquetWriterProperties) = gparquet_writer_properties_new();
    var writer: c_ptr(GParquetArrowFileWriter) = gparquet_arrow_file_writer_new_path(
                                                                                     garrow_table_get_schema(table.tbl),
                                                                                     path.c_str(): c_ptr(gchar), 
                                                                                     writer_properties,
                                                                                     c_ptrTo(error));
    if(isNull(writer)){
      printGError("failed to initialize writer:", error);
      exit(EXIT_FAILURE);
    }
    var success: gboolean = gparquet_arrow_file_writer_write_table(writer,
                                                                   table.tbl ,
                                                                   10 : guint64, // Should not be hardcoded
                                                                   c_ptrTo(error));
    if(!success){
      printGError("failed to write table:", error);
      exit(EXIT_FAILURE);
    }
    success = gparquet_arrow_file_writer_close(writer, c_ptrTo(error));
    if(!success){
      printGError("could not close writer:", error);
      exit(EXIT_FAILURE);
    }
  }
  
  //----------------------- Functions for printing ----------------------------
  proc printArray(arr: arrowArray) {
    printArray(arr.val);
  }
  proc printArray(array: c_ptr(GArrowArray)) {
    var error: GErrorPtr;
    var str: c_ptr(gchar) = garrow_array_to_string(array, c_ptrTo(error));
    if(isNull(str)){
      g_print("Failed to print: %s\n".c_str(): c_ptr(gchar), error.deref().message);
      g_error_free(error);
      return;
    }
    g_print("%s\n".c_str(): c_ptr(gchar),str);
  }
  proc printRecordBatch(recordBatch: arrowRecordBatch){
    printRecordBatch(recordBatch.rcbatch);
  }
  proc printRecordBatch(recordBatch: c_ptr(GArrowRecordBatch)) {
    var error: GErrorPtr;
    var str: c_ptr(gchar) = garrow_record_batch_to_string(recordBatch, c_ptrTo(error));
    if(isNull(str)){
      g_print("Failed to print: %s\n".c_str(): c_ptr(gchar), error.deref().message);
      g_error_free(error);
      return;
    }
    g_print("%s\n".c_str(): c_ptr(gchar),str: c_ptr(gchar));
  }
  proc printTable(table: arrowTable) {
    printTable(table.tbl);
  }
  proc printTable(table: c_ptr(GArrowTable)) {
    if(isNull(table)) then return;
    var error: GErrorPtr;
    var str: c_ptr(gchar) = garrow_table_to_string(table, c_ptrTo(error));
    if(isNull(str)){
      g_print("Failed to print: %s\n".c_str(): c_ptr(gchar), error.deref().message);
      g_error_free(error);
      return;
    }
    g_print("%s\n".c_str(): c_ptr(gchar),str: c_ptr(gchar));
  }

  // MY STUFF
  // -------------------------- Parquet -------------------------------------
  proc getSingleSchemaType(schema, idx: int) {
    var field = garrow_schema_get_field(schema, idx:guint);
    var id = garrow_data_type_get_id(garrow_field_get_data_type(field));
    if id == 13 then return "string";
    else return "int";
  }

  record parquetFileWriter {
    var fname: string;
    var chunks: guint64; // Max number of rows
    var table: c_ptr(GArrowTable) = nil;
    var numRows = 0;

    // can't create table til we have the first column
    proc init(fname) {
      this.fname = fname;
    }

    proc addColumn(arr: [] ?eltType, idx: int, colName: string) {
      if arr.size > numRows then numRows = arr.size;
      var col = (new arrowArray(arr)).val;
      var field = garrow_field_new(colName.c_str(): c_ptr(gchar), garrow_array_get_value_data_type(col: c_ptr(GArrowArray)));
      var e: GErrorPtr;
      // need to create table
      if table == nil {
        var f: c_ptr(GList) = nil;

        f = g_list_append(f, field);
        var schema = garrow_schema_new(f);

        var vals: c_ptr(GList);
        vals = g_list_append(vals, col);

        table = garrow_table_new_values(schema, vals, c_ptrTo(e));
        if isNull(table) {
          printGError("failed to create table: ", e);
        }
      } else { // can just add the array
        var l:c_ptr(GList);
        l = g_list_append(l, col);
        var chunk = garrow_chunked_array_new(l);
        table = garrow_table_add_column(table, idx: guint, field, chunk, c_ptrTo(e));
        
      }
    }

    proc finish() {
      var e: GErrorPtr;
      var props: c_ptr(GParquetWriterProperties);
      var writer = gparquet_arrow_file_writer_new_path(garrow_table_get_schema(table),
                                                       fname.c_str(): c_ptr(gchar),
                                                       props,
                                                       c_ptrTo(e));
      if(!gparquet_arrow_file_writer_write_table(writer, table, numRows:guint64, c_ptrTo(e))) {
        printGError("failed to write table:", e);
        exit(EXIT_FAILURE);
      }
      
      if(!gparquet_arrow_file_writer_close(writer, c_ptrTo(e))){
        printGError("could not close writer:", e);
        exit(EXIT_FAILURE);
      }
    }
  }

  proc getArrSizeAndType(filenames: [?D] string, col: int) {
    extern proc strlen(asd): c_int;
    var sizes: [D] int;
    var ty: string;
    for i in D {
      var error: GErrorPtr;
      var reader = gparquet_arrow_file_reader_new_path(filenames[i].c_str(): c_ptr(gchar), c_ptrTo(error));

      var t = gparquet_arrow_file_reader_read_table(reader, c_ptrTo(error));
      sizes[i] = garrow_table_get_n_rows(t):int;

      var schema = garrow_table_get_schema(t);
      var field = garrow_schema_get_field(schema, col: guint);
      var gtype = garrow_data_type_get_id(garrow_field_get_data_type(field));
      // TODO: need to check to make sure all types match
      // TODO: need to add checking against all int types, not just 64
      if(gtype == GARROW_TYPE_INT64) then ty = "int";
      else ty = "string";
    }
    return (sizes, ty);
  }

  proc readFileColumnToDist(A, filename, col) {
    var perLoc = A.size/numLocales;
    
    coforall (loc, i) in zip(A.targetLocales(), 0..#numLocales) do on loc {
      var localFilename = filename;
      var file = new parquetFileReader(localFilename);
      A[i*perLoc..#perLoc] = file.readColumn(col)[i*perLoc..#perLoc];
    }
  }

  proc getSubdomains(lengths: [?FD] int) {
    var subdoms: [FD] domain(1);
    var offset = 0;
    for i in FD {
      subdoms[i] = {offset..#lengths[i]};
      offset += lengths[i];
    }
    return (subdoms, (+ reduce lengths));
  }

  proc domain_intersection(d1: domain(1), d2: domain(1)) {
    var low = max(d1.low, d2.low);
    var high = min(d1.high, d2.high);
    if (d1.stride !=1) && (d2.stride != 1) {
      //TODO: change this to throw
      halt("At least one domain must have stride 1");
    }
    var stride = max(d1.stride, d2.stride);
    return {low..high by stride};
  }
  
  proc readFiles(A, filenames: [] string, sizes: [] int) {
    var (subdoms, length) = getSubdomains(sizes);

    coforall loc in A.targetLocales() do on loc {
        var locFiles = filenames;
        var locFiledoms = subdoms;
        for (filedom, filename) in zip(locFiledoms, locFiles) {
          for locdom in A.localSubdomains() {
            const intersection = domain_intersection(locdom, filedom);
            if intersection.size > 0 {
              var pqReader = new parquetFileReader(filename);
              var col = pqReader.readColumn(0);
              A[filedom] = col;
            }
          }
        }
    }
  }

  proc writeDistArray(A, filename) {
    var filenames: [0..#A.targetLocales().size] string;
    for i in 0..#A.targetLocales().size {
      var suffix = i: string;
      filenames[i] = filename + "_LOCALE" + suffix + ".parquet";
    }

    coforall (loc, idx) in zip(A.targetLocales(), filenames.domain) do on loc {
        const myFilename = filenames[idx];

        var writer = new parquetFileWriter(myFilename);
        var locDom = A.localSubdomain();
        writer.addColumn(A[locDom], 0, "col");
        writer.finish();
      }
  }
  
  // Record that stores a reader for the file and can serve columns
  // as requested. Also stores schema
  record parquetFileReader {
    var path: string;
    var pqFileReader: c_ptr(GParquetArrowFileReader);
    var schema: c_ptr(GArrowSchema);
    proc init(path: string) {
      this.path = path;
      var error: GErrorPtr;
      pqFileReader = gparquet_arrow_file_reader_new_path(this.path.c_str(): c_ptr(gchar), c_ptrTo(error));
      if isNull(pqFileReader) {
        printGError("failed to open file: ", error);
        exit(EXIT_FAILURE);
      }
      schema = gparquet_arrow_file_reader_get_schema(pqFileReader, c_ptrTo(error));
    }

    proc useThreads() {
      gparquet_arrow_file_reader_set_use_threads(pqFileReader, true:gboolean);
    }
    
    proc readColumn(col: int) {
      var error: GErrorPtr;
      var chunk = gparquet_arrow_file_reader_read_column_data(pqFileReader, col: gint, c_ptrTo(error));
      var len = garrow_chunked_array_get_n_rows(chunk);
      var ret: [0..#len] int;
      var loc = garrow_chunked_array_get_chunk(chunk, 0:guint):c_ptr(GArrowInt64Array);
      forall i in 0..#len {
        ret[i] = garrow_int64_array_get_value(loc, i);
      }
      return ret;
    }
    
    proc readColumnStr(col: int) {
      var error: GErrorPtr;
      extern proc strlen(str): int;
      var chunk = gparquet_arrow_file_reader_read_column_data(pqFileReader, col: gint, c_ptrTo(error));
      var len = garrow_chunked_array_get_n_rows(chunk);
      var ret: [0..#len] string;
      
      var loc = garrow_chunked_array_get_chunk(chunk, 0:guint):c_ptr(GArrowStringArray);
      forall i in 0..#len {
        var gstr = garrow_string_array_get_string(loc, i);
        ret[i] = try! createStringWithNewBuffer(gstr:c_string, length=strlen(gstr));
      }
      return ret;
    }
    
    proc writeSchema() {
      extern proc strlen(str): int;
      extern proc g_hash_table_get_keys(a): c_ptr(GList);
      extern proc g_list_length(a): guint;
      var gstr = garrow_schema_to_string(schema);
      var sch = try! createStringWithNewBuffer(gstr:c_string, length=strlen(gstr));
      writeln(sch);

      var m = garrow_schema_get_metadata(schema);
      if !isNull(m) {
        var l = g_hash_table_get_keys(m);
        writeln(g_list_length(l):int);
      }

      gstr = garrow_schema_to_string_metadata(schema, true:gboolean);
      sch = try! createStringWithNewBuffer(gstr:c_string, length=strlen(gstr));
      writeln(sch);
    }
  }
    proc read(path: string) {
    var error: GErrorPtr;
    var pqFileReader: c_ptr(GParquetArrowFileReader) = gparquet_arrow_file_reader_new_path(
                                                                                           path.c_str(): c_ptr(gchar), c_ptrTo(error));
    if(isNull(pqFileReader)){
      printGError("failed to open the file:", error);
      exit(EXIT_FAILURE);
    }
    // Reading the whole table
    var table: c_ptr(GArrowTable) = gparquet_arrow_file_reader_read_table(pqFileReader, c_ptrTo(error));
    if(isNull(table)){
      printGError("failed to read the table:", error);
      exit(EXIT_FAILURE);
    }
    return table;
  }
  proc getColumnType(table, col: int) {
    var chunk = garrow_table_get_column_data(table, col:gint);
    return garrow_chunked_array_get_value_type(chunk);
  }
}
