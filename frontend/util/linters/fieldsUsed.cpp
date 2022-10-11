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

// based on clang-tools-extra/tool-template/ToolTemplate.cpp

// Testing with clang 13

// This is a WIP rewrite of the python next/util/lint
// program. Currently its a mixed success, many things are much better
// working on the AST directly

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Execution.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/Refactoring/AtomicChange.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Signals.h"
#include <clang/AST/APValue.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/Expr.h>
#include <clang/AST/Type.h>
#include <cstdint>
#include <llvm/ADT/StringRef.h>
#include <unordered_map>
#include <unordered_set>

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

// Have to get information out after matching b/c the lifetime of the asts is
// over by the time we go to report errors (since we have to visit everything
// before figuring out what we haven't seen
struct PrintInfo {
  std::string name, location;
};

using FieldDeclSet = std::unordered_set<const FieldDecl*>;
using MethodFieldUseMap =
    std::unordered_map<const CXXMethodDecl*, FieldDeclSet>;
using PtrPrintInfoMap = std::unordered_map<const void*, PrintInfo>;

// Create a set of type `Out` with each element in `range` where the predicate
// `f` is true
template <typename Out, typename Range, typename F>
static auto filterIntoSet(F f, const Range& range) {
  Out ret;
  for (const auto& x : range) {
    if (f(x)) {
      ret.insert(x);
    }
  }
  return ret;
}

//
// This class records every use of a field of `this` (both implicit
// and explicit) in a method
//
// On the first time seeing a method, we populate a set of all fields
// of `this` after filtering them by `filterFields`
//
// Each use then removes the corresponding field from the set
//
// We have to return results by string b/c the AST lifetime is over by
// the time we've finished doing our matching. This is inline with how
// the other tools report results, but is still not ideal. I wonder if
// this would be better as an explicit AST visitor, but I do like the
// matchers, so maybe a combination could work in the future
//
template <typename F>
class MemberExprsCallback : public MatchFinder::MatchCallback {
 public:
  MemberExprsCallback(PtrPrintInfoMap& infoMap, MethodFieldUseMap& useMap,
                      F filterFields)
      : infoMap_(infoMap), methodFieldsUsed_(useMap),
        filterFields_(filterFields) {}

  void run(const MatchFinder::MatchResult& result) override {
    const auto* member = result.Nodes.getNodeAs<MemberExpr>("member");
    const auto* method = result.Nodes.getNodeAs<CXXMethodDecl>("method");
    const auto& sm = method->getASTContext().getSourceManager();

    // First time we see this method, populate the use map with all `this`
    // fields
    auto it = methodFieldsUsed_.find(method);
    if (it == methodFieldsUsed_.end()) {
      recordMethod(sm, method);
      const CXXRecordDecl* record = method->getParent();
      it = methodFieldsUsed_.emplace_hint(
          it, method,
          filterIntoSet<FieldDeclSet>(filterFields_, record->fields()));
      // Capture info on all fields in our set
      for (auto field : it->second)
        recordField(sm, field);
    }
    auto& fieldsNotUsedYet = it->second;

    // a MemberDecl is either a Field, static member, method or enum constant
    // we only care about a field
    if (const FieldDecl* field = dyn_cast<FieldDecl>(member->getMemberDecl())) {
      fieldsNotUsedYet.erase(field);
      recordField(sm, field);
    }
  }

