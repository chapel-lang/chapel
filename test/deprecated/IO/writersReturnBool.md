# Steps to clean up Deprecation of bool-returning ch.write_ methods:

This was a complex deprecation. Cleaning up will be non-trivial

IO.chpl:
---

1. Delete `config param WritersReturnbool`
2. Delete overloads of `channel.write`, `channel.writeln`, `channel.writeBytes`, `channel.writebits`, and `channel.writef` that have `where WritersReturnBool == true` clauses
3. Delete internals: `_channel._write`, `channel._writeln`, `channel._writef`, and `channel._writeBytes`.Move their implementaitons into the relevant writer procs with `where WritersReturnBool == false` claues
4. Delete all the `where WritersReturnBool == false` clauses


ChapelIO.chpl
---

1. Switch `ch._write`, `ch._writeln`, and `ch._writef` calls to `ch.write`, `ch.writeln` and `ch.writef`
2. Delete the `where WritersReturnBool == true` overloads of `writef`
3. Delete the `where WritersReturnBool == false` cluases from the other `writef` procs


All Modules (and tools/mason/*)
---

Switch `ch._write`, `ch._writeln`, `ch._writef`, and `ch._writeBytes` calls to `ch.write`, `ch.writeln`, `ch.writef` and `ch.writeBytes`


Tests
---

1. Remove COMPOPTS with `-s WritersReturnBool=false` from the test directory 
2. In `start_test.py` : `set_up_environment()`, remove ` + " -s WritersReturnBool=false"` from lines 693, 695 (at the time this was written)
3. Update `test/library/standard/Spawn/cat-stdout-stderr` to use `ch.write` instead of `ch._write`
