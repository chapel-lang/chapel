#ifndef CHPL_QUERIES_CONTEXT_H
#define CHPL_QUERIES_CONTEXT_H

#include "chpl/AST/UniqueString.h"
#include "chpl/Queries/ContextDetail.h"
#include "chpl/Util/memory.h"

#include <unordered_map>

namespace chpl {

// Allow ast::UniqueString to be just called UniqueString
// TODO: Should it even be ast::UniqueString at all?
using chpl::ast::UniqueString;

/**
  This class stores the compilation-wide context.
  In particular it handles unique'd strings.
 */
class Context {
 private:
  // map that supports uniqueCString / UniqueString
  std::unordered_map<const char*, const char*, chpl::detail::UniqueStrHash, chpl::detail::UniqueStrEqual> uniqueStringsTable;

  // map from a query name to appropriate QueryMap object.
  // maps to an 'owned' heap-allocated thing to manage having subclasses
  // without slicing.
  std::unordered_map<UniqueString, owned<chpl::querydetail::QueryMapBase>> queryDB;

  struct QueryDepsEntry {
    UniqueString queryName;
    chpl::querydetail::QueryDependencyVec dependencies;
    std::vector<ast::ErrorMessage> errors;
    QueryDepsEntry(UniqueString queryName)
      : queryName(queryName), dependencies(), errors() {
    }
  };

  // this is used to compute the dependencies
  std::vector<QueryDepsEntry> queryDeps;

  chpl::querydetail::RevisionNumber currentRevisionNumber;

  Context();
  const char* getOrCreateUniqueString(const char* s);
  bool queryCanUseSavedResult(chpl::querydetail::QueryMapResultBase* resultEntry);
  void saveDependenciesAndErrorsInParent(chpl::querydetail::QueryMapResultBase* resultEntry);
  void endQueryHandleDependency(chpl::querydetail::QueryMapResultBase* result);

  // Future Work: support marking used strings and garbage collecting the rest
  // Could store an atomic uint_8 just after the string for the mark.

  // Future Work: make the context thread-safe

  // Future Work: allow moving some AST to a different context
  //              (or, at least, that can handle the unique strings)

 public:
  /**
    Create a new AST Context.
   */
  static owned<Context> build();
  ~Context();

  /**
    Get or create a unique string for a NULL-terminated C string
    and return it as a C string. If the passed string is NULL,
    this function will return an empty string.

    Strings returned by this function will always be aligned to 2 bytes.

    The function `UniqueString::build` returns such a string
    with a wrapper type. It should be preferred for type safety
    and to reduce redundant checks.
   */
  const char* uniqueCString(const char* s);


  template<typename ResultType, typename... ArgTs>
  chpl::querydetail::QueryMap<ResultType,ArgTs...>*
    queryBeginGetMap(UniqueString queryName, const std::tuple<ArgTs...>& tupleOfArgs);

  bool queryCanUseSavedResultAndPushIfNot(UniqueString queryName,
      chpl::querydetail::QueryMapResultBase* resultEntry);
  template<typename ResultType>
  const ResultType& queryGetSavedResult(chpl::querydetail::QueryMapResult<ResultType>* resultEntry);

  void queryNoteError(ast::ErrorMessage error);
  template<typename ResultType, typename... ArgTs>
  ResultType& queryEnd(UniqueString queryName, ResultType result,
                      const std::tuple<ArgTs...>& tupleOfArgs,
                      chpl::querydetail::QueryMap<ResultType,ArgTs...>* queryMap);
};

} // end namespace chpl

#endif
