#!/usr/bin/env python3
#
# Copyright 2024-2024 Hewlett Packard Enterprise Development LP
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

from typing import (
    Any,
    Callable,
    Dict,
    Generic,
    Iterable,
    List,
    Optional,
    Set,
    Tuple,
    TypeVar,
    Union,
)
from collections import defaultdict
from dataclasses import dataclass, field
from bisect_compat import bisect_right
from symbol_signature import get_symbol_signature
import os


import chapel
from chapel.lsp import location_to_range, error_to_diagnostic
from chapel.visitor import visitor, enter
from pygls.server import LanguageServer
from lsprotocol.types import (
    Location,
    Diagnostic,
    Range,
    Position,
)
from lsprotocol.types import TEXT_DOCUMENT_DID_OPEN, DidOpenTextDocumentParams
from lsprotocol.types import TEXT_DOCUMENT_DID_SAVE, DidSaveTextDocumentParams
from lsprotocol.types import TEXT_DOCUMENT_DEFINITION, DefinitionParams
from lsprotocol.types import TEXT_DOCUMENT_REFERENCES, ReferenceParams
from lsprotocol.types import (
    TEXT_DOCUMENT_COMPLETION,
    CompletionParams,
    CompletionOptions,
    CompletionList,
    CompletionItem,
    CompletionItemKind,
)
from lsprotocol.types import (
    TEXT_DOCUMENT_DOCUMENT_SYMBOL,
    DocumentSymbolParams,
    SymbolInformation,
    SymbolKind,
)
from lsprotocol.types import (
    TEXT_DOCUMENT_HOVER,
    HoverParams,
    Hover,
    MarkupContent,
    MarkupKind,
)

from lsprotocol.types import WorkspaceFolder
import json


def decl_kind(decl: chapel.core.NamedDecl) -> Optional[SymbolKind]:
    if isinstance(decl, chapel.core.Module) and decl.kind() != "implicit":
        return SymbolKind.Module
    elif isinstance(decl, chapel.Class):
        return SymbolKind.Class
    elif isinstance(decl, chapel.Record):
        return SymbolKind.Struct
    elif isinstance(decl, chapel.Interface):
        return SymbolKind.Interface
    elif isinstance(decl, chapel.Enum):
        return SymbolKind.Enum
    elif isinstance(decl, chapel.EnumElement):
        return SymbolKind.EnumMember
    elif isinstance(decl, chapel.Function):
        if decl.is_method():
            return SymbolKind.Method
        elif decl.name() in ("init", "init="):
            return SymbolKind.Constructor
        elif decl.kind() == "operator":
            return SymbolKind.Operator
        else:
            return SymbolKind.Function
    elif isinstance(decl, chapel.Variable):
        if decl.is_field():
            return SymbolKind.Field
        elif decl.intent() == "<const-var>":
            return SymbolKind.Constant
        elif decl.intent() == "type":
            return SymbolKind.TypeParameter
        else:
            return SymbolKind.Variable
    return None


def decl_kind_to_completion_kind(kind: SymbolKind) -> CompletionItemKind:
    conversion_map = {
        SymbolKind.Module: CompletionItemKind.Module,
        SymbolKind.Class: CompletionItemKind.Class,
        SymbolKind.Struct: CompletionItemKind.Struct,
        SymbolKind.Interface: CompletionItemKind.Interface,
        SymbolKind.Enum: CompletionItemKind.Enum,
        SymbolKind.EnumMember: CompletionItemKind.EnumMember,
        SymbolKind.Method: CompletionItemKind.Method,
        SymbolKind.Constructor: CompletionItemKind.Constructor,
        SymbolKind.Operator: CompletionItemKind.Operator,
        SymbolKind.Function: CompletionItemKind.Function,
        SymbolKind.Field: CompletionItemKind.Field,
        SymbolKind.Constant: CompletionItemKind.Constant,
        SymbolKind.TypeParameter: CompletionItemKind.TypeParameter,
        SymbolKind.Variable: CompletionItemKind.Variable,
    }
    return conversion_map[kind]


