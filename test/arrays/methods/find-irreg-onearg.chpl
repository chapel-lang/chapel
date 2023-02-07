enum color {red, green, blue};
use color;

config param which = 0;

var A: [false..true] real = [1.2, 3.4];
var B: [red..blue] real = [5.6, 7.8, 9.0];
var C = [red=>1.3, green=>2.4, blue=>3.5];
var bi = false;
var ei = green;

ref Arr = if which==0 then A else if which==1 then B else C;

writeln(Arr.find(0.0));

