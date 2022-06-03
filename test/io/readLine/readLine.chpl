use IO;

proc testReadLineRef(type dataType) {
  var f = opentmp();
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
    got = ch.readLine(line, maxSize = 10);
    assert( got && line == "e f g h i ":dataType);
    got = ch.readLine(line, maxSize = 100, stripNewline=true);
    assert( got && line == "j k l m n o p q r s t u v w x y z":dataType);
    got = ch.readLine(line);
    assert( got && line == "Really Long Line this is aliquam vestibulum morbi blandit cursus risus at ultrices mi tempus imperdiet nulla malesuada pellentesque elit eget gravida cum sociis natoque penatibus et magnis dis parturient montes nascetur ridiculus mus mauris vitae ultricies leo integer malesuada nunc vel risus commodo viverra maecenas accumsan lacus vel facilisis volutpat est velit egestas dui id ornare arcu odio ut sem nulla pharetra diam sit amet nisl suscipit adipiscing bibendum est ultricies integer quis auctor elit sed vulputate mi sit amet mauris commodo quis imperdiet massa tincidunt nunc pulvinar sapien et ligula ullamcorper malesuada proin libero nunc consequat interdum varius sit amet mattis vulputate enim\n":dataType);
    got = ch.readLine(line);
    assert( !got );

    writeln("Tests Pass for ",dataType:string, "!");
  }
}


proc testReadLineType(type dataType){
  var f = opentmp();
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
    assert( ch.readLine(dataType, maxSize = 10) == "e f g h i ":dataType);
    assert( ch.readLine(dataType, maxSize = 100, stripNewline=true) == "j k l m n o p q r s t u v w x y z":dataType);
    
    assert( ch.readLine(dataType) == "Really Long Line this is aliquam vestibulum morbi blandit cursus risus at ultrices mi tempus imperdiet nulla malesuada pellentesque elit eget gravida cum sociis natoque penatibus et magnis dis parturient montes nascetur ridiculus mus mauris vitae ultricies leo integer malesuada nunc vel risus commodo viverra maecenas accumsan lacus vel facilisis volutpat est velit egestas dui id ornare arcu odio ut sem nulla pharetra diam sit amet nisl suscipit adipiscing bibendum est ultricies integer quis auctor elit sed vulputate mi sit amet mauris commodo quis imperdiet massa tincidunt nunc pulvinar sapien et ligula ullamcorper malesuada proin libero nunc consequat interdum varius sit amet mattis vulputate enim\n":dataType);
    assert( ch.readLine(dataType) == "":dataType);

    writeln("Tests Pass for ",dataType:string, "!");
  }
}


proc main(){
  testReadLineRef(string);
  testReadLineRef(bytes);
  testReadLineType(string);
  testReadLineType(bytes);
}