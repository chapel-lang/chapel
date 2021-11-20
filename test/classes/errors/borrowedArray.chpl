record Chunk {
  type eltType;
  var D: domain(1);
  var data: borrowed [D] eltType;

  proc init(type eltType) {
    this.eltType = eltType;
  }

  proc init(array: [?X] ?t) {
    this.eltType = t;
    this.D = X;
    this.data = array;
  }
}

proc main()
{
  var x = new Chunk([1, 2, 3]);
}
