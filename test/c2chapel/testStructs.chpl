
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
  y.a = "Hello".c_str();
  y.b = "World".c_str();
  stringStruct_print(y);

  var z : fnStruct;
  z.fn = c_ptrTo(foo);
  fnStruct_call(z);

  z.fn = c_ptrTo(bar);
  fnStruct_call(z);

  var fds1: forwardDeclStruct1;
  fds1.x = 8;
  printForwardDeclStruct1(fds1);

  var fds2: forwardDeclStruct2;
  fds2.x = 16;
  printForwardDeclStruct2(fds2);
}
