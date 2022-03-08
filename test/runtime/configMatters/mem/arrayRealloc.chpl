// Realloc arrays by growing and shrinking. Cross the regular to comm allocated
// mem threshold for configurations that have comm allocated arrays.

config const maxArraySize = arraySize();
config type arrayType = int;

// Grow and shrink
{
  var s = 1;
  var dom = {0..#s};
  var A: [dom] arrayType;

  while (s < maxArraySize) {
    s *= 2;
    dom = {0..#s};
  }

  while (s > 0) {
    s /= 2;
    dom = {0..#s};
  }
}

// Grow
{
  var s = 1;
  var dom = {0..#s};
  var A: [dom] arrayType;

  while (s < maxArraySize) {
    s *= 2;
    dom = {0..#s};
  }
}

// Shrink
{
  var s = maxArraySize;
  var dom = {0..#s};
  var A: [dom] arrayType;

  while (s > 0) {
    s /= 2;
    dom = {0..#s};
  }
}

// We want our arrays to cross the boundary betweeen normal and comm allocated
// memory. For configs that have comm thresholds go with 4x that and otherwise
// just use 64MB.
proc arraySize() {
  use CTypes;
  extern proc chpl_comm_regMemAllocThreshold(): size_t;
  extern var SIZE_MAX: size_t;
  if chpl_comm_regMemAllocThreshold() != SIZE_MAX then
    return chpl_comm_regMemAllocThreshold():int * 4 / numBytes(arrayType);
  else
    return 64*1024*1024 / numBytes(arrayType);
}
