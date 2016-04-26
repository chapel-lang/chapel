use Reflection;

proc hi0() {
  writeln("hi");
}

proc hi1(x:int) {
  writeln("hi", x);
}

proc hi2(x:int, y) {
  writeln("hi", x);
}


var x:bool;

x = canResolve("hi0");
assert(x);

x = canResolve("BADhi0");
assert(!x); // no such fn

x = canResolve("hi0", 1);
assert(!x); // too many args

x = canResolve("hi1", 1);
assert(x);

x = canResolve("hi1");
assert(!x); // not enough args

x = canResolve("hi1", "blarg");
assert(!x); // type mismatch

x = canResolve("hi2", 1, 2);
assert(x);

x = canResolve("hi2", 1);
assert(!x); // not enough args

x = canResolve("hi2", "blarg");
assert(!x); // not enough args, wrong type

x = canResolve("hi2", 1, "blarg");
assert(x); 

x = canResolve("hi2", "blarg", 1);
assert(!x); // wrong type

proc string.mymethod0() {
  writeln(this);
}
proc string.mymethod1(x:int) {
  writeln(this, x);
}
proc string.mymethod2(x, y:int) {
  writeln(this, x, y);
}

var obj="hi";

x = canResolveMethod(obj, "mymethod0");
assert(x);

x = canResolveMethod(obj, "BADmymethod0");
assert(!x);

x = canResolveMethod(obj, "mymethod0", 1);
assert(!x); // too many args

x = canResolveMethod(obj, "mymethod1", 2);
assert(x);

x = canResolveMethod(obj, "mymethod1");
assert(!x); // not enough args

x = canResolveMethod(obj, "mymethod1", "blarg");
assert(!x); // type mismatch

x = canResolveMethod(obj, "mymethod2", 1, 2);
assert(x);

x = canResolveMethod(obj, "mymethod2", 1);
assert(!x); // not enough args

x = canResolveMethod(obj, "mymethod2", "blarg", 2);
assert(x);

x = canResolveMethod(obj, "mymethod2", 1, "blarg");
assert(!x);

