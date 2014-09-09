config const debug = false;

use Sort;

iter listdir(path: string, dotfiles=false, dirs=true, files=false): string {
  extern type DIRptr;
  extern type direntptr;
  extern proc opendir(name: c_string): DIRptr;
  extern proc readdir(dirp: DIRptr): direntptr;
  extern proc closedir(dirp: DIRptr): c_int;
  extern proc chpl_rt_isDir(pathname: c_string): c_int;

  proc direntptr.d_name(): c_string {
    extern proc chpl_rt_direntptr_getname(d: direntptr): c_string;

    return chpl_rt_direntptr_getname(this);
  }
  // End inlining

  var dir: DIRptr;
  var ent: direntptr;
  //  writeln("***Trying ", path);
  dir = opendir(path:c_string);
  if (!is_c_nil(dir)) {
    ent = readdir(dir);
    while (!is_c_nil(ent)) {
      const filename = ent.d_name();
      if (dotfiles || filename.substring(1) != '.') {
        if (filename != "." && filename != "..") {
          const fullpath = path + "/" + filename;
          if (chpl_rt_isDir(fullpath:c_string)) {
            if (dirs) then
              yield filename;
          } else {
            if (files) then
              yield filename;
          }
        }
      }
      ent = readdir(dir);
    }
    closedir(dir);
  } else {
    extern proc perror(s: c_string);
    perror("error in listdir(): ");
  }
}


iter walkdirs(path: string=".", topdown=true, depth=max(int), dotfiles=false, followlinks=false, sort = false): string {
  //  writeln("topdown = ", topdown);
  //  writeln("walkdirs() called with ", path, " depth=",depth);

  if (topdown) then
    yield path;

  if (depth) {
    var subdirs = listdir(path, dotfiles=dotfiles, dirs=true);
    if (sort) then
      QuickSort(subdirs);
    for subdir in subdirs {
      const fullpath = path + "/" + subdir;
      //      yield fullpath;
      //      writeln("Calling walkdirs with ", fullpath);
      for subdir in walkdirs(fullpath, topdown, depth-1, dotfiles, followlinks) do
        yield subdir;
    }
  }

  if (!topdown) then
    yield path;
}

iter wordexp(pattern="*") {
  extern type wordexp_t;
  //  extern proc chpl_wordexp(pattern:c_string, flags:c_int, ref ret_glob:wordexp_t):c_int;
  extern proc wordexp(pattern:c_string, ref ret_glob: wordexp_t, flags_c_int): c_int;
  extern proc chpl_wordexp_num(x:wordexp_t): size_t;
  extern proc chpl_wordexp_index(x:wordexp_t, idx:size_t): c_string;
  extern proc wordfree(ref glb:wordexp_t);

  var glb : wordexp_t;

  const err = wordexp(pattern:c_string, glb, 0);

  for i in 0..chpl_wordexp_num(glb)-1 {
    yield chpl_wordexp_index(glb, i): string;
  }

  wordfree(glb);
}

iter glob(pattern="*") {
  extern type glob_t;
  //  extern proc chpl_glob(pattern:c_string, flags:c_int, ref ret_glob:glob_t):c_int;
  extern proc glob(pattern:c_string, flags: c_int, errfunc:c_void_ptr, ref ret_glob:glob_t):c_int;
  extern proc chpl_glob_num(x:glob_t): size_t;
  extern proc chpl_glob_index(x:glob_t, idx:size_t): c_string;
  extern proc globfree(ref glb:glob_t);

  var glb : glob_t;

  const err = glob(pattern:c_string, 0, c_nil, glb);

  for i in 0..chpl_glob_num(glb) - 1 do
    yield chpl_glob_index(glb, i): string;

  globfree(glb);
}

iter findfiles(startdir = ".", recur=false, dotfiles=false) {
  if (recur) then
    for subdir in walkdirs(startdir, dotfiles=dotfiles) do
      for file in listdir(subdir, dotfiles=dotfiles, dirs=false, files=true) do
        yield subdir+"/"+file;
  else
    for file in listdir(startdir, dotfiles=dotfiles, dirs=false, files=true) do
      yield startdir+"/"+file;
}

/* An incomplete start at putting together the routine in the proposal
 *
iter findfiles(pattern="*",
               startdir=".",
               depth=max(int),
               files=true,
               dirs=false,
               dotfiles=false,
               sorted=false,
               expand=false) {
  if (expand) then
    halt("not yet implemented: findfiles(..., expand=true)");
  if (pattern.substring(1) == "/") then
    for file in glob(pattern) do
      yield file;
  else
    for subdir in walkdirs(startdir, true, depth, dotfiles, false, sorted) {
      const subdirslash = subdir + "/";
      if (dirs) then
        yield subdirslash;
      for file in glob(subdirslash + pattern) do
        yield file;
    }
}
*/
               