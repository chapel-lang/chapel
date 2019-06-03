proc foo(ref a:bool(?), const ref b:bool(?)) { a = b; }
var a = false, b = true;
foo(a, b);
writeln((a,b));
