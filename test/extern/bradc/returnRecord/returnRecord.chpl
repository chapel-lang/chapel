extern record R {
  var x: int(32);
}

extern proc foo(): R;

var myR: R;

myR = foo();

writeln("myR is: ", myR);
