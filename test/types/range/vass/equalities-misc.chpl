var r1: range(int) = 0..1;
var r2: range(uint)= 0..1:uint;
writeln(r1==r2);
writeln(r1!=r2);

writeln( (0..(1:int)) == (0..(1:uint)) );

writeln( [0..(1:int)] == [0..(1:uint)] );
writeln( [0..(1:int)] != [0..(1:uint)] );
