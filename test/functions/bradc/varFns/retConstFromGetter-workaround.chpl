var   zero = 0;
var data = 1;

proc myVarFn() var {
  if setter then
    return data;
  else
    return zero;
}

writeln("data is: ", data);

var x = myVarFn();

writeln("x is: ", x);

myVarFn() = x;

writeln("data is: ", data);

writeln("zero is: ", zero);
