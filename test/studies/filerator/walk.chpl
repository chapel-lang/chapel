config const debug = false;

iter walkdirs(path: string=".", topdown=true, depth=max(int), dotfiles=false, followlinks=false): string {
  extern type DIRptr;
  extern type direntptr;
  extern proc opendir(name: c_string): DIRptr;
  extern proc readdir(dirp: DIRptr): direntptr;
  extern proc closedir(dirp: DIRptr): c_int;

  proc direntptr.d_name(): c_string {
    extern proc chpl_rt_direntptr_getname(d: direntptr): c_string;

    return chpl_rt_direntptr_getname(this);
  }

  proc direntptr.isDir() {
    extern proc chpl_rt_direntptr_isDir(d: direntptr): bool;

    return chpl_rt_direntptr_isDir(this);
  }

  if (topdown) then
    yield path;

  if (depth) {
    var dir: DIRptr;
    var ent: direntptr;
    if debug then
      writeln("***Trying ", path);
    dir = opendir(path:c_string);
    if (!is_c_nil(dir)) {
      ent = readdir(dir);
      while (!is_c_nil(ent)) {
        const filename = ent.d_name();
        if (ent.isDir() && filename != "." && filename != "..") {
          if debug then
            writeln("***It's a dir!");
          if (dotfiles || filename.substring(1) != '.') {
            const fullpath = path + "/" + filename;
            //
            // feature request: This is a nice place for a yieldall concept
            //
            for dirname in walkdirs(fullpath, topdown, depth-1, dotfiles, followlinks) do
              yield dirname;
          }
        }
        ent = readdir(dir);
      }
    } else {
      extern proc perror(s: c_string);
      perror("error in walkdirs(): ");
    }
    closedir(dir);
  }

  if (!topdown) then
    yield path;
}

iter sorted(x) {
  use Sort;

  var y = x;
  QuickSort(y);
  for i in y do
    yield i;
}

config const startdir = "subdir";
config const topdown = true;
config const depth = max(int);
config const dotfiles = false;
config const followlinks = false;

config const sort = true;
config const defaults = false;

if defaults then
  if sort then
    for filename in sorted(walkdirs()) do
      writeln(filename);
 else
   for filename in walkdirs() do
     writeln(filename);
else
  if sort then
    for filename in sorted(walkdirs(startdir, topdown, depth, dotfiles, followlinks)) do
      writeln(filename);
 else
   for filename in walkdirs(startdir, topdown, depth, dotfiles, followlinks) do
     writeln(filename);
