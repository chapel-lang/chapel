/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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
  This file implements the generic chpl::serialize and chpl::deserialize
  as well as specializations for some common types.
 */

#ifndef CHPL_FRAMEWORK_LIBRARY_FILE_H
#define CHPL_FRAMEWORK_LIBRARY_FILE_H

namespace chpl {
class Context;


namespace detail {
  static const uint64_t FILE_HEADER_MAGIC =      0x4C5048434550487F;
  static const uint64_t MODULE_SECTION_MAGIC =   0x0030d01e5ec110e0;
  static const uint64_t SYMBOL_TABLE_MAGIC =     0x0003bb1e5ec110e0;
  static const uint32_t LONG_STRINGS_TABLE_MAGIC =       0x51e17601;
  static const uint64_t UAST_SECTION_MAGIC =     0x0003bb1e5ec110e0;
  static const uint64_t LOCATION_SECTION_MAGIC = 0x10ca11075ec110e0;

  struct FileHeader {
    uint64_t magic;
    uint32_t chplVersionMajor;
    uint32_t chplVersionMinor;
    uint32_t chplVersionUpdate;
    uint32_t nModules;
    // followed by nModules 8 bytes file offsets
  };

  struct ModuleHeader {
    uint64_t magic;
    uint64_t flags;
    int32_t majorVersion;
    int32_t minorVersion;
    // the following are offsets relative to the start of the module header
    uint64_t symbolTable;
    uint64_t longStringsTable;
    uint64_t uAstSection;
    uint64_t locationSection;
    uint64_t typesSection;
    uint64_t functionsSection;
    uint64_t dependenciesSection;
    // followed by a variable-byte length & string storing the module ID
  };

  struct SymbolTableHeader {
    uint64_t magic;
    uint32_t nEntries;
    // followed by nEntries SymbolTableEntrys
  };

  struct SymbolTableEntry {
    // relative to the start of the module header:
    uint64_t uAstEntry;
    uint64_t locationEntry;
    uint64_t typeOrFnEntry;
    // followed by
    //  * a byte storing uAst tag
    //  * a variable-byte length & string storing a symbol table ID
  };

  struct LongStringsTableHeader {
    uint32_t magic;
    uint32_t nLongStrings;
    // followed by module-section relative offset for each long string
    // followed by an extra offset (just after the last long string)
  };
  
  struct uAstSectionHeader {
    uint64_t magic;
    uint64_t nEntries;
    // followed by nEntries uAstEntrys
  };

  struct LocationSectionHeader {
    uint64_t magic;
    uint32_t nFilePaths;
    uint32_t nGroups;
    // followed by file path strings
    // then followed nGroups LocationGroups
  };
  struct LocationGroupHeader {
    // relative to the start of the module header:
    uint64_t uAstEntry;
    uint32_t startingLineNumber;
    uint32_t nLocations;
    // followed by location entries
  };
};

/** For writing a .dyno library file */
class LibraryFileWriter {
  
};

/** For reading a .dyno LibraryFile.
    Some data is read from the file on-demand. */
class LibraryFile {

};


} // end namespace chpl

#endif
