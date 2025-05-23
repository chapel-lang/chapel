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

  .. warning::

    This module is unstable. Support for dynamic loading is experimental
    and its representation could change at any time.

  This module provides the ability to load a binary at runtime. Procedures
  contained in a dynamically loaded binary can be retrieved and called on
  any locale without compile-time knowledge of their names or locations.

  A hypothetical C binary could contain a procedure named ``foo``:

  .. code-block:: c

    // Compile with: 'cc -o TestBinary TestBinary.c'
    //

    #include <stdio.h>

    void foo(void);
    void foo(void) {
      printf("Hello world from %s!\n", __FUNCTION__);
    }

    int main(void) {
      foo();
      return 0;
    }

  This binary can be can be loaded in Chapel at runtime as follows:

  .. code-block:: chapel

    use DynamicLoading;

    // A binary may or may not exist at this path.
    const path = './TestBinary';

    // If loading fails an error will be issued and the 'try!' will halt.
    const bin = try! binary.load(path);

  And a procedure named ``foo`` with type ``proc(): void`` can be retrieved:

  .. code-block::chapel

    // Declare the type of the procedure.
    type P = proc(): void;

    // Retrieve a procedure named 'foo' from 'bin' with the type 'P'.
    const p1 = try! bin.retrieve('foo', P);

    // Call the procedure.
    p1();

  When a procedure is retrieved from a loaded binary, the returned procedure
  value is callable on any locale despite :proc:`binary.retrieve()` only
  being called on a single locale. The returned procedure is considered to
  be ``extern`` and this is reflected in its type.

  .. note::

    Currently, only procedures can be retrieved from loaded binaries.
    Support for retrieving references to data stored in a binary could
    be added in the future.
*/
@unstable()
module DynamicLoading {

// This internal module contains the low-level implementation.
private use ChapelDynamicLoading;

if !fcfsUsePointerImplementation {
  compilerError('This module cannot be used unless the experimental ' +
                'procedure pointer implementation is activated. Set the ' +
                'config param \'fcfsUsePointerImplementation\' to ' +
                '\'true\' when compiling to activate it.');
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
    binary, though a "shared library" or "dynamic library" is most common.

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
  @unstable('The \'load()\' procedure is experimental')
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

      The procedure type ``t`` provided when calling ``binary.retrieve()`` is
      used verbatim and is not checked against the type of the underlying
      symbol in any way. If the type provided does not match the actual type
      of the underlying procedure, then the resulting behavior when the
      retrieved procedure is called is undefined. Crashes or other bugs
      could occur.

    The retrieved procedure is considered to be ``extern`` and will use an
    extern calling convention, the same as is used for other ``extern``
    procedures declared in Chapel code.

    :arg name: The name of the procedure to retrieve
    :type name: `string`

    :arg t: The type of the retrieved procedure
    :throws DynLoadError: if procedure lookup fails
  */
  @unstable('The \'retrieve()\' method is experimental')
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
    const ptr: c_ptr(t) = nil;
    return ptr.deref();
  }
}

}
