/* ChapelIO.chpl:1 */
void __init_ChapelIO(int32_t _ln, _string _fn) {
  chpl_bool T1;
  _cfile T2;
  file T5 = NULL;
  file T3 = NULL;
  _syncvar_uint64_t T4 = NULL;
  _cfile T6;
  file T9 = NULL;
  file T7 = NULL;
  _syncvar_uint64_t T8 = NULL;
  _cfile T10;
  file T13 = NULL;
  file T11 = NULL;
  _syncvar_uint64_t T12 = NULL;
  T1 = (!__run_ChapelIO_firsttime3);
  if (T1) {
    goto _end___init_ChapelIO;
  }
  __run_ChapelIO_firsttime3 = false;
  T2 = get_stdin();
  T3 = (file)chpl_alloc(sizeof(_file), "instance of class _unknown", _ln, _fn);
  ((object)T3)->_cid = _e_file;
  T3->filename = "";
  T3->mode = FileAccessMode_read;
  T3->path = "";
  T3->_fp = 0;
  T3->filename = "stdin";
  T3->mode = FileAccessMode_read;
  T3->path = "/dev";
  T3->_fp = T2;
  T4 = _construct__syncvar(UINT64(0), _ln, _fn);
  T3->_lock = T4;
  T5 = _construct_file("stdin", FileAccessMode_read, "/dev", T2, T4, T3, _ln, _fn);
  _stdin_16853 = T5;
  T6 = get_stdout();
  T7 = (file)chpl_alloc(sizeof(_file), "instance of class _unknown", _ln, _fn);
  ((object)T7)->_cid = _e_file;
  T7->filename = "";
  T7->mode = FileAccessMode_read;
  T7->path = "";
  T7->_fp = 0;
  T7->filename = "stdout";
  T7->mode = FileAccessMode_write;
  T7->path = "/dev";
  T7->_fp = T6;
  T8 = _construct__syncvar(UINT64(0), _ln, _fn);
  T7->_lock = T8;
  T9 = _construct_file("stdout", FileAccessMode_write, "/dev", T6, T8, T7, _ln, _fn);
  _stdout_16871 = T9;
  T10 = get_stderr();
  T11 = (file)chpl_alloc(sizeof(_file), "instance of class _unknown", _ln, _fn);
  ((object)T11)->_cid = _e_file;
  T11->filename = "";
  T11->mode = FileAccessMode_read;
  T11->path = "";
  T11->_fp = 0;
  T11->filename = "stderr";
  T11->mode = FileAccessMode_write;
  T11->path = "/dev";
  T11->_fp = T10;
  T12 = _construct__syncvar(UINT64(0), _ln, _fn);
  T11->_lock = T12;
  T13 = _construct_file("stderr", FileAccessMode_write, "/dev", T10, T12, T11, _ln, _fn);
  _stderr_16889 = T13;
  _end___init_ChapelIO:;
  return;
}

/* ChapelIO.chpl:23 */
file _construct_file(_string _filename_59461, FileAccessMode _mode_59462, _string _path_59463, _cfile _fp, _syncvar_uint64_t _lock, file meme, int32_t _ln, _string _fn) {
  _syncvar_uint64_t T1 = NULL;
  file T3 = NULL;
  chpl_bool T2;
  Writer T4 = NULL;
  T1 = _pass(_lock);
  T2 = (nil==meme);
  if (T2) {
    T3 = (file)chpl_alloc(sizeof(_file), "instance of class file", _ln, _fn);
    ((object)T3)->_cid = _e_file;
  } else {
    T3 = meme;
  }
  T4 = (&(T3->super));
  _construct_Writer(T4, _ln, _fn);
  T3->filename = _filename_59461;
  T3->mode = _mode_59462;
  T3->path = _path_59463;
  T3->_fp = _fp;
  T3->_lock = T1;
  return T3;
}

/* ChapelIO.chpl:56 */
_string filename(file _this_652880) {
  _string T1;
  T1 = (_this_652880->filename);
  return T1;
}

/* ChapelIO.chpl:62 */
_string path(file _this_652754) {
  _string T1;
  T1 = (_this_652754->path);
  return T1;
}

