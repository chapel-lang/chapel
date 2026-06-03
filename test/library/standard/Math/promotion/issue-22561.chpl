import Math;

var Arr: [1..5] real = for i in 1..5 do (i*Math.pi + Math.pi/2);

writeln(Math.sin(Arr));
Arr = Math.sin(Arr);
writeln(Arr);
