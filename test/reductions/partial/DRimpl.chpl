
use utilities;

// + reduce (shape=DIMS) ARR

proc plusPR(DIMS,ARR) throws {
  const OP = new unmanaged SumReduceScanOp(eltType=ARR.eltType);
  defer { delete OP; }
  return ARR.domain.dist.dsiPartialReduce(OP, DIMS, ARR);
}

proc DefaultDist.dsiPartialReduce(const perElemOp, const resDimSpec,
                                  const srcArr)
  throws
{
  partRedEnsureArray(srcArr);
  const ref srcDom  = srcArr.domain;
  const     srcDims = srcDom.dims();

  const (resDom, resDims) =
    partRedCheckAndCreateResultDimensions(_to_unmanaged(this), resDimSpec, srcArr, srcDims);

  var resArr: [resDom] srcArr.eltType = perElemOp.identity;
  const resReduceOp = new unmanaged PartRedOp(eltType=resArr.type,
                                              perElemOp = perElemOp);

  forall (srcIdx, srcElm) in zip(srcDom, srcArr)
    with (resReduceOp reduce resArr)
  {
    const resIdx = fullIdxToReducedIdx(resDims, srcDims, srcIdx);
    resArr reduce= (resIdx, srcElm);
  }

  delete resReduceOp;
  return  resArr;
}

pragma "use default init"
class PartRedOp: ReduceScanOp {
  type eltType;
  const perElemOp;
  var value: eltType = perElemOp.identity;

  // User-accessible AS will be nothing.
  proc identity() {
    var x: void; return x;
  }
  proc initialAccumulate(x) {
/* We just created the array that's the outer var.
   So no need to do anything like:
    value += x;
   Verify that instead:
*/
    if boundsChecking then
      forall initElm in x do
        assert(initElm == perElemOp.identity);
  }
  proc accumulate(x) {
    // This should be invoked just before deleting the AS.
    // Nothing to do.
    compilerAssert(x.type == void);
  }
  proc accumulateOntoState(ref state, x) {
    compilerAssert(state.type == void);
    compilerAssert(isTuple(x) && x.size == 2);
    // Accumulate onto the built-in AS instead.
    perElemOp.accumulateOntoState(value[x(1)], x(2));
  }
  proc combine(x) {
    forall (parent, child) in zip(value, x.value) {
      // TODO replace with perElemOp.combine()
      perElemOp.accumulateOntoState(parent, child);
    }
  }
  // TODO (a): avoid this when not needed.
  // TODO (b): invoke perElemOp.generate() when needed.
  proc generate() ref return value;
  proc clone() return new unmanaged PartRedOp(eltType=eltType,
                                              perElemOp = perElemOp);
}
