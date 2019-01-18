/**
  * This test demonstrates the performance difference made when utilizing
  * explicit common subexpression removal, rather than relying on the compiler.
  *
  * Ideally, the Chapel compiler should optimize this out, or generate C code
  * in such a way that the back-end compiler can successfully optimize it out.
  *
  * Note that negative effects of the elegant version here become more severe
  * as the inner-loop operation becomes more costly.
 **/
use Time;

config const order: int = 10,
             subOrder: int = 5,
             iterations: int = 5,
             correctness: bool = true;

proc main() {

  var elegantTime = elegant(order, subOrder, iterations);

  var prfmnt_T = performant(order, subOrder, iterations);

  if correctness {
    writeln("correctness execution complete");
  } else {
    writeln("Elegant:    ", elegantTime);
    writeln("Performant: ", prfmnt_T);
  }
}


proc elegant(order, subOrder, iterations): real {

  var Dom = {1..order, 1..order},
   subDom = {1..subOrder, 1..subOrder};
  var Array : [Dom] real,
   subArray: [subDom] real = 2.0;

  var t = new Timer();

  t.start();

  for 1..iterations {
    for (i,j) in Dom {
      for k in 1..subOrder do
        Array[i, j] += i*j*subArray[k, k];
    }
  }

  return t.elapsed()/iterations;
}


proc performant(order, subOrder, iterations): real {

  var Dom = {1..order, 1..order},
   subDom = {1..subOrder, 1..subOrder};
  var Array : [Dom] real,
   subArray: [subDom] real = 2.0;

  var t = new Timer();

  t.start();

  var tmp: real = 0;

  for 1..iterations {
    tmp = 0;
    for (i,j) in Dom {
      for k in 1..subOrder do
        tmp += i*j*subArray[k, k];
      Array[i, j] += tmp;
    }
  }

  return t.elapsed()/iterations;
}
