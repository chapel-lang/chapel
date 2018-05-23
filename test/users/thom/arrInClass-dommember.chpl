class StoreSomeInts {
  var intsDom: domain(1);
  var m_someints: [intsDom] int;

  proc init(m_someints: [] int) {
    intsDom = m_someints.domain;
    this.m_someints = m_someints;
  }
}

var powers_arr: [1..3] int = (1, 2, 3);
var powers                 = new borrowed StoreSomeInts(powers_arr);

writeln("powers = ", powers);
