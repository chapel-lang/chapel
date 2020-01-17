.. _readme-dsi:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Domain Map Standard Interface
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. contents::

============
Introduction
============

This document defines **DSI** -- the Domain map Standard Interface.
It is the interface that a domain map must implement.
This interface is currently in place and is implemented by the standard
domain maps provided with this Chapel distribution.

The specification is split into "phases". The intention is to suggest
an implementation order. The requirements of first phase are relied
upon by the more essential operations on domains and arrays. Later
phases support additional functionality and/or improved performance.
The phasing is approximate and is not required.

Status of this document
-----------------------

This document, as well as the DSI interface itself, are work in progress.
We solicit feedback on both.

The following aspects are not documented yet:

* a summary of the six descriptor classes

* the requirements for a domain map to support
  associative or sparse domains;

* the significance of dsiClone();

* the "local descriptor" classes;

* some required methods.

Additional Resources
--------------------

Implementations of the standard domain maps may be consulted as
examples.  They are available in:

 ::

  $CHPL_HOME/modules/dists
  $CHPL_HOME/modules/layouts

In addition, the following publication provide more background
information on domain maps in general.

- "User-Defined Distributions and Layouts in Chapel: Philosophy and
  Framework." Bradford L. Chamberlain, Steven J. Deitz, David
  Iten, Sung-Eun Choi. 2nd USENIX Workshop on Hot Topics in
  Parallelism, June 2010.

    The high level motivation and overview of DSI.

    https://chapel-lang.org/publications/hotpar10-final.pdf

- "Authoring User-Defined Domain Maps in Chapel."  Bradford
  L. Chamberlain, Sung-Eun Choi, Steven J. Deitz, David Iten, Vassily
  Litvinov. CUG 2011, May 2011.

    A follow-on paper that outlines how to write a domain map.

    https://chapel-lang.org/publications/cug11-final.pdf


========
Overview
========

A domain map must provide three (or more) "global descriptor" classes.
Here they are called ``GlobalDistribution``, ``GlobalDomain``,
and ``GlobalArray``.
Their required fields and methods, as well as suggested naming,
are specified below.

A domain map is allowed to contain/provide features, data structures
and functionality (internally to facilitate implementation and/or
externally for users' benefit) beyond the DSI requirements.
For example, a distribution (i.e. a domain map that distributes data across
multiple locales) will typically define "local descriptor" classes as well.

A domain map author can provide additional functionality to users simply by
implementing methods on "global descriptor" classes. When user code contains a
method call on an array, domain, or distribution that is not handled by one of
the predefined methods, that call is forwarded to the corresponding global
descriptor.


======================================
Phase 1: The Essentials
======================================


class ``GlobalDistribution``
----------------------------

  This class is visible to the users of the domain map: the ``dmap`` wrapper
  in Chapel's `dmapped` clauses wraps instances of this class.
  This class must be a subclass of ``BaseDist``.

.. method:: proc GlobalDistribution.GlobalDistribution() // or with arguments

  Constructor(s)  These are not regulated by DSI - their specifics are
  at the domain map implementor's discretion.

  We suggest providing constructor(s) that accept, as an argument,
  an array of locales over which to distribute, with ``Locales``
  as the default value.

.. method:: proc GlobalDistribution.dsiClone(): GlobalDistribution

  Returns a duplicate of `this`.

.. [TODO: the specifics. E.g. we need to specify when it is not enough
   simply to return `this`. (Cf. the default domain map returns `this`.)]

.. method:: proc GlobalDistribution.dsiDisplayRepresentation(): void

  A debugging method. It implements displayRepresentation()
  on the dmap wrapper.

.. method:: proc GlobalDistribution.dsiEqualDMaps(that: /*some other GlobalDistribution*/): bool

  Return whether or not the two domain maps are "equal" (specify the
  same distribution).  This is invoked when ``==`` is applied to two
  domain maps.


class ``GlobalDomain``
----------------------

  An instance of this class is created whenever a ``GlobalDistribution``-mapped
  Chapel domain is created (e.g. when a domain variable is declared).
  There is a one-to-one correspondence at run time between a
  (conceptual) Chapel domain value and an instance of ``GlobalDomain``.
  For presentation simplicity, ``GlobalDomain``'s properties refer to
  the properties of the corresponding Chapel domain value, when
  clear from the context.

  Depending on the domain kind, this class is returned by the following
  method on ``GlobalDistribution``
  and must be a subclass of the following class:

    ===========   ========================    ===================
    domain kind   creating method             required superclass
    ===========   ========================    ===================
    rectangular   ``dsiNewRectangularDom``    ``BaseRectangularDom``
    associative   ``dsiNewAssociativeDom``    ``BaseAssociativeDom``
    sparse        ``dsiNewSparseDom``         ``BaseSparseDom``
    ===========   ========================    ===================

  It is legal for these methods to return instances of different classes
  in different circumstances. (For example, due to the absence of multiple
  inheritance, different domain kinds require different classes.)

  The DSI requirements apply to each of those classes.
  ``GlobalDomain`` is used in this document to refer to each such class.

  The DSI requirements depend on the domain kind that is supported.
  The following requirements apply when supporting rectangular domains.

.. [TODO: requirements for the other domain kinds.]

.. class:: GlobalDomain


  .. code-block:: chapel

    class GlobalDomain ... {
      param rank: int;
      type idxType;
      param stridable: bool;
      var dist;
      ...
    }

  The fields ``rank``, ``idxType``, ``stridable`` are the attributes
  of the corresponding Chapel domain. (They could be replaced with
  parentheses-less functions of the same names and param/type intents.)

  The field ``dist`` must contain a reference
  to the ``GlobalDistribution`` object
  that created this ``GlobalDomain``. It is used when creating the (runtime)
  type for this domain, which contains the domain's domain map.

.. method: proc GlobalDomain.GlobalDomain() // or with arguments

  Constructor(s). These are not regulated by DSI -- their specifics are
  at the domain map implementor's discretion.

.. method:: proc GlobalDomain.dsiMyDist() return dist;

  Returns this domain's domain map. This procedure should be provided as shown.
  (Exception: see ``dsiLinksDistribution()``.)

.. method:: proc GlobalDistribution.dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool, inds) : GlobalDomain(rank, idxType, stridable)

  This method is invoked when the Chapel program is creating a domain
  value of the type domain(rank, idxType, stridable) mapped using the
  domain map `this` with initial indices `inds`.

  This method returns a new ``GlobalDomain`` instance that will correspond to
  that Chapel domain value, i.e., be that value's runtime representation.
  The field ``dist`` of the returned ``GlobalDomain`` must point to `this`.

