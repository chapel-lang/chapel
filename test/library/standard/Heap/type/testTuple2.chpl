use Heap;
import HeapTest;

proc test() {
  var hp = new heap((shared HeapTest.T, shared HeapTest.T));

  for i in 1..8 {
    var item = (new shared HeapTest.T(i), new shared HeapTest.T(-i));
    hp.push(item);
  }

  writeln(hp.size:string);

  for item in hp do writeln(item);

  while !hp.isEmpty() {
    var item = hp.top();
    hp.pop();
    writeln(item);
  }

  return;
}
test();
