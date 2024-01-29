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
import itertools
import os
import json
import re


import chapel
from chapel.lsp import location_to_range, error_to_diagnostic
from chapel.visitor import visitor, enter
from pygls.server import LanguageServer
from pygls.workspace import TextDocument
from lsprotocol.types import (
    Location,
    MessageType,
    Diagnostic,
    Range,
    Position,
)
from lsprotocol.types import TEXT_DOCUMENT_DID_OPEN, DidOpenTextDocumentParams
from lsprotocol.types import TEXT_DOCUMENT_DID_SAVE, DidSaveTextDocumentParams
from lsprotocol.types import TEXT_DOCUMENT_DEFINITION, DefinitionParams
from lsprotocol.types import TEXT_DOCUMENT_DECLARATION, DeclarationParams
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
from lsprotocol.types import (
    WORKSPACE_DID_CHANGE_WORKSPACE_FOLDERS,
    DidChangeWorkspaceFoldersParams,
)
from lsprotocol.types import (
    INITIALIZE,
    InitializeParams,
)
from lsprotocol.types import (
    TEXT_DOCUMENT_CODE_ACTION,
    CodeActionParams,
    CodeAction,
    TextEdit,
    CodeActionKind,
    WorkspaceEdit,
)


def decl_kind(decl: chapel.NamedDecl) -> Optional[SymbolKind]:
    if isinstance(decl, chapel.Module) and decl.kind() != "implicit":
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
        # TODO: should we use DocumentSymbol or SymbolInformation? LSP spec says
        # prefer DocumentSymbol, but nesting doesn't work out of the box.
        # implies that we need some kind of visitor pattern to build a DS tree
        # using symbol information for now, as it sort-of autogets the tree
        # structure
        is_deprecated = chapel.is_deprecated(decl)
        name = get_symbol_signature(decl)
        return SymbolInformation(loc, name, kind, deprecated=is_deprecated)
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
        elif isinstance(
            self.node,
            (chapel.Formal, chapel.Module, chapel.TypeDecl),
        ):
            self.rng = location_to_range(self.node.name_location())
        else:
            # TODO: Some NamedDecls are not reported using name_location().
            #       This is because name_location() is not correctly reported
            #       by the parser today.
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


class ContextContainer:
    def __init__(self, file: str, config: Optional["WorkspaceConfig"]):
        self.file_paths: List[str] = []
        self.module_paths: List[str] = [file]
        self.context: chapel.Context = chapel.Context()
        self.file_infos: List["FileInfo"] = []

        if config:
            file_config = config.for_file(file)
            if file_config:
                self.module_paths = file_config["module_dirs"]
                self.file_paths = file_config["files"]

        self.context.set_module_paths(self.module_paths, self.file_paths)

    def new_file_info(self, uri: str) -> Tuple["FileInfo", List[Any]]:
        """
        Creates a new FileInfo for a given URI. FileInfos constructed in
        this manner are tied to this ContextContainer, and have their
        indices rebuilt when the context updates. They also use
        this context object to perform parsing etc.
        """

        with self.context.track_errors() as errors:
            fi = FileInfo(uri, self)
            self.file_infos.append(fi)
        return (fi, errors)

    def advance(self) -> List[Any]:
        """
        Advances the Dyno context within to the next revision, and takes
        care of setting the necessary input queries in this revision. All
        dependent FileInfos are also updated since the file contents
        they represent may have changed.
        """

        self.context.advance_to_next_revision(False)
        self.context.set_module_paths(self.module_paths, self.file_paths)

        with self.context.track_errors() as errors:
            for fi in self.file_infos:
                fi.rebuild_index()
        return errors


@dataclass
@visitor
class FileInfo:
    uri: str
    context: ContextContainer
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

    def parse_file(self) -> List[chapel.AstNode]:
        """
        Parses this file and returns the toplevel ast elements

        Note: if there are errors they will be printed to the console.
        This call should be wrapped an appropriate error context.
        """

        return self.context.context.parse(self.uri[len("file://") :])

    def get_asts(self) -> List[chapel.AstNode]:
        """
        Returns toplevel ast elements. This method silences all errors.
        """
        with self.context.context.track_errors() as _:
            return self.parse_file()

    def _note_reference(
        self, node: Union[chapel.Dot, chapel.Identifier]
    ):
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


