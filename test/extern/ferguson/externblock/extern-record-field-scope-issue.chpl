module OuterModule {
  module C {
    extern {
      typedef struct my_struct {
        int my_field;
      } my_struct;

      static my_struct make_struct(void) {
        my_struct ret;
        ret.my_field = 0;
        return ret;
      }
    }
  }

  proc test() {
    var tmp = C.make_struct();
    var f = tmp.my_field;
    writeln(f);
  }

  test();
}
