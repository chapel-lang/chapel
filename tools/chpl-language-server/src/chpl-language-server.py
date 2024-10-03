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
    Type,
    TypeVar,
    Union,
)
from types import ModuleType
from collections import defaultdict
from dataclasses import dataclass, field
from bisect_compat import bisect_left, bisect_right
from symbol_signature import SymbolSignature
import itertools
import os
import json
import re
import sys
import importlib.util

import chapel
from chapel.lsp import location_to_range, error_to_diagnostic
from chapel.visitor import visitor, enter
from pygls.server import LanguageServer
from pygls.workspace import TextDocument
from lsprotocol.types import (
    Location,
    MessageType,
    Diagnostic,
    DiagnosticRelatedInformation,
    Range,
    Position,
)
from lsprotocol.types import TEXT_DOCUMENT_DID_OPEN, DidOpenTextDocumentParams
from lsprotocol.types import TEXT_DOCUMENT_DID_SAVE, DidSaveTextDocumentParams
from lsprotocol.types import TEXT_DOCUMENT_DEFINITION, DefinitionParams
from lsprotocol.types import TEXT_DOCUMENT_TYPE_DEFINITION, TypeDefinitionParams
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
from lsprotocol.types import (
    TEXT_DOCUMENT_INLAY_HINT,
    InlayHintParams,
    InlayHint,
    InlayHintLabelPart,
)
from lsprotocol.types import WORKSPACE_INLAY_HINT_REFRESH
from lsprotocol.types import WORKSPACE_SEMANTIC_TOKENS_REFRESH
from lsprotocol.types import TEXT_DOCUMENT_RENAME, RenameParams
from lsprotocol.types import (
    TEXT_DOCUMENT_DOCUMENT_HIGHLIGHT,
    DocumentHighlightParams,
    DocumentHighlight,
    DocumentHighlightKind,
)
from lsprotocol.types import (
    TEXT_DOCUMENT_CODE_LENS,
    CodeLensParams,
    CodeLens,
    Command,
)
from lsprotocol.types import (
    TEXT_DOCUMENT_SEMANTIC_TOKENS_FULL,
    SemanticTokensRegistrationOptions,
    SemanticTokensLegend,
    SemanticTokensParams,
    SemanticTokens,
    SemanticTokenTypes,
)
from lsprotocol.types import (
    TEXT_DOCUMENT_PREPARE_CALL_HIERARCHY,
    CALL_HIERARCHY_INCOMING_CALLS,
    CALL_HIERARCHY_OUTGOING_CALLS,
    CallHierarchyPrepareParams,
    CallHierarchyIncomingCallsParams,
    CallHierarchyOutgoingCallsParams,
    CallHierarchyItem,
    CallHierarchyOutgoingCall,
    CallHierarchyIncomingCall,
)

import argparse
import configargparse


class ChplcheckProxy:

    def __init__(
        self,
        main: ModuleType,
        config: ModuleType,
        lsp: ModuleType,
        driver: ModuleType,
        rules: ModuleType,
    ):
        self.main = main
        self.config = config
        self.lsp = lsp
        self.driver = driver
        self.rules = rules

    @classmethod
    def get(cls) -> Optional["ChplcheckProxy"]:

        def error(msg: str):
            if os.environ.get("CHPL_DEVELOPER", None):
                print("Error loading chplcheck: ", str(e), file=sys.stderr)

        chpl_home = os.environ.get("CHPL_HOME")
        if chpl_home is None:
            error("CHPL_HOME not set")
            return None

        chplcheck_path = os.path.join(chpl_home, "tools", "chplcheck", "src")
        # Add chplcheck to the path, but load via importlib
        sys.path.append(chplcheck_path)

        def load_module(module_name: str) -> Optional[ModuleType]:
            file_path = os.path.join(chplcheck_path, module_name + ".py")
            spec = importlib.util.spec_from_file_location(
                module_name, file_path
            )
            if spec is None:
                error(f"Could not load module from {file_path}")
                return None
            module = importlib.util.module_from_spec(spec)
            sys.modules[module_name] = module
            if spec.loader is None:
                error(f"Could not load module from {file_path}")
                return None
            spec.loader.exec_module(module)
            return module

        mods = []
        for mod in ["chplcheck", "config", "lsp", "driver", "rules"]:
            m = load_module(mod)
            if m is None:
                return None
            mods.append(m)
        proxy = ChplcheckProxy(*mods)

        return proxy


chplcheck = ChplcheckProxy.get()


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
        if decl.name() in ("init", "init="):
            return SymbolKind.Constructor
        elif decl.kind() == "operator":
            return SymbolKind.Operator
        elif decl.is_method():
            return SymbolKind.Method
        else:
            return SymbolKind.Function
    elif isinstance(decl, chapel.Variable):
        if decl.intent() == "type":
            return SymbolKind.TypeParameter
        elif decl.intent() == "param":
            return SymbolKind.Constant
        elif decl.is_field():
            return SymbolKind.Field
        elif decl.intent() == "<const-var>":
            return SymbolKind.Constant
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
    decl: chapel.NamedDecl, override_name: Optional[str] = None
) -> Optional[CompletionItem]:
    kind = decl_kind(decl)
    if not kind:
        return None

    # For now, we show completion for global symbols (not x.<complete>),
    # so it seems like we ought to rule out methods.
    if kind == SymbolKind.Method:
        return None

    # We don't want to show operators in completion lists, as they're
    # not really useful to the user in this context.
    if kind == SymbolKind.Operator:
        return None

    name_to_use = override_name if override_name else decl.name()
    return CompletionItem(
        label=name_to_use,
        kind=decl_kind_to_completion_kind(kind),
        insert_text=name_to_use,
        sort_text=name_to_use,
    )


def location_to_location(loc) -> Location:
    return Location(
        "file://" + os.path.abspath(loc.path()), location_to_range(loc)
    )


def get_symbol_information(
    decl: chapel.NamedDecl,
) -> Optional[SymbolInformation]:
    loc = location_to_location(decl.location())
    kind = decl_kind(decl)
    if kind:
        # TODO: should we use DocumentSymbol or SymbolInformation? LSP spec says
        # prefer DocumentSymbol, but nesting doesn't work out of the box.
        # implies that we need some kind of visitor pattern to build a DS tree
        # using symbol information for now, as it sort-of autogets the tree
        # structure
        is_deprecated = chapel.is_deprecated(decl)
        signature = SymbolSignature(decl)
        return SymbolInformation(
            loc, str(signature), kind, deprecated=is_deprecated
        )
    return None


