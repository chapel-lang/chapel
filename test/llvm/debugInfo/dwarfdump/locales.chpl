use Debugger;

// DWARFDUMP: chpl_localeID_t

proc main() {
  var a: int;

  writeln(a, " ", a.locale, " ", a.locale.id);
  breakpoint;

}
