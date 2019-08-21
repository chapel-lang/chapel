// taken from test/domains/methods/shape.chpl

// OpaqueDom
var opaqueDom: domain(opaque);
for 1..10 do opaqueDom.create();
assertEqual(opaqueDom.shape, (10,), msg='opaqueDom:');

var opaqueArray: [opaqueDom] int;
assertEqual(opaqueArray.shape, (10,), msg='opaqueArray:');

proc assertEqual(a, b, msg) {
  if a != b {
    writeln(msg);
    writeln('AssertionError: %t != %t'.format(a, b));
  }
}
