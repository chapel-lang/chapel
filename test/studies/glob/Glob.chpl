use SysBasic, SysCTypes;

/*********** Extern declarations **********/
extern type glob_t;
extern type wordexp_t;

extern proc chpl_study_glob(pattern:c_string, flags:c_int, ref ret_glob:glob_t):c_int;
extern proc chpl_wordexp(pattern:c_string, flags:c_int, ref ret_glob:wordexp_t):c_int;

extern proc glob_num(x:glob_t): size_t;
extern proc glob_index(x:glob_t, idx:size_t): c_string;

extern proc wordexp_num(x:wordexp_t): size_t;
extern proc wordexp_index(x:wordexp_t, idx:size_t): c_string;

extern proc chpl_isdir(path:c_string):c_int;
extern proc globfree(ref glb:glob_t);
extern proc wordfree(ref glb:wordexp_t);

// For glob
extern const GLOB_ERR      : c_int;
extern const GLOB_MARK     : c_int;
extern const GLOB_NOSORT   : c_int;
extern const GLOB_DOOFFS   : c_int;
extern const GLOB_NOCHECK  : c_int;
extern const GLOB_APPEND   : c_int;
extern const GLOB_NOESCAPE : c_int;

// For wordexp
extern const WRDE_APPEND : c_int;
extern const WRDE_DOOFFS : c_int;
extern const WRDE_NOCMD  : c_int;
extern const WRDE_REUSE  : c_int;

iter my_wordexp(pattern:string, recursive:bool = false, flags:int = 0, directory:string = ""):string {
  var err : c_int;
  var tx  : c_string;
  var glb : wordexp_t;

  err = chpl_wordexp((directory + pattern).c_str(), flags:c_int, glb);

  const wordexpNum = wordexp_num(glb);

  if wordexpNum then
  for i in 0..wordexpNum -1 {
    tx = wordexp_index(glb, i);
    if recursive && chpl_isdir(tx) == 1 {
      const pth = createStringWithNewBuffer(tx)+ "/";
      for fl in my_wordexp(pattern, recursive, flags, pth) do
        yield fl;
    } else yield createStringWithNewBuffer(tx);
  }

  wordfree(glb);
}

iter my_wordexp(param tag:iterKind, pattern:string, recursive:bool = false,
             flags:int = 0, directory:string = "") : string where tag == iterKind.leader {
  var err     : c_int;
  var tx      : c_string;
  var dirBuff : domain(string);

  dirBuff += directory;

  while (true) {

    // No more work left to accomplish
    if (dirBuff.size == 0) then
      break;

    //
    // make a copy of the directory buffer to avoid modifying a
    // collection while iterating over it.
    //
    const dirBuffCopy = dirBuff;
    dirBuff.clear();

    // Now spawn off tasks for each dir
    coforall dir in dirBuffCopy with (ref dirBuff) {
      for flConst in my_wordexp(pattern, false, flags, dir) {
        var fl = flConst;
        if recursive && chpl_isdir(fl.c_str()) == 1 {
          fl += "/";
          dirBuff += fl;
        }
        yield fl;
      }
    }
  }
}

iter my_wordexp(param tag:iterKind, pattern:string, recursive:bool = false,
    flags:int = 0, directory:string = "", followThis) : string where tag == iterKind.follower {
  yield followThis;
}

iter my_glob(pattern:string, recursive:bool = false, flags:int = 0, directory:string = ""):string {
  var err : c_int;
  var tx  : c_string;
  var glb : glob_t;

  err = chpl_study_glob((directory + pattern).c_str(), flags:c_int, glb);

  const globNum = glob_num(glb);

  if globNum then
  for i in 0..globNum - 1 {
    tx = glob_index(glb, i);
    if recursive && chpl_isdir(tx) == 1 {
      const pth = createStringWithNewBuffer(tx)+ "/";
      for fl in my_glob(pattern, recursive, flags, pth) do
        yield fl;
    } else yield createStringWithNewBuffer(tx);
  }

  globfree(glb);
}

iter my_glob(param tag:iterKind, pattern:string, recursive:bool = false,
          flags:int = 0, directory:string = "") : string where tag == iterKind.leader {
  var err     : c_int;
  var tx      : c_string;
  var dirBuff : domain(string);

  // We start out with the current directory
  dirBuff += directory;

  while (true) {

    // No more work left to accomplish
    if (dirBuff.size == 0) then
      break;

    //
    // make a copy of the directory buffer to avoid modifying a
    // collection while iterating over it.
    //
    const dirBuffCopy = dirBuff;
    dirBuff.clear();

    // Now spawn off tasks for each dir
    coforall dir in dirBuffCopy with (ref dirBuff) {
      for flConst in my_glob(pattern, false, flags, dir) {
        var fl = flConst;
        if recursive && chpl_isdir(fl.c_str()) == 1 {
          fl += "/";
          dirBuff += fl;
        }
        yield fl;
      }
    }
  }
}

iter my_glob(param tag:iterKind, pattern:string, recursive:bool = false,
    flags:int = 0, directory:string = "", followThis) : string where tag == iterKind.follower {
  yield followThis;
}
