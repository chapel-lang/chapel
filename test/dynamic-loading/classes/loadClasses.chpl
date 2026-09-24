use myClasses;
use CTypes;
use DynamicLoading;
import ChplConfig.CHPL_TARGET_PLATFORM;

proc libSuffix() param do
  return if CHPL_TARGET_PLATFORM == "darwin" then ".dylib" else ".so";
proc libName(name, prefix="", suffix=libSuffix()) {
  return prefix + name + suffix;
}
config const path = "./lib/";

proc main() {
  var c = new C();
  var b = new B();
  var a = new A();

  callAllMethods(c, b, a);
  doDynamicCasts(c.borrow());
  doDynamicCasts(b.borrow():borrowed C);
  doDynamicCasts(a.borrow():borrowed C);

  writeln("Loading Library");
  var lib = binary.load(libName("libmyClassesLib", path));

  writeln("Calling classes made in library");
  var callMyClasses = lib.retrieve("callMyClasses", proc():void);
  callMyClasses();

  writeln("Calling classes from this program in the library");
  var getAndCall = lib.retrieve("getAndCall", proc(c: c_ptr(void), b: c_ptr(void), a: c_ptr(void)):void);
  getAndCall(c_ptrTo(c), c_ptrTo(b), c_ptrTo(a));

  writeln("Doing dynamic casts");
  var getAndDyncCast = lib.retrieve("getAndDyncCast", proc(ptr: c_ptr(void)):void);
  getAndDyncCast(c_ptrTo(c));
  getAndDyncCast(c_ptrTo(b));
  getAndDyncCast(c_ptrTo(a));

  writeln("Getting new classes from the library");
  var getC = lib.retrieve("getC", proc(): c_ptr(void));
  var getB = lib.retrieve("getB", proc(): c_ptr(void));
  var getA = lib.retrieve("getA", proc(): c_ptr(void));

  var newC = (getC():unmanaged C?)!;
  var newB = (getB():unmanaged B?)!;
  var newA = (getA():unmanaged C?)!;

  callAllMethods(newC, newB, newA);
  doDynamicCasts(newC.borrow());
  doDynamicCasts(newB.borrow():borrowed C);
  doDynamicCasts(newA.borrow():borrowed C);


}