def encode_deltas(
    tokens: List[Tuple[int, int, int]], token_type: int, token_modifiers: int
) -> List[int]:
    """
    Given a (non-encoded) list of token positions, applies the LSP delta-encoding
    to it: each line is encoded as a delta from the previous line, and each
    column is encoded as a delta from the previous column.

    `tokens` must be sorted by line number, and then by column number within

    Returns tokens with type token_type, and modifiers token_modifiers.
    """

    encoded = []
    last_line = None
    last_col = 0
    for line, start, length in tokens:
        backup = line
        if line == last_line:
            start -= last_col
        if last_line is not None:
            line -= last_line
        last_line = backup

        encoded.extend([line, start, length, token_type, token_modifiers])
    return encoded


EltT = TypeVar("EltT")


@dataclass
class PositionList(Generic[EltT]):
    get_range: Callable[[EltT], Range]
    elts: List[EltT] = field(default_factory=list)

    def sort(self):
        self.elts.sort(key=lambda x: self.get_range(x).start)

    def append(self, elt: EltT):
        self.elts.append(elt)

    def _get_range(self, rng: Range):
        start = bisect_left(
            self.elts, rng.start, key=lambda x: self.get_range(x).start
        )
        end = bisect_right(
            self.elts, rng.end, key=lambda x: self.get_range(x).start
        )
        return (start, end)

    def overwrite(self, elt: EltT):
        rng = self.get_range(elt)
        start, end = self._get_range(rng)
        self.elts[start:end] = [elt]

    def remove_if(self, pred: Callable[[EltT], bool]):
        self.elts = [x for x in self.elts if not pred(x)]

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

    def range(self, rng: Range) -> List[EltT]:
        start, end = self._get_range(rng)
        return self.elts[start:end]


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
        path = os.path.abspath(self.node.location().path())
        return f"file://{path}"

    @staticmethod
    def for_entire_node(node: chapel.AstNode):
        """
        Create a NodeAndRange whose location spans the entire AST node, rather
        than its "relevant-for-hover" piece (i.e., its name).
        """
        res = NodeAndRange(node)
        res.rng = location_to_range(node.location())
        return res


@dataclass
class ResolvedPair:
    ident: NodeAndRange
    resolved_to: NodeAndRange


@dataclass
class References:
    in_file: "FileInfo"
    uses: List[NodeAndRange]

    def append(self, x: NodeAndRange):
        self.uses.append(x)

    def clear(self):
        self.uses.clear()

    def __iter__(self):
        return iter(self.uses)


@dataclass
class EndMarkerPattern:
    pattern: Union[Type, Set[Type]]
    header_location: Callable[[chapel.AstNode], Optional[chapel.Location]]
    goto_location: Callable[[chapel.AstNode], Optional[chapel.Location]]

    @classmethod
    def all(cls) -> Dict[str, "EndMarkerPattern"]:
        return {
            "loop": EndMarkerPattern(
                pattern=chapel.Loop,
                header_location=lambda node: (
                    node.header_location()
                    if node.block_style() != "implicit"
                    else None
                ),
                goto_location=lambda _: None,
            ),
            "decl": EndMarkerPattern(
                pattern=chapel.NamedDecl,
                header_location=lambda node: node.header_location(),
                goto_location=lambda node: node.name_location(),
            ),
            "block": EndMarkerPattern(
                pattern=set(
                    [
                        chapel.On,
                        chapel.Cobegin,
                        chapel.Begin,
                        chapel.Defer,
                        chapel.Serial,
                        chapel.Sync,
                        chapel.Local,
                        chapel.Manage,
                        chapel.Select,
                        chapel.When,
                    ]
                ),
                header_location=lambda node: (
                    node.block_header()
                    if not isinstance(node, chapel.SimpleBlockLike)
                    or node.block_style() != "implicit"
                    else None
                ),
                goto_location=lambda _: None,
            ),
        }


class ContextContainer:
    def __init__(self, file: str, config: Optional["WorkspaceConfig"]):
        self.config: Optional["WorkspaceConfig"] = config
        self.file_paths: List[str] = []
        self.module_paths: List[str] = [os.path.dirname(os.path.abspath(file))]
        self.context: chapel.Context = chapel.Context()
        self.file_infos: List["FileInfo"] = []
        self.global_uses: Dict[str, List[References]] = defaultdict(list)
        self.instantiation_ids: Dict[chapel.TypedSignature, str] = {}
        self.instantiation_id_counter = 0

        if config:
            file_config = config.for_file(file)
            if file_config:
                self.module_paths = file_config["module_dirs"]
                self.file_paths = file_config["files"]

        self.context.set_module_paths(self.module_paths, self.file_paths)

    def register_signature(self, sig: chapel.TypedSignature) -> str:
        """
        The language server can't send over typed signatures directly for
        situations such as call hierarchy items (but we need to reason about
        instantiations). Instead, keep a global unique ID for each signature,
        and use that to identify them.
        """
        if sig in self.instantiation_ids:
            return self.instantiation_ids[sig]

        self.instantiation_id_counter += 1
        uid = str(self.instantiation_id_counter)
        self.instantiation_ids[sig] = uid
        return uid

    def retrieve_signature(self, uid: str) -> Optional[chapel.TypedSignature]:
        for sig, sig_uid in self.instantiation_ids.items():
            if sig_uid == uid:
                return sig
        return None

    def new_file_info(
        self, uri: str, use_resolver: bool
    ) -> Tuple["FileInfo", List[Any]]:
        """
        Creates a new FileInfo for a given URI. FileInfos constructed in
        this manner are tied to this ContextContainer, and have their
        indices rebuilt when the context updates. They also use
        this context object to perform parsing etc.
        """

        with self.context.track_errors() as errors:
            fi = FileInfo(uri, self, use_resolver)
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


CallInTypeContext = Tuple[chapel.FnCall, Optional[chapel.TypedSignature]]
CallsInTypeContext = List[CallInTypeContext]


# We should show these variables in autocompletion even though they are 'nodoc'.
_ALLOWED_NODOC_DECLS = ["boundKind", "here", "strideKind"]


