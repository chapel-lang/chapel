// modified from https://github.com/chapel-lang/chapel/issues/14370
use Map;

class C	{
  var x: int;
}

var m: map(string, shared C);

// original test used `this` accessor
// which is invalid for non-default initializable classes on an empty slot
// correctly throws an error in this case
// m["hi"] = new shared C(42);

m.add("hi", new shared C(42));

proc foo1(): borrowed C {
  return m["hi"].borrow();
}
proc foo2(): borrowed C {
  return m["hi"]; 
}

{
  var elm = foo1();
  writeln(elm, " ", elm.type:string);
}
{
  var elm = foo2();
  writeln(elm, " ", elm.type:string);
}
