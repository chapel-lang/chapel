_extern record R {
  var x: int;
}

_extern proc foo(): R;

var myR: R;

myR = foo();

writeln("myR is: ", myR);
