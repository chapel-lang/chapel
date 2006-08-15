class _myadomain : _domain {
  param rank : int;
  var ranges : (rank * _aseq);

  def expand(dim : (rank*int)) {
    writeln("in the tuple expand");

    var x = _myadomain(rank);
    for i in 1..rank do {
      if (dim(i) == 0) {
        x.ranges(i) = ranges(i);
      } else {
        x.ranges(i) = ranges(i)._expand(dim(i));
        if (x.ranges(i)._low > x.ranges(i)._high) {
          halt("***Error: Degenerate dimension created in dimension ", i, "***");
        }
      }
    }
    return x;
  }  
 
  def expand(dim : int ...?numDims) {
    if (rank == numDims) {
      writeln("numDims: ", numDims);
      writeln("rank: ", rank);

      return expand(dim);
    } else {
      halt("rank/numDims mismatch");
    }
  }

}

var x = _myadomain(2);

writeln(x.expand(1));
