proc main {
  use SysCTypes;

  extern proc tcb_install_callbacks_1();
  extern proc tcb_uninstall_callbacks_1();
  extern proc tcb_install_callbacks_2();
  extern proc tcb_uninstall_callbacks_2();
  extern proc tcb_wait_for_nCallbacks(nCallbacks: c_int);
  extern proc tcb_report();

  var counter: sync int = 0;

  tcb_install_callbacks_1();
  tcb_install_callbacks_2();

  cobegin {
    counter = counter + 1;
    counter = counter + 1;
  }

  on Locales(numLocales-1) {
    counter = counter + 1;
  }

  tcb_wait_for_nCallbacks((if numLocales == 1 then 12 else 24): c_int);
  tcb_uninstall_callbacks_1();

  cobegin {
    counter = counter + 1;
    counter = counter + 1;
  }

  on Locales(numLocales-1) {
    counter = counter + 1;
  }

  tcb_wait_for_nCallbacks((if numLocales == 1 then 18 else 36): c_int);
  tcb_uninstall_callbacks_2();
  tcb_report();
}
