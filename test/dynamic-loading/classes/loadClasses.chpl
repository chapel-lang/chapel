use myClasses;
use CTypes;
import DynamicLibrary;

config const path = "./lib/";

proc main() {
  var c = new C();
  var b = new B();
  var a = new A();

  callAllMethods(c, b, a);
  doDynamicCasts(c.borrow():borrowed A?);
  doDynamicCasts(b.borrow():borrowed A?);
  doDynamicCasts(a.borrow():borrowed A?);

  writeln("Loading Library");
  var lib = DynamicLibrary.load("loadClassesLib", directory=path);

  var producer = DynamicLibrary.load("arrayProducer", directory=path);
  var makeC = producer.retrieve("makeC", proc(): c_ptr(void));
  var checkAndDeleteC = producer.retrieve("checkAndDeleteC", proc(ptr: c_ptr(void)): void);
  var updateArray = lib.retrieve("updateArray", proc(ptr: c_ptr(void)): void);
  var ptr = makeC();
  updateArray(ptr);
  checkAndDeleteC(ptr);

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
  var newA = (getA():unmanaged A?)!;

  callAllMethods(newC, newB, newA);
  doDynamicCasts(newC.borrow():borrowed A?);
  doDynamicCasts(newB.borrow():borrowed A?);
  doDynamicCasts(newA.borrow():borrowed A?);


}
