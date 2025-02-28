use BlockDist;

    extern proc chpl_equals_localeID(const ref x, const ref y): bool;
const Space = {1..10};

const D = Space dmapped new blockDist(Space);
var r = new myRec();

on Locales[1] {
  var A: [Space] int = 1;

  // we can _probably_ make AVE cover this if `r.B` turns into a local copy of
  // the remote record (I don't know), but right now the pattern is too
  // complicated.
  A[3..5] = r.B[3..5];
  writeln(A);
}

on Locales[1] {
  var A: [Space] int = 1;

  // here, we can't use AVE, because of its reliance on C pointers to store
  // information about the arrays it uses. Note that `remoteArrRecord` is going
  // to be remote here, where c_pointer_return will issue an error
  ref remoteArrRecord = r.B;
  A[3..5] = remoteArrRecord[3..5];
  writeln(A);
}


record myRec {
  var B: [D] int;
}
