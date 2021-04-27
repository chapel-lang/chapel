var baseDom = {1..10};

{
  test(baseDom);
}

{
  var cooDom: sparse subdomain(baseDom);
  cooDom += [3,4,5];
  test(cooDom);
}

{
  use LayoutCS;

  var dom2D = {1..10, 1..10};

  var csrDom: sparse subdomain(dom2D) dmapped CS(compressRows=true);
  csrDom += [(3,3),(4,4),(5,5)];
  test(csrDom);

  var cscDom: sparse subdomain(dom2D) dmapped CS(compressRows=false);
  cscDom += [(3,3),(4,4),(5,5)];
  test(cscDom);
}

{
  var assocDom: domain(string);
  assocDom += ["foo", "bar"];
  test(assocDom);
}


proc test(dom: domain) {
  var arr: [dom] int;

  for i in dom {
    arr.localAccess[i] = idxToInt(i);
  }

  for i in dom {
    writeln("index = ", i, " arr.localAccess[i] = ", arr.localAccess[i]);
  }
  writeln();

  inline proc idxToInt(i) {
    if isTuple(i) then return i[0];
    else if i.type == string then return i.byte(0):int;
    else return i;
  }
}
