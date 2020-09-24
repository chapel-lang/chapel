use BlockDist;

var B = newBlockArr({1..10}, real);

extern proc cprintarr(X: [] real);

cprintarr(B);
