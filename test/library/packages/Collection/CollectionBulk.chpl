use DistributedDeque;
use DistributedBag;



// Here we test the capabilities of the utility methods, 'addBulk' and 'removeBulk'.
config param isBoundedDeque = false;
config param isDeque = false;
config param isBag = false;

config param nElems = 1000;

var c = (
  if isBoundedDeque then new DistDeque(int, cap=nElems)
  else if isDeque then new DistDeque(int)
  else if isBag then new DistBag(int)
  else compilerError("Require 'isBoundedDeque', 'isDeque', or 'isBag' to be set...")
);

// Add Bulk Test
// We simply add a range of elements.
var range = (if isBoundedDeque then 1..nElems + 1 else 1..nElems);
var successfulInsertions = c.addBulk(range);
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
