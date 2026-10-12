use Debugger;
use CTypes;
proc main() {
  // DWARFDUMP: myAtomicBool
  // DWARFDUMP: atomic bool
  // DWARFDUMP: myAtomicInt8
  // DWARFDUMP: atomic int(8)
  // DWARFDUMP: myAtomicInt16
  // DWARFDUMP: atomic int(16)
  // DWARFDUMP: myAtomicInt32
  // DWARFDUMP: atomic int(32)
  // DWARFDUMP: myAtomicInt64
  // DWARFDUMP: atomic int(64)
  // DWARFDUMP: myAtomicUInt8
  // DWARFDUMP: atomic uint(8)
  // DWARFDUMP: myAtomicUInt16
  // DWARFDUMP: atomic uint(16)
  // DWARFDUMP: myAtomicUInt32
  // DWARFDUMP: atomic uint(32)
  // DWARFDUMP: myAtomicUInt64
  // DWARFDUMP: atomic uint(64)
  // DWARFDUMP: myAtomicReal32
  // DWARFDUMP: atomic real(32)
  // DWARFDUMP: myAtomicReal64
  // DWARFDUMP: atomic real(64)
  // DWARFDUMP: myAtomicPtr
  // DWARFDUMP: atomic c_ptr(int(64))
  // DWARFDUMP: myAtomicCPtr
  // DWARFDUMP: atomic c_ptrConst(real(64))
  var myAtomicBool: atomic bool = false;
  var myAtomicInt8: atomic int(8) = -5;
  var myAtomicInt16: atomic int(16);
  var myAtomicInt32: atomic int(32) = -32;
  var myAtomicInt64: atomic int(64) = 17;
  var myAtomicUInt8: atomic uint(8) = 5;
  var myAtomicUInt16: atomic uint(16);
  var myAtomicUInt32: atomic uint(32) = 32;
  var myAtomicUInt64: atomic uint(64);
  var myAtomicReal32: atomic real(32) = 1.5;
  var myAtomicReal64: atomic real(64) = 2.5;
  var myInt: int = 42;
  var myAtomicPtr: atomic c_ptr(int) = c_ptrTo(myInt);
  var myAtomicCPtr: atomic c_ptrConst(real);

  writeln(myAtomicBool.read(), myAtomicInt8.read(), myAtomicInt16.read(),
          myAtomicInt32.read(), myAtomicInt64.read(), sep=" | ");
  writeln(myAtomicUInt8.read(), myAtomicUInt16.read(),
          myAtomicUInt32.read(), myAtomicUInt64.read(), sep=" | ");
  writeln(myAtomicReal32.read(), myAtomicReal64.read(), sep=" | ");
  writeln(myAtomicPtr.read().deref(), myAtomicCPtr.read(), sep=" | ");
  breakpoint;
}
