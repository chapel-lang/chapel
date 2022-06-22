use IO;

proc test1() {
  // test a variety of reads when near the end of file and no newline exists 
  writeln("test1");
  var f = openmem();
  f.writer().write("événement");

  var s: string = "foo";

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
    var got = r.readLine(s, maxSize=9, stripNewline=true);
    assert(got);
    assert(s == "événement");
  }
  writeln("test1 part 2b");
  try! {
    var r = f.reader();
    // OK because there was no newline
    var got = r.readLine(s, maxSize=9, stripNewline=false);
    assert(got);
    assert(s == "événement");
  }


  writeln("test1 part 3a");
  try! {
    var r = f.reader();
    // OK
    var got = r.readLine(s, maxSize=10, stripNewline=true);
    assert(got);
    assert(s == "événement");
  }
  writeln("test1 part 3b");
  try! {
    var r = f.reader();
    // OK
    var got = r.readLine(s, maxSize=10, stripNewline=false);
    assert(got);
    assert(s == "événement");
  }

  writeln("test1 part 4a");
  try! {
    var r = f.reader();
    // OK
    var got = r.readLine(s, maxSize=11, stripNewline=true);
    assert(got);
    assert(s == "événement");
  }
  writeln("test1 part 4b");
  try! {
    var r = f.reader();
    // OK
    var got = r.readLine(s, maxSize=11, stripNewline=false);
    assert(got);
    assert(s == "événement");
    // check EOF
    got = r.readLine(s);
    assert(!got);
  }
}
test1();

proc test2() {
  // test a variety of reads when near the end of file and newline exists 
  writeln("test2");
  var f = openmem();
  f.writer().write("événement\n");

  var s: string = "foo";

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
    var got = r.readLine(s, maxSize=9, stripNewline=true);
    assert(got);
    assert(s == "événement");
  }
  writeln("test2 part 2b");
  try! {
    var r = f.reader();
    // should fail because data + newline is 10 codepoints
    r.readLine(s, maxSize=9, stripNewline=false);
    writeln("should not be reached!");
  } catch e {
    writeln("caught expected error: ", e);
  }

  writeln("test2 part 3a");
  try! {
    var r = f.reader();
    // OK
    var got = r.readLine(s, maxSize=10, stripNewline=true);
    assert(got);
    assert(s == "événement");
  }
  writeln("test2 part 3b");
  try! {
    var r = f.reader();
    // OK
    var got = r.readLine(s, maxSize=10, stripNewline=false);
    assert(got);
    assert(s == "événement\n");
  }

  writeln("test2 part 4a");
  try! {
    var r = f.reader();
    // OK
    var got = r.readLine(s, maxSize=11, stripNewline=true);
    assert(got);
    assert(s == "événement");
  }
  writeln("test2 part 4b");
  try! {
    var r = f.reader();
    // OK
    var got = r.readLine(s, maxSize=11, stripNewline=false);
    assert(got);
    assert(s == "événement\n");
    // check EOF
    got = r.readLine(s);
    assert(!got);
  }
}
test2();
