.. default-domain:: chpl

.. module:: LockFreeStack
   :synopsis: An implementation of the Treiber Stack [#]_, a lock-free stack. Concurrent safe

LockFreeStack
=============
**Usage**

.. code-block:: chapel

   use LockFreeStack;


An implementation of the Treiber Stack [#]_, a lock-free stack. Concurrent safe
memory reclamation is handled by an internal :record:`EpochManager`. Usage of the
stack can be seen below.

.. code-block:: chpl

  var lfs = new LockFreeStack(int);
  forall i in 1..N do lfs.push(i);
  var total : int;
  coforall tid in 1..here.maxTaskPar with (+ reduce total) {
    var (hasElt, elt) = lfs.pop();
    while hasElt {
      total += elt;
      (hasElt, elt) = lfs.pop();
    }
  }

As an optimization, the user can register to receive a :class:`~EpochManager.TokenWrapper`, and pass this
to the stack. This can provide significant improvement in performance by up to an order of magnitude
by avoiding the overhead of registering and unregistering for each operation.

.. code-block:: chpl

  var lfs = new LockFreeStack(int);
  forall i in 1..N with (var tok = lfs.getToken()) do lfs.push(i,tok);
  var total : int;
  coforall tid in 1..here.maxTaskPar with (+ reduce total) {
    var tok = lfs.getToken();
    var (hasElt, elt) = lfs.pop(tok);
    while hasElt {
      total += elt;
      (hasElt, elt) = lfs.pop(tok);
    }
  }

Lastly, to safely reclaim memory, the user must explicitly invoke ``tryReclaim``, or else
there will be a memory leak. This must be explicitly invoked so that the user may tune how often
reclamation will be attempted. Reclamation is concurrent-safe, but if called too frequently,
it can add unnecessary overhead. A complete example of what would be considered 'optimal'
usage of this lock-free stack.

.. code-block:: chpl

  var lfs = new LockFreeStack(int);
  forall i in 1..N with (var tok = lfs.getToken()) do lfs.push(i,tok);
  var total : int;
  coforall tid in 1..here.maxTaskPar with (+ reduce total) {
    var tok = lfs.getToken();
    var (hasElt, elt) = lfs.pop(tok);
    var n : int;
    while hasElt {
      total += elt;
      (hasElt, elt) = lfs.pop(tok);
      n += 1;
      if n % GC_THRESHOLD == 0 then lfs.tryReclaim();
    }
  } 

Also provided, is a utility method for draining the stack of all elements,
called ``drain``. This iterator will implicitly call ``tryReclaim`` at the
end and will optimally create one token per task.

.. code-block:: chpl

  var lfs = new LockFreeStack(int);
  forall i in 1..N with (var tok = lfs.getToken()) do lfs.push(i,tok);
  var total = + reduce lfs.drain();

.. [#] Hendler, Danny, Nir Shavit, and Lena Yerushalmi. 
    "A scalable lock-free stack algorithm." Proceedings of the sixteenth annual 
    ACM symposium on Parallelism in algorithms and architectures. ACM, 2004.

.. class:: Node

   .. attribute:: type eltType

   .. attribute:: var val: toNilableIfClassType(eltType)

   .. attribute:: var next: unmanaged nilable Node(eltType)

   .. method:: proc init(val: ?eltType)

   .. method:: proc init(type eltType)

.. class:: LockFreeStack

   .. attribute:: type objType

   .. attribute:: var _top: AtomicObject(unmanaged nilable Node(objType), hasGlobalSupport = true, hasABASupport = false)

   .. attribute:: var _manager = new owned LocalEpochManager()

   .. method:: proc objTypeOpt type

   .. method:: proc init(type objType)

   .. method:: proc getToken(): owned TokenWrapper

   .. method:: proc push(newObj: objType, tok: owned TokenWrapper = getToken())

   .. method:: proc pop(tok: owned TokenWrapper = getToken()): (bool, objType)

   .. itermethod:: iter drain(): objTypeOpt

   .. itermethod:: iter drain(param tag: iterKind): objTypeOpt

   .. method:: proc tryReclaim()

