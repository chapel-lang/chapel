module iters {
  iter myiter(nn: int, nt: int) {
    foreach i in 0..#nt {
      foreach j in i*nn..#nn {
        yield j;
      }
    }
  }

  // foreachs should get vectorized
  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.standalone {
    coforall i in 0..#nt {
      foreach j in i*nn..#nn {
        foreach k in j..j {
          yield k;
        }
      }
    }
  }

  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.leader {
    coforall i in 0..#nt {
      yield i*nn..#nn;
    }
  }

  // neither loop will get the pragma since this iter cannot be zippered
  // inline. I this is reasonable since it's pretty tricky to zipper nested
  // loops together. Not to say it's impossible.
  iter myiter(nn:int, nt: int, followThis, param tag: iterKind) where tag == iterKind.follower {
    foreach i in followThis {
      foreach j in i..i {
        yield j;
      }
    }
  }
}
