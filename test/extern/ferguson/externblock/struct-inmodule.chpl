module OuterModule {
  module C {
    extern {
      struct td_struct {
        double d;
      };
    }
  }

  use C;
  var td_strct: C.td_struct;
  td_strct.d = 7.5;

  writeln(td_strct.d);
}
