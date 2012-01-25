proc myfun(x:int) {
  writeln("Called myfun ", x);
}

extern proc c_func(inout x):int(32);

var x:volatile int;
x = 5;

myfun(x);

var y = c_func(x);

writeln(y);


