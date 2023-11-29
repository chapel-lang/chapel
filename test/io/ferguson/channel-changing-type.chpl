use IO;

proc test1() {
  var f = openMemFile();

  {
    var ch = f.writer(); // defaults to dynamic, text, locking
    // make a binary, unlocked channel using same buffer as ch
    var cha = ch.withSerializer(new binarySerializer(endianness.big));
    cha.write(1);
  }

  // check the output
  {
    var i: int;
    f.reader().readBinary(i, endianness.big);
    assert(i == 1);
  }
}
test1();

proc test2() {
  var f = openMemFile();

  {
    var ch = f.writer(); // defaults to dynamic, text, locking
    // make a binary, unlocked channel using same buffer as ch
    var cha = ch.withSerializer(new binarySerializer(endianness.big));
    cha.write(1);
  }

  // check the output
  {
    var i: int;
    f.reader().readBinary(i, endianness.big);
    assert(i == 1);
  }
}
test2();
