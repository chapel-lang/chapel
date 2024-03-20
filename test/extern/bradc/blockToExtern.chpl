use BlockDist;

var B = blockDist.createArray({1..10}, real);

extern proc cprintarr(X: [] real);

cprintarr(B);