class WorkspaceConfig:
    def __init__(self, ls: "ChapelLanguageServer", json: Dict[str, Any]):
        self.files: Dict[str, Dict[str, Any]] = {}

        for key in json:
            compile_commands = json[key]

            if not isinstance(compile_commands, list):
                ls.show_message(
                    "invalid .cls-info.json file", MessageType.Error
                )
                continue

            # There can be several compile commands. They can conflict,
            # so we can't safely merge them (chpl -M modulesA and chpl -M modulesB
            # can lead to two different to-IDs etc.). However, we do expect
            # at least one compile command.
            if len(compile_commands) == 0:
                ls.show_message(
                    ".cls-info.json file contains invalid file commands",
                    MessageType.Error,
                )
                continue

            self.files[key] = compile_commands[0]

    def for_file(self, path: str) -> Optional[Dict[str, Any]]:
        if path in self.files:
            return self.files[path]
        return None

    @staticmethod
    def from_file(ls: "ChapelLanguageServer", path: str):
        if os.path.exists(path):
            with open(path) as f:
                commands = json.load(f)
                return WorkspaceConfig(ls, commands)
        return None


class ChapelLanguageServer(LanguageServer):
    def __init__(self):
        super().__init__("chpl-language-server", "v0.1")

        self.contexts: Dict[str, ContextContainer] = {}
        self.file_infos: Dict[str, FileInfo] = {}
        self.configurations: Dict[str, WorkspaceConfig] = {}

        self._setup_regexes()

    def _setup_regexes(self):
        """
        sets up regular expressions for use in text replacement for code actions
        """
        prefix = "Warning: \\[Deprecation\\]:"
        chars = "[a-zA-Z0-9_.:;,'`\\- ]*?"
        ident = "[a-zA-Z0-9_.()]+?"
        pat1 = f"{prefix}{chars}(?:'(?P<original1>{ident})'{chars})?'(?P<replace1>{ident})'{chars}"
        pat2 = f"{prefix}{chars}use{chars}(?P<tick>[`' ])(?P<replace2>{ident})(?P=tick){chars}instead{chars}"
        # use pat2 first since it is more specific
        self._find_rename_deprecation_regex = re.compile(f"({pat2})|({pat1})")

    def get_deprecation_replacement(self, text: str) -> Tuple[Optional[str], Optional[str]]:
        """
        Given a deprecation warning message, return the string to replace the deprecation with if possible
        """

        m = re.match(self._find_rename_deprecation_regex, text)
        if m and (m.group("replace1") or m.group("replace2")):
            replacement = m.group("replace1") or m.group("replace2")
            original = None
            if m.group("original1"):
                original =  m.group("original1")
            return (original, replacement)

        return (None, None)

    def get_config_for_uri(self, uri: str) -> Optional[WorkspaceConfig]:
        """
        In case multiple workspace folders are in use, pick the root folder
        that matches the given URI.
        """

        folders = self.workspace.folders
        for f, ws in folders.items():
            if uri.startswith(f):
                uri = ws.uri
                if uri in self.configurations:
                    return self.configurations[uri]
        return None

    def get_context(self, uri: str) -> ContextContainer:
        """
        Get the Chapel context for a given URI. Creating a new context
        for a file associates it with the file, as well as with any
        files that are associated with the file. For instance, if
        A.chpl imports B.chpl, and a context is created for either A.chpl
        or B.chpl, both files are associated with this new context.
        """

        path = uri[len("file://") :]
        workspace_config = self.get_config_for_uri(uri)

        if path in self.contexts:
            return self.contexts[path]

        context = ContextContainer(path, workspace_config)
        for file in context.file_paths:
            self.contexts[file] = context
        self.contexts[path] = context

        return context

    def get_file_info(
        self, uri: str, do_update: bool = False
    ) -> Tuple[FileInfo, List[Any]]:
        """
        The language server maintains a FileInfo object per file. The FileInfo
        contains precomputed information (binary-search-ready tables for
        finding an element under a cursor).

        This method retrieves the FileInfo object for a particular URI,
        creating one if it doesn't exist. If do_update is set to True,
        then the FileInfo's index is reuilt even if it has already been
        computed. This is useful if the underlying file has changed.
        """

        errors = []

        if uri in self.file_infos:
            file_info = self.file_infos[uri]
            if do_update:
                errors = file_info.context.advance()
        else:
            file_info, errors = self.get_context(uri).new_file_info(uri)
            self.file_infos[uri] = file_info

        return (file_info, errors)

    def build_diagnostics(self, uri: str) -> List[Diagnostic]:
        """
        Parse a file at a particular URI, capture the errors, and return then
        as a list of LSP Diagnostics.
        """

        _, errors = self.get_file_info(uri, do_update=True)

        diagnostics = [error_to_diagnostic(e) for e in errors]
        return diagnostics

    def get_text(self, text_doc: TextDocument, rng: Range) -> str:
        """
        Get the text of a TextDocument within a Range
        """
        start_line = rng.start.line
        stop_line = rng.end.line
        if start_line == stop_line:
            return text_doc.lines[start_line][
                rng.start.character : rng.end.character
            ]
        else:
            lines = text_doc.lines[start_line : stop_line + 1]
            lines[0] = lines[0][rng.start.character :]
            lines[-1] = lines[-1][: rng.end.character]
            return "\n".join(lines)

    def register_workspace(self, uri: str):
        path = os.path.join(uri[len("file://") :], ".cls-info.json")
        config = WorkspaceConfig.from_file(self, path)
        if config:
            self.configurations[uri] = config

    def unregister_workspace(self, uri: str):
        if uri in self.configurations:
            del self.configurations[uri]


