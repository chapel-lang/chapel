

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

x = __primitive("call resolves", "hi0");
assert(x);

x = __primitive("call resolves", "BADhi0");
assert(!x); // no such fn

x = __primitive("call resolves", "hi0", 1);
assert(!x); // too many args

x = __primitive("call resolves", "hi1", 1);
assert(x);

x = __primitive("call resolves", "hi1");
assert(!x); // not enough args

x = __primitive("call resolves", "hi1", "blarg");
assert(!x); // type mismatch

x = __primitive("call resolves", "hi2", 1, 2);
assert(x);

x = __primitive("call resolves", "hi2", 1);
assert(!x); // not enough args

x = __primitive("call resolves", "hi2", "blarg");
assert(!x); // not enough args, wrong type

x = __primitive("call resolves", "hi2", 1, "blarg");
assert(x); 

x = __primitive("call resolves", "hi2", "blarg", 1);
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

x = __primitive("method call resolves", obj, "mymethod0");
assert(x);

x = __primitive("method call resolves", obj, "BADmymethod0");
assert(!x);

x = __primitive("method call resolves", obj, "mymethod0", 1);
assert(!x); // too many args

x = __primitive("method call resolves", obj, "mymethod1", 2);
assert(x);

x = __primitive("method call resolves", obj, "mymethod1");
assert(!x); // not enough args

x = __primitive("method call resolves", obj, "mymethod1", "blarg");
assert(!x); // type mismatch

x = __primitive("method call resolves", obj, "mymethod2", 1, 2);
assert(x);

x = __primitive("method call resolves", obj, "mymethod2", 1);
assert(!x); // not enough args

x = __primitive("method call resolves", obj, "mymethod2", "blarg", 2);
assert(x);

x = __primitive("method call resolves", obj, "mymethod2", 1, "blarg");
assert(!x);

