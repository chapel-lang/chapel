/* ChapelUtil.chpl:1 */
void __init_ChapelUtil(void) {
  chpl_bool T1;
  T1 = (!__run_ChapelUtil_firsttime8);
  if (T1) {
    goto _end___init_ChapelUtil;
  }
  __run_ChapelUtil_firsttime8 = false;
  _end___init_ChapelUtil:;
  return;
}

