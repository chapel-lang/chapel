use LayoutCS;

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

proc test(dom:domain, name) {
  use CyclicDist;

  writeln("Testing ", name);

  var destArr: [dom] int;
  var sourceArr: [dom] int;


  forall i in dom {
    destArr[i] = sourceArr[generateIdx(i)]; // RHS will be seen as non-local
  }

  writeln(destArr);

  writeln("End testing ", name);
  writeln();

  inline proc generateIdx(i) {
    return i;
  }
}
