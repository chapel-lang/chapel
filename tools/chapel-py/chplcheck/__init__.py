#!/usr/bin/env python3

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
import chapel.core
import chapel.replace
import re
import sys
import argparse
from driver import *

# === "user-defined" linter rule functions, used to implement warnings. ===

def name_for_linting(node):
    name = node.name()
    if name.startswith("chpl_"):
        name = name.removeprefix("chpl_")
    return name

def check_camel_case(node):
    return re.fullmatch(r'_?([a-z]+([A-Z][a-z]+|\d+)*|[A-Z]+)\$?', name_for_linting(node))

def check_pascal_case(node):
    return re.fullmatch(r'_?(([A-Z][a-z]+|\d+)+|[A-Z]+)\$?', name_for_linting(node))

@basic_rule(chapel.core.VarLikeDecl)
def CamelCaseVariables(node):
    if node.name() == "_": return True
    return check_camel_case(node)

@basic_rule(chapel.core.Record)
def CamelCaseRecords(node):
    return check_camel_case(node)

@basic_rule(chapel.core.Class)
def PascalCaseClasses(node):
    return check_pascal_case(node)

@basic_rule(chapel.core.Module)
def PascalCaseModules(node):
    return check_pascal_case(node)

@basic_rule(chapel.core.Loop)
def DoKeywordAndBlock(node):
    return node.block_style() != "unnecessary"

@basic_rule(chapel.core.Coforall)
def NestedCoforalls(node):
    parent = node.parent()
    while parent is not None:
        if isinstance(parent, chapel.core.Coforall):
            return False
        parent = parent.parent()
    return True

@basic_rule([chapel.core.Conditional, chapel.core.BoolLiteral, chapel.rest])
def BoolLitInCondStmt(node):
    return False

@basic_rule(chapel.core.NamedDecl)
def ChplPrefixReserved(node):
    if node.name().startswith("chpl_"):
        path = node.location().path()
        return ctx.is_bundled_path(path)
    return True

@advanced_rule
def ConsecutiveDecls(root):
    def is_relevant_decl(node):
        if isinstance(node, chapel.core.MultiDecl):
            for child in node:
                if isinstance(child, chapel.core.Variable): return child.kind()
        elif isinstance(node, chapel.core.Variable):
            return node.kind()

        return None

    def recurse(node, skip_direct = False):
        consecutive = []
        last_kind = None
        last_has_attribute = False

        for child in node:
            yield from recurse(child, skip_direct = isinstance(child, chapel.core.MultiDecl))

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

@advanced_rule
def MisleadingIndentation(root):
    prev = None
    for child in root:
        yield from MisleadingIndentation(child)

        if prev is not None:
            if child.location().start()[1] == prev.location().start()[1]:
                yield child

        if isinstance(child, chapel.core.Loop) and child.block_style() == "implicit":
            grandchildren = list(child)
            if len(grandchildren) > 0:
                prev = list(grandchildren[-1])[0]

SilencedRules.extend([ "CamelCaseVariables", "ConsecutiveDecls" ])

def run_checks(asts):
    for ast in asts:
        for rule in BasicRules:
            yield from check_basic_rule(ast, rule)

        for rule in AdvancedRules:
            yield from check_advanced_rule(ast, rule)

def run_lsp():
    from pygls.server import LanguageServer
    from lsprotocol.types import TEXT_DOCUMENT_DID_OPEN, DidOpenTextDocumentParams
    from lsprotocol.types import TEXT_DOCUMENT_DID_SAVE, DidSaveTextDocumentParams
    from lsprotocol.types import Diagnostic, Range, Position, DiagnosticSeverity

    server = LanguageServer('chplcheck', 'v0.1')
    contexts = {}

    def get_updated_context(uri):
        if uri in contexts:
            context = contexts[uri]
            context.advance_to_next_revision(False)
        else:
            context = chapel.core.Context()
            contexts[uri] = context
        return context

    def parse_file(uri):
        context = get_updated_context(uri)
        return context.parse(uri[len("file://"):])

    def build_diagnostics(uri):
        asts = parse_file(uri)
        diagnostics = []
        for (node, rule) in run_checks(asts):
            location = node.location()
            start = location.start()
            end = location.end()

            diagnostic = Diagnostic(
                range=Range(
                    start=Position(start[0]-1, start[1]-1),
                    end=Position(end[0]-1, end[1]-1)
                ),
                message="Lint: rule [{}] violated".format(rule),
                severity=DiagnosticSeverity.Warning
            )
            diagnostics.append(diagnostic)
        return diagnostics

    @server.feature(TEXT_DOCUMENT_DID_OPEN)
    async def did_open(ls, params: DidOpenTextDocumentParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)
        ls.publish_diagnostics(text_doc.uri, build_diagnostics(text_doc.uri))

    @server.feature(TEXT_DOCUMENT_DID_SAVE)
    async def did_save(ls, params: DidSaveTextDocumentParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)
        ls.publish_diagnostics(text_doc.uri, build_diagnostics(text_doc.uri))

    server.start_io()

def main():
    global ctx

    parser = argparse.ArgumentParser( prog='chplcheck', description='A linter for the Chapel language')
    parser.add_argument('filenames', nargs='*')
    parser.add_argument('--ignore-rule', action='append', dest='ignored_rules', default=[])
    parser.add_argument('--lsp', action='store_true', default=False)
    args = parser.parse_args()

    SilencedRules.extend(args.ignored_rules)

    if args.lsp:
        run_lsp()
        return

    for (filename, ctx) in chapel.files_with_contexts(args.filenames):
        asts = ctx.parse(filename)
        for (node, rule) in run_checks(asts):
            print_violation(node, rule)

if __name__ == "__main__":
    main()
