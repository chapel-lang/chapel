class C { }
type t = (shared C?, shared C?);
compilerWarning("isBorrowedClass(tuple) is " + isBorrowedClass(t):string); 
type useT = if isBorrowedClass(t) then (t:owned) else t;
