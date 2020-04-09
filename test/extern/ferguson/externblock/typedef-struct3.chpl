extern {
  struct td_struct {
    double d;
  };
  typedef struct td_struct td_struct_t;
}

var td_strct: td_struct;
td_strct.d = 7.5;

writeln(td_strct.d);

