def iter() {
  for i in 1..5 do
    yield i;
}

def iter(param tag: iterator) where tag == iterator.leader { }
def iter(param tag: iterator, follower) where tag == iterator.follower { }

coforall i in iter() do
  writeln(i);
