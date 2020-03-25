class MyClass {
  var field:int;
}

record MyRecord {
  var field:int;
}

var i:int;
var r:MyRecord;
var o:owned MyClass?;
var s:shared MyClass?;
var u:unmanaged MyClass?;
var b:borrowed MyClass?;
var c:MyClass = new borrowed MyClass(1);

proc myassert(param got) {
  if got != true then
    compilerWarning("myassert failed");
}

// check isOwnedClass with type argument
myassert( !isOwnedClass(int) );
myassert( !isOwnedClass(MyRecord) );
myassert( isOwnedClass(owned MyClass) );
myassert( !isOwnedClass(shared MyClass) );
myassert( !isOwnedClass(unmanaged MyClass) );
myassert( !isOwnedClass(borrowed MyClass) );
// check isOwnedClass with value argument
myassert( !isOwnedClass(i) );
myassert( !isOwnedClass(r) );
myassert( isOwnedClass(o) );
myassert( !isOwnedClass(s) );
myassert( !isOwnedClass(u) );
myassert( !isOwnedClass(b) );
myassert( !isOwnedClass(c) );


// check isSharedClass with type argument
myassert( !isSharedClass(int) );
myassert( !isSharedClass(MyRecord) );
myassert( !isSharedClass(owned MyClass) );
myassert( isSharedClass(shared MyClass) );
myassert( !isSharedClass(unmanaged MyClass) );
myassert( !isSharedClass(borrowed MyClass) );
// check isSharedClass with value argument
myassert( !isSharedClass(i) );
myassert( !isSharedClass(r) );
myassert( !isSharedClass(o) );
myassert( isSharedClass(s) );
myassert( !isSharedClass(u) );
myassert( !isSharedClass(b) );
myassert( !isSharedClass(c) );

// check isUnmanagedClass with type argument
myassert( !isUnmanagedClass(int) );
myassert( !isUnmanagedClass(MyRecord) );
myassert( !isUnmanagedClass(owned MyClass) );
myassert( !isUnmanagedClass(shared MyClass) );
myassert( isUnmanagedClass(unmanaged MyClass) );
myassert( !isUnmanagedClass(borrowed MyClass) );
// check isUnmanagedClass with value argument
myassert( !isUnmanagedClass(i) );
myassert( !isUnmanagedClass(r) );
myassert( !isUnmanagedClass(o) );
myassert( !isUnmanagedClass(s) );
myassert( isUnmanagedClass(u) );
myassert( !isUnmanagedClass(b) );
myassert( !isUnmanagedClass(c) );

// check isBorrowedClass with type argument
myassert( !isBorrowedClass(int) );
myassert( !isBorrowedClass(MyRecord) );
myassert( !isBorrowedClass(owned MyClass) );
myassert( !isBorrowedClass(shared MyClass) );
myassert( !isBorrowedClass(unmanaged MyClass) );
myassert( isBorrowedClass(borrowed MyClass) );
// check isBorrowedClass with value argument
myassert( !isBorrowedClass(i) );
myassert( !isBorrowedClass(r) );
myassert( !isBorrowedClass(o) );
myassert( !isBorrowedClass(s) );
myassert( !isBorrowedClass(u) );
myassert( isBorrowedClass(b) );
myassert( isBorrowedClass(c) );
