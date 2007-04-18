var arr : [1..5] sync int = 0;

var dummy : sync int = 0; 
var arr2: [1..5] sync int = readXX(dummy);

writeln("arr is: ", arr);
writeln("arr2 is: ", arr2);
