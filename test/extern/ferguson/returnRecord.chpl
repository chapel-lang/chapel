extern record R {
  var a,b,c,d,e,f,g,h,i,j,k: int(64);
}

extern proc foo(): R;

var myR: R;

myR = foo();

writeln("myR is: ", myR);
