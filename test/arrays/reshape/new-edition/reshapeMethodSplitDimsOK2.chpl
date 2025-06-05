var O = [11, 12, 13;
         21, 22, 23];

var P = reshape(O, 1..3, 1..2, checkDims=false);
writeln(P);

writeln("---");

var Q = reshape(O, {1..3, 1..2}, checkDims=false);
writeln(Q);