/* ChapelIO.chpl:68 */
FileAccessMode mode(file _this_655403) {
  FileAccessMode T1;
  T1 = (_this_655403->mode);
  return T1;
}

/* ChapelIO.chpl:74 */
chpl_bool isOpen(file _this_17529) {
  chpl_bool T5;
  chpl_bool T1;
  _cfile T2;
  _cfile T3;
  chpl_bool T4;
  T1 = false;
  T2 = (_this_17529->_fp);
  T3 = get_nullfile();
  T4 = (T2!=T3);
  if (T4) {
    T1 = true;
  }
  T5 = T1;
  return T5;
}

/* ChapelIO.chpl:114 */
void _checkOpen(file f, chpl_bool isRead, int32_t _ln, _string _fn) {
  chpl_bool T1;
  chpl_bool T2;
  _string T3;
  _string T4;
  _string T5;
  _string T6;
  _string T7;
  T1 = isOpen(f);
  T2 = (!T1);
  if (T2) {
    T3 = path(f);
    T4 = string_concat(T3, "/", _ln, _fn);
    T5 = filename(f);
    T6 = string_concat(T4, T5, _ln, _fn);
    T7 = string_copy(T6, _ln, _fn);
    if (isRead) {
      _halt_654857("***Error: You must open \"", T7, "\" before trying to read from it***", _ln, _fn);
    } else {
      _halt_654857("***Error: You must open \"", T7, "\" before trying to write to it***", _ln, _fn);
    }
  }
  return;
}

/* ChapelIO.chpl:249 */
void writeThis(_string _this_18382, Writer f, int32_t _ln, _string _fn) {
  chpl_bool T1;
  file T2 = NULL;
  chpl_bool T3;
  StringClass T4 = NULL;
  T1 = (((object)f)->_cid == _e_file);
  if (T1) {
    T2 = ((file)(f));
    writeIt(T2, _this_18382, _ln, _fn);
  } else {
    T3 = (((object)f)->_cid == _e_StringClass);
    if (T3) {
      T4 = ((StringClass)(f));
      _writeIt_18503(T4, _this_18382, _ln, _fn);
    } else {
      _writeIt_18704(f, _this_18382);
    }
  }
  return;
}

/* ChapelIO.chpl:253 */
void writeIt(file _this_18407, _string s, int32_t _ln, _string _fn) {
  chpl_bool T1;
  chpl_bool T2;
  FileAccessMode T3;
  int32_t T4;
  int32_t T5;
  chpl_bool T6;
  _string T7;
  _string T8;
  _cfile T9;
  int32_t T10;
  chpl_bool T11;
  _string T12;
  T1 = isOpen(_this_18407);
  T2 = (!T1);
  if (T2) {
    _checkOpen(_this_18407, false, _ln, _fn);
  }
  T3 = mode(_this_18407);
  T4 = ((int32_t)(FileAccessMode_write));
  T5 = ((int32_t)(T3));
  T6 = (T5!=T4);
  if (T6) {
    T7 = path(_this_18407);
    T8 = filename(_this_18407);
    _halt_657615("***Error: ", T7, "/", T8, " not open for writing***", _ln, _fn);
  }
  T9 = (_this_18407->_fp);
  T10 = fprintf(T9, "%s", s);
  T11 = (T10<0);
  if (T11) {
    T12 = get_errno();
    _halt_654857("***Error: Write failed: ", T12, "***", _ln, _fn);
  }
  return;
}

/* ChapelIO.chpl:265 */
StringClass _construct_StringClass(_string s, StringClass meme, int32_t _ln, _string _fn) {
  StringClass T2 = NULL;
  chpl_bool T1;
  Writer T3 = NULL;
  T1 = (nil==meme);
  if (T1) {
    T2 = (StringClass)chpl_alloc(sizeof(_StringClass), "instance of class StringClass", _ln, _fn);
    ((object)T2)->_cid = _e_StringClass;
  } else {
    T2 = meme;
  }
  T3 = (&(T2->super));
  _construct_Writer(T3, _ln, _fn);
  T2->s = s;
  return T2;
}

