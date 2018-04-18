use OwnedObject;
use utilities;

// + reduce (shape=DIMS) ARR

proc plusPR(DIMS,ARR) throws {
  const OP = new unmanaged SumReduceScanOp(eltType=ARR.eltType);
  defer { delete OP; }
  return ARR.domain.dist.dsiPartialReduce(OP, DIMS, ARR);
}

proc DefaultDist.dsiPartialReduce(const reduceOp, const resDimSpec,
                                  const srcArr)
  throws
{
  partRedEnsureArray(srcArr);
  const ref srcDom  = srcArr.domain;
  const     srcDims = srcDom.dims();

  const (resDom, resDims) =
    partRedCheckAndCreateResultDimensions(_to_unmanaged(this), resDimSpec, srcArr, srcDims);

  var resArr: [resDom] srcArr.eltType = reduceOp.identity;
  const resReduceOp = new unmanaged (reduceOp.type)(eltType=resArr.type);

  forall (srcIdx, srcElm) in zip(srcDom, srcArr)
    with (resReduceOp reduce resArr)
  {
    const resIdx = fullIdxToReducedIdx(resDims, srcDims, srcIdx);
    reduceOp.accumulateOntoState(resArr[resIdx], srcElm);
  }

  delete resReduceOp;
  return  resArr;
}
