var O = [11, 12, 13;
         21, 22, 23];

var P = reshape(O, 1..3, 1..2);
writeln(P);

writeln("---");

var Q = reshape(O, {1..3, 1..2});
writeln(Q);

writeln("---");

var R = reshape(O, 1..3, 1..2, copy=true);
writeln(R);

writeln("---");

var S = reshape(O, {1..3, 1..2}, copy=true);
writeln(S);

