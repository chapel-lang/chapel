var global = 10;

writeln("10 means a copy was made; 20 means some sort of ref");

proc test_begin() {
  const ref loc = global;
  sync {
    begin with (ref global) {
      global = 20;
      writeln("begin ", loc);
    }
  }
}
global = 10;
test_begin();

proc test_begin_in() {
  const ref loc = global;
  sync {
    begin with (ref global, in loc) {
      global = 20;
      writeln("begin in ", loc);
    }
  }
}
global = 10;
test_begin_in();

proc test_begin_const_in() {
  const ref loc = global;
  sync {
    begin with (ref global, const in loc) {
      global = 20;
      writeln("begin const in ", loc);
    }
  }
}
global = 10;
test_begin_const_in();

proc test_begin_ref() {
  const ref loc = global;
  sync {
    begin with (ref global, ref loc) {
      global = 20;
      writeln("begin ref ", loc);
    }
  }
}
global = 10;
test_begin_ref();

proc test_begin_const_ref() {
  const ref loc = global;
  sync {
    begin with (ref global, const ref loc) {
      global = 20;
      writeln("begin const ref ", loc);
    }
  }
}
global = 10;
test_begin_const_ref();
