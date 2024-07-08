/* A prior bug showed up when the remote variable was global (i.e., not
   in a function). This test checks the alternative (it IS in a function)
   to make sure both work. 'on_statement_var_decl' tests the global case. */
proc main() {
  on Locales(1) var x: real;

  on Locales(1) {
    writeln((x.locale, x));
    local {
      x = 2.0;
    }
    on Locales(0) {
      writeln((x.locale, x));
    }
  }
}
