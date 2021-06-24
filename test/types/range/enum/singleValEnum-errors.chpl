config param errorCase = 1;

enum color { red };
use color;

var r = red..red;

if errorCase == 1 then
  writeln(r.translate(0));

if errorCase == 2 {
  writeln(r.interior(0)); writeln(r.interior(1)); writeln(r.interior(2));
}
if errorCase == 3 then
  writeln(r.exterior(0));

if errorCase == 4 then
  writeln(r.expand(0));

if errorCase == 5 then
  writeln(r.offset(0));
