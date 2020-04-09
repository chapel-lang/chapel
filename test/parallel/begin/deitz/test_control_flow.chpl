
proc foo() {
  for 1..3 {
    begin {
      return false;
      break;
      continue;
    }
    cobegin {
      return true;
      break;
      continue;
    }
    on Locales[0] {
      return false;
      break;
      continue;
    }
  }
  coforall i in 1..3 {
    return false;
    break;
    continue;
  }
  forall i in 1..3 {
    return true;
    break;
    continue; // OK
  }
  return true;
}

// Do the same for an iterator so that "yield" is legitimate.
iter bar(param tag) { 
  for 1..3 {
    begin {
      yield 7;
      return;
      break;
      continue;
    }
    cobegin {
      yield 7; // ok
      return;
      break;
      continue;
    }
    on Locales[0] {
      yield 7; // ok
      return;
      break;
      continue;
    }
  }
  coforall 1..3 {
    yield 7; // ok
    return;
    break;
    continue;
  }
  forall 1..3 {
    yield 7; // ok
    return;
    break;
    continue; // ok
  }
}

proc baz {
  label namedOuter
    for iii in Locales {
      forall jjj in Locales {

        continue;             // OK - relates to 'forall'
        continue namedOuter;  // error: cannot "jump out" of a forall

        label namedInner
          for kkkk in Locales {
            continue;            // OK - relates to namedInner
            continue namedInner; // ditto
            continue namedOuter; // error: cannot "jump out" of a forall

          }
      }
    }
  return 77;
}


writeln(foo());
writeln(bar(iterKind.standalone));
writeln(baz());
