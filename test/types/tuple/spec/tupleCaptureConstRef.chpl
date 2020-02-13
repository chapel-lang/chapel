//
// What happens when you capture a `const ref` element in a tuple?
//

record someRecord { var x: int = 0; }

var rec: someRecord;

const ref foo = rec;

// Shouldn't this be illegal, since "foo" is a `const ref`?
(foo,)[1].x = 128;

writeln(foo.x:string);
