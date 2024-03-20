class AbstractParent {
  // less generic fields
  const runtimeRankValue: int;

    proc init(runtimeRankValue: int) {
        this.runtimeRankValue = runtimeRankValue;
    }

  proc getDomain(): domain(?) {
    halt("Pure virutal method");
  } 
}

class DerivedChild: AbstractParent {  
  param rank: int;
  param strides: strideKind;
  var dom: domain(rank, strides=strides); 
    //generic fields
  
    proc init(size: domain(?)) {
        super.init(size.rank);
        this.rank = size.rank;
        this.strides = size.strides;
        this.dom = size;
    }
  
  override proc getDomain(): domain(?) {
    return this.dom;
  }
}

proc test1() {
  var d1: domain(2) = {1..2, 1..3};
  var dervChild1 = new shared DerivedChild(d1);
  writeln(dervChild1.getDomain());
}

proc test2() {
  proc foo(): domain(?) {
    var D: domain(rank=2, strides=strideKind.one);
    return D;
  }
  foo();
}

proc main() {
  test1();
  test2();
}
