
const ParentDom = {0..10};
var SparseDom: sparse subdomain(ParentDom);
var A:[SparseDom] int;

proc f(const ref x)
{
  writeln(x.locale.id);
}

for i in ParentDom {
  f(A[i]);
}

for i in ParentDom {
  writeln(A[i].locale.id);
}


