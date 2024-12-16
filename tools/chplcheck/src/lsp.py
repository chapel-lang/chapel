#
# Copyright 2023-2024 Hewlett Packard Enterprise Development LP
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

from typing import Union, List
import chapel
import chapel.lsp
from pygls.server import LanguageServer
from lsprotocol.types import TEXT_DOCUMENT_DID_OPEN, DidOpenTextDocumentParams
from lsprotocol.types import TEXT_DOCUMENT_DID_SAVE, DidSaveTextDocumentParams
from lsprotocol.types import (
    TEXT_DOCUMENT_CODE_ACTION,
    CodeActionParams,
    CodeAction,
    CodeActionKind,
    WorkspaceEdit,
    TextEdit,
)
from lsprotocol.types import (
    Diagnostic,
    Range,
    Position,
    DiagnosticSeverity,
    CodeDescription,
)
from fixits import Fixit, Edit
from driver import LintDriver


def _get_location(node: chapel.AstNode):
    """Helper to get the location of a node"""
    if isinstance(node, chapel.NamedDecl):
        return chapel.lsp.location_to_range(node.name_location())
    else:
        return chapel.lsp.location_to_range(node.location())


def get_lint_diagnostics(
    context: chapel.Context, driver: LintDriver, asts: List[chapel.AstNode]
) -> List[Diagnostic]:
    """
    Run the linter rules on the Chapel ASTs and return them as LSP diagnostics.
    """
    diagnostics = []
    # Silence errors from scope resolution etc., especially since they
    # may be emitted from other files (dependencies).

    # get the version, keep only the major and minor version
    version = ".".join(context.get_compiler_version().split(".")[:2])
    base_url = (
        "https://chapel-lang.org/docs/{}/tools/chplcheck/chplcheck.html".format(
            version
        )
    )
    with context.track_errors() as _:
        for loc, node, rule, fixits in driver.run_checks(context, asts):
            diagnostic = Diagnostic(
                range=chapel.lsp.location_to_range(loc),
                message="Lint: rule [{}] violated".format(rule),
                severity=DiagnosticSeverity.Warning,
                code=rule,
                code_description=CodeDescription(base_url + "#" + rule.lower()),
            )
            if fixits:
                fixits = [Fixit.to_dict(f) for f in fixits]
                diagnostic.data = {"rule": rule, "fixits": fixits}
            diagnostics.append(diagnostic)
    return diagnostics


def get_lint_actions(diagnostics: List[Diagnostic]) -> List[CodeAction]:
    """
    Return LSP code actions for the given diagnostics.
    """
    actions = []
    for d in diagnostics:
        if not d.data:
            continue
        name = d.data.get("rule", None)
        if not name:
            continue
        if "fixits" not in d.data:
            continue
        fixits = [Fixit.from_dict(f) for f in d.data["fixits"]]
        if not fixits:
            continue

        for f in fixits:
            if not f:
                continue
            changes = dict()
            for e in f.edits:
                uri = "file://" + e.path
                start = e.start
                end = e.end
                rng = Range(
                    start=Position(max(start[0] - 1, 0), max(start[1] - 1, 0)),
                    end=Position(max(end[0] - 1, 0), max(end[1] - 1, 0)),
                )
                edit = TextEdit(range=rng, new_text=e.text)
                if uri not in changes:
                    changes[uri] = []
                changes[uri].append(edit)
            title = "Apply Fix for {}".format(name)
            if f.description:
                title += " ({})".format(f.description)
            action = CodeAction(
                title=title,
                kind=CodeActionKind.QuickFix,
                diagnostics=[d],
                edit=WorkspaceEdit(changes=changes),
            )
            actions.append(action)
    return actions


def run_lsp(driver: LintDriver):
    """
    Start a language server on the standard input/output, and use it to
    report linter warnings as LSP diagnostics.
    """

    server = LanguageServer("chplcheck", "v0.1")

    contexts = {}

    def get_updated_context(uri: str):
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
            context.set_module_paths([], [])
        else:
            context = chapel.core.Context()
            context.set_module_paths([], [])
            contexts[uri] = context
        return context

    def parse_file(context: chapel.Context, uri: str):
        """
        Given a file URI, return the ASTs making up that file. Advances
        the context if one already exists to make sure an updated result
        is returned.
        """

        return context.parse(uri[len("file://") :])

    def build_diagnostics(uri: str):
        """
        Parse a file at a particular URI, run the linter rules on the resulting
        ASTs, and return them as LSP diagnostics.
        """

        context = get_updated_context(uri)
        with context.track_errors() as errors:
            asts = parse_file(context, uri)

        diagnostics = get_lint_diagnostics(context, driver, asts)

        # process the errors from syntax/scope resolution
        # TODO: should chplcheck still do this?
        diagnostics += list([chapel.lsp.error_to_diagnostic(e) for e in errors])
        return diagnostics

    # The following functions are handlers for LSP events received by the server.

    @server.feature(TEXT_DOCUMENT_DID_OPEN)
    @server.feature(TEXT_DOCUMENT_DID_SAVE)
    async def did_open(
        ls: LanguageServer,
        params: Union[DidOpenTextDocumentParams, DidSaveTextDocumentParams],
    ):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)
        ls.publish_diagnostics(text_doc.uri, build_diagnostics(text_doc.uri))

    @server.feature(TEXT_DOCUMENT_CODE_ACTION)
    async def code_action(ls: LanguageServer, params: CodeActionParams):
        diagnostics = params.context.diagnostics
        actions = get_lint_actions(diagnostics)
        return actions

    server.start_io()
