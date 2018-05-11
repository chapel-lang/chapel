class StoreSomeInts {
  var intsDom: domain(1);
  var m_someints: [intsDom] int;

  proc init(intTuple:int...?k) {
    intsDom = {1..k};
    this.m_someints = intTuple;
  }
}

var powers = new borrowed StoreSomeInts(1, 2, 3);

writeln("powers = ", powers);
