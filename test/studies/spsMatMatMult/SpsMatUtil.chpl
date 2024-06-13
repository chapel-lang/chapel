module SpsMatUtil {
  // The following are routines that should arguably be supported directly
  // by the LayoutCS and SparseBlockDist modules themselves
  //
  //  public use LayoutCSUtil, SparseBlockDistUtil;

  use BlockDist, LayoutCS, Map, Random;

  enum layout { CSR, CSC };
  public use layout;

  config const seed = 0,
               printSeed = seed == 0;

  var rands = if seed == 0 then new randomStream(real)
                         else new randomStream(real, seed);

  // print library-selected seed, for reproducibility
  //
  if printSeed then
    writeln("Using seed: ", rands.seed);

  record sparseMatDat {
    forwarding var m: map(2*int, int);

    proc ref add(idx: 2*int, val: int) {
      if val != 0 {
        if m.contains(idx) {
          m[idx] += val;
        } else {
          m.add(idx, val);
        }
      }
    }
  }



  // create a local random sparse matrix within the space of 'Dom' of
  // the given density and layout.  If distributed is true, this will
  // be a block-distributed sparse matrix, otherwise it'll be local.
  //
  proc randSparseDomain(parentDom, density, param layout, param distributed)
   where distributed == false {

    var SD: sparse subdomain(parentDom) dmapped new dmap(new CS(compressRows=(layout==CSR)));

    for (i,j) in parentDom do
      if rands.next() <= density then
        SD += (i,j);

    return SD;
  }

  proc randSparseDomain(parentDom, density, param layout, param distributed)
   where distributed == true {
    const locsPerDim = sqrt(numLocales:real): int,
          grid = {0..<locsPerDim, 0..<locsPerDim},
          localeGrid = reshape(Locales[0..<grid.size], grid);


    if grid.size != numLocales then
      writeln("Warning: Only using ", grid.size, " of ", numLocales,
              " locales");

    // writeln(grid);

    type layoutType = CS(compressRows=(layout==CSR));
    const DenseBlkDom = parentDom dmapped new blockDist(boundingBox=parentDom,
                                                  targetLocales=localeGrid,
                                                  sparseLayoutType=layoutType);

    var SD: sparse subdomain(DenseBlkDom);

    for (i,j) in parentDom do
      if rands.next() <= density then
        SD += (i,j);

    return SD;
  }


  proc emptySparseDomLike(Mat) {
    var ResDom: sparse subdomain(Mat.domain.parentDom);
    return ResDom;
  }


  // print out a sparse matrix (in a dense format)
  //
  proc writeSparseMatrix(msg, Arr) {
    const ref SparseDom = Arr.domain,
              DenseDom = SparseDom.parentDom;

    writeln(msg);

    for r in DenseDom.dim(0) {
      for c in DenseDom.dim(1) {
        write(Arr[r,c], " ");
      }
      writeln();
    }
    writeln();    
  }


  // create a new sparse matrix from a map from sparse indices to values
  //
  proc makeSparseMat(parentDom, spsData) {
    use Sort;

    var CDom: sparse subdomain(parentDom) dmapped new dmap(new CS());
    var inds: [0..<spsData.size] 2*int;
    for (idx, i) in zip(spsData.keys(), 0..) do
      inds[i] = idx;

    sort(inds);
  
    for ij in inds do
      CDom += ij;

    var C: [CDom] int;
    for ij in inds do
      try! C[ij] += spsData[ij];  // TODO: Should this really throw?

    return C;
  }


  // create a new sparse matrix from a collection of nonzero indices
  // (nnzs) and values (vals)
  //
  proc makeSparseMat(parentDom, nnzs, vals) {
    var CDom: sparse subdomain(parentDom);
    for ij in nnzs do
      CDom += ij;

    var C: [CDom] int;
    for (ij, c) in zip(nnzs, vals) do
      C[ij] += c;
    return C;
  }


  // This is a custom reduction, and a good case study for why our custom
  // reduction interface needs a refresh
  //
  class merge: ReduceScanOp {
    type eltType = sparseMatDat;
    var value: eltType;  // TODO: lots of deep copying here to avoid

    proc identity {
      var ident: eltType;
      return ident;
    }

    /*
    proc initialAccumulate(x) {
      if x.size != 0 then
        halt("Error shouldn't call merge.initialAccumulate() with a non-empty table");
    }
    */

    proc accumulate(x) {
      // Why is this ever called with a sparseMatDat as the argument?!?
      for (k,v) in zip(x.keys(), x.values()) {
        if value.contains(k) {
          value[k] += v;
        } else {
          value.add(k, v);
        }
      }
    }
    
    proc accumulateOntoState(ref state, x) {
      halt("Error, shouldn't call merge.accumulateOntoState()");
    }

    proc combine(x) {
      accumulate(x.value);
    }

    proc generate() {
      return value;
    }
    
    inline proc clone() {
      return new unmanaged merge(eltType=eltType);
    }
  }
}
