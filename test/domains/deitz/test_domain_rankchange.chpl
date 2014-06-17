var D = {1..5, 2..6, 3..7};

writeln(D[1..5, 1..5, 1..5]);
writeln(D[2..6, 0..4, 2..4]);
writeln(D[3.., ..3, ..]);
writeln();
writeln(D[1, 1..7, 1..7]);
writeln(D[1..7, 2, 1..7]);
writeln(D[1..7, 1..7, 3]);
writeln();
writeln(D[1..7, 2, 3]);
writeln(D[1, 1..7, 3]);
writeln(D[1, 2, 1..7]);
writeln();
writeln(D[.., 2, 3]);
writeln(D[1, .., 3]);
writeln(D[1, 2, ..]);
