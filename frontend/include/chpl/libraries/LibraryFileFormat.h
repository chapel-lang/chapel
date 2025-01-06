/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

/*
  This file implements several Plain Old Data classes that are
  parts of the library file format. Please see file-format.rst
  for a description of the file format.
 */

#ifndef CHPL_LIBRARIES_LIBRARY_FILE_FORMAT_H
#define CHPL_LIBRARIES_LIBRARY_FILE_FORMAT_H

#include <cstdint>

namespace chpl {
namespace libraries {


// magic numbers
// file magic number is <7F>LIBCHPL
static const uint64_t FILE_HEADER_MAGIC =      0x4c50484342494c7f;
static const uint64_t MODULE_SECTION_MAGIC =   0x4d4dd01e5ec14d4d;
static const uint64_t SYMBOL_TABLE_MAGIC =     0x4d59531e5ec110e0;
static const uint64_t UAST_SECTION_MAGIC =     0x5453411e5ec110e0;
static const uint32_t LONG_STRINGS_TABLE_MAGIC =       0x52545301;
static const uint64_t LOCATION_SECTION_MAGIC = 0x434F4C075ec110e0;
static const uint64_t GEN_CODE_SECTION_MAGIC = 0x4e4547075ec110e0;

// current file format version numbers
static const uint32_t FORMAT_VERSION_MAJOR =  0;
static const uint32_t FORMAT_VERSION_MINOR =  1;

// number of bytes in a file hash -- currently using SHA-256
static const int HASH_SIZE = 256/8;

// maximum number of of various elements; more than this many
// will result in an error. Purpose is to keep the processing
// from using too much space if the file is invalid anyway.
static const int MAX_NUM_MODULES = 100000000;
static const int MAX_NUM_SYMBOLS = 100000000;
static const int MAX_NUM_FILES = 100000000;
static const int MAX_NUM_ASTS = 100000000;

struct FileHeader {
  uint64_t magic;
  uint32_t fileFormatVersionMajor;
  uint32_t fileFormatVersionMinor;
  uint32_t chplVersionMajor;
  uint32_t chplVersionMinor;
  uint32_t chplVersionUpdate;
  uint32_t nModules;
  uint8_t hash[HASH_SIZE];
  // followed by nModules 8 bytes file offsets plus one more for
  // just after the last module
};

struct Region {
  uint64_t start; // start offset, maybe relative
  uint64_t end; // end offset; so length = end-start
};

static inline Region makeRegion(uint64_t startOffset, uint64_t endOffset) {
  Region ret;
  ret.start = startOffset;
  ret.end = endOffset;
  return ret;
}

struct ModuleHeader {
  uint64_t magic;
  uint64_t flags;
  // the following are offsets relative to the start of the module header
  Region symbolTable;
  Region astSection;
  Region longStringsTable;
  Region locationSection;
  Region genCodeSection;
  // TODO: add other sections

  // followed by a variable-byte length & string storing the module ID
};

struct SymbolTableHeader {
  uint64_t magic;
  uint32_t nEntries;
  uint32_t unused;
  // followed by nEntries SymbolTableEntrys
};

struct SymbolTableEntry {
  // relative to the start of the module header:
  uint32_t astEntry;
  uint32_t locationEntry;
  // TODO: type entry / function entry.
  // followed by
  //  * a byte storing ast tag
  //  * a variable-byte length & string storing a symbol table ID
};

struct AstSectionHeader {
  uint64_t magic;
  uint64_t nEntries;
  // followed by nEntries astEntrys
};

struct LongStringsTableHeader {
  uint32_t magic;
  uint32_t nLongStrings;
  // followed by strings-section relative offset for each long string
  // followed by an extra offset (just after the last long string)
};

struct LocationSectionHeader {
  uint64_t magic;
  uint32_t nFilePaths;
  uint32_t nGroups;
  // followed by file path strings
  // then followed serialized location groups
};

struct GenCodeSectionHeader {
  uint64_t magic;
  uint64_t len;
  // followed by the LLVM IR bc data
};



} // end namespace libraries
} // end namespace chpl

#endif
