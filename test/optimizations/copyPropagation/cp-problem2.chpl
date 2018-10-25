// This is a trimmed-down version of an attempted change to
// proc chpl__initCopy(ir: _iteratorRecord)

var RRR = 1..2;

// no bug without 'inline'
inline proc allocateData(ref data:_ddata(int)) {
  var callAgain: bool;
  var subloc = c_sublocid_none;
  // no bug if I allocate 'data' by passing it to some function by ref
  __primitive("array_alloc", data, 3, subloc, c_ptrTo(callAgain), c_nil);
}

proc main {
  var data:_ddata(int) = nil;

  allocateData(data);

  if data == nil then writeln("bad"); else writeln("good");

  for elt in RRR {
    if data == nil then writeln("nope"); else writeln("OK");
  }
}
