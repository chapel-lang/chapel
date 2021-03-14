module Order {
  var roy = "o";
  var haley = "r";
  var elan = "d";
  var durkon = "e";
  var vaar = "r";
  var belkar = "'s";
}

module Godsmoot {
  public use Order only durkon as durkula;
  public use Order except durkon;
}

module Future {
  public use Godsmoot except durkula;
  public use Godsmoot only durkula as durkon;

  proc main() {
    writeln("hail hail, the ", roy, haley, elan, durkon, vaar, belkar, " all here"); 
  }
}
