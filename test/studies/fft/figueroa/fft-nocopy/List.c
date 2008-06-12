/* List.chpl:1 */
listNode_BaseArray _construct_listNode(BaseArray data, listNode_BaseArray next, listNode_BaseArray meme, int32_t _ln, _string _fn) {
  listNode_BaseArray T2 = NULL;
  chpl_bool T1;
  object T3 = NULL;
  T1 = (nil==meme);
  if (T1) {
    T2 = (listNode_BaseArray)chpl_alloc(sizeof(_listNode_BaseArray), "instance of class listNode", _ln, _fn);
    ((object)T2)->_cid = _e_listNode_BaseArray;
  } else {
    T2 = meme;
  }
  T3 = (&(T2->super));
  _construct_object(T3, _ln, _fn);
  T2->data = data;
  T2->next = next;
  return T2;
}

/* List.chpl:1 */
void __init_List(void) {
  chpl_bool T1;
  T1 = (!__run_List_firsttime16);
  if (T1) {
    goto _end___init_List;
  }
  __run_List_firsttime16 = false;
  _end___init_List:;
  return;
}

/* List.chpl:7 */
list_BaseArray __copy_225794(list_BaseArray* const x) {
  list_BaseArray T5;
  listNode_BaseArray T1 = NULL;
  listNode_BaseArray T2 = NULL;
  int32_t T3;
  list_BaseArray T4;
  T1 = ((*x).first);
  T2 = ((*x).last);
  T3 = ((*x).length);
  T4 = _construct_list(T1, T2, T3);
  T5 = T4;
  return T5;
}

/* List.chpl:7 */
list_BaseArray _construct_list(listNode_BaseArray first, listNode_BaseArray last, int32_t _length_225277) {
  list_BaseArray T1;
  T1.first = first;
  T1.last = last;
  T1.length = _length_225277;
  return T1;
}

/* List.chpl:21 */
void append(_ref_list_BaseArray _this_298165, BaseArray e, int32_t _ln, _string _fn) {
  listNode_BaseArray T1 = NULL;
  object T2 = NULL;
  chpl_bool T3;
  listNode_BaseArray T4 = NULL;
  listNode_BaseArray T7 = NULL;
  listNode_BaseArray T5 = NULL;
  listNode_BaseArray T6 = NULL;
  listNode_BaseArray T8 = NULL;
  listNode_BaseArray T9 = NULL;
  listNode_BaseArray T12 = NULL;
  listNode_BaseArray T10 = NULL;
  listNode_BaseArray T11 = NULL;
  listNode_BaseArray T13 = NULL;
  int32_t T14;
  int32_t T15;
  T1 = ((*_this_298165).last);
  T2 = ((object)(T1));
  T3 = (T2!=nil);
  if (T3) {
    T4 = ((*_this_298165).last);
    T5 = (listNode_BaseArray)chpl_alloc(sizeof(_listNode_BaseArray), "instance of class _unknown", _ln, _fn);
    ((object)T5)->_cid = _e_listNode_BaseArray;
    T5->data = nil;
    T5->next = nil;
    T5->data = e;
    T6 = ((listNode_BaseArray)(nil));
    T5->next = T6;
    T7 = _construct_listNode(e, T6, T5, _ln, _fn);
    T4->next = T7;
    T8 = ((*_this_298165).last);
    T9 = (T8->next);
    (*_this_298165).last = T9;
  } else {
    T10 = (listNode_BaseArray)chpl_alloc(sizeof(_listNode_BaseArray), "instance of class _unknown", _ln, _fn);
    ((object)T10)->_cid = _e_listNode_BaseArray;
    T10->data = nil;
    T10->next = nil;
    T10->data = e;
    T11 = ((listNode_BaseArray)(nil));
    T10->next = T11;
    T12 = _construct_listNode(e, T11, T10, _ln, _fn);
    (*_this_298165).first = T12;
    T13 = ((*_this_298165).first);
    (*_this_298165).last = T13;
  }
  T14 = ((*_this_298165).length);
  T15 = (T14+1);
  (*_this_298165).length = T15;
  return;
}

