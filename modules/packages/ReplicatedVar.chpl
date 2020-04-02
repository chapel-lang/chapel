/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

/*
Support for user-level replicated variables.

A "replicated" variable is a variable for which there is a copy on each locale.
Referencing a replicated variable
(in a stylized way, see :ref:`below <ReplicatedVar_basic-usage>`)
accesses its copy on the current locale.

Features:

* The variable's copies are not synchronized automatically among the locales.

Limitations:

* It is "user-level", i.e. the user is required to handle the variable
  in specific ways to achieve the desired result.

* Tree-shape communication (like for reductions) is not provided.

* Using a replicated variable of an array type is not straightforward.
  Workaround: declare that array itself as replicated, then access it normally,
  e.g.:

.. code-block:: chapel

   var replArray: [MyDomain dmapped Replicated()] real;

.. _ReplicatedVar_basic-usage:

-------------------------------
How to use replicated variables
-------------------------------

.. code-block:: chapel

    use ReplicatedVar;

    // declare a replicated variable of the type 'MyType'
    var myRepVar: [rcDomain] MyType;

    // access its copy on the current locale (read or write) (either option)
    ... myRepVar(1) ...  // must use 1
    ... rcLocal(myRepVar) ...

    // "replicate": assign 'valToRep' to copies on all locales
    rcReplicate(myRepVar, valToRep);

    // "collect": assign from each copy of 'myRepVar' to
    // corresponding element of an array 'collected'
    var collected: [LocaleSpace] MyType;
    rcCollect(myRepVar, collected);

    // access directly a remote copy on the locale 'remoteLoc' (read or write)
    ... rcRemote(myRepVar, remoteLoc) ...

.. _ReplicatedVar_subset-of-locales:

------------------------------------
Replicating over a subset of locales
------------------------------------

To replicate a variable over a subset of locales, say ``myLocales``,
modify the above variable declarations as follows:

.. code-block:: chapel

    var myRepVar: [rcDomainBase dmapped Replicated(myLocales,
                     "over which to replicate 'myRepVar'")] MyType;
    var collected: [myLocales.domain] MyType;


Tip: if the domain of the desired array of locales cannot be described
as a rectangular domain (which could be strided, multi-dimensional,
and/or sparse), make that array's domain associative over int.

----------------------------------------------
Declarations
----------------------------------------------
*/
module ReplicatedVar {

public use ReplicatedDist;

private const rcDomainIx   = 1; // todo convert to param
/* Use this domain when replicating over a subset of locales,
   as shown :ref:`above <ReplicatedVar_subset-of-locales>`. */
const rcDomainBase = {rcDomainIx..rcDomainIx};
private const rcLocales    = Locales;
private const rcDomainMap  = new unmanaged Replicated(rcLocales);
/* Use this domain to declare a user-level replicated variable,
   as shown :ref:`above <ReplicatedVar_basic-usage>` . */
const rcDomain     = rcDomainBase dmapped new dmap(rcDomainMap);
private param _rcErr1 = " must be 'rcDomain' or 'rcDomainBase dmapped Replicated(an array of locales)'";

private proc _rcTargetLocalesHelper(replicatedVar: [?D])
  where isSubtype(_to_borrowed(replicatedVar._value.type), ReplicatedArr)
{
  return replicatedVar.targetLocales();
}

pragma "no doc" // documented with the following entry
proc rcReplicate(replicatedVar: [?D] ?MYTYPE, valToReplicate: MYTYPE): void
{ compilerError("the domain of first argument to rcReplicate()", _rcErr1); }

/* Assign a value `valToReplicate` to copies of the replicated variable
   `replicatedVar` on all locales. */
proc rcReplicate(replicatedVar: [?D] ?MYTYPE, valToReplicate: MYTYPE): void
  where isSubtype(_to_borrowed(replicatedVar._value.type), ReplicatedArr)
{
  assert(replicatedVar.domain == rcDomainBase);
  coforall loc in _rcTargetLocalesHelper(replicatedVar) do
    on loc do
      replicatedVar[rcDomainIx] = valToReplicate;
}

pragma "no doc" // documented with the following entry
proc rcCollect(replicatedVar: [?D] ?MYTYPE, collected: [?CD] MYTYPE): void
  where ! isSubtype(_to_borrowed(replicatedVar._value.type), ReplicatedArr)
{ compilerError("the domain of first argument to rcCollect()", _rcErr1); }

/* Copy the value of the replicated variable `replicatedVar` on each locale
   into the element of the array `collected` that corresponds to that locale.*/
proc rcCollect(replicatedVar: [?D] ?MYTYPE, collected: [?CD] MYTYPE): void
  where isSubtype(_to_borrowed(replicatedVar._value.type), ReplicatedArr)
{
  var targetLocales = _rcTargetLocalesHelper(replicatedVar);
  assert(replicatedVar.domain == rcDomainBase);
  for idx in collected.domain do assert(targetLocales.domain.contains(idx));
  coforall (idx, col) in zip(targetLocales.domain.sorted(), collected) do
    on targetLocales[idx] do
      col = replicatedVar[rcDomainIx];
}

/*
Access the copy of `replicatedVar` on the current locale.

This is equivalent to ``replicatedVar[1]``.
*/
proc rcLocal(replicatedVar: [?D] ?MYTYPE) ref: MYTYPE {
  return replicatedVar[rcDomainIx];
}

/*
Access the copy of `replicatedVar` on the locale `remoteLoc`.
*/
proc rcRemote(replicatedVar: [?D] ?MYTYPE, remoteLoc: locale) ref: MYTYPE {
  var result: MYTYPE;
  on remoteLoc do
    result = replicatedVar[rcDomainIx];
  return result;
}


/////////////////////////////////////////////////////////////////////////////
// code examples

// This example illustrates the use of a replicated variable as follows:
// - create a replicated variable of the type MyType,
// - initializes all its copies to 'initVal',
// - stores 'newVal' into its copy on the locale 'newLocale',
// - collects all its copies into an array 'collected'.
//
pragma "no doc" // TODO: move to a primer
proc rcExample(initVal: ?MyType, newVal: MyType, newLocale: locale): void {
  writeln("starting rcExample");

  // declare a replicated variable
  var myRepVar: [rcDomain] MyType;

  // initialize all copies to 'initVal'
  rcReplicate(myRepVar, initVal);

  writeln("\nafter initialization, myRepVar copies are:");
  writeReplicands(myRepVar, Locales); // a helper function to print each locale's copy

  // go to 'newLocale' and update its copy to 'newVal'
  on newLocale {
    // myRepVar[1] references myRepVar's copy on the current locale
    writeln("\non ", newLocale, ", myRepVar is:\n",
            "  before assignment: ", myRepVar[1]);

    // update the current locale's copy
    myRepVar[1] = newVal;

    writeln("  after assignment:  ", myRepVar[1]);
  }

  writeln("\nafter update, myRepVar copies are:");
  writeReplicands(myRepVar, Locales);

  // collect all copies of 'myRepVar' into an array
  var collected: [LocaleSpace] MyType;
  rcCollect(myRepVar, collected);

  writeln("\ncollected copies of myRepVar are:\n", collected);
}

// This is a helper function to print each locale's replicand, labeled
//
private proc writeReplicands(RV, locs) {
  for loc in locs {
    on loc {
      writeln(loc, ":");
      writeln(RV);
    }
  }
}

// This is the same as 'rcExample', except the user can provide
// specific locales to replicate over. The two differences are marked.
//
pragma "no doc" // TODO: move to a primer
proc rcExampleOverLocales(initVal: ?MyType, newVal: MyType, newLocale: locale,
                          localesToReplicateOver: [] locale = Locales): void {
  writeln("starting rcExampleOverLocales over ", localesToReplicateOver);

  // declare a replicated variable
  // DIFFERENT from rcExample(): the domain in myRepVar's type
  var myRepVar: [rcDomainBase dmapped Replicated(localesToReplicateOver,
   "over which to replicate 'myRepVar' in rcExampleOverLocales()")] MyType;

  // initialize all copies to 'initVal'
  rcReplicate(myRepVar, initVal);

  writeln("\nafter initialization, myRepVar copies are:");
  writeReplicands(myRepVar, localesToReplicateOver);

  // go to 'newLocale' and update its copy to 'newVal'
  on newLocale {
    // myRepVar[1] references myRepVar's copy on the current locale
    writeln("\non ", newLocale, ", myRepVar is:\n",
            "  before assignment: ", myRepVar[1]);

    // update the current locale's copy
    myRepVar[1] = newVal;

    writeln("  after assignment:  ", myRepVar[1]);
  }

  writeln("\nafter update, myRepVar copies are:");
  writeReplicands(myRepVar, localesToReplicateOver);

  // collect all copies of 'myRepVar' into an array
  // DIFFERENT from rcExample(): the domain in collected's type
  var collected: [localesToReplicateOver.domain] MyType;
  rcCollect(myRepVar, collected);

  writeln("\ncollected copies of myRepVar are:\n", collected);
}

} // module ReplicatedVar
