// see issue #24594; same for classes
// can compile with --minimal-modules upon removal of compilerWarning()

record SymEntry {
  type etype;
  var a = makeDistArray();
  proc init(a) {
    this.etype = nothing;
    this.a = a;
  }
}

proc makeDistArray() do return true;

const se = new SymEntry(5);        // should be an error
compilerWarning(se.type:string);
compilerWarning(se.a.type:string);

const se2 = new SymEntry(true);    // should be OK
compilerWarning(se2.type:string);
compilerWarning(se2.a.type:string);
