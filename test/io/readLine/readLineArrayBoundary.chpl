use IO;

var evenementArr:[0..8] uint(8) = ["e".toByte(),
                                   "v".toByte(),
                                   "e".toByte(),
                                   "n".toByte(),
                                   "e".toByte(),
                                   "m".toByte(),
                                   "e".toByte(),
                                   "n".toByte(),
                                   "t".toByte()];

proc isEvenement(arr) {
  for i in 0..8 {
    if arr[i] != evenementArr[i] {
      return false;
    }
  }
  return true;
}
proc isEvenementNewline(arr) {
  for i in 0..8 {
    if arr[i] != evenementArr[i] {
      return false;
    }
  }
  if arr[9] != "\n".toByte() {
    return false;
  }

  return true;
}


proc test1() {
  // test a variety of reads when near the end of file and no newline exists 
  writeln("test1");
  var f = openmem();
  f.writer().write("evenement");

  var s:[0..100] uint(8);

  writeln("test1 part 1a");
  try {
    var r = f.reader();
    r.readLine(s, maxSize=1, stripNewline=true);
    writeln("should not be reached!");
  } catch e {
    writeln("caught expected error: ", e);
  }

  writeln("test1 part 1b");
  try {
    var r = f.reader();
    r.readLine(s, maxSize=1, stripNewline=false);
    writeln("should not be reached!");
  } catch e {
    writeln("caught expected error: ", e);
  }

  writeln("test1 part 2a");
  try! {
    var r = f.reader();
    // OK because there was no newline
    s = 255;
    var got = r.readLine(s, maxSize=9, stripNewline=true);
    assert(got==9);
    assert(isEvenement(s));
    assert(s[9] == 255);
  }
  writeln("test1 part 2b");
  try! {
    var r = f.reader();
    // OK because there was no newline
    s = 255;
    var got = r.readLine(s, maxSize=9, stripNewline=false);
    assert(got==9);
    assert(isEvenement(s));
    assert(s[9] == 255);
  }

  writeln("test1 part 3a");
  try! {
    var r = f.reader();
    // OK
    s = 255;
    var got = r.readLine(s, maxSize=10, stripNewline=true);
    assert(got==9);
    assert(isEvenement(s));
    assert(s[9] == 255);
  }
  writeln("test1 part 3b");
  try! {
    var r = f.reader();
    // OK
    s = 255;
    var got = r.readLine(s, maxSize=10, stripNewline=false);
    assert(got==9);
    assert(isEvenement(s));
    assert(s[9] == 255);
  }

  writeln("test1 part 4a");
  try! {
    var r = f.reader();
    // OK
    s = 255;
    var got = r.readLine(s, maxSize=11, stripNewline=true);
    assert(got==9);
    assert(isEvenement(s));
    assert(s[9] == 255);
  }
  writeln("test1 part 4b");
  try! {
    var r = f.reader();
    // OK
    s = 255;
    var got = r.readLine(s, maxSize=11, stripNewline=false);
    assert(got==9);
    assert(isEvenement(s));
    assert(s[9] == 255);

    // check EOF handling
    s = 255;
    got = r.readLine(s);
    assert(got==0);
    assert(s[0] == 255);
  }
}
test1();

proc test2() {
  // test a variety of reads when near the end of file and newline exists 
  writeln("test2");
  var f = openmem();
  f.writer().write("evenement\n");

  var s:[0..100] uint(8);

  writeln("test2 part 1a");
  try {
    var r = f.reader();
    r.readLine(s, maxSize=1, stripNewline=true);
    writeln("should not be reached!");
  } catch e {
    writeln("caught expected error: ", e);
  }

  writeln("test2 part 1b");
  try {
    var r = f.reader();
    r.readLine(s, maxSize=1, stripNewline=false);
    writeln("should not be reached!");
  } catch e {
    writeln("caught expected error: ", e);
  }

  writeln("test2 part 2a");
  try! {
    var r = f.reader();
    // OK because the newline was stripped
    s = 255;
    var got = r.readLine(s, maxSize=9, stripNewline=true);
    assert(got==9);
    assert(isEvenement(s));
    assert(s[9] == 255);
  }
  writeln("test2 part 2b");
  try! {
    var r = f.reader();
    // should fail because data + newline is 10 bytes
    r.readLine(s, maxSize=9, stripNewline=false);
    writeln("should not be reached!");
  } catch e {
    writeln("caught expected error: ", e);
  }

  writeln("test2 part 3a");
  try! {
    var r = f.reader();
    // OK
    s = 255;
    var got = r.readLine(s, maxSize=10, stripNewline=true);
    assert(got==9);
    assert(isEvenement(s));
    assert(s[9] == 255);
  }
  writeln("test2 part 3b");
  try! {
    var r = f.reader();
    // OK
    s = 255;
    var got = r.readLine(s, maxSize=10, stripNewline=false);
    assert(got==10);
    assert(isEvenementNewline(s));
    assert(s[10] == 255);
  }

  writeln("test2 part 4a");
  try! {
    var r = f.reader();
    // OK
    s = 255;
    var got = r.readLine(s, maxSize=11, stripNewline=true);
    assert(got==9);
    assert(isEvenement(s));
    assert(s[9] == 255);
  }
  writeln("test2 part 4b");
  try! {
    var r = f.reader();
    // OK
    s = 255;
    var got = r.readLine(s, maxSize=11, stripNewline=false);
    assert(got==10);
    assert(isEvenementNewline(s));
    assert(s[10] == 255);

    // check EOF handling
    s = 255;
    got = r.readLine(s);
    assert(got==0);
    assert(s[0] == 255);
  }
}
test2();
