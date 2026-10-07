
/*****
      ex_domainop.chpl -
      Examples of operations on domains: accessing it indices, changing its
      size, etc.  See text for details.

      c 2015-2018 Primordial Machine Vision Systems
*****/

var dom1 = { 1..3, 4..6 };

writeln("Demonstrating domain iteration");
writeln(" ", dom1, "   = ", for i in dom1 do i);

proc domfn1((x, y) : (int, int)) {
  writeln("  ", x, " + ", y, " = ", x+y);
  return x+y;
}

writeln(" as function argument/array assignment:");
var arr1 : [dom1] int;
var arr2 : [dom1] int;

/* NOTE: this assignment has been serialized to ensure deterministic output. */
serial { arr2 = domfn1(dom1); }
writeln("  arr2");
writeln(arr2);

writeln("\n adjusting domain iterations");
writeln("  dom1 by 2          ", dom1 by 2,
        "   = ", for i in dom1 by 2 do i);
writeln("  dom1 # (2, 1)      ", dom1 # (2, 1),
        "   = ", for i in dom1 # (2,1) do i);

var dom2 = { 1..10, 20..30 };

writeln("\n adjusting domain ranges");
writeln("  base dom2                        ", dom2);
writeln("  dom2 expand(5,3)                 ", dom2.expand(5,3));
writeln("  dom2 expand(5,3)  expand(-2,-1)  ", dom2.expand(5,3).expand(-2,-1));
writeln("  dom2 translate(4,5)              ", dom2.translate(4,5));

writeln("\n inside/outside domains");
writeln("  dom2 exterior(2,3)               ", dom2.exterior(2,3));
writeln("  dom2 exterior(-2,-3)             ", dom2.exterior(-2,-3));
writeln("  dom2 interior(2,3)               ", dom2.interior(2,3));
writeln("  dom2 interior(-2,-3)             ", dom2.interior(-2,-3));
writeln("  base unchanged                   ", dom2);
