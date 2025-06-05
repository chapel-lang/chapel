/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_RESOLUTION_RESOLUTION_CONTEXT_H
#define CHPL_RESOLUTION_RESOLUTION_CONTEXT_H

#include "chpl/framework/Context.h"
#include "chpl/framework/ID.h"
#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"

#include <cstdint>
#include <unordered_set>
#include <vector>

namespace chpl {
namespace resolution {

// Forward declare some types we reference but can't include yet, as their
// declarations may depend on this file.
struct Resolver;
class ResolutionResultByPostorderID;
class ResolvedFunction;
class TypedFnSignature;
class UntypedFnSignature;
class MatchingIdsWithName;
class ImplementationWitness;

/**
  This class is used to manage stack frames that may be necessary while
  resolving. Any resolution function that may require stack frames will
  take a 'ResolutionContext*' as its first argument instead of 'Context*'.

  In the vast majority of cases an end-user of the API can call such a
  function using the following sort of pattern:

    ResolutionContext rcval(context);
    auto crr = resolveCall(&rcval, ...);

  The 'ResolutionContext' consists of a series of 'frames' that roughly
  correspond to typical stack frames. There are two types of frame,
  _stable_ frames and _unstable_ frames. Unstable frames are generally
  created to wrap around a mutable 'Resolver*' instance. Stable frames
  wrap around immutable query results such as 'const ResolvedFunction*'.

  At any point in time, a 'ResolutionContext*' is either stable or unstable.
  It is unstable if it has one or more unstable frames.

  When the 'ResolutionContext*' is unstable, any 'CHPL_RESOLUTION_QUERY...'
  will consult the passed in query arguments in order to determine if the
  'Context*' query cache can be used. If the arguments may refer to a
  nested function, then the computed result is not cached in the global
  query cache.
*/
class ResolutionContext {
 private:
  // This class is used to implement a cache for stored results.
  class StoredResultBase {
   public:
    StoredResultBase() = default;
    virtual ~StoredResultBase() = default;
    virtual void* get() { return nullptr; }
    virtual bool operator==(const StoredResultBase& rhs) const = 0;
    virtual size_t hash() const = 0;
    struct OwnedKeyByValEquals {
      bool operator()(const owned<StoredResultBase>& lhs,
                      const owned<StoredResultBase>& rhs) const {
        return *lhs == *rhs;
      }
    };
    struct OwnedKeyByValHash {
      size_t operator()(const owned<StoredResultBase>& x) const {
        return x->hash();
      }
    };
  };

  // This class is used to implement a cache for stored results.
  template <typename T>
  class StoredResult final : public StoredResultBase {
    T t_;
   public:
    StoredResult(T&& t) : t_(std::move(t)) {}
    virtual ~StoredResult() = default;
    virtual void* get() override { return &t_; }
    virtual size_t hash() const override { return chpl::hash(t_); }
    virtual bool operator==(const StoredResultBase& rhs) const override {
      auto rptr = ((const StoredResult<T>*) &rhs);
      return this->t_ == rptr->t_;
    }
  };

 public:

  /** This class represents a single 'frame' for a symbol. It is currently
      a thin wrapper around the internal 'Resolver' type. Generally, a
      frame is pushed every time specific resolvers are created, and popped
      when they are destroyed. */
  class Frame {
   public:
    enum Kind { FUNCTION, MODULE, INTERFACE, UNKNOWN };

   private:
    friend class ResolutionContext;
    using StoreSlot = owned<StoredResultBase>;
    using StoreHash = StoredResultBase::OwnedKeyByValHash;
    using StoreEqual = StoredResultBase::OwnedKeyByValEquals;
    using Store = std::unordered_set<StoreSlot, StoreHash, StoreEqual>;
    static constexpr int64_t BASE_FRAME_INDEX = -1;
    static const ID EMPTY_AST_ID;

    Resolver* rv_ = nullptr;
    const ResolvedFunction* rf_ = nullptr;
    const types::InterfaceType* ift_ = nullptr;
    const ImplementationWitness* witness_ = nullptr;
    int64_t index_ = BASE_FRAME_INDEX;
    Store cachedResults_;
    Kind kind_ = UNKNOWN;

