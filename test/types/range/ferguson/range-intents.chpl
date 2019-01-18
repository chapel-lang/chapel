// This test runs a series of test methods to verify
// that range behaves similarly to int as far as
// argument intents are concerned.

config param testrange = true;

proc make(x:int) {
  if testrange then
    return 1..x;
  else
    return x;
}
proc upper(x) { return x.high; }
proc upper(x:int) { return x; }

var start = make(10);
var change = make(20);

writeln(upper(start), " means a copy was made; ", upper(change), " means some sort of ref");

var global = start;

proc test_blank( arg ) {
  global = change;
  writeln("default ", upper(arg));
}
global = start;
test_blank(global);

proc test_const( const arg ) {
  global = change;
  writeln("const ", upper(arg));
}
global = start;
test_const(global);

proc test_in( in arg ) {
  global = change;
  writeln("in ", upper(arg));
}
global = start;
test_in(global);

proc test_const_in( const in arg ) {
  global = change;
  writeln("const in ", upper(arg));
}
global = start;
test_const_in(global);

proc test_const_ref( const ref arg ) {
  global = change;
  writeln("const ref ", upper(arg));
}
global = start;
test_const_ref(global);

proc test_ref( ref arg ) {
  global = change;
  writeln("ref ", upper(arg));
}
global = start;
test_ref(global);

proc range.test_this() {
  global = change;
  writeln("this ", upper(this));
}
proc int.test_this() {
  global = change;
  writeln("this ", upper(this));
}
global = start;
global.test_this();

proc const range.test_const_this() {
  global = change;
  writeln("const this ", upper(this));
}
proc const int.test_const_this() {
  global = change;
  writeln("const this ", upper(this));
}
global = start;
global.test_const_this();

// We don't support 'in' or 'const in' as 'this' intents.
// If we did, I'd want these functions uncommented to test it.

/*proc in range.test_in_this() {
  global = change;
  writeln("in this ", upper(this));
}
proc in int.test_in_this() {
  global = change;
  writeln("in this ", upper(this));
}
global = start;
global.test_in_this();
*/
/*proc const in range.test_const_in_this() {
  global = change;
  writeln("const in this ", upper(this));
}
proc const in int.test_const_in_this() {
  global = change;
  writeln("const in this ", upper(this));
}
global = start;
global.test_const_in_this();
*/
proc const ref range.test_const_ref_this() {
  global = change;
  writeln("const ref this ", upper(this));
}
proc const ref int.test_const_ref_this() {
  global = change;
  writeln("const ref this ", upper(this));
}
global = start;
global.test_const_ref_this();

proc ref range.test_ref_this() {
  global = change;
  writeln("ref this ", upper(this));
}
proc ref int.test_ref_this() {
  global = change;
  writeln("ref this ", upper(this));
}
global = start;
global.test_ref_this();

proc test_begin(const ref loc) {
  sync {
    begin with (ref global) {
      global = change;
      writeln("begin ", upper(loc));
    }
  }
}
global = start;
test_begin(global);

proc test_begin_in(const ref loc) {
  sync {
    begin with (ref global, in loc) {
      global = change;
      writeln("begin in ", upper(loc));
    }
  }
}
global = start;
test_begin_in(global);

proc test_begin_const_in(const ref loc) {
  sync {
    begin with (ref global, const in loc) {
      global = change;
      writeln("begin const in ", upper(loc));
    }
  }
}
global = start;
test_begin_const_in(global);

proc test_begin_ref(const ref loc) {
  sync {
    begin with (ref global, ref loc) {
      global = change;
      writeln("begin ref ", upper(loc));
    }
  }
}
global = start;
test_begin_ref(global);

proc test_begin_const_ref(const ref loc) {
  sync {
    begin with (ref global, const ref loc) {
      global = change;
      writeln("begin const ref ", upper(loc));
    }
  }
}
global = start;
test_begin_const_ref(global);
