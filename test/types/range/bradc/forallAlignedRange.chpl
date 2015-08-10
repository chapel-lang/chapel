var x: [1..64] real = [i in 1..64] i;

for i in 1..64 by 16 align 9 do
  writeln(i);

writeln((1..64 by 16 align 9).member(41));

var tmp: [1..4] real = x[1..64 by 16 align 9];;

writeln(tmp);

forall (a,b) in zip(tmp, x[1..64 by 16 align 9]) {
  a = -b;
}

writeln(tmp);


