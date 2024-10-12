from collections import defaultdict
import json
import os
import tempfile
import typing
from collections.abc import Sequence
import functools

from lsprotocol.types import (
    DefinitionParams,
    DeclarationParams,
    TypeDefinitionParams,
)
from lsprotocol.types import ReferenceParams, ReferenceContext
from lsprotocol.types import InlayHintParams, InlayHintKind, InlayHint
from lsprotocol.types import (
    CodeLensParams,
    CodeLens,
    Command,
    ExecuteCommandParams,
)
from lsprotocol.types import LocationLink, Location, Position, Range
from lsprotocol.types import TextDocumentIdentifier
from lsprotocol.types import (
    DidSaveTextDocumentParams,
    TEXT_DOCUMENT_PUBLISH_DIAGNOSTICS,
    WORKSPACE_INLAY_HINT_REFRESH,
    WORKSPACE_SEMANTIC_TOKENS_REFRESH,
)
from lsprotocol.types import (
    DocumentSymbolParams,
    SymbolKind,
    DocumentSymbol,
    SymbolInformation,
)
from lsprotocol.types import (
    DidChangeWorkspaceFoldersParams,
    WorkspaceFoldersChangeEvent,
    WorkspaceFolder,
)
from pytest_lsp import LanguageClient, make_test_lsp_client

from .config import CHPL_HOME


def strip_leading_whitespace(text: str) -> str:
    lines = text.split("\n")

    # Ignore leading empty lines
    while lines and not lines[0].strip():
        lines.pop(0)

    min_indent = min(
        len(line) - len(line.lstrip()) for line in lines if line.strip()
    )
    return "\n".join(line[min_indent:] for line in lines)


def get_base_client() -> LanguageClient:
    client = make_test_lsp_client()

    @client.feature(WORKSPACE_INLAY_HINT_REFRESH)
    def on_inlay_hint_refresh(params):
        """stub method for inlay hint refresh notifications"""
        pass

    @client.feature(WORKSPACE_SEMANTIC_TOKENS_REFRESH)
    def on_semantic_token_refresh(params):
        """stub method for semantic token refresh notifications"""
        pass

    return client


class SourceFilesContext:
    def __init__(
        self,
        client: LanguageClient,
        files: typing.Dict[str, str],
        build_cls_commands: bool = True,
    ):
        self.tempdir = tempfile.TemporaryDirectory()
        self.client = client

        commands = {}
        allfiles = []
        for name, contents in files.items():
            # make the directory structure, last component is the name
            dir_path = self.tempdir.name
            components = os.path.normpath(name).split(os.sep)
            name = components[-1]
            for component in components[:-1]:
                dir_path = os.path.join(dir_path, component)
                os.makedirs(dir_path, exist_ok=True)

            filepath = os.path.join(dir_path, name + ".chpl")
            with open(filepath, "w") as f:
                f.write(strip_leading_whitespace(contents))

            allfiles.append(filepath)
            commands[filepath] = [{"module_dirs": [], "files": allfiles}]

        commandspath = os.path.join(self.tempdir.name, ".cls-commands.json")
        if build_cls_commands:
            with open(commandspath, "w") as f:
                json.dump(commands, f)

    def _get_doc(self, name: str) -> TextDocumentIdentifier:
        return TextDocumentIdentifier(
            uri=f"file://{os.path.join(self.tempdir.name, name + '.chpl')}"
        )

    async def __aenter__(self):
        self.tempdir.__enter__()

        # Let the client know we added a new workspace folder.
        uri = "file://" + self.tempdir.name
        added = [WorkspaceFolder(uri=uri, name="Temp Workspace")]
        event = WorkspaceFoldersChangeEvent(added=added, removed=[])
        params = DidChangeWorkspaceFoldersParams(event=event)
        self.client.workspace_did_change_workspace_folders(params)

        return functools.partial(self._get_doc)

    async def __aexit__(self, *exc):
        return self.tempdir.__exit__(*exc)


