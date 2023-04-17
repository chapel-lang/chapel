/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include <algorithm>
#include <cctype>
#include <cstring>
#include <ios>
#include <iostream>
#include <fstream>
#include <limits>
#include <regex>
#include <unordered_map>
#include <queue>

#include "arg.h"
#include "arg-helpers.h"

#include "chpl/parsing/Parser.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/compiler-configuration.h"
#include "chpl/framework/Context.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/framework/query-impl.h"
#include "chpl/framework/stringify-functions.h"
#include "chpl/framework/update-functions.h"
#include "chpl/uast/TypeDecl.h"
#include "chpl/uast/all-uast.h"
#include "chpl/util/string-escapes.h"
#include "chpl/util/filesystem.h"
#include "chpl/uast/chpl-syntax-printer.h"
#include "chpl/framework/global-strings.h"
#include "chpl/resolution/scope-queries.h"

#include "llvm/Support/FileSystem.h"
#include "chpl/util/version-info.h"


using namespace chpl;
using namespace uast;
using namespace parsing;

using CommentMap = std::unordered_map<ID, const Comment*>;

char fDocsAuthor[256] = "";
bool fDocsAlphabetize = false;
char fDocsCommentLabel[256] = "";
char fDocsFolder[256] = "";
bool fDocsTextOnly = false;
char fDocsSphinxDir[256] = "";
bool fDocsHTML = true;
char fDocsProjectVersion[256] = "0.0.1";
bool printSystemCommands = false;
bool fPrintCopyright = false;
bool fPrintHelp = false;
bool fPrintEnvHelp = false;
bool fPrintSettingsHelp = false;
bool fPrintChplHome = false;
bool fPrintVersion = false;
bool fWarnUnknownAttributeToolname = true;

std::vector<UniqueString> usingAttributeToolNames;
std::vector<std::string> usingAttributeToolNamesStr;

Context* gContext;


static const int indentPerDepth = 3;
std::string commentStyle_;
std::string outputDir_;
bool textOnly_ = false;
std::string CHPL_HOME;
bool processUsedModules_ = false;
bool fDocsProcessUsedModules = false;

static
void docsArgSetCommentLabel(const ArgumentDescription* desc, const char* label) {
  assert(label != NULL);
  size_t len = strlen(label);
  if (len != 0) {
    if (len > sizeof(fDocsCommentLabel)) {
      std::cerr << "error: the label is too large!" << std::endl;
      clean_exit(1);
    } else {
      strcpy(fDocsCommentLabel, label);
    }
  }
}


static
void driverSetHelpTrue(const ArgumentDescription* desc, const char* unused) {
  fPrintHelp = true;
}

static
void setHome(const ArgumentDescription* desc, const char* arg) {
  CHPL_HOME = std::string(arg);
}

/*
  Because the context is not setup during argument processing, it will not work
  to use UniqueString::get() to get the UniqueString for the toolnames. So we
  temporarily store the names as strings during arg processing and then promote
  them to unique strings here.
*/
static void promoteAttributeToolNameStrToUniqueString() {
  for (auto toolName : usingAttributeToolNamesStr) {
    UniqueString name = UniqueString::get(gContext, toolName);
    usingAttributeToolNames.push_back(name);
  }
  usingAttributeToolNamesStr.clear();
}

/*
  this function is called when a toolname is passed through the command line
  with the --using-attribute-toolname flag. It is called each time the flag is
  found in the command line, which may be multiple. These toolnames will then
  be treated as known to the compiler and we won't warn about them. We store
  them as strings temporarily because the context is not setup prior to argument
  processing.
*/
static void addUsingAttributeToolNameStr(const ArgumentDescription* desc,
                                         const char* arg) {
  std::string name = std::string(arg);
  usingAttributeToolNamesStr.push_back(name);
}

#define DRIVER_ARG_COPYRIGHT \
  {"copyright", ' ', NULL, "Show copyright", "F", &fPrintCopyright, NULL, NULL}

#define DRIVER_ARG_HELP \
  {"help", 'h', NULL, "Help (show this list)", "F", &fPrintHelp, NULL, NULL}

#define DRIVER_ARG_HELP_ENV \
  {"help-env", ' ', NULL, "Environment variable help", "F", &fPrintEnvHelp, "", driverSetHelpTrue}

#define DRIVER_ARG_HELP_SETTINGS \
  {"help-settings", ' ', NULL, "Current flag settings", "F", &fPrintSettingsHelp, "", driverSetHelpTrue}

#define DRIVER_ARG_HOME \
  {"home", ' ', "<path>", "Path to Chapel's home directory", "S", NULL, "_CHPL_HOME", setHome}

#define DRIVER_ARG_PRINT_CHPL_HOME \
  {"print-chpl-home", ' ', NULL, "Print CHPL_HOME and path to this executable and exit", "F", &fPrintChplHome, NULL,NULL}

#define DRIVER_ARG_VERSION \
  {"version", ' ', NULL, "Show version", "F", &fPrintVersion, NULL, NULL}

#define DRIVER_ARG_LAST \
  {0}

static ArgumentState sArgState = {
  0,
  0,
  "program",
  "path",
  NULL
};

ArgumentDescription docs_arg_desc[] = {
 {"", ' ', NULL, "Documentation Options", NULL, NULL, NULL, NULL},
 {"output-dir", 'o', "<dirname>", "Sets the documentation directory to <dirname>", "S256", fDocsFolder, NULL, NULL},
 {"author", ' ', "<author>", "Documentation author string.", "S256", fDocsAuthor, "CHPLDOC_AUTHOR", NULL},
 {"comment-style", ' ', "<indicator>", "Only includes comments that start with <indicator>", "S256", fDocsCommentLabel, NULL, docsArgSetCommentLabel},
 {"process-used-modules", ' ', NULL, "Also parse and document 'use'd modules", "F", &fDocsProcessUsedModules, NULL, NULL},
 {"save-sphinx",  ' ', "<directory>", "Save generated Sphinx project in directory", "S256", fDocsSphinxDir, NULL, NULL},
 {"text-only", ' ', NULL, "Generate text documentation only", "F", &fDocsTextOnly, NULL, NULL},
 {"html", ' ', NULL, "[Don't] generate html documentation (on by default)", "N", &fDocsHTML, NULL, NULL},
 {"project-version", ' ', "<projectversion>", "Sets the documentation version to <projectversion>", "S256", fDocsProjectVersion, "CHPLDOC_PROJECT_VERSION", NULL},

 {"print-commands", ' ', NULL, "[Don't] print system commands", "N", &printSystemCommands, "CHPL_PRINT_COMMANDS", NULL},
 {"warn-unknown-attribute-toolname", ' ', NULL, "Enable warnings when an unknown tool name is found in an attribute", "N", &fWarnUnknownAttributeToolname, "CHPL_WARN_UNKNOWN_ATTRIBUTE_TOOLNAME", NULL},
 {"using-attribute-toolname", ' ', "<toolname>", "Specify additional tool names for attributes that are expected in the source", "S", NULL, "CHPL_ATTRIBUTE_TOOLNAMES", addUsingAttributeToolNameStr},
 {"", ' ', NULL, "Information Options", NULL, NULL, NULL, NULL},

 DRIVER_ARG_HELP,
 DRIVER_ARG_HELP_ENV,
 DRIVER_ARG_HELP_SETTINGS,
 DRIVER_ARG_VERSION,
 DRIVER_ARG_COPYRIGHT,
 {"", ' ', NULL, "Developer Flags", NULL, NULL, NULL, NULL},
 DRIVER_ARG_HOME,
 DRIVER_ARG_PRINT_CHPL_HOME,
 DRIVER_ARG_LAST
};


static void printStuff(const char* argv0, void* mainAddr) {
  bool shouldExit       = false;
  bool printedSomething = false;

  if (fPrintVersion) {
    std::string version = chpl::getVersion();
    fprintf(stdout, "%s version %s\n", sArgState.program_name, version.c_str());

    fPrintCopyright  = true;
    printedSomething = true;
    shouldExit       = true;
  }

  if (fPrintCopyright) {
    fprintf(stdout,
#include "COPYRIGHT"
            );

    printedSomething = true;
  }

  if( fPrintChplHome ) {
    std::string guess = findProgramPath(argv0, mainAddr);
    printf("%s\t%s\n", CHPL_HOME.c_str(), guess.c_str());
    printedSomething = true;
  }

  if (fPrintHelp || (!printedSomething && sArgState.nfile_arguments < 1)) {
    if (printedSomething) printf("\n");

    usage(&sArgState, !fPrintHelp, fPrintEnvHelp, fPrintSettingsHelp);

    shouldExit       = true;
    printedSomething = true;
  }

  if (printedSomething && sArgState.nfile_arguments < 1) {
    shouldExit       = true;
  }

  if (shouldExit) {
    clean_exit(0);
  }
}



const std::string templateUsage = R"RAW(**Usage**

.. code-block:: chapel

   use $MODULE;


or

.. code-block:: chapel

   import $MODULE;)RAW";

const std::string textOnlyTemplateUsage = R"RAW(

Usage:
   use $MODULE;

or

   import $MODULE;)RAW";