/* ChapelIO.chpl:267 */
void _writeIt_18503(StringClass _this_18574, _string s, int32_t _ln, _string _fn) {
  _string T1;
  _string T2;
  _string T3;
  T1 = (_this_18574->s);
  T2 = string_concat(T1, s, _ln, _fn);
  T3 = string_copy(T2, _ln, _fn);
  _this_18574->s = T3;
  return;
}

/* ChapelIO.chpl:271 */
void _write_262811(_ref__string _this_262816, _string _e0_args, int32_t _ln, _string _fn) {
  _string T1;
  StringClass T3 = NULL;
  StringClass T2 = NULL;
  Writer T4 = NULL;
  _string T5;
  _string T6;
  T1 = *(_this_262816);
  T2 = (StringClass)chpl_alloc(sizeof(_StringClass), "instance of class _unknown", _ln, _fn);
  ((object)T2)->_cid = _e_StringClass;
  T2->s = "";
  T2->s = T1;
  T3 = _construct_StringClass(T1, T2, _ln, _fn);
  T4 = ((Writer)(T3));
  _write_262981(T4, _e0_args, _ln, _fn);
  T5 = (T3->s);
  T6 = string_copy(T5, _ln, _fn);
  (*_this_262816) = T6;
  return;
}

/* ChapelIO.chpl:271 */
void _write_602038(_ref__string _this_602043, int32_t _e0_args, int32_t _ln, _string _fn) {
  _string T1;
  StringClass T3 = NULL;
  StringClass T2 = NULL;
  Writer T4 = NULL;
  _string T5;
  _string T6;
  T1 = *(_this_602043);
  T2 = (StringClass)chpl_alloc(sizeof(_StringClass), "instance of class _unknown", _ln, _fn);
  ((object)T2)->_cid = _e_StringClass;
  T2->s = "";
  T2->s = T1;
  T3 = _construct_StringClass(T1, T2, _ln, _fn);
  T4 = ((Writer)(T3));
  _write_602097(T4, _e0_args, _ln, _fn);
  T5 = (T3->s);
  T6 = string_copy(T5, _ln, _fn);
  (*_this_602043) = T6;
  return;
}

/* ChapelIO.chpl:277 */
chpl_bool lockWrite(file _this_18623, int32_t _ln, _string _fn) {
  chpl_bool T7;
  uint64_t T1;
  chpl_bool T3;
  _syncvar_uint64_t T2 = NULL;
  uint64_t T5;
  _syncvar_uint64_t T4 = NULL;
  chpl_bool T6;
  _syncvar_uint64_t T9 = NULL;
  _syncvar_uint64_t T8 = NULL;
  T1 = chpl_thread_id();
  T2 = (_this_18623->_lock);
  T3 = isFull(T2);
  if (T3) {
    T4 = (_this_18623->_lock);
    T5 = readXX(T4);
    T6 = (T5==T1);
    if (T6) {
      T7 = false;
      goto _end_lockWrite;
    }
  }
  T8 = (_this_18623->_lock);
  T9 = __EQUAL__661104(T8, T1, _ln, _fn);
  _this_18623->_lock = T9;
  T7 = true;
  _end_lockWrite:;
  return T7;
}

/* ChapelIO.chpl:286 */
void unlockWrite(file _this_18681) {
  _syncvar_uint64_t T1 = NULL;
  T1 = (_this_18681->_lock);
  reset(T1);
  return;
}

/* ChapelIO.chpl:290 */
Writer _construct_Writer(Writer meme, int32_t _ln, _string _fn) {
  Writer T2 = NULL;
  chpl_bool T1;
  object T3 = NULL;
  T1 = (nil==meme);
  if (T1) {
    T2 = (Writer)chpl_alloc(sizeof(_Writer), "instance of class Writer", _ln, _fn);
    ((object)T2)->_cid = _e_Writer;
  } else {
    T2 = meme;
  }
  T3 = (&(T2->super));
  _construct_object(T3, _ln, _fn);
  return T2;
}

/* ChapelIO.chpl:291 */
void _writeIt_18704(Writer _this_18826, _string s) {
  return;
}

