// this is a library function `Reflection.getFieldRef`, specifically.
import Reflection;

class FieldType {
  var x: int;
  proc setup() {
    ref xRef = Reflection.getFieldRef(this, 0);
    xRef = 20;
  }
}

record MainType {
  var field: owned FieldType?;

  proc init() {
    this.field = new FieldType();
    init this;
    this.field!.setup();  // Resolved Bug: non-lvalue actual is passed to 'ref'
                          // formal 'this' of setup()

    assert(this.field != nil);
  }
}

var obj: MainType;
writeln(obj);
