
/* feature request: Compiler warning/error if enum not covered by select.

   Compiler currently does not warn if there's an enum value that is not
   checked by a when/othewise.
*/

enum clrs { RED, GREEN, BLUE };
var c = clrs.GREEN;

select c {
  when clrs.RED do writeln("c is RED");
  when clrs.BLUE do writeln("c is BLUE");
}