def completion_item_for_decl(
    decl: chapel.NamedDecl,
) -> Optional[CompletionItem]:
    kind = decl_kind(decl)
    if not kind:
        return None

    return CompletionItem(
        label=decl.name(),
        kind=decl_kind_to_completion_kind(kind),
        insert_text=decl.name(),
        sort_text=decl.name(),
    )


def get_symbol_information(
    decl: chapel.NamedDecl, uri: str
) -> Optional[SymbolInformation]:
    loc = Location(uri, location_to_range(decl.location()))
    kind = decl_kind(decl)
    if kind:
        # TODO: should we use DocumentSymbol or SymbolInformation LSP spec says
        # prefer DocumentSymbol, but nesting doesn't work out of the box.
        # implies that we need some kind of visitor pattern to build a DS tree
        # using symbol information for now, as it sort-of autogets the tree
        # structure
        is_deprecated = chapel.is_deprecated(decl)
        return SymbolInformation(
            loc, decl.name(), kind, deprecated=is_deprecated
        )
    return None


EltT = TypeVar("EltT")


@dataclass
class PositionList(Generic[EltT]):
    get_range: Callable[[EltT], Range]
    elts: List[EltT] = field(default_factory=list)

    def sort(self):
        self.elts.sort(key=lambda x: self.get_range(x).start)

    def append(self, elt: EltT):
        self.elts.append(elt)

    def clear(self):
        self.elts.clear()

    def find(self, pos: Position) -> Optional[EltT]:
        idx = bisect_right(
            self.elts, pos, key=lambda x: self.get_range(x).start
        )
        idx -= 1
        if idx < 0 or pos > self.get_range(self.elts[idx]).end:
            return None
        return self.elts[idx]


@dataclass
class NodeAndRange:
    node: chapel.AstNode
    rng: Range = field(init=False)

    def __post_init__(self):
        if isinstance(self.node, chapel.Dot):
            self.rng = location_to_range(self.node.field_location())
        elif isinstance(self.node, chapel.NamedDecl):
            self.rng = location_to_range(self.node.name_location())
        else:
            self.rng = location_to_range(self.node.location())

    def get_location(self):
        return Location(self.get_uri(), self.rng)

    def get_uri(self):
        path = self.node.location().path()
        return f"file://{path}"


@dataclass
class ResolvedPair:
    ident: NodeAndRange
    resolved_to: NodeAndRange


