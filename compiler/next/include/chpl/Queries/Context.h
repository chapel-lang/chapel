#ifndef CHPL_QUERIES_CONTEXT_H
#define CHPL_QUERIES_CONTEXT_H

#include "chpl/AST/UniqueString.h"
#include "chpl/AST/ID.h"
#include "chpl/Queries/ContextDetail.h"
#include "chpl/Util/memory.h"

#include <unordered_map>

namespace chpl {

/**
  This class stores the compilation-wide context.
  In particular it handles unique'd strings.
 */
class Context {
 private:
  // map that supports uniqueCString / UniqueString
   typedef std::unordered_map<const char*, char*, chpl::detail::UniqueStrHash, chpl::detail::UniqueStrEqual> UniqueStringsTableType;
  UniqueStringsTableType uniqueStringsTable;

  // map from a query name to appropriate QueryMap object.
  // maps to an 'owned' heap-allocated thing to manage having subclasses
  // without slicing.
  std::unordered_map<UniqueString, owned<chpl::querydetail::QueryMapBase>> queryDB;

  // Since IDs include module names but not file paths, use this
  // map to go from module name to file path.
  // (If this proves too restrictive for some reason, we could
  //  start including file path in IDs).
  std::unordered_map<UniqueString, UniqueString> modNameToFilepath;

  struct QueryDepsEntry {
    UniqueString queryName;
    chpl::querydetail::QueryDependencyVec dependencies;
    std::vector<ErrorMessage> errors;
    QueryDepsEntry(UniqueString queryName)
      : queryName(queryName), dependencies(), errors() {
    }
  };

  // this is used to compute the dependencies
  std::vector<QueryDepsEntry> queryDeps;

  chpl::querydetail::RevisionNumber currentRevisionNumber;
  chpl::querydetail::RevisionNumber lastPrepareToGCRevisionNumber;
  chpl::querydetail::RevisionNumber gcCounter;

  Context();
  const char* getOrCreateUniqueString(const char* s);
  bool queryCanUseSavedResult(chpl::querydetail::QueryMapResultBase* resultEntry);
  void saveDependenciesAndErrorsInParent(chpl::querydetail::QueryMapResultBase* resultEntry);
  void endQueryHandleDependency(chpl::querydetail::QueryMapResultBase* result);

  template<typename ResultType, typename... ArgTs>
  chpl::querydetail::QueryMapResult<ResultType>*
  updateResultForQuery(UniqueString queryName,
                       const std::tuple<ArgTs...>& tupleOfArgs,
                       ResultType result,
                       bool& changed);

  // Future Work: support marking used strings and garbage collecting the rest
  // Could store an atomic uint_8 just after the string for the mark.

  // Future Work: make the context thread-safe

  // Future Work: allow moving some AST to a different context
  //              (or, at least, that can handle the unique strings)

  // Performance: Add fine-grained timing to measure
  //  * the total time spent in each query
  //  * the time spent in each query in Context functions
  //    (i.e. hashtable manipulations)
  //  * the time spent in each query in other queries
  //  * the time spent in each query in other query code

  // Performance: How can we arrange for better locality of reference/
  // cache reuse for the maps from IDs? The IDs within a function could
  // be just stored in a vector, but that would add an indirection to
  // the hashtable. Is there a way to adjust the hashing function and
  // tune the hashtable bucket size, or something? Do we need a custom
  // hashtable?

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

  /**
    Return the name of the module containing this ID.
   */
  UniqueString moduleNameForID(ID id);
  /**
    Return the file path for the file containing this ID.
   */
  UniqueString filePathForID(ID id);
  
  /**
    Query to get a file path given a module name
   */
  UniqueString filePathForModuleName(UniqueString modName);

  /**
    This function increments the current revision number stored
    in the context. After it is called, the setters below can
    be used to provide the input at that revision.

    If the prepareToGC argument is true, when processing queries
    in that revision, will prepare to garbage collect (by marking
    elements appropriately).
   */
  void advanceToNextRevision(bool prepareToGC);

  /**
    This function runs garbage collection, but it only has an effect
    if the last call to advanceToNextRevision passed
    prepareToGC=true.
   */
  void collectGarbage();

  // setters for named queries.

  /**
    Sets the file path for the given toplevel module name. It does not bump
    the current revision counter so is suitable for calling from
    a parse query.
    Returns 'true' if this function caused a new result to be saved
    in the context.
   */
  bool setFilePathForModuleName(UniqueString modName, UniqueString path);

  /**
    setFileText will set the text for a particular file path.
    Returns 'true' if this function caused a new result to be saved
    in the context.
   */
  bool setFileText(UniqueString path, std::string data);

  // the following functions are called by the macros defined in QueryImpl.h
  // and should not be called directly

  /// \cond DO_NOT_DOCUMENT
  template<typename... ArgTs>
  void queryTraceBegin(UniqueString queryName, const char* func,
                       const std::tuple<ArgTs...>& tupleOfArg);

  template<typename... ArgTs>
  void queryTraceEnd(UniqueString queryName, const char* func,
                     const std::tuple<ArgTs...>& tupleOfArg,
                     bool changed);


  template<typename ResultType, typename... ArgTs>
  chpl::querydetail::QueryMap<ResultType,ArgTs...>*
    queryGetMap(UniqueString queryName, const std::tuple<ArgTs...>& tupleOfArgs);

  // queryFunc is only used for tracing
  bool queryCanUseSavedResultAndPushIfNot(UniqueString queryName,
      const char* queryFunc,
      chpl::querydetail::QueryMapResultBase* resultEntry);

  template<typename ResultType>
  const ResultType& queryGetSavedResult(chpl::querydetail::QueryMapResult<ResultType>* resultEntry);

  void queryNoteError(ErrorMessage error);
  template<typename ResultType, typename... ArgTs>
  ResultType& queryEnd(UniqueString queryName, const char* func,
                      ResultType result,
                      const std::tuple<ArgTs...>& tupleOfArgs,
                      chpl::querydetail::QueryMap<ResultType,ArgTs...>* queryMap);
  /// \endcond
};

} // end namespace chpl

#endif
