class C { }
var global = new unmanaged C() : unmanaged C?;
iter I(): unmanaged C  { yield global; }
for i in I() do;
