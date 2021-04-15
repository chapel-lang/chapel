#ifndef FILES_H
#define FILES_H

#include <cstdio>
#include <string>

namespace chpl {

namespace ast {
  // forward declare
  class ErrorMessage;
}

/**
  Open a file. If the open failed, return nullptr and set errorOut.
 */
FILE* openfile(const char* path, const char* mode, ast::ErrorMessage& errorOut);

/**
  Close a file. If the close failed, return false and set errorOut.
 */
bool closefile(FILE* fp, const char* path, ast::ErrorMessage& errorOut);

/**
  Reads the contents of a file into a string.
  If something failed, returns false and sets errorOut.
 */
bool readfile(const char* path, std::string& strOut, ast::ErrorMessage& errorOut);

} // end namespace chpl

#endif