.. method:: proc GlobalDomain.dsiGetIndices(): rank * range(idxType, BoundedRangeType.bounded, stridable)

  Returns a tuple of ranges describing the dimensions of this domain.

  ``dsiDims()`` and ``dsiGetIndices()`` have the same specification
  and so may be implemented in terms of one another.

.. method:: proc GlobalDomain.dsiSetIndices(dom: domain(rank, idxType, stridable)): void

  Updates the internal representation of `this`
  to match the index set of ``dom``.

  ``dsiSetIndices`` and ``dsiGetIndices`` are used to assign a domain value to
  a domain variable (or any domain l-value).
  Note: the arrays associated with this domain will be updated
  outside these method.

.. method:: proc GlobalDomain.dsiSetIndices(ranges: rank * range(idxType)): void

  The same as the other ``dsiSetIndices``. Could be implemented like this:

   .. code-block:: chapel

    { dsiSetIndices([(...rangesArg)]); }

  It is used to initialize the index set of the object returned by
  ``dsiNewRectangularDom()`` to the index set of the corresponding Chapel
  domain value.

.. method:: proc GlobalDomain.dsiAssignDomain(rhs: domain, lhsPrivate:bool): void

  Set one domain to another. The receiver is the domain being set.
  The rhs could be any domain, but this function should raise a
  compilation error if the types do not match sufficiently.

  In some cases when this method is called, the LHS can't be shared
  or have any arrays declared over it. In that case, ``lhsPrivate=true``
  is passed.

  Note that the method ``chpl_assignDomainWithGetSetIndices`` can be called 
  to use ``dsiReallocate``/``getIndices``/``dsiSetIndices``/
  ``dsiPostReallocate`` to accomplish the assignment.

.. method:: iter GlobalDomain.these()

  The serial iterator over the indices of this domain.
  Yields values of the type ``idxType`` if ``rank==1``,
  otherwise ``rank*idxType``.

.. method:: iter GlobalDomain.these(param tag) where tag == iterKind.leader
.. method:: iter GlobalDomain.these(param tag, followThis) where tag == iterKind.follower

  The "leader" and "follower" iterators (defined below). They are
  invoked implicitly to implement parallel ("forall") loops over this domain.
  Currently the serial iterator must also be defined for "forall" loops to work.

  The presentation below is tailored to the use of leader/follower iterators
  in domain maps. For a more general discussion, see:

   ::

     $CHPL_HOME/examples/primers/parIters.chpl

  The job of the leader iterator is to:

   * subdivide the given domain into subdomains,

   * introduce parallelism between these subdomains, and

   * place computations for each subdomain on the desired locale,
     as appropriate for this domain map.

  The leader iterator must yield, for each subdomain it chooses,
  a description of that subdomain, created as defined below.
  Parallelism and placement are achieved, for example, by placing the
  yield statements within parallel loops and `on` statements.
  The leader is invoked (implicitly) once per parallel loop or expression
  over the corresponding domain.

  The job of the follower iterator is simply to iterate sequentially
  over a subdomain whose description is yielded by the leader iterator,
  yielding all indices in that subdomain.
  Each time the leader yields a description, the follower is invoked
  (implicitly) with that description passed to its ``followThis`` argument.
  (The argument name must be exactly "followThis".)

  For a zippered loop, only the leader for the first of the zippered
  items is invoked. Each time that leader yields a description,
  all followers are invoked in a zippered manner, with that description
  passed to their ``followThis`` arguments.

  In general, it is up to the implementer of the leaders/followers
  whether and how to support their interoperability in this situation.
  That is, whether and how the description from the leader of one
  zippered item is handled by the follower of another item.
  For example, one could support zippering of similar items
  and generate a compile-time or run-time error when the items
  are not "similar".

  .. [example? e.g. associative domains over the same type?]

  DSI requires interoperability between domain maps only
  for *rectangular* domains, by prescribing:

   A. how the leader builds the description of a subdomain, and

   B. in what order the follower yields the indices of that subdomain.

