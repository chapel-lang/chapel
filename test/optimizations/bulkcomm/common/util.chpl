
var errorIfMismatch = false;
var debugDefault = false;

proc checkShape(A : [?ad], B : [?bd]) {
  for param i in 0..ad.rank-1 {
    if ad.dim(i).size != bd.dim(i).size then return false;
  }
  return true;
}

proc stridedAssign(A : [], B : [], debug=debugDefault) {
  stridedAssign(A, A.domain, B, B.domain, debug=debug);
}

// returns:
// -1 if the assignment result in incorrect data
// 0 if the assignment did not occur (say, for arrays whose size did not match)
// 1 if the assignment was successfull
proc stridedAssign(A : [], sa, B : [], sb, debug=debugDefault) {
  ref left = if isDomain(sa) then A[sa] else A[(...sa)];
  ref right = if isDomain(sb) then B[sb] else B[(...sb)];
  const ldom = left.domain;

  // If 'ldom' is a Stencil-distributed domain, it can return 'true' for
  // indices in the fluff region, but that's not really what we want in this
  // particular case. Instead, use a DR domain.
  const memberDom = {(...ldom.dims())};

  var ret = 1;

  proc isMember(idx) {
    if isTuple(idx) {
      if isDomain(sa) {
        return memberDom.contains(idx);
      } else {
        var ret = true;
        for param i in 0..sa.size-1 {
          if isRange(sa(i)) then ret &&= sa(i).contains(idx(i));
          else ret &&= (idx(i) == sa(i));
        }
        return ret;
      }
    } else {
      return memberDom.contains(idx);
    }
  }

  if checkShape(left, right) {
    right = 42;

    if debug then writeln(ldom, " = ", right.domain);

    const success = chpl__bulkTransferArray(left, right);
    if !success then
      writeln("FAILURE: bulk transfer did not happen!");

    var failOut = false,
        failIn  = false;
    forall i in A.domain with ( || reduce failOut, || reduce failIn) {
      const isFortyTwo = A[i] == 42;
      const inSlice = isMember(i);
      failIn = failIn || (inSlice && !isFortyTwo);
      failOut = failOut || (!inSlice && isFortyTwo);
      A[i] = 1; // reset for next assignment
    }
    if failOut || failIn {
      ret = -1;
      writeln("FAILURE for ", sa, " = ", sb);
      if failOut then writeln("\twrote outside of destination");
      if failIn then writeln("\tincorrect value in destination");
    }
    right = 3;
  } else ret = 0;

  if ret == 0 && errorIfMismatch then
    writeln("FAILURE: domains cannot be used to assign: ", left.domain, " vs. ", right.domain);

  return ret;
}
