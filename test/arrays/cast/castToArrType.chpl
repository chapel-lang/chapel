var A = [1, 2, 3, 4]: [0..3] uint(8);
writeln(A);
writeln(A.type:string);

var tmp = (reshape([1,2,3,4], {0..1, 0..1}): [{0..1, 0..1}] uint(8));
writeln(tmp);
writeln(tmp.type:string);
