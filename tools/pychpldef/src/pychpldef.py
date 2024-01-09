#!/usr/bin/env python3
#
# Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

from typing import Any, Callable, Dict, List, Optional, Tuple, Union


import chapel.core
from pygls.server import LanguageServer
from lsprotocol.types import TEXT_DOCUMENT_DID_OPEN, DidOpenTextDocumentParams
from lsprotocol.types import TEXT_DOCUMENT_DID_SAVE, DidSaveTextDocumentParams
from lsprotocol.types import TEXT_DOCUMENT_FORMATTING, DocumentFormattingParams
from lsprotocol.types import TEXT_DOCUMENT_DEFINITION, DefinitionParams, Location
from lsprotocol.types import (
    Diagnostic,
    Range,
    Position,
    DiagnosticSeverity,
    TextEdit,
    MessageType,
    TextDocumentEdit,
    WorkspaceConfigurationParams,
    WorkspaceEdit,
    ConfigurationItem,
    TEXT_DOCUMENT_CODE_ACTION,
    TEXT_DOCUMENT_DOCUMENT_SYMBOL,
    DocumentSymbolParams,
    CodeActionParams,
    CodeActionKind,
    CodeAction,
    SymbolInformation,
    SymbolKind,
)

import subprocess as sp
import re
from dataclasses import dataclass, field
from bisect import bisect_right


def location_to_range(location) -> Range:
    """
    Convert a Chapel location into a lsprotocol.types Range, which is
    used for e.g. reporting diagnostics.
    """

    start = location.start()
    end = location.end()
    return Range(
        start=Position(start[0] - 1, start[1] - 1), end=Position(end[0] - 1, end[1] - 1)
    )

def node_has_attribute(node: chapel.core.AstNode, marker: str) -> bool:
    """
    a symbol has an attribute if it has an attribute that matches the argument
    """
    return (attrs := node.attribute_group()) and any(a.name() == marker for a in attrs)


def is_deprecated(node: chapel.core.AstNode) -> bool:
    """a node is deprecated if it or its parent has an attribute that is deprecated"""
    return node_has_attribute(node, "deprecated")

def decl_kind(decl: chapel.core.NamedDecl) -> Optional[SymbolKind]:
    # File
    if isinstance(decl, chapel.core.Module):
        return SymbolKind.Module
    elif isinstance(decl, chapel.core.Class):
        return SymbolKind.Class
    elif isinstance(decl, chapel.core.Record):
        return SymbolKind.Struct
    elif isinstance(decl, chapel.core.Interface):
        return SymbolKind.Interface
    elif isinstance(decl, chapel.core.Enum):
        return SymbolKind.Enum
    elif isinstance(decl, chapel.core.EnumElement):
        return SymbolKind.EnumMember
    elif isinstance(decl, chapel.core.Function):
        if decl.is_method():
            return SymbolKind.Method
        elif decl.name() in ("init", "init="):
            return SymbolKind.Constructor
        elif decl.kind() == "operator":
            return SymbolKind.Operator
        else:
            return SymbolKind.Function
    elif isinstance(decl, chapel.core.Variable) and decl.is_field():
        return SymbolKind.Field
    elif isinstance(decl, chapel.core.Variable):
        if decl.is_field():
            return SymbolKind.Field
        # TODO: replace storage_kind with intent
        elif decl.storage_kind() == "<const-var>":
            return SymbolKind.Constant
        elif decl.storage_kind() == "type":
            return SymbolKind.TypeParameter
        else:
            return SymbolKind.Variable
    return None


def NamedDecl_to_SymbolInformation(
    decl: chapel.core.NamedDecl, uri: str
) -> Optional[SymbolInformation]:
    loc = Location(uri, location_to_range(decl.location()))
    if kind := decl_kind(decl):
        return SymbolInformation(loc, decl.name(), kind, deprecated=is_deprecated(decl))
    return None


@dataclass
class NodeAndRange:
    node: chapel.core.AstNode
    rng: Range = field(init=False)

    def __post_init__(self):
        self.rng = location_to_range(self.node.location())

    def get_location(self):
        path = self.node.location().path()
        return Location(f"file://{path}", self.rng)


