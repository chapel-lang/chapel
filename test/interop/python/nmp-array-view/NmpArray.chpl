module NmpArray {
  use CTypes;
  use List;
  use HaltWrappers;

  /**
  */
  class Array {
    var rank: int;
    var dims: [0..<rank] int;

    /**
    */
    proc ptrToBuffer(): c_void_ptr {
      pureVirtualMethodHalt();
      return nil;
    }

    /**
    */
    proc rank(): int {
      return rank;
    }

    /**
    */
    proc display(): void {
      pureVirtualMethodHalt();
    }
  }

  pragma "no doc"
  class ArrayConcrete : Array {
    param paramRank: int;
    type T;
    var dom: domain(paramRank);
    var arr: [dom] T;

    proc init(param paramRank: int, type T, dims: [0..<paramRank] int) {
      super.init(paramRank, dims);
      this.paramRank = paramRank;
      this.T = T;
      this.complete();

      // Build the tuple of dimension values.
      var tup: paramRank*range;
      for i in dims.domain {
        var dim = dims[i];
        if dim <= 0 then halt('invalid dimension value: ' + dim:string);
        tup[i] = 0..<dim;
      }

      // Set up the domain (and by extension, the array elements).
      dom = tup;
    }

    override proc ptrToBuffer(): c_void_ptr {
      var ret = c_ptrTo(arr);
      return ret;
    }

    override proc display(): void {
      writeln(arr);
    }
  }

  /**
  */
  proc type Array.create(rank: int, type T, dims: [0..<rank] int) {
    var ret: unmanaged Array? = nil;
    select rank {
      when 1 do ret = new unmanaged ArrayConcrete(1, T, dims);
      when 2 do ret = new unmanaged ArrayConcrete(2, T, dims);
      when 3 do ret = new unmanaged ArrayConcrete(3, T, dims);
      when 4 do ret = new unmanaged ArrayConcrete(4, T, dims);
      when 5 do ret = new unmanaged ArrayConcrete(5, T, dims);
      when 6 do ret = new unmanaged ArrayConcrete(5, T, dims);
      when 7 do ret = new unmanaged ArrayConcrete(5, T, dims);
      when 8 do ret = new unmanaged ArrayConcrete(5, T, dims);
      otherwise do boundsCheckHalt('unsupported rank: ' + rank:string);
    }
    assert(ret != nil);
    return ret;
  }
}

