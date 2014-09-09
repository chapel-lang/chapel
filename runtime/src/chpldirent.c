#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "chpldirent.h"
#include "chplrt.h"

int chpl_rt_isDir(const char* pathname) {
  struct stat fileinfo;
  stat(pathname, &fileinfo);
  return S_ISDIR(fileinfo.st_mode);
  /*  return fileinfo.st_mode & S_IFDIR;*/
}
