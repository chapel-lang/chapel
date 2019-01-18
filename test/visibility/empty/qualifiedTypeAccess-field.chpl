module C {
  record my_struct {
    var my_field:c_int;
  }

  proc make_struct():my_struct {
    var ret:my_struct;
    ret.my_field = 0;
    return ret;
  }
}

proc test() {
  use C only;

  var tmp = C.make_struct();
  var f = tmp.my_field;
  writeln(f);
}

test();