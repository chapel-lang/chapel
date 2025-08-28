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

// TODO: Can we make tests out of the code blocks? We'd need to be able to
// add a 'precomp' and 'compopts'.
//

/*
Support for dynamic loading in Chapel.

.. note::

  To ``use`` this module, the experimental procedure pointer feature
  must be activated. Do this by setting the ``config param`` named
  ``useProcedurePointers`` to ``true``.

This module provides the ability to load a binary at runtime. Procedures
contained in a dynamically loaded binary can be retrieved and called on
any locale without compile-time knowledge of their names or locations.

A hypothetical C binary could contain a procedure named ``foo``:

.. literalinclude:: ../../../../test/library/packages/DynamicLoading/doc-examples/TestBinary.c
   :language: c
   :start-after: START_EXAMPLE
   :end-before: STOP_EXAMPLE

This binary can be can be loaded in Chapel at runtime as follows:

.. literalinclude:: ../../../../test/library/packages/DynamicLoading/doc-examples/ModuleDocTest.chpl
   :language: chapel
   :start-after: START_EXAMPLE_0
   :end-before: STOP_EXAMPLE_0

And a procedure named ``foo`` with type ``proc(): void`` can be retrieved:

.. literalinclude:: ../../../../test/library/packages/DynamicLoading/doc-examples/ModuleDocTest.chpl
   :language: chapel
   :start-after: START_EXAMPLE_1
   :end-before: STOP_EXAMPLE_1

When a procedure is retrieved from a loaded binary, the returned procedure
value is callable on any locale despite :proc:`binary.retrieve()` only
being called on a single locale. The returned procedure is considered to
be ``extern`` and this is reflected in its type.

.. note::

  Currently, only procedures can be retrieved from loaded binaries.
  Support for retrieving references to data stored in a binary could
  be added in the future.
*/
@unstable('Dynamic loading support is experimental and unstable.')
module DynamicLoading {

// This internal module contains the low-level implementation.
private use ChapelDynamicLoading;

// Publish this error class for use by users.
public use ChapelDynamicLoading only DynLoadError;

if !useProcedurePointers {
  compilerError('This module cannot be used unless the experimental ' +
                'procedure pointer implementation is activated. Set the ' +
                'config param \'useProcedurePointers\' to ' +
                '\'true\' when compiling to activate it');
}

/*
  A wrapper around a dynamically loaded binary.
*/
record binary {
  @chpldoc.nodoc
  var _bin: unmanaged chpl_BinaryInfo;

  @chpldoc.nodoc
  proc init(bin: unmanaged chpl_BinaryInfo) {
    this._bin = bin;
  }

  /* The initialized binary refers to the same binary stored in ``rhs``. */
  proc init=(rhs: binary) {
    this._bin = rhs._bin;
  }

  @chpldoc.nodoc
  proc postinit() do _bin.bumpRefCount();

  proc deinit() {
    // TODO: Could mark symbols for GC or reclaim the binary on drop.
    _bin.dropRefCount();
  }

  /*
    After assignment, ``lhs`` will refer to the same binary stored in ``rhs``.
  */
  operator=(ref lhs: binary, rhs: binary) {
    rhs._bin.bumpRefCount();
    lhs._bin.dropRefCount();
    lhs._bin = rhs._bin;
  }

  /*
    Create a record representing a dynamically loaded binary using a string
    that stores the path to a binary file. The file may be any executable
    binary, though a "shared library" or "dynamic library" is by far the
    most common and well supported.

    The implementation will attempt to load a binary on all locales. If
    loading should fail on any locale then the entire process will be
    aborted and an error will be thrown.

    Dynamic loading can fail for a variety of reasons. It can fail if the
    binary could not be loaded on any locale. It can fail if no file was
    found at ``path`` or if a file was found but it did not have a suitable
    representation (e.g., the file format was not natively executable, or
    it was not marked as executable).

    :arg path: The path to the binary file to dynamically load
    :type path: `string`

    :throws DynLoadError: if dynamic loading fails
  */
  proc type load(path: string) throws {
    var err: owned DynLoadError?;
    const bin = chpl_BinaryInfo.create(path, err);
    if err then throw err;

    // Should hold by construction.
    assert(bin != nil);

    return new binary(bin!);
  }

  /*
    Fetch a procedure from a dynamically loaded binary. Throws an error if
    no procedure could be found.

    .. warning::

      The procedure type ``t`` provided when calling :proc:`binary.retrieve()`
      is used verbatim and is not checked against the type of the underlying
      symbol in any way. If the type provided does not match the actual type
      of the underlying procedure, then the resulting behavior when the
      retrieved procedure is called is undefined. Crashes or other bugs
      could occur.

    The retrieved procedure is considered to be ``extern`` and will use an
    extern calling convention, the same as is used for other ``extern``
    procedures declared in Chapel code. This impacts how it can be used.
    For example, it cannot currently be assigned to a procedure value that
    is not also considered ``extern``.

    :arg name: The name of the procedure to retrieve
    :type name: `string`

    :arg t: The type of the retrieved procedure
    :throws DynLoadError: if procedure lookup fails
  */
  proc retrieve(name: string, type t) throws where isProcedureType(t) {
    var err: owned DynLoadError?;
    var ret = if _bin then _bin!.loadSymbol(name, t, err) else nil;
    if err != nil then throw err;
    return ret;
  }

  @chpldoc.nodoc
  proc retrieve(name: string, type t) ref throws where !isProcedureType(t) {
    compilerError('The \'retrieve()\' method currently only works with ' +
                  'procedure types (tried to retrieve type \'' + t:string +
                  '\')');
    // TODO: We can return a (wide) reference to a value that is loaded up,
    // but it will only refer to the version of the value stored on a
    // single locale. The process will look like this except that it will
    // return a dereference of a non-nil pointer.
    //
    use CTypes;

    halt('Should never reach here!');

    // This produces an invalid ref, but that's OK. It will never compile.
    const ptr: c_ptr(t) = nil;
    return ptr.deref();
  }
}

}