    Frame() = default;
    Frame(Resolver* rv, Kind kind, int64_t index)
      : rv_(rv), index_(index), kind_(kind) {
    }
    Frame(const ResolvedFunction* rf, int64_t index)
      : rf_(rf), index_(index), kind_(FUNCTION) {
    }
    Frame(const types::InterfaceType* ift, const ImplementationWitness* witness, int64_t index)
      : ift_(ift), witness_(witness), index_(index), kind_(INTERFACE) {
    }

   public:
   ~Frame() = default;
    Frame(const Frame& rhs) = delete;
    Frame(Frame&& rhs) = default;
    Frame& operator=(Frame&& rhs) = default;
    Frame& operator=(const Frame& rhs) = delete;

    const Frame* parent(const ResolutionContext* rc) const {
      if ((index_ - 1) < 0) return nullptr;
      return &rc->frames_[index_ - 1];
    }
    Frame* parent(ResolutionContext* rc) {
      if ((index_ - 1) < 0) return nullptr;
      return &rc->frames_[index_ - 1];
    }

    Resolver* rv() { return rv_; }
    const ResolvedFunction* rf() const { return rf_; }
    const types::InterfaceType* ift() const { return ift_; }
    const ImplementationWitness* witness() const { return witness_; }

    bool isEmpty() { return !rv() && !rf(); }
    const ID& id() const;
    const TypedFnSignature* signature() const;
    const types::QualifiedType typeForContainedId(ResolutionContext* rc, const ID& id) const;
    bool isUnstable() const;

    template <typename T>
    const T& cache(T t) {
      auto v = toOwned(new StoredResult<T>(std::move(t)));
      auto it = cachedResults_.insert(std::move(v)).first;
      StoredResultBase* base = it->get();
      CHPL_ASSERT(base);
      auto ptr = static_cast<T*>(base->get());
      return *ptr;
    }
  };

 private:
  friend struct Resolver;

  Context* context_ = nullptr;
  std::vector<Frame> frames_;
  int numUnstableFrames_ = 0;
  Frame baseFrame_;

  const Frame* pushFrame(const ResolvedFunction* rf);
  const Frame* pushFrame(const types::InterfaceType* t, const ImplementationWitness* witness);
  const Frame* pushFrame(Resolver* rv, Frame::Kind kind);
  void popFrame(const ResolvedFunction* rf);
  void popFrame(Resolver* rv);

  Frame* lastFrameMutable() {
    return frames_.empty() ? nullptr : &frames_.back();
  }
  Frame& lastFrameOrBaseMutable() {
    return frames_.empty() ? baseFrame_ : frames_.back();
  }

  template <typename F>
  Frame* findFrameMatchingMutable(F predicate) {
    for (auto f = lastFrameMutable(); f; f = f->parent(this)) {
      if (f && predicate(*f)) return f;
    }
    return nullptr;
  }

  Resolver* findParentResolverFor(const TypedFnSignature* sig);

 public:
  /** Create an empty 'ResolutionContext'. */
  explicit ResolutionContext(Context* context) : context_(context) {}
  ResolutionContext(const ResolutionContext& rhs) = delete;
  ResolutionContext(ResolutionContext&& rhs) = default;
 ~ResolutionContext() = default;

  /** Get the underlying compiler context. */
  Context* context() const { return context_; }

  /** If 'true', then this 'ResolutionContext' has no state available. */
  bool isEmpty() const { return frames_.empty(); }

  /** If 'true', then this contains one or more unstable frames. */
  bool isUnstable() const { return numUnstableFrames_ != 0; }

  /** Get the last frame from the frame stack. */
  const Frame* lastFrame() const {
    return frames_.empty() ? nullptr : &frames_.back();
  }

  /** Get the frame stack. */
  const std::vector<Frame>& frames() const { return frames_; }

  /** Get the base frame. It is not on the frame stack. */
  const Frame& baseFrame() const { return baseFrame_; }

  /** Find the first stack frame matching a predicate in LIFO order. */
  template <typename F>
  const Frame* findFrameMatching(F predicate) const {
    for (auto f = lastFrame(); f; f = f->parent(this)) {
      if (f && predicate(*f)) return f;
    }
    return nullptr;
  }

  /** Find the first stack frame containing _or_ matching an id. */
  const Frame* findFrameWithId(const ID& id) const {
    return findFrameMatching([&](auto& f) {
      return f.id() == id || f.id().contains(id);
    });
  }

