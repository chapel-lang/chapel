use Allocators;

class Tree {
  type poolType;
  var pool: poolType;
  var left: unmanaged Tree?(poolType);
  var right: unmanaged Tree?(poolType);
  var value: int;
  proc init(value, pool) {
    this.poolType = pool.type;
    this.pool = pool;
    this.left = nil;
    this.right = nil;
    this.value = value;
  }
  proc deinit() {
    if left != nil then
      deleteWithAllocator(pool, left);
    if right != nil then
      deleteWithAllocator(pool, right);
  }
  proc insert(value) {
    if value < this.value {
      if left == nil {
        left = newWithAllocator(pool, this.type:unmanaged, value, pool);
      } else {
        left!.insert(value);
      }
    } else {
      if right == nil {
        right = newWithAllocator(pool, this.type:unmanaged, value, pool);
      } else {
        right!.insert(value);
      }
    }
  }
  proc print() {
    if left != nil {
      left!.print();
    }
    writeln(value);
    if right != nil {
      right!.print();
    }
  }
}

{
  var pool = new mallocWrapper();
  var tree = newWithAllocator(pool, unmanaged Tree?, 5, pool);
  tree!.insert(3);
  tree!.insert(7);
  tree!.insert(1);
  tree!.insert(4);
  tree!.insert(6);
  tree!.insert(8);
  tree!.print();
}