static void checkKnownAttributes(const AttributeGroup* attrs) {
  // TODO: we need a way to get all the toolspaced attributes for chpldoc, or
  // any tool, in one go. That will allow us to check for any unrecognized
  // attributes.
  for (auto attr : attrs->children()) {
    if (attr->toAttribute()->name().startsWith(USTR("chpldoc."))) {
      if (attr->toAttribute()->name() == UniqueString::get(gContext, "chpldoc.nodoc")) {
        // ignore, it's a known attribute
      } else {
        // process the Error about unknown Attribute
        std::string msg = "Unknown attribute '";
        msg += attr->toAttribute()->name().c_str();
        msg += "'";
        auto loc = locateId(gContext, attr->id());
        auto err = GeneralError::get(ErrorBase::ERROR, loc, msg);
        gContext->report(std::move(err));
      }
      // warning about other attribute toolnames is handled by the dyno library
    }
  }
}

static bool isNoDoc(const Decl* e) {
  if (auto attrs = e->attributeGroup()) {
    auto attr = attrs->getAttributeNamed(UniqueString::get(gContext, "chpldoc.nodoc"));
    if (attr || attrs->hasPragma(pragmatags::PRAGMA_NO_DOC)) {
      return true;
    }
  }
  return false;
}

static std::vector<std::string> splitLines(const std::string& s) {
  std::stringstream ss(s);
  std::string line;
  std::vector<std::string> ret;
  while (std::getline(ss, line)) {
    ret.push_back(line);
  }
  return ret;
}

static std::string filenameFromModuleName(std::string name,
                                          std::string docsWorkDir) {
  // Borrowed from chpldoc
  std::string filename = name;
  size_t location = filename.rfind("/");
  if (location != std::string::npos) {
    filename = filename.substr(0, location + 1);

    // Check for files starting with the CHPL_HOME internal modules
    // path, and if we find one, chop everything but 'internal/' and
    // whatever follows out of the path in order to create the
    // appropriate relative path within the sphinx output directory.
    std::string modPath = CHPL_HOME + "/modules/";
    std::string intModPath = modPath + "internal/";
    if (strncmp(intModPath.c_str(), filename.c_str(), intModPath.length()) == 0) {
      filename = filename.substr(modPath.length());
    }
  } else {
    filename = "";
  }
  filename = docsWorkDir + "/" + filename;

  return filename;
}

static bool hasSubmodule(const Module* mod) {
  for (const AstNode* child : mod->stmts()) {
    if (auto mod = child->toModule())
      if (mod->visibility() != chpl::uast::Decl::PRIVATE && !isNoDoc(mod)) {
        return true;
    }
  }
  return false;
}

static std::string strip(const std::string& s,
                         std::string pattern = "^\\s+|\\s+$") {
  auto re = std::regex(pattern);
  return std::regex_replace(s, re, "");
}

static std::string templateReplace(const std::string& templ,
                                   const std::string& key,
                                   const std::string& value) {
  return std::regex_replace(templ, std::regex(std::string("\\$") + key), value);
}

static UniqueString getNodeName(AstNode* node) {
  if (node->isNamedDecl()) {
    return node->toNamedDecl()->name();
  } else if (node->isIdentifier()) {
    return node->toIdentifier()->name();
  } else {
    assert(false && "no name defined for node");
    return UniqueString();
  }
}

/*
 * Returns the current working directory. Does not report failures. Use
 * currentWorkingDir() if you need error reports.
 */
static
std::string getCwd() {
  std::string ret;
  if (auto err = currentWorkingDir(ret)) {
    return "";
  } else {
    return ret;
  }
}

static
std::string runCommand(std::string& command) {
  // Run arbitrary command and return result
  char buffer[256];
  std::string result = "";

  // Call command
  FILE* pipe = popen(command.c_str(), "r");
  if (!pipe) {
    std::cerr << "error: running " << command << std::endl;
  }

  // Read output of command into result via buffer
  while (!feof(pipe)) {
    if (fgets(buffer, 256, pipe) != NULL) {
      result += buffer;
    }
  }

  if (pclose(pipe)) {
    std::cerr << "error: '" << command << "' did not run successfully"
              << std::endl;
  }

  return result;
}

static int myshell(std::string command,
                   std::string description,
                   bool        ignoreStatus = false,
                   bool        quiet = false,
                   bool        printSystemCommands = false) {

  int status = 0;

  if (printSystemCommands && !quiet) {
    printf("\n# %s\n", description.c_str());
    printf("%s\n", command.c_str());
    fflush(stdout);
    fflush(stderr);
  }

  // Treat a '#' at the start of a line as a comment
  if (command[0] == '#')
    return 0;

  status = system(command.c_str());

  if (status == -1) {
    std::cerr << "error: system() fork failed: " << strerror(errno)
              << std::endl;
  } else if (status != 0 && ignoreStatus == false) {
    std::cerr << "error: " << description << std::endl;
  }

  return status;
}


static
std::string getChplDepsApp() {
  // Runs `util/chplenv/chpl_home_utils.py --chpldeps` and removes the newline
  std::string command = "CHPLENV_SUPPRESS_WARNINGS=true CHPL_HOME=" +
                         CHPL_HOME + " python3 ";
  command += CHPL_HOME + "/util/chplenv/chpl_home_utils.py --chpldeps";

  std::string venvDir = runCommand(command);
  venvDir.erase(venvDir.find_last_not_of("\n\r")+1);

  return venvDir;
}

static char* checkProjectVersion(char* projectVersion) {
  int length = strlen(projectVersion);
  int i = 0;
  int dot = 0;
  bool check = true;
  int tagIndex = 0;

  // Supported version tags
  const char * tags[] = {"alpha", "beta", "rc"};
  const char * error = "";
  for(i=0; i<length; i++) {
    if(i>0 && projectVersion[i] == '.') {
      if(projectVersion[i-1] != '.') {
        dot++;
        if(dot>2) {
          error = "Required only two dots which separates three numbers";
          check = false;
          break;
        }
        if(i == length-1) {
          error = "Cannot end with dot, can end with either number or tag";
          check = false;
          break;
        }
      } else {
        error = "Missing number between dots";
        check = false;
        break;
      }
    } else if(projectVersion[i] == '-' && dot == 2) {
      if(projectVersion[i-1] != '.') {
        tagIndex = i+1;
        break;
      } else {
        error = "Missing number before tag";
        check = false;
        break;
      }
    } else if(projectVersion[i] == '-' && dot != 2) {
      error = "Required only two dots which separates three numbers";
      check = false;
      break;
    } else if((int)projectVersion[i] > (int)'9' ||
              (int)projectVersion[i] < (int)'0') {
      error = "Invalid Characters, only digits and dots permitted before a hyphen";
      check = false;
      break;
    }
  }
  if(dot != 2 && i == length) {
    error = "Required two dots which separates three numbers";
    check = false;
  }
  if(check && tagIndex>0) {
    int count = sizeof(tags)/sizeof(*tags);
    for(int i=0; i<count; i++) {
      if(strcmp(projectVersion+tagIndex,tags[i]) == 0) {
        check = true;
        break;
      } else {
        error = "Tag not supported, supported tags are alpha/beta/rc";
        check = false;
      }
    }
  }
  if(check) {
    return projectVersion;
  } else {
    std::cerr << "error: Invalid version format: "
              << projectVersion << " due to: " << error << std::endl;
    clean_exit(1);
  }
  return NULL;
}

static std::string indentLines(const std::string& s, int count) {
  if (s.empty())
    return s;
  std::string head = std::string(count, ' ');
  std::string ret = head + s;
  return ret;
}

static size_t countLeadingSpaces(const std::string& s) {
  size_t i = 0;
  while (std::isspace(s[i])) {i += 1;}
  return i;
}

static std::ostream& indentStream(std::ostream& os, size_t num) {
  const char* spaces = "        ";
  size_t N = strlen(spaces);
  while (num > N) {
    os << spaces;
    num -= N;
  }
  auto leftover = N - num;
  os << (spaces + leftover);

  return os;
}

// Remove the leading+trailing commentStyle (/*+, *+/)
// Dedent by the least amount of leading whitespace
// Return is a list of strings which may have newline chars
static std::string prettifyComment(const std::string& comment,
                                       const std::string& commentStyle,
                                       std::vector<std::string>& lines) {
  std::string ret = comment;
  std::string commentEnd = commentStyle;
  reverse(commentEnd.begin(), commentEnd.end());
  const int styleLen = commentStyle.size();
  // try to match the commentStyle to the start and end of this comment
  // and remove the comment indicators
  if (ret.substr(0, styleLen) == commentStyle) {
    size_t l = ret.length();
    if (ret.substr(l - styleLen, styleLen) != commentEnd) {
      // this is a broken comment at this point
      auto msg = "chpldoc comment not closed, ignoring comment: "
                 "This comment is not closed properly\n";
      return msg;
    }
    ret.erase(l - styleLen, styleLen);
    ret.erase(0, styleLen);
  } else {
    return "";
  }

  lines = splitLines(ret);
  if (lines.empty()) return "";

  size_t toTrim = std::numeric_limits<size_t>::max();
  // exclude the first line from the minimum
  for (auto it = lines.begin(); it < lines.end(); ++it) {
    if (strip(*it).empty()) continue;
    if (it == lines.begin()) {
      *it = strip(*it, "^\\s+");
      continue;
    }
    toTrim = std::min(toTrim, countLeadingSpaces(*it));
  }

  // probably a comment with one leading text line and some trailing empty lines
  if (!(toTrim < std::numeric_limits<size_t>::max() || lines.size()==1)) {
    toTrim = 0;
  }

  for (auto it = lines.begin(); it < lines.end(); ++it) {
    if (it != lines.begin())
      it->erase(0, toTrim);
  }

  return "";
}


