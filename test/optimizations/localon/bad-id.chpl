
proc main() {
  var x = 0;

  var LM = here:LocaleModel;

  on LM.getChild(0) {
    writeln(x.locale);
  }

  local on LM.getChild(0) {
    writeln(x.locale);
  }
}