/* ChapelIO.chpl:292 */
chpl_bool _lockWrite_18710(Writer _this_18830) {
  chpl_bool T1;
  T1 = false;
  return T1;
}

/* ChapelIO.chpl:293 */
void _unlockWrite_18718(Writer _this_18834) {
  return;
}

/* ChapelIO.chpl:294 */
void _write_262981(Writer _this_262985, _string _e0_args, int32_t _ln, _string _fn) {
  chpl_bool T1;
  chpl_bool T3;
  file T2 = NULL;
  chpl_bool T4;
  file T5 = NULL;
  T1 = (((object)_this_262985)->_cid == _e_file);
  if (T1) {
    T2 = ((file)(_this_262985));
    T3 = lockWrite(T2, _ln, _fn);
  } else {
    T3 = _lockWrite_18710(_this_262985);
  }
  writeThis(_e0_args, _this_262985, _ln, _fn);
  if (T3) {
    T4 = (((object)_this_262985)->_cid == _e_file);
    if (T4) {
      T5 = ((file)(_this_262985));
      unlockWrite(T5);
    } else {
      _unlockWrite_18718(_this_262985);
    }
  }
  return;
}

/* ChapelIO.chpl:294 */
void _write_650174(Writer _this_650178, _string _e0_args, _string _e1_args, _string _e2_args, int32_t _ln, _string _fn) {
  chpl_bool T1;
  chpl_bool T3;
  file T2 = NULL;
  chpl_bool T4;
  file T5 = NULL;
  T1 = (((object)_this_650178)->_cid == _e_file);
  if (T1) {
    T2 = ((file)(_this_650178));
    T3 = lockWrite(T2, _ln, _fn);
  } else {
    T3 = _lockWrite_18710(_this_650178);
  }
  writeThis(_e0_args, _this_650178, _ln, _fn);
  writeThis(_e1_args, _this_650178, _ln, _fn);
  writeThis(_e2_args, _this_650178, _ln, _fn);
  if (T3) {
    T4 = (((object)_this_650178)->_cid == _e_file);
    if (T4) {
      T5 = ((file)(_this_650178));
      unlockWrite(T5);
    } else {
      _unlockWrite_18718(_this_650178);
    }
  }
  return;
}

/* ChapelIO.chpl:294 */
void _write_602097(Writer _this_602101, int32_t _e0_args, int32_t _ln, _string _fn) {
  chpl_bool T1;
  chpl_bool T3;
  file T2 = NULL;
  _string T4;
  chpl_bool T5;
  file T6 = NULL;
  T1 = (((object)_this_602101)->_cid == _e_file);
  if (T1) {
    T2 = ((file)(_this_602101));
    T3 = lockWrite(T2, _ln, _fn);
  } else {
    T3 = _lockWrite_18710(_this_602101);
  }
  T4 = int32_t_to_string(_e0_args);
  writeThis(T4, _this_602101, _ln, _fn);
  if (T3) {
    T5 = (((object)_this_602101)->_cid == _e_file);
    if (T5) {
      T6 = ((file)(_this_602101));
      unlockWrite(T6);
    } else {
      _unlockWrite_18718(_this_602101);
    }
  }
  return;
}