/*
 *  Gets the first non-blank line of a comment and return it, stripped of any
 *  leading whitespace. Returns an empty string if there are no non-blank lines.
 */
static std::string commentSynopsis(const Comment* c,
                                   const std::string& commentStyle,
                                   std::string& errMsg) {
  if (!c) return "";

  std::vector<std::string> lines;
  errMsg = prettifyComment(c->str(), commentStyle, lines);

  if (lines.empty()) return "";
  uint i = 0;
  // skip empty lines for synopsis collection
  while (i < lines.size()) {
    if (!lines[i].empty())
    return strip(lines[i], "^\\s+");
    i++;
  }
  return "";
}


static const char* kindToRstString(bool isMethod, Function::Kind kind) {
  switch (kind) {
  case Function::Kind::PROC: return isMethod ? "method" : "function";
  case Function::Kind::ITER: return isMethod ? "itermethod" : "iterfunction";
  case Function::Kind::OPERATOR: return isMethod ? "method" : "function";
  case Function::Kind::LAMBDA: return "lambda";
  }
  assert(false);
  return "";
}

static const char* kindToString(Function::Visibility kind) {
  switch (kind) {
  case Function::Visibility::PRIVATE: return "private";
  case Function::Visibility::PUBLIC: return "public";
  case Function::Visibility::DEFAULT_VISIBILITY: assert(false);
  }
  assert(false);
  return "";
}

static const char* kindToString(Function::Kind kind) {
  switch (kind) {
  case Function::Kind::PROC: return "proc";
  case Function::Kind::ITER: return "iter";
  case Function::Kind::OPERATOR: return "operator";
  case Function::Kind::LAMBDA: return "lambda";
  }
  assert(false);
  return "";
}

static const char* kindToString(Qualifier kind) {
  switch (kind) {
    case Qualifier::CONST_INTENT: return "const";
    case Qualifier::VAR: return "var";
    case Qualifier::CONST_VAR: return "const";
    case Qualifier::CONST_REF: return "const ref";
    case Qualifier::REF: return "ref";
    case Qualifier::IN: return "in";
    case Qualifier::CONST_IN: return "const in";
    case Qualifier::OUT: return "out";
    case Qualifier::INOUT: return "inout";
    case Qualifier::PARAM: return "param";
    case Qualifier::TYPE: return "type";
    case Qualifier::DEFAULT_INTENT: assert(false);
    default: return "";
  }
  return "";
}

static const char* kindToString(New::Management kind) {
  switch (kind) {
  case New::Management::BORROWED: return "borrowed";
  case New::Management::OWNED: return "owned";
  case New::Management::SHARED: return "shared";
  case New::Management::UNMANAGED: return "unmanaged";
  case New::Management::DEFAULT_MANAGEMENT: assert(false);
  default:
    assert(false);
  }
  assert(false);
  return "";
}

static bool isCalleReservedWord(const AstNode* callee) {
  if (callee->isIdentifier() &&
      (callee->toIdentifier()->name() == USTR("borrowed")
       || callee->toIdentifier()->name() == USTR("owned")
       || callee->toIdentifier()->name() == USTR("unmanaged")
       || callee->toIdentifier()->name() == USTR("shared")
       || callee->toIdentifier()->name() == USTR("sync")
       || callee->toIdentifier()->name() == USTR("single")
       || callee->toIdentifier()->name() == USTR("atomic")))
      return true;
    return false;
}

/**
 Converts an AstNode into an RST format that is suitable for the RHS of
something like:

Function:
.. function:: proc foo(arg1: int): bool
              ^^^^^^^^^^^^^^^^^^^^^^^^^
Enum:
.. enum:: enum Foo { a = 0, b = 2 }
          ^^^^^^^^^^^^^^^^^^^^^^^^^
etc.
 */
struct RstSignatureVisitor {
  std::ostream& os_;
  bool printingType_ = false;
  /** traverse each elt of begin..end, outputting `separator` between each.
   * `surroundBegin` and `surroundEnd` are output before and after respectively
   * if not null */
  template<typename It>
  void interpose(It begin, It end, const char* separator,
                 const char* surroundBegin=nullptr,
                 const char* surroundEnd=nullptr) {
    bool first = true;
    if (surroundBegin) os_ << surroundBegin;
    for (auto it = begin; it != end; it++) {
      if (!first) os_ << separator;
      (*it)->traverse(*this);
      first = false;
    }
    if (surroundEnd) os_ << surroundEnd;
  }

  template<typename T>
  void interpose(T xs, const char* separator,
                 const char* surroundBegin=nullptr,
                 const char* surroundEnd=nullptr) {
    interpose(xs.begin(), xs.end(), separator, surroundBegin, surroundEnd);
  }

  template<typename T>
  bool enterLiteral(const T* l) {
    os_ << l->text().c_str();
    return false;
  }

  bool isPostfix(const OpCall* op) {
    return (op->isUnaryOp() &&
            (op->op() == USTR("postfix!") || op->op() == USTR("?")));
  }


  /*
    helper for printing binary op calls, special handling for keyword operators
    to conditionally add spaces around the operator
  */
  void printBinaryOp(const OpCall* node) {
    assert(node->numActuals() == 2);
    bool addSpace = wantSpaces(node->op(), printingType_) ||
                    node->op() == USTR("by") ||
                    node->op() == USTR("align") ||
                    node->op() == USTR("reduce=") ||
                    node->op() == USTR("reduce") ||
                    node->op() == USTR("scan") ||
                    node->op() == USTR("dmapped");
    opHelper(node, 0, false);
    if (addSpace && node->op() != USTR(":"))
      os_ << " ";
    os_ << node->op();
    if (addSpace)
      os_ << " ";
    opHelper(node, 1, false);
  }

  /*
    helper for printing unary op calls
  */
  void printUnaryOp(const OpCall* node) {
    assert(node->numActuals() == 1);
    UniqueString unaryOp;
    bool isPostFixBang = false;
    bool isNilable = false;
    unaryOp = node->op();
    if (unaryOp == USTR("postfix!")) {
      isPostFixBang = true;
      unaryOp = USTR("!");
    } else if (node->op() == USTR("?")) {
      isNilable = true;
    } else {
      os_ << unaryOp;
    }
    opHelper(node, 0, (isPostFixBang || isNilable));
    if (isPostFixBang || isNilable) {
      os_ << unaryOp;
    }
  }


  /*
    Helper for printing operands of binary and unary op calls
  */
  void opHelper(const OpCall* node, int pos, bool postfix) {
    bool needsParens = false;
    bool isRHS = pos;
    UniqueString outerOp, innerOp;
    outerOp = node->op();
    if (node->actual(pos)->isOpCall()) {
      innerOp = node->actual(pos)->toOpCall()->op();
      needsParens = needParens(outerOp, innerOp, node->isUnaryOp(), postfix,
                               node->actual(pos)->toOpCall()->isUnaryOp(),
                               isPostfix(node->actual(pos)->toOpCall()), isRHS);
    }
    // handle printing parens around tuples
    // ex: 3*(4*string) != 3*4*string
    needsParens = tupleOpNeedsParens(node, needsParens, isRHS, outerOp,
                                     innerOp);
    if (needsParens) os_ << "(";
    node->actual(pos)->traverse(*this);
    if (needsParens) os_ << ")";
  }

  // check if this is a star tuple decl, for example:
  // 3*string or 2*(3*string), etc
  bool isStarTupleDecl(UniqueString op, const OpCall* node) const {
    // TODO: need to adjust the rules as to what exactly can construct
    //  a star tuple, it's not clear how to filter FnCalls
    //  ex: 3*(real(64)) is a * op with lhs = IntLiteral(3) and
    //  rhs = FnCall(real->64)
    bool ret = false;
    if (node && op == USTR("*") && node->actual(0)->isIntLiteral() &&
        (node->actual(1)->isIdentifier() ||
         node->actual(1)->isTuple() ||
         node->actual(1)->isFnCall())) {
      ret = true;
    }
    return ret;
  }

  /*
    Helper to determine if we should print parens around a tuple, typically
    because it is a star tuple declaration
  */
  bool tupleOpNeedsParens(const OpCall* node, bool needsParens, bool isRHS,
                          UniqueString& outerOp, UniqueString& innerOp) const {
    if (needsParens || !isRHS) return needsParens;
    bool ret = needsParens;
    bool isOuterStarTuple = isStarTupleDecl(outerOp, node);
    bool isInnerStarTuple = isStarTupleDecl(innerOp,
                                            node->actual(1)->toOpCall());
    if ((isOuterStarTuple || isInnerStarTuple)) {
      ret = true;
    }
    return ret;
  }

  bool enter(const Array* arr) {
    interpose(arr->children(), ", ", "[", "]");
    return false;
  }

  bool enter(const Block* b) {
    return true;
  }

