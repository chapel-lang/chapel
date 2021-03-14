//Vars as key/vals
var Foo = "foo";
var Bar = "bar";
var arr1 = [Foo => "bar", Bar => Foo];
writeln(arr1);

//Consts as key/vals
const C1 = 1;
const C3 = 3;
const C4 = 4; 
var arr2 = ["1" => C1, C3:string => C3, "4" => C4];
writeln(arr2);

//Params as key/vals
param P1 = 3.1;
param P2 = 9.45;
param P3 = 3.21;
param P4 = 900.6;
var arr3 = [P1 => P1:string, P3 => "9.45", P2 => P2:string, P4 => P4:string];
writeln(arr3);

//A mix as key/vals
var arr4 = [C1:string => 1.0, "2" => P4, Foo => 3.0];
writeln(arr4);
