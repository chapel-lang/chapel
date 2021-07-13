This document outlines a new approach to implementing interfaces.
See `Interfaces.md` for background.

Compilation Scenarios
=====================

Here are the various aspects of compilation for us to support.

Basic early resolution of an IC function
----------------------------------------

When early-resolving an IC function, we need to ensure that the following
additional functions are visible in its body, ideally also throughout
the IC function's signature and where clause:

* required functions for the interfaces referenced in the ifc constraints
* required functions analogously available through the associated constraints
  of these interfaces
* methods on the interface types that return associated types
  (see an example below), to allow users to reference these associated types

In the above, the required functions, associated types, and associated
constraints need to be properly instantiated, for example:

```chpl
interface IFC(T1, T2) {
  proc reqFn(arg1: T1, arg2: T2);
}

proc ICfun(arg: ?Q) where implements IFC(Q, int) {
  // here, this function is considered available:
  //   proc reqFn(arg1: Q, arg2: int);
  ... user code ...
}
```

Here is an example of methods on interface types:

```chpl
interface Ifc1(T1) {
  type AT1;
  AT1 implements Ifc2;
}

interface Ifc2(T2) {
  type AT2;
}

proc ICfun(arg: ?Q) where Q implements Ifc1 {
  // here, we represent the associated types AT1 and AT2 with unique
  // types, call them Type1 and Type2;
  // also, the following methods are considered available:
  //   proc      Q.AT1     type return Type1;
  //   proc type Q.AT1     type return Type1;
  //   proc      Type1.AT2 type return Type2;
  //   proc type Type1.AT2 type return Type2;
  ... user code ...
}
```

In the above examples, the implementation of `reqFn` and the instantiations of
`Type1` and `Type2` are to be provided by istm witness(es) that satisfy
ICfuns' implements constraints when these ICfuns are invoked.

Basic function call
-------------------

Scenario: resolving a function call where the target is an IC function.
This is similar to the case where the target is a TG function.

* Gather resolution candidates.
  Candidates can come from ifc constraints in enclosing scopes.
  For example, if the call is in a function with a constraint
  `T implements MyInterface`, the set of candidates includes
  relevant required functions in MyInterface.

* Filter applicable candidates. For an IC candidate:
  - early-resolve the candidate if it has not been already
  - instantiate the candidate's query types;
    this entail computing a substitution for each query type
    in the same way we do for a TG candidate
  - using these substitutions, instantiate the candidate's ifc constraints
  - check that these instantiated ifc constraints are satisfied;
    this entails finding a witness istm for each ifc constraint
  - these witnesses provide instantiations aka substitutions for ATs
  - instantiate the candidate's signature -- essentially the formals' types --
    using the substitutions for the query types and ATs; analogously
    to the case of a TG candidate, this means creating a new FnSymbol,
    call it IIC (Instantiated IC function), whose signature is concrete
  - check applicability of the instantiated signature
    using the rules for a concrete candidate;
    this is the same step we use for a TG candidate

* Chose the best candidate.
  We may want to prefer candidates from ifc constraints to other candidates.

* Instantiate the body of IIC using the same substitutions that we used
  to instantiate its signature.

* Resolve the body of this new FnSymbol.
  Since we are considering the case where the candidate is an IC function,
  its body has already been resolved. However, IIC's body needs to be
  post-processed additionally:
  - interface types and associated types need to be substituted by their
    implementations, as defined by witness istms
    (this is probably done while instantiating the candidate into IIC)
  - calls to required functions need to be directed to their implementations,
    as defined by witness istms
  - remaining compile-time conditionals and param loops need to be un/folded
    (this is not implemented in our initial implementation of interfaces)

Overview of all scenarios
-------------------------

The non-trivial cases where we need to cater to IC generics fall in these
categories:

* resolving a function call while resolving a function that is:
  - concrete
  - IC function
  - the default implementation of a required function in an interface

* early-resolving a function call within an IC function (from the first bullet)
  whose target is:
  - an IC function
  - a required function in an interface
  - a required function in the interface of an associated constraint

* ... while passing a value of:
  - an interface type
  - an associated type
  - an associated type of the interface of an associated constraint

* ... while the ifc constraints that need to be satisfied are satisfied by
  one or a combination of:
  - the IC function's ifc constraints
  - an associated constraint of the interface of such an ifc constraint
  - an associated constraint of the interface of such an associated constraint

* resolving a function call to determine the implementation of a required
  function while resolving an istm, where:
  - the required function is not an IC function
  - the required function IS an IC function

```chpl
interface IFC(Self) {
  proc reqFn1(arg: Self);
  proc reqFn2(arg1: Self, arg2: ?Q) where Q implements IFC2;
}

int implements IFC;

// means we need to resolve these calls:
reqFn1(an actual of the type int);
// for reqFn2, conceptually wrap the call in an IC function:
proc helper(temp: ?Q) where Q implements IFC2 {
  reqFn2(an actual of the type int, an actual of the type Q);
}
```

* note: early-resolving a default implementation (from the first bullet)
  is equivalent to early-resolving an IC function as follows:

```chpl
interface IFC(Self) {
  type AT;
  proc reqFn(arg1: Self, arg2: AT) { body }
}

// early-resolving reqFn as the default implementation
// is equivalent to early-resolving reqFn as the following IC function

proc reqFn(arg1: ?Self, arg2: Self.AT)
  where Self implements IFC
{ body }
```

New Implementation Strategy
===========================

The new implementation strategy is based on viewing an IC function
as accepting one implicit formal argument per its ifc constraint.

For example:
```chpl
proc ICfun(arg: ?Q) where Q implements IFC {
  ICfun2(arg);
}

proc ICfun2(arg: ?R) where R implements IFC {
}

// these are viewed as:

proc ICfun(arg: ?Q, con1: InterfaceConstraint(IFC, Q)) {
  ICfun2(arg, con1);
}

proc ICfun2(arg: ?R, con1: InterfaceConstraint(IFC, R)) {
}
```

More generally, we can talk about viewing ifc constraints as variables,
with the following characteristics:

* Let's call them "Constraint Symbols", or ConSyms.

* ConSyms are visible according to the usual lexical scoping rules.

* A ConSym that is visible participates in resolution: it makes
  the corresponding required functions, associated types, and
  associated constraints available.

* Consider each ConSym to be of an `InterfaceConstraint` type.

* `InterfaceConstraint` acts as a generic type that is instantiated
  with the interface symbol (in its first position) and the type
  arguments of the ifc constraint. For example, the ifc constraint:
  `implements IFC(T1, T2)`
  corresponds to a ConSym of the type:
  `InterfaceConstraint(IFC, T1, T2)`

* An `InterfaceConstraint` type will be always instantiated with an interface
  symbol. This type will still be generic - and amenable to instantiation -
  when it has generic type argument(s).

* Consider an implements statement to be a value of an InterfaceConstraint type.
