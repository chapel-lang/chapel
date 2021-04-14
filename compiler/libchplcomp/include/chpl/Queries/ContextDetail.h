#ifndef CHPL_QUERIES_CONTEXT_DETAIL_H
#define CHPL_QUERIES_CONTEXT_DETAIL_H

#include "chpl/AST/UniqueString.h"
#include "chpl/Util/memory.h"

#include <cstring>
#include <unordered_map>
#include <vector>

/// \cond DO_NOT_DOCUMENT
namespace chpl {
namespace detail {

struct UniqueStrEqual {
  bool operator()(const char* lhs, const char* rhs) const {
    return strcmp(lhs, rhs) == 0;
  }
};

struct UniqueStrHash {
  std::size_t operator()(const char* s) const {
    // this hash is from StringHashFns in the old map.h
    unsigned int h = 0;
    while (*s) h = h * 27 + (unsigned char)*s++;
    return h;
  }
};

typedef int64_t RevisionNumber;
class QueryMapResultBase;
typedef std::vector<QueryMapResultBase*> DependencyVec;

class QueryMapResultBase {
 public:
  std::vector<QueryMapResultBase*> dependencies;
  RevisionNumber lastChecked;
  RevisionNumber lastChanged;
};
template<typename ResultType>
class QueryMapResult : public QueryMapResultBase {
 public:
  ResultType result;
  QueryMapResult(ResultType result)
    : result(std::move(result)) {
  }
};

class QueryMapBase {
 public:
   virtual ~QueryMapBase() = 0; // this is an abstract base class

   /**
    The "name" of the query which is based upon the
    source file/line/function that runs BEGIN_QUERY.
    */
   ast::UniqueString queryName;
};
template<typename ResultType, typename ArgTupleType>
class QueryMap : public QueryMapBase {
 public:
  typedef QueryMapResult<ResultType> TheResultType;
  typedef std::unordered_map<ArgTupleType, owned<TheResultType>> MapType;
  MapType map;
};

} // end namespace detail
} // end namespace chpl
/// \endcond

#endif
