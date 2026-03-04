
/*****
      ex_rangeop.chpl -
      Examples of operations on ranges: slicing, methods.  See text for
      details.

      c 2015-2018 Primordial Machine Vision Systems
*****/

var rng1 = 1..10;
var rng2 = 1..20 by 4 align 3;

writeln("Demonstrating range operations");
writeln(" base rng1                ", rng1,
        "   = ", for i in rng1 do i);
writeln(" rng1[..8]                ", rng1[..8],
        "   = ", for i in rng1[..8] do i);
writeln(" rng1[2.. by 3]           ", rng1[2.. by 3],
        "   = ", for i in rng1[2.. by 3] do i);
writeln(" rng1[3.. by 3 align 2]   ", rng1[3.. by 3 align 2],
        "   = ", for i in rng1[3.. by 3 align 2] do i);
writeln(" rng1[..8 by 3]           ", rng1[..8 by 3],
        "   can't iterate, bad alignment");
writeln(" rng1[..8 by 3 align 2]   ", rng1[..8 by 3 align 2],
        "   = ", for i in rng1[..8 by 3 align 2] do i);
writeln(" rng1[3.. # 3 by 2]       ", rng1[3.. # 3 by 2],
        "   = ", for i in rng1[3.. # 3 by 2] do i);
writeln(" rng1[3.. by 2 # 3]       ", rng1[3.. by 2 # 3],
        "   = ", for i in rng1[3.. by 2 # 3] do i);
writeln(" rng1 + 2                 ", rng1 + 2,
        "   = ", for i in (rng1 + 2) do i);
writeln(" rng1 - 2                 ", rng1 - 2,
        "   = ", for i in (rng1 - 2) do i);
writeln(" rng1.contains(9)           ", rng1.contains(9));
writeln(" rng1.contains(11)          ", rng1.contains(11));
writeln(" rng1.contains(2..5)        ", rng1.contains(2..5));
writeln(" rng1.contains(2..11)       ", rng1.contains(2..11));
writeln(" rng1.contains(2..)         ", rng1.contains(2..));
writeln(" rng1.contains(..8)         ", rng1.contains(..8));
writeln(" range rng2 with step, align   ", rng2,
        "   = ", for i in rng2 do i);
writeln(" rng2.expand(2)                ", rng2.expand(2),
        "   = ", for i in rng2.expand(2) do i);
writeln(" rng2.exterior(3)              ", rng2.exterior(3),
        "   = ", for i in rng2.exterior(3) do i);
writeln(" rng2.exterior(-3)             ", rng2.exterior(-3),
        "   = ", for i in rng2.exterior(-3) do i);
writeln(" rng2.interior(3)              ", rng2.interior(3),
        "   = ", for i in rng2.interior(3) do i);
writeln(" rng2.interior(-3)             ", rng2.interior(-3),
        "   = ", for i in rng2.interior(-3) do i);
writeln(" rng2.offset(3)                ", rng2.offset(3),
        "   = ", for i in rng2.offset(3) do i);
writeln(" rng2.offset(-3)               ", rng2.offset(-3),
        "   = ", for i in rng2.offset(-3) do i);
writeln(" rng2.translate(3)             ", rng2.translate(3),
        "   = ", for i in rng2.translate(3) do i);
writeln(" rng2.translate(-3)            ", rng2.translate(-3),
        "   = ", for i in rng2.translate(-3) do i);
