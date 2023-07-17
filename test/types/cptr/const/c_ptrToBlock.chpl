use BlockDist, CTypes;

var B = Block.createArray({1..10}, real);

var bptr_const = c_ptrToConst(B);
