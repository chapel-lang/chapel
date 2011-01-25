class StoreSomeInts {
  var numInts: int;
  var m_someints: [1..numInts] int;

  proc StoreSomeInts(m_someints: [] int) {
    numInts = m_someints.numElements;
    this.m_someints = m_someints;
  }
}

var powers_arr: [1..3] int = (1, 2, 3);
var powers = new StoreSomeInts(powers_arr);
writeln("powers = ", powers);
