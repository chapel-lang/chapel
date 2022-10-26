// test that normal instances of "illegal use of function that does not return a value"
//  still print the non-specialized error message

proc returnsVoid(a: [] int) {
    a[0] += 1;
}

if returnsVoid([1, 2, 3]) then
    writeln("this won't print...");
