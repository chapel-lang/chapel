// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "gnuTest.h";

// Note: Generated with fake std headers

use CTypes;
// Anonymous union or struct was encountered within and skipped.
extern "struct _GValue" record _GValue {
  var g_type : GType;
}

// ==== c2chapel typedefs ====

extern type GType = gsize;

extern type GValue = _GValue;

extern type gdouble = c_double;

extern type gint64 = c_longlong;

extern type gsize = c_ulong;

extern type guint64 = c_ulonglong;

