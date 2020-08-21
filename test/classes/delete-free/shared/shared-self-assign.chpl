// the three code blocks below are from
// https://github.com/chapel-lang/chapel/issues/16121
// Note that execopts passes --memLeaks explicitly for this test
{
  class C { var x = 10; }

  var x: shared C?;

  x = new shared C(20);
  x = x;
  writeln(x); // prints nil and leaks for `shared`, `unmanaged` works fine
}

{
  class C { var x = 10; }

  var a: [0..0] shared C?;
  a[0] = new shared C(20);

  a[0] = a[a.size-1];  // leaks

  writeln(a);
}

{
  private use List;

  class T { var value = 0; }

  record heap {

    var _data: list((shared T, shared T));

    proc push(in element) {
      _data.append(element);
    }

    proc pop() {
      _data(0) <=> _data(_data.size-1);
      var ret = _data.pop();
      return ret;
    }
  }

  var l = new heap();
  l.push((new shared T(1), new shared T(2)));
  //l.push((new shared T(3), new shared T(4))); // uncomment and leak goes away


  writeln(l.pop());
}
