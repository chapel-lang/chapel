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

/* Support for channels that can transfer typed data between tasks.

 This module contains the implementation of channels which can be used to move
 typed data between Chapel tasks.

 A channel is a parallel-safe data structure that provides a mechanism for
 concurrently executing functions to communicate by sending and receiving values
 of a specified element type. A channel can be buffered or unbuffered. A
 buffered channel has a maximum capacity specified by ``bufferSize``. There are
 mainly three operations that can be performed on a channel.

  * :proc:`channel.send` : Send a value to the channel.
  * :proc:`channel.recv` : Receive a value from the channel.
  * :proc:`channel.close` : Close a channel such that no other values/data can be sent to it.

 The channel operations are blocking, i.e., the calling task will be suspended
 if an operation cannot be completed. The channel follows First-In-First-Out
 mechanism, i.e., the first value sent to the channel will be received first.
*/

module Channel {
  import ChapelLocks;
  use CTypes;
  use Sort;
  use CTypes;
  use List;
  use Random;

  @chpldoc.nodoc
  type _lockType = ChapelLocks.chpl_LocalSpinlock;

  @chpldoc.nodoc
  class _LockWrapper {
    var lock$ = new _lockType();

    inline proc lock() {
      lock$.lock();
    }

    inline proc unlock() {
      lock$.unlock();
    }
  }

  @chpldoc.nodoc
  class Waiter {
    // Class used to maintain the properties of suspended tasks.

    type valueType;
    var val : c_ptr(valueType);
    var processPtr : c_ptr(single bool);
    var isSelect : bool;

    // These variables are specific to the waiters for select statement
    var isSelectDone : c_ptr(atomic int);
    var selectId : int = -1;

    var prev : unmanaged Waiter(valueType)?;
    var next : unmanaged Waiter(valueType)?;

    proc init(ref value, ref process$ : single bool) {
      valueType = value.type;
      val = c_ptrTo(value);
      processPtr = c_ptrTo(process$);
      isSelect = false;
    }

    proc init(ref value : c_ptr, ref process$ : single bool, ref selectDone : atomic int, caseId : int) {
      valueType = value.eltType;
      val = value;
      processPtr = c_ptrTo(process$);
      isSelect = true;
      isSelectDone = c_ptrTo(selectDone);
      selectId = caseId;
    }

    proc suspend() : bool {
      return processPtr.deref().readFF();
    }

    proc notify(status : bool) {
      processPtr.deref().writeEF(status);
    }

    proc tryUpdate() : bool {
      return isSelectDone.deref().compareAndSwap(-1, selectId);
    }
  }

  /*
  Implementation of doubly-ended queue to keep track of suspended receiving
  and sending tasks.
  */
  @chpldoc.nodoc
  class WaiterQueue {
    type eltType;
    var front : unmanaged Waiter(eltType)?;
    var back : unmanaged Waiter(eltType)?;

    /* Push the `waiter` into the queue */
    proc enqueue(waiter : unmanaged Waiter(eltType)) {
      if front == nil {
        front = waiter;
        back = waiter;
      }
      else {
        back!.next = waiter;
        waiter.prev = back;
        back = waiter;
      }
    }

    proc isEmpty() : bool {
      return (front == nil);
    }

    /* Pop the first value from the queue */
    proc dequeue() : unmanaged Waiter(eltType) {
      var waiter : unmanaged Waiter(eltType)?;
      if front == back {
        waiter = front;
        front = nil;
        back = nil;
      }
      else {
        waiter = front;
        front = front!.next;
        front!.prev = nil;
        waiter!.next = nil;
      }
      return waiter!;
    }

    /* Remove the specified entry from the queue */
    proc dequeue(waiter : unmanaged Waiter(eltType)) {
      if waiter == front {
        dequeue();
      }
      else if waiter == back {
        back = back!.prev;
        back!.next = nil;
      }
      else if waiter.prev != nil && waiter.next != nil {
        waiter.prev!.next = waiter.next;
        waiter.next!.prev = waiter.prev;
      }
      else {
      // This function is also used specifically by select statements.
      // It may happen that the waiter is already dequeued from the queue.
      // In that case we don't have to do anything.
      }

    }
  }

  record channel {
    type eltType;
    var channelObj : shared chan(eltType);

    /*
     Initialize a channel

     :arg elt: The element type used for sending and receiving
     :type elt: `type`

     :arg size: Specify the maximum capacity for the channel ``bufferSize``.
     :type size: `int`
    */
    proc init(type eltType, size = 0) {
      this.eltType = eltType;
      channelObj = new shared chan(eltType, size);
    }

    proc lock() {
      this.channelObj.lock();
    }

    proc unlock() {
      this.channelObj.unlock();
    }

    /*
     Receive the first value in the channel buffer. It will suspend the
     calling task, until data is sent to the channel. If the channel is
     closed and the buffer is empty, it will return `false` indicating that
     the receive operation was not successful.

     :arg val: Storage for the received value.
     :type val: `eltType`

     :return: `true` if the receive was successful, else `false`.
     :rtype: `bool`
    */
    proc recv(out val : eltType) : bool {
      return recv(val, true);
    }

