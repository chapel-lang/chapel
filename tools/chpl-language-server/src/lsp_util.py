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
import sys
import importlib.util
import copy
import configargparse
import functools

import chapel
from chapel.lsp import location_to_range
from chapel.visitor import visitor, enter
from lsprotocol.types import (
    Location,
    MessageType,
    Range,
    Position,
)
from lsprotocol.types import (
    CompletionItem,
    CompletionItemKind,
)
from lsprotocol.types import (
    SymbolInformation,
    SymbolKind,
)


def log(*args, **kwargs):
    print(*args, **kwargs, file=sys.stderr)


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
                print("Error loading chplcheck: ", str(msg), file=sys.stderr)

        chpl_home = chapel.Context().get_chpl_home()
        if not chpl_home:
            error("CHPL_HOME not set")
            return None

        chplcheck_path = os.path.join(chpl_home, "tools", "chplcheck", "src")
        # Add chplcheck to the path, but load via importlib
        sys.path.insert(0, chplcheck_path)

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


_chplcheck = ChplcheckProxy.get()


def chplcheck():
    return _chplcheck


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
    elif isinstance(decl, chapel.VarLikeDecl):
        if decl.intent() == "type":
            return SymbolKind.TypeParameter
        elif decl.intent() == "param":
            return SymbolKind.Constant
        elif isinstance(decl, chapel.Variable) and decl.is_field():
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
    decl: chapel.NamedDecl,
    override_name: Optional[str] = None,
    override_sort: Optional[str] = None,
) -> Optional[CompletionItem]:
    kind = decl_kind(decl)
    if not kind:
        return None

    # We don't want to show operators in completion lists, as they're
    # not really useful to the user in this context.
    if kind == SymbolKind.Operator:
        return None

    name_to_use = override_name if override_name else decl.name()
    sort_text = override_sort if override_sort else name_to_use
    return CompletionItem(
        label=name_to_use,
        kind=decl_kind_to_completion_kind(kind),
        insert_text=name_to_use,
        sort_text=sort_text,
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
    """
    The function that retrieves the range of an element in the list.
    """

    elts: List[EltT] = field(default_factory=list)
    """
    A list of elements in the list, sorted by their start positions. Example
    list of items:

        |------------| A
               |-------------| B
                       |--| C
                                  |---------| D
    """

    segments: List[Tuple[Position, Optional[EltT], int]] = field(
        default_factory=list
    )
    """
    A flattened representation of the list of elements, where each element
    represents the beginning of a new item that continues until the next
    element in the list. The following segments are equivalent to the above
    list of items:

        |------|-------|--|--|----|---------|
         A      B       C  B  None D

    Note that 'B' occurs twice, and there's a 'None' in the middle. The
    'None' serves to clear the segment that was started by 'B'.

    This representation makes it easy to find the exact item at a given
    position in logarithmic time.
    """

    def _elements_to_segments(
        self, elts: List[EltT], into: List[Tuple[Position, Optional[EltT], int]]
    ):
        # A list of not-yet-closed segments, sorted descending by their end positions
        # (so that we can pop the last one to close it).
        ongoing: List[Tuple[Position, EltT, int]] = []

        # To be able to insert ongoing segments in descending order.
        def get_negated_pos(pos: Position):
            return (-pos.line, -pos.character)

        def push_segment(pos: Position, elt: Optional[EltT], idx: int):
            # Don't create duplicate segments for the same position.
            while len(into) > 0 and into[-1][0] == pos:
                into.pop()
            into.append((pos, elt, idx))

        # Close any ongoing segments that we need to close.
        #
        # When we close the segment, we switch to the one underneath.
        def push_segment_from_ongoing(pos: Position):
            # If there's a segment underneath, restart it.
            if len(ongoing) > 0:
                push_segment(pos, ongoing[-1][1], ongoing[-1][2])
            # No segment underneath; just clear the current one.
            else:
                push_segment(pos, None, -1)

        for idx, elt in enumerate(elts):
            rng = self.get_range(elt)

            # Close segments that end before this element starts.
            while len(ongoing) > 0 and ongoing[-1][0] <= rng.start:
                pos, _, _ = ongoing.pop()

                # We maintain the invariant that no ongoing segments end
                # in the same place, so the segment underneath at the top after
                # popping is the one we want to continue.
                push_segment_from_ongoing(pos)

            # Start a new segment for this element.
            push_segment(rng.start, elt, idx)

            # Remove all segments from 'ongoing' that end before this element.
            ongoing = [x for x in ongoing if x[0] > rng.end]

            # Add this element to 'ongoing' so that we can close or continue it later.
            idx = bisect_right(
                ongoing,
                get_negated_pos(rng.end),
                key=lambda x: get_negated_pos(x[0]),
            )
            ongoing.insert(idx, (rng.end, elt, idx))

        # Close all remaining segments.
        while len(ongoing) > 0:
            pos, _, _ = ongoing.pop()
            push_segment_from_ongoing(pos)

    def _rebuild_segments(self):
        self.segments.clear()
        self._elements_to_segments(self.elts, self.segments)

    def sort(self):
        """
        Re-ensure this segment list has its invariants upheld, by sorting
        the list of items and re-building the segments.
        """
        self.elts.sort(key=lambda x: self.get_range(x).start)
        self._rebuild_segments()

    def append(self, elt: EltT):
        self.elts.append(elt)

    def _get_elt_range(self, rng: Range):
        start = bisect_left(
            self.elts, rng.start, key=lambda x: self.get_range(x).start
        )
        end = bisect_right(
            self.elts, rng.end, key=lambda x: self.get_range(x).start
        )
        return (start, end)

    def _get_segment_range(self, rng: Range):
        start = bisect_left(self.segments, rng.start, key=lambda x: x[0])
        end = bisect_left(self.segments, rng.end, key=lambda x: x[0])
        return (start, end)

    def _update_segments(
        self, rng: Range, new_segments: List[Tuple[Position, EltT, int]]
    ):
        new_segments = [
            seg for seg in new_segments if rng.start <= seg[0] < rng.end
        ]

        seg_start, seg_end = self._get_segment_range(rng)
        if seg_end > 0:
            after_value, after_idx = self.segments[seg_end - 1][1:]
        else:
            after_value, after_idx = None, -1

        to_insert = []

        # If the segments start halfway through the range, insert a new segment,
        # ensure that between rng.start and the start of the first segment, there
        # is a 'None' segment to clear the preceding segment.
        if len(new_segments) == 0 or new_segments[0][0] > rng.start:
            to_insert.append((rng.start, None, -1))

        # Insert the new segments.
        to_insert.extend(new_segments)

        # Resume whatever was continuing after the range, unless the next
        # segment starts right after the range.
        if seg_end >= len(self.segments) or self.segments[seg_end][0] > rng.end:
            to_insert.append((rng.end, after_value, after_idx))

        self.segments[seg_start:seg_end] = to_insert

    def clear_range(self, rng: Range):
        elt_start, elt_end = self._get_elt_range(rng)
        self.elts[elt_start:elt_end] = []

        self._update_segments(rng, [])

    def _set_range(self, rng: Range, elts: List[EltT]):
        start, end = self._get_elt_range(rng)
        self.elts[start:end] = elts

        elt_segs = []
        self._elements_to_segments(elts, elt_segs)
        self._update_segments(rng, elt_segs)

    def overwrite(self, elt: EltT):
        self._set_range(self.get_range(elt), [elt])

    def overwrite_range(self, rng: Range, other: "PositionList[EltT]"):
        other_start, other_end = other._get_elt_range(rng)
        self._set_range(rng, other.elts[other_start:other_end])

    def clear(self):
        self.elts.clear()
        self.segments.clear()

    def find(self, pos: Position) -> Optional[EltT]:
        idx = bisect_left(self.segments, pos, key=lambda x: x[0])

        if idx >= 1 and self.segments[idx - 1][1] is not None:
            return self.segments[idx - 1][1]

        # In some cases, we may be on the boundary between two segments.
        # In this case, the next segment's start position is the same as
        # the current position, and we should return the next segment.
        if idx < len(self.segments) and self.segments[idx][0] == pos:
            return self.segments[idx][1]

        return None

    def range(self, rng: Range) -> List[EltT]:
        start, end = self._get_segment_range(rng)
        return [x[1] for x in self.segments[start:end] if x[1] is not None]


@dataclass
class NodeAndRange:
    node: chapel.AstNode
    rng: Range = field(init=False)

    def __post_init__(self):
        if isinstance(self.node, chapel.Dot):
            self.rng = location_to_range(self.node.field_location())
        elif isinstance(self.node, chapel.NamedDecl):
            self.rng = location_to_range(self.node.name_location())
        elif isinstance(self.node, chapel.Include):
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
class ScopedNodeAndRange:
    node: chapel.AstNode
    scopes: List[chapel.Scope] = field(default_factory=list)

    @staticmethod
    def create(node: chapel.AstNode) -> Optional["ScopedNodeAndRange"]:
        scopes = []
        scope = node.scope()
        while scope:
            scopes.append(scope)
            scope = scope.parent_scope()
        if len(scopes) == 0:
            return None
        return ScopedNodeAndRange(node, scopes)

    @property
    def rng(self):
        return location_to_range(self.node.location())


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
    def __init__(
        self,
        file: str,
        cls_config: "CLSConfig",
        config: Optional["WorkspaceConfig"],
    ):
        self.cls_config: CLSConfig = cls_config
        self.config: Optional["WorkspaceConfig"] = config
        self.file_paths: List[str] = []
        self.std_module_root: str = ""
        self.module_paths: List[str] = [os.path.dirname(os.path.abspath(file))]
        self.context: chapel.Context = chapel.Context()
        self.file_infos: List["FileInfo"] = []
        self.global_uses: Dict[chapel.AstNode, List[References]] = defaultdict(
            list
        )
        self.instantiation_ids: Dict[chapel.TypedSignature, str] = {}
        self.instantiation_id_counter = 0

        if config:
            file_config = config.for_file(file)
            if file_config:
                self.module_paths = file_config["module_dirs"]
                self.file_paths = file_config["files"]

        self.std_module_root = self.cls_config.get("std_module_root")
        self.module_paths.extend(self.cls_config.get("module_dir"))

        self.context._set_module_paths(
            self.std_module_root, self.module_paths, self.file_paths
        )

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
        self.context._set_module_paths(
            self.std_module_root, self.module_paths, self.file_paths
        )

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
    scope_segments: PositionList[ScopedNodeAndRange] = field(init=False)
    call_segments: PositionList[ResolvedPair] = field(init=False)
    instantiation_segments: PositionList[
        Tuple[NodeAndRange, chapel.TypedSignature]
    ] = field(init=False)
    uses_here: Dict[chapel.AstNode, References] = field(init=False)
    instantiations: Dict[
        str,
        Dict[chapel.TypedSignature, CallsInTypeContext],
    ] = field(init=False)
    siblings: chapel.SiblingMap = field(init=False)

    def __post_init__(self):
        self.use_segments = PositionList(lambda x: x.ident.rng)
        self.def_segments = PositionList(lambda x: x.rng)
        self.scope_segments = PositionList(lambda x: x.rng)
        self.call_segments = PositionList(lambda x: x.ident.rng)
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

    def _get_use_container(self, node: chapel.AstNode) -> References:
        if node in self.uses_here:
            return self.uses_here[node]

        refs = References(self, [])
        self.uses_here[node] = refs
        self.context.global_uses[node].append(refs)
        return refs

    def _note_reference(
        self, node: Union[chapel.Dot, chapel.Identifier, chapel.Include]
    ):
        """
        Given a node that can refer to another node, note what it refers
        to in by updating the 'use' segment table and the list of uses.
        """
        to = node.to_node()
        if not to:
            return

        self._get_use_container(to).append(NodeAndRange(node))
        self.use_segments.append(
            ResolvedPair(NodeAndRange(node), NodeAndRange(to))
        )

    def _note_scope(self, node: chapel.AstNode):
        if not node.creates_scope():
            return
        s = ScopedNodeAndRange.create(node)
        if not s:
            return
        self.scope_segments.append(s)

    def _resolve_call(
        self,
        node: chapel.FnCall,
        via: Optional[chapel.TypedSignature],
    ) -> Optional[Tuple[chapel.Function, chapel.TypedSignature]]:
        """
        Given a function call node, note the call in the call segment table,
        and return the function and signature it refers to.
        """

        rr = node.resolve_via(via) if via else node.resolve()
        if not rr:
            return None

        candidate = rr.most_specific_candidate()
        if not candidate:
            return None

        sig = candidate.function()
        fn = sig.ast()
        if not fn or not isinstance(fn, chapel.Function):
            return None

        return (fn, sig)

    @enter
    def _enter_AstNode(self, node: chapel.AstNode):
        self._note_scope(node)

    @enter
    def _enter_Identifier(self, node: chapel.Identifier):
        self._note_reference(node)

    @enter
    def _enter_Dot(self, node: chapel.Dot):
        self._note_reference(node)

    @enter
    def _enter_Include(self, node: chapel.Include):
        self._note_reference(node)

    @enter
    def _enter_Module(self, node: chapel.Module):
        # Trigger scope resolution to error duplicate variable warnings.
        _ = node.scope_resolve()

        self.def_segments.append(NodeAndRange(node))
        self._note_scope(node)

    @enter
    def _enter_Function(self, node: chapel.Function):
        # Trigger scope resolution to error duplicate variable warnings.
        _ = node.scope_resolve()

        self.def_segments.append(NodeAndRange(node))
        self._note_scope(node)

    @enter
    def _enter_Formal(self, node: chapel.Formal):
        # do not enter the `this` formal, unless it has an explicit type
        # i.e. (it is a secondary method)
        if not (node.is_this() and node.type_expression() is None):
            self.def_segments.append(NodeAndRange(node))
            self._note_scope(node)

    @enter
    def _enter_NamedDecl(self, node: chapel.NamedDecl):
        self.def_segments.append(NodeAndRange(node))
        self._note_scope(node)

    def get_visible_nodes(
        self, pos: Position
    ) -> List[Tuple[str, chapel.AstNode, int]]:
        """
        Returns the visible nodes at a given position.
        """

        def visible_nodes_for_scope(
            name: str, nodes: List[chapel.AstNode], in_bundled_module: bool
        ) -> Optional[Tuple[str, chapel.AstNode]]:
            """
            Narrow the list of visible nodes to those that are actually visible

            The heuristic here is to avoid showing internal symbols to the user,
            i.e. those that start with 'chpl_' or '_'. We also avoid showing nodes
            with the @chpldoc.nodoc attribute.
            """
            # Don't show internal symbols to the user, even if they
            # are technically in scope. The exception is if we're currently
            # editing a standard file.
            skip_prefixes = ["chpl_", "chpldev_", "_"]
            if any(name.startswith(prefix) for prefix in skip_prefixes):
                if not in_bundled_module:
                    return None

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
                return None

            # Just take the first value to avoid showing N entries for
            # overloaded functions.
            return name, documented_nodes[0]

        @functools.cache
        def files_named_in_use_or_import(scope: chapel.Scope) -> Set[str]:
            files = set()
            for m in scope.modules_named_in_use_or_import():
                files.add(m.location().path())
            return files

        def apply_depth_heuristic(
            scope: chapel.Scope,
            name: str,
            node: chapel.AstNode,
            original_depth: int,
            cur_file: str,
        ) -> Tuple[str, chapel.AstNode, int]:
            """
            Heuristic to provide results in a more useful order, since
            most clients will sort alphabetically. We can provide a
            depth that is used to sort the results, so that the most
            relevant results are shown first.
            """
            depth = original_depth
            vis_path = node.location().path()
            if vis_path != cur_file:
                # if from a different file, increase the depth by 1
                depth += 1
                # if from a bundled path increase the depth by 1
                depth += int(self.context.context.is_bundled_path(vis_path))
                # if not explicitly used, increase the depth by 1
                files_named_in_use = files_named_in_use_or_import(scope)
                depth += int(vis_path not in files_named_in_use)
            return (name, node, depth)

        def visible_nodes_for_scopes(
            node: chapel.AstNode, scopes: List[chapel.Scope]
        ):
            visible_nodes = []
            cur_file = node.location().path()
            in_bundled_module = self.context.context.is_bundled_path(cur_file)
            # for each scope of the node
            for depth, scope in enumerate(scopes):
                # for all of the visible nodes in the scope
                for name, nodes in scope.visible_nodes():
                    # narrow the list of visible nodes to those that are
                    # actually visible to the user (i.e. not nodoc/internal)
                    visible_node = visible_nodes_for_scope(
                        name, nodes, in_bundled_module
                    )
                    if visible_node is None:
                        continue
                    vn = apply_depth_heuristic(
                        scope, *visible_node, depth, cur_file
                    )
                    visible_nodes.append(vn)
            return visible_nodes

        visible_nodes = []
        segment = self.get_scope_segment_at_position(pos)

        if segment:
            vns = visible_nodes_for_scopes(segment.node, segment.scopes)
            visible_nodes.extend(vns)
        else:
            # no segment found, use the top level nodes
            for a in self.get_asts():
                if isinstance(a, chapel.Comment):
                    continue
                s = a.scope()
                if s:
                    visible_nodes.extend(visible_nodes_for_scopes(a, [s]))

        return visible_nodes

    def _search_instantiations(
        self,
        root: chapel.AstNode,
        via: Optional[chapel.TypedSignature] = None,
    ):
        for node, _ in chapel.each_matching(
            root, chapel.FnCall, iterator=chapel.preorder
        ):
            resolved = self._resolve_call(node, via)
            if not resolved:
                continue
            fn, sig = resolved

            # Only store the call in the segment table if this is a concrete
            # functions. There may be multiple instantiations per file,
            # and until the user has selected one, we shouldn't enable go-to-def
            # on calls within then.
            include_in_segments = via is None
            if include_in_segments:
                self.call_segments.append(
                    ResolvedPair(
                        NodeAndRange(node.called_expression()), NodeAndRange(fn)
                    )
                )

            # Even if we don't descend into it (and even if it's not an
            # instantiation), track the call that invoked this function.
            # This will help with call hierarchy.
            insts = self.instantiations[fn.unique_id()]
            already_visited = sig in insts
            insts[sig].append((node, via))

            if not sig.is_instantiation() or already_visited:
                continue

            self._search_instantiations(fn, via=sig)

    def find_decl_by_unique_id(self, unique_id: str) -> Optional[NodeAndRange]:
        """
        Traverse the (location-key'ed) definition segment table and
        find the definition segment with the given ID, or none.
        """
        return next(
            (
                decl
                for decl in self.def_segments.elts
                if decl.node.unique_id() == unique_id
            ),
            None,
        )

    def find_instantiation_by_unique_id(
        self, unique_id: str
    ) -> Optional[Tuple[NodeAndRange, chapel.TypedSignature]]:
        """
        Traverse the (location-key'ed) definition segment table and
        find the definition segment with the given ID, or none.
        """
        return next(
            (
                decl
                for decl in self.instantiation_segments.elts
                if decl[0].node.unique_id() == unique_id
            ),
            None,
        )

    def update_call_segments_from_instantiations(self, rng: Range):
        self.call_segments.clear_range(rng)
        start, end = self.instantiation_segments._get_segment_range(rng)

        segments_for_elt = {}

        def process_instantiation_segment(
            inst: NodeAndRange, sig: chapel.TypedSignature, elt_idx: int
        ):
            if elt_idx in segments_for_elt:
                return segments_for_elt[elt_idx]

            calls_in_inst = PositionList[ResolvedPair](lambda x: x.ident.rng)

            for node, _ in chapel.each_matching(
                inst.node, chapel.FnCall, iterator=chapel.preorder
            ):
                resolved = self._resolve_call(node, via=sig)
                if not resolved:
                    continue
                fn, sig = resolved

                new_call = NodeAndRange(node.called_expression())
                calls_in_inst.append(ResolvedPair(new_call, NodeAndRange(fn)))

            # Call segments are currently appended (not inserted); perform sort now.
            calls_in_inst.sort()
            segments_for_elt[elt_idx] = calls_in_inst
            return calls_in_inst

        for i in range(start, end):
            # Find the segment and where it starts
            begin_pos, value_at_segment, elt_idx = (
                self.instantiation_segments.segments[i]
            )
            if value_at_segment is None:
                continue

            # Find where the segment ends as far as the range-to-update is concerned
            end_pos = rng.end
            if i + 1 < len(self.instantiation_segments.segments):
                next_pos, _, _ = self.instantiation_segments.segments[i + 1]
                end_pos = min(end_pos, next_pos)

            # Figure out the calls in this instantiation. The same instantiation
            # can appear in multiple segments, since it could be interrupted
            # by a nested instantiation.
            calls_in_inst = process_instantiation_segment(
                *value_at_segment, elt_idx
            )
            self.call_segments.overwrite_range(
                Range(begin_pos, end_pos), calls_in_inst
            )

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
        self.scope_segments.clear()
        self.call_segments.clear()
        self.visit(asts)
        self.use_segments.sort()
        self.def_segments.sort()
        self.scope_segments.sort()
        # call segments via ._search_instantiations, so sort them there.

        self.siblings = chapel.SiblingMap(asts)

        if self.use_resolver:
            for ast in asts:
                self._search_instantiations(ast)
            self.call_segments.sort()

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

    def get_scope_segment_at_position(
        self, position: Position
    ) -> Optional[ScopedNodeAndRange]:
        """lookup a scope segment based upon a Position, likely a user mouse location"""
        return self.scope_segments.find(position)

    def get_call_segment_at_position(
        self, position: Position
    ) -> Optional[ResolvedPair]:
        """lookup a call segment based upon a Position, likely a user mouse location"""
        return self.call_segments.find(position)

    def get_inst_segment_at_position(
        self, position: Position
    ) -> Optional[chapel.TypedSignature]:
        """lookup a instantiation segment based upon a Position, likely a user mouse location"""
        segment = self.instantiation_segments.find(position)
        if segment:
            return segment[1]
        return None

    def get_target_segment_at_position(
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

        segment = self.get_call_segment_at_position(position)
        if segment:
            return segment.resolved_to

        segment = self.get_use_segment_at_position(position)
        if segment:
            return segment.resolved_to

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
            default_config_files=[
                os.path.join(os.getcwd(), "chpl-language-server.cfg"),
                os.path.join(os.getcwd(), ".chpl-language-server.cfg"),
                os.path.join(os.getcwd(), "Mason.toml"),
            ],
            config_file_parser_class=configargparse.CompositeConfigParser(
                [
                    configargparse.YAMLConfigFileParser,
                    configargparse.TomlConfigParser(
                        ["tool.chpl-language-server"]
                    ),
                ]
            ),
            args_for_setting_config_path=["--config", "-c"],
        )

        chplcheck().config.add_bool_flag(
            self.parser, "resolver", "resolver", False
        )
        self.parser.add_argument(
            "--std-module-root", default="", help=configargparse.SUPPRESS
        )
        self.parser.add_argument(
            "--module-dir", "-M", action="append", default=[]
        )
        chplcheck().config.add_bool_flag(
            self.parser, "type-inlays", "type_inlays", True
        )
        chplcheck().config.add_bool_flag(
            self.parser, "param-inlays", "param_inlays", True
        )
        chplcheck().config.add_bool_flag(
            self.parser, "literal-arg-inlays", "literal_arg_inlays", True
        )
        chplcheck().config.add_bool_flag(
            self.parser, "dead-code", "dead_code", True
        )
        chplcheck().config.add_bool_flag(
            self.parser, "evaluate-expressions", "eval_expressions", True
        )
        chplcheck().config.add_bool_flag(
            self.parser, "show-instantiations", "show_instantiations", True
        )
        self.parser.add_argument("--end-markers", default="none")
        self.parser.add_argument("--end-marker-threshold", type=int, default=10)

        chplcheck().config.add_bool_flag(
            self.parser, "chplcheck", "do_linting", False
        )
        if chplcheck:
            chplcheck().config.Config.add_arguments(self.parser, "chplcheck-")

    def _parse_end_markers(self):
        self.args["end_markers"] = [
            a.strip() for a in self.args["end_markers"].split(",")
        ]

    def _validate_end_markers(self):
        valid_choices = ["all", "none"] + list(EndMarkerPattern.all().keys())
        for marker in self.args["end_markers"]:
            if marker not in valid_choices:
                raise configargparse.ArgumentError(
                    None, f"Invalid end marker choice: {marker}"
                )
        n_markers = len(self.args["end_markers"])
        if n_markers != len(set(self.args["end_markers"])):
            raise configargparse.ArgumentError(
                None, "Cannot specify the same end marker multiple times"
            )
        if "none" in self.args["end_markers"] and n_markers > 1:
            raise configargparse.ArgumentError(
                None, "Cannot specify 'none' with other end marker choices"
            )
        if "all" in self.args["end_markers"] and n_markers > 1:
            raise configargparse.ArgumentError(
                None, "Cannot specify 'all' with other end marker choices"
            )

    def parse_args(self):
        self.args = copy.deepcopy(vars(self.parser.parse_args()))
        self._parse_end_markers()
        self._validate_end_markers()

    def get(self, key: str):
        return self.args[key]
