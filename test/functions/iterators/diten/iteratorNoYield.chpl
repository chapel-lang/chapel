def itr() {
  for i in 1..5 do
    yield i;
}

def itr(param tag: iterator) where tag == iterator.leader { }
def itr(param tag: iterator, follower) where tag == iterator.follower { }

forall i in itr() do
  writeln(i);
