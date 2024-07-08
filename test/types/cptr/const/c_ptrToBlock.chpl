use BlockDist, CTypes;

var B = blockDist.createArray({1..10}, real);

var bptr_const = c_ptrToConst(B);
