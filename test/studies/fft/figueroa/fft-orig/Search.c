/* Search.chpl:1 */
void __init_Search(void) {
  chpl_bool T1;
  T1 = (!__run_Search_firsttime23);
  if (T1) {
    goto _end___init_Search;
  }
  __run_Search_firsttime23 = false;
  _end___init_Search:;
  return;
}

