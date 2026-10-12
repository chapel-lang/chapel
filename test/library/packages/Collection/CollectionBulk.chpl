use DistributedDeque;

// Here we test the capabilities of the utility methods, 'addBulk' and 'removeBulk'.
config param isBoundedDeque = false;
config param isDeque = false;

config param nElems = 1000;

var c = (
  if isBoundedDeque then new DistDeque(int, cap=nElems)
  else if isDeque then new DistDeque(int)
  else compilerError("Require 'isBoundedDeque' or 'isDeque' to be set...")
);

// Add Bulk Test
// We simply add a range of elements.
var r = (if isBoundedDeque then 1..nElems + 1 else 1..nElems);
var successfulInsertions = c.addBulk(r);
assert(successfulInsertions == nElems);
assert(c.getSize() == nElems);

// Remove Bulk Test
// We simply remove the range of elements we added.
var iterations = 0;
for elt in c.removeBulk(nElems) {
	iterations += 1;
}
assert(iterations == nElems);

writeln("SUCCESS");
