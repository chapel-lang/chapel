use GMP;

proc main(){
  writeln("hello world");
  var x = new BigInt();
  x.owned = true;
  x.set(100);
  var y = x; // does not leak
  var z : BigInt = x; // does not leak
  var z2 = new BigInt(x); // does not leak
  var z3 : BigInt; // does not leak
  z3.set(x);  // does not leak
  z3 = 101; // does not leak (uses BigInt.set(int))

  var x2 = new BigInt(true, 8); // does not leak
  var x3 = new BigInt(100); // does not leak
  var x4 = new BigInt("1100101", 2); // does not leak

  var x6 = new BigInt(x4); // does not leak
  var x7 : BigInt = x6; //does not leak
  var x8 = x3; 
  x3.writeThis(stdout); //leaks 1 x 4 bytes of 'glom strings data'
  writeln();

  //mpz_clear(x2.mpz);
  //x2.owned = false;
  //x2.mpz = x3.mpz;

/*
  var r : range = 1..10;
  var A : [r] BigInt;
  var sum = new BigInt(0);
  for (a, i) in zip (A, r) do {
    a = i;
    a.fac_ui((10000*i):uint(32));
    sum.add(sum, a);
  }
  writeln(sum.sizeinbase(10));
  var modulus = new BigInt("10000000000000000000000000000000000000000");
  modulus.nextprime(modulus); 
  writeln(modulus);
  sum.mod(sum, modulus);
  writeln(sum);
*/

}
