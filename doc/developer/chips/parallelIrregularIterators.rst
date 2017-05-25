Parallel Iterators On Irregular Arrays and Domains
==================================================

Status
  Draft

Authors
  Engin Kayraklioglu,
  Michael Ferguson

Abstract
--------

This document covers semantics of zippering regular and irregular domains and
arrays defined on them. Rather than going into rigorous details, it covers
what an intended behaviour could be and what are possible ways of implementing
l/f iterators providing such support. In most cases error/bounds checking issues
are handled superficially, if at all.

Introduction
------------

Current sparse array iterator semantics
+++++++++++++++++++++++++++++++++++++++

While thinking about zippering sparse iterators with dense ones, sparse array
iterators always felt a bit unnatural to me(although I cannot say that it's
wrong)::

    iter these() ref {
      for i in 1..dom.nnz do yield data[i];
    }

What makes me a bit uneasy here is that IRVs are never yielded by sparse arrays.
Although I find this a bit counter-intuitive, it makes sense when you think
about::

  forall v in SparseArr do v = 5;

An iterator that would yield IRVs would not work in this case as you cannot
assign to a non existing index of a sparse array(First you have to add the index
to array's domain explicitly).

One option that I can think of is to have a ref/nonref pair of l/f iterator
sets. In that case the following snippet would write only to existing indices in
the domain::

  forall v in SparseArr do v = 5;

whereas following one would print out IRVs as well::

  forall v in SparseArr do writeln(v);

ref/nonref functions of same name resolve correctly today. But I'd be surprised
if what I proposed works. At the same time, I don't expect that implementing
support for this would be difficult.

I have mixed feelings towards this idea as the iterator behavior depends on
usage.

Current sparse domain iterator semantics
++++++++++++++++++++++++++++++++++++++++

I don't have much to talk about current sparse domain iterator behaviour.

How To Interpret Cross-Type Zippered Loops
------------------------------------------

Dense/Sparse - Sparse/Dense Zippering
+++++++++++++++++++++++++++++++++++++

Domains
_______

::

  forall (i,j) in zip(RectDom, SparseDom) do ...
  forall (i,j) in zip(SparseDom, RectDom) do ...

I cannot think of many use cases that would require zippering such iterators. A
user might want to do it where domains don't overlap completely and hope that
follower indices would be offset according to difference between two iterators.
However, I believe that kind of burden should not be loaded on top of l/f
iterators. Such index offsetting can be handled at user level through tuple
arithmetic easily.

As I cannot explain why someone would need this, I don't have a strong opinion
on what kind of check should be run on ``SparseDom.domain._value.parentDomain`` and
``RectDom``. In general, it seems to me that it is not easy to define a good set of
conditions that increases safety while keeping performance overhead at bay. My
general opinion on l/f iterators(second part of this document) makes me think we
shouldn't be worried about them at all, as I personally support very loose
coupling between leaders and followers.

Arrays
______

::

  forall (i,j) in zip(RectArr, SparseArr) do ...
  forall (i,j) in zip(SparseArr, RectArr) do ...

When we are talking about zippering array iterators, there are a couple scenarios
I can think of why we would need such loops.

First, this looks like something that is going to fire when sparse/dense arrays
are assigned to each other. With ref/nonref sparse array iterator pairs, both of
the above zippered loops would behave very naturally.::

  RectArr = SparseArr;  // RectArray would have exact same values as sparse array
                        // including IRVs

If, on the other hand, user wants to avoid assigning IRVs, they should use
following::

  forall i in SparseArr.domain do RectArr[i] = SparseArr[i];

In this scenario, array assignment in the other direction means populating a
sparse array using a dense list of values. Personally, this feels intuitive to
me::

    SparseArr = RectArr; //populate a sparse array using _all_ values in RectArr

So, in effect, this is same as::

  SparseArr._value.data = RectArr;


If, ``RectArr`` is a "dense version" of ``SparseArr``,(ie indices need to be
"plucked out" from ``RectArr``) user has to use domain iterators, a full code
would look like::

  const Dom = {1..10};
  var DenseArr: [DenseDom] int;

  var SparseDom: sparse subdomain(Dom);
  var SparseArr: [SparseDom] int;

  SparseDom += [3,5];

  forall i in DenseDom do DenseArr[i] = f(i);

  forall i in SparseDom do SparseArr[i] = DenseArr[i]; //no need for zip

Dense/Assoc - Assoc/Dense zippering
+++++++++++++++++++++++++++++++++++

Currently, both cases generate different compile time errors. Even if that's the
desired behaviour, errors are thrown for the wrong reasons and messages are not
very helpful.

Regardsless, there are some possible scenarios I can think of for zippering such
iterators.

Where ``idxType`` s are different
---------------------------------

Consider following snippet where unique ids added to objects in an associative
array::

  var AssocDom: domain(string);
  var dictionary: [AssocDom] dictObject;

  forall (obj,i) in zip(dictionary, 1..) do obj.setUID(i);

Here zippering order shouldn't have any effect on the behaviour.

Where ``idxType`` s are same::
------------------------------

::

  var AssocDom: domain(int);
  AssocDom += [1,2,3,4,5];

  var RegDom = {1..5};

  forall (i,j) in zip(AssocDom, RegDom) do ...

In which order indices would be yielded from either domain is a bit unclear.
However, user must be aware of unordered nature of associative domains, therefore
shouldn't write such code if specific ordering is desired. When I read this code
all I can interpret is that *some* associative indices will be matched with
regular indices.

In that sense, implementation and sematic-wise, I do not see any difference when
``idxType`` s are some or different.

In terms of behaviour, I don't see any valuable difference between associative
arrays and domains.

A Possible Implementation
-------------------------

If we want to allow cross-type zippering, semantics should be very simple. Going
back to the basics, if we have::

  forall (i,j) in zip(iter1, iter2) do something();

should always be interpreted exactly as::

  i1 = iter1.first;
  i2 = iter2.first;
  while(i1 && i2) {
    something();
    i1 = i1.next;
    i2 = i2.next;
  }

(I personally believe that there shouldn't be any size checks in zippered
iterator. i.e. if one of them returns, then the loop should end gracefully)

To implement such semantics, l/f iterators should follow the basic idea of
yielding a single range and following it. A rough sketch is::

  iter these(param tag) where tag==iterKind.leader {
    var numThings; //how many things I want this iterator to yield
    var numTasks;
    coforall t in numTasks do
      // Ideally I would like something like
      // yield from chunkIterator(numThings, numTasks);
      yield getChunk(numThings, numTasks,t);
  }

  iter these(param tag, followThis) where tag==iterKind.follower {
    for i in followThis do
      yield thingIWantToIterate[i];
  }

Notes
-----

- Zero-based ranges should be enforced for compatibility. ``getChunk`` and
  ``chunkIterator`` in the above implementation can do shifting.

- For unbounded ranges or other unbounded iterators(input streams?), a ``config
  param maxChunkSize`` can be used to chunk up the unbounded space.

- These suggestions might break some operator promotions that rely on current
  semantics. In which case, those operators should have specific overloads.

- This l/f implementation is different than e.g. current ``DefaultRectangular``
  iterators as they are rank-aware. I think rank-oblivious iterators can help
  answer hard questions such as zippering domains/arrays of different ranks. One
  can easily "flatten" a multi dimensional array::

    const multiDom = {1..N, 1..N};
    const singleDom = {1..N**2};

    forall (i,j) in zip(multiDom,singleDom) do singleArr[j] = multiArr[i];
    // or even
    forall (s,m) in (singleDimArr, multiDimArr) do s = m;

  Note that the order of zippered iterators should not matter.

- I don't have a strong opinion on exactly what should be yielded by leader. A
  single range should suffice to provide basic functionality. However, we might
  want to pass additional data for checking. e.g. ``numElems`` for halting if
  ``boundsCheck==true``

  Best approach I can think of is having a record in the internal modules with
  fields that cover the bare minimums of desired functionality. Then, most
  common internal leaders should yield variables of that record type. If more
  *exotic* behaviour is desired, a child record can be implemented. This would
  allow those exotic iterators to be zippered with standard ones. When *exotic*
  behaviour is desired, such iterator should be the leader. If an *exotic*
  follower follows a standard leader, it can be detected through type system or
  metaprogramming. After that follower can chose to (1) change its behaviour (2)
  generate a compile time error.
