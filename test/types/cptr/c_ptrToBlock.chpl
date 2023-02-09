use BlockDist, CTypes;

var B = Block.createArray({1..10}, real);

var bptr = c_ptrTo(B);
