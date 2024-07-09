public use BlockDist except testFastFollowerOptimization;
public use CyclicDist except testFastFollowerOptimization;
public use BlockCycDist;
public use HashedDist;
public use StencilDist;

config type distType = blockDist;

proc createDom(space) {
  if space.rank > 2 {
    compilerError("Only 1 and 2 dimensional domains");
  }

  if distType == blockDist {
    return blockDist.createDomain(space);
  }
  else if distType == cyclicDist {
    return cyclicDist.createDomain(space);
  }
  else if distType == blockCycDist {
    if space.rank == 1 {
      return space dmapped new blockCycDist(startIdx=(space.low,), blocksize=(2,));
    }
    else {
      return space dmapped new blockCycDist(startIdx=space.low, blocksize=(2,2));
    }
  }
  else if distType == stencilDist {
    if space.rank == 1 {
      return space dmapped new stencilDist(space, fluff=(1,));
    }
    else {
      return space dmapped new stencilDist(space, fluff=(1,1));
    }
  }
  else if distType == hashedDist {
    var D: domain(int) dmapped new hashedDist(idxType=int);
    for i in space {
      D += i;
    }
    return D;
  }
  else {
    compilerError("Unrecognized distribution type");
  }
}

proc createArr(space, type t) {
  var D = createDom(space);
  var A: [D] t;
  return A;
}
