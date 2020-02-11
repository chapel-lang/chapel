//
// When a tuple is constructed with the tuple expression, that tuple
// will refer to any elements (read, capture them by `ref`) for which the
// blank argument intent is `ref` or `const ref`.
//

private use Types;

enum TestKind {
  TestArray
}

class SomeClass { var x: int = 0; }
record someRecord { var x: int = 0; }

var arr: [0..127] int;
var sharedClass1 = new shared SomeClass(128)?;
var sharedClass2 = new shared SomeClass(256)?;
const unmanagedClass1 = new unmanaged SomeClass(512)?;
const unmanagedClass2 = new unmanaged SomeClass(1024)?;
var rec: someRecord;
var num: int;
ref refNum = num;

//
// Records are captured by `ref`, because their default argument intent is
// `const ref`.
//
writeln((rec,)[1].type:string);
(rec,)[1].x = 128;
assert(rec.x == 128);

//
// Arrays are captured by `ref`, because their default argument intent is
// `ref`.
//
writeln((arr,)[1].type:string);
(arr,)[1][0] = 128;
assert(arr[0] == 128);

// Primitive types should be copied.
writeln((num,)[1].type:string);
(num,)[1] = 128;
assert(num == 0);

// Each capture of a primitive type should result in a new copy.
assert((num,)[1] == 0);

// References to primitive types should be copied.
num = 0;
refNum = num;
writeln((refNum,)[1].type:string);
(refNum,)[1] = 128;
assert(num == 0);
assert(refNum == 0);

// Shared classes should be captured by ref.
writeln((sharedClass1,)[1].type:string);
assert(sharedClass1!.x != 0);
(sharedClass1,)[1] = new SomeClass(-64);
assert(sharedClass1!.x == -64);


delete unmanagedClass1;
delete unmanagedClass2;
