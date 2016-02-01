use Reflection;

proc hi(args...) {
  writeln((...args));
}

var x:bool;

x = canResolve("hi", 1);
assert(x);

x = canResolve("hi", 1, 2);
assert(x);

x = canResolve("hi", "a", 2);
assert(x);

proc string.mymethod(args...) {
  writeln((...args));
}

var obj="hi";

x = canResolveMethod(obj, "mymethod", 1);
assert(x);

x = canResolveMethod(obj, "mymethod", 1, 2);
assert(x);

x = canResolveMethod(obj, "mymethod", "a", 2);
assert(x);
