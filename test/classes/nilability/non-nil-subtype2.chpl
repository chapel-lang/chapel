class Parent { }
class Child : Parent { }

writeln("borrowed - borrowed");
writeln(isSubtype(borrowed Child, borrowed Parent));
writeln(isSubtype(borrowed Child, borrowed Parent?));
writeln(isSubtype(borrowed Child?, borrowed Parent));
writeln(isSubtype(borrowed Child?, borrowed Parent?));

writeln("owned - owned");
writeln(isSubtype(owned Child, owned Parent));
writeln(isSubtype(owned Child, owned Parent?));
writeln(isSubtype(owned Child?, owned Parent));
writeln(isSubtype(owned Child?, owned Parent?));

writeln("borrowed - generic");
writeln(isSubtype(borrowed Child, Parent));
writeln(isSubtype(borrowed Child, Parent?));
writeln(isSubtype(borrowed Child?, Parent));
writeln(isSubtype(borrowed Child?, Parent?));

writeln("owned - generic");
writeln(isSubtype(owned Child, Parent));
writeln(isSubtype(owned Child, Parent?));
writeln(isSubtype(owned Child?, Parent));
writeln(isSubtype(owned Child?, Parent?));

proc toBorrowedParent(type t:Parent) { }
proc toBorrowedParentQ(type t:Parent?) { }
proc toOwnedParent(type t:Parent) { }
proc toOwnedParentQ(type t:Parent?) { }
proc toGenericParent(type t:Parent) { }
proc toGenericParentQ(type t:Parent?) { }

// these should all resolve
// see also #13541
toBorrowedParent(borrowed Child);
toBorrowedParentQ(borrowed Child);
toBorrowedParentQ(borrowed Child?);

toOwnedParent(owned Child);
toOwnedParentQ(owned Child);
toOwnedParentQ(owned Child?);

toBorrowedParent(owned Child);
toBorrowedParentQ(owned Child);
toBorrowedParentQ(owned Child?);

toGenericParent(borrowed Child);
toGenericParentQ(borrowed Child);
toGenericParentQ(borrowed Child?);

toGenericParent(owned Child);
toGenericParentQ(owned Child);
toGenericParentQ(owned Child?);
