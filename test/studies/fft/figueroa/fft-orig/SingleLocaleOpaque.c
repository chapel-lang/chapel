/* SingleLocaleOpaque.chpl:1 */
void __init_SingleLocaleOpaque(void) {
  chpl_bool T1;
  T1 = (!__run_SingleLocaleOpaque_firsttime13);
  if (T1) {
    goto _end___init_SingleLocaleOpaque;
  }
  __run_SingleLocaleOpaque_firsttime13 = false;
  if (__run_List_firsttime16) {
    __init_List();
  }
  __run_List_firsttime16 = false;
  _end___init_SingleLocaleOpaque:;
  return;
}