  bool enter(const BoolLiteral* b) {
    os_ << (b->value() ? "true" : "false");
    return false;
  }

  bool enter(const BracketLoop* bl) {
    os_ << "[";
    if (bl->index()) {
      bl->index()->traverse(*this);
      os_ << " in ";
    }
    if (bl->isMaybeArrayType() &&
        bl->iterand()->isDomain() &&
        bl->iterand()->toDomain()->numExprs() == 1) {
      bl->iterand()->toDomain()->expr(0)->traverse(*this);
    } else {
      bl->iterand()->traverse(*this);
    }

    if (bl->withClause()) {
      os_<< " ";
      bl->withClause()->traverse(*this);
    }
    os_ << "]";
    if (bl->numStmts() > 0) {
      os_ << " ";
        if (bl->stmt(0)->isOpCall()) {
          interpose(bl->stmts(), "", "(", ")");
          if (!bl->isExpressionLevel())
            os_ << ";";
        } else
          interpose(bl->stmts(), "");
      }
    return false;
  }

  bool enter(const Class* c) {
    os_ << c->name().c_str();
    if (c->parentClass()) {
      os_ << " : ";
      c->parentClass()->traverse(*this);
    }
    return false;
  }

  bool enter(const Conditional* node) {
    node->stringify(os_, StringifyKind::CHPL_SYNTAX);
    return false;
  }

  bool enter(const CStringLiteral* l) {
    os_ << "c\"" << escapeStringC(l->value().str()) << '"';
    return false;
  }

  bool enter(const Domain* dom) {
    if (dom->usedCurlyBraces()) {
      interpose(dom->exprs(), ", ", "{", "}");
    } else {
      interpose(dom->exprs(), ", ");
    }
    return false;
  }

  bool enter(const Dot* d) {
    d->receiver()->traverse(*this);
    os_ << "." << d->field().c_str();
    return false;
  }

  bool enter(const Enum* e) {
    os_ << "enum " << e->name().c_str() << " ";
    interpose(e->enumElements(), ", ", "{ ", " }");
    return false;
  }

  bool enter(const EnumElement* e) {
    os_ << e->name().c_str();
    if (const AstNode* ie = e->initExpression()) {
      os_ <<  " = ";
      ie->traverse(*this);
    }
    return false;
  }

  bool enter(const FnCall* call) {
    const AstNode* callee = call->calledExpression();
    assert(callee);
    callee->traverse(*this);
    if (isCalleReservedWord(callee)) {
      os_ << " ";
      call->actual(0)->traverse(*this);
    } else {
      if (call->callUsedSquareBrackets()) {
        os_ << "[";
      } else {
        os_ << "(";
      }
      std::string sep;
      for (int i = 0; i < call->numActuals(); i++) {
        os_ << sep;
        if (call->isNamedActual(i)) {
          os_ << call->actualName(i);
          // The spaces around = are just to satisfy old tests
          // TODO: Remove spaces around `=` when removing old parser
          os_ << " = ";
        }

        call->actual(i)->traverse(*this);

        sep = ", ";
      }
      if (call->callUsedSquareBrackets()) {
        os_ << "]";
      } else {
        os_ << ")";
      }
    }
    return false;
  }

  bool enter(const Formal* f) {
    if (f->intent() != Formal::DEFAULT_INTENT) {
      os_ << kindToString((Qualifier) f->intent()) << " ";
    }
    os_ << f->name().c_str();
    if (const AstNode* te = f->typeExpression()) {
      os_ << ": ";
      printingType_ = true;
      te->traverse(*this);
      printingType_ = false;
    }
    if (const AstNode* ie = f->initExpression()) {
      os_ << " = ";
      ie->traverse(*this);
    }
    return false;
  }

  bool enter(const Function* f) {
    // Linkage
    if (f->linkage() == Decl::Linkage::EXPORT)
      os_ << "export ";

    // Visibility
    if (f->visibility() != Function::Visibility::DEFAULT_VISIBILITY) {
      os_ << kindToString(f->visibility()) << " ";
    }

    if (f->isOverride()) {
      os_ << "override ";
    }

    // Function Name
    os_ << kindToString(f->kind());
    os_ << " ";

    // storage kind
    if (f->thisFormal() != nullptr
        && f->thisFormal()->storageKind() != Qualifier::DEFAULT_INTENT
        && f->thisFormal()->storageKind() != Qualifier::CONST_INTENT
        && f->thisFormal()->storageKind() != Qualifier::CONST_REF) {
      os_ << kindToString(f->thisFormal()->storageKind()) <<" ";
    }

    // print out the receiver type for secondary methods
    if (f->isMethod() && !f->isPrimaryMethod()) {
      auto typeExpr = f->thisFormal()->typeExpression();
      assert(typeExpr);

      if (auto ident = typeExpr->toIdentifier()) {
        os_ << ident->name().str();
      } else {
        os_ << "(";
        typeExpr->traverse(*this);
        os_ << ")";
      }

      os_ << ".";
    }

    if (f->kind() == Function::Kind::OPERATOR && f->name() == USTR("=")) {
      // TODO: remove this once the old parser is out of the question
      // TODO: this is only here to support tests from the old parser
      // printing extra spaces around an assignment operator
      os_ << " ";
      // Function Name
      os_ << f->name().str();
      os_ << " ";
    } else {
      // Function Name
      os_ << f->name().str();
    }

    // Formals
    int numThisFormal = f->thisFormal() ? 1 : 0;
    int nFormals = f->numFormals() - numThisFormal;
    if (nFormals == 0 && f->isParenless()) {
      // pass
    } else if (nFormals == 0) {
      os_ << "()";
    } else {
      auto it = f->formals();
      interpose(it.begin() + numThisFormal, it.end(), ", ", "(", ")");
    }

    // Return Intent
    if (f->returnIntent() != Function::ReturnIntent::DEFAULT_RETURN_INTENT &&
        f->returnIntent() != Function::ReturnIntent::CONST) {
      os_ << " " << kindToString((Qualifier) f->returnIntent());
    }

    // Return type
    if (const AstNode* e = f->returnType()) {
      os_ << ": ";
      printingType_ = true;
      e->traverse(*this);
      printingType_ = false;
    }

    // throws
    if (f->throws()) os_ << " throws";

    return false;
  }

  bool enter(const Identifier* r) {
    os_ << r->name().c_str();
    return false;
  }

  bool enter(const Import* node) {
    node->stringify(os_, StringifyKind::CHPL_SYNTAX);
    return false;
  }

  bool enter(const New* d) {
    os_ << "new ";
    if (d->management() != New::Management::DEFAULT_MANAGEMENT) {
      os_ << kindToString(d->management()) << " ";
    }
    d->typeExpression()->traverse(*this);
    return false;
  }

  /* Numeric Literals */
  bool enter(const IntLiteral* l)  { return enterLiteral(l); }
  bool enter(const UintLiteral* l) { return enterLiteral(l); }
  bool enter(const RealLiteral* l) { return enterLiteral(l); }
  bool enter(const ImagLiteral* l) { return enterLiteral(l); }

  bool enter(const OpCall* node) {
    if (node->isUnaryOp()) {
      printUnaryOp(node);
    } else if (node->isBinaryOp()) {
      printBinaryOp(node);
    }
    return false;
  }

  bool enter(const Range* range) {
    if (auto lb = range->lowerBound()) {
      lb->traverse(*this);
    }
    os_ << "..";
    if (auto ub = range->upperBound()) {
      if (range->opKind() == Range::OPEN_HIGH) {
        os_ << "<";
      }
      ub->traverse(*this);
    }
    return false;
  }

  bool enter(const Record* r) {
    // TODO: Shouldn't this be record, not Record?
    if (textOnly_) os_ << "Record: ";
    os_ << r->name().c_str();
    return false;
  }

  bool enter(const StringLiteral* l) {
    os_ << '"' << escapeStringC(l->value().str()) << '"';
    return false;
  }

  bool enter(const Tuple* tup) {
    os_ << "(";
    bool first = true;
    for (auto it = tup->children().begin(); it != tup->children().end(); it++) {
      if (!first) os_ << ", ";
      (*it)->traverse(*this);
      first = false;
    }
    if (tup->numChildren() == 1) {
      os_ << ",";
    }
    os_ << ")";
    return false;
  }

  bool enter(const TypeQuery* node) {
    node->stringify(os_, StringifyKind::CHPL_SYNTAX);
    return false;
  }

  bool enter(const Use* node) {
    node->stringify(os_, StringifyKind::CHPL_SYNTAX);
    return false;
  }

  bool enter(const VarArgFormal* node) {
    node->stringify(os_, StringifyKind::CHPL_SYNTAX);
    return false;
  }

  bool enter(const Variable* v) {
    if (v->isConfig()) {
      os_ << "config ";
    }
    if (v->kind() != Variable::Kind::INDEX) {
      os_ << kindToString((Qualifier) v->kind()) << " ";
    }
    os_ << v->name().c_str();
    if (const AstNode* te = v->typeExpression()) {
      printingType_ = true;
      os_ << ": ";
      te->traverse(*this);
      printingType_ = false;
    }
    if (const AstNode* ie = v->initExpression()) {
      os_ << " = ";
      if (v->storageKind() == chpl::uast::Qualifier::TYPE)
        printingType_ = true;
      ie->traverse(*this);
      if (v->storageKind() == chpl::uast::Qualifier::TYPE)
        printingType_ = false;
    }
    return false;
  }

