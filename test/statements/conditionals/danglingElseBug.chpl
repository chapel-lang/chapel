// reproducer from https://github.com/chapel-lang/chapel/issues/29429
// test code is AI generated

// Standalone reproducer for a C-backend dangling-else codegen bug.
//
// When an `else` block consists of a single `if` whose condition reads a
// *wide* reference, the C backend collapses it to `else if` but the wide
// read is emitted as a separate statement first, giving
//     } else chpl_gen_comm_get(&tmp, ...); if (tmp == K) { ... } else { ... }
// The GET becomes the whole else-body, the `if` escapes it and tests an
// uninitialized temp, and the rest of the chain runs after the *first*
// branch already ran.
//
// Ingredients:
//   * --no-local (or CHPL_COMM!=none) plus --no-infer-local-fields so the
//     zip iterator class's array-instance field stays wide;
//   * a zippered loop that is lowered to iterator classes rather than being
//     inlined (the array-producing for-expression in the body does this);
//   * an if/else-if chain whose else-if conditions are single comparisons.
//
// Compile:  chpl --no-local --no-infer-local-fields --savec gen3 foo2.chpl
// Inspect:  grep -n '} else chpl_gen_comm_get' gen3/foo2.c
// Expected output on every locale: (2, 0, 0, 1)
//   kinds 0 and 2 -> twos, kind 6 with objType 3 -> others, nothing -> fours
// Buggy output: fours != 0 (kind 6/objType 3 also falls through the chain)
use CTypes;

config const n = 10;

var kinds: [0..#n] int;
var objTypes: [0..#n] int;
var sizes: [0..#n] int;

proc main() throws {
  kinds = 0..#n;
  sizes = 1;
  objTypes[6] = 3;
  coforall loc in Locales do on loc {
    const colDom = {0..#n};
    var datas: [colDom] c_ptrConst(void);

    var twos, threes, fours, others = 0;
    for (data, kind, objType, size) in zip(datas, kinds, objTypes, sizes) {
      if objType == 3 && kind == 6 {
        others += 1;
      } else if objType == 3 {
        others += 2;
      } else if kind == 0 || kind == 2 {
        twos += 1;
      } else if kind == 6 {
        // array-producing for-expr, like the Parquet write() body; keeps the
        // zip loop from being inlined
        const lvls = for s in 0..#size do (s != 0): int(16);
        fours += lvls.size;
      }
    }
    writeln((twos, threes, fours, others));
  }
}