  /** Determine if the global query cache stored in the 'Context*' can
      be used. If this 'ResolutionContext*' is not unstable, then the
      global query cache can always be used.

      If not, then consider 't'. In general, if 't' is a type that
      refers to a nested function, conservatively return 'false'. */
  template <typename T>
  bool canUseGlobalCacheConsidering(const T& t) const {
    if (!isUnstable()) return true;
    return ResolutionContext::canUseGlobalCache(context_, t);
  }
  template <typename T>
  bool canUseGlobalCacheConsidering(const T* t) const {
    return canUseGlobalCacheConsidering(*t);
  }
  template <typename... Ts>
  bool canUseGlobalCacheConsidering(const std::tuple<Ts...>& ts) {
    return std::apply([=](auto&... x) {
      return (canUseGlobalCacheConsidering(x) && ...);
    }, ts);
  }

  // This overload takes at least two items to differentiate itself from
  // the single argument overloads above (it is possible for the parameter
  // pack to be empty).
  template <typename T1, typename T2, typename... Ts>
  bool canUseGlobalCacheConsidering(const T1& t1, const T2& t2,
                                    const Ts& ...ts) {
    if (isEmpty() || !isUnstable()) return true;
    auto tup = std::forward_as_tuple(t1, t2, ts...);
    return canUseGlobalCacheConsidering(tup);
  }

  /** Can the global query cache can be invoked considering only 't'?
      By default, assume it can be. */
  template <typename T>
  static bool canUseGlobalCache(Context* context, const T& t) {
    return true;
  }
  template <typename T>
  static bool canUseGlobalCache(Context* context, const T*& t) {
    if (t == nullptr) return true;
    return canUseGlobalCache(context, *t);
  }
  template <typename T>
  static bool canUseGlobalCache(Context* context, const T* const& t) {
    if (t == nullptr) return true;
    return canUseGlobalCache(context, *t);
  }
  template <typename T>
  static bool canUseGlobalCache(Context* context, const std::vector<T>& v) {
    for (auto& x : v) if (!canUseGlobalCache(context, x)) return false;
    return true;
  }

  // Concrete overloads for types commonly encountered in resolution queries.
  static bool
  canUseGlobalCache(Context* context, const UntypedFnSignature& t);
  static bool
  canUseGlobalCache(Context* context, const TypedFnSignature& t);
  static bool
  canUseGlobalCache(Context* context, const ID& t);
  static bool
  canUseGlobalCache(Context* context, const MatchingIdsWithName& ids);

  //
  // Forward declare nested structs that are used to implement query classes.
  // These are defined later in the file.
  // ---

  template <auto F, typename RetByVal, typename... ArgsByValue>
  struct GlobalQueryWrapper;

  template <auto F, typename InvokeRet, typename... InvokeArgs>
  class GlobalQuery;

  template <auto F, typename InvokeArgsTuple>
  struct CanUseGlobalCache;

  template <auto F, typename RetByVal, typename InvokeArgsTuple>
  struct UnstableCache;

  template <auto F, typename InvokeArgsTuple>
  struct GlobalComputeSetup;

  template <auto F, typename InvokeRet, typename... InvokeArgs>
  class Query;

  /** Called to construct an instance of a 'ResolutionContext::Query'.
      These implement the 'CHPL_RESOLUTION_QUERY...' family of
      macros defined below. */
  template <auto F, typename... InvokeArgs>
  auto createQueryClass(const char* name, InvokeArgs&&... args) {
    using InvokeRet = decltype(F(this, args...));
    using Q = ResolutionContext::Query<F, InvokeRet, InvokeArgs...>;
    return Q(this, name, std::forward<InvokeArgs>(args)...);
  }
};

/** This struct forms an adapter between queries that work with the
    'ResolutionContext*' and traditional queries that work with the
    'Context*' (the global query cache).

    The function that contains the user's query code is the template
    value 'F'. The function stored in the global query cache is
    'query', defined below. The only time 'query' should be called is
    when the 'Context*' is recomputing queries after bumping the
    revision. In that case, 'query' creates an empty 'ResolutionContext'
    and uses it to call into 'F'.
*/
template <auto F, typename RetByVal, typename... ArgsByValue>
struct ResolutionContext::GlobalQueryWrapper {
  static const RetByVal& query(Context* context, ArgsByValue... args) {
    ResolutionContext rcval(context);
    return F(&rcval, std::move(args)...);
  }
};

// TODO: Consider moving all global queries to use this implementation.
/** This is a class-based wrapper around global context queries. */
template <auto F, typename InvokeRet, typename... InvokeArgs>
class ResolutionContext::GlobalQuery {
  static auto getStopwatchType() {
    // These values are invalid, but that's OK, we just need their types.
    Context* context = nullptr;
    querydetail::QueryMapBase* base = nullptr;
    ArgsByValueTuple* ap = nullptr;
    return context->makeQueryTimingStopwatch(base, *ap);
  }

