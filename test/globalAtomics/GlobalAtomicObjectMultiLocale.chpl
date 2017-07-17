use GlobalAtomicObject;

class D { var d : int; }
var arr : [{1..100}] D;
for i in 1 .. 100 {
  on Locales[i % numLocales] do arr[i] = new D(i);
}

var x = new GlobalAtomicObject(D);
x.write(arr[1]);

for i in 1 .. 99 {
  assert(x.read() == arr[i]);
  var result = x.compareExchange(arr[i], arr[i+1]);
  assert(result);
  delete arr[i];
}
