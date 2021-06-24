Glossary
========

| Abbrev | concept                           | AST               |
| :----- | :-------------------------------- | :---------------- |
| ifc    | interface                         | `InterfaceSymbol` |
| AT     | associated type                   | `ConstrainedType` |
| reqFn  | required function of an interface | `FnSymbol`        |
| ICfun  | interface-constrained function    | `FnSymbol`        |
| fn     | function                          | `FnSymbol`        |
| icon   | interface constraint              | `IfcConstraint`   |
| istm   | implements statement              | `ImplementsStmt`  |

**TG** or **traditional generic** --
the kind of generic Chapel has always had

**IC** or **interface generic** or interface-constrained generic --
what this writeup is about

**witness** is one of:
* the istm that satisfies the interface constraint at hand
* the implementation of a reqFn provided by this istm
* the instantiation of an AT provided by this istm

These PRs have extensive writeups on implementation details:
https://github.com/chapel-lang/chapel/pull/16813
https://github.com/chapel-lang/chapel/pull/17618
https://github.com/chapel-lang/chapel/pull/17678
Also, the file `compiler/resolution/interfaceResolution.cpp`
has extensive comments for some parts of the code.

Interface Declaration
=====================

Basic syntax
------------

```chpl
interface IFC(FORMALS...) {        // 1 or more formals
  proc REQFN1(FORMALS...);         // no default implementation
  proc REQFN2(FORMALS...) {...}    // with implementation
  type AT;
  AT implements IFC;               // "associated constraint"
}
```

Each reqFn in the above behaves almost like an ICfun.  \
It can have interface constraints, too.

Implementation
--------------

**class `InterfaceSymbol`**

Fields computed upon resolution:

* `associatedTypes` maps the name of each AT to its Type*

* `associatedConstrains` stores all associated constraints

* `requiredFns` for each reqFn, maps its FnSymbol* to:
  - itself, if it has a default implementation
  - gDummyWitness, otherwise


Implements Statement
====================

Basic syntax
------------

```chpl
implements IFC(TYPE...);  // 1 type per ifc formal
```

Each TYPE can be a traditional generic, see #17618.

Implementation
--------------

**class `ImplementsStmt`**

Fields computed upon resolution:

* `witnesses.symWits`: a map
  - for each reqFn: its FnSymbol* --> FnSymbol* for its implementation
  - for each AT: its TypeSymbol* --> TypeSymbol* for the instantiating type

* `witnesses.conWits`: for each associated constraint,
  contains its witness istm (these istms are stored in the order that the
  associated constraints occur in istm->ifcSymbol()->associatedConstraints)

Wrapper Functions
-----------------

See:
`FLAG_IMPLEMENTS_WRAPPER` \
`implementsWrapperName` , `wrapOneImplementsStatement` , \
`wrapperFnForImplementsStmt` , `implementsStmtForWrapperFn`

Each `ImplementsStmt` is kept in the AST within a dedicated `FnSymbol`
known as a "wrapper function for ImplementsStmt". `FnSymbol::name`
is "|" followed by the interface name.

This allows us to gather visible implements statements using
`findVisibleFunctionsAllPOIs()` rather than a custom traversal.

// Todo: figure out and implement proper interaction with POIs;
issue an error when there are duplicates in the same scope.


Interface-Constrained Function
==============================

Basic Syntax
------------

```chpl
proc ..... where IFC_CONSTRAINT...  // where-clause constains 1 or more
{ .... }                            // interface constraints &&-ed together

IFC_CONSTRAINT ::= implements IFC_NAME(TYPE...)
```

Implementation
--------------

**class `InterfaceInfo`** -- stored in `FnSymbol::interfaceInfo`

`fn->isConstrainedGeneric()` is true when fn->interfaceInfo!=nullptr.
Such an FnSymbol remains "generic" after it is resolved "early".
This way it can be instantiated for a given call.

Fields of `InterfaceInfo` computed upon resolution:

* `constrainedTypes` - DefExprs for the freshly-introduced ConstrainedTypes

* `interfaceConstraints` - the IfcConstraints pulled from where-clauses

* `ifcReps` - one element per IfcConstraint, containing:

  - `symReps` - maps each symbol defined in ifc body (i.e., a reqfn or an AT)
    to its "representative" throughout the body of the ICfun

  - `conReps` - for representatives of required functions
    from the interfaces referenced in the associated constraints;
    more details in https://github.com/chapel-lang/chapel/pull/17678

* `invokedCGfns` - a set of "interim instantiations" of other ICfuns
  invoked from this ICfun

Example 1: constrainedTypes, interfaceConstraints
-------------------------------------------------

```chpl
proc icFun(arg: ?Q) where Q implements IFC {...}
```

`Q` is represented with a fresh `ConstrainedType` \
its DefExpr is added to fn->interfaceInfo->constrainedTypes

`Q implements IFC` is represented with an `IfcConstraint` \
it is moved from fn->where to fn->interfaceInfo->interfaceConstraints

Example 2: ifcReps.symReps
--------------------------

