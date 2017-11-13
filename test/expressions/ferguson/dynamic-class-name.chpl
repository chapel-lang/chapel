proc getNameFromClass(obj:object) : string
{
  var cid =  __primitive("getcid", obj);
  var cs: c_string = __primitive("class name by id", cid);
  var str = cs:string;
  return str;
}

class Parent {
  var x:int;
}

class Child : Parent {
  var y:int;
}

var x:Parent = new Parent(1);
var y:Parent = new Child(1,2);
writeln(getNameFromClass(x), " ", x);
writeln(getNameFromClass(y), " ", y);

delete y, x;
