use BlockDist;

var B = Block.createArray({1..10}, real);

extern proc cprintarr(X: [] real);

cprintarr(B);
