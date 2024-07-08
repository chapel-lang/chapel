// this is a library function `Reflection.getFieldRef`, specifically.
import Reflection;

config param useOwned = true;
type fieldType = if useOwned then owned FieldType? else shared FieldType?;

class FieldType {
  var x: int;
  proc setup() {
    ref xRef = Reflection.getFieldRef(this, 0);
    xRef = 20;
  }
}

record MainType {
  var field: fieldType;

  proc init() {
    if useOwned then
      this.field = new owned FieldType();
    else
      this.field = new shared FieldType();
    init this;
    this.field!.setup();  // Resolved Bug: non-lvalue actual is passed to 'ref'
                          // formal 'this' of setup()

    assert(this.field != nil);
  }
}

var obj: MainType;
writeln(obj);