 public:
  using Ret = InvokeRet;
  using RetNoRef = typename std::remove_reference<Ret>::type;
  using RetByVal = typename std::remove_const<RetNoRef>::type;
  template <typename T>
  using Value = std::decay_t<T>;
  using InvokeArgsTuple = std::tuple<InvokeArgs...>;
  using ArgsByValueTuple = std::tuple<Value<InvokeArgs>...>;
  using Wrapper = GlobalQueryWrapper<F, RetByVal, Value<InvokeArgs>...>;
  using TraceFn = std::function<TraceElement(const ArgsByValueTuple&)>;

  #if CHPL_QUERY_TIMING_AND_TRACE_ENABLED
    static constexpr bool STOPWATCH_IS_ACTIVE = true;
  #else
    static constexpr bool STOPWATCH_IS_ACTIVE = false;
  #endif

 private:
  // Exposing the implementation details of 'QUERY_BEGIN', 'QUERY_END'.
  using QueryMap = querydetail::QueryMap<RetByVal, Value<InvokeArgs>...>;
  using QueryRet = querydetail::QueryMapResult<RetByVal, Value<InvokeArgs>...>;
  using RecomputeMarker = Context::RecomputeMarker;
  using Stopwatch = decltype(getStopwatchType());

  static constexpr bool RETURNS_CONST = std::is_const<RetNoRef>::value;
  static constexpr bool RETURNS_REF = std::is_reference<Ret>::value;
  static constexpr auto QUERY = Wrapper::query;

  // Assert early that the user-called function is returning by 'const&'.
  static_assert(RETURNS_CONST, "Query needs to return by 'const'!");
  static_assert(RETURNS_REF, "Query needs to return by '&'!");

  Context* context_ = nullptr;
  const char* name_ = nullptr;

  // References to the arguments as they were passed in.
  InvokeArgsTuple args_;

  // TODO: Right now we need to copy in 'args_' by value to pass them
  // to the 'context->query...' methods. Can this pack be removed in
  // the future?
  ArgsByValueTuple ap_ = args_;

  // This state is used to mimick 'QUERY_BEGIN' and 'QUERY_END'.
  RecomputeMarker recomputeMarker_;
  QueryMap* beginMap_ = nullptr;
  const QueryRet* beginRet_ = nullptr;
  Stopwatch stopwatch_;
  bool isInput_ = false;

 public:
  GlobalQuery(Context* context, const char* name, InvokeArgs&&... args)
        : context_(context),
          name_(name),
          args_({std::forward<InvokeArgs>(args)...}),
          stopwatch_(context_->makeQueryTimingStopwatch(nullptr, ap_)) {
    CHPL_ASSERT(context_ != nullptr);
  }

  GlobalQuery(const GlobalQuery& rhs) = delete;
  GlobalQuery(GlobalQuery&& rhs) = default;

  const InvokeArgsTuple& args() const { return args_; }

  const RetByVal* begin() {
    // Fetch/emplace the map entries, which indicates the query is running.
    beginMap_ = context_->queryBeginGetMap(QUERY, ap_, name_, isInput_);
    beginRet_ = context_->queryBeginGetResult(beginMap_, ap_);

    // Exit early if we've found a result for this revision.
    if (context_->queryUseSaved(QUERY, beginRet_, name_)) {
      auto ret = &context_->queryGetSaved(beginRet_);
      beginMap_ = nullptr;
      beginRet_ = nullptr;
      return ret;
    }

    // Otherwise we are computing, so set the recompute marker.
    // We don't want it to go out of scope now (since that undoes the mark),
    // but this is just the 'begin' function; we do want it to go out of scope
    // when the query is done (when 'end' is called). So, marker it in a field.
    const bool isRecomputing = false;
    auto activeRecomputeMarker = context_->markRecomputing(isRecomputing);

    // We better not be saving another marker, since we only have room to save one.
    CHPL_ASSERT(recomputeMarker_.isCleared());
    std::swap(recomputeMarker_, activeRecomputeMarker);

    // Set the stopwatch if it is compile-time enabled.
    if constexpr (STOPWATCH_IS_ACTIVE) {
      context_->queryBeginTrace(name_, ap_);
      stopwatch_ = context_->makeQueryTimingStopwatch(beginMap_, ap_);
    }

    return nullptr;
  }