/* ChapelIO.chpl:294 */
void _write_532216(Writer _this_532220, _complex128* const _e0_args, int32_t _ln, _string _fn) {
  chpl_bool T1;
  chpl_bool T3;
  file T2 = NULL;
  _string T9;
  _real64 T4;
  chpl_bool T5;
  chpl_bool T6;
  _real64 T7;
  chpl_bool T8;
  _real64 T10;
  _string T11;
  _string T12;
  _string T15;
  _real64 T13;
  _string T14;
  _real64 T16;
  _real64 T17;
  chpl_bool T18;
  _real64 T19;
  _real64 T20;
  _string T21;
  _string T22;
  _string T23;
  int32_t T24;
  chpl_bool T25;
  _string T26;
  _string T27;
  _string T28;
  _string T29;
  _string T30;
  chpl_bool T31;
  file T32 = NULL;
  T1 = (((object)_this_532220)->_cid == _e_file);
  if (T1) {
    T2 = ((file)(_this_532220));
    T3 = lockWrite(T2, _ln, _fn);
  } else {
    T3 = _lockWrite_18710(_this_532220);
  }
  T4 = ((*_e0_args).re);
  T5 = isnan(T4);
  if (T5) {
    T6 = true;
  } else {
    T7 = ((*_e0_args).im);
    T8 = isnan(T7);
    T6 = T8;
  }
  if (T6) {
    T9 = "nan";
    goto _end__cast;
  }
  T10 = ((*_e0_args).re);
  T11 = _real64_to_string(T10);
  T12 = " + ";
  T13 = ((*_e0_args).im);
  T14 = _real64_to_string(T13);
  T15 = T14;
  T16 = ((*_e0_args).im);
  T17 = ((_real64)(0));
  T18 = (T16<T17);
  if (T18) {
    T19 = ((*_e0_args).im);
    T20 = (-T19);
    T21 = _real64_to_string(T20);
    T22 = string_copy(T21, _ln, _fn);
    T15 = T22;
    T23 = string_copy(" - ", _ln, _fn);
    T12 = T23;
  } else {
    T24 = _string_compare(T14, "-0.0");
    T25 = (T24==0);
    if (T25) {
      T26 = string_copy("0.0", _ln, _fn);
      T15 = T26;
      T27 = string_copy(" - ", _ln, _fn);
      T12 = T27;
    }
  }
  T28 = string_concat(T11, T12, _ln, _fn);
  T29 = string_concat(T28, T15, _ln, _fn);
  T30 = string_concat(T29, "i", _ln, _fn);
  T9 = T30;
  _end__cast:;
  writeThis(T9, _this_532220, _ln, _fn);
  if (T3) {
    T31 = (((object)_this_532220)->_cid == _e_file);
    if (T31) {
      T32 = ((file)(_this_532220));
      unlockWrite(T32);
    } else {
      _unlockWrite_18718(_this_532220);
    }
  }
  return;
}

/* ChapelIO.chpl:294 */
void _write_530783(Writer _this_530787, SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 _e0_args, int32_t _ln, _string _fn) {
  chpl_bool T1;
  chpl_bool T3;
  file T2 = NULL;
  chpl_bool T4;
  file T5 = NULL;
  T1 = (((object)_this_530787)->_cid == _e_file);
  if (T1) {
    T2 = ((file)(_this_530787));
    T3 = lockWrite(T2, _ln, _fn);
  } else {
    T3 = _lockWrite_18710(_this_530787);
  }
  _writeThis_531087(_e0_args, _this_530787, _ln, _fn);
  if (T3) {
    T4 = (((object)_this_530787)->_cid == _e_file);
    if (T4) {
      T5 = ((file)(_this_530787));
      unlockWrite(T5);
    } else {
      _unlockWrite_18718(_this_530787);
    }
  }
  return;
}

/* ChapelIO.chpl:294 */
void _write_530015(Writer _this_530019, _string _e0_args, _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0* const _e1_args, _string _e2_args, _string _e3_args, int32_t _ln, _string _fn) {
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T1;
  chpl_bool T2;
  chpl_bool T4;
  file T3 = NULL;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T5;
  chpl_bool T6;
  file T7 = NULL;
  T1 = (*_e1_args);
  T2 = (((object)_this_530019)->_cid == _e_file);
  if (T2) {
    T3 = ((file)(_this_530019));
    T4 = lockWrite(T3, _ln, _fn);
  } else {
    T4 = _lockWrite_18710(_this_530019);
  }
  writeThis(_e0_args, _this_530019, _ln, _fn);
  T5 = T1;
  _writeThis_530684(&(T5), _this_530019, _ln, _fn);
  writeThis(_e2_args, _this_530019, _ln, _fn);
  writeThis(_e3_args, _this_530019, _ln, _fn);
  if (T4) {
    T6 = (((object)_this_530019)->_cid == _e_file);
    if (T6) {
      T7 = ((file)(_this_530019));
      unlockWrite(T7);
    } else {
      _unlockWrite_18718(_this_530019);
    }
  }
  return;
}

