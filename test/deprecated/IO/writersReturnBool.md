# Steps to clean up Deprecation of bool-returning ch.write_ methods:
IO.chpl:
---

1. Delete the declaration of: `config param WritersReturnBool`
2. Delete all overloads of `channel.write`, `channel.writeln`, `channel.writeBytes`, `channel.writebits`, and `channel.writef` that have `where WritersReturnBool == true` clauses
3. Delete internal versions of ch methods: `_channel._write`, `channel._writeln`, `channel._writef`, and `channel._writeBytes`
    - Move their implementations back into the corresponding channel methods with `where WritersReturnBool == false` clauses. (i.e. move the body of `ch._write` into `ch.write`)
4. Delete the `where WritersReturnBool == false` clauses from the remaining channel methods


ChapelIO.chpl
---

1. Switch `ch._write`, `ch._writeln`, and `ch._writef` calls back to `ch.write`, `ch.writeln` and `ch.writef`
2. Delete the `where WritersReturnBool == true` overloads of `writef`
3. Delete the `where WritersReturnBool == false` clauses from the other `writef` procs


All Modules (and tools/mason/*)
---

Switch `ch._write`, `ch._writeln`, `ch._writef`, and `ch._writeBytes` calls back to `ch.write`, `ch.writeln`, `ch.writef` and `ch.writeBytes`


Tests
---

1. In `start_test.py` : `set_up_environment()`, remove ` + " -s WritersReturnBool=false"` from lines 693, 695 (at the time this was written)
2. Update `test/library/standard/Spawn/cat-stdout-stderr` to use `ch.write` instead of `ch._write`
