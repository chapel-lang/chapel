/* Random.chpl:1 */
void __init_Random(int32_t _ln, _string _fn) {
  chpl_bool T1;
  SeedGeneratorClass T3 = NULL;
  SeedGeneratorClass T2 = NULL;
  T1 = (!__run_Random_firsttime18);
  if (T1) {
    goto _end___init_Random;
  }
  __run_Random_firsttime18 = false;
  if (__run_Time_firsttime17) {
    __init_Time();
  }
  __run_Time_firsttime17 = false;
  T2 = (SeedGeneratorClass)chpl_alloc(sizeof(_SeedGeneratorClass), "instance of class _unknown", _ln, _fn);
  ((object)T2)->_cid = _e_SeedGeneratorClass;
  T3 = _construct_SeedGeneratorClass(T2, _ln, _fn);
  SeedGenerator = T3;
  _end___init_Random:;
  return;
}

/* Random.chpl:15 */
SeedGeneratorClass _construct_SeedGeneratorClass(SeedGeneratorClass meme, int32_t _ln, _string _fn) {
  SeedGeneratorClass T2 = NULL;
  chpl_bool T1;
  object T3 = NULL;
  T1 = (nil==meme);
  if (T1) {
    T2 = (SeedGeneratorClass)chpl_alloc(sizeof(_SeedGeneratorClass), "instance of class SeedGeneratorClass", _ln, _fn);
    ((object)T2)->_cid = _e_SeedGeneratorClass;
  } else {
    T2 = meme;
  }
  T3 = (&(T2->super));
  _construct_object(T3, _ln, _fn);
  return T2;
}

/* Random.chpl:16 */
int64_t clockMS(SeedGeneratorClass _this_49867, int32_t _ln, _string _fn) {
  int64_t T3;
  _real64 T1;
  int64_t T2;
  T1 = getCurrentTime(TimeUnits_microseconds, _ln, _fn);
  T2 = ((int64_t)(T1));
  T3 = T2;
  return T3;
}

/* Random.chpl:23 */
RandomStream _construct_RandomStream(int64_t _seed_64118, _real64 arand, _real64 r23, _real64 t23, _real64 r46, _real64 t46, int64_t internalSeed, _real64 cursorVal, RandomStream meme, int32_t _ln, _string _fn) {
  RandomStream T2 = NULL;
  chpl_bool T1;
  object T3 = NULL;
  T1 = (nil==meme);
  if (T1) {
    T2 = (RandomStream)chpl_alloc(sizeof(_RandomStream), "instance of class RandomStream", _ln, _fn);
    ((object)T2)->_cid = _e_RandomStream;
  } else {
    T2 = meme;
  }
  T3 = (&(T2->super));
  _construct_object(T3, _ln, _fn);
  T2->seed = _seed_64118;
  T2->arand = arand;
  T2->r23 = r23;
  T2->t23 = t23;
  T2->r46 = r46;
  T2->t46 = t46;
  T2->internalSeed = internalSeed;
  T2->cursorVal = cursorVal;
  initialize(T2, _ln, _fn);
  return T2;
}

/* Random.chpl:36 */
void initialize(RandomStream _this_50547, int32_t _ln, _string _fn) {
  int64_t T1;
  int64_t T2;
  int64_t T3;
  int64_t T4;
  int64_t T5;
  chpl_bool T6;
  int64_t T7;
  int64_t T8;
  int64_t T9;
  T1 = (_this_50547->seed);
  _this_50547->internalSeed = T1;
  T2 = (_this_50547->internalSeed);
  T3 = ((int64_t)(2));
  T4 = (T2%T3);
  T5 = ((int64_t)(0));
  T6 = (T4==T5);
  if (T6) {
    T7 = (_this_50547->internalSeed);
    T8 = ((int64_t)(1));
    T9 = (T7+T8);
    _this_50547->internalSeed = T9;
  }
  initCursorVal(_this_50547, _ln, _fn);
  return;
}

/* Random.chpl:43 */
void initCursorVal(RandomStream _this_50551, int32_t _ln, _string _fn) {
  int64_t T1;
  _real64 T2;
  T1 = (_this_50551->internalSeed);
  T2 = ((_real64)(T1));
  _this_50551->cursorVal = T2;
  return;
}

/* Random.chpl:47 */
_real64 randlc(RandomStream _this_527658, _ref__real64 x, _ref__real64 a) {
  _real64 T1;
  _real64 T37;
  _real64 T2;
  _real64 T3;
  _real64 T4;
  _real64 T5;
  _real64 T6;
  _real64 T7;
  _real64 T8;
  _real64 T9;
  _real64 T10;
  _real64 T11;
  _real64 T12;
  _real64 T13;
  _real64 T14;
  _real64 T15;
  _real64 T16;
  _real64 T17;
  _real64 T18;
  _real64 T19;
  _real64 T20;
  _real64 T21;
  _real64 T22;
  _real64 T23;
  _real64 T24;
  _real64 T25;
  _real64 T26;
  _real64 T27;
  _real64 T28;
  _real64 T29;
  _real64 T30;
  _real64 T31;
  _real64 T32;
  _real64 T33;
  _real64 T34;
  _real64 T35;
  _real64 T36;
  T1 = *(x);
  T2 = (_this_527658->r23);
  T3 = *(a);
  T4 = (T2*T3);
  T5 = floor(T4);
  T6 = (_this_527658->t23);
  T7 = (T6*T5);
  T8 = *(a);
  T9 = (T8-T7);
  T10 = (_this_527658->r23);
  T11 = (T10*T1);
  T12 = floor(T11);
  T13 = (_this_527658->t23);
  T14 = (T13*T12);
  T15 = (T1-T14);
  T16 = (T5*T15);
  T17 = (T9*T12);
  T18 = (T16+T17);
  T19 = (_this_527658->r23);
  T20 = (T19*T18);
  T21 = floor(T20);
  T22 = (_this_527658->t23);
  T23 = (T22*T21);
  T24 = (T18-T23);
  T25 = (_this_527658->t23);
  T26 = (T25*T24);
  T27 = (T9*T15);
  T28 = (T26+T27);
  T29 = (_this_527658->r46);
  T30 = (T29*T28);
  T31 = floor(T30);
  T32 = (_this_527658->t46);
  T33 = (T32*T31);
  T34 = (T28-T33);
  T35 = (_this_527658->r46);
  T36 = (T35*T34);
  T37 = T36;
  (*x) = T34;
  return T37;
}

