Internal errors
===============

This documentation seeks to describe what different internal error
messages mean.

``... already in tree``
  This error message is usually generated in a constructor.  If it is encountered, it means that an expression or statement you are inserting into the ``AST`` is already in there.  Expressions and statements can only appear in the tree once.  (Contrast that to symbols and types.)  You should probably copy the expression or statement to avoid problems.

``Illegal call to Stmt::copy``

``Illegal call to Expr::copy``

``Illegal call to Symbol::copy``

``Illegal call to Type::copy``
  These should not exist.  They are abstract base classes.

``Attempt to traverse the definition of a statement``

``Attempt to traverse the definition of an expression``
  Only symbols and types distinguish between traversing their definition.

``Bad ...::astType``
  Something major-league has gone wrong.  The dynamic type and the ``astType`` are not matching up.  These should always match up.

``Unexpected case in ...::replaceChild``
  Something has gone wrong with replace.  You are trying to replace something that is thought to be in ..., but it is not.

``Ill-formed list in BaseAST::remove``
  You are attempting to remove a node in a list that only has a previous or a next pointer, not both.

``Argument is in a list in BaseAST::...``
  You are attempting to ... with something that is already in a list.  Remove or copy it.

``Ill-formed list in BaseAST::...``
  You are attempting to ... a node in a list that only has a previous or a next pointer, not both.
