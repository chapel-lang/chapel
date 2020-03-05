// #15080: we would like (an array)! to be compatible with an array
// as much as possible. This comes handy, for example, when we want
// a non-nilable array field and initialize it by building a nilable array
// then passing (the nilable array)! to the enclosing class's initializer.

class Block {
  proc init(targetLocales: [] locale) {
    writeln("Block.init");
  }
}

var targetLocales: [1..1] locale?;
targetLocales[1] = here;  // compute the elements of 'targetLocales'

var a: [1..1] locale = targetLocales!;
var b = new Block(targetLocales!);