@dataclass
@visitor
class FileInfo:
    uri: str
    context: chapel.Context
    workspace: Optional[WorkspaceFolder]
    use_segments: PositionList[ResolvedPair] = field(init=False)
    def_segments: PositionList[NodeAndRange] = field(init=False)
    uses_here: Dict[str, List[NodeAndRange]] = field(init=False)
    siblings: chapel.SiblingMap = field(init=False)
    used_modules: List[chapel.Module] = field(init=False)
    possibly_visible_decls: List[chapel.NamedDecl] = field(init=False)

    def __post_init__(self):
        self.use_segments = PositionList(lambda x: x.ident.rng)
        self.def_segments = PositionList(lambda x: x.rng)
        self.rebuild_index()

    def get_workspace_config(self) -> Optional[Dict]:
        if self.workspace is None:
            return None
        wsPath = self.workspace.uri[len("file://") :]
        configPath = os.path.join(wsPath, ".cls-config.json")
        config = dict()
        if os.path.exists(configPath):
            with open(configPath, "r") as fp:
                config = json.load(fp)
        return config

    def parse_file(self) -> List[chapel.AstNode]:
        """
        Parses this file and returns the toplevel ast elements

        Note: if there are errors they will be printed to the console.
        This call should be wrapped an appropriate error context.
        """
        path = self.uri[len("file://") :]

        directory = os.path.dirname(path)

        # prior to parsing the file, make sure its path is in the search path
        # this allows us to resolve sibling files
        self.context.add_module_path(directory)

        config = self.get_workspace_config()
        if config:
            module_dirs = config.get(path, [])
            for module_dir in module_dirs:
                for d in module_dir.get("module_dirs", []):
                    self.context.add_module_path(d)

        return self.context.parse(path)

    def get_asts(self) -> List[chapel.AstNode]:
        """
        Returns toplevel ast elements. This method silences all errors.
        """
        with self.context.track_errors() as _:
            return self.parse_file()

    def _note_reference(self, node: chapel.AstNode):
        """
        Given a node that can refer to another node, note what it refers
        to in by updating the 'use' segment table and the list of uses.
        """
        to = node.to_node()
        if not to:
            return

        self.uses_here[to.unique_id()].append(NodeAndRange(node))
        self.use_segments.append(
            ResolvedPair(NodeAndRange(node), NodeAndRange(to))
        )

    @enter
    def _enter_Identifier(self, node: chapel.Identifier):
        self._note_reference(node)

    @enter
    def _enter_Dot(self, node: chapel.Dot):
        self._note_reference(node)

    @enter
    def _enter_NamedDecl(self, node: chapel.NamedDecl):
        self.def_segments.append(NodeAndRange(node))

    def _collect_used_modules(self, asts: List[chapel.AstNode]):
        self.used_modules = []
        for ast in asts:
            scope = ast.scope()
            if scope:
                self.used_modules.extend(scope.used_imported_modules())

    def _collect_possibly_visible_decls(self):
        self.possibly_visible_decls = []
        for mod in self.used_modules:
            for child in mod:
                if not isinstance(child, chapel.NamedDecl):
                    continue

                if child.visibility() == "private":
                    continue

                self.possibly_visible_decls.append(child)

    def rebuild_index(self):
        """
        Rebuild the cached line info and siblings information

        Note: this is a potentially expensive operation, it should only be done
        when advancing the revision
        """
        asts = self.parse_file()

        # Use this class as an AST visitor to rebuild the use and definition segment
        # table, as well as the list of references.
        self.uses_here = defaultdict(list)
        self.use_segments.clear()
        self.def_segments.clear()
        self.visit(asts)
        self.use_segments.sort()
        self.def_segments.sort()

        self.siblings = chapel.SiblingMap(asts)
        self._collect_used_modules(asts)
        self._collect_possibly_visible_decls()

    def get_use_segment_at_position(
        self, position: Position
    ) -> Optional[ResolvedPair]:
        """lookup a use segment based upon a Position, likely a user mouse location"""
        return self.use_segments.find(position)

    def get_def_segment_at_position(
        self, position: Position
    ) -> Optional[NodeAndRange]:
        """lookup a def segment based upon a Position, likely a user mouse location"""
        return self.def_segments.find(position)


