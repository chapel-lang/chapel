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

iter findfiles(startdir = ".", recur=false, dotfiles=false) {
  if (recur) then
    for subdir in walkdirs(startdir, dotfiles=dotfiles) do
      for file in listdir(subdir, dotfiles=dotfiles, dirs=false, files=true) do
        yield subdir+"/"+file;
  else
    for file in listdir(startdir, dotfiles=dotfiles, dirs=false, files=true) do
      yield startdir+"/"+file;
}