use IO;

enum myenum {red, green, blue}
use myenum;

writeln("WRITELN OF ENUM VALS");
writeln(myenum.red);
writeln(myenum.green);
writeln(myenum.blue);
writeln();

writeln("JSON WRITE OF ENUM VALS");
writef("%jt\n", myenum.red);
writef("%jt\n", myenum.green);
writef("%jt\n", myenum.blue);
writeln();

proc reade(s: string) : myenum {
  var f = openmem();
  f.writer().write(s);
    
  var reader = f.reader();

  var e: myenum = myenum.red;
  reader.read(e);
  return e;
}

proc readj(s: string) : myenum {
  var f = openmem();
  f.writer().write(s);
    
  var reader = f.reader();

  var e: myenum = myenum.red;
  reader.readf("%jt", e);
  return e;
}


writeln("READING ENUM VALS WITH READ");
writeln('reading red -> ', reade('red'));
writeln('reading green -> ', reade('green'));
writeln('reading blue -> ', reade('blue'));
writeln('reading myenum.red -> ', reade('myenum.red'));
writeln('reading myenum.green -> ', reade('myenum.green'));
writeln('reading myenum.blue -> ', reade('myenum.blue'));
writeln();

writeln("READING ENUM VALS WITH JSON READ");
writeln('reading "red" -> ', readj('"red"'));
writeln('reading "green" -> ', readj('"green"'));
writeln('reading "blue" -> ', readj('"blue"'));
writeln('reading "myenum.red" -> ', readj('"myenum.red"'));
writeln('reading "myenum.green" -> ', readj('"myenum.green"'));
writeln('reading "myenum.blue" -> ', readj('"myenum.blue"'));
writeln();