/* ChapelIO.chpl:294 */
void _write_426281(Writer _this_426285, _string _e0_args, _real64 _e1_args, _string _e2_args, int32_t _ln, _string _fn) {
  chpl_bool T1;
  chpl_bool T3;
  file T2 = NULL;
  _string T4;
  chpl_bool T5;
  file T6 = NULL;
  T1 = (((object)_this_426285)->_cid == _e_file);
  if (T1) {
    T2 = ((file)(_this_426285));
    T3 = lockWrite(T2, _ln, _fn);
  } else {
    T3 = _lockWrite_18710(_this_426285);
  }
  writeThis(_e0_args, _this_426285, _ln, _fn);
  T4 = _real64_to_string(_e1_args);
  writeThis(T4, _this_426285, _ln, _fn);
  writeThis(_e2_args, _this_426285, _ln, _fn);
  if (T3) {
    T5 = (((object)_this_426285)->_cid == _e_file);
    if (T5) {
      T6 = ((file)(_this_426285));
      unlockWrite(T6);
    } else {
      _unlockWrite_18718(_this_426285);
    }
  }
  return;
}

/* ChapelIO.chpl:294 */
void _write_424892(Writer _this_424896, _string _e0_args, int64_t _e1_args, _string _e2_args, int32_t _ln, _string _fn) {
  chpl_bool T1;
  chpl_bool T3;
  file T2 = NULL;
  _string T4;
  chpl_bool T5;
  file T6 = NULL;
  T1 = (((object)_this_424896)->_cid == _e_file);
  if (T1) {
    T2 = ((file)(_this_424896));
    T3 = lockWrite(T2, _ln, _fn);
  } else {
    T3 = _lockWrite_18710(_this_424896);
  }
  writeThis(_e0_args, _this_424896, _ln, _fn);
  T4 = int64_t_to_string(_e1_args);
  writeThis(T4, _this_424896, _ln, _fn);
  writeThis(_e2_args, _this_424896, _ln, _fn);
  if (T3) {
    T5 = (((object)_this_424896)->_cid == _e_file);
    if (T5) {
      T6 = ((file)(_this_424896));
      unlockWrite(T6);
    } else {
      _unlockWrite_18718(_this_424896);
    }
  }
  return;
}

/* ChapelIO.chpl:294 */
void _write_421726(Writer _this_421730, _string _e0_args, int64_t _e1_args, int32_t _ln, _string _fn) {
  chpl_bool T1;
  chpl_bool T3;
  file T2 = NULL;
  _string T4;
  chpl_bool T5;
  file T6 = NULL;
  T1 = (((object)_this_421730)->_cid == _e_file);
  if (T1) {
    T2 = ((file)(_this_421730));
    T3 = lockWrite(T2, _ln, _fn);
  } else {
    T3 = _lockWrite_18710(_this_421730);
  }
  writeThis(_e0_args, _this_421730, _ln, _fn);
  T4 = int64_t_to_string(_e1_args);
  writeThis(T4, _this_421730, _ln, _fn);
  if (T3) {
    T5 = (((object)_this_421730)->_cid == _e_file);
    if (T5) {
      T6 = ((file)(_this_421730));
      unlockWrite(T6);
    } else {
      _unlockWrite_18718(_this_421730);
    }
  }
  return;
}

/* ChapelIO.chpl:302 */
void _writeln_426041(Writer _this_426045, _string _e0_args, _real64 _e1_args, int32_t _ln, _string _fn) {
  _write_426281(_this_426045, _e0_args, _e1_args, "\n", _ln, _fn);
  return;
}

/* ChapelIO.chpl:302 */
void _writeln_425500(Writer _this_425504, _string _e0_args, int64_t _e1_args, int32_t _ln, _string _fn) {
  _write_424892(_this_425504, _e0_args, _e1_args, "\n", _ln, _fn);
  return;
}

/* ChapelIO.chpl:302 */
void _writeln_649934(Writer _this_649938, _string _e0_args, _string _e1_args, int32_t _ln, _string _fn) {
  _write_650174(_this_649938, _e0_args, _e1_args, "\n", _ln, _fn);
  return;
}

