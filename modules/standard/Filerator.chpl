/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

use Sort;

//
// This module defines several iterators that are designed to help
// support reasoning about directory and file contents.  At present,
// all are serial and single-locale, though the ultimate intention
// is to also support parallel and multi-locale options.  
//
// These interfaces should be considered experimental and subject to
// change for the 1.10 release.  For that reason, send in feedback if
// you would like to see changes or additional arguments/features.
// And, as always, send in bug reports if you think we got something
// wrong.
//

/* iter listdir(path: string, dotfiles=false, dirs=true, files=true, 
                listlinks=true): string
  
    listdir() lists the contents of a directory, similar to 'ls'
      * path: the directory whose contents should be listed
      * dotfiles: should files/directories that start with '.' be listed?
      * dirs: should dirs be listed?
      * files: should files be listed?
      * listlinks: should symbolic links be listed?
  
   By default this routine lists all files and directories in the
   current directory, including symbolic links, as long as they don't
   start with '.'
*/

iter listdir(path: string, dotfiles=false, dirs=true, files=true, 
             listlinks=true): string {
  extern type DIRptr;
  extern type direntptr;
  extern proc opendir(name: c_string): DIRptr;
  extern proc readdir(dirp: DIRptr): direntptr;
  extern proc closedir(dirp: DIRptr): c_int;

  proc direntptr.d_name(): c_string {
    extern proc chpl_rt_direntptr_getname(d: direntptr): c_string;

    return chpl_rt_direntptr_getname(this);
  }

  var dir: DIRptr;
  var ent: direntptr;
  dir = opendir(path:c_string);
  if (!is_c_nil(dir)) {
    ent = readdir(dir);
    while (!is_c_nil(ent)) {
      const filename = ent.d_name();
      if (dotfiles || filename.substring(1) != '.') {
        if (filename != "." && filename != "..") {
          //
          // use FileSystem;  // Doesn't work, see comment below
          //
          const fullpath = path + "/" + filename;
          {
            //
            // The use of this compound statement to restrict the
            // impact of the 'use' of FileSystem is unfortunate
            // (compared to placing it in the more logical place
            // above), yet seemingly required at present; otherwise
            // the 'path' argument gets shadowed by a
            // (compiler-introduced?) method coming from one of the
            // standard or internal modules.  See
            // test/modules/bradc/useFileSystemShadowsPath.chpl for
            // a smaller standalone test exhibiting the issue (or
            // uncomment the 'use' above to see it here).
            //
            use FileSystem;

            if (listlinks || !isLink(fullpath)) {
              if (dirs && isDir(fullpath)) then
                yield filename;
              else if (files && isFile(fullpath)) then
                yield filename;
            }
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


/* iter walkdirs(path: string=".", topdown=true, depth=max(int), 
                 dotfiles=false, followlinks=false, sort=false): string
  
   walkdirs() recursively walks a directory structure, yielding
   directory names.  The strings that are generated will be rooted
   from 'path'.

     * path: the directory to start from
     * topdown: indicates whether to yield the directories using a
       preorder (vs. postorder) traversal
     * depth: indicates the maximal depth of recursion to use
     * dotfiles: indicates whether to enter directories with dotfile names
     * followlinks: indicates whether to follow symbolic links or not
     * sort: indicates whether to consider subdirectories in sorted
       order or not
  
   by default, walkdirs() will start in the current directory, process
   directories in preorder; recursively traverse subdirectories; and
   neither follow dotfile directories nor symbolic links.  It will not
   sort the directories by default.
*/

iter walkdirs(path: string=".", topdown=true, depth=max(int), dotfiles=false, 
              followlinks=false, sort=false): string {

  if (topdown) then
    yield path;

  if (depth) {
    var subdirs = listdir(path, dotfiles=dotfiles, files=false, listlinks=followlinks);
    if (sort) then
      QuickSort(subdirs);
    for subdir in subdirs {
      const fullpath = path + "/" + subdir;
      for subdir in walkdirs(fullpath, topdown, depth-1, dotfiles, 
                             followlinks, sort) do
        yield subdir;
    }
  }

  if (!topdown) then
    yield path;
}


/* iter wordexp(pattern="*")

   wordexp() gives a glob-like capability, implemented with C's wordexp()
   (which is, itself a routine that provides a glob-like capability :)

     * pattern: the glob pattern to match against
  
   By default, it will list all files/directories in the current directory
*/

iter wordexp(pattern="*") {
  extern type wordexp_t;
  extern proc wordexp(pattern:c_string, ref ret_glob: wordexp_t, 
                      flags_c_int): c_int;
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


/* iter glob(pattern="*")

   glob() gives glob() capabilities and is implemented using C's glob()

     * pattern: the glob pattern to match against

   By default, it will list all files/directories in the current directory
*/

iter glob(pattern="*") {
  extern type glob_t;
  extern proc chpl_glob(pattern:c_string, flags: c_int, 
                        ref ret_glob:glob_t):c_int;
  extern proc chpl_glob_num(x:glob_t): size_t;
  extern proc chpl_glob_index(x:glob_t, idx:size_t): c_string;
  extern proc globfree(ref glb:glob_t);

  var glb : glob_t;

  const err = chpl_glob(pattern:c_string, 0, glb);
  const num = chpl_glob_num(glb);
  if (num) then
    for i in 0..num-1 do
      yield chpl_glob_index(glb, i): string;


  globfree(glb);
}


/* iter findfiles(startdir = ".", recursive=false, dotfiles=false)

   findfiles() is a simple find-like utility implemented using the
   above routines

     * startdir: where to start when looking for files
     * recursive: tells whether or not to descend recurisvely
     * dotfiles: tells whether or not to yield dotfiles
*/

iter findfiles(startdir = ".", recursive=false, dotfiles=false) {
  if (recursive) then
    for subdir in walkdirs(startdir, dotfiles=dotfiles) do
      for file in listdir(subdir, dotfiles=dotfiles, dirs=false, files=true, listlinks=true) do
        yield subdir+"/"+file;
  else
    for file in listdir(startdir, dotfiles=dotfiles, dirs=false, files=true, listlinks=false) do
      yield startdir+"/"+file;
}
