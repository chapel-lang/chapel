use OwnedObject;

class MyClass {
  var data:int;
}

record R {
  // TODO - get init with owned fields working
  var c:Owned(MyClass);// = new Owned(nil:MyClass);
  proc init() {
    //var tmp = new Owned(new MyClass(data));
    //c = tmp;
    //c.retain(new MyClass(data));
  }
  pragma "safe"
  proc get(): MyClass {
    return c.borrow();
  }
}

record MyCollection {
  var a: R;
  var b: R;
  proc init() {
  }
  pragma "safe"
  proc this(i:int): MyClass {
    if i == 1 then
      return a.get();
    else
      return b.get();
  }
  iter these(): MyClass {
    yield a.get();
    yield b.get();
  }
  pragma "safe"
  proc returnsNil() {
    return nil:MyClass;
  }
}

pragma "safe"
proc ok5() {
  var group:MyCollection;
  group.a.c.retain(new MyClass(1));
  group.b.c.retain(new MyClass(2));

  var first:MyClass = nil;

  for i in group {
    if first == nil then
      first = i;
  }
}



proc test() {
  ok5();
}

test();
