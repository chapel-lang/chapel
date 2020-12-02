config const N = 2**20;
const R = 1..N;
const D = {R};
var A: [D] real;

enum localityCheck_t { localityUnknown, localityRight, localityWrong };
use localityCheck_t;

proc localityStr(loc: localityCheck_t): string {
  if loc == localityUnknown then return 'unknown';
  else if loc == localityRight then return 'right';
  else if loc == localityWrong then return 'wrong';
  else return '?what?';
}


//
// Supporting stuff elsewhere, e.g. the runtime.
//
extern proc sizeof(type x): size_t;

extern proc chpl_getHeapPageSize(): size_t;
const pgSize = chpl_getHeapPageSize();
const pgMask = pgSize - 1;

extern proc chpl_topo_getMemLocality(p: c_ptr): chpl_sublocID_t;


{
  var locality: localityCheck_t;
  locality = checkMemLocalityParts(c_ptrTo(A), A.domain.size,
                                   A.eltType);

  writeln('A localization: ', localityStr(locality));
}


//
// Check that the page locality is either unset or a sequence of chunks,
// more or less the same size, on the sublocales in order.
//
proc checkMemLocalityParts(p: c_ptr, size, type eltType) {
  var myP = __primitive("cast", c_ptr(int(8)), p);
  var sizeBytes = size:size_t * sizeof(eltType);

  //
  // We ignore any leading partial edge page.
  //
  const offInPage = __primitive("cast", size_t, myP) & pgMask;
  if offInPage != 0 {
    const offToPage = pgSize - offInPage;
    if sizeBytes <= offToPage then
      return localityUnknown;
    myP       += offToPage;
    sizeBytes -= offToPage;
  }

  const nPages = sizeBytes / pgSize;
  const nSublocs = here.getChildCount();
  const nPagesPerSubloc = nPages:real / nSublocs:real;

  if chpl_topo_getMemLocality(myP) == c_sublocid_any then
    return localityUnknown;

  for subloc in 0..#nSublocs {
    var nPagesCurrSubloc: int;
    while sizeBytes > pgSize && chpl_topo_getMemLocality(myP) == subloc {
      nPagesCurrSubloc += 1;
      myP       += pgSize;
      sizeBytes -= pgSize;
    }
    if (nPagesCurrSubloc < nPagesPerSubloc - 1.0
        || nPagesCurrSubloc > nPagesPerSubloc + 1.0) then
      return localityWrong;
  }

  return localityRight;
}


//
// Check that the page locality is either unset or all on the expected
// sublocale.
//
proc checkMemLocalityWhole(p: c_ptr, size, type eltType,
                           subloc: chpl_sublocID_t) {
  extern proc chpl_topo_getMemLocality(p: c_ptr): chpl_sublocID_t;

  var myP = __primitive("cast", c_ptr(int(8)), p);
  var sizeBytes = size:size_t * sizeof(eltType);

  //
  // We ignore any leading partial edge page.
  //
  const offInPage = __primitive("cast", size_t, myP) & pgMask;
  if offInPage != 0 {
    const offToPage = pgSize - offInPage;
    if sizeBytes <= offToPage then
      return localityUnknown;
    myP       += offToPage;
    sizeBytes -= offToPage;
  }

  const sublocFirst = chpl_topo_getMemLocality(myP);
  var locality: localityCheck_t;

  if sublocFirst == c_sublocid_any then
    locality = localityUnknown;
  else if sublocFirst == subloc then
    locality =localityRight;
  else
    return localityWrong;

  if sizeBytes <= pgSize then
    return locality;

  myP       += pgSize;
  sizeBytes -= pgSize;

  //
  // We ignore any partial trailing page.
  //
  if (&& reduce [off in 0..#sizeBytes by pgSize]
      chpl_topo_getMemLocality(myP + off) == sublocFirst) then
    return locality;
  return localityWrong;
}
