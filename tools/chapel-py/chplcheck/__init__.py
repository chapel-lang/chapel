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
import sys
import argparse
from driver import *
from rules import *

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
