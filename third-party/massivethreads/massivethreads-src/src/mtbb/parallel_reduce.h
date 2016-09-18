/* 
 * parallel_reduce implementation on top of MassiveThreads

 it supports an interface similar to TBB's parallel_reduce
 http://www.threadingbuildingblocks.org/docs/help/reference/algorithms/parallel_reduce_func.htm
 (there seems a bug in the above page. see below)

template<typename Range, typename Value, 
         typename Func, typename Reduction>
Value parallel_reduce( const Range& range, const Value& identity,
                     const Func& func, const Reduction& reduction,
                     [, partitioner[, task_group_context& group]] );

template<typename Range, typename Body> 
void parallel_reduce( const Range& range, Body& body
                      [, partitioner[, task_group_context& group]] );

in the above page, signature of this function is described as

template<typename Range, typename Body> 
void parallel_reduce( const Range& range, ****const**** Body& body
                      [, partitioner[, task_group_context& group]] );

but I believe this is a documentation bug, since body is supposed to
modify its variables along the way.  tbb's implementation also does
not have const qualifier, as a matter of fact.

*/

#pragma once
#include <mtbb/task_group.h>

namespace mtbb {

  /* functional reduce */
  template<typename Range, typename Value, 
    typename Func, typename Reduction>
    Value parallel_reduce(const Range& range, 
                          const Value& identity,
                          const Func& func, 
                          const Reduction& reduction);

  template<typename Range, typename Value, 
    typename Func, typename Reduction>
    struct parallel_reduce_functional_callable {
      Value& result;
      const Range& range;
      const Value& identity;
      const Func& func;
      const Reduction& reduction;
    parallel_reduce_functional_callable(Value& result_,
                                        const Range& range_, const Value& identity_,
                                        const Func& func_, const Reduction& reduction_) :
      result(result_), range(range_), identity(identity_),
        func(func_), reduction(reduction_) {}
      void operator() () {
        result = mtbb::parallel_reduce(range, identity, func, reduction);
      }
    };
  
  template<typename Range, typename Value, 
    typename Func, typename Reduction>
    Value parallel_reduce(const Range& range, 
                          const Value& identity,
                          const Func& func, 
                          const Reduction& reduction) {
    if (range.empty()) {
      return identity;
    } else if (!range.is_divisible()) {
      return func(range);
    } else {
      task_group tg;
      Range left(range);
      const Range right(left, tbb::split());
      Value left_result;
      tg.run(mtbb::parallel_reduce_functional_callable<Range,Value,Func,Reduction>(left_result, left, identity, func, reduction));
      const Value right_result = mtbb::parallel_reduce(right, identity, func, reduction);
      tg.wait();
      return reduction(left_result, right_result);
    }
  }
  
  /* imperative reduce */
  template<typename Range, typename Body> 
    void parallel_reduce( const Range& range, Body& body );

  template<typename Range, typename Body> 
    struct parallel_reduce_imperative_callable {
      const Range& range;
      Body& body;
    parallel_reduce_imperative_callable(const Range& range_, 
                                        Body& body_) :
      range(range_), body(body_) {}
      void operator() () {
        mtbb::parallel_reduce<Range,Body>(range, body);
      }
    };
  
  template<typename Range, typename Body> 
    void parallel_reduce( const Range& range, Body& body ) {
    if (range.empty()) {
      return;
    } else if (!range.is_divisible()) {
      body(range);
    } else {
      task_group tg;
      Range left(range);
      const Range right(left, tbb::split());
      Body right_body(body, tbb::split());
      tg.run(mtbb::parallel_reduce_imperative_callable<Range,Body>(left, body));
      mtbb::parallel_reduce<Range,Body>(right, right_body);
      tg.wait();
      body.join(right_body);
    }
  }
}

