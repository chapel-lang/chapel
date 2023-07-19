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

var ownS1 = new owned SubReader();
var s1 = ownS1.borrow();

var x:int;
var y:real;

s1.read(x);
s1.read(y);
