/* Test domain.shape method */

//
// Regular Domains & Arrays
//

// RectangularDom
var rectDom = {1..10, 1..10};
writeln(rectDom.shape);

//
// Irregular Domains & Arrays
//

// SparseDom
var sparseDom: sparse subdomain(rectDom);
writeln(sparseDom.shape);

// OpaqueDom
var opaqueDom: domain(opaque);
opaqueDom.create();
opaqueDom.create();
writeln(opaqueDom.shape);

// AssociativeDom
var associativeDom: domain(int);
associativeDom += 1;
associativeDom += 2;
writeln(associativeDom.shape);

// EnumDom
enum compass {N, E, S, W};
var enumDom: domain(compass);
writeln(enumDom.shape);



