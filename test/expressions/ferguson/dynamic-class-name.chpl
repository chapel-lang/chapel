proc getNameFromClass(obj:borrowed object) : string
{
  var cid =  __primitive("getcid", obj);
  var cs: c_string = __primitive("class name by id", cid);
  var str = createStringWithNewBuffer(cs);
  return str;
}

class Parent {
  var x:int;
}

class Child : Parent {
  var y:int;
}

var x:owned Parent = new owned Parent(1);
var y:owned Parent = new owned Child(1,2);
writeln(getNameFromClass(x), " ", x);
writeln(getNameFromClass(y), " ", y);
