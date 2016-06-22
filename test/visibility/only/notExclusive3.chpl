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
  use Godsmoot except durkon;
  use Godsmoot only durkon;

  proc main() {
    writeln("hail hail, the ", roy, haley, elan, durkon, vaar, belkar, " all here");
  }
}
