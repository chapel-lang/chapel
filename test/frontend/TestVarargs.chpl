use Print;

var x = 0;
var y = 0.0;
var z = false;

proc foo(const args...?n) {
  if n >= 1 then println(args[0]);
  if n >= 2 then println(args[1]);
  if n >= 3 then println(args[2]);
}

foo(x);
foo(x, y);
foo(x, y, z);

proc helper(args...) {
   foo((...args));
}
helper(1, 2, 3);

proc unpacker() {
  var x = (4, 5, 6);
  foo((...x));
}
unpacker();