class SourceFileContext:
    def __init__(
        self,
        main_file: str,
        client: LanguageClient,
        files: typing.Dict[str, str],
        num_errors: typing.Optional[int],
    ):
        self.main_file = main_file
        self.source_files_context = SourceFilesContext(client, files)
        self.num_errors = num_errors

    async def __aenter__(self):
        doc = await self.source_files_context.__aenter__()
        return doc(self.main_file)

    async def __aexit__(self, *exc):
        if self.num_errors is not None:
            client = self.source_files_context.client
            doc = self.source_files_context._get_doc(self.main_file)
            await save_file(client, doc)
            assert len(client.diagnostics[doc.uri]) == self.num_errors

        return await self.source_files_context.__aexit__(*exc)


def source_files(client: LanguageClient, **files: str):
    """
    Context manager that creates a temporary directory and populates
    it with the given files. The names of the keyword arguments are
    used as the names of the files. Yields a function that can be used to
    get the URI of a file by name.

    Also creates a .cls-commands.json file that can be used by the
    language server to connect the files together in the workspace.
    """
    return SourceFilesContext(client, files)


def unrelated_source_files(client: LanguageClient, **files: str):
    """
    Same as 'source_files', but doesn't create a .cls-commands.json file that
    would cause the files to be treated as "connected" and resolved together.
    """
    return SourceFilesContext(client, files, build_cls_commands=False)


def source_file(
    client: LanguageClient,
    contents: str,
    num_errors: typing.Optional[int] = 0,
):
    """
    Context manager that creates a temporary directory and populates
    it with the given file. Yields the path to the file.
    """
    return SourceFileContext("main", client, {"main": contents}, num_errors)


def source_files_dict(client: LanguageClient, files: typing.Dict[str, str]):
    """
    Context manager that creates a temporary directory and populates it with
    the given files. The files dictionary uses the keys as file paths and the
    values as the content of the files. Yields a function that can be used to
    get the URI of a file by name.

    Also creates a .cls-commands.json file that can be used by the
    language server to connect the files together in the workspace.
    """
    return SourceFilesContext(client, files)


async def save_file(client: LanguageClient, *docs: TextDocumentIdentifier):
    """
    Saves the given document.
    """
    for doc in docs:
        client.text_document_did_save(
            params=DidSaveTextDocumentParams(text_document=doc)
        )
        await client.wait_for_notification(TEXT_DOCUMENT_PUBLISH_DIAGNOSTICS)


def pos(coord: typing.Tuple[int, int]) -> Position:
    """
    Shorthand for writing position literals.
    """

    line, column = coord
    return Position(line=line, character=column)


def depos(coord: Position) -> typing.Tuple[int, int]:
    """
    Shorthand for deconstructing position literals.
    """

    return coord.line, coord.character


def rng(
    start: typing.Union[typing.Tuple[int, int], Position],
    end: typing.Union[typing.Tuple[int, int], Position],
) -> Range:
    """
    Shorthand for writing range literals.
    """
    if isinstance(start, tuple):
        start = pos(start)
    if isinstance(end, tuple):
        end = pos(end)
    return Range(start=start, end=end)


def endpos(text: str) -> Position:
    """
    Returns the position at the end of the given text.
    """

    text = strip_leading_whitespace(text)
    lines = text.splitlines()
    if len(lines) == 0:
        return pos((0, 0))
    return pos((len(lines) - 1, len(lines[-1])))


def standard_module(name: str):
    """
    Retrieves the path of a standard module with the given name.
    """

    fullname = name + ".chpl"
    return TextDocumentIdentifier(
        f"file://{os.path.join(CHPL_HOME(), 'modules', 'standard', fullname)}"
    )


def internal_module(name: str):
    """
    Retrieves the path of an internal module with the given name.
    """

    fullname = name + ".chpl"
    return TextDocumentIdentifier(
        f"file://{os.path.join(CHPL_HOME(), 'modules', 'internal', fullname)}"
    )