/* ChapelIO.chpl:302 */
void _writeln_528933(Writer _this_528937, _string _e0_args, _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0* const _e1_args, _string _e2_args, int32_t _ln, _string _fn) {
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T1;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T2;
  T1 = (*_e1_args);
  T2 = T1;
  _write_530015(_this_528937, _e0_args, &(T2), _e2_args, "\n", _ln, _fn);
  return;
}

/* ChapelIO.chpl:305 */
void writeln(Writer _this_18846, int32_t _ln, _string _fn) {
  _write_262981(_this_18846, "\n", _ln, _fn);
  return;
}

/* ChapelIO.chpl:310 */
void _write_421278(_string _e0_args, int64_t _e1_args, int32_t _ln, _string _fn) {
  Writer T1 = NULL;
  T1 = ((Writer)(_stdout_16871));
  _write_421726(T1, _e0_args, _e1_args, _ln, _fn);
  return;
}

/* ChapelIO.chpl:310 */
void _write_424291(_string _e0_args, int64_t _e1_args, _string _e2_args, int32_t _ln, _string _fn) {
  Writer T1 = NULL;
  T1 = ((Writer)(_stdout_16871));
  _write_424892(T1, _e0_args, _e1_args, _e2_args, _ln, _fn);
  return;
}

/* ChapelIO.chpl:314 */
void _writeln_425807(_string _e0_args, _real64 _e1_args, int32_t _ln, _string _fn) {
  Writer T1 = NULL;
  T1 = ((Writer)(_stdout_16871));
  _writeln_426041(T1, _e0_args, _e1_args, _ln, _fn);
  return;
}

/* ChapelIO.chpl:314 */
void _writeln_425379(_string _e0_args, int64_t _e1_args, int32_t _ln, _string _fn) {
  Writer T1 = NULL;
  T1 = ((Writer)(_stdout_16871));
  _writeln_425500(T1, _e0_args, _e1_args, _ln, _fn);
  return;
}

/* ChapelIO.chpl:314 */
void _writeln_528639(_string _e0_args, _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0* const _e1_args, _string _e2_args, int32_t _ln, _string _fn) {
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T1;
  _array_int64_t__complex128_1_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0 T2;
  Writer T3 = NULL;
  T1 = (*_e1_args);
  T2 = T1;
  T3 = ((Writer)(_stdout_16871));
  _writeln_528933(T3, _e0_args, &(T2), _e2_args, _ln, _fn);
  return;
}

/* ChapelIO.chpl:314 */
void _writeln_649732(_string _e0_args, _string _e1_args, int32_t _ln, _string _fn) {
  Writer T1 = NULL;
  T1 = ((Writer)(_stdout_16871));
  _writeln_649934(T1, _e0_args, _e1_args, _ln, _fn);
  return;
}

/* ChapelIO.chpl:318 */
void _writeln_18890(int32_t _ln, _string _fn) {
  Writer T1 = NULL;
  T1 = ((Writer)(_stdout_16871));
  writeln(T1, _ln, _fn);
  return;
}

/* ChapelIO.chpl:362 */
_string __tuple2string_658104(_tuple_5__string__string__string__string__string* const t, int32_t _ln, _string _fn) {
  _string T1;
  _string T2;
  _string T3;
  _string T4;
  _string T5;
  _string T12;
  _string T6;
  _ref__string T7 = NULL;
  _ref__string T8 = NULL;
  _ref__string T9 = NULL;
  _ref__string T10 = NULL;
  _ref__string T11 = NULL;
  T1 = ((*t).x1);
  T2 = ((*t).x2);
  T3 = ((*t).x3);
  T4 = ((*t).x4);
  T5 = ((*t).x5);
  T6 = "";
  T7 = &(T6);
  _write_262811(T7, T1, _ln, _fn);
  T8 = &(T6);
  _write_262811(T8, T2, _ln, _fn);
  T9 = &(T6);
  _write_262811(T9, T3, _ln, _fn);
  T10 = &(T6);
  _write_262811(T10, T4, _ln, _fn);
  T11 = &(T6);
  _write_262811(T11, T5, _ln, _fn);
  T12 = T6;
  return T12;
}

