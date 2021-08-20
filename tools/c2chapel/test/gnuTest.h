typedef struct _GValue GValue;

__extension__ typedef unsigned long long guint64;
__extension__ typedef signed long long gint64;
typedef double gdouble;
typedef unsigned long gsize;
typedef gsize GType;


struct _GValue
{
  GType g_type;

  union {
    gdouble v_double;
  } data[2];
};