  // Defaults for all other node types not handled above
  bool enter(const AstNode* a) {
    printChapelSyntax(os_, a);
    return false;
  }
  void exit(const AstNode* a) {}
};

/**
 Stores the rst docs result of an AstNode and its children.
 `doc` looks like: (using Python's triple quote syntax)
'''.. function:: proc procName(arg1: int): bool

I am a doc comment'''

 `children` is a vector of non-owning, non-null pointers to its children.

 Another option would be to store the full std::string in rstResult, including
your children. This ends up with a quadratic time and space. Neither is
particularly concerning for this application. Between the two, space is more of
a concern because some doc comments might be quite large-ish. Remember that the
n here is the depth of nesting, which won't be very large and if you concatenate
with a stringstream, that would be plenty fast in time.
 */
struct RstResult {
  std::string doc;
  // The child pointers are non-owning. The query framework owns them.
  std::vector<RstResult*> children;

  bool indentChildren;

  RstResult(std::string doc, std::vector<RstResult*> children,
            bool indentChildren)
      : doc(doc), children(children), indentChildren(indentChildren) {}

  // TODO which one of these do I want?
  static bool update(owned<RstResult>& keep, owned<RstResult>& addin) {
    // return update(*keep, *addin);
    return chpl::defaultUpdateOwned(keep, addin);
  }
  // static bool update(rstResult& keep, rstResult& addin) {
  //   bool changed = false;
  //   changed |= chpl::update<std::string>{}(keep.doc, addin.doc);
  //   changed |= chpl::update<decltype(children)>{}(keep.children, addin.children);
  //   changed |= chpl::update<bool>{}(keep.indentChildren, addin.indentChildren);
  //   return changed;
  // }
  bool operator==(const RstResult& other) {
    return doc == other.doc && children == other.children &&
           indentChildren == other.indentChildren;
  }

  void mark(const Context *c) const {}

  void outputModule(std::string outDir, std::string name, int indentPerDepth) {
    std::string ext = textOnly_ ? ".txt" : ".rst";
    auto outpath = outDir + "/" + name + ext;

    std::error_code err = makeDir(outDir, true);

    if (err != std::error_code()) {
      if (err) {
        std::cerr << "Could not create " << outDir << " because "
                  << err.message() << "\n";
        return;
      }
    }
    std::ofstream ofs = std::ofstream(outpath, std::ios::out);
    output(ofs, indentPerDepth);
  }

  void output(std::ostream& os, int indentPerDepth) {
    output(os, indentPerDepth, 0);
  }

  private: void output(std::ostream& os, int indentPerDepth, int depth) {
    os << indentLines(doc, indentPerDepth * depth);
    if (indentChildren) depth += 1;
    for (auto& child : children) {
      child->output(os, indentPerDepth, depth);
    }
  }
};


static const owned<RstResult>& rstDoc(Context * context, ID id);
static const Comment* const& previousComment(Context* context, ID id);
static const std::vector<UniqueString>& getModulePath(Context* context, ID id);

struct RstResultBuilder {
  Context* context_;
  std::stringstream os_;
  std::vector<RstResult*> children_;
  const std::string commentStyle = commentStyle_;
  static const int commentIndent = 3;
  int indentDepth_ = 1;

  RstResultBuilder(Context* context) : context_(context),
                                       os_(std::stringstream()) {}

  bool showComment(const Comment* comment, std::string& errMsg, bool indent=true) {
    if (!comment || comment->str().substr(0, 2) == "//") {
      os_ << '\n';
      return false;
    }
    int addDepth = textOnly_ ? 1 : 0;
    int indentChars = indent ? (addDepth + indentDepth_) * commentIndent : 0;
    std::vector<std::string> lines;
    errMsg = prettifyComment(comment->str(), commentStyle, lines);
    if (!lines.empty())
      os_ << '\n';
    for (const auto& line : lines) {
      if (line.empty()) {
        // insert blank spaces here to match test output from chpldoc
        // TODO: See if spaces needed for the docs to format correctly
        indentStream(os_, indentChars) << '\n';
      } else {
        indentStream(os_, indentChars) << line << '\n';
      }
    }
    return true;
  }
  bool showComment(const AstNode* node, bool indent=true) {
    std::string errMsg;
    auto lastComment = previousComment(context_, node->id());
    bool commentShown = showComment(lastComment, errMsg, indent);
    if (!errMsg.empty()) {
      // process the warning about comments
      auto br = parseFileContainingIdToBuilderResult(context_, node->id());
      auto loc = br->commentToLocation(lastComment);
      auto err = GeneralError::get(ErrorBase::WARNING, loc, errMsg);
      context_->report(std::move(err));
    }
    // TODO: The presence of these node exceptions means we're probably missing
    //  something from the old implementation
    if (commentShown &&
       ((textOnly_ && !node->isModule() && !node->isVariable()) || !textOnly_)) {
      os_ << "\n";
    }
    return commentShown;
  }

  template<typename T>
  bool show(const std::string& kind, const T* node, bool indentComment=true) {
    if (isNoDoc(node)) return false;

    if (!textOnly_) os_ << ".. " << kind << ":: ";
    RstSignatureVisitor ppv{os_};
    node->traverse(ppv);
    if (!textOnly_) os_ << "\n";
    bool commentShown = showComment(node, indentComment);
    // TODO: Fix all this because why are we checking for specific node types
    //  just to add a newline?
    if (commentShown && !textOnly_ && (node->isEnum() ||
                                       node->isClass() ||
                                       node->isRecord() ||
                                       node->isModule())) {
      os_ << "\n";
    }

    showDeprecationMessage(node, indentComment);
    // TODO: how do deprecation and unstable messages interplay?
    showUnstableWarning(node, indentComment);
    return commentShown;
  }

  void showUnstableWarning(const Decl* node, bool indentComment=true) {
    if (auto attrs = node->attributeGroup()) {
      if (attrs->isUnstable()) {
        int commentShift = 0;
          if (indentComment) {
            indentStream(os_, indentDepth_ * indentPerDepth);
            commentShift = 1;
          }
          os_ << ".. warning::\n\n";
          indentStream(os_, (indentDepth_ + commentShift) * indentPerDepth);
        os_ << strip(attrs->unstableMessage().c_str());
        os_ << "\n\n";
      }
    }
  }

  void showDeprecationMessage(const Decl* node, bool indentComment=true) {
    if (auto attrs = node->attributeGroup()) {
      if (attrs->isDeprecated() && !textOnly_) {
        auto comment = previousComment(context_, node->id());
        if (comment && !comment->str().empty() &&
            comment->str().substr(0, 2) == "/*" &&
            comment->str().find("deprecat") != std::string::npos ) {
            // do nothing because deprecation was mentioned in doc comment
        } else {
          // write the deprecation warning and message
          int commentShift = 0;
          if (indentComment) {
            indentStream(os_, indentDepth_ * indentPerDepth);
            commentShift = 1;
          }
          os_ << ".. warning::\n\n";
          indentStream(os_, (indentDepth_ + commentShift) * indentPerDepth);
          if (attrs->deprecationMessage().isEmpty()) {
            // write a generic message because there wasn't a specific one
            os_ << getNodeName((AstNode*) node) << " is deprecated";
          } else {
            // use the specific deprecation message
            os_ << strip(attrs->deprecationMessage().c_str());
          }
          os_ << "\n\n";
        }
      }
    }
  }

  void visitChildren(const AstNode* n) {
    for (auto child : n->children()) {
      // don't visit child modules as they were gathered earlier
      if (!child->isModule()) {
        if (auto &r = rstDoc(context_, child->id())) {
          children_.push_back(r.get());
        }
      }
    }
  }

  /*
    Helper for multi decls to make sure type and initializer information is
    propagated down through each decl and changes as needed.
  */
  void multiDeclHelper(const Decl* decl, std::string& prevTypeExpression,
                       std::string& prevInitExpression) {
    if (!prevTypeExpression.empty()) {
      // write prevTypeExpression
      os_ << prevTypeExpression;
      // set prevTypeExpression = ": thisVariableName.type"
      prevTypeExpression = ": " + decl->toVariable()->name().str() + ".type";
    }
    if (!prevInitExpression.empty()) {
      // write prevInitExpression
      os_ << prevInitExpression;
      // set prevInitExpression  = "= thisVariableName"
      prevInitExpression = " = " + decl->toVariable()->name().str();
    }
  }

  owned<RstResult> visit(const Class* c) {
    if (isNoDoc(c) || c->visibility() == chpl::uast::Decl::PRIVATE) return {};
    if (textOnly_) os_ << "Class: ";
    show("class", c);
    visitChildren(c);
    return getResult(true);
  }

  owned<RstResult> visit(const Enum* e) {
    if (isNoDoc(e)) return {};
    show("enum", e);
    return getResult();
  }

  owned<RstResult> visit(const Function* f) {
    if (f->visibility() == Decl::Visibility::PRIVATE || isNoDoc(f))
      return {};
    bool doIndent = f->isMethod() && f->isPrimaryMethod();
    if (doIndent) indentDepth_ ++;
    show(kindToRstString(f->isMethod(), f->kind()), f);
    if (doIndent) indentDepth_ --;
    return getResult();
  }

