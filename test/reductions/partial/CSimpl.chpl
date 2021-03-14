
use utilities;
use LayoutCS;

// RES = + reduce (shape=RES.domain) [idx in DOM] FEXPR(idx)

proc plusPRinto(ref RES,DOM,FEXPR) throws {
  if !isDomain(DOM) then
    compilerError("partial reductions over forall-expressions are currently available only for forall-expressions over domains");
  type eltType = partRedForallExprElmType(DOM, FEXPR);
  const OP = new unmanaged SumReduceScanOp(eltType=eltType);
  defer {
    delete OP;
  }
  DOM.dist._value.dsiPartialReduceInto(RES, OP, DOM, FEXPR);
}

// At the moment, this is an exact copy of DefaultDist.dsiPartialReduce()
// except for the receiver class.
proc CS.dsiPartialReduceInto(ref resArr, const perElemOp,
                             const srcDom, const fExpr)
  throws
{
//  const ref srcDom  = srcArr.domain;
  const srcDims = srcDom.dims();

  const ref resDom = resArr.domain;
  const resDims = resDom.dims();
  resArr = perElemOp.identity;
  const resReduceOp = new unmanaged PartRedOp(eltType=resArr.type,
                                              perElemOp = perElemOp);

  forall srcIdx in srcDom
    with (resReduceOp reduce resArr)
  {
    const resIdx = fullIdxToReducedIdx(resDims, srcDims, srcIdx);
    resArr reduce= (resIdx, fExpr(srcIdx));
    //writeln(fExpr(srcIdx));
  }

  delete resReduceOp;
}