/* ChapelIO.chpl:362 */
_string __tuple2string_654936(_tuple_3__string__string__string* const t, int32_t _ln, _string _fn) {
  _string T1;
  _string T2;
  _string T3;
  _string T8;
  _string T4;
  _ref__string T5 = NULL;
  _ref__string T6 = NULL;
  _ref__string T7 = NULL;
  T1 = ((*t).x1);
  T2 = ((*t).x2);
  T3 = ((*t).x3);
  T4 = "";
  T5 = &(T4);
  _write_262811(T5, T1, _ln, _fn);
  T6 = &(T4);
  _write_262811(T6, T2, _ln, _fn);
  T7 = &(T4);
  _write_262811(T7, T3, _ln, _fn);
  T8 = T4;
  return T8;
}

/* ChapelIO.chpl:362 */
_string __tuple2string_601585(_tuple_3__string_int32_t__string* const t, int32_t _ln, _string _fn) {
  _string T1;
  int32_t T2;
  _string T3;
  _string T8;
  _string T4;
  _ref__string T5 = NULL;
  _ref__string T6 = NULL;
  _ref__string T7 = NULL;
  T1 = ((*t).x1);
  T2 = ((*t).x2);
  T3 = ((*t).x3);
  T4 = "";
  T5 = &(T4);
  _write_262811(T5, T1, _ln, _fn);
  T6 = &(T4);
  _write_602038(T6, T2, _ln, _fn);
  T7 = &(T4);
  _write_262811(T7, T3, _ln, _fn);
  T8 = T4;
  return T8;
}

/* ChapelIO.chpl:362 */
_string _tuple2string(_tuple_1__string* const t, int32_t _ln, _string _fn) {
  _string T1;
  _string T4;
  _string T2;
  _ref__string T3 = NULL;
  T1 = ((*t).x1);
  T2 = "";
  T3 = &(T2);
  _write_262811(T3, T1, _ln, _fn);
  T4 = T2;
  return T4;
}

/* ChapelIO.chpl:383 */
void _halt_654857(_string _e0_args, _string _e1_args, _string _e2_args, int32_t _ln, _string _fn) {
  _tuple_3__string__string__string T1;
  _string T2;
  _string T3;
  T1.x1 = _e0_args;
  T1.x2 = _e1_args;
  T1.x3 = _e2_args;
  T2 = __tuple2string_654936(&(T1), _ln, _fn);
  T3 = string_concat("halt reached - ", T2, _ln, _fn);
  chpl_error(T3, _ln, _fn);
  return;
}

/* ChapelIO.chpl:383 */
void _halt_657615(_string _e0_args, _string _e1_args, _string _e2_args, _string _e3_args, _string _e4_args, int32_t _ln, _string _fn) {
  _tuple_5__string__string__string__string__string T1;
  _string T2;
  _string T3;
  T1.x1 = _e0_args;
  T1.x2 = _e1_args;
  T1.x3 = _e2_args;
  T1.x4 = _e3_args;
  T1.x5 = _e4_args;
  T2 = __tuple2string_658104(&(T1), _ln, _fn);
  T3 = string_concat("halt reached - ", T2, _ln, _fn);
  chpl_error(T3, _ln, _fn);
  return;
}

/* ChapelIO.chpl:383 */
void halt(_string _e0_args, int32_t _ln, _string _fn) {
  _tuple_1__string T1;
  _string T2;
  _string T3;
  T1.x1 = _e0_args;
  T2 = _tuple2string(&(T1), _ln, _fn);
  T3 = string_concat("halt reached - ", T2, _ln, _fn);
  chpl_error(T3, _ln, _fn);
  return;
}

/* ChapelIO.chpl:383 */
void _halt_601364(_string _e0_args, int32_t _e1_args, _string _e2_args, int32_t _ln, _string _fn) {
  _tuple_3__string_int32_t__string T1;
  _string T2;
  _string T3;
  T1.x1 = _e0_args;
  T1.x2 = _e1_args;
  T1.x3 = _e2_args;
  T2 = __tuple2string_601585(&(T1), _ln, _fn);
  T3 = string_concat("halt reached - ", T2, _ln, _fn);
  chpl_error(T3, _ln, _fn);
  return;
}

