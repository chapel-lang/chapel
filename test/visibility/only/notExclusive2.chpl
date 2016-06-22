// The only thing that varies between these versions is the order of the
// only/except pairs
module Order {
  var roy = "o";
  var haley = "r";
  var elan = "d";
  var durkon = "e";
  var vaar = "r";
  var belkar = "'s";
}

module Godsmoot {
  use Order except durkon;
  use Order only durkon;
}

module Future {
  use Godsmoot only durkon;
  use Godsmoot except durkon;

  proc main() {
    writeln("hail hail, the ", roy, haley, elan, durkon, vaar, belkar, " all here");
  }
}
