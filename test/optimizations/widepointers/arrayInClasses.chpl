
class Chunk {
  var data : [1..10] int;
}

// This test exists to measure a pattern where a user has manually chunked
// their computation into a class per locale, which contains arrays. When we
// know that the class is local, we do not want wide pointer overhead when
// using those arrays.
//
// Sometimes the arrays in such classes might need to be accessed remotely
// (e.g. exchanging array elements on edge of stencil computation). The
// compiler should make the static type of such arrays a wide pointer, but
// through optimizations should be able to localize that pointer when
// appropriate.
//
// This block exists to ensure that the static type of the 'data' array is
// a wide pointer for this test.
{
  var c = new unmanaged Chunk();
  on Locales[numLocales-1] do c.data[1] = 5;
  delete c;
}

proc testSimpleClass() {
  writeln("----- simple class -----");
  var c = new unmanaged Chunk();
  writeln("class is wide: ", __primitive("is wide pointer", c));
  writeln("array is wide: ", __primitive("is wide pointer", c.data._value.shiftedData));
}

proc testArrayOfClasses() {
  writeln("----- array of classes -----");

  var chunks : [0..#numLocales] unmanaged Chunk?;

  for loc in Locales do on loc {
    chunks[loc.id] = new unmanaged Chunk();
  }

  coforall c in chunks do on c {
    writeln("class is wide: ", __primitive("is wide pointer", c));
    ref data = c!.data;
    writeln("array is wide: ", __primitive("is wide pointer", data._value.shiftedData));
    for d in data do d += 1;
  }

  for c in chunks do writeln(c!.data);

  delete chunks;
}

proc main() {
  testSimpleClass();
  testArrayOfClasses();
}
