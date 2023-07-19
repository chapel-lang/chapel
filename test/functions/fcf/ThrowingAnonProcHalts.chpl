// Make sure that halting works for throws in anon procs.
const p = proc(): int throws { throw new Error('Halt!'); };
try! writeln(p());
