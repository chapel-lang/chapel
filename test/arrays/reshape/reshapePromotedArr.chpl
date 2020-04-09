var A = reshape([1, 2, 3, 4]:uint(8), {1..2, 1..2});
writeln(A);
writeln(A.domain);
writeln(A.type:string);