def run_lsp():
    """
    Start a language server on the standard input/output
    """
    server = ChapelLanguageServer()

    # The following functions are handlers for LSP events received by the server.

    @server.feature(INITIALIZE)
    async def initialize(
        ls: ChapelLanguageServer,
        params: InitializeParams,
    ):
        if params.workspace_folders is None:
            return

        for ws in params.workspace_folders:
            ls.register_workspace(ws.uri)

    @server.feature(WORKSPACE_DID_CHANGE_WORKSPACE_FOLDERS)
    async def did_change_folders(
        ls: ChapelLanguageServer,
        params: DidChangeWorkspaceFoldersParams,
    ):
        for added in params.event.added:
            ls.register_workspace(added.uri)
        for removed in params.event.removed:
            ls.unregister_workspace(removed.uri)

    @server.feature(TEXT_DOCUMENT_DID_OPEN)
    @server.feature(TEXT_DOCUMENT_DID_SAVE)
    async def did_save(
        ls: ChapelLanguageServer,
        params: Union[DidSaveTextDocumentParams, DidOpenTextDocumentParams],
    ):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)
        diag = ls.build_diagnostics(text_doc.uri)
        ls.publish_diagnostics(text_doc.uri, diag)

    @server.feature(TEXT_DOCUMENT_DECLARATION)
    @server.feature(TEXT_DOCUMENT_DEFINITION)
    async def get_def(
        ls: ChapelLanguageServer,
        params: Union[DefinitionParams, DeclarationParams],
    ):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)
        segment = fi.get_use_segment_at_position(params.position)
        if segment:
            return segment.resolved_to.get_location()
        return None

    @server.feature(TEXT_DOCUMENT_REFERENCES)
    async def get_refs(ls: ChapelLanguageServer, params: ReferenceParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)

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
    async def get_sym(ls: ChapelLanguageServer, params: DocumentSymbolParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)

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
    async def hover(ls: ChapelLanguageServer, params: HoverParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)
        segment = fi.get_use_segment_at_position(params.position)
        if not segment:
            return None
        resolved_to = segment.resolved_to
        node_fi, _ = ls.get_file_info(resolved_to.get_uri())

        signature = get_symbol_signature(resolved_to.node)
        docstring = chapel.get_docstring(resolved_to.node, node_fi.siblings)
        text = f"```chapel\n{signature}\n```"
        if docstring:
            text += f"\n---\n{docstring}"
        content = MarkupContent(MarkupKind.Markdown, text)
        return Hover(content, range=resolved_to.get_location().range)

    @server.feature(TEXT_DOCUMENT_COMPLETION, CompletionOptions())
    async def complete(ls: ChapelLanguageServer, params: CompletionParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)

        items = []
        items.extend(
            completion_item_for_decl(decl) for decl in fi.possibly_visible_decls
        )
        items.extend(completion_item_for_decl(mod) for mod in fi.used_modules)

        items = [item for item in items if item]

        return CompletionList(is_incomplete=False, items=items)

    @server.feature(TEXT_DOCUMENT_CODE_ACTION)
    async def code_action(ls: ChapelLanguageServer, params: CodeActionParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)
        actions = []

        diagnostics_used: List[Diagnostic] = []
        edits_to_make: List[TextEdit] = []

        for d in params.context.diagnostics:
            original, replacement = ls.get_deprecation_replacement(d.message)
            if replacement is not None:
                full_text = ls.get_text(text_doc, d.range)
                if original is None:
                    original = full_text
                to_replace = full_text.replace(original, replacement)
                te = TextEdit(d.range, to_replace)
                msg = f"Resolve Deprecation: replace {original} with {to_replace}"
                diagnostics_used.append(d)
                edits_to_make.append(te)
                ca = CodeAction(
                    msg,
                    CodeActionKind.QuickFix,
                    diagnostics=[d],
                    edit=WorkspaceEdit(changes={text_doc.uri: [te]}),
                )
                actions.append(ca)

        if len(edits_to_make) > 0:
            actions.append(
                CodeAction(
                    "Resolve Deprecations",
                    CodeActionKind.SourceFixAll,
                    diagnostics=diagnostics_used,
                    edit=WorkspaceEdit(changes={text_doc.uri: edits_to_make}),
                    is_preferred=True,
                )
            )

        return actions

    server.start_io()


def main():
    run_lsp()


if __name__ == "__main__":
    main()