A. Subdomain Description
^^^^^^^^^^^^^^^^^^^^^^^^

  Let ``Dwhole`` be the Chapel domain corresponding to this ``GlobalDomain``.

  Let ``Dsub`` be a subdomain chosen by the leader iterator.

  Let ``DD`` be the number of dimensions in Dwhole and Dsub.

  The description of ``Dsub`` is its "densification" w.r.t. ``Dwhole``:

   ::

    // densification of a domain is a tuple of densifications
    // of ranges for each dimension
    densify(Dsub, Dwhole) =
      ( densify(Dsub.dim(1), Dwhole.dim(1)),
        ...,
        densify(Dsub.dim(DD), Dwhole.dim(DD)) )

    // densification of a range is the sequence of densifications
    // of its elements, preserving their order
    if Rsub describes the sequence
      i_1, ..., i_N
    then densify(Rsub, Rwhole) describes the sequence
      densify(i_1, Rwhole), ..., densify(i_N, Rwhole)

    For example, densify(Rwhole, Rwhole) == 0.. #(Rwhole.length)

    // densification of an index is its indexOrder
    densify(i, Rwhole) = Rwhole.indexOrder(i)

  The optional module DSIUtil provides the following implementations
  of densifications and the reverse transformation:

   ::

    densify(Dsub, Dwhole)
    densify(RSsub, RSwhole)  // on tuples of ranges
    densify(Rsub, Rwhole)
    unDensify(Dsub, Dwhole)
    unDensify(RSsub, RSwhole)  // on tuples of ranges
    unDensify(Rsub, Rwhole)

  Hint: when developing a new domain map, skip densification and
  de-densification, but ensure it is used in zippering only
  with domains/arrays of the same domain map.

B. Follower Yield Order
^^^^^^^^^^^^^^^^^^^^^^^

  The follower iterator must traverse the subdomain for which the above
  description is created in the order given by the default domain map.
  (That order can be observed, e.g., by assigning ``Rsub`` to a domain
  variable whose type does not specify a domain map explicitly,
  then invoking ``write()`` on that variable.)

  (Interoperability of domain maps implies that the follower must
  accept a densification of any subdomain of ``Dwhole``, not just the
  ones that can be generated by its leader.)

.. method:: iter GlobalDomain.these(param tag) where tag == iterKind.standalone

  A "standalone" parallel iterator. It is optional. If it is provided,
  it is used in non-zippered "forall" loops
  instead of a combination of leader+follower iterators.

.. method:: proc GlobalDomain.dsiSerialWrite(f: Writer): void

  .. [TODO: GlobalDomain.dsiSerialRead(f: Reader): void]

  Writes out the domain to the given Writer (e.g. a file or stdout) serially.
  Is used to implement write() on the corresponding domain.

  This method will typically invoke f.write() on ``GlobalDomain``'s components
  (e.g. bounds) and strings (e.g. "[" and "]"). Such invocations, whether
  direct or indirect, must occur within the same task that ``dsiSerialWrite()``
  is invoked in, and not within any `on` statements, direct or indirect.
  Otherwise a deadlock may occur.
  (Invoking ``write()`` on ``Writers`` other than ``f``
  is not affected by this.)

  ``dsiSerialWrite()`` will always be invoked on ``Locales(0)``.

  .. [TODO: will this remain this way or is it specific to our implementation?]

.. method:: proc GlobalDomain.dsiDisplayRepresentation(): void

  A debugging method. It implements ``displayRepresentation()``
  on the corresponding Chapel domain value.


class ``GlobalArray``
---------------------

  An instance of this class is created whenever a Chapel array is created
  over a domain represented by a ``GlobalDomain``.
  There is a one-to-one correspondence at run time between a
  (conceptual) Chapel array value and an instance of ``GlobalArray``.

  This class is returned by ``GlobalDomain.dsiBuildArray``.
  It must be a subclass of ``BaseArr``.

  It is legal for ``GlobalDomain.dsiBuildArray`` to return instances of
  different classes in different circumstances.
  The DSI requirements apply to each of those classes.
  Here ``GlobalArray`` refers to each such class.

  .. code-block:: chapel

    class GlobalArray ... {
      type eltType;
      var dom;
      ...
    }

  The field ``eltType`` gives the type of the array elements.

  The field ``dom`` must contain a reference to the ``GlobalDomain`` object
  that created this ``GlobalArray``. This is used when creating the (runtime)
  type for this array, which contains the array's domain.