    @chpldoc.nodoc
    proc recv(out val : eltType, blocking : bool) : bool {
      return this.channelObj.recv(val, blocking);
    }

    /*
     Send a value to the channel buffer. If the buffer is at maximum
     capacity it will suspend the waiting task, until there is space in the
     buffer or a receiving task awakes it. If a channel is closed no more
     data can be sent to it.

     :arg val: Data to be sent to the channel
     :type val: `eltType`

     :throws ChannelError: If ``send`` is called on a closed channel.
    */
    proc send(in val : eltType) throws {
      send(val, true);
    }

    @chpldoc.nodoc
    proc send(in val : eltType, blocking : bool) : bool throws {
      return this.channelObj.send(val, blocking);
    }

    /*
     This function is used to close a channel indicating that no more data
     can be sent to it.

     :throws ChannelError: If called on a closed channel.
    */

    proc close() throws {
      this.channelObj.close();
    }

    /* Iterator to receive data from the channel until it is closed. */
    iter these() {
      while true {
        var received : eltType;
        var status = recv(received);
        if !status then break;
        yield received;
      }
    }

  }

  @chpldoc.nodoc
  class chan {

    /* The type of elements that can be sent to the channel. */
    type eltType;

    /* Maximum number of elements that the channel can hold at a time. */
    var bufferSize : int;
    var buffer : [0..#bufferSize] eltType;
    var sendIdx = 0;
    var recvIdx = 0;
    var count = 0;
    var closed = false;
    var sendWaiters : owned WaiterQueue;
    var recvWaiters : owned WaiterQueue;
    var lock$ = new _LockWrapper();

    proc init(type elt, size = 0) {
      eltType = elt;
      bufferSize = size;
      sendWaiters = new WaiterQueue(elt);
      recvWaiters = new WaiterQueue(elt);
    }

    proc lock() {
      lock$.lock();
    }

    proc unlock() {
      lock$.unlock();
    }

    proc recv(out val : eltType, blocking : bool) : bool {
      if blocking then lock();

      if closed && count == 0 {
        if blocking then unlock();
        if !blocking then return true;
        else return false;
      }

      while !sendWaiters.isEmpty() && sendWaiters.front!.isSelect {
        if sendWaiters.front!.tryUpdate() {
          break;
        }
        else sendWaiters.dequeue();
      }

      if count == 0 && sendWaiters.isEmpty() {
        if !blocking then return false;
        var process$ : single bool;
        var processing = new unmanaged Waiter(val, process$);
        recvWaiters.enqueue(processing);
        unlock();
        var status = processing.suspend();
        delete processing;
        return status;
      }

      if bufferSize > 0 {
        val = buffer[recvIdx];
      }

      if !sendWaiters.isEmpty() {
        var sender = sendWaiters.dequeue();
        if bufferSize > 0 {
          buffer[recvIdx] = sender.val.deref();

          sendIdx = (sendIdx + 1) % bufferSize;
          recvIdx = (recvIdx + 1) % bufferSize;
        }
        else {
          val = sender.val.deref();
        }
        sender.notify(true);
      }
      else {
        recvIdx = (recvIdx + 1) % bufferSize;
        count -= 1;
      }
      if blocking then unlock();

      return true;

    }

    proc send(in val : eltType, blocking : bool) : bool throws {
      if blocking then lock();

      if closed {
        if blocking then unlock();
        throw new owned ChannelError("Trying to send on a closed channel");
      }

      while !recvWaiters.isEmpty() && recvWaiters.front!.isSelect {
        if recvWaiters.front!.tryUpdate() {
          break;
        }
        else recvWaiters.dequeue();
      }

      if count == bufferSize && recvWaiters.isEmpty() {
        if !blocking then return false;
        var process$ : single bool;
        var processing = new unmanaged Waiter(val, process$);

        sendWaiters.enqueue(processing);
        unlock();
        var status = processing.suspend();
        delete processing;
        if status == false {
          throw new owned ChannelError("Trying to send on a closed channel");
        }
        return status;

      }
      else {
        if !recvWaiters.isEmpty() {
          var receiver = recvWaiters.dequeue();
          receiver.val.deref() = val;

          receiver.notify(true);
        }
        else {
          buffer[sendIdx] = val;

          sendIdx = (sendIdx + 1) % bufferSize;
          count += 1;
        }

        if blocking then unlock();
        return true;
      }
    }

    proc close() throws {
      lock();
      if closed {
        unlock();
        throw new owned ChannelError("Trying to close a closed channel");
      }
      closed = true;
      var queued = new WaiterQueue(eltType);
      while !recvWaiters.isEmpty() {
        var receiver = recvWaiters.dequeue();
        if(receiver.isSelect) {
          if receiver.tryUpdate() then
            queued.enqueue(receiver);
        }
        else queued.enqueue(receiver);
      }

      while !sendWaiters.isEmpty() {
        var sender = sendWaiters.dequeue();
        if(sender.isSelect) {
          if sender.tryUpdate() then
            queued.enqueue(sender);
        }
        else queued.enqueue(sender);
      }
      unlock();

      while !queued.isEmpty() {
        var waiter = queued.dequeue();
        waiter.notify(false);
      }
    }

  }