  owned<RstResult> visit(const Module* m) {
    bool includedByDefault = false;

    // see if we have any included modules we should add a submodule toctree for
    bool hasIncludes = false;
    for (auto stmt : m->stmts()) {
      if (auto inc = stmt->toInclude()) {
        auto incMod = getIncludedSubmodule(context_, inc->id());
        if (!isNoDoc(incMod)) {
          hasIncludes = true;
          break;
        }
      }
    }
    // lookup the module path
    std::vector<UniqueString> modulePath = getModulePath(context_, m->id());
    std::string moduleName;
    // build up the module path string using `.` to separate the components
    for (auto p : modulePath) {
      moduleName += p.str();
      if (p != modulePath.back()) {
        moduleName += ".";
      }
    }
    assert(!moduleName.empty());
    const Comment* lastComment = nullptr;
    if (auto attrs = m->attributeGroup()) {
      if (attrs->hasPragma(pragmatags::PRAGMA_MODULE_INCLUDED_BY_DEFAULT)) {
        includedByDefault = true;
      }
    }
    // header
    if (!textOnly_) {
      os_ << ".. default-domain:: chpl\n\n";
      // This is hard coded in chpldoc to recognize ChapelSysCTypes like this
      //  the comment indicates it prevents sphinx from complaining about
      //  duplicate definitions.
      if (moduleName == "ChapelSysCTypes") {
        visitChildren(m);
        return getResult(textOnly_);
      }
        os_ << ".. module:: " << m->name().c_str() << '\n';
      // Don't index internal modules since that will make them show up
      // in the module index (chpl-modindex.html).  This has the side
      // effect of making references to the :mod: tag for the module
      // illegal, which is appropriate since the modules are not
      // user-facing.
      if (idIsInInternalModule(context_, m->id())) {
        os_ << "   :noindex:" << std::endl;
      } else {
        lastComment = previousComment(context_, m->id());
        if (lastComment) {
          std::string errMsg;
          auto synopsis = commentSynopsis(lastComment, commentStyle, errMsg);
          if (!errMsg.empty()) {
            // process the warning about comments
            auto br = parseFileContainingIdToBuilderResult(context_, m->id());
            auto loc = br->commentToLocation(lastComment);
            auto err = GeneralError::get(GeneralError::WARNING, loc, errMsg);
            context_->report(std::move(err));
          }
          if (!synopsis.empty()) {
            indentStream(os_, 1 * indentPerDepth);
            os_ << ":synopsis: " << synopsis
                << '\n';
          }
        }
      }
        os_ << '\n';

        // module title
        os_ << m->name().c_str() << "\n";
        os_ << std::string(m->name().length(), '=') << "\n";

        // usage
        if (includedByDefault) {
          os_ << ".. note::" << std::endl << std::endl;
          indentStream(os_, 1 * indentPerDepth);
          os_ <<
                "All Chapel programs automatically ``use`` this module by default.";
          os_ << std::endl;
          indentStream(os_, 1 * indentPerDepth);
          os_ << "An explicit ``use`` statement is not necessary.";
          os_ << std::endl;
        } else {
          os_ << templateReplace(templateUsage, "MODULE", moduleName) << "\n";
        }

      } else {
        os_ << m->name().c_str();
        os_ << templateReplace(textOnlyTemplateUsage, "MODULE", moduleName) << "\n";
        lastComment = previousComment(context_, m->id());
      }

    if (hasSubmodule(m) || hasIncludes) {
      moduleName = m->name().c_str();
      if (!textOnly_) {
        os_ << "\n";
        os_ << "**Submodules**" << std::endl << std::endl;

        os_ << ".. toctree::" << std::endl;
        indentStream(os_, 1 * indentPerDepth);

        os_ << ":maxdepth: 1" << std::endl;
        indentStream(os_, 1 * indentPerDepth);

        os_ << ":glob:" << std::endl << std::endl;
        indentStream(os_, 1 * indentPerDepth);

        os_ << moduleName << "/*" << std::endl;
      } else {
        os_ << "\nSubmodules for this module are located in the ";
        os_ << moduleName << "/ directory" << std::endl;
      }
    }
    if (textOnly_) indentDepth_ --;
    showComment(m, textOnly_);
    showDeprecationMessage(m, false);
    // TODO: Are we not printing these for modules?
    // showUnstableWarning(m, false);
    if (textOnly_) indentDepth_ ++;

    visitChildren(m);

    return getResult(textOnly_);
  }

  owned<RstResult> visit(const MultiDecl* md) {
    if (isNoDoc(md)) return {};

    std::string kind;
    std::queue<const AstNode*> expressions;

    for (auto decl : md->decls()) {
      if (decl->toVariable()->typeExpression() ||
          decl->toVariable()->initExpression()) {
        expressions.push(decl);
      }
      kind = decl->toVariable()->isField() ? "attribute" : "data";
    }
    std::string prevTypeExpression;
    std::string prevInitExpression;
    for (auto decl : md->decls()) {
      if (kind=="attribute" && decl != md->decls().begin()->toDecl()) {
        indentStream(os_, 1 * indentPerDepth);
      }
      // write kind
      if (!textOnly_) os_ << ".. " << kind << ":: ";
      if (decl->toVariable()->kind() != Variable::Kind::INDEX) {
        os_ << kindToString((Qualifier) decl->toVariable()->kind()) << " ";
      }
      // write name
      os_ << decl->toVariable()->name();

      if (expressions.empty()) {
        multiDeclHelper(decl, prevTypeExpression, prevInitExpression);
        // for when we have something like var x, y, z;
        // if we want to print the following, uncomment the line below
        // var x
        // var y: x.type
        // var z: y.type
        // prevTypeExpression = ": " + decl->toVariable()->name().str() + ".type";
      } else {
        // our decl doesn't have a typeExpression or initExpression,
        // or it is the same one
        // is previousInit or previousType set?
        if (!prevInitExpression.empty() || !prevTypeExpression.empty()) {
          multiDeclHelper(decl, prevTypeExpression, prevInitExpression);
        } else { // use the expression in the queue
          // take the one from the front of the expressions queue
          if (auto te = expressions.front()->toVariable()->typeExpression()) {
            //write out type expression->stringify
            os_ << ": ";
            te->stringify(os_, CHPL_SYNTAX);
            //set previous type expression = ": variableName.type"
            prevTypeExpression =
                ": " + decl->toVariable()->name().str() + ".type";
          }
          if (auto exp = expressions.front()->toVariable()->initExpression()) {
            // write out initExpression->stringify
            os_ << " = ";
            exp->stringify(os_, CHPL_SYNTAX);
            // set previous init expression = "= variableName"
            prevInitExpression = " = " + decl->toVariable()->name().str();
          }
        }
        if (decl == expressions.front()) {
          // our decl is the same as the first one in the queue, so we should
          // pop it and clear the previousInit and previousType
          expressions.pop();
          prevTypeExpression.clear();
          prevInitExpression.clear();
        }
      }
      showComment(md, true);

      if (auto attrs = md->attributeGroup()) {
        if (attrs->isDeprecated() && !textOnly_) {
          indentStream(os_, 1 * indentPerDepth) << ".. warning::\n";
          indentStream(os_, 2 * indentPerDepth) << attrs->deprecationMessage();
          os_ << "\n\n";
        }
      }
      os_ << "\n";
    }
    return getResult();
  }

  owned<RstResult> visit(const Record* r) {
    if (isNoDoc(r)) return {};
    show("record", r);
    visitChildren(r);
    return getResult(true);
  }

  owned<RstResult> visit(const Variable* v) {
    if (v->visibility() == Decl::Visibility::PRIVATE || isNoDoc(v))
      return {};

    if (v->isField()) {
      indentDepth_ ++;
      show("attribute", v);
      indentDepth_ --;
    }
    else if (v->storageKind() == Qualifier::TYPE)
      show("type", v);
    else
      show("data", v);

    return getResult();
  }

  // TODO all these nullptr gets stored in the query map... can we avoid that?
  owned<RstResult> visit(const AstNode* n) { return {}; }

  owned<RstResult> getResult(bool indentChildren = false) {
    return std::make_unique<RstResult>(os_.str(), children_, indentChildren);
  }
};


struct GatherModulesVisitor {
  std::set<ID> modules;
  Context* context_;
  GatherModulesVisitor(Context* context) {
      context_ = context;
  }

  void handleUseOrImport(const AstNode* node) {
    if (processUsedModules_) {
      auto scope = resolution::scopeForId(context_, node->id());
      auto used = resolution::findUsedImportedModules(context_, scope);
      for (auto id: used) {
        if (idIsInBundledModule(context_, id)) {
          continue;
        }
        // only add it and visit its children if we haven't seen it already
        if (modules.find(id) == modules.end()) {
          modules.insert(id);
          auto ast = idToAst(context_, id);
          ast->traverse(*this);
        }
      }
    }
  }

  bool enter(const Module* m) {
    if (m->visibility() == Decl::Visibility::PRIVATE || isNoDoc(m)) {
      return false;
    }
    modules.insert(m->id());
    return true;
  }

  // will handle a use or import multiple times in the case that a module has
  // multiple use statements.
  // every time handleUseOrImport is called, it will try to add the module
  // to the set
  void exit(const Use* node) {
    handleUseOrImport(node);
  }

