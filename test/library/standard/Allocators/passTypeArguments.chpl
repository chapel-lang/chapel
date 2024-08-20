use Allocators;

class MyClass {
  type myType;
  var myVar: myType;
  proc init(type myType, myVar: myType) {
    this.myType = myType;
    this.myVar = myVar;
  }
}

var pool = new mallocWrapper();
var c = newWithAllocator(pool, unmanaged MyClass?, int, 17);
writeln(c.type: string, ": ", c);
deleteWithAllocator(pool, c);