/* Random.chpl:65 */
_real64 getNext(RandomStream _this_50559) {
  _real64 T8;
  _real64 T2;
  _real64 T1;
  _real64 T7;
  _ref__real64 T3 = NULL;
  _ref__real64 T6 = NULL;
  _real64 T5;
  _real64 T4;
  T1 = (_this_50559->cursorVal);
  T2 = T1;
  T3 = &(T2);
  T4 = (_this_50559->arand);
  T5 = T4;
  T6 = &(T5);
  T7 = randlc(_this_50559, T3, T6);
  _this_50559->cursorVal = T2;
  T8 = T7;
  return T8;
}

/* Random.chpl:100 */
void _fillRandom_527479(RandomStream _this_527483, _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0* const x, int32_t _ln, _string _fn) {
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T2;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T1;
  _domain_1_SingleLocaleArithmeticDomain_1_int64_t_0 T4;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T3;
  SingleLocaleArithmeticDomain_1_int64_t_0 T5 = NULL;
  range_int64_t_bounded_0 T7;
  _tuple_1_range_int64_t_bounded_0 T6;
  range_int64_t_bounded_0 T8;
  int64_t T9;
  int64_t T11;
  int64_t T10;
  int64_t T13;
  int64_t T12;
  _ref_int64_t T14 = NULL;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T15 = NULL;
  int64_t T16;
  int64_t T17;
  int64_t T18;
  int64_t T19;
  int32_t T20;
  _ddata__complex128 T21 = NULL;
  _ref__complex128 T22 = NULL;
  _real64 T23;
  SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T24 = NULL;
  int64_t T25;
  int64_t T26;
  int64_t T27;
  int64_t T28;
  int32_t T29;
  _ddata__complex128 T30 = NULL;
  _ref__complex128 T31 = NULL;
  _real64 T32;
  T1 = (*x);
  T2 = T1;
  T3 = T1;
  T4 = __dom_470951(&(T3), _ln, _fn);
  T5 = (T4._value);
  T6 = (T5->ranges);
  T7 = (T6.x1);
  T8 = __copy_429953(&(T7));
  T9 = 0;
  T10 = (T8._low);
  T11 = T10;
  T12 = (T8._high);
  T13 = T12;
  for (T9 = T11; T9 <= T13; T9 += 1) {
    T14 = &(T9);
    T15 = (T2._value);
    T16 = (T15->origin);
    T17 = (T16+T9);
    T18 = (T15->factoredOffs);
    T19 = (T17-T18);
    T20 = ((int32_t)(T19));
    T21 = (T15->data);
    T22 = _ARRAY_GET(T21, T20);
    T23 = getNext(_this_527483);
    (*T22).re = T23;
    T24 = (T2._value);
    T25 = (T24->origin);
    T26 = (T25+T9);
    T27 = (T24->factoredOffs);
    T28 = (T26-T27);
    T29 = ((int32_t)(T28));
    T30 = (T24->data);
    T31 = _ARRAY_GET(T30, T29);
    T32 = getNext(_this_527483);
    (*T31).im = T32;
  }
  return;
}

/* Random.chpl:124 */
void fillRandom(_array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0* const x, int64_t initseed, int32_t _ln, _string _fn) {
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T2;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T1;
  RandomStream T12 = NULL;
  RandomStream T3 = NULL;
  _real64 T4;
  _real64 T5;
  _real64 T6;
  _real64 T7;
  _real64 T8;
  _real64 T9;
  _real64 T10;
  _real64 T11;
  T1 = (*x);
  T2 = T1;
  T3 = (RandomStream)chpl_alloc(sizeof(_RandomStream), "instance of class _unknown", _ln, _fn);
  ((object)T3)->_cid = _e_RandomStream;
  T3->seed = 0;
  T3->arand = 0.0;
  T3->r23 = 0.0;
  T3->t23 = 0.0;
  T3->r46 = 0.0;
  T3->t46 = 0.0;
  T3->internalSeed = 0;
  T3->cursorVal = 0.0;
  T3->seed = initseed;
  T3->arand = 1220703125.0;
  T4 = ((_real64)(23));
  T5 = pow(0.5, T4);
  T3->r23 = T5;
  T6 = ((_real64)(23));
  T7 = pow(2.0, T6);
  T3->t23 = T7;
  T8 = ((_real64)(46));
  T9 = pow(0.5, T8);
  T3->r46 = T9;
  T10 = ((_real64)(46));
  T11 = pow(2.0, T10);
  T3->t46 = T11;
  T3->internalSeed = 0;
  T3->cursorVal = 0.0;
  T12 = _construct_RandomStream(initseed, 1220703125.0, T5, T7, T9, T11, 0, 0.0, T3, _ln, _fn);
  _fillRandom_527479(T12, &(T2), _ln, _fn);
  return;
}