@dataclass
@visitor
class FileInfo:
    uri: str
    context: ContextContainer
    use_resolver: bool
    use_segments: PositionList[ResolvedPair] = field(init=False)
    def_segments: PositionList[NodeAndRange] = field(init=False)
    instantiation_segments: PositionList[
        Tuple[NodeAndRange, chapel.TypedSignature]
    ] = field(init=False)
    uses_here: Dict[str, References] = field(init=False)
    instantiations: Dict[
        str,
        Dict[chapel.TypedSignature, CallsInTypeContext],
    ] = field(init=False)
    siblings: chapel.SiblingMap = field(init=False)
    visible_decls: List[Tuple[str, chapel.AstNode]] = field(init=False)

    def __post_init__(self):
        self.use_segments = PositionList(lambda x: x.ident.rng)
        self.def_segments = PositionList(lambda x: x.rng)
        self.instantiation_segments = PositionList(lambda x: x[0].rng)
        self.uses_here = {}
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

    def _get_use_container(self, uid: str) -> References:
        if uid in self.uses_here:
            return self.uses_here[uid]

        refs = References(self, [])
        self.uses_here[uid] = refs
        self.context.global_uses[uid].append(refs)
        return refs

    def _note_reference(self, node: Union[chapel.Dot, chapel.Identifier]):
        """
        Given a node that can refer to another node, note what it refers
        to in by updating the 'use' segment table and the list of uses.
        """
        to = node.to_node()
        if not to:
            return

        self._get_use_container(to.unique_id()).append(NodeAndRange(node))
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
    def _enter_Module(self, node: chapel.Module):
        # Trigger scope resolution to error duplicate variable warnings.
        _ = node.scope_resolve()

        self.def_segments.append(NodeAndRange(node))

    @enter
    def _enter_Function(self, node: chapel.Function):
        # Trigger scope resolution to error duplicate variable warnings.
        _ = node.scope_resolve()

        self.def_segments.append(NodeAndRange(node))

    @enter
    def _enter_NamedDecl(self, node: chapel.NamedDecl):
        self.def_segments.append(NodeAndRange(node))

    def _collect_possibly_visible_decls(self, asts: List[chapel.AstNode]):
        self.visible_decls = []
        for ast in asts:
            if isinstance(ast, chapel.Comment):
                continue

            scope = ast.scope()
            if not scope:
                continue

            file = ast.location().path()
            in_bundled_module = self.context.context.is_bundled_path(file)

            for name, nodes in scope.visible_nodes():
                # Don't show internal symbols to the user, even if they
                # are technically in scope. The exception is if we're currently
                # editing a standard file.
                skip_prefixes = ["chpl_", "chpldev_", "_"]
                if any(name.startswith(prefix) for prefix in skip_prefixes):
                    if not in_bundled_module:
                        continue

                # Only show nodes without @chpldoc.nodoc. The exception
                # about standard files applies here too.
                documented_nodes = []
                for node in nodes:
                    # apply aforementioned exception
                    if in_bundled_module:
                        documented_nodes.append(node)
                        continue

                    # avoid nodes with nodoc attribute.
                    ag = node.attribute_group()
                    show = False
                    if not ag or not ag.get_attribute_named("chpldoc.nodoc"):
                        show = True
                    elif name in _ALLOWED_NODOC_DECLS:
                        # If users declare variables like 'here' themselves,
                        # we will not show them if they're @chpldoc.nodoc,
                        # since they're not special.
                        decl_file = node.location().path()
                        is_standard_decl = self.context.context.is_bundled_path(
                            decl_file
                        )
                        show = is_standard_decl

                    if show:
                        documented_nodes.append(node)

                if len(documented_nodes) == 0:
                    continue

                # Just take the first value to avoid showing N entries for
                # overloaded functions.
                self.visible_decls.append((name, documented_nodes[0]))

    def _search_instantiations(
        self,
        root: Union[chapel.AstNode, List[chapel.AstNode]],
        via: Optional[chapel.TypedSignature] = None,
    ):
        for node in chapel.preorder(root):
            if not isinstance(node, chapel.FnCall):
                continue

            rr = node.resolve_via(via) if via else node.resolve()
            if not rr:
                continue

            candidate = rr.most_specific_candidate()
            if not candidate:
                continue

            sig = candidate.function()
            fn = sig.ast()
            if not fn or not isinstance(fn, chapel.Function):
                continue

            # Even if we don't descend into it (and even if it's not an
            # instantiation), track the call that invoked this function.
            # This will help with call hierarchy.
            insts = self.instantiations[fn.unique_id()]
            already_visited = sig in insts
            insts[sig].append((node, via))

            if not sig.is_instantiation() or already_visited:
                continue

            self._search_instantiations(fn, via=sig)

    def rebuild_index(self):
        """
        Rebuild the cached line info and siblings information

        Note: this is a potentially expensive operation, it should only be done
        when advancing the revision
        """
        asts = self.parse_file()

        # Use this class as an AST visitor to rebuild the use and definition segment
        # table, as well as the list of references.
        self.instantiations = defaultdict(lambda: defaultdict(list))
        for _, refs in self.uses_here.items():
            refs.clear()
        self.use_segments.clear()
        self.def_segments.clear()
        self.visit(asts)
        self.use_segments.sort()
        self.def_segments.sort()

        self.siblings = chapel.SiblingMap(asts)
        self._collect_possibly_visible_decls(asts)

        if self.use_resolver:
            # TODO: suppress resolution errors due to false-positives
            # this should be removed once the resolver is finished
            with self.context.context.track_errors() as _:
                self._search_instantiations(asts)

    def called_function_at_position(
        self, position: Position
    ) -> Optional[chapel.TypedSignature]:
        """
        Given a particular position, finds the function being called at that
        position.

        Note: this function implies using resolution, and should only
        be called if the resolver is enabled.
        """

        # TODO: Performance:
        # since we don't have "call segments" (or segments for any other type
        # of node), we have to iterate over all calls and check if they're in
        # range. We can do better if we track segments for these.
        call = None
        for ast, _ in chapel.each_matching(self.get_asts(), chapel.FnCall):
            rng = location_to_range(ast.location())
            if rng.start <= position <= rng.end:
                call = ast

        if call is None:
            return None

        instantiation = self.get_inst_segment_at_position(position)
        rr = (
            call.resolve_via(instantiation) if instantiation else call.resolve()
        )

        if rr is None:
            return None

        msc = rr.most_specific_candidate()
        if msc is None:
            return None

        return msc.function()

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

    def get_inst_segment_at_position(
        self, position: Position
    ) -> Optional[chapel.TypedSignature]:
        """lookup a instantiation segment based upon a Position, likely a user mouse location"""
        segment = self.instantiation_segments.find(position)
        if segment:
            return segment[1]
        return None

    def get_use_or_def_segment_at_position(
        self, position: Position
    ) -> Optional[NodeAndRange]:
        """
        Retrieve the definition or reference to a definition at the given position.
        This method is intended for LSP queries that ask for some property
        of a definition: its type, references to it, etc. However, it is
        convenient to be able to "find references" and "go to type definition"
        from references to a definition too. Thus, this method returns
        a reference when it can, and falls back to definition otherwise.
        """

        segment = self.get_use_segment_at_position(position)
        if segment:
            return segment.resolved_to
        else:
            segment = self.get_def_segment_at_position(position)
            if segment:
                return segment

        return None

    def file_lines(self) -> List[str]:
        file_text = self.context.context.get_file_text(
            self.uri[len("file://") :]
        )
        return file_text.splitlines()

    def instantiation_at_index(
        self, fn: chapel.Function, idx: int
    ) -> chapel.TypedSignature:
        """
        Given a function, return its nth instantiation. This uses the list of
        instantiations collected while rebuilding the index.
        """
        return next(
            itertools.islice(self.instantiations[fn.unique_id()], idx, None)
        )

    def index_of_instantiation(
        self, fn: chapel.Function, sig: chapel.TypedSignature
    ) -> int:
        """
        Given a function and an instantiation of that function, returns the
        instantiation's index in the list of all instantiations.
        """
        return next(
            (
                i
                for i, s in enumerate(self.instantiations[fn.unique_id()])
                if s == sig
            ),
            -1,
        )

    def concrete_instantiation_for(
        self, fn: chapel.Function
    ) -> Optional[chapel.TypedSignature]:
        """
        If all we have is a function ID, we can still select a particular
        typed signature for it in some cases, even without calls: the
        concrete signature when a function is non-generic. Return
        that signature, if it exists for the given function.
        """
        uid = fn.unique_id()
        if uid in self.instantiations:
            for sig in self.instantiations[uid]:
                if not sig.is_instantiation():
                    return sig
        return None


