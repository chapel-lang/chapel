use List;

type L = list(int);

const l1: L = 1..3, l2: L = 100..105, l3: L = 9..12;
writeln(concat(l1, l2, l3));
