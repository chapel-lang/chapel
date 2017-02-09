
proc checkShape(A : [?ad], B : [?bd]) {
  for param i in 1..ad.rank {
    if ad.dim(i).length != bd.dim(i).length then return false;
  }
  return true;
}

proc stridedAssign(A : [], B : []) {
  stridedAssign(A, A.domain, B, B.domain);
}

// returns:
// -1 if the assignment result in incorrect data
// 0 if the assignment did not occur (say, for arrays whose size did not match)
// 1 if the assignment was successfull
proc stridedAssign(A : [], sa, B : [], sb, debug=false) {
  ref left = if isDomain(sa) then A[sa] else A[(...sa)];
  ref right = if isDomain(sb) then B[sb] else B[(...sb)];
  const ldom = left.domain;

  var ret = 1;

  proc isMember(idx) {
    if isTuple(idx) {
      if isDomain(sa) {
        return ldom.member(idx);
      } else {
        var ret = true;
        for param i in 1..sa.size {
          if isRange(sa(i)) then ret &&= sa(i).member(idx(i));
          else ret &&= (idx(i) == sa(i));
        }
        return ret;
      }
    } else {
      return ldom.member(idx);
    }
  }

  if checkShape(left, right) {
    right = 42;

    if debug then writeln(ldom, " = ", right.domain);

    assert(chpl__getActualArray(left).oneDData);  // fend off multi-ddata
    assert(chpl__getActualArray(right).oneDData); // fend off multi-ddata
    left._value.doiBulkTransferStride(right, chpl__getViewDom(left));

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

  return ret;
}
