
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
    partRedCheckAndCreateResultDimensions(this, resDimSpec, srcArr, srcDims);

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
