public use BlockDist;
public use CyclicDist;
public use BlockCycDist;
public use HashedDist;
public use StencilDist;

config type distType = Block;

proc createDom(space) {
  if space.rank > 2 {
    compilerError("Only 1 and 2 dimensional domains");
  }

  if distType == Block {
    return newBlockDom(space);
  }
  else if distType == Cyclic {
    return newCyclicDom(space);
  }
  else if distType == BlockCyclic {
    if space.rank == 1 {
      return space dmapped BlockCyclic(startIdx=(space.low,), blocksize=(2,));
    }
    else {
      return space dmapped BlockCyclic(startIdx=space.low, blocksize=(2,2));
    }
  }
  else if distType == Stencil {
    if space.rank == 1 {
      return space dmapped Stencil(space, fluff=(1,));
    }
    else {
      return space dmapped Stencil(space, fluff=(1,1));
    }
  }
  else if distType == Hashed {
    var D: domain(int) dmapped Hashed(idxType=int);
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

