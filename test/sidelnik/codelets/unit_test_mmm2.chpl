use CodeletDist;

proc main
{
  //var dist = new dmap(new DefaultDist());
  var dist = new dmap(new CodeletDist());
  const N = [0..15, 0..15];
  var dom = N dmapped dist;
  var A, B, C : [dom] int;

  const loopRange = [0..15];
  var loopDom = loopRange dmapped dist;
  
  /* Initialize Arrays */
  for i in loopDom {
    for j in loopDom {
      A(i, j) = i;
      B(i, j) = j;
      C(i, j) = 0;
    }
  }

  /* Kernel */
  for i in loopDom {
    forall j in loopDom {
      for k in loopDom {
        C(i, j) += A(i, k) * B(k, j);
      }
    }
  }

  writeln(A);
  writeln();
  writeln(B);
  writeln();
  writeln(C);
}