.. method:: proc GlobalArray.GlobalArray() // or with arguments

  Constructor(s). These are not regulated by DSI -- their specifics are
  at the domain map implementor's discretion.

.. method:: proc GlobalDomain.dsiBuildArray(type eltType) : GlobalArray(eltType, this.type)

  This method is invoked when the Chapel program is creating
  an array value over the domain `this` with the element type ``eltType``.
  This method returns a new ``GlobalArray`` instance that will correspond to
  that Chapel array value, i.e., be that value's runtime representation.
  The field ``dom`` of the returned ``GlobalDomain`` must point to `this`.

.. method:: proc GlobalArray.dsiGetBaseDom() return dom;

  Returns this array's ``GlobalDomain``.
  This procedure should be provided as shown.

.. method:: proc GlobalArray.dsiAccess(indexx) var: eltType

  Given an index, returns the corresponding array element (as an l-value).

  The domain map implementer is allowed to restrict the type of indexx
  that this method accepts.

.. method:: proc GlobalArray.dsiSerialWrite(f: Writer): void

  .. [TODO: GlobalArray.dsiSerialRead(f: Reader): void]

  Writes out the array to the given ``Writer``
  (e.g. a ``file`` or ``stdout``) serially.
  Is used to implement ``write()`` on the corresponding array.

  The restrictions on this method are the same as on
  ``GlobalDomain.dsiSerialWrite()``.

.. method:: iter GlobalArray.these() var: eltType

  The serial iterator over the elements of this array.

.. method:: iter GlobalArray.these(param tag) where tag == iterKind.leader
.. method:: iter GlobalArray.these(param tag, followThis) var  where tag == iterKind.follower
.. method:: iter GlobalArray.these(param tag) where tag == iterKind.standalone

  The leader, follower, and standalone iterators.

  These are defined in the same way as the leader and follower for
  ``GlobalDomain``, except the follower must yield array locations.

  Given a subdomain description passed to the ``followThis`` argument,
  the ``GlobalArray`` follower iterator must yield array locations
  corresponding to the indices yielded by the ``GlobalDomain`` follower,
  in the same order.

.. method:: proc GlobalArray.dsiReallocate(d: domain): void

  .. [TODO: GlobalArray.dsiPostReallocate]

  When this array's domain is assigned a new value, say ``newDom``,
  first ``dsiReallocate(newDom)`` is invoked on this array.
  Then, ``dsiSetIndices(newDom)`` is invoked on this array's ``dom``.

  Correspondingly, ``dsiReallocate`` needs to adjust everything
  that won't be taken care of in ``dsiSetIndices``.

  NOTE: the formal's name must be exactly ``d``
  at present (due to compiler specifics).

  NOTE: this method can/should be a no-op if:

   * ``GlobalArray`` stores its array elements in Chapel array(s), and

   * the domain(s) of those array(s) are updated by ``dsiSetIndices``,

  as in the following simplified example:

   .. code-block:: chapel

    class GlobalDomain : BaseRectangularDom {
      // required
      param rank: int;
      type idxType;
      param stridable: bool;
      const dist;
      // for example, store indices as a single Chapel domain
      var myIndices: domain(rank, idxType, stridable);
    }

    proc GlobalDomain.dsiSetIndices(dom: domain(rank,idxType,stridable)): void
    { myIndices = dom; }

    class GlobalArray : BaseArr {
      // required
      type eltType;
      const dom;
      // for example, store elements as a single Chapel array
      var myElements: [dom.myIndices] eltType;
    }

  Given one instance of each class, say ``gd`` and ``ga``,
  where ``ga.dom == gd``
  the domain of ``ga.myElements`` is ``gd.myIndices``.
  A call ``gd.dsiSetIndices(newDom)`` updates ``gd.myIndices``, which, in turn,
  resizes ``ga.myElements``, according to Chapel's array semantics.
  Nothing remains to do in ``dsiReallocate()``.

  (Behind the scene, resizing of ``ga.myElements`` is implemented by
  ``dsiReallocate`` of the default domain map. That's the domain map
  that ``myIndices`` is distributed with, since ``myIndices``'s type
  provides no explicit domain map.)

.. method:: proc GlobalArray.dsiDisplayRepresentation(): void

  A debugging method. It implements ``displayRepresentation()``
  on the corresponding Chapel array value.


Naming
------

Once you have chosen the name for your domain map, say, MyMap,
we suggest naming the descriptor classes as follows:

  ======================   ================
  in this document         in your code
  ======================   ================
  ``GlobalDistribution``   ``MyMapDist``
  ``GlobalDomain``         ``MyMapDom``
  ``GlobalArray``          ``MyMapArr``
  ``LocalDistribution``    ``LocMyMapDist``
  ``LocalDomain``          ``LocMyMapDom``
  ``LocalArray``           ``LocMyMapArr``
  ======================   ================