DestinationTestType = typing.Union[
    None,
    Position,
    TextDocumentIdentifier,
    typing.Tuple[TextDocumentIdentifier, Position],
]
"""
The type of 'expected destinations' to be used with the various checking
functions below.
"""


def check_dst(
    doc: TextDocumentIdentifier,
    dst_actual: typing.Union[Location, LocationLink],
    dst_expected: DestinationTestType,
    expect_str: typing.Optional[str] = None,
) -> bool:
    """
    Matches a location against what it is expected to be.

    * If the expected destination is a position, matches the position
      and expects the same file as the source document.
    * If the expected destination is a document, only ensures that the
      actual location is in that document, and ignores the line etc.
    * If the expected destination is a document-position tuple, ensures
      that both the file and line number are as specified by the tuple.

    If expect_str is provided, also ensures that the first line of the location
    contains the given string. This is useful to help validate against module
    code, whose line numbers might change as the module is updated.
    """

    if isinstance(dst_actual, LocationLink):
        got_dst_pos = dst_actual.target_range.start
        got_dst_uri = dst_actual.target_uri
    else:
        got_dst_pos = dst_actual.range.start
        got_dst_uri = dst_actual.uri

    if isinstance(dst_expected, tuple):
        if got_dst_uri != dst_expected[0].uri or got_dst_pos != dst_expected[1]:
            return False
    elif isinstance(dst_expected, TextDocumentIdentifier):
        if got_dst_uri != dst_expected.uri:
            return False
    else:
        if got_dst_pos != dst_expected or got_dst_uri != doc.uri:
            return False

    if expect_str is not None:
        assert got_dst_uri.startswith("file://")
        file_content = open(got_dst_uri[len("file://") :]).read()
        return expect_str in file_content.split("\n")[got_dst_pos.line]

    return True


async def check_goto_decl_def(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    src: Position,
    dst: DestinationTestType,
    expect_str: typing.Optional[str] = None,
):
    """
    Ensures that go-to-definition and go-to-declaration on a symbol
    work, taking a source cursor position to an expected destination position.
    In Chapel, declaration and definition are the same, so the same check
    is used for both.
    """

    def validate(
        results: typing.Optional[
            typing.Union[
                Location, typing.List[Location], typing.List[LocationLink]
            ]
        ]
    ):
        if dst is None:
            assert results is None or (
                isinstance(results, list) and len(results) == 0
            )
            return

        assert results is not None
        result = None
        if isinstance(results, list):
            assert len(results) == 1
            result = results[0]
        else:
            result = results

        assert check_dst(doc, result, dst, expect_str)

    results = await client.text_document_definition_async(
        params=DefinitionParams(text_document=doc, position=src)
    )
    validate(results)

    results = await client.text_document_declaration_async(
        params=DeclarationParams(text_document=doc, position=src)
    )
    validate(results)


async def check_goto_type_def(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    src: Position,
    dst: DestinationTestType,
    expect_str: typing.Optional[str] = None,
):
    """
    Ensures that go-to-type-definition works, taking a source cursor position for a variable to an expected destination position.
    """

    def validate(
        results: typing.Optional[
            typing.Union[
                Location, typing.List[Location], typing.List[LocationLink]
            ]
        ]
    ):
        if dst is None:
            assert results is None or (
                isinstance(results, list) and len(results) == 0
            )
            return

        assert results is not None
        result = None
        if isinstance(results, list):
            assert len(results) == 1
            result = results[0]
        else:
            result = results

        assert check_dst(doc, result, dst, expect_str)

    results = await client.text_document_type_definition_async(
        params=TypeDefinitionParams(text_document=doc, position=src)
    )
    validate(results)


