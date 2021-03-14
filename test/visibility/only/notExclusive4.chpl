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
  public use Order only durkon;
  public use Order except durkon;
}

module Future {
  public use Godsmoot except durkon;
  public use Godsmoot only durkon;

  proc main() {
    writeln("hail hail, the ", roy, haley, elan, durkon, vaar, belkar, " all here");
  }
}
