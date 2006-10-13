def main() {

use Random;

var Vector1 = [1..20];
var Vec1 : [Vector1] float;

var rng = RandomStream(333.0,444.0);

rng.fillRandom(Vec1);
writeln(Vec1);

fillRandom(Vec1,333.0,444.0);
writeln(Vec1);
fillRandom(Vec1,333.0,444.0);
writeln(Vec1);
}

