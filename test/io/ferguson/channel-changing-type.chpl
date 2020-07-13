use IO;

proc test1() {
  var f = openmem();

  {
    var ch = f.writer(); // defaults to dynamic, text, locking
    // make a binary, unlocked channel using same buffer as ch
    var cha: channel(writing=true, kind=iokind.big, locking=false);
    cha; // no split init
    cha = ch;
    cha.write(1);
  }

  // check the output
  {
    var i: int;
    f.reader().readf("%>8i", i);
    assert(i == 1);
  }
}
test1();

proc test2() {
  var f = openmem();

  {
    var ch = f.writer(); // defaults to dynamic, text, locking
    // make a binary, unlocked channel using same buffer as ch
    var cha: channel(writing=true, kind=iokind.big, locking=false) = ch;
    cha.write(1);
  }

  // check the output
  {
    var i: int;
    f.reader().readf("%>8i", i);
    assert(i == 1);
  }
}
test2();
