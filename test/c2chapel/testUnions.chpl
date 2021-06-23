use unionGen;
use CPtr;

proc foo(x : int) {
  writeln("In foo function, given: ", x);
}

proc bar(x: int) {
  writeln("In bar function! got: ", x);
}

proc main() {
  var x : intUnion;
  x.a = 1;
  x.b = 2;
  x.c = 3;
  intUnion_print(x);

  var y : stringUnion;
  y.a = c"Hello";
  y.b = c"World";
  stringUnion_print(y);

  var z : fnUnion;
  z.fn = c_ptrTo(foo);
  fnUnion_call(z);

  z.fn = c_ptrTo(bar);
  fnUnion_call(z);
}