async def check_goto_decl_def_module(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    src: Position,
    mod: TextDocumentIdentifier,
):
    pieces = os.path.split(mod.uri)
    mod_name = pieces[-1][: -len(".chpl")]
    await check_goto_decl_def(client, doc, src, mod, f"module {mod_name} ")


async def check_references(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    src: Position,
    dsts: typing.List[DestinationTestType],
) -> typing.List[Location]:
    """
    Given a document, a 'hover' position and a list of expected results,
    validates that the language server finds the given references.
    """

    references = await client.text_document_references_async(
        params=ReferenceParams(
            text_document=doc,
            position=src,
            context=ReferenceContext(include_declaration=True),
        )
    )
    assert references is not None

    # Two-way check: make sure that all references are expected, and
    # all expected references are found.

    for ref in references:
        assert any(check_dst(doc, ref, dst) for dst in dsts)

    for dst in dsts:
        assert any(check_dst(doc, ref, dst) for ref in references)

    return references


async def check_references_and_cross_check(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    src: Position,
    dsts: Sequence[DestinationTestType],
):
    """
    Does two things:
        1. Performs the usual references list, ensuring that the references
           found by the language server match the expected ones.
        2. For each reference point found, repeats that check. Thus,
           if hovering over A finds B and C, then hovering over B should
           find A and C.
    This way, we can test a lot more cases automatically.
    """

    references = await check_references(client, doc, src, dsts)
    locations = [
        (TextDocumentIdentifier(uri=ref.uri), ref.range.start)
        for ref in references
    ]
    for ref in references:
        new_doc = TextDocumentIdentifier(uri=ref.uri)
        await check_references(client, new_doc, ref.range.start, locations)


async def check_inlay_hints(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    rng: Range,
    expected_inlays: Sequence[
        typing.Tuple[Position, str, typing.Optional[InlayHintKind]]
    ],
) -> typing.List[InlayHint]:
    """
    Check that the inlay hints in the document match the expected inlays. The
    expected inlays list should be sorted in the order that the inlays appear
    in the document.

    Each tuple in the expected inlays list should be a tuple of the position of
    the inlay hint, the text of the inlay hint, and kind of inlay. If the kind
    is None, it is ignored.
    """
    results = await client.text_document_inlay_hint_async(
        params=InlayHintParams(text_document=doc, range=rng)
    )

    if len(expected_inlays) == 0:
        assert results is None or len(results) == 0
        return []

    assert results is not None
    assert len(expected_inlays) == len(results)

    sorted_results = sorted(results, key=lambda x: x.position)

    for expected, actual in zip(expected_inlays, sorted_results):
        assert expected[0] == actual.position

        actual_label = actual.label
        if isinstance(actual_label, list):
            actual_label = "".join([l.value for l in actual_label])

        assert expected[1] == actual_label
        assert expected[2] == actual.kind

    return results


async def check_type_inlay_hints(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    rng: Range,
    expected_inlays: Sequence[
        typing.Union[
            typing.Tuple[Position, str], typing.Tuple[Position, str, bool]
        ]
    ],
) -> typing.List[InlayHint]:
    """
    Helper method for `check_inlay_hints`. Adds the `: ` prefix.

    `expected_inlays` is a list of tuples. The tuples can be either of length 2
    or 3.
    - If the tuple is of length 2, the first element is the expected position
      and the second element is the expected text of the type. This implies that
      the inlay hint is insertable.
    - If the tuple is of length 3, then the first two elements are the same as
      the 2-tuple. The third element is a boolean that indicates whether the
      inlay hint is insertable.
    """
    # we currently do not make use of InlayHintKind.Type for type inlays in CLS
    inlays_with_colon = [(i[0], f": {i[1]}", None) for i in expected_inlays]
    inlays = [
        (i[0], f": {i[1]}", i[2] if len(i) >= 3 else True)
        for i in expected_inlays
    ]
    actual_inlays = await check_inlay_hints(client, doc, rng, inlays_with_colon)

    # Check that the inlays are insertable
    for expected, actual in zip(inlays, actual_inlays):
        if expected[2]:
            # the list of inlay text edits should have one element and have the
            # same text/range as the inlay
            assert actual.text_edits is not None
            assert len(actual.text_edits) == 1
            assert actual.text_edits[0].range.start == expected[0]
            assert actual.text_edits[0].new_text == expected[1]
        else:
            # the inlay should not be insertable
            assert actual.text_edits is None

    return actual_inlays