Some domain maps in this Chapel distribution use just the domain map
name for their ``GlobalDistribution`` classes, e.g. Block and Cyclic.


=================================
Phase 2: Additional Operations
=================================

The operations in this phase are required by DSI.
However, if a domain map is in use by a limited set of applications,
these operations do not need to be implemented up front. Instead,
each of them could be implemented later, when the need arises.
The "unresolved call" compilation errors could be used
as an indication of what procedure(s) need to be defined.

.. method:: proc GlobalDistribution.dsiIndexToLocale(indexx): locale

  Given an index ``indexx``, returns the locale that "owns" that index,
  i.e. on which the corresponding data is located.
  This is used to implement ``idxToLocale()`` on the ``dmap`` wrapper.

  The domain map implementer is allowed to restrict the type of ``indexx``
  that this method accepts.

.. method:: proc GlobalDomain.dsiDim(dim: int): range(idxType, BoundedRangeType.bounded, stridable)

.. method:: proc GlobalDomain.dsiDims(): rank * range(idxType, BoundedRangeType.bounded, stridable)

.. method:: proc GlobalDomain.dsiLow

.. method:: proc GlobalDomain.dsiHigh

.. method:: proc GlobalDomain.dsiStride

.. method:: proc GlobalDomain.dsiNumIndices

.. method:: proc GlobalDomain.dsiMember(indexx)

.. method:: proc GlobalDomain.dsiIndexOrder(indexx)

  These methods implement the corresponding queries
  (``dim``, ``dims``, ``low``, etc.)
  of the domain value for which this ``GlobalDomain`` instance was created.
  For example, ``dsiDim(d)`` returns the range describing the domain's
  ``d``-th dimension.

  ``dsiDims()`` and ``dsiGetIndices()`` have the same specification
  and so may be implemented in terms of one another.

.. [TODO: the following seems correct. The returned object can be an instance
  of a different class than the receiver, at the implementor's discretion.
  That class must satisfy the requirements on ``GlobalDomain`` defined in this
  document.]

.. method:: proc GlobalDomain.linksDistribution() param

.. method:: proc GlobalDomain.dsiLinksDistribution()

  Typically these should not be defined.

  If the domains mapped using ``GlobalDistribution`` do NOT need to be tracked
  and the ``GlobalDistribution`` itself does NOT need to be reference counted,
  these two methods should be defined to return `false`;
  Also in this case ``dsiMyDist()`` does not need to be defined.


=================================
Phase 3: Privatization
=================================


What is privatization?
----------------------

Privatization of an object ``X`` means providing a local copy of ``X`` on
each locale. Such a copy is called the "privatized copy".
On ``X.locale``, ``X`` itself serves as the privatized copy.
We refer to ``X`` as the "original object".

Privatization aims at reducing communication between locales.
When ``X`` needs to be accessed (by reading its fields or invoking
its methods) from another locale, its privatized copy
on the current locale is used instead.
Therefore, communication to ``X.locale`` is eliminated.


How does privatization work with DSI?
-------------------------------------

Privatization is optional in DSI. Each of the global descriptor classes
can support privatization independently of the others. A class indicates
to the Chapel implementation whether it supports privatization via
its method ``dsiSupportsPrivatization()`` (see below).

Layouts (i.e. the domain maps that do not distribute domains across locales)
can potentially benefit from privatization. However, the standard
layouts do not support it.

The remainder of DSI privatization requirements must be implemented
by each global descriptor class that chooses to support privatization.

The domain map implementation must provide methods to:

* create a privatized copy given the original object, and

* update a privatized copy when some other privatized copy changes
  (see "reprivatization" below).

The Chapel implementation:

* invokes DSI privatization methods to create or update privatized copies, and

* redirects original object accesses to its privatized copies.

The Chapel implementation creates privatized copies (over *all* locales)
greedily as follows (if that class supports privatization):

* of a ``GlobalDistribution`` - when it is wrapped in ``new dmap()``
  and when that wrapper is copied;

* of a ``GlobalDomain`` or ``GlobalArray`` - when the corresponding
  Chapel domain or array is created.


What is reprivatization?
------------------------

Should any privatized copy be modified, the changes need to be
propagated to all the other privatized copies.
This propagation is called reprivatization.

The domain map implementation provides methods to update a privatized copy.
The Chapel implementation invokes these methods when necessary.


DSI privatization requirements
------------------------------

The following requirements apply individually to each global descriptor
class that chooses to support privatization.
``Global`` denotes such a class.

.. method:: proc Global.dsiSupportsPrivatization() param return true;

  Returns `true` to indicate that privatization is supported.
  NOTE: do not specify the return type (due to a bug in the compiler).


.. code-block:: chapel

    class Global ... {
      ...
      var pid = -1;
      ...
    }

The field ``pid`` should be provided as shown.
It should not be accessed by the DSI implementation except
in conjunction with ``chpl_getPrivatizedCopy()`` as discussed later.

