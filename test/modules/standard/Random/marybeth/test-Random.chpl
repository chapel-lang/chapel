def main() {

use Random;

var Vector1 = [1..20];
var Vec1 : [Vector1] real;

var rng = RandomStream(333);

rng.fillRandom(Vec1);
writeln(Vec1);

fillRandom(Vec1,333);
writeln(Vec1);
fillRandom(Vec1,333);
writeln(Vec1);
}

