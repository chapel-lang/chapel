#
# Copyright 2020-2023 Hewlett Packard Enterprise Development LP
# Copyright 2004-2019 Cray Inc.
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

import chapel
from chapel.core import *
import re

def name_for_linting(node):
    name = node.name()
    if name.startswith("chpl_"):
        name = name.removeprefix("chpl_")

    # Strip dollar signs.
    name = name.replace("$", "")

    return name

def check_camel_case(node):
    return re.fullmatch(r'_?([a-z]+([A-Z][a-z]+|\d+)*|[A-Z]+)?', name_for_linting(node))

def check_pascal_case(node):
    return re.fullmatch(r'_?(([A-Z][a-z]+|\d+)+|[A-Z]+)?', name_for_linting(node))

def register_rules(driver):
    @driver.basic_rule(VarLikeDecl, default=False)
    def CamelCaseVariables(context, node):
        if node.name() == "_": return True
        return check_camel_case(node)

    @driver.basic_rule(Record)
    def CamelCaseRecords(context, node):
        return check_camel_case(node)

    @driver.basic_rule(Class)
    def PascalCaseClasses(context, node):
        return check_pascal_case(node)

    @driver.basic_rule(Module)
    def PascalCaseModules(context, node):
        return node.kind() == "implicit" or check_pascal_case(node)

    @driver.basic_rule(Module, default=False)
    def UseExplicitModules(context, node):
        return node.kind() != "implicit"

    @driver.basic_rule(Loop)
    def DoKeywordAndBlock(context, node):
        return node.block_style() != "unnecessary"

    @driver.basic_rule(Coforall)
    def NestedCoforalls(context, node):
        parent = node.parent()
        while parent is not None:
            if isinstance(parent, Coforall):
                return False
            parent = parent.parent()
        return True

    @driver.basic_rule([Conditional, BoolLiteral, chapel.rest])
    def BoolLitInCondStmt(context, node):
        return False

    @driver.basic_rule(NamedDecl)
    def ChplPrefixReserved(context, node):
        if node.name().startswith("chpl_"):
            path = node.location().path()
            return context.is_bundled_path(path)
        return True

    @driver.basic_rule(Record)
    @driver.basic_rule(Class)
    def MethodsAfterFields(context, node):
        method_seen = False
        for child in node:
            if isinstance(child, VarLikeDecl) and method_seen:
                return False
            if isinstance(child, Function):
                method_seen = True
        return True

    @driver.advanced_rule(default=False)
    def ConsecutiveDecls(context, root):
        def is_relevant_decl(node):
            if isinstance(node, MultiDecl):
                for child in node:
                    if isinstance(child, Variable): return child.kind()
            elif isinstance(node, Variable):
                return node.kind()
            return None

        def recurse(node, skip_direct = False):
            consecutive = []
            last_kind = None
            last_has_attribute = False

            for child in node:
                yield from recurse(child, skip_direct = isinstance(child, MultiDecl))

                if skip_direct: continue

                new_kind = is_relevant_decl(child)
                has_attribute = child.attribute_group() is not None
                any_has_attribute = last_has_attribute or has_attribute
                compatible_kinds = not any_has_attribute and (last_kind is None or last_kind == new_kind)
                last_kind = new_kind
                last_has_attribute = has_attribute

                # If we ran out of compatible decls, see if we can return them.
                if not compatible_kinds:
                    if len(consecutive) > 1:
                        yield consecutive[1]
                    consecutive = []

                # If this could be a compatible decl, start a new list.
                if new_kind is not None:
                    consecutive.append(child)

            if len(consecutive) > 1:
                yield consecutive[1]

        yield from recurse(root)

    @driver.advanced_rule
    def MisleadingIndentation(context, root):
        prev = None
        for child in root:
            if isinstance(child, Comment): continue
            yield from MisleadingIndentation(context, child)

            if prev is not None:
                if child.location().start()[1] == prev.location().start()[1]:
                    yield child

            prev = None
            if isinstance(child, Loop) and child.block_style() == "implicit":
                grandchildren = list(child)
                # safe to access [-1], loops must have at least 1 child
                for blockchild in reversed(list(grandchildren[-1])):
                    if isinstance(blockchild, Comment): continue
                    prev = blockchild
                    break

    @driver.advanced_rule(default=False)
    def UnusedFormal(context, root):
        formals = dict()
        uses = set()

        for (formal, _) in chapel.each_matching(root, Formal):
            # For now, it's harder to tell if we're ignoring 'this' formals
            # (what about method calls with implicit receiver?). So skip
            # 'this' formals.
            if formal.name() == "this":
                continue

            # extern functions have no bodies that can use their formals.
            if formal.parent().linkage() == "extern":
                continue

            formals[formal.unique_id()] = formal

        for (use, _) in chapel.each_matching(root, Identifier):
            if refersto := use.to_node():
                uses.add(refersto.unique_id())

        for unused in formals.keys() - uses:
            yield formals[unused]

    @driver.advanced_rule
    def UnusedLoopIndex(context, root):
        indices = dict()
        uses = set()

        def variables(node):
            if isinstance(node, Variable):
                yield node
            elif isinstance(node, TupleDecl):
                for child in node:
                    yield from variables(child)

        for (_, match) in chapel.each_matching(root, [IndexableLoop, ("?decl", Decl), chapel.rest]):
            node = match["decl"]

            for index in variables(node):
                indices[index.unique_id()] = index

        for (use, _) in chapel.each_matching(root, Identifier):
            if refersto := use.to_node():
                uses.add(refersto.unique_id())

        for unused in indices.keys() - uses:
            yield indices[unused]
