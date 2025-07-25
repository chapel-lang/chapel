/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

module MemConsistency {
  pragma "c memory order type"
  extern "chpl_memory_order" type memory_order;

  // When I finish removing PRIM_INIT before initialization to a known
  // value, then this method should work.  Until then, my stopgap will be
  // an external function in the runtime.

  //inline proc _defaultOf(type t:memory_order): memory_order
  //  return memory_order_seq_cst;

  pragma "last resort"
  @chpldoc.nodoc
  inline proc _defaultOf(type t:memory_order) {
    @chpldoc.nodoc
    extern proc _defaultOfMemoryOrder(): memory_order;

    return _defaultOfMemoryOrder();
  }

  operator memory_order.==(a:memory_order, b:memory_order):bool {
    return __primitive("==", a, b);
  }
  operator memory_order.!=(a:memory_order, b:memory_order):bool {
    return __primitive("!=", a, b);
  }
  operator memory_order.=(ref lhs:memory_order, rhs:memory_order) {
    __primitive("=", lhs, rhs);
  }

  proc memory_order.serialize(writer, ref serializer) throws {
    if this == memory_order_relaxed then
      writer.write("memory_order_relaxed");
    else if this == memory_order_consume then
      writer.write("memory_order_consume");
    else if this == memory_order_acquire then
      writer.write("memory_order_acquire");
    else if this == memory_order_release then
      writer.write("memory_order_release");
    else if this == memory_order_acq_rel then
      writer.write("memory_order_acq_rel");
    else if this == memory_order_seq_cst then
      writer.write("memory_order_seq_cst");
    else
      writer.write("memory_order_unknown");
  }

  extern "chpl_memory_order_relaxed" const memory_order_relaxed:memory_order;
  extern "chpl_memory_order_consume" const memory_order_consume:memory_order;
  extern "chpl_memory_order_acquire" const memory_order_acquire:memory_order;
  extern "chpl_memory_order_release" const memory_order_release:memory_order;
  extern "chpl_memory_order_acq_rel" const memory_order_acq_rel:memory_order;
  extern "chpl_memory_order_seq_cst" const memory_order_seq_cst:memory_order;

  pragma "memory order type"
  enum memoryOrder {seqCst, acqRel, release, acquire, relaxed}


  // Given an input memory order, return an order that can be used for an
  // atomic load or for the compareExchange failure case.
  proc readableOrder(param order: memoryOrder) param {
    if order == memoryOrder.release || order == memoryOrder.acqRel {
      return memoryOrder.acquire;
    } else {
      return order;
    }
  }

  // Given an input memory order, returns 'true' if that
  // order has a release component
  proc orderIncludesRelease(param order: memoryOrder) param : bool {
    return order == memoryOrder.seqCst ||
           order == memoryOrder.acqRel ||
           order == memoryOrder.release;
  }

  inline proc c_memory_order(param order: memoryOrder) {
    import HaltWrappers;
    select order {
      when memoryOrder.relaxed do return memory_order_relaxed;
      when memoryOrder.acquire do return memory_order_acquire;
      when memoryOrder.release do return memory_order_release;
      when memoryOrder.acqRel  do return memory_order_acq_rel;
      when memoryOrder.seqCst  do return memory_order_seq_cst;
      otherwise do compilerError("Invalid memoryOrder");
    }
  }

  // These functions are memory consistency fences (ie acquire or
  // release fences) for the remote data cache.
  // Calls to them are added by the compiler when --cache-remote is used.

  pragma "insert line file info"
  pragma "compiler added remote fence"
  extern proc chpl_rmem_consist_release();
  pragma "insert line file info"
  pragma "compiler added remote fence"
  extern proc chpl_rmem_consist_acquire();
  pragma "insert line file info"
  pragma "compiler added remote fence"
  extern proc chpl_rmem_consist_maybe_release(order:memory_order);
  pragma "compiler added remote fence"
  proc chpl_rmem_consist_maybe_release(param order:memoryOrder) {
    chpl_rmem_consist_maybe_release(c_memory_order(order));
  }
  pragma "insert line file info"
  pragma "compiler added remote fence"
  extern proc chpl_rmem_consist_maybe_acquire(order:memory_order);
  pragma "compiler added remote fence"
  proc chpl_rmem_consist_maybe_acquire(param order:memoryOrder) {
    chpl_rmem_consist_maybe_acquire(c_memory_order(order));
  }

  // This one can be used in module code.
  pragma "insert line file info"
  extern proc chpl_rmem_consist_fence(order:memory_order);
  proc chpl_rmem_consist_fence(param order:memoryOrder) {
    chpl_rmem_consist_fence(c_memory_order(order));
  }

  // Local memory consistency is handled in Atomics.chpl
  // and can be done from C.

}