 private:
  void recordField(const clang::SourceManager& sm, const FieldDecl* field) {
    auto it = infoMap_.find(field);
    if (it == infoMap_.end()) {
      infoMap_.emplace_hint(it, field,
                            PrintInfo{field->getNameAsString(),
                                      field->getBeginLoc().printToString(sm)});
    }
  }
  void recordMethod(const clang::SourceManager& sm,
                    const CXXMethodDecl* method) {
    auto it = infoMap_.find(method);
    if (it == infoMap_.end()) {
      infoMap_.emplace_hint(it, method,
                            PrintInfo{method->getQualifiedNameAsString(),
                                      method->getBeginLoc().printToString(sm)});
    }
  }
  // We store the name and location for each ptr we have in methodFieldsUsed_
  PtrPrintInfoMap& infoMap_;
  // For each method we've seen, this tracks the set of `this` fields not yet
  // used So at the end of processing, anything left in this map is unused
  MethodFieldUseMap& methodFieldsUsed_;
  // Selects which fields of a type are required to be used
  F filterFields_;
};

static auto makeMemberExprsMatcher(llvm::StringRef pattern) {
  return namespaceDecl(
      hasName("chpl"),
      forEachDescendant(
          cxxMethodDecl(
              matchesName(pattern),
              forEachDescendant(memberExpr(hasObjectExpression(cxxThisExpr()))
                                    .bind("member")))
              .bind("method")));
}

// Run MemberExprsCallback over functions which match `pattern`.
// Require uses of all fields of `this` that meet the `requireField`
// filter function
// F: const FieldDecl* -> bool
template <typename F>
static size_t runMemberExprsMatcher(clang::tooling::ToolExecutor* ex,
                                    llvm::StringRef pattern, F requireField) {
  ast_matchers::MatchFinder finder;
  PtrPrintInfoMap infoMap;
  MethodFieldUseMap useMap;
  auto callback = MemberExprsCallback<F>(infoMap, useMap, requireField);

  // This isn't the best query, since if we pass multiple TUs on the
  // command line, I think we'll revisit functions, but having the
  // query be on the leaves is a) convenient and b) driving a nested
  // matcher is still something I'm not sure how to do
  // TODO make the matchesName an argument
  finder.addMatcher(makeMemberExprsMatcher(pattern), &callback);

  auto err = ex->execute(newFrontendActionFactory(&finder));
  if (err) {
    llvm::errs() << llvm::toString(std::move(err)) << "\n";
    return 1;
  }

  // At this point, everything in useMap is a freed pointer, but we just look up
  // the info in the map we've returned. This is my workaround to not finishing
  // understanding the lifetime of AST in clang in one sitting

  size_t errCount = 0;

  for (auto it : useMap) {
    const PrintInfo& methodInfo = infoMap.at(it.first);

    for (const void* fieldNotUsed : it.second) {
      const PrintInfo& fieldInfo = infoMap.at(fieldNotUsed);
      errCount += 1;
      llvm::outs() << methodInfo.location << "\n  " << methodInfo.name
                   << " does not use " << fieldInfo.name << "\n";
    }
  }

  return errCount;
}

static bool allFields(const FieldDecl* fd) {
  (void)fd;
  return true;
}

static bool fieldsForMarkFunction(const FieldDecl* fd) {
  auto t = fd->getType();
  if (t->isBuiltinType() || t->isEnumeralType())
    return false;
  std::string name = fd->getType().getAsString();
  auto re = llvm::Regex("std::string");
  if (re.match(name)) {
    return false;
  }
  return true;
}

// Boilerplate: Set up the command line options
static cl::extrahelp commonHelp(CommonOptionsParser::HelpMessage);
static cl::OptionCategory toolTemplateCategory("tool-template options");

int main(int argc, const char** argv) {
  llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);

  auto executor = clang::tooling::createExecutorFromCommandLineArgs(
      argc, argv, toolTemplateCategory);

  if (!executor) {
    llvm::errs() << llvm::toString(executor.takeError()) << "\n";
    return 1;
  }

  // This is a StandaloneToolExecutor under the hood
  auto ex = executor->get();

  size_t errCount = 0;

  errCount +=
      runMemberExprsMatcher(ex, "update$|swap$|hash$|operator==", allFields);
  errCount += runMemberExprsMatcher(ex, "mark$", fieldsForMarkFunction);

  return errCount > 0;
}
