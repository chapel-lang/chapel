#
# Copyright 2024-2025 Hewlett Packard Enterprise Development LP
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

"""
=======
Visitor
=======

This module contains utilities for defining and using visitors for :class:`chapel.AstNode` hierarchies.
"""

# use core to prevent circular dependency
import chapel.core

from collections import defaultdict
from inspect import signature


def _try_call(dispatch_table, visitor, node, method_type):
    """
    Not every AST node in the dispatch table has an entry, unless the user
    literally defined a method for every node type. We want to approximate
    the behavior of inheritance when calling a method on the visitor:
    if there's no `enter` method for a function declaration, but there
    is an `enter` method for a named declaration (of which a function is a subclass),
    then we want to call the parent `enter`.

    To this end, search upwards from the current node's type until we hit
    'object', at which point we have traversed the entire inheritance hierarchy.
    For each class, starting from the node's class itself, check the
    table for a method called `method_type`. If one exists, call it and
    return its result, whatever it is, wrapped in a list.

    Example traversal:
      Function -> NamedDecl -> Decl -> AstNode -> object

    Results are wrapped in a list to distinguish between _try_call not
    finding a method to call at all (and thus returning `None`) from
    the method that was called itself returning `None` (in which case,
    the result will be `[None]`).
    """

    node_type = type(node)
    while node_type is not object:
        if node_type in dispatch_table:
            methods = dispatch_table[node_type]
            if method_type in methods:
                return [methods[method_type](visitor, node)]
        node_type = node_type.__base__
    return None


def _visitor_visit(dispatch_table):
    def _do_visit(self, node):
        if isinstance(node, list):
            for child in node:
                _do_visit(self, child)
            return

        if _try_call(dispatch_table, self, node, "enter") != [False]:
            for child in node:
                _do_visit(self, child)
        _try_call(dispatch_table, self, node, "exit")

    return _do_visit


def enter(method):
    """
    Annotates a class method as being an 'enter' function. An 'enter'
    function is called on a node before its children are visited. If
    it returns a boolean, this boolean is used to determine whether or not
    the children are visited. Returning 'False' indicates that children
    should be skipped; returning 'True' indicates that children should be
    visited.

    The 'exit' function is always called, even if the children were not
    visited.
    """
    method.__chapel_visitor_method__ = "enter"
    return method


def exit(method):
    """
    Annotates a class method as being an 'exit' function. An 'exit'
    function is called after a node's children were (or would have been)
    visited.
    """
    method.__chapel_visitor_method__ = "exit"
    return method


def visitor(clazz):
    """
    Marks a class as being a visitor. This will add a 'visit' method to
    the class (overriding one if it exists), which can be used to recursively
    traverse an AST. The behavior of the 'visit' method is controlled via
    other methods defined on the class, particularly those annotated with
    @enter and @exit.

    The traversal proceeds by means of 'enter' and 'exit' functions; see
    the documentation for those functions for more information. Enter
    and exit functions are both expected to take a single argument in addition
    to 'self': the node type being visited. A type annotation is required,
    and is used to determine what type of node the function should operate on.
    """

    dispatch_table = defaultdict(lambda: {})

    # Detect all visitor-like methods.
    for name, var in vars(clazz).items():
        if not callable(var):
            continue
        if not hasattr(var, "__chapel_visitor_method__"):
            continue

        sig = signature(var)
        if "self" not in sig.parameters:
            continue
        if len(sig.parameters) != 2:
            continue

        # Detect the type of the second argument. It's an OrderedDict
        # so we can use numbers.
        arg = list(sig.parameters.values())[1]

        # Check if the type is a subtype of AstNode
        if not issubclass(arg.annotation, chapel.core.AstNode):
            continue

        dispatch_table[arg.annotation][var.__chapel_visitor_method__] = var

    clazz.visit = _visitor_visit(dispatch_table)
    return clazz
