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
  use Order only durkon as durkula;
  use Order except durkon;
}

module Future {
  use Godsmoot only durkula as durkon;
  use Godsmoot except durkula;

  proc main() {
    writeln("hail hail, the ", roy, haley, elan, durkon, vaar, belkar, " all here");
  }
}
