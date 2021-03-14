use IO;

proc test(param expect, args ...)
{
  param isPrim = _can_stringify_direct(args);
  writeln("expect ", expect, " for ", args.type:string, " isPrim=", isPrim);
}

test(true, 1); // true
test(true, 1.0); // true
test(true, "a"); // true

test(true, 1, 1); // true
test(true, 1.0, 1.0); // true
test(true, "a", "a"); // true
test(true, "a", 1); // true


record R {
  var x:int;
}
class C {
  var x:int;
}


test(false, new R(10)); // false
test(false, new borrowed C(10)); // false

test(false, new R(10), 1); // false
test(false, 1, new R(10), 1); // false