class WorkspaceConfig:
    def __init__(self, ls: "ChapelLanguageServer", json: Dict[str, Any]):
        self.files: Dict[str, Dict[str, Any]] = {}

        for key in json:
            compile_commands = json[key]

            if not isinstance(compile_commands, list):
                ls.show_message(
                    "invalid .cls-commands.json file", MessageType.Error
                )
                continue

            # There can be several compile commands. They can conflict,
            # so we can't safely merge them (chpl -M modulesA and chpl -M modulesB
            # can lead to two different to-IDs etc.). However, we do expect
            # at least one compile command.
            if len(compile_commands) == 0:
                ls.show_message(
                    ".cls-commands.json file contains invalid file commands",
                    MessageType.Error,
                )
                continue

            self.files[key] = compile_commands[0]

    def file_paths(self) -> Iterable[str]:
        return self.files.keys()

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


class CLSConfig:
    def __init__(self):
        self._construct_parser()
        self.args = dict()

    def _construct_parser(self):
        self.parser = configargparse.ArgParser(
            default_config_files=[],  # Empty for now because cwd() is odd with VSCode etc.
            config_file_parser_class=configargparse.YAMLConfigFileParser,
        )

        def add_bool_flag(name: str, dest: str, default: bool):
            self.parser.add_argument(
                f"--{name}", dest=dest, action="store_true"
            )
            self.parser.add_argument(
                f"--no-{name}", dest=dest, action="store_false"
            )
            self.parser.set_defaults(**{dest: default})

        add_bool_flag("resolver", "resolver", False)
        add_bool_flag("type-inlays", "type_inlays", True)
        add_bool_flag("param-inlays", "param_inlays", True)
        add_bool_flag("literal-arg-inlays", "literal_arg_inlays", True)
        add_bool_flag("dead-code", "dead_code", True)
        add_bool_flag("evaluate-expressions", "eval_expressions", True)
        add_bool_flag("show-instantiations", "show_instantiations", True)
        self.parser.add_argument("--end-markers", default="none")
        self.parser.add_argument("--end-marker-threshold", type=int, default=10)

        add_bool_flag("chplcheck", "do_linting", False)
        if chplcheck:
            chplcheck.config.Config.add_arguments(self.parser, "chplcheck-")

    def _parse_end_markers(self):
        self.args["end_markers"] = [
            a.strip() for a in self.args["end_markers"].split(",")
        ]

    def _validate_end_markers(self):
        valid_choices = ["all", "none"] + list(EndMarkerPattern.all().keys())
        for marker in self.args["end_markers"]:
            if marker not in valid_choices:
                raise argparse.ArgumentError(
                    None, f"Invalid end marker choice: {marker}"
                )
        n_markers = len(self.args["end_markers"])
        if n_markers != len(set(self.args["end_markers"])):
            raise argparse.ArgumentError(
                None, "Cannot specify the same end marker multiple times"
            )
        if "none" in self.args["end_markers"] and n_markers > 1:
            raise argparse.ArgumentError(
                None, "Cannot specify 'none' with other end marker choices"
            )
        if "all" in self.args["end_markers"] and n_markers > 1:
            raise argparse.ArgumentError(
                None, "Cannot specify 'all' with other end marker choices"
            )

    def parse_args(self):
        self.args = vars(self.parser.parse_args())
        self._parse_end_markers()
        self._validate_end_markers()

    def get(self, key: str):
        return self.args[key]


