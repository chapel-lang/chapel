extern {
  typedef struct td_struct {
    double d;
  } td_struct;
}

var td_strct: td_struct;
td_strct.d = 7.5;

writeln(td_strct.d);

