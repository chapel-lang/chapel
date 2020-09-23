use CPtr;
var global: c_array(int, 3);

var count = 0;

proc setit( arg: c_array(int, 3) ) {
  arg[0] = 0;
  arg[1] = 1;
  arg[2] = 2;
}
proc increment( arg: c_array(int, 3) ) {
  arg[0] += 1;
  arg[1] += 1;
  arg[2] += 1;
}

proc byblank( arg: c_array(int, 3) ) {
  increment(global);
  increment(global);
  increment(arg);
  writeln(arg);
}
proc testblank() {
  writeln("testblank");
  setit(global);
  byblank(global);
  writeln(global);
}
testblank();

proc byconst( const arg: c_array(int, 3) ) {
  increment(global);
  increment(global);
  //increment(arg); - should be an error
  writeln(arg);
}
proc testconst() {
  writeln("testconst");
  setit(global);
  byconst(global);
  writeln(global);
}
testconst();

proc byref( ref arg: c_array(int, 3) ) {
  increment(global);
  increment(global);
  increment(arg);
  writeln(arg);
}
proc testref() {
  writeln("testref");
  setit(global);
  byref(global);
  writeln(global);
}
testref();

proc byconstref( const ref arg: c_array(int, 3) ) {
  increment(global);
  increment(global);
  //increment(arg); - should be an error
  writeln(arg);
}
proc testconstref() {
  writeln("testconstref");
  setit(global);
  byconstref(global);
  writeln(global);
}
testconstref();

proc byin( in arg: c_array(int, 3) ) {
  increment(global);
  increment(global);
  increment(arg);
  writeln(arg);
}
proc testin() {
  writeln("testin");
  setit(global);
  byin(global);
  writeln(global);
}
testin();


proc byconstin( const in arg: c_array(int, 3) ) {
  increment(global);
  increment(global);
  //increment(arg); - should be an error
  writeln(arg);
}
proc testconstin() {
  writeln("testconstin");
  setit(global);
  byconstin(global);
  writeln(global);
}
testconstin();


proc byout( out arg: c_array(int, 3) ) {
  increment(global);
  increment(global);
  increment(arg);
  writeln(arg);
  writeln(global);
}
proc testout() {
  writeln("testout");
  setit(global);
  byout(global);
  writeln(global);
}
testout();


proc byinout( inout arg: c_array(int, 3) ) {
  increment(global);
  increment(global);
  increment(arg);
  writeln(arg);
  writeln(global);
}
proc testinout() {
  writeln("testinout");
  setit(global);
  byinout(global);
  writeln(global);
}
testinout();
