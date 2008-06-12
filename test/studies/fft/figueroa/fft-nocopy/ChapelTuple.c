/* ChapelTuple.chpl:1 */
void __init_ChapelTuple(void) {
  chpl_bool T1;
  T1 = (!__run_ChapelTuple_firsttime4);
  if (T1) {
    goto _end___init_ChapelTuple;
  }
  __run_ChapelTuple_firsttime4 = false;
  _end___init_ChapelTuple:;
  return;
}

/* ChapelTuple.chpl:52 */
_ref_range_int64_t_bounded_1 _this_548880(_ref__tuple_1_range_int64_t_bounded_1 _this_548884, int32_t i, int32_t _ln, _string _fn) {
  _ref_range_int64_t_bounded_1 T2 = NULL;
  chpl_bool T1;
  T1 = (i==1);
  if (T1) {
    T2 = (&((*_this_548884).x1));
    goto _end_this;
  }
  halt("tuple indexing out-of-bounds error", _ln, _fn);
  T2 = (&((*_this_548884).x1));
  _end_this:;
  return T2;
}

/* ChapelTuple.chpl:52 */
int64_t _this_458783(_ref__tuple_1_int64_t _this_458789, int32_t i, int32_t _ln, _string _fn) {
  int64_t T2;
  chpl_bool T1;
  T1 = (i==1);
  if (T1) {
    T2 = ((*_this_458789).x1);
    goto _end_this;
  }
  halt("tuple indexing out-of-bounds error", _ln, _fn);
  T2 = ((*_this_458789).x1);
  _end_this:;
  return T2;
}

/* ChapelTuple.chpl:52 */
_ref_int64_t _this_458681(_ref__tuple_1_int64_t _this_458685, int32_t i, int32_t _ln, _string _fn) {
  _ref_int64_t T2 = NULL;
  chpl_bool T1;
  T1 = (i==1);
  if (T1) {
    T2 = (&((*_this_458685).x1));
    goto _end_this;
  }
  halt("tuple indexing out-of-bounds error", _ln, _fn);
  T2 = (&((*_this_458685).x1));
  _end_this:;
  return T2;
}

/* ChapelTuple.chpl:52 */
range_int64_t_bounded_1 _this_548982(_ref__tuple_1_range_int64_t_bounded_1 _this_548988, int32_t i, int32_t _ln, _string _fn) {
  range_int64_t_bounded_1 T2;
  chpl_bool T1;
  T1 = (i==1);
  if (T1) {
    T2 = ((*_this_548988).x1);
    goto _end_this;
  }
  halt("tuple indexing out-of-bounds error", _ln, _fn);
  T2 = ((*_this_548988).x1);
  _end_this:;
  return T2;
}

/* ChapelTuple.chpl:52 */
int32_t _this_293204(_ref__tuple_1_int32_t _this_293210, int32_t i, int32_t _ln, _string _fn) {
  int32_t T2;
  chpl_bool T1;
  T1 = (i==1);
  if (T1) {
    T2 = ((*_this_293210).x1);
    goto _end_this;
  }
  halt("tuple indexing out-of-bounds error", _ln, _fn);
  T2 = ((*_this_293210).x1);
  _end_this:;
  return T2;
}

/* ChapelTuple.chpl:52 */
range_int32_t_bounded_0 this(_ref__tuple_1_range_int32_t_bounded_0 _this_288709, int32_t i, int32_t _ln, _string _fn) {
  range_int32_t_bounded_0 T2;
  chpl_bool T1;
  T1 = (i==1);
  if (T1) {
    T2 = ((*_this_288709).x1);
    goto _end_this;
  }
  halt("tuple indexing out-of-bounds error", _ln, _fn);
  T2 = ((*_this_288709).x1);
  _end_this:;
  return T2;
}

/* ChapelTuple.chpl:52 */
range_int64_t_bounded_0 _this_457915(_ref__tuple_1_range_int64_t_bounded_0 _this_457921, int32_t i, int32_t _ln, _string _fn) {
  range_int64_t_bounded_0 T2;
  chpl_bool T1;
  T1 = (i==1);
  if (T1) {
    T2 = ((*_this_457921).x1);
    goto _end_this;
  }
  halt("tuple indexing out-of-bounds error", _ln, _fn);
  T2 = ((*_this_457921).x1);
  _end_this:;
  return T2;
}

