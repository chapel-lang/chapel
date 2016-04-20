for i in chpl__primes {
  if i % 4 != 3 { 
    writeln(i, " is ", i % 4, " mod 4");
    assert(false);
  }
}

