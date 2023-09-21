use Math;

var one = -1;
var two: uint = 2;
var three = -3.0;
var four: imag(64) = -4.0i;
var five: imag(32) = -5.0i;
var six = -6 + 0i;
writeln(abs(i=one));
writeln(abs(i=two));
writeln(abs(i=-3)); // Should trigger the param version
writeln(abs(r=three));
writeln(abs(im=four));
writeln(abs(im=five));
writeln(abs(z=six));
