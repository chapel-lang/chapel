
//
// This test exists to guard against errors in the case that the documentation
// for a nested type (e.g. 'Inner') contains a code-block as the final
// element of the doc-comment. At one point in history, the documentation for
// 'Inner' was incorrectly indented such that it was aligned with the
// '.. record:: Inner' line in the output file. The result was that the
// 'innerMethod' indentation was then incorrectly aligned with the code-block,
// and appeared as literal rst in the rendered HTML.
//

/*
  Documentation for record R.

  .. code-block:: text

    a code block!
*/
record R {
  var x : int;

  /* Document 'foo' */
  proc foo() {
  }

  /*
    Documentation for ``Inner``

    .. code-block:: text

      This code-block should not overflow to 'innerMethod'!

  */
  record Inner {
    /*
       Inner Method.

       Multiline comment.
    */
    proc innerMethod() {
    }

    /*
      Documentation for ``InnerMost``

      .. code-block:: text

        This code-block should not overflow to 'innerMostMethod'!

    */
    record InnerMost {
      /*
         InnerMost Method.

         Multiline comment.
      */
      proc innerMostMethod() {
      }
    }
  }
}

/*
  Documentation for class C.

  .. code-block:: text

    a code block!
*/
class C {
  var x : int;

  /* Document 'foo' */
  proc foo() {
  }

  /*
    Documentation for ``Inner``

    .. code-block:: text

      This code-block should not overflow to 'innerMethod'!

  */
  class Inner {
    /*
       Inner Method.

       Multiline comment.
    */
    proc innerMethod() {
    }

    /*
      Documentation for ``InnerMost``

      .. code-block:: text

        This code-block should not overflow to 'innerMostMethod'!

    */
    class InnerMost {
      /*
         InnerMost Method.

         Multiline comment.
      */
      proc innerMostMethod() {
      }
    }
  }
}
