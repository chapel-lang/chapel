use Allocators;
{
  class MyClass {
    var myInt: int;
  }
  record A {
    // I should be able to have this class be generic with respect to the allocator
    var x: allocator;
    var y: unmanaged MyClass?;
    proc init(x: allocator) {
      this.x = x;
      this.y = newWithAllocator(this.x, unmanaged MyClass?, 1);
      init this;
    }
    proc deinit() {
      deleteWithAllocator(this.x, this.y);
    }
    proc print() {
      writeln(this.myInt);
    }
  }
  var x: allocator = new mallocPool();
  var a = new A(x);
  a.print();
}
