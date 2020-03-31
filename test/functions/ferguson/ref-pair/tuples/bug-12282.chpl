record item {
  var data: int = 0;

  proc writeThis(f) throws { f <~> data; }
}

record itemset {
  var dataset: 3*item;

  proc up() {
    dataset(0).data += 1;
  }
}

const zero_itemset: itemset;

proc main() {
  var is1 = zero_itemset;
  is1.up();
  writeln(is1);

  var is2: itemset;
  is2 = zero_itemset; // error: const actual is passed to 'ref' formal '_arg2' of =
  is2.up();
  writeln(is2);

  var is3: itemset;
  is3 = zero_itemset: itemset;
  is3.up();
  writeln(is3);

  assert(is1 == new itemset((new item(1), new item(0), new item(0))),
         is1 == is2,
         is2 == is3);
}