class ChapelLanguageServer(LanguageServer):
    def __init__(self, config: CLSConfig):
        super().__init__("chpl-language-server", "v0.1")

        self.contexts: Dict[str, ContextContainer] = {}
        self.context_ids: Dict[ContextContainer, str] = {}
        self.context_id_counter = 0
        self.file_infos: Dict[Tuple[str, Optional[str]], FileInfo] = {}
        self.configurations: Dict[str, WorkspaceConfig] = {}

        self.use_resolver: bool = config.get("resolver")
        self.type_inlays: bool = config.get("type_inlays")
        self.literal_arg_inlays: bool = config.get("literal_arg_inlays")
        self.param_inlays: bool = config.get("param_inlays")
        self.dead_code: bool = config.get("dead_code")
        self.eval_expressions: bool = config.get("eval_expressions")
        self.show_instantiations: bool = config.get("show_instantiations")
        self.end_markers: List[str] = config.get("end_markers")
        self.end_marker_threshold: int = config.get("end_marker_threshold")
        self.end_marker_patterns = self._get_end_marker_patterns()
        self.do_linting: bool = config.get("do_linting")

        self.lint_driver = None
        self._setup_linter(config)

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

    def _setup_linter(self, clsConfig: CLSConfig):
        """
        Setup the linter, if it is enabled
        """
        if not (self.do_linting and chplcheck):
            return

        config = chplcheck.config.Config.from_args(clsConfig.args)
        self.lint_driver = chplcheck.driver.LintDriver(config)

        chplcheck.rules.register_rules(self.lint_driver)

        for p in config.add_rules:
            chplcheck.main.load_module(self.lint_driver, os.path.abspath(p))

        self.lint_driver.disable_rules(*config.disabled_rules)
        self.lint_driver.enable_rules(*config.enabled_rules)

    def get_deprecation_replacement(
        self, text: str
    ) -> Tuple[Optional[str], Optional[str]]:
        """
        Given a deprecation warning message, return the string to replace the deprecation with if possible
        """

        m = re.match(self._find_rename_deprecation_regex, text)
        if m and (m.group("replace1") or m.group("replace2")):
            replacement = m.group("replace1") or m.group("replace2")
            original = None
            if m.group("original1"):
                original = m.group("original1")
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
        self.context_id_counter += 1
        self.context_ids[context] = str(self.context_id_counter)

        return context

    def retrieve_context(self, context_id: str) -> Optional[ContextContainer]:
        for ctx, cid in self.context_ids.items():
            if cid == context_id:
                return ctx
        return None

    def eagerly_process_all_files(self, context: ContextContainer):
        cfg = context.config
        if cfg:
            for file in cfg.files:
                self.get_file_info("file://" + file, do_update=False)

    def get_file_info(
        self, uri: str, do_update: bool = False, context_id: Optional[str] = None
    ) -> Tuple[FileInfo, List[Any]]:
        """
        The language server maintains a FileInfo object per file. The FileInfo
        contains precomputed information (binary-search-ready tables for
        finding an element under a cursor).

        This method retrieves the FileInfo object for a particular URI,
        creating one if it doesn't exist. If do_update is set to True,
        then the FileInfo's index is rebuilt even if it has already been
        computed. This is useful if the underlying file has changed.

        Most of the tiem, we will create a new context for a given URI. When
        requested, however, context_id will be used to create a FileInfo
        for a specific context. This is useful if e.g., file A wants to display
        an instantiation in file B.
        """

        errors = []

        fi_key = (uri, context_id)
        if fi_key in self.file_infos:
            file_info = self.file_infos[fi_key]
            if do_update:
                errors = file_info.context.advance()
        else:
            if context_id:
                context = self.retrieve_context(context_id)
                assert(context)
            else:
                context = self.get_context(uri)

            file_info, errors = context.new_file_info(
                uri, self.use_resolver
            )
            self.file_infos[fi_key] = file_info

            # Also make this the "default" context for this file in case we
            # open it.
            if (uri, None) not in self.file_infos:
                self.file_infos[(uri, None)] = file_info

        # filter out errors that are not related to the file
        cur_path = uri[len("file://") :]
        errors = [e for e in errors if e.location().path() == cur_path]

        return (file_info, errors)

    def build_diagnostics(self, uri: str) -> List[Diagnostic]:
        """
        Parse a file at a particular URI, capture the errors, and return then
        as a list of LSP Diagnostics.
        """

        fi, errors = self.get_file_info(uri, do_update=True)

        diagnostics = []
        for e in errors:
            diag = error_to_diagnostic(e)
            diag.related_information = [
                DiagnosticRelatedInformation(
                    location_to_location(note_loc), note_msg
                )
                for (note_loc, note_msg) in e.notes()
            ]
            diagnostics.append(diag)

        # get lint diagnostics if applicable
        if self.lint_driver and chplcheck:
            lint_diagnostics = chplcheck.lsp.get_lint_diagnostics(
                fi.context.context, self.lint_driver, fi.get_asts()
            )
            diagnostics.extend(lint_diagnostics)

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
        path = os.path.join(uri[len("file://") :], ".cls-commands.json")
        config = WorkspaceConfig.from_file(self, path)
        if config:
            self.configurations[uri] = config

    def unregister_workspace(self, uri: str):
        if uri in self.configurations:
            del self.configurations[uri]

    def _get_param_inlays(
        self, decl: NodeAndRange, qt: chapel.QualifiedType
    ) -> List[InlayHint]:
        if not self.param_inlays:
            return []

        _, _, param = qt
        if not param:
            return []

        return [
            InlayHint(
                position=decl.rng.end,
                label="param value is " + str(param),
                padding_left=True,
            )
        ]

    def _get_type_inlays(
        self, decl: NodeAndRange, qt: chapel.QualifiedType
    ) -> List[InlayHint]:
        if not self.type_inlays:
            return []

        # Only show type hints for variable declarations that don't have
        # an explicit type, and whose type is valid.
        _, type_, _ = qt
        if (
            not isinstance(decl.node, (chapel.Variable, chapel.Formal))
            or decl.node.type_expression() is not None
            or isinstance(type_, chapel.ErroneousType)
        ):
            return []
        # skip implicit this formals
        if (
            isinstance(decl.node, chapel.Formal)
            and isinstance(decl.node.parent(), chapel.Function)
            and decl.node.parent().this_formal() is not None
            and decl.node.unique_id()
            == decl.node.parent().this_formal().unique_id()
        ):
            return []

        name_rng = location_to_range(decl.node.name_location())
        type_str = ": " + str(type_)
        text_edits = [TextEdit(Range(name_rng.end, name_rng.end), type_str)]
        colon_label = InlayHintLabelPart(": ")
        label = InlayHintLabelPart(str(type_))
        if isinstance(type_, chapel.CompositeType):
            typedecl = type_.decl()
            if typedecl and isinstance(typedecl, chapel.NamedDecl):
                label.location = location_to_location(typedecl.name_location())
            elif typedecl:
                label.location = location_to_location(typedecl.location())

        # if the inlay hint is for a loop index type, we cannot insert the type
        # as it would be a syntax error
        parent_loop = decl.node.parent()
        if parent_loop and isinstance(parent_loop, chapel.IndexableLoop):
            index = parent_loop.index()
            if index and index.unique_id() == decl.node.unique_id():
                text_edits = None

        return [
            InlayHint(
                position=name_rng.end,
                label=[colon_label, label],
                text_edits=text_edits,
            )
        ]

    def get_decl_inlays(
        self,
        decl: NodeAndRange,
        via: Optional[chapel.TypedSignature] = None,
    ) -> List[InlayHint]:
        if not self.use_resolver:
            return []

        rr = decl.node.resolve_via(via) if via else decl.node.resolve()
        if not rr:
            return []

        qt = rr.type()
        if qt is None:
            return []

        inlays = []
        inlays.extend(self._get_param_inlays(decl, qt))
        inlays.extend(self._get_type_inlays(decl, qt))
        return inlays

    def get_call_inlays(
        self, call: chapel.FnCall, via: Optional[chapel.TypedSignature] = None
    ) -> List[InlayHint]:
        if not self.literal_arg_inlays or not self.use_resolver:
            return []

        rr = call.resolve_via(via) if via else call.resolve()
        if not rr:
            return []

        msc = rr.most_specific_candidate()
        if not msc:
            return []

        fn = msc.function().ast()
        if not fn or not isinstance(fn, chapel.core.Function):
            return []

        inlays = []
        for i, act in zip(msc.formal_actual_mapping(), call.actuals()):
            if not isinstance(act, chapel.core.AstNode):
                # Named arguments are represented using (name, node)
                # tuples. We don't need hints for those.
                continue

            if not chapel.is_literal_like(act):
                # Only show named arguments for literals.
                continue

            fml = fn.formal(i)
            if not isinstance(fml, chapel.core.Formal):
                continue

            begin = location_to_range(act.location()).start
            inlays.append(
                InlayHint(
                    position=begin,
                    label=fml.name() + " = ",
                )
            )

        return inlays

    def get_tooltip(
        self,
        node: chapel.AstNode,
        siblings: chapel.SiblingMap,
        via: Optional[chapel.TypedSignature] = None,
    ) -> str:
        signature = SymbolSignature(node)
        if self.use_resolver:
            signature.compute_type(via)
            if self.eval_expressions:
                signature.compute_value(via)

        docstring = chapel.get_docstring(node, siblings)
        text = f"```chapel\n{signature}\n```"
        if docstring:
            text += f"\n---\n{docstring}"
        return text

    def get_dead_code_tokens(
        self,
        node: chapel.Conditional,
        lines: list[str],
        via: Optional[chapel.TypedSignature] = None,
    ) -> List[Tuple[int, int, int]]:
        if not self.dead_code or not self.use_resolver:
            return []

        rr = (
            node.condition().resolve_via(via)
            if via
            else node.condition().resolve()
        )
        if not rr:
            return []

        qt = rr.type()
        if qt is None:
            return []

        _, _, val = qt
        if isinstance(val, chapel.BoolParam):
            dead_branch = (
                node.else_block() if val.value() else node.then_block()
            )
            if dead_branch:
                loc = dead_branch.location()
                return chapel.range_to_tokens(loc, lines)

        return []

    def sym_to_call_hierarchy_item(
        self, sym: chapel.NamedDecl
    ) -> CallHierarchyItem:
        """
        Given a Chapel symbol declaration as a NamedDecl, return the
        corresponding call hierarchy item.
        """
        loc = location_to_location(sym.location())

        inst_id = ""
        context_id = None

        return CallHierarchyItem(
            name=sym.name(),
            detail=str(SymbolSignature(sym)),
            kind=decl_kind(sym) or SymbolKind.Function,
            uri=loc.uri,
            range=loc.range,
            selection_range=location_to_range(sym.name_location()),
            data=[sym.unique_id(), inst_id, context_id],
        )

    def fn_to_call_hierarchy_item(
        self, sig: chapel.TypedSignature, caller_context: ContextContainer
    ) -> CallHierarchyItem:
        """
        Like sym_to_call_hierarchy_item, but for function instantiations.
        The additional information can be used to represent call hierarchy
        situations where only some instantiations of a function call
        another function.
        """
        fn: chapel.Function = sig.ast()
        item = self.sym_to_call_hierarchy_item(fn)
        item.data[1] = caller_context.register_signature(sig)
        item.data[2] = self.context_ids[caller_context]

        return item

    def unpack_call_hierarchy_item(
        self, item: CallHierarchyItem
    ) -> Optional[
        Tuple[FileInfo, chapel.Function, Optional[chapel.TypedSignature]]
    ]:
        if (
            item.data is None
            or not isinstance(item.data, list)
            or not isinstance(item.data[0], str)
            or not isinstance(item.data[1], str)
            or not isinstance(item.data[2], str)
        ):
            self.show_message(
                "Call hierarchy item contains missing or invalid additional data",
                MessageType.Error,
            )
            return None
        uid, idx, ctx = item.data

        context_id = None
        if ctx != "":
            context_id = ctx

        print(f"Context: {context_id}", file=sys.stderr)

        fi, _ = self.get_file_info(item.uri, context_id=context_id)

        # TODO: Performance:
        # Once the Python bindings supports it, we can use the
        # "ID to AST" function from parsing to do this without iterating.
        for node, _ in chapel.each_matching(fi.get_asts(), chapel.Function):
            if node.unique_id() == item.data[0]:
                fn = node
                break
        else:
            # The call hierarchy item could've been a module or something else.
            # We don't handle that here.
            return None

        instantiation = fi.context.retrieve_signature(idx)

        return (fi, fn, instantiation)

    def _get_end_marker_patterns(self) -> Dict[str, EndMarkerPattern]:
        """
        Returns the patterns for the active end markers, based upon `--end-markers`
        """
        active_patterns = dict()
        patterns = EndMarkerPattern.all()
        if "all" in self.end_markers:
            active_patterns = patterns
        elif "none" not in self.end_markers:
            for marker in self.end_markers:
                active_patterns[marker] = patterns[marker]
        return active_patterns

    def get_end_markers(
        self, ast: List[chapel.AstNode], file_lines: List[str]
    ) -> List[InlayHint]:
        """
        Get the inlay hints that mark the end of significant blocks of code.
        This is based upon the active patterns in `end_marker_patterns`. This
        function also checks if the block is less than some threshold.
        """
        inlays = []

        for pattern in self.end_marker_patterns.values():
            for node, _ in chapel.each_matching(ast, pattern.pattern):
                end_loc = location_to_range(node.location()).end
                header_loc = pattern.header_location(node)
                goto_loc = pattern.goto_location(node)

                if header_loc is None:
                    continue
                # skip blocks that are smaller than the threshold
                block_size = end_loc.line - header_loc.end()[0]
                if block_size < self.end_marker_threshold:
                    continue
                # skip blocks where other text already exists
                curly_line = file_lines[end_loc.line].rstrip()
                assert len(curly_line) > 0
                if curly_line[-1] != "}":
                    continue

                text = chapel.range_to_lines(header_loc, file_lines)
                text = " ".join([t.strip() for t in text])
                loc = location_to_location(goto_loc) if goto_loc else None
                to_insert = " // " + text
                edit = TextEdit(
                    Range(end_loc, Position(end_loc.line, len(to_insert))),
                    to_insert,
                )
                inlay = InlayHint(
                    position=end_loc,
                    padding_left=True,
                    label=[InlayHintLabelPart(text, location=loc)],
                    text_edits=[edit],
                )
                inlays.append(inlay)
        return inlays


