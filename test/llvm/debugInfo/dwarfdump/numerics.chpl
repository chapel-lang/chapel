use Debugger;
use BigInteger;

proc main() {
  // DWARFDUMP: myInt8
  var myInt8: int(8) = 8; // TODO: how to get this to not print as a char!
  // DWARFDUMP: myInt16
  var myInt16: int(16) = 16;
  // DWARFDUMP: myInt32
  var myInt32: int(32) = 32;
  // DWARFDUMP: myInt64
  var myInt64: int(64) = 64;
  // DWARFDUMP: myUInt8
  var myUInt8: uint(8) = 8;
  // DWARFDUMP: myUInt16
  var myUInt16: uint(16) = 16;
  // DWARFDUMP: myUInt32
  var myUInt32: uint(32) = 32;
  // DWARFDUMP: myUInt64
  var myUInt64: uint(64) = 64;
  // DWARFDUMP: myReal32
  var myReal32: real(32) = 3.2;
  // DWARFDUMP: myReal64
  var myReal64: real(64) = 6.4;
  // DWARFDUMP: myImag32
  var myImag32: imag(32) = 3.2i;
  // DWARFDUMP: myImag64
  var myImag64: imag(64) = 6.4i;
  // DWARFDUMP: myComplex64
  var myComplex64: complex(64) = 3.2 + 4.0i;
  // DWARFDUMP: myComplex128
  var myComplex128: complex(128) = 6.4 + 8.0i;
  // DWARFDUMP: myBool
  var myBool: bool = false;
  // DWARFDUMP: myBigInt
  var myBigInt = "12345678901234567890":bigint;

  writeln(myInt8, myInt16, myInt32, myInt64, sep=" | ");
  writeln(myUInt8, myUInt16, myUInt32, myUInt64, sep=" | ");
  writeln(myReal32, myReal64, sep=" | ");
  writeln(myImag32, myImag64, sep=" | ");
  writeln(myComplex64, myComplex128, sep=" | ");
  writeln(myBool, myBigInt, sep=" | ");

  breakpoint;
}
