// This test shows a pattern that was causing problems
// when deprecating channel in favor of fileReader / fileWriter

record G {
  param p;
  param q;
}

type GT = G(p=true, ?);

record R {
  param q;
  var x: GT(q=q); 
}

proc main() {
  var b = new R(3);
  writeln(b.type:string, " ", b.x.type:string);
  
  var c:R(4);
  writeln(c.type:string, " ", c.x.type:string);
}
