module iters {
  iter myiter(nn: int, nt: int) {
    for i in 0..#nt {
      for j in i*nn..#nn {
        yield j;
      }
    }
  }

  // do while loop in standalone with yield should get vector pragma
  pragma "order independent yielding loops"
  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.standalone {
    coforall i in 0..#nt {
      const followThis = i*nn..#nn;
      var   idx  = followThis.first;
      const last = followThis.last;
      const str  = followThis.stride;
      if (idx != last) {
        do {
          yield idx;
          idx += str;
        } while(idx != last + str);
      }
    }
  }

  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.leader {
    coforall i in 0..#nt {
      yield i*nn..#nn;
    }
  }

  // do while loop in follower with yield should get vector pragma
  // Note: This loop will currently not get the pragma because zippered
  // iterator inlining will not fire for this iterator. Zippered iterator
  // inlining gives up if the loop is a 'do while' or if there is a conditional
  // anywhere inside the iterator. I believe both of these restrictions can be
  // relaxed, in which case the do while loop should get the pragma
  pragma "order independent yielding loops"
  iter myiter(nn:int, nt: int, followThis, param tag: iterKind) where tag == iterKind.follower {
    var   idx  = followThis.first;
    const last = followThis.last;
    const str  = followThis.stride;
    if (idx != last) {
      do {
        yield idx;
        idx += str;
      } while(idx != last + str);
    }
  }
}