def run_lsp():
    """
    Start a language server on the standard input/output
    """
    server = LanguageServer("chpl-language-server", "v0.1")

    contexts: Dict[str, FileInfo] = {}

    def get_workspace_for_uri(
        ls: LanguageServer, uri: str
    ) -> Optional[WorkspaceFolder]:
        folders = ls.workspace.folders
        for f, ws in folders.items():
            if uri.startswith(f):
                return ws
        return None

    def get_context(
        ls: LanguageServer, uri: str, do_update: bool = False
    ) -> Tuple[FileInfo, List[Any]]:
        """
        The LSP driver maintains one Chapel context per-file. If there is no context, this function creates a context. If `do_update` is set, this function assumes the file content has change and advances revisions.
        """

        errors = []

        if uri in contexts:
            file_info = contexts[uri]
            if do_update:
                file_info.context.advance_to_next_revision(False)
                with file_info.context.track_errors() as errors:
                    file_info.rebuild_index()
        else:
            context = chapel.Context()
            with context.track_errors() as errors:
                ws = get_workspace_for_uri(ls, uri)
                file_info = FileInfo(uri, context, ws)
            contexts[uri] = file_info

        return (file_info, errors)

    def build_diagnostics(ls: LanguageServer, uri: str) -> List[Diagnostic]:
        """
        Parse a file at a particular URI, capture the errors, and return then
        as a list of LSP Diagnostics.
        """

        fi, errors = get_context(ls, uri, do_update=True)

        diagnostics = [error_to_diagnostic(e) for e in errors]
        return diagnostics

    # The following functions are handlers for LSP events received by the server.

    @server.feature(TEXT_DOCUMENT_DID_OPEN)
    @server.feature(TEXT_DOCUMENT_DID_SAVE)
    async def did_save(
        ls: LanguageServer,
        params: Union[DidSaveTextDocumentParams, DidOpenTextDocumentParams],
    ):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)
        diag = build_diagnostics(ls, text_doc.uri)
        ls.publish_diagnostics(text_doc.uri, diag)

    @server.feature(TEXT_DOCUMENT_DEFINITION)
    async def get_def(ls: LanguageServer, params: DefinitionParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = get_context(ls, text_doc.uri)
        segment = fi.get_use_segment_at_position(params.position)
        if segment:
            return segment.resolved_to.get_location()
        return None

    @server.feature(TEXT_DOCUMENT_REFERENCES)
    async def get_refs(ls: LanguageServer, params: ReferenceParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = get_context(ls, text_doc.uri)

        node_and_loc = None
        # First, search definitions. If the cursor is over a declaration,
        # that's what we're looking for.
        segment = fi.get_def_segment_at_position(params.position)
        if segment:
            node_and_loc = segment
        else:
            # Also search identifiers. If the cursor is over a reference,
            # we might as well try find all the other references.
            segment = fi.get_use_segment_at_position(params.position)
            if segment:
                node_and_loc = segment.resolved_to

        if not node_and_loc:
            return None

        locations = [node_and_loc.get_location()]
        for use in fi.uses_here[node_and_loc.node.unique_id()]:
            locations.append(use.get_location())

        return locations

    @server.feature(TEXT_DOCUMENT_DOCUMENT_SYMBOL)
    async def get_sym(ls: LanguageServer, params: DocumentSymbolParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = get_context(ls, text_doc.uri)

        # doesn't descend into nested definitions for Functions
        def preorder_ignore_funcs(node):
            yield node
            if isinstance(node, chapel.Function):
                return
            for child in node:
                yield from preorder_ignore_funcs(child)

        syms = []
        for node, _ in chapel.each_matching(
            fi.get_asts(),
            chapel.NamedDecl,
            iterator=preorder_ignore_funcs,
        ):
            si = get_symbol_information(node, text_doc.uri)
            if si:
                syms.append(si)

        return syms

    @server.feature(TEXT_DOCUMENT_HOVER)
    async def hover(ls: LanguageServer, params: HoverParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = get_context(ls, text_doc.uri)
        segment = fi.get_use_segment_at_position(params.position)
        if not segment:
            return None
        resolved_to = segment.resolved_to
        node_fi, _ = get_context(ls, resolved_to.get_uri())

        signature = get_symbol_signature(resolved_to.node)
        docstring = chapel.get_docstring(resolved_to.node, node_fi.siblings)
        text = f"```chapel\n{signature}\n```"
        if docstring:
            text += f"\n---\n{docstring}"
        content = MarkupContent(MarkupKind.Markdown, text)
        return Hover(content, range=resolved_to.get_location().range)

    @server.feature(TEXT_DOCUMENT_COMPLETION, CompletionOptions())
    async def complete(ls: LanguageServer, params: CompletionParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = get_context(ls, text_doc.uri)

        items = []
        items.extend(
            completion_item_for_decl(decl) for decl in fi.possibly_visible_decls
        )
        items.extend(completion_item_for_decl(mod) for mod in fi.used_modules)

        items = [item for item in items if item]

        return CompletionList(is_incomplete=False, items=items)

    server.start_io()


def main():
    run_lsp()


if __name__ == "__main__":
    main()