  const RetByVal& end(RetByVal x) {
    // Restoring the marker also clears it.
    recomputeMarker_.restore();

    // Pass in the result and indicate the query has ended.
    auto& ret = context_->queryEnd(QUERY, beginMap_, beginRet_, ap_,
                                   std::move(x),
                                   name_);
    beginMap_ = nullptr;
    beginRet_ = nullptr;

    return ret;
  }

  void inactiveStore(RetByVal x) {
    context_->querySetterUpdateResult(QUERY, ap_, std::move(x),
                                      name_,
                                      isInput_,
                                      /* markExternallySet */ false);
  }

  bool isRunning() {
    return context_->isQueryRunning(QUERY, ap_);
  }

  bool hasCurrentResult() {
    return context_->hasCurrentResultForQuery(QUERY, ap_);
  }

  void registerTracer(TraceFn&& fn) {
    beginMap_->registerTracer(std::move(fn));
  }
};

template <auto F, typename InvokeArgsTuple>
struct ResolutionContext::GlobalComputeSetup {
  /** Called before the query is run, returns 'true' if prep was done. */
  bool enter(ResolutionContext* rc, const InvokeArgsTuple& args) {
    return false;
  }
  /** Called before the query returns, only if 'enter' returned 'true'. */
  void leave(ResolutionContext* rc, const InvokeArgsTuple& args) {
  }
};

template <auto F, typename InvokeArgsTuple>
struct ResolutionContext::CanUseGlobalCache {
  /** Consider 'args' and determine if the global query cache can be used. */
  bool operator()(ResolutionContext* rc, const InvokeArgsTuple& args) {
    return std::apply([&](auto... xs) {
      return rc->canUseGlobalCacheConsidering(xs...);
    }, args);
  }
};

/** This struct can be specialized to allow queries to specify how computed
    results are cached when the 'ResolutionContext*' is unstable. */
template <auto F, typename RetByVal, typename InvokeArgsTuple>
struct ResolutionContext::UnstableCache {

  /** If the 'ResolutionContext' is unstable and the global query cache
      cannot be used, determine if a value stored in stack frames can be
      reused for the given input. By default, no lookup occurs. */
  const RetByVal*
  fetchOrNull(ResolutionContext* rc, const InvokeArgsTuple& args) {
    (void) rc;
    (void) args;
    return nullptr;
  }

  /** Implements the 'caching' of unstable query computations by storing
      them into a 'ResolutionContext::Frame'. If no frame is suitable the
      base frame may be used (which has a lifetime as long as the RC).
      By default, use the closest frame. */
  const RetByVal&
  store(ResolutionContext* rc, RetByVal x, const InvokeArgsTuple& args) {
    Frame& f = rc->lastFrameOrBaseMutable();
    return f.cache(std::move(x));
  }
};

/** This class represents an instance of a query created by the macro
    'CHPL_RESOLUTION_QUERY_BEGIN...'. A resolution query is itself a
    wrapper around a global query (cached by the 'Context*'), which is
    represented by the 'GlobalQuery' type. */
template <auto F, typename InvokeRet, typename... InvokeArgs>
class ResolutionContext::Query {
  // TODO: Refactor this into the more general notion of a "global query"
  // (e.g., the 'GlobalQuery' defined above) that can be augmented with
  // additional state or subclassed.
  //
  // NOTE: The 'T' suffixes to prevent a GCC error message.
  using GlobalQueryT = GlobalQuery<F, InvokeRet, InvokeArgs...>;
  using RetByVal = typename GlobalQueryT::RetByVal;
  using InvokeArgsTuple = typename GlobalQueryT::InvokeArgsTuple;
  using UnstableCacheT = UnstableCache<F, RetByVal, InvokeArgsTuple>;
  using CanUseGlobalCacheT = CanUseGlobalCache<F, InvokeArgsTuple>;
  using GlobalComputeSetupT = GlobalComputeSetup<F, InvokeArgsTuple>;

  ResolutionContext* rc_ = nullptr;
  bool canUseGlobalCache_ = false;
  bool didGlobalSetupOccur_ = false;
  GlobalComputeSetupT globalSetup_;
  GlobalQueryT global_;

