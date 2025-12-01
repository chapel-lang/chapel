use Debugger;

proc main() {

  writeln("Running on locale ", here.id);
  var a: [1..10] int;
  for i in a.domain do
    a[i] = i * here.id;
  writeln("Array on locale ", a.locale.id, ": ", a);
  breakpoint;

  on Locales[1] {
    writeln("In on-clause on locale ", here.id);
    writeln("Array on locale ", a.locale.id, ": ", a);
    breakpoint;
  }

}
