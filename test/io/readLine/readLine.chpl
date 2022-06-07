use IO;

proc testReadLineRef(type dataType) {
  var f = openmem();
  {
    var ch = f.writer();
    ch.writeln("a b":dataType);
    ch.writeln("c d":dataType);
    ch.writeln("e f g h i j k l m n o p q r s t u v w x y z":dataType);
    ch.writeln("Really Long Line this is aliquam vestibulum morbi blandit cursus risus at ultrices mi tempus imperdiet nulla malesuada pellentesque elit eget gravida cum sociis natoque penatibus et magnis dis parturient montes nascetur ridiculus mus mauris vitae ultricies leo integer malesuada nunc vel risus commodo viverra maecenas accumsan lacus vel facilisis volutpat est velit egestas dui id ornare arcu odio ut sem nulla pharetra diam sit amet nisl suscipit adipiscing bibendum est ultricies integer quis auctor elit sed vulputate mi sit amet mauris commodo quis imperdiet massa tincidunt nunc pulvinar sapien et ligula ullamcorper malesuada proin libero nunc consequat interdum varius sit amet mattis vulputate enim":dataType);
    ch.flush();
  }

  {
    var ch = f.reader();
    var line:dataType;
    var got:bool;
    got = ch.readLine(line);
    assert( got && line == "a b\n":dataType);
    got = ch.readLine(line, stripNewline=true);
    assert( got && line == "c d":dataType);
    try {got = ch.readLine(line, maxSize=10, stripNewline=true);}
    catch e {writeln(e);}
    got = ch.readLine(line, maxSize = 100, stripNewline=true);
    assert( got && line == "e f g h i j k l m n o p q r s t u v w x y z":dataType);
    got = ch.readLine(line);
    assert( got && line == "Really Long Line this is aliquam vestibulum morbi blandit cursus risus at ultrices mi tempus imperdiet nulla malesuada pellentesque elit eget gravida cum sociis natoque penatibus et magnis dis parturient montes nascetur ridiculus mus mauris vitae ultricies leo integer malesuada nunc vel risus commodo viverra maecenas accumsan lacus vel facilisis volutpat est velit egestas dui id ornare arcu odio ut sem nulla pharetra diam sit amet nisl suscipit adipiscing bibendum est ultricies integer quis auctor elit sed vulputate mi sit amet mauris commodo quis imperdiet massa tincidunt nunc pulvinar sapien et ligula ullamcorper malesuada proin libero nunc consequat interdum varius sit amet mattis vulputate enim\n":dataType);
    got = ch.readLine(line);
    assert( !got );

    writeln("Tests Pass for ",dataType:string, "!");
  }
}


proc testReadLineType(type dataType){
  var f = openmem();
  {
    var ch = f.writer();
    ch.writeln("a b":dataType);
    ch.writeln("c d":dataType);
    ch.writeln("e f g h i j k l m n o p q r s t u v w x y z":dataType);
    ch.writeln("Really Long Line this is aliquam vestibulum morbi blandit cursus risus at ultrices mi tempus imperdiet nulla malesuada pellentesque elit eget gravida cum sociis natoque penatibus et magnis dis parturient montes nascetur ridiculus mus mauris vitae ultricies leo integer malesuada nunc vel risus commodo viverra maecenas accumsan lacus vel facilisis volutpat est velit egestas dui id ornare arcu odio ut sem nulla pharetra diam sit amet nisl suscipit adipiscing bibendum est ultricies integer quis auctor elit sed vulputate mi sit amet mauris commodo quis imperdiet massa tincidunt nunc pulvinar sapien et ligula ullamcorper malesuada proin libero nunc consequat interdum varius sit amet mattis vulputate enim":dataType);
    ch.flush();
  }

  {
    var ch = f.reader();
    assert( ch.readLine(dataType) == "a b\n":dataType);
    assert( ch.readLine(dataType, stripNewline=true) == "c d":dataType);
    try { ch.readLine(dataType, maxSize = 10);}
    catch e {writeln(e);}
    assert( ch.readLine(dataType, maxSize = 100, stripNewline=true) == "e f g h i j k l m n o p q r s t u v w x y z":dataType);

    assert( ch.readLine(dataType) == "Really Long Line this is aliquam vestibulum morbi blandit cursus risus at ultrices mi tempus imperdiet nulla malesuada pellentesque elit eget gravida cum sociis natoque penatibus et magnis dis parturient montes nascetur ridiculus mus mauris vitae ultricies leo integer malesuada nunc vel risus commodo viverra maecenas accumsan lacus vel facilisis volutpat est velit egestas dui id ornare arcu odio ut sem nulla pharetra diam sit amet nisl suscipit adipiscing bibendum est ultricies integer quis auctor elit sed vulputate mi sit amet mauris commodo quis imperdiet massa tincidunt nunc pulvinar sapien et ligula ullamcorper malesuada proin libero nunc consequat interdum varius sit amet mattis vulputate enim\n":dataType);
    assert( ch.readLine(dataType) == "":dataType);

    writeln("Tests Pass for ",dataType:string, "!");
  }
}

proc testReadLineArray(){
  var f = openmem();
  {
    var ch = f.writer();
    ch.writeln("a b");
    ch.writeln("c d");
    ch.writeln("e f g h i j k l m n o p q r s t u v w x y z");
    ch.writeln("Really Long Line this is aliquam vestibulum morbi blandit cursus risus at ultrices mi tempus imperdiet nulla malesuada pellentesque elit eget gravida cum sociis natoque penatibus et magnis dis parturient montes nascetur ridiculus mus mauris vitae ultricies leo integer malesuada nunc vel risus commodo viverra maecenas accumsan lacus vel facilisis volutpat est velit egestas dui id ornare arcu odio ut sem nulla pharetra diam sit amet nisl suscipit adipiscing bibendum est ultricies integer quis auctor elit sed vulputate mi sit amet mauris commodo quis imperdiet massa tincidunt nunc pulvinar sapien et ligula ullamcorper malesuada proin libero nunc consequat interdum varius sit amet mattis vulputate enim");
    ch.flush();
  }

  {
    var ch = f.reader();
    var numRead : int;

    var array : [0..300] uint(8);
    numRead = ch.readLine(array);
    check_expected(array, "a b\n", numRead);
    numRead = ch.readLine(array, maxSize=10, stripNewline=true);
    check_expected(array, "c d",numRead);
    try {numRead = ch.readLine(array, maxSize=10, stripNewline=true);}
    catch e {writeln(e);}
    // We should have reverted to the front of the line again.
    numRead = ch.readLine(array);
    check_expected(array, "e f g h i j k l m n o p q r s t u v w x y z\n", numRead);

    writeln("Tests pass for Array!");
  }
}

/*
Borrowed the check_expected() function from test/io/casella/readline.chpl */
proc check_expected(data, expected:string, len) {
  for i in 0..#min(len, expected.numBytes) {
    var n = data[i];
    var c = expected[i:byteIndex];
    if n != c.toByte() {
      writeln("miscompare at ", i, ": expected ", c, " (", c.toByte(), "), got ", n);
    }
  }

  if len < expected.numBytes {
    writeln("Short read -- expected ", expected.numBytes, " bytes, got ", len);
  } else if len > expected.numBytes {
    writeln("Got extra ", len - expected.numBytes, " bytes: ");
    writeln(data[expected.numBytes+1..len]);
  }
}

proc main(){
  testReadLineRef(string);
  testReadLineRef(bytes);
  testReadLineType(string);
  testReadLineType(bytes);
  testReadLineArray();
}
