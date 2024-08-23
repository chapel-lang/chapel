// Some of the cases in this test aren't optimized as expected. Creating futures
// are somewhat difficult for auto aggregation tests, so I am keeping this as a
// regular test for now.
// 
// See: https://github.com/Cray/chapel-private/issues/1897

use LayoutCS;

var baseDom1D = {1..10};
var baseDom2D = {1..10, 1..10};

config param case = ""; // choose which case to test

if case == "1D DR" {
  test(baseDom1D, "1D DR");
}
if case == "2D DR" {
  test(baseDom2D, "2D DR");
}

if case == "1D COO" {
  var cooDom: sparse subdomain(baseDom1D);
  cooDom += [1,2];
  test(cooDom, "1D COO");
}

if case == "2D COO" {
  var cooDom: sparse subdomain(baseDom2D);
  cooDom += [(1,1),(2,2)];
  test(cooDom, "2D COO");
}

if case == "CSR" {
  var csrDom: sparse subdomain(baseDom2D) dmapped new cs();
  csrDom += [(1,1),(2,2)];
  test(csrDom, "CSR");
}

if case == "CSC" {
  var cscDom: sparse subdomain(baseDom2D) dmapped new cs(compressRows=false);
  cscDom += [(1,1),(2,2)];
  test(cscDom, "CSC");
}

if case == "associative domain with string keys" {
  var assocDom: domain(string) = {"foo", "bar"};

  test(assocDom, "associative domain with string keys");
}

proc test(dom:domain(?), name) {
  writeln("Testing ", name);

  var arr: [dom] int;

  forall i in dom with (ref arr) {
    arr[i] = idxToInt(i);
  }

  writeln(arr);

  writeln("End testing ", name);
  writeln();

  inline proc idxToInt(i) {
    if isTuple(i) then return i[0];
    else if i.type == string then return i.byte(0):int;
    else return i;
  }
}