@dataclass
class FileInfo:
    uri: str
    context: chapel.core.Context
    segments: List[Tuple[NodeAndRange, NodeAndRange]] = field(default_factory=list)

    def __post_init__(self):
        self.rebuild_segments()

    def parse_file(self) -> List[chapel.core.AstNode]:
        """
        Given a file URI, return the ASTs making up that file. Advances
        the context if one already exists to make sure an updated result
        is returned.
        """

        return self.context.parse(self.uri[len("file://") :])

    def get_asts(self) -> List[chapel.core.AstNode]:
        with self.context.track_errors() as _:
            return self.parse_file()

    def rebuild_segments(self):
        asts = self.get_asts()
        # get ids
        self.segments = [
            (NodeAndRange(node), NodeAndRange(to))
            for node, _ in chapel.each_matching(asts, chapel.core.Identifier)
            if (to := node.to_node())
        ]
        self.segments.sort(key=lambda s: s[0].rng.start)


def run_lsp():
    """
    Start a language server on the standard input/output, and use it to
    report linter warnings as LSP diagnostics.
    """

    server = LanguageServer("chplcheck", "v0.1")

    contexts: Dict[str, FileInfo] = {}

    def get_context(uri: str, do_update: bool = False) -> Tuple[FileInfo, List[Any]]:
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

        errors = []

        if uri in contexts:
            file_info = contexts[uri]
            if do_update:
                file_info.context.advance_to_next_revision(False)
                with file_info.context.track_errors() as errors:
                    file_info.rebuild_segments()
        else:
            context = chapel.core.Context()
            with context.track_errors() as errors:
                file_info = FileInfo(uri, context)
            contexts[uri] = file_info

        return (file_info, errors)

    def build_diagnostics(uri):
        """
        Parse a file at a particular URI, run the linter rules on the resulting
        ASTs, and return them as LSP diagnostics.
        """

        fi, errors = get_context(uri, do_update=True)
        with fi.context.track_errors() as new_errors:
            _ = fi.parse_file()
        errors.extend(new_errors)

        diagnostics = []

        kind_to_severity = {
            "error": DiagnosticSeverity.Error,
            "syntax": DiagnosticSeverity.Error,
            "note": DiagnosticSeverity.Information,
            "warning": DiagnosticSeverity.Warning,
        }

        for error in errors:
            diagnostic = Diagnostic(
                range=location_to_range(error.location()),
                message="{}: [{}]: {}".format(
                    error.kind().capitalize(), error.type(), error.message()
                ),
                severity=kind_to_severity[error.kind()],
            )
            diagnostics.append(diagnostic)
        return diagnostics

    # The following functions are handlers for LSP events received by the server.

    @server.feature(TEXT_DOCUMENT_DID_OPEN)
    @server.feature(TEXT_DOCUMENT_DID_SAVE)
    async def did_save(
        ls: LanguageServer,
        params: Union[DidSaveTextDocumentParams, DidOpenTextDocumentParams],
    ):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)
        diag = build_diagnostics(text_doc.uri)
        ls.publish_diagnostics(text_doc.uri, diag)

    @server.feature(TEXT_DOCUMENT_DEFINITION)
    async def get_def(ls: LanguageServer, params: DefinitionParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = get_context(text_doc.uri)
        segments = fi.segments

        idx = bisect_right(segments, params.position, key=lambda s: s[0].rng.start) - 1
        if idx >= 0 and params.position < segments[idx][0].rng.end:
            return [segments[idx][0].get_location(), segments[idx][1].get_location()]

        return None

    @server.feature(TEXT_DOCUMENT_DOCUMENT_SYMBOL)
    async def get_sym(ls: LanguageServer, params: DocumentSymbolParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = get_context(text_doc.uri)
        # TODO: should use custom traversal to ignore defs inside of functions
        syms = [
            si
            for node, _ in chapel.each_matching(fi.get_asts(), chapel.core.NamedDecl)
            if (si := NamedDecl_to_SymbolInformation(node, text_doc.uri))
        ]

        return syms

    server.start_io()


def main():
    run_lsp()


if __name__ == "__main__":
    main()
