use BlockDist;

const D = {1..8} dmapped Block({1..8});
var A: [D] real;
var SD: sparse subdomain(D);
SD += (1,);
SD += (3,);
SD += (6,);
SD += (8,);
var AS: [SD] real;

proc exercise(desc, Dom, Arr) {
  writeln(desc, ": Dom");
  forall i in Dom do
    Arr[i] = here.id;
  writeln(Arr);
  writeln();

  writeln(desc, ": Arr");
  forall a in Arr do
    a = -here.id;
  writeln(Arr);
  writeln();

  writeln(desc, ": (Dom,Arr)");
  forall (i,a) in zip(Dom,Arr) do
    a = here.id;
  writeln(Arr);
  writeln();

  writeln(desc, ": (Arr,Dom)");
  forall (a,i) in zip(Arr,Dom) do
    a = here.id;
  writeln(Arr);
  writeln();
}

exercise("Dense", D, A);
exercise("Sparse", SD, AS);
