var   zero = 0;
var data = 1;

proc myVarFn() ref {
  return data;
}
proc myVarFn()  {
  return zero;
}


writeln("data is: ", data);

var x = myVarFn();

writeln("x is: ", x);

myVarFn() = x;

writeln("data is: ", data);

writeln("zero is: ", zero);
