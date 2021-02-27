use BlockDist;

var D = newBlockDom(1..10, 1..10);

{
  // access index doesn't match cleanly
  var A: [D] int;
  forall (i,j) in D {
    A[i+0, j] = i;
  }
}

{
  // access base is yielded from the loop iterator
  var ArrOfArr: [D][1..10] int;

  forall arr in ArrOfArr {
    for i in 1..10 {
      arr[i] = i;
    }
  }
}

{
  // access base is defined in loop body
  forall i in D {
    var a: [1..10, 1..10] 2*int;

    a[i] = i;
  }
}

{
  // access base is a shadow variable
  var A: [D] 2*int;
  forall i in D with (ref a = A) {
    a[i] = i;
  }
}

{
  // the closest locality-determining to the call is not the forall in question
  var A: [D] 2*int;
  forall i in D {
    on Locales[0] {
      A[i] = i;
    }
  }

  // ditto
  /* 
   This currently hits an auto-aggregation bug
  forall i in D {
    forall a in A {
      A[i] = a;
    }
  }

  */
}