.. method:: proc Global.dsiGetPrivatizeData()

  Returns the data to be used as the argument to ``dsiPrivatize()``.
  It can be a tuple of values or any other type, at the implementor's
  discretion. See ``dsiPrivatize()`` for explanation.

.. method:: proc Global.dsiPrivatize(privatizeData): Global

  Returns a privatized copy of `this`. The Chapel implementation
  invokes this method as follows:

  * on the locale where the privatized copy is to be located,

  * `this` is either the original object or its privatized copy
    created by ``dsiPrivatize`` on some other locale,

  * ``privatizeData`` is the result of invoking dsiGetPrivatizeData()
    on the original object.

  Typically ``dsiPrivatize()`` will need to copy and/or privatize
  some information from the original object into the privatized copy
  being created. This information could be obtained by querying `this`
  directly. Alternatively, it could be passed from the original object
  via ``privatizeData``. This approach could allow the needed information
  to be bundled into the original active message, rather than requiring
  additional communications when accessing `this`.

.. method:: proc Global.dsiGetReprivatizeData()

  Similar do ``dsiGetPrivatizeData()``, except the result is used
  as the argument to ``dsiReprivatize()``.

.. method:: proc Global.dsiReprivatize(other: Global, reprivatizeData): void

  Updates a privatized copy.

  `this` is the object to be updated as part of reprivatization.

  ``other`` is either the object whose modification originally
  necessitated reprivatization or one of the privatized copies
  that have already been updated for that modification
  by ``dsiReprivatize()``.

  ``reprivatizeData`` is the result of invoking dsiGetReprivatizeData()
  on the originally-modified object.

  Q: What modifications must ``dsiReprivatize()`` reflect?

  A: Any changes that the domain map implementation may perform on
  an instance of ``Global`` (or its privatized copy) after that instance
  has been created and privatized.

  For example, ``GlobalDomain.dsiReprivatize()`` must reflect any changes
  that ``GlobalDomain.dsiSetIndices()`` may perform.


Tip: testing privatization
--------------------------

In addition to the usual correctness testing, it may be useful to check
whether array access is purely local, i.e., results in no communication.
This can be done using `local` statements.

(A `local` statement performs runtime checks that report an error
whenever any communication occurs within the its body.)

Here is a simple example:

 .. code-block:: chapel

  // declare an array that uses the domain map to be tested
  var A: ...;

  // initialize it
  A = value1;

  forall loc in (locales that A is distributed over) do
    // run the check on each locale
    on loc {
      // value2 to be different from value1
      var valTemp = value2;
      const idxTemp = (an index (tuple) that is mapped to the locale 'loc');
      local {
        // Access the array.
	// This statement succeeds if there is no communication.
        valTemp = A[idxTemp];
      }
      // reference the read value
      assert(valTemp == value1);
    }


Tip: privatized copies should reference privatized copies
---------------------------------------------------------

The global descriptor classes are required to reference each other
(e.g. ``GlobalArray.dom`` references ``GlobalDomain``; ``GlobalDomain.dist``
references ``GlobalDistribution``). Therefore, if it is desired to
eliminate communication completely upon array references,
all the three global descriptor classes may have to be privatized.
When one descriptor object references another, a privatized copy of
the former needs to reference a privatized copy of the latter.

To obtain a privatized copy of an object, e.g. for use within
``dsiPrivatize()``, use the following procedure:

 .. code-block:: chapel

  proc chpl_getPrivatizedCopy(type objectType, objectPid:int): objectType

Notes:

* The first argument is type of the object being privatized.

* The second argument is the original object's ``pid`` field.

* The procedure returns a privatized copy of the original object.

* The procedure can be applied only to objects that have already
  been privatized.

  - A ``GlobalArray`` is privatized after its ``GlobalDomain``, which is
    privatized after its ``GlobalDistribution``.

  - The ``pid`` field is set to a different value than ``-1``
    when an object is privatized.

Here is an example of using it for privatizing ``GlobalDomain``,
assuming that ``GlobalDistribution`` also supports privatization:

 .. code-block:: chapel

  proc GlobalDomain.dsiGetPrivatizeData() {
    // include the desired 'pid'
    return (this.dist.pid, ... other data as needed ...);
  }

  proc GlobalDomain.dsiPrivatize(privatizeData) {
    // extract the 'pid' provided above
    const distPid = privatizeData(1);
    // obtain the privatized copy of the GlobalDistribution object
    const privatizedDMap = chpl_getPrivatizedCopy(this.dist.type, distPid);

    return new GlobalDomain(dist = privatizedDMap,
                            ... other fields as needed ...)
  }


Tip: "privatize" the domains and arrays used in the implementation
------------------------------------------------------------------

If a global descriptor class to be privatized uses domains and arrays,
those need to be "privatized", too, to reduce communication. In the
case of domains and arrays mapped using the default layout,
their "privatization" is achieved by copying.