  /* Error class for Channel */
  @chpldoc.nodoc
  class ChannelError : Error {
    var msg:string;

    proc init(msg: string) {
      this.msg = msg;
    }

    override proc message() {
      return msg;
    }
  }

  /* Base class used for aggregating different select-cases */
  @chpldoc.nodoc
  class SelectBaseClass {
    proc lockChannel() { }
    proc unlockChannel() { }
    proc getId() : int { return 0; }
    proc sendRecv() : bool { return true; }
    proc getAddr() : c_uintptr { return 0 : c_uintptr; }
    proc enqueueWaiter(ref process$ : single bool, ref isDone : atomic int) { }
    proc dequeueWaiter() { }
  }

  /* Enum to specify the operation in a select-case */
  @chpldoc.nodoc
  enum selectOperation { recv, send }

  @chpldoc.nodoc
  class SelectCase : SelectBaseClass {
    type eltType;
    var val : c_ptr(eltType);
    var selectChannel : channel(eltType);
    var operation : selectOperation;
    var waiter : unmanaged Waiter(eltType)?;
    var id : int;

    proc init(ref value, ref selectChannel : channel(?), op : selectOperation, caseId : int) {
      this.eltType = value.type;
      this.val = c_ptrTo(value);
      this.selectChannel = selectChannel;
      this.operation = op;
      this.id = caseId;
    }

    override proc lockChannel() {
      selectChannel.lock();
    }

    override proc unlockChannel() {
      selectChannel.unlock();
    }

    override proc getId() : int {
      return id;
    }

    /* Carry out the case operation and return the status */
    override proc sendRecv() : bool {
      if operation == selectOperation.recv {
        return selectChannel.recv(val.deref(), false);
      }
      else return (try! selectChannel.send(val.deref(), false));
    }

    /* Retrieve the address of the involved channel */
    override proc getAddr() : c_uintptr {
      return ((selectChannel.channelObj : c_void_ptr) : c_uintptr);
    }

    override proc enqueueWaiter(ref process$ : single bool, ref isDone : atomic int) {
      waiter = new unmanaged Waiter(val, process$, isDone, id);
      if operation == selectOperation.recv {
        selectChannel.channelObj.recvWaiters.enqueue(waiter!);
      }
      else {
        selectChannel.channelObj.sendWaiters.enqueue(waiter!);
      }
    }

    override proc dequeueWaiter() {
      if operation == selectOperation.recv {
        selectChannel.channelObj.recvWaiters.dequeue(waiter!);
      }
      else selectChannel.channelObj.sendWaiters.dequeue(waiter!);
      delete waiter;
    }
  }

  /* Comparator used for sorting the channels according to their memory
  addresses.
  */
  @chpldoc.nodoc
  record Comparator {
    proc compare(case1, case2) {
      return (case1.getAddr() - case2.getAddr()) : int;
    }
  }

  /* Acquire the lock of all involved channels */
  @chpldoc.nodoc
  proc lockSelect(lockOrder : list(shared SelectBaseClass)) {
    for channelWrapper in lockOrder do channelWrapper.lockChannel();
  }

  /* Release the lock all involved channels */
  @chpldoc.nodoc
  proc unlockSelect(lockOrder : list(shared SelectBaseClass)) {
    for idx in lockOrder.indices by -1 do lockOrder[idx].unlockChannel();
  }

  /* Entry point for select statements */
  @chpldoc.nodoc
  proc selectProcess(cases : [] shared SelectBaseClass, default : bool = false) : int{
    var numCases = cases.domain.size;

    var addrCmp : Comparator;
    // Sort all cases according to their channel addresses
    sort(cases, comparator = addrCmp);

    /*
    Determine the lock order of the involved channels based on their
    addresses. This helps prevent deadlock with other concurrently
    executing select statements
    */
    var lockOrder = new list(shared SelectBaseClass);
    for idx in cases.domain {
      if idx == 0 || cases[idx].getAddr() != cases[idx - 1].getAddr() {
        lockOrder.pushBack(cases[idx]);
      }
    }
    var done = -1;
    lockSelect(lockOrder);

    /*
    Check all the cases in a random order. This is done to prevent
    starvation on multiple calls to the select statement.
    */
    shuffle(cases);
    for case in cases {
      if case.sendRecv() {
        done = case.getId();
        break;
      }
    }

    if done != -1 || default {
      unlockSelect(lockOrder);
      return done;
    }

    /* If none of the channels was ready, enqueue the select task to each
    channel's waiting queue and wait for other task to awaken us.
    */
    var isDone : atomic int = -1;
    var process$ : single bool;

    for case in cases {
      case.enqueueWaiter(process$, isDone);
    }

    unlockSelect(lockOrder);
    process$.readFF();

    lockSelect(lockOrder);

    /* Dequeue the waiters from each involved case */
    for case in cases {
      case.dequeueWaiter();
    }
    unlockSelect(lockOrder);
    return isDone.read();
  }
}
