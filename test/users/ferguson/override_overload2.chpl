class BaseReader {
  proc readPrimitive(inout x) {
    writeln("Generic Primitive ", typeToString(x.type));
  }
  proc read(inout x) {
    readPrimitive(x);
  }
}

class SubReader : BaseReader {
  proc readPrimitive(inout x) {
    writeln("Sub Primitive ", typeToString(x.type));
  }
}

var s1 = new SubReader();

var x:int;
var y:real;
s1.read(x);
s1.read(y);