Careful consideration is needed to eliminate all communication.
For example:

* Copying an array preserves the source's domain - unless
  the destination's domain is declared explicitly.

* Copying a domain preserves the source's domain map - unless
  the destination's type is declared explicitly.

* If an array's domain or a domain's domain map is declared explicitly,
  ensure that privatized copies of those are used.

* There is no need to privatize the default layout.
  Specifically, copying a domain declared without an explicit domain map
  is sufficient to privatize that domain.

* The same considerations are valid for reprivatization.

Here is an example of "privatizing" domains/arrays within ``GlobalDomain``.
The key insight here is that ``auxArrayG``'s domain needs to be "privatized".
Cf. the domains of ``auxArrayED1`` and ``auxArrayED2`` are created
implicitly for each ``GlobalDomain`` object and so will be local in any case.

 .. code-block:: chapel

  class GlobalDomain ... {
    ...

    // this field is generic
    var auxArrayG;

    // domain is specified explicitly
    var auxArrayED1: [1..10] int;

    // similar
    var auxDomain: domain(1);
    var auxArrayED2: [auxDomain] int;
  }

  proc GlobalDomain.dsiGetPrivatizeData() {
    return (this.dist.pid, auxArrayG,
            auxArrayED1, auxDomain, auxArrayED2, ...);
  }

  proc GlobalDomain.dsiPrivatize(privatizeData) {

    // To privatize auxArrayG, we must "privatize" its domain first.
    // No need to declare privDom's type if auxArrayG uses the default layout.
    var privDom = privatizeData(2).domain;
    var privArr: [privDom] privatizeData(2).eltType = privatizeData(2);

    // If the following were used, privArr.domain would be the same
    // as privatizeData(2).domain, i.e., it would not be privatized.
    //var privArr = privatizeData(2);

    return new GlobalDomain(..., auxArrayG = privArr,
      // the other fields can be simply copied
      auxArrayED1 = privatizeData(3),
      auxDomain   = privatizeData(4),
      auxArrayED2 = privatizeData(5));

  }

=================================
Phase 4: Bulk-Transfer Interface
=================================

What is the bulk-transfer interface?
------------------------------------

While the Chapel language does not require a specific implementation for array
assignment, in practice the values are assigned individually:

 .. code-block:: chapel

  for[all] (d, s) in zip(dest, src) {
    d = s;
  }

When assignment occurs between arrays on different locales, this can result in
significant communication overhead. The bulk-transfer interface aims at
reducing communication between locales by enabling domain maps to handle
certain kinds of assignments themselves.

How does the bulk-transfer interface work with DSI?
---------------------------------------------------

The bulk-transfer interface is an optional interface supported only on array
descriptors. The following methods are prefixed with ``doi`` for "Domain map
Optional Interface." An array descriptor can implement one or more of these
methods to opt-in to support for bulk-transfers:

.. method:: proc GlobalArray.doiBulkTransferToKnown(myDom:domain, otherClass, otherDom:domain) : bool

  Perform an array assignment from ``this`` to ``otherClass``.

.. method:: proc GlobalArray.doiBulkTransferFromKnown(myDom:domain, otherClass, otherDom:domain) : bool

  Perform an array assignment from ``otherClass`` to ``this``

.. method:: proc GlobalArray.doiBulkTransferToAny(myDom:domain, otherClass, otherDom:domain) : bool

  Perform an array assignment from ``this`` to ``otherClass``.

.. method:: proc GlobalArray.doiBulkTransferFromAny(myDom:domain, otherClass, otherDom:domain) : bool

  Perform an array assignment from ``otherClass`` to ``this``

Each method shares these arguments:

* ``myDom:domain`` - the indices to be transferred to/from ``this``

* ``otherClass`` - a class inheriting from ``BaseArr`` that represents the
  other array in the assignment

* ``otherDom:domain`` - the indices to be transferred to/from ``otherClass``

Callers of these methods must satisfy the following guarantees for these
arguments:

* ``this.rank == myDom.rank``

* ``otherClass.rank == otherDom.rank``

* ``myDom.size == otherDom.size``

.. note::

  ``myDom`` and ``otherDom`` are not required to be distributed, even if the
  corresponding arrays are distributed. They simply represent the indices
  involved in the transfer.

Each method returns a ``bool`` indicating whether the assignment occurred. This
allows domain maps to resort to the Chapel implementation of array assignment
if they are incapable or unwilling to perform the assignment themselves.

Known and Any methods
---------------------

There are two kinds of transfers accounted for in this interface.

The first kind of transfer is a ``Known`` transfer. These are transfers that
the implementer believes to be optimal. Typically this means that the
implementer knows the type and internals of the other domain map involved in
the transfer.

The second kind of transfer is an ``Any`` transfer. These are transfers to/from
domain maps whose type and internals are **not** known. These transfers
typically rely on attempting to have the unknown domain map transfer to/from
the internal data of the domain map on which the method is implemented. See
`Implementing 'Any' Methods`_ for more detail.

