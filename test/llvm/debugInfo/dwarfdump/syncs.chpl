use Debugger;

class MyClass {
  var x: int = 17;
}
record myRecord {
  var x;
}
enum myEnum {
  E1
}

proc main() {
  // DWARFDUMP: mySyncInt
  // DWARFDUMP: sync int(64)
  // DWARFDUMP: _qthreads_synccls(int(64))
  // DWARFDUMP: mySyncBool
  // DWARFDUMP: sync bool
  // DWARFDUMP: _qthreads_synccls(bool)
  // DWARFDUMP: mySyncReal
  // DWARFDUMP: sync real(64)
  // DWARFDUMP: _synccls(real(64))
  // DWARFDUMP: mySyncImag
  // DWARFDUMP: sync imag(64)
  // DWARFDUMP: _synccls(imag(64))
  // DWARFDUMP: mySyncComplex
  // DWARFDUMP: sync complex(128)
  // DWARFDUMP: _synccls(complex(128))
  // DWARFDUMP: mySyncRecord
  // DWARFDUMP: sync myRecord(int(64))
  // DWARFDUMP: _synccls(myRecord(int(64)))
  // DWARFDUMP: mySyncRecord
  // DWARFDUMP: sync myRecord(owned MyClass)
  // DWARFDUMP: _synccls(myRecord(owned MyClass))
  // DWARFDUMP: mySyncEnum
  // DWARFDUMP: sync myEnum
  // DWARFDUMP: _synccls(myEnum)
  // DWARFDUMP: mySyncClass
  // DWARFDUMP: sync shared MyClass
  // DWARFDUMP: _synccls(shared MyClass)
  // DWARFDUMP: mySyncNilableClass
  // DWARFDUMP: sync borrowed MyClass?
  // DWARFDUMP: _synccls(borrowed MyClass?)
  // DWARFDUMP: mySyncString
  // DWARFDUMP: sync string
  // DWARFDUMP: _synccls(string)
  // DWARFDUMP: mySyncBytes
  // DWARFDUMP: sync bytes
  // DWARFDUMP: _synccls(bytes)
  var mySyncInt: sync int = 1;
  var mySyncBool: sync bool = true;
  var mySyncReal: sync real = 3.14;
  var mySyncImag: sync imag = 2.71i;
  var mySyncComplex: sync complex = 1.0 + 2.0i;
  var mySyncRecord: sync myRecord(int) = new myRecord(5);
  var mySyncRecordWrapped: sync myRecord(owned MyClass) = new myRecord(new owned MyClass());
  var mySyncEnum: sync myEnum = myEnum.E1;
  var mySyncClass: sync shared MyClass = new shared MyClass();
  var mySyncNilableClass: sync borrowed MyClass? = nil;
  var mySyncString: sync string = "hello";
  var mySyncBytes: sync bytes = b"world";

  writeln(mySyncInt.readFE(), mySyncBool.readFE(), mySyncReal.readFE(), mySyncImag.readFE(), mySyncComplex.readFE(), sep =" | ");
  writeln(mySyncRecord.readFE(), mySyncRecordWrapped.readFE(), mySyncEnum.readFE(), mySyncClass.readFE(), mySyncNilableClass.readFE(), sep =" | ");
  writeln(mySyncString.readFE(), mySyncBytes.readFE(), sep =" | ");
  breakpoint;
}
