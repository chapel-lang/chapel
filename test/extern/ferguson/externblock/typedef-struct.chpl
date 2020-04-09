extern {
  typedef struct td_struct {
    double d;
  } td_struct_t;
}

var td_strct: td_struct_t;
td_strct.d = 7.5;

writeln(td_strct.d);