```chpl
interface IFC(Self) {
  proc reqFn(arg: Self);   // call this FnSymbol "fn(IFC.reqFn)"
}

proc icFun(arg: ?Q) where Q implements IFC
{
  reqFn(arg);              // resolves to fn(icFun.reqFn)

  // This is a compiler-introduced "representative" for IFC.reqFn.
  // It is inTree() only while early-resolving this icFun().
  proc reqFn(arg: Q);      // call this FnSymbol "fn(icFun.reqFn)"
}
```

`ifcReps[0].symReps` maps fn(IFC.reqFn) --> fn(icFun.reqFn) \
which is used to instantiate ICfuns without re-resolving them. See below.

Example 3: invokedCGfns and interim instantiations
-------------------------------------------------

```chpl
proc icFun1(arg: ?Q) where Q implements IFC {
  icFun2(arg);  // resolves to the interim instantiation of icFun2
}

proc icFun2(arg: ?Q) where Q implements IFC {
  ......
}
```

icFun1's `invokedCGfns` contains the interim instantiation of icFun2,
which the call icFun2(arg) resolves to.

Interim instantiations are never fully instantiated with finalizeCopy().
See `FLAG_CG_INTERIM_INST`.


RESOLVING A CALL TO IC FUNCTION
===============================

```chpl
interface IFC(Self) {
  proc reqFn(arg: Self);   // call it "fn(IFC.reqFn)"
}

proc icFun(arg: ?Q)        // call it "fn(icFun)"
  where Q implements IFC
{
  reqFn(arg);  // early-resolved to the representative fn(icFun.reqFn)
}

proc reqFn(arg: int) { }   // call it "fn(reqFn.int)"

int implements IFC;        // call it "istm(int.IFC)"

icFun(1);
```

#### Prior to resolving the call `icFun(1)`:

fn(icFun)->ifcReps[0].symReps maps `fn(IFC.reqFn)` --> `fn(icFun.reqFn)` \
istm(int.IFC)->witnesses.symWits maps `fn(IFC.reqFn)` --> `fn(reqFn.int)`

#### When resolving the call `icFun(1)`:

* candidate: `fn(icFun)`
* it is applicable; its constraint is satisfied by the witness: `istm(int.IFC)`
* the two maps above compose into the map `fn(icFun.reqFn)` --> `fn(reqFn.int)`
* icFun is instantiated into `icFun(int)`
  using this composed map
* upon instantiation, the baseExpr for the call reqFn(arg)
  is redirected from `fn(icFun.reqFn)` to `fn(reqFn.int)`
* so, `icFun(int)` will invoke `fn(reqFn.int)`
  even though it does not undergo "resolution"


RESOLUTION SCENARIOS TO HANDLE
==============================

Resolve an ifc declaration
--------------------------

For now this amounts mostly to resolving the types for formal arguments and
the bodies of default implementations (when provided).


Resolve a user call to a non-mixed ICfun
-----------------------------------------

* **non-mixed** ICfun means it has no TG formals
* this case is illustrated above
* all actuals are concrete

Steps:
* compute the map that instantiates ICfun's generics;
  this map is ResolutionCandidate::`substitutions`
* check the instantiated interface constraints (see below)
* extend `substitutions` to map ICfun's representatives
  to the witnesses of the required functions and associated types
  that are provided by the witness istms for these constraints
* instantiate ICfun using `substitutions`;
  see also `adjustForCGinstantiation()`


Resolve a user call to a mixed ICfun
------------------------------------

* **mixed** ICfun means it has TG formals and perhaps constraints
  in addition to IC formals and ifc constraints
* all actuals are concrete

Outline: done in two phases. \
The first phase instantiates TG generics, producing a non-mixed ICfun. \
The second phase handles the non-mixed ICfun as before.

Steps:
* instantiate ICfun's TG generics, obtaining ICfun2;
  NB need to carry over fn->`interfaceInfo`
* check ICfun2's where-clause, if present
* ICfun2 is now a non-mixed IC function
* early-resolve ICfun2 as a non-mixed ICfun (see below)
* proceed as in the non-mixed case above


Check a concrete istm
---------------------

* **concrete** istm means all implementing types are concrete
* the outcomes are: istm->`witnesses.symWits`, istm->`witnesses.conWits`

Illustration:
```chpl
// given this interface:
interface IFC(Self) {
  type AT;
  AT implements IFC2;
  proc reqFn(arg: Self);
  proc icReqFn(arg: Self, icArg: ?Q) where Q implements IFC3;
}

// the following steps are performed
// to check this statement:
int implements IFC;

// * find the witness for the associated type 'AT'
//   (then store it in istm->witnesses.symWits)
//   by resolving this:
var temp: int;
type ATinst = temp.AT;

// * check the associated constraint
//   (then add its witness istm to istm->witnesses.conWits)
//   after instantiating it with whatever type we obtained above:
ATinst implements IFC2;

// * find the witness for the non-IC required function 'reqFn'
//   (then store it in istm->witnesses.symWits)
//   by resolving this:
var arg: int;
reqFn(arg);

// * find the witness for the IC required function 'icReqFn'
//   (the witness should be an ICfun; store it in istm->witnesses.symWits)
//   by resolving this:
var arg: int;
var icArg: Q;
Q implements IFC3
icReqFn(arg, icArg);
// in the above call, the Type* of icArg is from `constrainedTypes`
// of the FnSymbol that is defined by the InterfaceSymbol for "IFC"
```

