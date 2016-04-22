class StoreSomeInts {
  var intsDom: domain(1);
  var m_someints: [intsDom] int;

  proc StoreSomeInts(intTuple:int...?k) {
    intsDom = {1..k};
    this.m_someints = intTuple;
  }
}

var powers = new StoreSomeInts(1, 2, 3);

writeln("powers = ", powers);

delete powers;

