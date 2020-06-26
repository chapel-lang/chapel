use BlockDist;
use CyclicDist;
use BlockCycDist;
use HashedDist;
use StencilDist;

config type distType = Block;

proc createDom(space) {
  if distType == Block {
    return newBlockDom(space);
  }
  else if distType == Cyclic {
    return newCyclicDom(space);
  }
  else if distType == BlockCyclic {
    return newBlockCycDom(space);
  }
  else if distType == Stencil {
    if space.rank == 1 {
      return space dmapped Stencil(space, fluff=(1,));
    }
    else if space.rank == 2 {
      return space dmapped Stencil(space, fluff=(1,1));
    }
    else {
      compilerError("Only 1 and 2 dimensional domains");
    }
  }
  else if distType == Hashed {
    var D: domain(string) dmapped Hashed(idxType=string);
    for i in space {
      D += i:string;
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

