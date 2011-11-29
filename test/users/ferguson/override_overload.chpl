class BaseWriter {
  proc writeIt(x) {
    writeln("Generic Writer.writeIt called");
    //compilerError("Generic Writer.writeIt called");
  }
  proc write(x) {
    writeIt(x);
  }
}

class SubWriterOne : BaseWriter {
  proc writeIt(x) {
    writeln("in SubWriterOne writing ", x);
  }
}

class SubWriterTwo : BaseWriter {
  proc writeIt(x:int) {
    writeln("in SubWriterTwo writing int ", x);
  }
  proc writeIt(x) {
    writeln("in SubWriterTwo writing ", x);
  }
}

var s1 = new SubWriterOne();
var s2 = new SubWriterTwo();

var s1w = s1:BaseWriter;
var s2w = s2:BaseWriter;
writeln("Calling from a type BaseWriter");
s1w.write(5);
s1w.write("baz");
s2w.write(7);
s2w.write("bar");


