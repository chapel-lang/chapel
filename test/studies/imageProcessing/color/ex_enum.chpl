
/*****
      ex_enum.chpl -
      Examples showing the (ab)use of enums.  See text for details.

      Call:
        ex_enum
          --space=[HSV|LAB|YUV|RGB]

      c 2015-2018 Primordial Machine Vision Systems
*****/

/* HSV=1, LAB=2, YUV=1, RGB=2 */
enum clrspace_v2 {
  HSV=1, LAB=2, YUV=1, RGB=2
}

if (clrspace_v2.HSV:int == clrspace_v2.YUV:int) then writeln("HSV, YUV identical");
if (clrspace_v2.LAB:int == clrspace_v2.RGB:int) then writeln("LAB, RGB identical");
if (clrspace_v2.HSV:int == clrspace_v2.LAB:int) then writeln("HSV, LAB identical");

proc print_space(space : clrspace_v2) {
  writeln("picked color space ", space);
}

print_space(clrspace_v2.HSV);
print_space(clrspace_v2.YUV);

config const space : clrspace_v2;
writef("\nFrom the command line you ");
print_space(space);

enum clrmasks {
  RED = 0xff0000, GREEN = 0x00ff00, BLUE = 0x0000ff
}

/* Using the value directly works. */
writef("\nenum vals             R 0x%06xu  G 0x%06xu  B 0x%06xu\n",
       clrmasks.RED, clrmasks.GREEN, clrmasks.BLUE);

/* You need to cast here because the compiler can't figure out the type to
   pass to the & operator. */
writef("decompose 0x123456    R 0x%06xu  G 0x%06xu  B 0x%06xu\n",
       0x123456 & (clrmasks.RED : uint), 0x123456 & (clrmasks.GREEN : uint),
       0x123456 & (clrmasks.BLUE : uint));
writeln("convert int to enum   ", 0xff0000 : clrmasks);


/*
  This fails
if (clrmasks.RED == 0xff0000) {
  writeln("after check enum is   ", 0xff0000 : clrmasks);
}
and this doesn't
if (clrmasks.RED == 0xff0000 : clrmasks) {
  writeln("after check enum is   ", 0xff0000 : clrmasks);
}
*/
