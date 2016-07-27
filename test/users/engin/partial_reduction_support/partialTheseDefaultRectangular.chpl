config const N = 10;

//basic 2D
const dom2 = {10..#N, 20..#N};

forall i in dom2._value.dsiPartialThese(1) do writeln(i);
/*writeln();*/
forall i in dom2._value.dsiPartialThese(2) do writeln(i);
/*writeln();*/

//get weird
const dom3 = {30..#N by 3, 40..#N by -2, 50..#N by 4 align 3};
forall i in dom3._value.dsiPartialThese(1) do writeln(i);
/*writeln();*/
forall i in dom3._value.dsiPartialThese(2) do writeln(i);
/*writeln();*/
forall i in dom3._value.dsiPartialThese(3) do writeln(i);
/*writeln();*/
