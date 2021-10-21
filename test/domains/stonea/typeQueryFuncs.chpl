proc runQueries(description, obj) {
  writeln("Given a ", description, ": ");
  writeln("  obj.isRectangularDom() = ", obj.isRectangular());
  writeln("  obj.isIrregularDom()   = ", obj.isIrregular());
  writeln("  obj.isAssociativeDom() = ", obj.isAssociative());
  writeln("  obj.isSparseDom()      = ", obj.isSparse());
}

var rectangularDom = {1..2, 1..2};
var rectangularArr : [rectangularDom] int;

var assocDom : domain(string);
assocDom += "Hello";
assocDom += "World";
var assocArr : [assocDom] int;

var sparseDom : sparse subdomain(rectangularDom);
var sparseArr : [sparseDom] int;

runQueries("rectangular domain", rectangularDom);
runQueries("rectangular array", rectangularArr);
runQueries("associative domain", assocDom);
runQueries("associative array", assocArr);
runQueries("sparse domain", sparseDom);
runQueries("sparse array", sparseArr);

