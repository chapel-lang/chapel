use Reflection;

proc hi1(x:int) {
  writeln("hi", x);
}

proc hi2(x:int, y) {
  writeln("hi", x);
}


var one8 = 1:uint(8);
var one16 = 1:uint(16);
var one = 1;
var x:bool;

x = __primitive("call resolves", "hi1", one);
assert(x);

// check that coercions are handled in can resolve
x = __primitive("call resolves", "hi1", one8);
assert(x);

x = __primitive("call resolves", "hi1", one16);
assert(x);

x = __primitive("call resolves", "hi2", one, one);
assert(x);

x = __primitive("call resolves", "hi2", one8, one8);
assert(x);

proc string.mymethod1(x:int) {
  writeln(this, x);
}
proc string.mymethod2(x, y:int) {
  writeln(this, x, y);
}

var obj="hi";

x = __primitive("method call resolves", obj, "mymethod1", one);
assert(x);

x = __primitive("method call resolves", obj, "mymethod1", one8);
assert(x);

x = __primitive("method call resolves", obj, "mymethod1", one16);
assert(x);

x = __primitive("method call resolves", obj, "mymethod2", one, one);
assert(x);

x = __primitive("method call resolves", obj, "mymethod2", one8, one8);
assert(x);
