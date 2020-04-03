// Utility functions available for partial reduction implementations
// in individual domain maps.

proc partRedEnsureArray(srcArr) {
  if !isArray(srcArr) then
    compilerError("partial reductions are currently available only for arrays");
}

proc partRedForallExprElmType(srcDom, fExpr) type {
  var idx: srcDom.rank * srcDom.idxType;
  return fExpr(idx).type;
}

// TODO if resDimSpec is a tuple of singletons and unbounded ranges (..),
// then we can determine the preserved vs. reduced dimensions at compile
// time and use fewer conditionals in the generated code.

proc partRedCheckAndCreateResultDimensions(dist, resDimSpec, srcArr, srcDims)
  throws
{
  if isDomain(resDimSpec)
  {
    const ref resDom = resDimSpec;
    const resDims = resDom.dims();

    partRedHelpCheckNumDimensions(resDims, srcDims);
    partRedHelpCheckDimensions(resDims, srcDims);

    return (resDom, resDims);
  }
  else if isTuple(resDimSpec)
  {
    var resDims: resDimSpec.size * srcDims(1).type;

    partRedHelpCheckNumDimensions(resDims, srcDims);

    for param dim in 0..resDims.size-1 {
      ref specD = resDimSpec(dim);
      if isIntegral(specD) {
        resDims(dim) = (specD..specD) : srcDims(1).type;
      }
      else if isRange(specD) {
        if specD.boundedType == BoundedRangeType.bounded then
          resDims(dim) = specD;
        else if specD.boundedType == BoundedRangeType.boundedNone then
          resDims(dim) = srcDims(dim);
        else
          compilerError("the range in the the dimension " + dim + " of the shape of the partial reduction is neither fully bounded nor fully unbounded");
      }
      else {
        compilerError("the dimension " + dim + " of the shape of the partial reduction is neither a range nor an individual index");
      }
    }

    partRedHelpCheckDimensions(resDims, srcDims);

/* Cannot create a domain for resDims for non-rectangular domain maps:
    const resDom = if isSubtype(dist.type, DefaultDist)
      then {(...resDims)}
      else {(...resDims)} dmapped new dmap(dist);
*/
    // For now, the result will always be a dense DR domain/array.
    // TODO: support distributions like Block.
    const resDom = {(...resDims)};

    return (resDom, resDims);
  }
  else
    compilerError("the shape of the partial reduction is neither a domain nor a tuple of ranges/indices");
}


proc fullIdxToReducedIdx(const resDims, const srcDims, const srcIdx)
{
  var resIdx: resDims.size * resDims(0).idxType;
  //compilerWarning(resIdx.type:string);

  for param dim in 0..resDims.size-1 do
    if isReducedDim(resDims, srcDims, dim) then
      resIdx(dim) = resDims(dim).alignedLow;
    else
      resIdx(dim) = srcIdx(dim);

  //debug(srcIdx, "  >>  ", resIdx);
  return resIdx;
}

proc isReducedDim(const resDims, const srcDims, param dim)
  return resDims(dim).size == 1;

proc isPreservedDim(const resDims, const srcDims, param dim)
  return !isReducedDim(resDims, srcDims, dim);


proc partRedHelpCheckNumDimensions(resDims, srcDims) {
  if resDims.size != srcDims.size then
    compilerError("the shape of the partial reduction has a different number of dimensions than the array being reduced");
}

proc partRedHelpCheckDimensions(resDims, srcDims) throws {
  for param dim in 0..resDims.size-1 {
    if resDims(dim).size == 1 ||   // reduced dimension
       resDims(dim) == srcDims(dim)  // preserved dimension
    then ; // OK
    else throw new owned IllegalArgumentError("the preserved dimension " + dim:string + " in the shape of the partial reduction differs from that in the array being reduced");
  }
}

/* This class implements the reduction of partial results. */
class PartRedOp: ReduceScanOp {
  type eltType;
  const perElemOp;
  var value: eltType = perElemOp.identity;

  // User-accessible AS will contain no data, i.e. none.
  proc identity() { return none; }
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
    compilerAssert(x.type == nothing);
  }
  proc accumulateOntoState(ref state, x) {
    compilerAssert(state.type == nothing);
    compilerAssert(isTuple(x) && x.size == 2);
    // Accumulate onto the built-in AS instead.
    perElemOp.accumulateOntoState(value[x(0)], x(1));
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
