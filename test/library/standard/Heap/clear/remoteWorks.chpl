use Heap;

on Locales[0] {
  var h = new heap(int);
  h.push(6);

  writeln("Before: isEmpty = ", h.isEmpty());
  writeln("Before: size = ", h.size);

  on Locales[1] {
    h.clear();
  }

  writeln("After: isEmpty = ", h.isEmpty());
  writeln("After: size = ", h.size);
}
