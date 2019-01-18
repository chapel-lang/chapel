class BaseReader {
  proc readPrimitive(inout x) {
    writeln("Generic Primitive ", x.type:string);
  }
  proc read(inout x) {
    readPrimitive(x);
  }
}

class SubReader : BaseReader {
  override proc readPrimitive(inout x) {
    writeln("Sub Primitive ", x.type:string);
  }
}

var s1 = new borrowed SubReader();

var x:int;
var y:real;

s1.read(x);
s1.read(y);
