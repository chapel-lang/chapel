/* Test domain.shape method */

use BlockDist;

//
// Regular Domains & Arrays
//

// RectangularDom
var rectDom = {1..10, 1..10};
writeln(rectDom.shape);

// RectangularArray
var rectArray: [rectDom] int;
writeln(rectArray.shape);

//
// Distributed Domains & Arrays
//

// Block-distributed
var blockDom = rectDom dmapped Block(boundingBox=rectDom);
writeln(blockDom.shape);

//
// Irregular Domains & Arrays
//

// SparseDom
var sparseDom: sparse subdomain(rectDom);
writeln(sparseDom.shape);

// RectangularArray
var sparseArray: [sparseDom] int;
writeln(sparseArray.shape);

// OpaqueDom
var opaqueDom: domain(opaque);
opaqueDom.create();
opaqueDom.create();
writeln(opaqueDom.shape);

var opaqueArray: [opaqueDom] int;
writeln(opaqueArray.shape);

// AssociativeDom
var associativeDom: domain(int);
associativeDom += 1;
associativeDom += 2;
writeln(associativeDom.shape);

var associativeArray: [associativeDom] int;
writeln(associativeArray.shape);

// EnumDom (a flavor of AssociateDom)
enum compass {N, E, S, W};
var enumDom: domain(compass);
writeln(enumDom.shape);

var enumArray: [enumDom] int;
writeln(enumArray.shape);



