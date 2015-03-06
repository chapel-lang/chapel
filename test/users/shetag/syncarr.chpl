var arr : [1..5] sync int = 0;

var dummy : sync int = 0; 
var arr2: [1..5] sync int = dummy.readXX();

writeln("arr is: ", arr.readFF());
writeln("arr2 is: ", arr2.readFF());
