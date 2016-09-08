
type A1 = [1..2] int;
type A2 = [1..2] uint;
type A3 = [1..2] uint(8);

writeln(1:A1);
writeln(1:A2);
writeln(1:A3);
writeln();

writeln(_prod_id(A1));
writeln(_prod_id(A2));
writeln(_prod_id(A3));
writeln();

writeln(_bor_id(A1));
writeln(_bor_id(A2));
writeln(_bor_id(A3));
writeln();

writeln(_bxor_id(A1));
writeln(_bxor_id(A2));
writeln(_bxor_id(A3));
writeln();
