var D: domain(3) = {1..5, 1..5, 1..5};

writeln(D[2..4, 2..4, 2..4]);

writeln(D[.., .., ..]);

writeln(D(.., 2, 3));