  void exit(const Import* node) {
    handleUseOrImport(node);
  }

  void exit(const Include* node) {
    if (auto mod = getIncludedSubmodule(context_, node->id())) {
      if (!isNoDoc(mod)) {
        modules.insert(mod->id());
      }
    }
  }

  bool enter(const AttributeGroup* node ) {
    checkKnownAttributes(node);
    return false;
  }

  bool enter(const AstNode* n) {
    return true;
  }

  void exit(const AstNode* n) {
    // do nothing
  }

};


/**
 Visitor that collects a mapping from ID -> comment
 Note that we only store this info for comments that we think we might
 need in the future (since not all nodes get doc comments)
 This is more efficient than answering a query like getPreviousComment, which
 would have to goto parent, linear search, and check if previousSibling is a
 Comment
*/
struct CommentVisitor {
  CommentMap& map;
  const Comment* lastComment_ = nullptr;

  void put(const AstNode* n) {
    if (lastComment_) {
      // check that we didn't already use this comment, which can happen
      // if a module has a comment and it's first function does not,
      // in that case we improperly apply the comment to both the module
      // and the function.
      for (const auto& mapEntry : map) {
        if (mapEntry.second == lastComment_)
          return;
      }
      map.emplace(n->id(), lastComment_);
    }
  }

  bool enter(const Comment* comment) {
    lastComment_ = comment;
    return false;
  }
  void exit(const Comment* ast) {}
  void exit(const AstNode* ast) { lastComment_ = nullptr; }

#define DEF_ENTER(type, recurse)                                               \
  bool enter(const type* node) {                                               \
    put(node);                                                                 \
    return recurse;                                                            \
  }

  DEF_ENTER(Module, true)
  DEF_ENTER(TypeDecl, true)
  DEF_ENTER(Function, false)
  DEF_ENTER(Variable, false)

#undef DEF_ENTER

  bool enter(const AstNode* ast) {
    return false;
  }
};

/**
 Get a mapping from ID -> doc comment for an entire file. This mapping is only
 populated for IDs that can have a doc comment. We work at a file level here
 because the doc comment for a Module is its previous sibling
 */
static const CommentMap&
commentMap(Context* context, ID id) {

  // TODO:
  //  redundant work done if we read through a parent module and read its submodule symbols
  //  then later we read the submodule symbols without the parent as well
  // Possible solution -> write into helper function that calls a query (commentMapQuery) and (commentMap)
  // ID has symbol path, so we can compute the parent symbol from there by getting it from ID function to split path
  // if it is !empty, compute all the parent ids, loop over from rootest to leafest
  // builderResultForID (is the id we care about is IN the builder result)
  // if it is in the BR, use the comment map associated with the BR
  // otherwise call commentMap to generate a new one
  QUERY_BEGIN(commentMap, context, id);
  CommentMap result;
  const auto& builderResult = parseFileContainingIdToBuilderResult(context,
                                                       id);

  CommentVisitor cv{result};
  for (const chpl::uast::AstNode* ast : builderResult->topLevelExpressions()) {
    /* note the use of the above pattern rather than `const auto& ast:`
    was motivated by a compiler error from chapelmac during smoketests
    that complained about the pattern and suggested this replacement,
    which it seems satisfied with.
    */
    ast->traverse(cv);
  }

  return QUERY_END(result);
}

/**
 This is a "projection query" of commentMap to get the doc comment for an id.
 This insulates a specific id from irrelevant changes.
 */
static const Comment* const& previousComment(Context* context, ID id) {
  QUERY_BEGIN(previousComment, context, id);
  const Comment* result = nullptr;
  UniqueString modPath;
  UniqueString parentPath;
  assert(context->filePathForId(id, modPath, parentPath));
  const auto& map = commentMap(context, id);
  auto it = map.find(id);
  if (it != map.end()) {
    result = it->second;
  }
  return QUERY_END(result);
}

static const owned<RstResult>& rstDoc(Context* context, ID id) {
  QUERY_BEGIN(rstDoc, context, id);
  owned<RstResult> result;
  // Comments have empty id
  if (!id.isEmpty()) {
    const AstNode* node = idToAst(context, id);
    RstResultBuilder cqv{context};
    result = node->dispatch<owned<RstResult>>(cqv);
  }

  return QUERY_END(result);
}


/*
  Query to get the module path for a given id. Path may be just the name of
  the module in the case of a top-level module, or it may contain the parent(s)
  of the module if it is a submodule.
*/
static const std::vector<UniqueString>& getModulePath(Context* context, ID id) {
  QUERY_BEGIN(getModulePath, context, id);
  std::vector<UniqueString> result;
  if (!id.isEmpty()) {
    ID parentID = idToParentModule(context, id);
    while (!parentID.isEmpty()) {
      // a submodule
      UniqueString modulePath = parentID.symbolName(context);
      // place parent names in the front
      result.insert(result.begin(), modulePath);
      // see if we have another parent
      parentID = idToParentModule(context, parentID);
    }
    // a top level module
    result.push_back(id.symbolName(context));
  }
  return QUERY_END(result);
}

const std::string testString = R"RAW(
/*
  comment 1
  This thing is super long

*/
module M {
  // comment 2
  var x = 10;

  //
  const y = doSomething(10, x);

  // comment 2.5
  proc foo(x : int) { return 10; }


  // random comment

  // comment for an enum
  enum myEnum {a=1, b, c=32};

  // comment for a param
  param paramX = 10;

  // comment for a param with type
  param paramY:int = 10;

  // split init const
  const splitInit;

  splitInit = 10;

  // comment for a config const
  config const configConstY = 10;

  // comment for a type
  type t = int;

  // comment for record
  record R {
    // comment for a type
    type t;

    // comment for a constant
    const myConst = 100.1;

    // this operator
    operator +(x:R) {return new R();}

    // comment for a method
    proc foo(a:real, b:int=37) { return 10; }

    // paren-less proc
    proc bar: int { return 10; }
  }

  proc R.secondaryMethod() const { return 42; }

  private proc privateProc { return 37; }

  pragma "no doc"
  proc procNoDoc { return 42; }

  // got a comment for ya
  deprecated "yo this thing is deprecated"
  proc oldNews { return 327; }

  var x : [1..3] int = [1, 2, 3];
}

pragma "no doc"
module N { }
/* comment 4 */;
)RAW";

  // /* nested module comment */
  // module NestedModule {
  //   // nested var comment
  //   var nestedX = 22;
  // }

// Command line options and some defaults for dyno-chpldoc
struct Args {
  std::string saveSphinx = "";
  bool textOnly = false;
  std::string outputDir;
  bool processUsedModules = false;
  std::string author;
  std::string commentStyle =  "/*";
  std::string projectVersion = "0.0.1";
  std::vector<std::string> files;
  bool printSystemCommands = false;
  bool noHTML = false;
};

static Args parseArgs(int argc, char **argv, void* mainAddr) {
  Args ret;
  init_args(&sArgState, argv[0], mainAddr);
  init_arg_desc(&sArgState, docs_arg_desc);
  process_args(&sArgState, argc, argv);
  ret.author = std::string(fDocsAuthor);
  if (fDocsCommentLabel[0] != '\0') {
    ret.commentStyle = std::string(fDocsCommentLabel);
  }
  ret.outputDir = std::string(fDocsFolder);
  ret.processUsedModules = fDocsProcessUsedModules;
  ret.textOnly = fDocsTextOnly;
  ret.saveSphinx = std::string(fDocsSphinxDir);
  ret.printSystemCommands = printSystemCommands;
  ret.projectVersion = checkProjectVersion(fDocsProjectVersion);
  ret.noHTML = !fDocsHTML;
  // add source files
  // TODO: Check for proper file type, duplicate file names, was file found, etc.
  for (int i = 0; i < sArgState.nfile_arguments; i++) {
    ret.files.push_back(std::string(sArgState.file_argument[i]));
  }
  return ret;
}


/*
 * Create new sphinx project at given location and return path where .rst files
 * should be placed.
 */
static
std::string generateSphinxProject(std::string dirpath, bool printSystemCommands) {
  // Create the output dir under the docs output dir.
  std::string sphinxDir = dirpath;
  // Copy the sphinx template into the output dir.
  std::string sphinxTemplate = CHPL_HOME +
                               "/third-party/chpl-venv/chpldoc-sphinx-project/*";
  std::string cmd = "cp -r " + sphinxTemplate + " " + sphinxDir + "/";
  if( printSystemCommands ) {
    printf("%s\n", cmd.c_str());
  }
  myshell(cmd, "copying chpldoc sphinx template", false, false,
          printSystemCommands);

  std::string moddir = sphinxDir + "/source/modules";
  return moddir;
}

/*
 * Invoke sphinx-build using sphinxDir to find conf.py and rst sources, and
 * outputDir for generated html files.
 */
