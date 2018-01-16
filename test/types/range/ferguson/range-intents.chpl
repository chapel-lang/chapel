var global = 1..10;

writeln("1..10 means a copy was made; 1..20 means some sort of ref");

proc test_blank( arg ) {
  global = 1..20;
  writeln("default ", arg);
}
global = 1..10;
test_blank(global);

proc test_const( const arg ) {
  global = 1..20;
  writeln("const ", arg);
}
global = 1..10;
test_const(global);

proc test_const_in( const in arg ) {
  global = 1..20;
  writeln("const in ", arg);
}
global = 1..10;
test_const_in(global);

proc test_const_ref( const ref arg ) {
  global = 1..20;
  writeln("const ref ", arg);
}
global = 1..10;
test_const_ref(global);

proc test_ref( ref arg ) {
  global = 1..20;
  writeln("ref ", arg);
}
global = 1..10;
test_ref(global);

proc range.test_this() {
  global = 1..20;
  writeln("this ", this);
}
global = 1..10;
global.test_this();

proc const range.test_const_this() {
  global = 1..20;
  writeln("const this ", this);
}
global = 1..10;
global.test_const_this();

proc test_begin(const ref loc) {
  sync {
    begin with (ref global) {
      global = 1..20;
      writeln("begin ", loc);
    }
  }
}
global = 1..10;
test_begin(global);

proc test_begin_in(const ref loc) {
  sync {
    begin with (ref global, in loc) {
      global = 1..20;
      writeln("begin in ", loc);
    }
  }
}
global = 1..10;
test_begin_in(global);

proc test_begin_const_in(const ref loc) {
  sync {
    begin with (ref global, const in loc) {
      global = 1..20;
      writeln("begin const in ", loc);
    }
  }
}
global = 1..10;
test_begin_const_in(global);

proc test_begin_ref(const ref loc) {
  sync {
    begin with (ref global, ref loc) {
      global = 1..20;
      writeln("begin ref ", loc);
    }
  }
}
global = 1..10;
test_begin_ref(global);

proc test_begin_const_ref(const ref loc) {
  sync {
    begin with (ref global, const ref loc) {
      global = 1..20;
      writeln("begin const ref ", loc);
    }
  }
}
global = 1..10;
test_begin_const_ref(global);
