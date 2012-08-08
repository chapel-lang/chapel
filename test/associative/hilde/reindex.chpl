// This test used to fail because foo would attempt to reindex the passed-in array.
// reindex() calls dsiDim, which is not defined for associated arrays.
var D:domain(string);
D+="john";
proc foo(X:[D]) {
  for k in X.domain do
    writeln(k, " => ", X[k]);
}
var A:[D] string;
A["john"] = "lennon";
foo(A);

