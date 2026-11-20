module BlockEntry {
  use BlockDist;

  class Entry {
    type eltType = int;
    var values: [blockDist.createDomain({0..<20})] eltType;
  }
}
