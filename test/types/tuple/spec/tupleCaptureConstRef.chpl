//
// What happens when you capture a `const` element in a tuple? This is a
// test from the tuple section of the spec that I've chosen not to embed
// because it's a future.
//
// See: #14902
//

record someRecord { var x: int; }

const rec = new someRecord(1);

// Shouldn't this be illegal, since "rec" is a `const`?
(rec,)[1].x = 128;

writeln(rec.x:string);
