/* SingleLocaleEnum.chpl:1 */
void __init_SingleLocaleEnum(void) {
  chpl_bool T1;
  T1 = (!__run_SingleLocaleEnum_firsttime12);
  if (T1) {
    goto _end___init_SingleLocaleEnum;
  }
  __run_SingleLocaleEnum_firsttime12 = false;
  if (__run_List_firsttime16) {
    __init_List();
  }
  __run_List_firsttime16 = false;
  _end___init_SingleLocaleEnum:;
  return;
}

