use utilities;
use dsiMethods;
use templates;

config const N = 10;

const Space = {10..#N, 20..#N, 30..#N};
const Dom = Space;

writeln(Dom._value.dsiPartialDomain(0));
writeln(Dom._value.dsiPartialDomain(1));
writeln(Dom._value.dsiPartialDomain(2));
