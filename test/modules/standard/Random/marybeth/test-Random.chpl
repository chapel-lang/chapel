def main() {

use Random;

var Vector1 = [1..20];
var Vec1 : [Vector1] real;

var rng = RandomStream(334);

rng.fillRandom(Vec1);
writeln(Vec1);

fillRandom(Vec1,334);
writeln(Vec1);
fillRandom(Vec1,334);
writeln(Vec1);
}

