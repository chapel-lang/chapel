writeln("all var");
{
  var A: [1..10] real;
  writeln("After A");
  var B = A;
  writeln("After B");
  var C: [1..10] real = A;
  writeln("After C");
  on Locales[numLocales-1] {
    var D = A;
    writeln("After D");
    local {
      var sz = D.size;
    }
  }
}
writeln();

writeln("const A, var others");
{
  const A: [1..10] real;
  writeln("After A");
  var B = A;
  writeln("After B");
  var C: [1..10] real = A;
  writeln("After C");
  on Locales[numLocales-1] {
    var D = A;
    writeln("After D");
    local {
      var sz = D.size;
    }
  }
}
writeln();

writeln("var A, const others");
{
  var A: [1..10] real;
  writeln("After A");
  const B = A;
  writeln("After B");
  const C: [1..10] real = A;
  writeln("After C");
  on Locales[numLocales-1] {
    const D = A;
    writeln("After D");
    local {
      var sz = D.size;
    }
  }
}
writeln();

writeln("var Dom, var A, const others");
{
  var Dom = {1..10};
  var A: [Dom] real;
  writeln("After A");
  const B = A;
  writeln("After B");
  const C: [1..10] real = A;
  writeln("After C");
  on Locales[numLocales-1] {
    const D = A;
    writeln("After D");
    local {
      var sz = D.size;
    }
  }
}
