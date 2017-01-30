/* Test domain.shape method (called from array.shape) */

use BlockDist;

//
// Regular Domains & Arrays
//

// RectangularDom
var rectDom = {1..10, 1..10};
assertEqual(rectDom.shape, (10, 10), msg='rectDom:');

// RectangularArray
var rectArray: [rectDom] int;
assertEqual(rectArray.shape, (10, 10), msg='rectArray:');

//
// Irregular Domains & Arrays
//

// SparseDom
var sparseDom: sparse subdomain(rectDom);
assertEqual(sparseDom.shape, (10, 10), msg='sparseDom:');

// RectangularArray
var sparseArray: [sparseDom] int;
assertEqual(sparseArray.shape, (10, 10), msg='sparseArray:');

// OpaqueDom
var opaqueDom: domain(opaque);
for 1..10 do opaqueDom.create();
assertEqual(opaqueDom.shape, (10,), msg='opaqueDom:');

var opaqueArray: [opaqueDom] int;
assertEqual(opaqueArray.shape, (10,), msg='opaqueArray:');

// AssociativeDom
var associativeDom: domain(int);
for i in 1..10 do associativeDom += i;
assertEqual(associativeDom.shape, (10,), msg='associateDom:');

var associativeArray: [associativeDom] int;
assertEqual(associativeArray.shape, (10,), msg='associateArray:');

// EnumDom (a flavor of AssociateDom)
enum digits {zero, one, two, three, four, five, six, seven, eight, nine};
var enumDom: domain(digits);
assertEqual(enumDom.shape, (10,), msg='enumDom:');

var enumArray: [enumDom] int;
assertEqual(enumArray.shape, (10,), msg='enumArray:');

//
// Distributed Domains & Arrays
//

// Block-distributed
var blockDom = rectDom dmapped Block(boundingBox=rectDom);
assertEqual(blockDom.shape, (10, 10), msg='blockDom');

var blockArray: [blockDom] int;
assertEqual(blockArray.shape, (10, 10), msg='blockArray:');


proc assertEqual(a, b, msg) {
  if a != b {
    writeln(msg);
    writeln('AssertionError: %t != %t'.format(a, b));
  }
}