This interface recognizes these kinds of transfers because otherwise it is
difficult to determine which distribution implements the best transfer.

For example, the author of ``PopularDist`` could implement ``Any`` methods for
better performance with unknown third-party domain maps. The author of
``NewDist`` could implement ``Known`` methods to ``PopularDist`` due to its
popularity. By doing so the author of ``NewDist`` has informed Chapel that it
knows better than ``PopularDist`` and a more optimal transfer occurs.

Chapel will call ``Known`` methods before ``Any`` methods if they can be
resolved. A recommended convention is to implement ``Known`` methods with a
where-clause that constrains the type of the ``otherClass`` argument. This
prevents the ``Known`` method from being called with unknown types.

.. note::

  There is no defined preference for ``To`` and ``From`` methods. Domain map
  authors should not currently rely on a particular direction being attempted
  first.

When Does a Bulk-Transfer Occur?
--------------------------------

There are a number of conditions that need to be true for a bulk-transfer to
occur:

* The config param ``useBulkTransfer`` must be true

* The element types of the arrays must be identical

* The element type must be one of the following:

    * ``integral``
    * ``real``
    * ``complex``
    * records or tuples that are "Plain Old Data"

The element type is restricted in order to preserve copy semantics of each
element. For example, ``integral`` types can be bit-copied and so have no
side-effects. A record with a copy-initializer may have side effects that
should not be eliminated by the bulk-transfer optimization. In order to avoid
eliminating these side-effects, only records or tuples that are "Plain Old
Data" are considered valid for bulk-transfers. The (currently undocumented)
function ``isPODType(t)`` returns a param boolean indicating whether the given
type is "Plain Old Data".

The ``useBulkTransfer`` config param is ``true`` by default but can be disabled
at compilation time. For example:

.. code-block:: bash

  chpl myProgram.chpl -suseBulkTransfer=false

Implementing 'Any' Methods
--------------------------

`Any <Known and Any Methods_>`_ methods are generally implemented by
transferring data of an internal type of ``this`` to/from a subset of indices
in ``otherClass``. For example, a distribution can be implemented in terms of
Chapel's default rectangular arrays. An ``Any`` method on that distribution
could be implemented by attempting to transfer data to/from ``otherClass`` to
a default rectangular array, like so (in pseudo-chapel):

.. code-block:: none

  // For a single locale's data
  var localDom = localData.domain[myDom]; // local subset of indices for ``this``
  var locOther = <translate localDom to otherDom's coordinates>;
  otherClass.doiBulkTransfer[To|From][Any|Known](locOther, localData, localDom);

Implementers who call these ``doiBulkTransfer`` methods should note that they
can return ``false``, meaning the caller needs to complete the transfer.

.. warning::

  Beware of implementing bulk-transfers by manually calling other
  ``doiBulkTransfer`` methods. For example:

  .. code-block:: chapel

    proc GlobalArray.doiBulkTransferFromAny(myDom:domain, otherClass, otherDom:domain) {
      otherClass.doiBulkTransferToAny(otherDom, this, myDom);
    }

  This pattern can lead to potential recursion depending on the implementation
  of the other domain map's bulk-transfer method. Instead of implementing the
  example method above, the author could have not implemented it at all and
  let the Chapel internals invoke the ``doiBulkTransferToAny`` directly.

Transfers for Aliasing Domain Maps
----------------------------------

It is valid to write a domain map that aliases another domain map's data. For
example, a user could write their own variant of a Chapel array slice in terms
of the Domain Map Standard Interface.

When aliasing other domain maps it is recommended to only implement `Known <Known and Any Methods_>`_
transfers that forward to the underlying domain map's bulk-transfer methods.
By implementing `Known <Known and Any Methods_>`_ transfers the underlying domain map will have a
chance to perform a more optimal transfer before the `Any <Known and Any Methods_>`_ transfers are
attempted.

Transfers for Rank-changes
--------------------------

Implementing a bulk-transfer when rank-changed can be tricky. In the interest
of simplicity, Chapel by default will not attempt to call bulk-transfer methods
on arrays that have been rank-changed.

Domain map authors can opt-in to handling rank changes by implementing the
following method:

.. method:: proc GlobalArray.doiCanBulkTransferRankChange() param : bool

If this method is resolvable and returns ``true`` then Chapel will attempt
to call bulk-transfer methods on the array.

Domain map authors should note that this means that ``this.rank`` may not be
equal to ``otherClass.rank`` when rank-changes are involved. They should also
note that while ``myDom`` and ``otherDom`` are of the same size, their
dimensions may not match in size. For example:

.. code-block:: chapel

  var A, B : [1..10, 1..10] int;
  A[.., 1] = B[1, ..];

The call to a bulk-transfer method could look like:

.. code-block:: chapel

  classA.doiBulkTransferFromKnown({1..10, 1..1}, classB, {1..1, 1..10});

The implementer of this method is left with the task of determining which
dimensions were rank-changed.
