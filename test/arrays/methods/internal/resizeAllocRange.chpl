/* Test to exercise the Grow code paths in _array.resizeAllocRange */

// Dummy array to call methods on
var Array = [1];


//
// direction = +1
//

// Grow by 1, small (r1+1)
assert(Array.resizeAllocRange(1..3, 1..4) == 1..4);

// Grow by 1, large (1.5x)
assert(Array.resizeAllocRange(1..100, 1..1) == 1..150);

// Grow by more than 1 (r2)
assert(Array.resizeAllocRange(1..3, 1..7) == 1..7);


//
// direction = -1
//

// Grow by 1, small (r1+1)
assert(Array.resizeAllocRange(1..3, 1..4, direction=-1) == 0..3);

// Grow by 1, large (1.5x)
assert(Array.resizeAllocRange(1..100, 1..1, direction=-1) == -49..100);

// Grow by more than 1 (r2)
assert(Array.resizeAllocRange(1..3, 1..7, direction=-1) == -3..3);

