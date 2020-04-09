/*use ChapelBase; // DEBUG*/

iter test(n:int = 10) {
  for i in 1..n do yield i;
}

iter test(param tag:iterKind, n:int = 10) 
  where tag == iterKind.leader {

    var lock: atomic int;
    /*writeln("addr of lock should be: ", chpldev_refToString(lock)); // DEBUG*/
    for i in 1..n { 
      writeln("i = ", i , " lock = ", lock);
      yield (i, lock); 
    }
}

iter test(param tag:iterKind, n:int = 10, followThis) 
  where tag == iterKind.follower {
    /*writeln("Actual addr of lock in follower: ", chpldev_refToString(followThis(2))); // DEBUG*/
    followThis(1).add(1);
    yield followThis(0);
    
  }

forall i in test() {}