TODO: handle the case where the implementation of `icReqFn` makes calls
to required functions of `IFC3`.


Check a TG istm
---------------

* **TG** istm means it contains TG types
* no early checking is performed for such istm
* it "kicks in" when it matches a concrete constraint being checked
* if so: instantiate the TG istm \
  then check the instantiation as a concrete istm (see above)


Early-resolve a non-mixed ICfun
-------------------------------

* **non-mixed** ICfun means it has no TG formals
* **early** resolution means to resolve it prior to making any instantiations
* difference from TG resolution: ICfun body can contain calls
  to required functions from relevant interfaces

Steps:
* create representatives for reqFns for the interfaces referenced in ICfun's
  interface constraints, adding their DefExprs to fn->body
* TODO: ditto reps for the interfaces mentioned in those interfaces'
  associated constraints
* resolve the body using existing code
* support for resolving calls to reqFns is discussed below

TODO: creating representatives may increase compilation time when the number
of reqFns in the interface is high whereas only a small part of them is
usually invoked. Which we expect to be the case with the array and/or DSI
interfaces we are planning to introduce. An alternative approach will be
needed for production use. For example, instead of having a call with
`baseExpr` pointing to a representative, having it to be a primitive
that points to the position of the required function in the interface
(1st, 2nd, ...). This change will require additional support in the code
to resolve calls (see resolveNormalCall()).


Resolve a mixed ICfun
---------------------

* **mixed** ICfun means it has both TG and IC formals \
  it must have interface constraint(s); it may have more in its where-clause
* TODO: this case is to be implemented

The easiest approach would be to resolve it as TG: instantiate it fully,
verify that the interface constraints are met, then resolve it as a concrete
function. However, this may choose incorrect targets for those calls that
should invoke required functions. Instead...

The proposed approach:

* resolve it only for a given instantiation
* see the case of resolving a call to a mixed ICfun above


Early-resolve a call to a reqFn within an ICfun
-----------------------------------------------

These calls are a key difference between resolving an IC vs. concrete
function.

#### Example 1: reqFn comes from the interface referenced in ICfun's constraint

```chpl
interface IFC(Self) {
  proc reqFn(arg: Self);
}

proc icFun(arg: ?Q) where Q implements IFC
{
  reqFn(arg);   // this is a call to IFC's required function
}
```

Steps before resolving icFun's body:
* compute the mapping: Self --> Q
  by matching icFun's constraint `Q implements IFC` against IFC's formal
* instantiate IFC's reqFn into the **representative** `proc reqFn(arg: Q);`
* insert this representative's DefExpr into icFun's body

Steps for resolving the call `reqFn(arg)`:
* the instantiation of reqFn is visible and applicable using the existing
  visibility machinery (see findVisibleFunctionsAndCandidates())
* if there are other definitions of reqFn(), they will not match
  the call because the type `Q` is unique
* therefore the instantiation of reqFn is chosen as the call's target

Steps after resolving icFun's body:
* remove the representatives' DefExprs, otherwise they will be cloned
  into icFun's concrete instantiations

See also: `ifcReps` and its example above.

#### Example 2: reqFn comes from an associated constraint

This uses the same strategy of creating representatives. However it is a
temporary solution because of the potentially exponential growth of the number
of representatives as we follow from the AT of the current ifc to the next ifc.

The following example illustrates that reqFns can come from an associated
constraint or from an associated constraint's associated constraint, and
so on.

```chpl
interface I1 {
  proc reqFn1(arg: Self);
  proc get1(arg: Self): AT1;
  type AT1;
  AT1 implements I2;
}

interface I2 {
  proc reqFn2(arg: Self);
  proc get2(arg: Self): AT2;
  type AT2;
  AT2 implements I3;
}

interface I3 {
  proc reqFn3(arg: Self);
  proc get3(arg: Self): AT3;
  type AT3;
  // and so on
}

proc icFun(x1: ?Q1) where Q1 implements I1 {
  reqFn1(x1);  // this is the base case, see the previous example

  const x2 = get1(x1);  // x2: x1.AT1
  reqFn2(x2);   // legal because AT1 implements I2

  const x3 = get2(x2);   // x3: x1.AT1.AT2
  reqFn3(x3);   // legal because AT2 implements I3

  const x4 = get3(x3);   // x4: x1.AT1.AT2.AT3
  // and so on
}
```


Early-resolve a call to another ICfun from within an ICfun
----------------------------------------------------------

See the above example for `invokedCGfns`.


Infer a constraint
------------------

Currently inference is not relied on much. \
See the comment for `constraintIsSatisfiedAtCallSite()` in lieu of
documentation.

