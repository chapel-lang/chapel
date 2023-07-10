
use structGen;
use CTypes;

proc foo(x : int) {
  writeln("In foo function, given: ", x);
}

proc bar(x: int) {
  writeln("In bar function! got: ", x);
}

proc main() {
  var x : intStruct;
  x.a = 1;
  x.b = 2;
  x.c = 3;
  intStruct_print(x);

  var y : stringStruct;
  y.a = "Hello".c_ptr_c_char();
  y.b = "World".c_ptr_c_char();
  stringStruct_print(y);

  // var yy : stringStruct;
  // yy.a = c_ptrToConst_helper("Hello");
  // yy.b = c_ptrToConst_helper("World");
  // stringStruct_print(yy);

  var z : fnStruct;
  z.fn = c_ptrTo(foo);
  fnStruct_call(z);

  z.fn = c_ptrTo(bar);
  fnStruct_call(z);
}
