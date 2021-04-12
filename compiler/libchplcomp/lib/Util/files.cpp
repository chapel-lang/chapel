#include "files.h"

#include "./mystrerror.h"

#include "chpl/AST/ErrorMessage.h"
#include "chpl/AST/Location.h"

namespace chpl {

FILE* openfile(const char* path, const char* mode, ast::ErrorMessage& errorOut) {
  FILE* fp = fopen(path, mode);
  if (fp == nullptr) {
    std::string strerr = my_strerror(errno);
    auto emptyLocation = ast::Location();
    // set errorOut. NULL will be returned.
    errorOut = ast::ErrorMessage::build(emptyLocation, "opening %s: %s",
                                   path, strerr.c_str());
  }

  return fp;
}

bool closefile(FILE* fp, const char* path, ast::ErrorMessage& errorOut) {
  int rc = fclose(fp);
  if (rc != 0) {
    std::string strerr = my_strerror(errno);
    auto emptyLocation = ast::Location();
    errorOut = ast::ErrorMessage::build(emptyLocation, "closing %s: %s",
                                   path, strerr.c_str());
    return false;
  }
  return true;
}

}
