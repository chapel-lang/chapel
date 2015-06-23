const on_locale_zero_value = true;

on Locales[1] {
  writeln(here.id != on_locale_zero_value.locale.id);
}

