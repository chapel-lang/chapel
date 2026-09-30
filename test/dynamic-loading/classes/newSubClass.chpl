use DynamicLoading;
use CTypes;
import ChplConfig.CHPL_TARGET_PLATFORM;
class A {
  var x:int;
  proc getX() {
    return x;
  }
}
class B : A {
  override proc getX() {
    return x + 1;
  }
}
class C: B {
  override proc getX() {
    return x + 2;
  }
}

proc libSuffix() param do
  return if CHPL_TARGET_PLATFORM == "darwin" then ".dylib" else ".so";
proc libName(name, prefix="", suffix=libSuffix()) {
  return prefix + name + suffix;
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

  var bin = binary.load(libName("libnewSubClassLib", path));
  var callGetX = bin.retrieve("callGetX", proc(ptr: c_ptr(void)): int);

  writeln(callGetX(c_ptrTo(a)));
  writeln(callGetX(c_ptrTo(b)));
  writeln(callGetX(c_ptrTo(c)));

}
