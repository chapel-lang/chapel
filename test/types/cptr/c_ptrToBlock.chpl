use BlockDist, CTypes;

var B = blockDist.createArray({1..10}, real);

var bptr = c_ptrTo(B);
