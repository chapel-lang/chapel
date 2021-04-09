use LayoutCS;

var thisCalls: atomic int;
var localAccessCalls: atomic int;

var baseDom1D = {1..10};
var baseDom2D = {1..10, 1..10};

{ // DR
  test(baseDom1D, "1D DR");
  test(baseDom2D, "2D DR");
}

{
  var cooDom: sparse subdomain(baseDom1D);
  cooDom += [1,2];
  test(cooDom, "1D COO");
}

{
  var cooDom: sparse subdomain(baseDom2D);
  cooDom += [(1,1),(2,2)];
  test(cooDom, "2D COO");
}

{
  var csrDom: sparse subdomain(baseDom2D) dmapped CS();
  csrDom += [(1,1),(2,2)];
  test(csrDom, "CSR");
}

{
  var cscDom: sparse subdomain(baseDom2D) dmapped CS(compressRows=false);
  cscDom += [(1,1),(2,2)];
  test(cscDom, "CSC");
}

{
  var assocDom: domain(string);
  assocDom += ["foo", "bar"];
  test(assocDom, "associative domain with string keys");
}

// hijack these methods to provide some output
inline proc _array.this(i: string) ref {
  thisCalls.add(1);
  return this._value.dsiAccess(i);
}

inline proc _array.localAccess(i: string) ref {
  localAccessCalls.add(1);
  return this._value.dsiAccess(i);
}

inline proc _array.this(i: int) ref {
  thisCalls.add(1);
  return this._value.dsiAccess((i:int,));
}

inline proc _array.this(i: 2*int) ref {
  thisCalls.add(1);
  return this._value.dsiAccess(i);
}

inline proc _array.localAccess(i: int) ref {
  localAccessCalls.add(1);
  return this._value.dsiAccess((i:int,));
}

inline proc _array.localAccess(i: 2*int) ref {
  localAccessCalls.add(1);
  return this._value.dsiAccess(i);
}

proc resetCounters() {
  thisCalls.write(0);
  localAccessCalls.write(0);
}

proc printCounters() {
  writeln("Calls to `this`: ", thisCalls.read());
  writeln("Calls to `localAccess`: ", localAccessCalls.read());
}

proc test(dom:domain, name) {
  writeln("Testing ", name);

  var arr: [dom] int;

  forall i in dom {
    arr[i] = idxToInt(i);
  }

  writeln(arr);

  printCounters();
  resetCounters();

  writeln("End testing ", name);
  writeln();

  inline proc idxToInt(i) {
    if isTuple(i) then return i[0];
    else if i.type == string then return i.byte(0):int;
    else return i;
  }
}
