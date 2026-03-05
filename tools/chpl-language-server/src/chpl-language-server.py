#
# Copyright 2024-2026 Hewlett Packard Enterprise Development LP
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
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)
from collections import defaultdict
from symbol_signature import SymbolSignature
import os
import re

import chapel
from chapel.lsp import location_to_range, error_to_diagnostic
from pygls.server import LanguageServer
from pygls.workspace import TextDocument
from lsprotocol.types import (
    MessageType,
    Diagnostic,
    DiagnosticRelatedInformation,
    Range,
    Position,
)
from lsprotocol.types import TEXT_DOCUMENT_DID_OPEN, DidOpenTextDocumentParams
from lsprotocol.types import TEXT_DOCUMENT_DID_SAVE, DidSaveTextDocumentParams
from lsprotocol.types import TEXT_DOCUMENT_DID_CLOSE, DidCloseTextDocumentParams
from lsprotocol.types import TEXT_DOCUMENT_DEFINITION, DefinitionParams
from lsprotocol.types import TEXT_DOCUMENT_TYPE_DEFINITION, TypeDefinitionParams
from lsprotocol.types import TEXT_DOCUMENT_DECLARATION, DeclarationParams
from lsprotocol.types import TEXT_DOCUMENT_REFERENCES, ReferenceParams
from lsprotocol.types import (
    TEXT_DOCUMENT_DID_CHANGE,
    DidChangeTextDocumentParams,
    TextDocumentContentChangeEvent_Type1,
    TextDocumentContentChangeEvent_Type2,
)
from lsprotocol.types import (
    TEXT_DOCUMENT_COMPLETION,
    CompletionParams,
    CompletionOptions,
    CompletionList,
)
from lsprotocol.types import (
    TEXT_DOCUMENT_DOCUMENT_SYMBOL,
    DocumentSymbolParams,
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

from lsp_util import *


class ChapelLanguageServer(LanguageServer):
    def __init__(self, config: CLSConfig):
        super().__init__("chpl-language-server", "v0.1")

        self.config: CLSConfig = config
        self.contexts: Dict[str, ContextContainer] = {}
        self.context_ids: Dict[ContextContainer, str] = {}
        self.context_id_counter = 0
        self.file_infos: Dict[Tuple[str, Optional[str]], FileInfo] = {}
        self.configurations: Dict[str, WorkspaceConfig] = {}

        self.use_resolver: bool = config.get("resolver")
        self.type_inlays: bool = config.get("type_inlays")
        self.literal_arg_inlays: bool = config.get("literal_arg_inlays")
        self.param_inlays: bool = config.get("param_inlays")
        self.enum_inlays: bool = config.get("enum_inlays")
        self.dead_code: bool = config.get("dead_code")
        self.eval_expressions: bool = config.get("eval_expressions")
        self.show_instantiations: bool = config.get("show_instantiations")
        self.end_markers: List[str] = config.get("end_markers")
        self.end_marker_threshold: int = config.get("end_marker_threshold")
        self.end_marker_patterns = self._get_end_marker_patterns()
        self.do_linting: bool = config.get("do_linting")

        self.is_vscode: bool = False

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
        if not (self.do_linting and chplcheck()):
            return

        config = chplcheck().config.Config.from_args(clsConfig.args)
        self.lint_driver = chplcheck().driver.LintDriver(config)

        chplcheck().rules.rules(self.lint_driver)

        for p in config.add_rules:
            chplcheck().main.load_module(self.lint_driver, os.path.abspath(p))

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

        context = ContextContainer(path, self.config, workspace_config)
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
        self,
        uri: str,
        do_update: bool = False,
        context_id: Optional[str] = None,
    ) -> Tuple[FileInfo, List[Any]]:
        """
        The language server maintains a FileInfo object per file. The FileInfo
        contains precomputed information (binary-search-ready tables for
        finding an element under a cursor).

        This method retrieves the FileInfo object for a particular URI,
        creating one if it doesn't exist. If do_update is set to True,
        then the FileInfo's index is rebuilt even if it has already been
        computed. This is useful if the underlying file has changed.

        Most of the time, we will create a new context for a given URI. When
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
                assert context
            else:
                context = self.get_context(uri)

            file_info, errors = context.new_file_info(uri, self.use_resolver)
            self.file_infos[fi_key] = file_info

            # Also make this the "default" context for this file in case we
            # open it.
            if (uri, None) not in self.file_infos:
                self.file_infos[(uri, None)] = file_info

        # filter out errors that are not related to the file
        cur_path = uri[len("file://") :]
        errors = [e for e in errors if e.location().path() == cur_path]

        return (file_info, errors)

    def clear_file_info(self, uri: str):
        """
        Clear any cached FileInfo for a given URI

        Currently only handles the default context, contexts paired
        with a specific context ID are not cleared.
        """

        to_delete = (uri, None)
        path = uri[len("file://") :]
        container = self.contexts.get(path, None)
        if container and len(container.file_infos) == 1:
            del self.file_infos[to_delete]
            del self.contexts[path]

    def build_diagnostics(self, uri: str) -> List[Diagnostic]:
        """
        Parse a file at a particular URI, capture the errors, and return then
        as a list of LSP Diagnostics.
        """

        fi, errors = self.get_file_info(uri, do_update=True)

        diagnostics = []
        for e in errors:
            diag = error_to_diagnostic(e)
            if diag.related_information is None:
                diag.related_information = [
                    DiagnosticRelatedInformation(
                        location_to_location(note_loc), note_msg
                    )
                    for (note_loc, note_msg) in e.notes()
                ]
            diagnostics.append(diag)

        # get lint diagnostics if applicable
        if self.lint_driver and chplcheck():
            lint_diagnostics = chplcheck().lsp.get_lint_diagnostics(
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

    def _get_enum_inlays(self, decl: NodeAndRange) -> List[InlayHint]:
        if not self.enum_inlays:
            return []

        node = decl.node
        if not isinstance(node, chapel.EnumElement):
            return []

        enum_value = node.assigned_value()
        inlay_value: str | None = None
        if isinstance(enum_value, chapel.IntParam):
            inlay_value = str(enum_value.value())
        elif isinstance(enum_value, chapel.UintParam):
            inlay_value = str(enum_value.value())

        if not inlay_value:
            return []

        node_init = node.init_expression()
        edits = []
        if isinstance(node_init, chapel.Literal):
            return []
        elif isinstance(node_init, chapel.AstNode):
            position = location_to_range(node_init.location()).end
            value = " /* " + inlay_value + " */"
        else:
            position = location_to_range(node.location()).end
            value = " = " + inlay_value
            edits = [TextEdit(Range(position, position), value)]

        return [
            InlayHint(
                position=position,
                label=value,
                text_edits=edits,
            )
        ]

    def _get_param_inlays(
        self, decl: NodeAndRange, qt: chapel.QualifiedType
    ) -> List[InlayHint]:
        if not self.param_inlays:
            return []

        _, ty, param = qt
        if not param:
            return []

        val = str(param)
        val = val.replace("\n", "\\n").replace("\t", "\\t").replace("\r", "\\r")
        if isinstance(ty, chapel.CompositeType):
            if ty_decl := ty.decl():
                assert isinstance(ty_decl, chapel.NamedDecl)
                if ty_decl.name() == "_bytes":
                    val = "b" + val

        return [
            InlayHint(
                position=decl.rng.end,
                label="param value is " + val,
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
        if isinstance(decl.node, chapel.Formal) and decl.node.is_this():
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
            if index == decl.node:
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

        inlays = []
        inlays.extend(self._get_enum_inlays(decl))

        rr = decl.node.resolve_via(via) if via else decl.node.resolve()
        if not rr:
            return inlays

        qt = rr.type()
        if qt is None:
            return inlays

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

        # If the call is a method call (x.foo(a, b, c)), then by the time
        # we see 'a', we have already skipped over the 'this' actual 'x', either
        # because it was explicitly specified or because it was inserted.
        # So, treat that as an extra actual. But, there's no good way to show
        # inlay hints for it anyway, so keep it None and it'll be skipped,
        # only affecting the index in enumerate.
        actuals = call.actuals()
        if fn.is_method():
            actuals = [None] + list(actuals)

        inlays = []

        for i, act in zip(msc.formal_actual_mapping(), actuals):
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

        inst_id = None
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
            or not isinstance(item.data[1], (str, None))
            or not isinstance(item.data[2], (str, None))
        ):
            self.show_message(
                "Call hierarchy item contains missing or invalid additional data",
                MessageType.Error,
            )
            return None
        uid, inst_id, ctx = item.data

        fi, _ = self.get_file_info(item.uri, context_id=ctx)

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

        instantiation = fi.context.retrieve_signature(inst_id)

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
        self,
        ast: List[chapel.AstNode],
        file_lines: List[str],
        requested_range: Range,
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

                end_range = Range(end_loc, end_loc)
                if not range_overlap(end_range, requested_range):
                    continue

                header_loc = pattern.header_location(node)
                goto_loc = pattern.goto_location(node)

                if header_loc is None:
                    continue

                # skip blocks that are smaller than the threshold
                block_size = end_loc.line - header_loc.end()[0]
                if block_size < self.end_marker_threshold:
                    continue
                # skip blocks where other text already exists
                # skip empty lines, this only occurs in edge cases (like having
                # standard modules from different branches open at the same
                # time) that should be skipped
                curly_line = file_lines[end_loc.line].rstrip()
                if len(curly_line) == 0:
                    continue
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
        if params.workspace_folders is not None:
            for ws in params.workspace_folders:
                ls.register_workspace(ws.uri)

        if params.client_info is not None:
            name = params.client_info.name
            ls.is_vscode = name == "Visual Studio Code"

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

        # This file is loaded from disk, no changes since save, so reset
        # earliest changed position.
        if isinstance(params, DidSaveTextDocumentParams):
            fi, _ = ls.get_file_info(text_doc.uri)
            fi.earliest_changed_pos = None

        # note: this also recomputes uses, definitions, etc.
        diag = ls.build_diagnostics(text_doc.uri)

        ls.publish_diagnostics(text_doc.uri, diag)
        ls.lsp.send_request_async(WORKSPACE_INLAY_HINT_REFRESH)
        ls.lsp.send_request_async(WORKSPACE_SEMANTIC_TOKENS_REFRESH)

    @server.feature(TEXT_DOCUMENT_DID_CLOSE)
    async def did_close(
        ls: ChapelLanguageServer, params: DidCloseTextDocumentParams
    ):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)
        ls.clear_file_info(text_doc.uri)
        ls.publish_diagnostics(text_doc.uri, [])

    @server.feature(TEXT_DOCUMENT_DECLARATION)
    @server.feature(TEXT_DOCUMENT_DEFINITION)
    async def get_def(
        ls: ChapelLanguageServer,
        params: Union[DefinitionParams, DeclarationParams],
    ):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)
        segment = fi.get_target_segment_at_position(params.position)
        if segment:
            return segment.get_location()
        return None

    @server.feature(TEXT_DOCUMENT_DID_CHANGE)
    async def get_changed(
        ls: ChapelLanguageServer, params: DidChangeTextDocumentParams
    ):
        new_least_pos: Optional[Position] = None
        for change in params.content_changes:
            # If the whole document changed, no more iteration.
            if isinstance(change, TextDocumentContentChangeEvent_Type2):
                new_least_pos = Position(0, 0)
                break

            new_least_pos = min_pos(change.range.start, new_least_pos)

        if not new_least_pos:
            return

        fi, _ = ls.get_file_info(params.text_document.uri)
        fi.earliest_changed_pos = min_pos(
            new_least_pos,
            fi.earliest_changed_pos,
        )
        ls.lsp.send_request_async(WORKSPACE_INLAY_HINT_REFRESH)
        ls.lsp.send_request_async(WORKSPACE_SEMANTIC_TOKENS_REFRESH)

    @server.feature(TEXT_DOCUMENT_REFERENCES)
    async def get_refs(ls: ChapelLanguageServer, params: ReferenceParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)

        node_and_loc = fi.get_target_segment_at_position(params.position)
        if not node_and_loc:
            return None

        ls.eagerly_process_all_files(fi.context)

        locations = [node_and_loc.get_location()]
        for uselist in fi.context.global_uses[node_and_loc.node]:
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

        node_and_loc = fi.get_target_segment_at_position(params.position)
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
        segment = fi.get_target_segment_at_position(params.position)
        instantiation = fi.get_inst_segment_at_position(params.position)
        if not segment:
            return None
        node_fi, _ = ls.get_file_info(segment.get_uri())

        text = ls.get_tooltip(segment.node, node_fi.siblings, instantiation)
        content = MarkupContent(MarkupKind.Markdown, text)
        return Hover(content, range=segment.get_location().range)

    # TODO: can we make use of 'trigger_character' to provide completions?
    # since we can't parse 'foo.', can we use the presence of a trigger '.' to
    # read a identifier from the file buffer, lookup the scope for that name,
    # and provide completions based on that scope?
    @server.feature(TEXT_DOCUMENT_COMPLETION, CompletionOptions())
    async def complete(ls: ChapelLanguageServer, params: CompletionParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)

        names = set()
        items = []
        for name, node, depth in fi.get_visible_nodes(params.position):
            if not isinstance(node, chapel.NamedDecl):
                continue
            # if name is already suggested, skip it
            if name in names:
                continue
            # use the depth to sort the suggestions, lower depths first
            sort_name = f"{depth:03d}{name}"
            item = completion_item_for_decl(
                node, override_name=name, override_sort=sort_name
            )
            if item:
                items.append(item)
                names.add(name)

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
        if ls.lint_driver and chplcheck():
            lint_actions = chplcheck().lsp.get_lint_actions(
                params.context.diagnostics
            )
            actions.extend(lint_actions)

        return actions

    @server.feature(TEXT_DOCUMENT_RENAME)
    async def rename(ls: ChapelLanguageServer, params: RenameParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)
        fi, _ = ls.get_file_info(text_doc.uri)

        node_and_loc = fi.get_target_segment_at_position(params.position)
        if not node_and_loc:
            return None

        edits: Dict[str, List[TextEdit]] = {}

        def add_to_edits(nr: NodeAndRange):
            if nr.get_uri() not in edits:
                edits[nr.get_uri()] = []
            edits[nr.get_uri()].append(TextEdit(nr.rng, params.new_name))

        ls.eagerly_process_all_files(fi.context)

        add_to_edits(node_and_loc)
        for uselist in fi.context.global_uses[node_and_loc.node]:
            for use in uselist:
                add_to_edits(use)

        return WorkspaceEdit(changes=edits)

    @server.feature(TEXT_DOCUMENT_INLAY_HINT)
    async def inlay_hint(ls: ChapelLanguageServer, params: InlayHintParams):
        text_doc = ls.workspace.get_text_document(params.text_document.uri)
        fi, _ = ls.get_file_info(text_doc.uri)
        ast = fi.get_asts()
        inlays: List[InlayHint] = []

        # Dyno computes inlays etc. from the file on disk. We have a
        # possible edited file in the buffer. Inlays after the earliest
        # changed position may be invalid, so we limit the range.
        requested_range = params.range
        if fi.earliest_changed_pos is not None:
            requested_range = Range(
                requested_range.start,
                min(requested_range.end, fi.earliest_changed_pos),
            )

        file_lines = fi.file_lines()
        block_inlays = ls.get_end_markers(ast, file_lines, requested_range)
        if len(block_inlays) > 0:
            inlays.extend(block_inlays)

        # The get_decl_inlays and get_call_inlays methods also check
        # and return early if the resolver is not being used, but for
        # the time being all hints are resolver-based, so we may
        # as well save ourselves the work of finding declarations and
        # calls to feed to those methods.
        if not ls.use_resolver:
            return inlays

        decls = fi.def_segments.range(requested_range)
        calls = list(
            call for call, _ in chapel.each_matching(ast, chapel.core.FnCall)
        )

        for decl in decls:
            instantiation = fi.get_inst_segment_at_position(decl.rng.start)
            inlays.extend(ls.get_decl_inlays(decl, instantiation))

        for call in calls:
            call_range = location_to_range(call.location())
            if not range_overlap(call_range, requested_range):
                continue

            # call is in the range, but not all of its inlay hints may be.
            instantiation = fi.get_inst_segment_at_position(call_range.start)
            inlays_from_call = ls.get_call_inlays(call, instantiation)

            for inlay in inlays_from_call:
                inlay_range = Range(inlay.position, inlay.position)
                if not range_overlap(inlay_range, requested_range):
                    continue
                inlays.append(inlay)

        return inlays

    @server.feature(TEXT_DOCUMENT_DOCUMENT_HIGHLIGHT)
    async def document_highlight(
        ls: ChapelLanguageServer, params: DocumentHighlightParams
    ):
        text_doc_uri = params.text_document.uri
        text_doc = ls.workspace.get_text_document(text_doc_uri)

        fi, _ = ls.get_file_info(text_doc.uri)

        node_and_loc = fi.get_target_segment_at_position(params.position)
        if not node_and_loc:
            return None

        # todo: it would be nice if this differentiated between read and write
        highlights = []
        # only highlight the declaration if it is in the current document
        if node_and_loc.get_uri() == text_doc_uri:
            dh = DocumentHighlight(node_and_loc.rng, DocumentHighlightKind.Text)
            highlights.append(dh)
        uses = fi.uses_here.get(node_and_loc.node, [])
        highlights += [
            DocumentHighlight(use.rng, DocumentHighlightKind.Text)
            for use in uses
        ]

        return highlights

    @server.feature(TEXT_DOCUMENT_CODE_LENS)
    async def code_lens(ls: ChapelLanguageServer, params: CodeLensParams):

        text_doc = ls.workspace.get_text_document(params.text_document.uri)
        fi, _ = ls.get_file_info(text_doc.uri)
        filename = fi.uri[len("file://") :]

        actions = []

        if ls.is_vscode:
            for mod, _ in chapel.each_matching(fi.get_asts(), chapel.Module):
                assert isinstance(mod, chapel.Module)
                test_functions = mod.find_test_functions()
                for func in test_functions:
                    name = func.name()
                    lens = CodeLens(
                        data=(
                            filename,
                            name,
                        ),
                        command=Command(
                            "▶ Run {}".format(name),
                            "chapel.mason.invokeTestFile",
                            [filename, name],
                        ),
                        range=location_to_range(func.header_location()),
                    )
                    actions.append(lens)

                test_main = mod.find_unittest_main()
                if test_main is None:
                    continue
                lens = CodeLens(
                    data=(filename,),
                    command=Command(
                        "▶ Run All",
                        "chapel.mason.invokeTestFile",
                        [filename],
                    ),
                    range=location_to_range(test_main.location()),
                )
                actions.append(lens)

        # return early if the resolver is not being used or the feature is disabled
        if not ls.use_resolver:
            return actions
        if not ls.show_instantiations:
            return actions

        text_doc = ls.workspace.get_text_document(params.text_document.uri)

        fi, _ = ls.get_file_info(text_doc.uri)

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
                            "Show Instantiation",
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
        decl = fi.find_decl_by_unique_id(unique_id)

        if decl is None:
            return

        node = decl.node
        if not isinstance(node, chapel.Function):
            return

        inst = fi.instantiation_at_index(node, i)
        node_and_range = NodeAndRange.for_entire_node(decl.node)
        fi.instantiation_segments.overwrite((node_and_range, inst))
        fi.update_call_segments_from_instantiations(node_and_range.rng)

        ls.lsp.send_request_async(WORKSPACE_INLAY_HINT_REFRESH)
        ls.lsp.send_request_async(WORKSPACE_SEMANTIC_TOKENS_REFRESH)

    @server.command("chpl-language-server/showGeneric")
    async def show_generic(ls: ChapelLanguageServer, data: Tuple[str, str]):
        uri, unique_id = data

        fi, _ = ls.get_file_info(uri)
        decl = fi.find_instantiation_by_unique_id(unique_id)
        if decl is None:
            return

        affected_range = decl[0].rng
        fi.instantiation_segments.clear_range(affected_range)
        fi.update_call_segments_from_instantiations(affected_range)

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
            if instantiation and instantiation.ast() == node:
                sigs.append(instantiation)
            elif uid in fi.instantiations:
                sigs.extend(fi.instantiations[uid].keys())

        # Oddly, returning multiple here makes for no child nodes in the VSCode
        # UI. Just take one signature for now.
        return next(
            ([ls.fn_to_call_hierarchy_item(sig, fi.context)] for sig in sigs),
            [],
        )

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
