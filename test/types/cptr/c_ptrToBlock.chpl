use BlockDist, CTypes;

var B = newBlockArr({1..10}, real);

var bptr = c_ptrTo(B);