 public:
  Query(ResolutionContext* rc, const char* name, InvokeArgs&&... args)
      : rc_(rc), global_(rc->context(), name, args... ) {
    CHPL_ASSERT(rc_ && context());
    canUseGlobalCache_ = CanUseGlobalCacheT{}(rc, global_.args());
  }
  Query(const Query& rhs) = delete;
  Query(Query&& rhs) = default;

  Context* context() { return rc_->context(); }

  bool canUseGlobalCache() { return canUseGlobalCache_; }

  const RetByVal* begin() {
    // TODO: Use specialization to eliminate this branch?
    if (canUseGlobalCache()) {
      if (auto ret = global_.begin()) return ret;
      didGlobalSetupOccur_ = globalSetup_.enter(rc_, global_.args());
      return nullptr;
    } else {
      UnstableCacheT uc;
      return uc.fetchOrNull(rc_, global_.args());
    }
  }

  const RetByVal& end(RetByVal x) {
    if (canUseGlobalCache()) {
      auto& ret = global_.end(std::move(x));
      if (didGlobalSetupOccur_) {
        globalSetup_.leave(rc_, global_.args());
        didGlobalSetupOccur_ = false;
      }
      return ret;
    } else {
      UnstableCacheT uc;
      return uc.store(rc_, std::move(x), global_.args());
    }
  }

  bool isGlobalQueryRunning() {
    return global_.isRunning();
  }

  void inactiveStore(RetByVal x) {
    if (canUseGlobalCache()) {
      global_.inactiveStore(std::move(x));
    } else {
      UnstableCacheT uc;
      uc.store(rc_, std::move(x), global_.args());
    }
  }

  void registerTracer(typename GlobalQueryT::TraceFn&& fn) {
    global_.registerTracer(std::move(fn));
  }
};

// TODO: This is meant as a temporary placeholder/sentinel for cases where we
// currently don't have access to a pre-existing ResolutionContext. Eventually
// uses of this function, and the function itself, should be removed.
/// \cond DO_NOT_DOCUMENT
ResolutionContext createDummyRC(Context* context);
/// \endcond DO_NOT_DOCUMENT

/** This macro can be used like 'QUERY_BEGIN', except it prevents the results
    of a query from being cached in the context query cache if the computed
    result could rely on state taken from the type 'ResolutionContext'.

    Queries guarded by this macro must always return by 'const&'. When a
    result is not stored in the context query cache, it will be stored in
    a temporary cache maintained by the innermost 'ResolutionContext::Frame'
    instead. The lifetime of temporarily cached values is the lifetime of
    the associated 'ResolutionContext::Frame'.
*/
#define CHPL_RESOLUTION_QUERY_BEGIN(fn__, rc__, ...) \
  auto rcquery__ = rc__->createQueryClass<fn__>(#fn__, __VA_ARGS__); \
  if (auto ptr__ = rcquery__.begin()) return *ptr__;

/** This macro can be used like 'QUERY_END'. It always returns a reference,
    and in the case that the context cache is not used then the returned
    value is stored in the cache of a 'ResolutionContext' frame. */
#define CHPL_RESOLUTION_QUERY_END(ret__) (rcquery__.end(std::move(ret__)))

/** Within a 'CHPL_RESOLUTION_QUERY...' use this to get the query handle. */
#define CHPL_RESOLUTION_REF_TO_CURRENT_QUERY_HANDLE() (rcquery__)

/** Use this to store a result for any 'CHPL_RESOLUTION_QUERY...' query. */
#define CHPL_RESOLUTION_QUERY_UNSAFE_STORE_RESULT(fn__, rc__, x__, ...) do { \
    auto rcq__ = rc__->createQueryClass<fn__>(#fn__, __VA_ARGS__); \
    rcq__.inactiveStore(std::move(x__)); \
  } while (0)

/** Check if the global part of a 'CHPL_RESOLUTION_QUERY...' is running. */
#define CHPL_RESOLUTION_IS_GLOBAL_QUERY_RUNNING(fn__, rc__, ...) ([&]() { \
    auto rcq__ = rc__->createQueryClass<fn__>(#fn__, __VA_ARGS__); \
    return rcq__.isGlobalQueryRunning(); \
  }())

#define CHPL_RESOLUTION_QUERY_REGISTER_TRACER(tracerBody) \
  rcquery__.registerTracer([](auto& args) { \
      tracerBody; \
  });

} // end namespace resolution
} // end namespace chpl

#endif
