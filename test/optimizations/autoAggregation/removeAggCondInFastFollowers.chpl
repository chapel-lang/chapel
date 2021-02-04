use BlockDist;

class Arr {
  type t;
  var d = newBlockDom(1..10);
  var a: [d] t;

}

// this is a case I bumped into while trying to compile Arkouda. The
// assignment's LHS doesn't outlive the forall and so we don't optimize. This
// means we'll need to cleanup the aggregation code we have generated. However,
// the tricky part is we have fast and slow followers within the same body that
// are using the same aggregator symbol. So a single aggregator is actually used
// in two aggregation blocks within the forall body. This was against my
// assumptions while writing the cleanup code and it triggered an assertion.

// In short, we don't expect this code to use aggregation, but it should compile
// fine.

proc sliceIndexMsg(): string throws {
  var slice: range(stridable=true) = 1..10;

  proc sliceHelper(type t) throws {
      var e = new Arr(int);
      var a = new Arr(int);
      ref ea = e.a;
      ref aa = a.a;
      forall (elt,j) in zip(aa, slice) {
        elt =ea[j];
      }
      return "Done";
  }
    
  return sliceHelper(int);
}

writeln(sliceIndexMsg());
