proc main {
  extern proc install_callbacks();
  extern proc report_callbacks(nCallbacks: c_int);

  var counter: sync int = 0;

  install_callbacks();

  cobegin {
    counter = counter + 1;
    counter = counter + 1;
  }

  on Locales(numLocales-1) {
    counter = counter + 1;
  }

  report_callbacks((if numLocales == 1 then 8 else 16): c_int);
}
