use Time;

config const order: int = 10,
             iterations: int = 5,
             correctness: bool = true;

proc main() {

  var elegantTime = elegant(order, iterations);

  var prfmnt_T = performant(order, iterations);

  if correctness {
    writeln("correctness execution complete");
  } else {
    writeln("Elegant:    ", elegantTime);
    writeln("Performant: ", prfmnt_T);
  }
}


proc elegant(order, iterations): real {

  var Dom = {1..order, 1..order};
  var Array : [Dom] real;

  var t = new Timer();

  t.start();

  for i in 1..iterations do
    for (i,j) in Dom do
      for k in 1..iterations do
        Array += i+j*k;

  return t.elapsed()/iterations;
}


proc performant(order, iterations): real {

  var Dom = {1..order, 1..order};
  var Array : [Dom] real;

  var t = new Timer();

  t.start();

  var tmp: real = 0;

  for i in 1..iterations {
    tmp = 0;
    for (i,j) in Dom do
      for k in 1..iterations do
        tmp += i+j*k;
    Array += tmp;
  }


  return t.elapsed()/iterations;
}
