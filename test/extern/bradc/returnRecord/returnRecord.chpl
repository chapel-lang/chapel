extern record R {
  var x: int;
}

extern proc foo(): R;

var myR: R;

myR = foo();

writeln("myR is: ", myR);
