class StoreSomeInts {
  var numInts: int;
  var m_someints: [1..numInts] int;

  proc init(m_someints: [] int) {
    numInts = m_someints.size;
    this.m_someints = m_someints;
  }
}

var powers_arr: [1..3] int = (1, 2, 3);
var powers = new owned StoreSomeInts(powers_arr);
writeln("powers = ", powers);
