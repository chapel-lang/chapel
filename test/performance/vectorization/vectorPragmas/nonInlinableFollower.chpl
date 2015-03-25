module iters {
  iter myiter(nn: int, nt: int) {
    for i in 0..#nt {
      for j in i*nn..#nn {
        yield j;
      }
    }
  }

  // for loop in standalone should NOT get vector pragma because loop can't be
  // inlined (2 yields), and will result in a 'reentrant' advance function
  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.standalone {
    coforall i in 0..#nt {
      for j in i*nn..#nn-1 {
        yield j;
      }
      yield i*nn..#nn.last;
    }
  }

  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.leader {
    coforall i in 0..#nt {
      yield i*nn..#nn;
    }
  }

  // for loop in follower should NOT get vector pragma because loop can't be
  // inlined (2 yields), and will result in a 'reentrant' advance function
  iter myiter(nn:int, nt: int, followThis, param tag: iterKind) where tag == iterKind.follower {
      for j in followThis # (followThis.size - 1) {
        yield j;
      }
      yield followThis.last;
  }
}
