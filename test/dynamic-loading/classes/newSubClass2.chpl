use DynamicLibrary;
use CTypes;
use newSubClassCommon;

// The executable and library define different types named C.
class C: B {
  override proc getX() {
    return x + 2;
  }
}

config const path = "./lib/";

proc main() {
  var a = new A(1);
  var b = new B(1);
  var c = new C(1);

  writeln(a.getX());
  writeln(b.getX());
  writeln(c.getX());
  writeln("="*80);

  var lib = load("newSubClass2Lib", directory=path);
  var callGetX = lib.retrieve("callGetX", proc(ptr: c_ptr(void)): int);

  writeln(callGetX(c_ptrTo(a)));
  writeln(callGetX(c_ptrTo(b)));
  writeln(callGetX(c_ptrTo(c)));

}
