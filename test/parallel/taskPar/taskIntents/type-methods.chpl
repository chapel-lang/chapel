record R {
  proc type bar(i) { return i; }
  iter type iterator(dom) {
    for i in dom {
      yield i;
    }
  }
  iter type iterator(param tag: iterKind, dom) where tag == iterKind.standalone {
    foreach i in dom {
      yield this.bar(i);
    }
  }

  proc type useForall(dom) {
    var arr: [dom] int;
    forall i in dom {
      arr[i] = bar(i);
    }
    writeln(arr);
  }

  proc type useForeach(dom) {
    foreach i in dom {
      writeln(this.bar(i));
    }
  }

  proc type useCoforall(dom) {
    var arr: [dom] int;
    coforall i in dom {
      arr[i] = bar(i);
    }
    writeln(arr);
  }

  proc type useBegin(dom) {
    // synchronization to work around https://github.com/chapel-lang/chapel/issues/27769
    var x: atomic int = 1;
    begin {
      writeln(bar(dom));
      x.add(-1);
    }
    x.waitFor(0);
  }
  proc type useCobegin(dom) {
    cobegin {
      writeln(this.bar(dom));
      writeln(bar(dom));
    }
  }

}
{
  var arr: [1..10] int;
  forall i in R.iterator({1..10}) {
    arr[i] = i;
  }
  writeln(arr);
}

R.useForall({1..10});
R.useForeach({1..10});
R.useCoforall({1..10});
R.useBegin({1..10});
R.useCobegin({1..10});
