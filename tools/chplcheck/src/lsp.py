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

import chapel.core
from pygls.server import LanguageServer
from lsprotocol.types import TEXT_DOCUMENT_DID_OPEN, DidOpenTextDocumentParams
from lsprotocol.types import TEXT_DOCUMENT_DID_SAVE, DidSaveTextDocumentParams
from lsprotocol.types import Diagnostic, Range, Position, DiagnosticSeverity

def location_to_range(location):
    """
    Convert a Chapel location into a lsprotocol.types Range, which is
    used for e.g. reporting diagnostics.
    """

    start = location.start()
    end = location.end()
    return Range(
        start=Position(start[0]-1, start[1]-1),
        end=Position(end[0]-1, end[1]-1)
    )

def run_lsp(driver):
    """
    Start a language server on the standard input/output, and use it to
    report linter warnings as LSP diagnostics.
    """

    server = LanguageServer('chplcheck', 'v0.1')

    contexts = {}
    def get_updated_context(uri):
        """
        The LSP driver maintains one Chapel context per-file. This is to avoid
        having to reset all files' text etc. when a single file is updated.
        There may be a more principled approach we can take in the future.

        This function returns an _update_ context, which is effectively a context
        in which we can save / make use of updated file text. If there wasn't
        a context for a URI, a brand new context will do. For existing contexts,
        an older version of the file's text is probably stored, so advance
        the context to next revision, invalidating that cache.

        Thus, this method is effectively allocate-or-advance-context.
        """

        if uri in contexts:
            context = contexts[uri]
            context.advance_to_next_revision(False)
        else:
            context = chapel.core.Context()
            contexts[uri] = context
        return context

    def parse_file(context, uri):
        """
        Given a file URI, return the ASTs making up that file. Advances
        the context if one already exists to make sure an updated result
        is returned.
        """

        return context.parse(uri[len("file://"):])

    def build_diagnostics(uri):
        """
        Parse a file at a particular URI, run the linter rules on the resulting
        ASTs, and return them as LSP diagnostics.
        """

        context = get_updated_context(uri)
        with context.track_errors() as errors:
            asts = parse_file(context, uri)

        # Silence errors from scope resolution etc., especially since they
        # may be emitted from other files (dependencies).
        with context.track_errors() as _:
            diagnostics = []
            for (node, rule) in driver.run_checks(context, asts):
                diagnostic = Diagnostic(
                    range= location_to_range(node.location()),
                    message="Lint: rule [{}] violated".format(rule),
                    severity=DiagnosticSeverity.Warning
                )
                diagnostics.append(diagnostic)

        kind_to_severity = {
            "error": DiagnosticSeverity.Error,
            "syntax": DiagnosticSeverity.Error,
            "note": DiagnosticSeverity.Information,
            "warning": DiagnosticSeverity.Warning,
        }

        for error in errors:
            diagnostic = Diagnostic(
                range= location_to_range(error.location()),
                message="{}: [{}]: {}".format(error.kind().capitalize(), error.type(), error.message()),
                severity=kind_to_severity[error.kind()]
            )
            diagnostics.append(diagnostic)
        return diagnostics

    # The following functions are handlers for LSP events received by the server.

    @server.feature(TEXT_DOCUMENT_DID_OPEN)
    async def did_open(ls, params: DidOpenTextDocumentParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)
        ls.publish_diagnostics(text_doc.uri, build_diagnostics(text_doc.uri))

    @server.feature(TEXT_DOCUMENT_DID_SAVE)
    async def did_save(ls, params: DidSaveTextDocumentParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)
        ls.publish_diagnostics(text_doc.uri, build_diagnostics(text_doc.uri))

    server.start_io()
