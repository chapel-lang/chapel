#include "chpl/Queries/Context.h"

#include <cstdlib>
#include <cassert>

namespace chpl {

using namespace chpl::querydetail;

Context::Context()
  : uniqueStringsTable(), queryDB(), currentRevisionNumber(1) {
}

owned<Context> Context::build() {
  return toOwned(new Context());
}

Context::~Context() {
  // free all of the unique'd strings
  for (auto& item: this->uniqueStringsTable) {
    const char* s = item.second;
    free((void*)s);
  }
}

#define ALIGN_DN(i, size)  ((i) & ~((size) - 1))
#define ALIGN_UP(i, size)  ALIGN_DN((i) + (size) - 1, size)

static char* allocateEvenAligned(size_t amt) {
  char* buf = (char*) malloc(amt);
  // Normally, malloc returns something that is aligned to 16 bytes,
  // but it's technically possible that a platform library
  // could not do so. So, here we check.
  // We only need even alignment here.
  if ((((uintptr_t)buf) & 1) != 0) {
    free(buf);
    // try again with an aligned allocation
    size_t alignment = sizeof(void *);
    size_t align_up_len = ALIGN_UP(amt, sizeof(void*));
    buf = (char*) aligned_alloc(alignment, align_up_len);
  }
  assert(buf);
  assert((((uintptr_t)buf) & 1) == 0);
  return buf;
}

const char* Context::getOrCreateUniqueString(const char* str) {
  auto search = this->uniqueStringsTable.find(str);
  if (search != this->uniqueStringsTable.end()) {
    return search->second;
  }
  size_t len = strlen(str)+1;
  char* buf = allocateEvenAligned(len);
  // copy the string data, including the null terminator
  memcpy(buf, str, len);
  this->uniqueStringsTable.insert(search, {buf, buf});
  return buf;
}

bool Context::queryCanUseSavedResult(QueryMapResultBase* resultEntry) {
  // If we already checked this query in this revision,
  // we can use this result
  if (resultEntry->lastComputed == this->currentRevisionNumber ||
      resultEntry->lastCheckedAndReused == this->currentRevisionNumber) {
    return true;
  }

  // Otherwise, check the dependencies. Have any of them
  // changed since the last revision in which we computed this?
  for (QueryMapResultBase* dependency : resultEntry->dependencies) {
    if (dependency->lastChanged > resultEntry->lastComputed) {
      return false;
    }
  }

  // Otherwise, all of the inputs have not changed
  // since this result was last computed.
  resultEntry->lastCheckedAndReused = this->currentRevisionNumber;
  return true;
}
void Context::saveDependenciesAndErrorsInParent(QueryMapResultBase* resultEntry) {
  // Record that the parent query depends upon this one. 
  //
  // we haven't pushed the query beginning yet, so the
  // parent query is at queryDeps.back()
  if (queryDeps.size() > 0) {
    queryDeps.back().dependencies.push_back(resultEntry);
    if (resultEntry->errors.size() > 0) {
      for (ErrorMessage e : resultEntry->errors) {
        queryDeps.back().errors.push_back(e);
      }
    }
  }
}
void Context::endQueryHandleDependency(QueryMapResultBase* result) {
  // queryDeps.back() is the dependency vector for this query
  // which is now ending. So, replace result->dependencies with it.
  result->dependencies.swap(queryDeps.back().dependencies);
  result->errors.swap(queryDeps.back().errors);
  queryDeps.pop_back();
  // additionally, we've run a query and there might well be
  // a parent query. In that event, we should update the dependency
  // vector for the parent query.
  saveDependenciesAndErrorsInParent(result);
}

void Context::queryNoteError(ErrorMessage error) {
  assert(queryDeps.size() > 0);
  queryDeps.back().errors.push_back(error);
}

const char* Context::uniqueCString(const char* s) {
  if (s == NULL) s = "";
  return this->getOrCreateUniqueString(s);
}


namespace querydetail {


template<>
std::size_t queryArgsHash<>(const std::tuple<>& tuple) {
  return 0;
}

template<>
bool queryArgsEquals<>(const std::tuple<>& lhs,
                       const std::tuple<>& rhs) {
  return true;
}

QueryMapResultBase::~QueryMapResultBase() {
}

QueryMapBase::~QueryMapBase() {
}


} // end namespace querydetail
} // end namespace chpl