def run_lsp():
    """
    Start a language server on the standard input/output
    """
    config = CLSConfig()
    config.parse_args()
    server = ChapelLanguageServer(config)

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
        ls.lsp.send_request_async(WORKSPACE_INLAY_HINT_REFRESH)
        ls.lsp.send_request_async(WORKSPACE_SEMANTIC_TOKENS_REFRESH)

    @server.feature(TEXT_DOCUMENT_DECLARATION)
    @server.feature(TEXT_DOCUMENT_DEFINITION)
    async def get_def(
        ls: ChapelLanguageServer,
        params: Union[DefinitionParams, DeclarationParams],
    ):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)
        segment = fi.get_use_or_def_segment_at_position(params.position)
        if segment:
            return segment.get_location()
        return None

    @server.feature(TEXT_DOCUMENT_REFERENCES)
    async def get_refs(ls: ChapelLanguageServer, params: ReferenceParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)

        node_and_loc = fi.get_use_or_def_segment_at_position(params.position)
        if not node_and_loc:
            return None

        ls.eagerly_process_all_files(fi.context)

        locations = [node_and_loc.get_location()]
        for uselist in fi.context.global_uses[node_and_loc.node.unique_id()]:
            for use in uselist:
                locations.append(use.get_location())

        return locations

    @server.feature(TEXT_DOCUMENT_TYPE_DEFINITION)
    async def get_type_def(
        ls: ChapelLanguageServer, params: TypeDefinitionParams
    ):
        if not ls.use_resolver:
            return None

        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)

        node_and_loc = fi.get_use_or_def_segment_at_position(params.position)
        if not node_and_loc:
            return None

        qt = node_and_loc.node.type()
        if qt is None:
            return None

        _, type_, _ = qt
        if not isinstance(type_, chapel.CompositeType):
            return None

        decl = type_.decl()
        if not decl:
            return None

        loc = (
            decl.name_location()
            if isinstance(decl, chapel.NamedDecl)
            else decl.location()
        )
        return location_to_location(loc)

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
            si = get_symbol_information(node)
            if si:
                syms.append(si)

        return syms

    @server.feature(TEXT_DOCUMENT_HOVER)
    async def hover(ls: ChapelLanguageServer, params: HoverParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)
        segment = fi.get_use_or_def_segment_at_position(params.position)
        instantiation = fi.get_inst_segment_at_position(params.position)
        if not segment:
            return None
        node_fi, _ = ls.get_file_info(segment.get_uri())

        text = ls.get_tooltip(segment.node, node_fi.siblings, instantiation)
        content = MarkupContent(MarkupKind.Markdown, text)
        return Hover(content, range=segment.get_location().range)

    @server.feature(TEXT_DOCUMENT_COMPLETION, CompletionOptions())
    async def complete(ls: ChapelLanguageServer, params: CompletionParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)

        items = []
        for name, decl in fi.visible_decls:
            if isinstance(decl, chapel.NamedDecl):
                items.append(completion_item_for_decl(decl, override_name=name))

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
                msg = (
                    f"Resolve Deprecation: replace {original} with {to_replace}"
                )
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

        # get lint fixits if applicable
        if ls.lint_driver and chplcheck:
            lint_actions = chplcheck.lsp.get_lint_actions(
                params.context.diagnostics
            )
            actions.extend(lint_actions)

        return actions

    @server.feature(TEXT_DOCUMENT_RENAME)
    async def rename(ls: ChapelLanguageServer, params: RenameParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)
        fi, _ = ls.get_file_info(text_doc.uri)

        node_and_loc = fi.get_use_or_def_segment_at_position(params.position)
        if not node_and_loc:
            return None

        edits: Dict[str, List[TextEdit]] = {}

        def add_to_edits(nr: NodeAndRange):
            if nr.get_uri() not in edits:
                edits[nr.get_uri()] = []
            edits[nr.get_uri()].append(TextEdit(nr.rng, params.new_name))

        ls.eagerly_process_all_files(fi.context)

        add_to_edits(node_and_loc)
        for uselist in fi.context.global_uses[node_and_loc.node.unique_id()]:
            for use in uselist:
                add_to_edits(use)

        return WorkspaceEdit(changes=edits)

    @server.feature(TEXT_DOCUMENT_INLAY_HINT)
    async def inlay_hint(ls: ChapelLanguageServer, params: InlayHintParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)
        fi, _ = ls.get_file_info(text_doc.uri)
        ast = fi.get_asts()
        inlays: List[InlayHint] = []

        file_lines = fi.file_lines()
        block_inlays = ls.get_end_markers(ast, file_lines)
        if len(block_inlays) > 0:
            inlays.extend(block_inlays)

        # The get_decl_inlays and get_call_inlays methods also check
        # and return early if the resolver is not being used, but for
        # the time being all hints are resolver-based, so we may
        # as well save ourselves the work of finding declarations and
        # calls to feed to those methods.
        if not ls.use_resolver:
            return inlays

        decls = fi.def_segments.range(params.range)
        calls = list(
            call for call, _ in chapel.each_matching(ast, chapel.core.FnCall)
        )

        with fi.context.context.track_errors() as _:
            for decl in decls:
                instantiation = fi.get_inst_segment_at_position(decl.rng.start)
                inlays.extend(ls.get_decl_inlays(decl, instantiation))

            for call in calls:
                instantiation = fi.get_inst_segment_at_position(
                    location_to_range(call.location()).start
                )
                inlays.extend(ls.get_call_inlays(call, instantiation))

        return inlays

    @server.feature(TEXT_DOCUMENT_DOCUMENT_HIGHLIGHT)
    async def document_highlight(
        ls: ChapelLanguageServer, params: DocumentHighlightParams
    ):
        text_doc_uri = params.text_document.uri
        text_doc = ls.workspace.get_text_document(text_doc_uri)

        fi, _ = ls.get_file_info(text_doc.uri)

        node_and_loc = fi.get_use_or_def_segment_at_position(params.position)
        if not node_and_loc:
            return None

        # todo: it would be nice if this differentiated between read and write
        highlights = []
        # only highlight the declaration if it is in the current document
        if node_and_loc.get_uri() == text_doc_uri:
            dh = DocumentHighlight(node_and_loc.rng, DocumentHighlightKind.Text)
            highlights.append(dh)
        uses = fi.uses_here.get(node_and_loc.node.unique_id(), [])
        highlights += [
            DocumentHighlight(use.rng, DocumentHighlightKind.Text)
            for use in uses
        ]

        return highlights

    @server.feature(TEXT_DOCUMENT_CODE_LENS)
    async def code_lens(ls: ChapelLanguageServer, params: CodeLensParams):

        # return early if the resolver is not being used or the feature is disabled
        if not ls.use_resolver:
            return None

        if not ls.show_instantiations:
            return None

        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)

        actions = []
        decls = fi.def_segments.elts
        for decl in decls:
            if (
                isinstance(decl.node, chapel.Function)
                and decl.node.unique_id() in fi.instantiations
            ):
                insts = fi.instantiations[decl.node.unique_id()]
                actions_per_decl = []
                for i, inst in enumerate(insts):
                    # Skip over "concrete" instantiations. They're in
                    # the list to track calls to concrete functions,
                    # but they don't have any type substitutions, so there's
                    # nothing to show.
                    if not inst.is_instantiation():
                        continue

                    action = CodeLens(
                        data=(decl.node.unique_id(), i),
                        command=Command(
                            "Show instantiation",
                            "chpl-language-server/showInstantiation",
                            [
                                params.text_document.uri,
                                decl.node.unique_id(),
                                i,
                            ],
                        ),
                        range=decl.rng,
                    )
                    actions_per_decl.append(action)

                if len(actions_per_decl) > 0:
                    action = CodeLens(
                        data=decl.node.unique_id(),
                        command=Command(
                            "Show Generic",
                            "chpl-language-server/showGeneric",
                            [
                                params.text_document.uri,
                                decl.node.unique_id(),
                            ],
                        ),
                        range=decl.rng,
                    )
                    actions.append(action)
                    actions.extend(actions_per_decl)

        return actions

    @server.command("chpl-language-server/showInstantiation")
    async def show_instantiation(
        ls: ChapelLanguageServer, data: Tuple[str, str, int]
    ):
        uri, unique_id, i = data

        fi, _ = ls.get_file_info(uri)
        decl = next(
            (
                decl
                for decl in fi.def_segments.elts
                if decl.node.unique_id() == unique_id
            ),
            None,
        )

        if decl is None:
            return

        node = decl.node
        if not isinstance(node, chapel.Function):
            return

        inst = fi.instantiation_at_index(node, i)
        fi.instantiation_segments.overwrite(
            (NodeAndRange.for_entire_node(decl.node), inst)
        )

        ls.lsp.send_request_async(WORKSPACE_INLAY_HINT_REFRESH)
        ls.lsp.send_request_async(WORKSPACE_SEMANTIC_TOKENS_REFRESH)

    @server.command("chpl-language-server/showGeneric")
    async def show_generic(ls: ChapelLanguageServer, data: Tuple[str, str]):
        uri, unique_id = data

        fi, _ = ls.get_file_info(uri)
        fi.instantiation_segments.remove_if(
            lambda x: x[0].node.unique_id() == unique_id
        )

        ls.lsp.send_request_async(WORKSPACE_INLAY_HINT_REFRESH)
        ls.lsp.send_request_async(WORKSPACE_SEMANTIC_TOKENS_REFRESH)

    @server.feature(
        TEXT_DOCUMENT_SEMANTIC_TOKENS_FULL,
        options=SemanticTokensLegend(
            token_types=[SemanticTokenTypes.Comment], token_modifiers=[]
        ),
    )
    async def semantic_tokens_range(
        ls: ChapelLanguageServer, params: SemanticTokensParams
    ):
        if not ls.use_resolver:
            return None

        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)

        tokens = []

        for ast in chapel.postorder(fi.get_asts()):
            if isinstance(ast, chapel.core.Conditional):
                start_pos = location_to_range(ast.location()).start
                instantiation = fi.get_inst_segment_at_position(start_pos)
                tokens.extend(
                    ls.get_dead_code_tokens(ast, fi.file_lines(), instantiation)
                )

        # sort tokens by line number, and then by column number
        tokens.sort(key=lambda x: (x[0], x[1]))
        return SemanticTokens(data=encode_deltas(tokens, 0, 0))

    @server.feature(TEXT_DOCUMENT_PREPARE_CALL_HIERARCHY)
    async def prepare_call_hierarchy(
        ls: ChapelLanguageServer, params: CallHierarchyPrepareParams
    ):
        if not ls.use_resolver:
            return None

        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)

        # Get function from a particular call under the cursor.
        sigs: List[chapel.TypedSignature] = []
        called_sig = fi.called_function_at_position(params.position)
        if called_sig is not None:
            sigs.append(called_sig)

        # To also handle the case where the user asked for a call hiearrchy
        # for a function declaration, see if there's a function under the
        # cursor.
        decl = fi.get_def_segment_at_position(params.position)
        node = decl.node if decl else None
        if isinstance(node, chapel.Function):
            # Get all the signatures we've found so far, unless the user
            # has already clicked "show instantiation" on this function.
            uid = node.unique_id()

            instantiation = fi.get_inst_segment_at_position(params.position)
            if instantiation and instantiation.ast().unique_id() == uid:
                sigs.append(instantiation)
            elif uid in fi.instantiations:
                sigs.extend(fi.instantiations[uid].keys())

        # Oddly, returning multiple here makes for no child nodes in the VSCode
        # UI. Just take one signature for now.
        return next(([ls.fn_to_call_hierarchy_item(sig, fi.context)] for sig in sigs), [])

    @server.feature(CALL_HIERARCHY_INCOMING_CALLS)
    async def call_hierarchy_incoming(
        ls: ChapelLanguageServer, params: CallHierarchyIncomingCallsParams
    ):
        if not ls.use_resolver:
            return None

        unpacked = ls.unpack_call_hierarchy_item(params.item)
        if unpacked is None:
            return None

        fi, fn, instantiation = unpacked

        # If there are no signatures that we found, there are no calls that
        # we are aware of.
        if instantiation is None:
            return []

        # TODO:
        # Here too, because there's no chapel-py way to convert an ID back
        # to a node, note the node whose ID we use in a dictionary (hack_id_to_node)
        # to look up later.
        calls = fi.instantiations[fn.unique_id()][instantiation]
        hack_id_to_node: Dict[str, chapel.NamedDecl] = {}
        incoming_calls: Dict[
            Union[chapel.TypedSignature, str], List[chapel.FnCall]
        ] = defaultdict(list)
        for call, via in calls:
            # If the call is from an instantiation, use the instantiation
            # as the hierarchy item anchor.
            if via is not None:
                incoming_calls[via].append(call)
            # Otherwise, the call is from a concrete function or something else,
            # like a module. Use the parent symbol's location as anchor.
            else:
                parent_sym: chapel.NamedDecl = call.parent_symbol()
                hack_id_to_node[parent_sym.unique_id()] = parent_sym
                incoming_calls[parent_sym.unique_id()].append(call)

        to_return = []
        for called_fn, calls in incoming_calls.items():
            if isinstance(called_fn, str):
                item = ls.sym_to_call_hierarchy_item(hack_id_to_node[called_fn])
            else:
                item = ls.fn_to_call_hierarchy_item(called_fn, fi.context)

            to_return.append(
                CallHierarchyIncomingCall(
                    item,
                    from_ranges=[
                        location_to_range(call.location()) for call in calls
                    ],
                )
            )

        return to_return

    @server.feature(CALL_HIERARCHY_OUTGOING_CALLS)
    async def call_hierarchy_outgoing(
        ls: ChapelLanguageServer, params: CallHierarchyOutgoingCallsParams
    ):
        if not ls.use_resolver:
            return None

        unpacked = ls.unpack_call_hierarchy_item(params.item)
        if unpacked is None:
            return None

        fi, fn, instantiation = unpacked

        outgoing_calls: Dict[chapel.TypedSignature, List[chapel.FnCall]] = (
            defaultdict(list)
        )
        for call, _ in chapel.each_matching(fn, chapel.FnCall):
            rr = (
                call.resolve_via(instantiation)
                if instantiation
                else call.resolve()
            )

            if rr is None:
                continue

            msc = rr.most_specific_candidate()
            if msc is None:
                continue

            outgoing_calls[msc.function()].append(call)

        to_return = []
        for called_fn, calls in outgoing_calls.items():
            item = ls.fn_to_call_hierarchy_item(called_fn, fi.context)
            to_return.append(
                CallHierarchyOutgoingCall(
                    item,
                    from_ranges=[
                        location_to_range(call.location()) for call in calls
                    ],
                )
            )

        return to_return

    server.start_io()


def main():
    run_lsp()


if __name__ == "__main__":
    main()
