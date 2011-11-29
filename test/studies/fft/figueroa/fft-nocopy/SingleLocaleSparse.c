/* SingleLocaleSparse.chpl:1 */
void __init_SingleLocaleSparse(void) {
  chpl_bool T1;
  T1 = (!__run_SingleLocaleSparse_firsttime11);
  if (T1) {
    goto _end___init_SingleLocaleSparse;
  }
  __run_SingleLocaleSparse_firsttime11 = false;
  if (__run_Search_firsttime23) {
    __init_Search();
  }
  __run_Search_firsttime23 = false;
  if (__run_List_firsttime16) {
    __init_List();
  }
  __run_List_firsttime16 = false;
  _end___init_SingleLocaleSparse:;
  return;
}

