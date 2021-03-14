use SysBasic;

extern type glob_t;
extern type wordexp_t;

extern proc chpl_study_glob(pattern:c_string, flags:c_int, ref ret_glob:glob_t):c_int;
extern proc chpl_wordexp(pattern:c_string, flags:c_int, ref ret_glob:wordexp_t):c_int;
extern proc chpl_isdir(path:c_string):c_int;
extern proc glob_num(x:glob_t): size_t;
extern proc glob_index(x:glob_t, idx:size_t): c_string;
extern proc wordexp_num(x:wordexp_t): size_t;
extern proc wordexp_index(x:wordexp_t, idx:size_t): c_string;

iter glob(pattern:string, flags:int, expand:bool = false, recursive:bool = false, extension:string = ""):string {
    var err: c_int;
    var tx:c_string;
    if expand { // use wordexp
        var glb:wordexp_t;
        err = chpl_wordexp((extension + pattern).c_str(), flags:c_int, glb);
        for i in 0..wordexp_num(glb) -1 {
            tx = wordexp_index(glb, i);
            if recursive {
                if chpl_isdir(tx) == 1 {
                    const pth = tx:string + "/";
                    for fl in glob(pattern, flags, expand, recursive, pth) do
                        yield fl;
                } else yield tx:string;
            } else yield tx:string;
        }
    } else { // else, use glob
        var glb:glob_t;
        err = chpl_study_glob((extension + pattern).c_str(), flags:c_int, glb);
        for i in 0..glob_num(glb) - 1 {
            tx = glob_index(glb, i);
            if recursive {
                if chpl_isdir(tx) == 1 {
                    const pth = tx:string + "/";
                    for fl in glob(pattern, flags, expand, recursive, pth) do
                        yield fl;
                } else yield tx:string;
            } else yield tx:string;
        }
    }
}

// TODO: make this a L/F on directories when we we want to recurse
iter glob(param tag:iterKind, pattern:string, flags:int, expand:bool = false, recursive:bool = false, extension:string = "") : string
where tag == iterKind.leader {
  var err: c_int;
  var tx:c_string;
  var dirBuff: domain(string);
  if expand { // use wordexp 
    var glb:wordexp_t;
    err = chpl_wordexp(pattern.c_str(), flags:c_int, glb);
    // Make this spawn a task if we encounter a dir, else yield in parallel
    for i in 0..wordexp_num(glb) - 1 {
      tx = wordexp_index(glb, i);
      if (recursive && chpl_isdir(tx) == 1) {
        dirBuff += tx;
      } else yield tx:string;
    }
  } else { // else, use glob
    var glb:glob_t;
    // Spawn a task if we encounter a dir, else yield in parallel
    err = chpl_study_glob(pattern.c_str(), flags:c_int, glb);
    for i in 0..glob_num(glb) - 1 {
      tx = glob_index(glb, i);
      if recursive && chpl_isdir(tx) == 1 {
        dirBuff += tx;
      } else yield tx:string;
    }
  }
  // Now spawn off tasks for each dir
  coforall dir in dirBuff {
    const pth = dir + "/";
    forall fl in glob(pattern, flags, expand, recursive, pth) {
      yield fl;
    }
  }
}

// ....
iter glob(param tag:iterKind, pattern:string, flags:int, expand:bool = false, recursive:bool = false, extension:string = "", followThis) : string
where tag == iterKind.follower {
    yield followThis;
}


for s in zip(glob("[a-z]*", 0, true, true), glob("[a-z]*", 0, false, true)) do
writeln(s);

writeln("----------------------------------");

forall s in glob("[a-z]*", 0, true, true) do
writeln(s);