static
void generateSphinxOutput(std::string sphinxDir, std::string outputDir,
                          std::string projectVersion, std::string author,
                          bool printSystemCommands) {
  std::string sphinxBuild = "python3 " + getChplDepsApp() + " sphinx-build";
  std::string venvProjectVersion = projectVersion;

  std::string envVars = "export CHPLDOC_AUTHOR='" + author + "' && " +
                        "export CHPLDOC_PROJECT_VERSION='"
                        + venvProjectVersion + "'";

  // Run:
  //   $envVars &&
  //     sphinx-build -b html
  //     -d $sphinxDir/build/doctrees -W
  //     $sphinxDir/source $outputDir
  std::string cmdPrefix = envVars + " && ";
  std::string cmd = cmdPrefix + sphinxBuild + " -b html -d " +
                    sphinxDir + "/build/doctrees" + " -W " +
                    sphinxDir + "/source " + outputDir;
  if( printSystemCommands ) {
    printf("%s\n", cmd.c_str());
  }
  if (myshell(cmd, "building html output from chpldoc sphinx project") == 0) {
    printf("HTML files are at: %s\n", outputDir.c_str());
  }
}

class ChpldocErrorHandler : public Context::ErrorHandler {
 private:
  std::vector<owned<ErrorBase>> reportedErrors;

 public:
  void report(Context* context, const ErrorBase* error) override {
    reportedErrors.push_back(error->clone());
  }

  size_t numErrors() const {
    return reportedErrors.size();
  }

  void printAndExitIfError(Context* context) {
    // TODO: handle errors better, don't rely on parse query to emit them
    // per @mppf: if dyno-chpldoc wants to quit on an error, it should
    // configure Context::reportError to have a custom function that does so.
    bool fatal = false;
    for (auto& e : reportedErrors) {
    // just display the error messages right now, see TODO above
      if (e->kind() == ErrorBase::Kind::ERROR ||
          e->kind() == ErrorBase::Kind::SYNTAX) {
            fatal = true;
      }
      Context::defaultReportError(context, e.get());
    }
    if (fatal) {
      clean_exit(1);
    }
    reportedErrors.clear();
  }
};

int main(int argc, char** argv) {
  Args args = parseArgs(argc, argv, (void*)main);
  std::string warningMsg;
  bool foundEnv = false;
  bool installed = false;
  // if user overrides CHPL_HOME from command line, don't go looking for trouble
  if (CHPL_HOME.empty()) {
    std::error_code err = findChplHome(argv[0], (void*)main, CHPL_HOME, installed, foundEnv, warningMsg);
    if (!warningMsg.empty()) {
      fprintf(stderr, "%s\n", warningMsg.c_str());
    }
    if (err) {
      fprintf(stderr, "CHPL_HOME not set to a valid value. Please set CHPL_HOME or pass a value "
                      "using the --home option\n" );
      clean_exit(1);
    }
  }

  // TODO: there is a future for this, asking for a better error message and I
  // think we can provide it by checking here.
  // see test/chpldoc/compflags/folder/save-sphinx/saveSphinxInDocs.doc.future
  // if (args.saveSphinx == "docs") { }

  textOnly_ = args.textOnly;
  if (args.commentStyle.substr(0,2) != "/*") {
    std::cerr << "error: comment label should start with /*" << std::endl;
    return 1;
  }
  commentStyle_ = args.commentStyle;
  processUsedModules_ = args.processUsedModules;


  Context::Configuration config;
  config.chplHome = CHPL_HOME;
  config.toolName = "chpldoc";
  Context context(config);
  gContext = &context;
  auto erroHandler = new ChpldocErrorHandler(); // wraped in owned on next line
  gContext->installErrorHandler(owned<Context::ErrorHandler>(erroHandler));
  gContext->setDetailedErrorOutput(false);
  auto chplEnv = gContext->getChplEnv();
  assert(!chplEnv.getError() && "not handling chplenv errors yet");
  // set any attribute toolnames we processed earlier and clear the local list.
  promoteAttributeToolNameStrToUniqueString();
  chpl::parsing::setAttributeToolNames(gContext, usingAttributeToolNames);
  usingAttributeToolNames.clear();
  chpl::CompilerFlags flags;
  flags.set(chpl::CompilerFlags::WARN_UNKNOWN_TOOL_SPACED_ATTRS,
            fWarnUnknownAttributeToolname);
  // Set the compilation flags all at once using a query.
  chpl::setCompilerFlags(gContext, flags);
  // This is the final location for the output format (e.g. the html files.).
  std::string docsOutputDir;
  if (args.outputDir.length() > 0) {
    docsOutputDir = args.outputDir;
  } else {
    docsOutputDir = getCwd() + "/docs";
  }

  // Root of the sphinx project and generated rst files. If
  // --docs-save-sphinx is not specified, it will be a temp dir.
  std::string docsSphinxDir;
  if (!args.saveSphinx.empty()) {
    docsSphinxDir = args.saveSphinx;
  } else {
    docsSphinxDir = gContext->tmpDir();
  }

  // Make the intermediate dir and output dir.
   if (auto err = makeDir(docsSphinxDir)) {
      std::cerr << "error: Failed to create directory: "
                << docsSphinxDir << " due to: "
                << err.message() << std::endl;
      return 1;
   }
   if (auto err = makeDir(docsOutputDir)) {
      std::cerr << "error: Failed to create directory: "
                << docsOutputDir << " due to: "
                << err.message() << std::endl;
      return 1;
   }

  // The location of intermediate rst files.
  std::string docsRstDir;
  if (textOnly_) {
    // For text-only mode, the output and working location is the same.
    docsRstDir = docsOutputDir;
  } else {
    // For rst mode, the working location is somewhere inside the temp dir.
    docsRstDir = generateSphinxProject(docsSphinxDir, args.printSystemCommands);
  }

  outputDir_ = docsRstDir;

  std::string modRoot = CHPL_HOME + "/modules";
  std::string internal = modRoot + "/internal";
  std::string bundled = modRoot + "/";

  // CHPL_MODULE_PATH isn't always in the output; check if it's there.
  auto it = chplEnv->find("CHPL_MODULE_PATH");
  auto chplModulePath = (it != chplEnv->end()) ? it->second : "";
  setupModuleSearchPaths(gContext,
                         CHPL_HOME,
                         false, //minimal modules
                         chplEnv->at("CHPL_LOCALE_MODEL"),
                         false, //task tracking
                         chplEnv->at("CHPL_TASKS"),
                         chplEnv->at("CHPL_COMM"),
                         chplEnv->at("CHPL_SYS_MODULES_SUBDIR"),
                         chplModulePath,
                         {}, //prependInternalModulePaths,
                         {}, //prependStandardModulePaths,
                         {}, //cmdLinePaths
                         args.files);
  GatherModulesVisitor gather(gContext);
  printStuff(argv[0], (void*)main);
  // evaluate all the files and gather the modules
  for (auto cpath : args.files) {
    UniqueString path = UniqueString::get(gContext, cpath);
    UniqueString emptyParent;

    std::vector<UniqueString> paths;
    size_t location = cpath.rfind("/");
    paths.push_back(UniqueString::get(gContext,"./"));
    if (location != std::string::npos) {
      paths.push_back(UniqueString::get(gContext, cpath.substr(0, location + 1)));
    }
    setModuleSearchPath(gContext, paths);

    // TODO: Change which query we use to parse files as suggested by @mppf
    // parseFileContainingIdToBuilderResult(Context* context, ID id);
    // and then work with the module ID to find the preceding comment.
    const BuilderResult& builderResult =
      parseFileToBuilderResultAndCheck(gContext, path, emptyParent);

    if (erroHandler->numErrors() > 0) {
      erroHandler->printAndExitIfError(gContext);
    }
    // gather all the top level and used/imported/included module IDs
    for (const chpl::uast::AstNode* ast : builderResult.topLevelExpressions()) {
      /* note the use of the above pattern rather than `const auto& ast:`
          was motivated by a compiler error from chapelmac during smoketests
          that complained about the pattern and suggested this replacement,
          which it seems satisfied with.
      */
      ast->traverse(gather);
    }
  }

  for (auto id : gather.modules) {
    if (auto& r = rstDoc(gContext, id)) {
        // given a module ID we can get the path to the file that we parsed
        UniqueString filePath;
        UniqueString parentSymbol;
        gContext->filePathForId(id, filePath, parentSymbol);
        std::string moduleName = id.symbolName(gContext).str();
        std::string parentPath;
        auto pathVec = id.expandSymbolPath(gContext, id.symbolPath());
        // remove last entry
        pathVec.pop_back();
        for (auto path : pathVec) {
          for (int i = 0; i <= path.second; i++) {
            if (path.first != id.symbolName(gContext)) {
              parentPath += unescapeStringId(path.first.str()) + "/";
            }
          }
        }
        std::string docsWorkingDir_ = filenameFromModuleName(filePath.c_str(), outputDir_);
        std::string outdir = docsWorkingDir_;
        // TODO: This is an ugly hack to handle included module paths
        if (parentSymbol.isEmpty()) {
          outdir += "/" + parentPath;
        }

        // need to check for a parent module in the path and add it to the directory structure if it exists
        r->outputModule(outdir, moduleName, indentPerDepth);
     }
   }

  // chpldoc-specific warnings could've been issued, make sure they're printed.
  erroHandler->printAndExitIfError(gContext);

  if (!textOnly_ && !args.noHTML) {
    generateSphinxOutput(docsSphinxDir, docsOutputDir,args.projectVersion,
                         args.author, args.printSystemCommands);
  }

  return 0;
}


