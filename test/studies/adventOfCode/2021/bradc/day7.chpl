use IO;

var horizontal: [0..8] int;

iter readHorizontals() {
  var pos: int;
  while read(pos) {
    yield pos;
    try {
      readf(",");
    } catch {
      return;
    }
  }
}

var positions = readHorizontals();

writeln(positions);

var maxpos = max reduce positions;

var fuel: [0..maxpos] int;

forall target in 0..maxpos do
  fuel[target] = + reduce (abs(positions-target));

writeln(fuel);


const (units,target) = minloc reduce zip(fuel, fuel.domain);

writeln("spend ", units, " fuel to meet at target ", target);