async def check_param_inlay_hints(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    rng: Range,
    expected_inlays: Sequence[typing.Tuple[Position, str]],
) -> typing.List[InlayHint]:
    """
    Helper method for `check_inlay_hints`. Adds the `param value is ` prefix.
    """
    inlays = [
        (pos, f"param value is {text}", None) for pos, text in expected_inlays
    ]
    actual_inlays = await check_inlay_hints(client, doc, rng, inlays)

    return actual_inlays


async def check_end_marker_inlay_hints(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    rng: Range,
    expected_inlays: Sequence[typing.Tuple[Position, str]],
) -> typing.List[InlayHint]:
    """
    Helper method for `check_inlay_hints`.
    """
    inlays = [(pos, text, None) for pos, text in expected_inlays]
    actual_inlays = await check_inlay_hints(client, doc, rng, inlays)

    return actual_inlays


async def check_symbol_information(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    expected_symbols: Sequence[typing.Tuple[Range, str, SymbolKind]],
) -> Sequence[typing.Union[SymbolInformation, DocumentSymbol]]:
    """
    Check that the document symbols match the expected symbols.
    """
    actual_symbols = await client.text_document_document_symbol_async(
        params=DocumentSymbolParams(doc)
    )
    assert actual_symbols is not None
    assert len(actual_symbols) == len(expected_symbols)

    actual_symbols = sorted(
        actual_symbols,
        key=lambda x: (
            x.location.range.start
            if isinstance(x, SymbolInformation)
            else x.range.start
        ),
    )
    for expected, actual in zip(expected_symbols, actual_symbols):
        assert actual.name == expected[1]
        assert actual.kind == expected[2]
        if isinstance(actual, DocumentSymbol):
            assert actual.range == expected[0]
        else:
            assert actual.location.range == expected[0]
            assert actual.location.uri == doc.uri
    return actual_symbols


async def check_generic_code_lenses(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    expected_lenses: Sequence[typing.Tuple[Position, int]],
) -> defaultdict[typing.Tuple[int, int], typing.List[CodeLens]]:
    """
    Check that code lenses for the document match the expected ones. This only checks the lenses for "Show Generic" and "Show Instantiation" for now.

    `expected_lenses` is a list of tuples, where each tuple is a position and
    an integer. The integer is the expected number of lenses at that position.
    The positions should be the start of the name of the decl for the lenses.

    Returns the actual lenses grouped by position.
    """
    actual_lenses = await client.text_document_code_lens_async(
        params=CodeLensParams(doc)
    )
    assert actual_lenses is not None

    # group the actual lenses by position
    actual_lens_by_pos: defaultdict[
        typing.Tuple[int, int], typing.List[CodeLens]
    ] = defaultdict(list)
    for lens in actual_lenses:
        actual_lens_by_pos[depos(lens.range.start)].append(lens)

    assert len(actual_lens_by_pos.keys()) == len(expected_lenses)

    # check that the number of lenses at each position is as expected
    for pos, expected_count in expected_lenses:
        key = depos(pos)
        assert key in actual_lens_by_pos
        actual_count = len(actual_lens_by_pos.get(key, []))
        assert actual_count == expected_count

    return actual_lens_by_pos


async def execute_command(client: LanguageClient, command: Command):
    """
    Executes the given command.
    """
    await client.workspace_execute_command_async(
        params=ExecuteCommandParams(
            command=command.command, arguments=command.arguments
        )
    )
