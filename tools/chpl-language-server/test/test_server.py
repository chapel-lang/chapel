import sys
import os
import tempfile
import typing
import json

from lsprotocol.types import ClientCapabilities
from lsprotocol.types import CompletionList, CompletionParams
from lsprotocol.types import DefinitionParams, DeclarationParams
from lsprotocol.types import ReferenceParams, ReferenceContext
from lsprotocol.types import InitializeParams
from lsprotocol.types import LocationLink, Location, Position
from lsprotocol.types import TextDocumentIdentifier
from lsprotocol.types import (
    DidChangeWorkspaceFoldersParams,
    WorkspaceFoldersChangeEvent,
    WorkspaceFolder
)

import pytest
import pytest_lsp
from pytest_lsp import ClientServerConfig
from pytest_lsp import LanguageClient

CHPL_HOME = os.environ.get("CHPL_HOME")
if not CHPL_HOME:
    raise ValueError(
        "The Language Server tests require the CHPL_HOME environment variable to be set."
    )


def strip_leading_whitespace(text: str) -> str:
    lines = text.split("\n")

    # Ignore leading empty lines
    while lines and not lines[0].strip():
        lines.pop(0)

    min_indent = min(
        len(line) - len(line.lstrip()) for line in lines if line.strip()
    )
    return "\n".join(line[min_indent:] for line in lines)


class SourceFilesContext:
    def __init__(self, client: LanguageClient, **files: str):
        self.tempdir = tempfile.TemporaryDirectory(delete=False)
        self.client = client

        commands = {}
        allfiles = []
        for name, contents in files.items():
            filepath = os.path.join(self.tempdir.name, name + ".chpl")
            with open(filepath, "w") as f:
                f.write(strip_leading_whitespace(contents))

            allfiles.append(filepath)
            commands[filepath] = [{"module_dirs": [], "files": allfiles}]

        commandspath = os.path.join(self.tempdir.name, ".cls-commands.json")
        with open(commandspath, "w") as f:
            json.dump(commands, f)

    def __enter__(self):
        self.tempdir.__enter__()

        # Let the client know we added a new workspace folder.
        uri = "file://" + self.tempdir.name
        added = [WorkspaceFolder(uri=uri, name="Temp Workspace")]
        event = WorkspaceFoldersChangeEvent(added=added, removed=[])
        params = DidChangeWorkspaceFoldersParams(event=event)
        self.client.workspace_did_change_workspace_folders(params)

        return lambda name: TextDocumentIdentifier(
            uri=f"file://{os.path.join(self.tempdir.name, name + '.chpl')}"
        )

    def __exit__(self, *exc):
        return self.tempdir.__exit__(*exc)


class SourceFileContext:
    def __init__(self, main_file: str, client: LanguageClient, **files: str):
        self.main_file = main_file
        self.source_files_context = SourceFilesContext(client, **files)

    def __enter__(self):
        return self.source_files_context.__enter__()(self.main_file)

    def __exit__(self, *exc):
        return self.source_files_context.__exit__(*exc)


def source_files(client: LanguageClient, **files: str):
    """
    Context manager that creates a temporary directory and populates
    it with the given files. The names of the keyword arguments are
    used as the names of the files. Yields a function that can be used to
    get the URI of a file by name.

    Also creates a .cls-commands.json file that can be used by the
    language server to connect the files together in the workspace.
    """
    return SourceFilesContext(client, **files)


def source_file(client: LanguageClient, contents: str):
    """
    Context manager that creates a temporary directory and populates
    it with the given file. Yields the path to the file.
    """
    return SourceFileContext("main", client, main=contents)


def pos(coord: typing.Tuple[int, int]):
    """
    Shorthand for writing position literals.
    """

    line, column = coord
    return Position(line=line, character=column)


def standard_module(name: str):
    """
    Retrieves the path of a standard module with the given name.
    """

    return TextDocumentIdentifier(
        f"file://{os.path.join(CHPL_HOME, 'modules', name)}"
    )


@pytest_lsp.fixture(
    config=ClientServerConfig(
        server_command=[
            sys.executable,
            os.path.join(
                CHPL_HOME,
                "tools",
                "chpl-language-server",
                "src",
                "chpl-language-server.py",
            ),
        ]
    ),
)
async def client(lsp_client: LanguageClient):
    # Setup
    params = InitializeParams(capabilities=ClientCapabilities())
    await lsp_client.initialize_session(params)

    yield

    # Teardown
    await lsp_client.shutdown_session()


"""
The type of 'expected destinations' to be used with the various checking
functions below.
"""
DestinationTestType = typing.Union[
    None,
    Position,
    TextDocumentIdentifier,
    typing.Tuple[TextDocumentIdentifier, Position],
]


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
    dsts: typing.List[DestinationTestType],
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


@pytest.mark.asyncio
async def test_global_completion(client: LanguageClient):
    """
    Test top-level autocompletion.

    All completion is based entirely on global symbols right now; thus,
    anywhere you try, you should get the same results.
    """

    positions = [pos((n, 0)) for n in range(1, 5)]
    file = """
           for i in 1..10 {
             for j in 1..10 {
               writeln("Hello, world!", (i, j));
             }
           }
           """

    global_symbols = ["here", "strideKind", "boundKind", "Locales"]

    with source_file(client, file) as doc:
        for position in positions:
            results = await client.text_document_completion_async(
                params=CompletionParams(position=position, text_document=doc)
            )

            # spec: it could be a completion item list, or a CompletionList,
            # or null. We care about the list of items.
            assert results is not None
            if isinstance(results, CompletionList):
                results = results.items
            result_names = [r.label for r in results]

            for symbol in global_symbols:
                assert symbol in result_names

        assert len(client.diagnostics) == 0


@pytest.mark.asyncio
async def test_go_to_definition_simple(client: LanguageClient):
    """
    Basic test cases for go-to-definition.
    """

    file = """

           var x: int = 5;
           var y: int = x;
           var z: int = x + y;
           {
             var x = 10;
             var y = x;
           }
           """

    with source_file(client, file) as doc:
        # Definitions link to themselves
        await check_goto_decl_def(client, doc, pos((0, 4)), pos((0, 4)))
        await check_goto_decl_def(client, doc, pos((1, 4)), pos((1, 4)))
        await check_goto_decl_def(client, doc, pos((2, 4)), pos((2, 4)))
        await check_goto_decl_def(client, doc, pos((4, 6)), pos((4, 6)))
        await check_goto_decl_def(client, doc, pos((5, 6)), pos((5, 6)))

        # References link to their definitions
        await check_goto_decl_def(client, doc, pos((1, 13)), pos((0, 4)))
        await check_goto_decl_def(client, doc, pos((2, 13)), pos((0, 4)))
        await check_goto_decl_def(client, doc, pos((2, 17)), pos((1, 4)))
        await check_goto_decl_def(client, doc, pos((5, 10)), pos((4, 6)))

        # Other things don't go anywhere
        # 'var' tokens
        await check_goto_decl_def(client, doc, pos((0, 2)), None)
        await check_goto_decl_def(client, doc, pos((1, 2)), None)
        await check_goto_decl_def(client, doc, pos((2, 2)), None)
        await check_goto_decl_def(client, doc, pos((4, 4)), None)
        await check_goto_decl_def(client, doc, pos((5, 4)), None)
        # numbers
        await check_goto_decl_def(client, doc, pos((0, 13)), None)
        await check_goto_decl_def(client, doc, pos((4, 10)), None)

        assert len(client.diagnostics) == 0


@pytest.mark.asyncio
async def test_go_to_definition_use_standard(client: LanguageClient):
    """
    Ensure that go-to-definition works on standard module symbols
    that are used or imported.
    """

    file = """
           use IO;
           use List, Map;
           import Time;
           """

    mod_IO = standard_module("standard/IO.chpl")
    mod_List = standard_module("standard/List.chpl")
    mod_Map = standard_module("standard/Map.chpl")
    mod_Time = standard_module("standard/Time.chpl")

    with source_file(client, file) as doc:
        await check_goto_decl_def(client, doc, pos((0, 4)), mod_IO)
        await check_goto_decl_def(client, doc, pos((1, 4)), mod_List)
        await check_goto_decl_def(client, doc, pos((1, 10)), mod_Map)
        await check_goto_decl_def(client, doc, pos((2, 8)), mod_Time)

        assert len(client.diagnostics) == 0


@pytest.mark.asyncio
async def test_go_to_definition_standard_rename(client: LanguageClient):
    """
    Ensure that go-to-definition works with using/importing symbols,
    even if renaking is in use.
    """

    file = """
           use IO as OI;
           import IO.{ioMode as im};
           use List only list;
           """

    mod_IO = standard_module("standard/IO.chpl")
    mod_List = standard_module("standard/List.chpl")

    with source_file(client, file) as doc:
        await check_goto_decl_def(
            client, doc, pos((0, 4)), mod_IO, expect_str="module IO"
        )
        await check_goto_decl_def(
            client, doc, pos((0, 10)), mod_IO, expect_str="module IO"
        )
        await check_goto_decl_def(
            client, doc, pos((1, 7)), mod_IO, expect_str="module IO"
        )
        await check_goto_decl_def(
            client, doc, pos((1, 11)), mod_IO, expect_str="enum ioMode"
        )
        await check_goto_decl_def(
            client, doc, pos((1, 21)), mod_IO, expect_str="enum ioMode"
        )
        await check_goto_decl_def(
            client, doc, pos((2, 4)), mod_List, expect_str="module List"
        )
        await check_goto_decl_def(
            client, doc, pos((2, 14)), mod_List, expect_str="record list"
        )

        assert len(client.diagnostics) == 0


@pytest.mark.asyncio
async def test_go_to_record_def(client: LanguageClient):
    """
    Ensure that 'go to definition' on a type actually works.
    """

    file = """
           record myRec {}
           var x: myRec;
           var y = new myRec();
           """

    with source_file(client, file) as doc:
        await check_goto_decl_def(client, doc, pos((0, 7)), pos((0, 7)))
        await check_goto_decl_def(client, doc, pos((1, 7)), pos((0, 7)))
        await check_goto_decl_def(client, doc, pos((2, 12)), pos((0, 7)))

        assert len(client.diagnostics) == 0


@pytest.mark.asyncio
async def test_list_references(client: LanguageClient):
    """
    Basic 'list references' test with shadowing and scopes.
    """

    file = """
           var x = 42;
           var y = x;
           for i in 1..10 {
                var z = x;
                var x = 42 + i;

                while true {
                    var i = 0;
                    var j = i + 1;
                }
           }
           """

    with source_file(client, file) as doc:
        # 'find references' on definitions;
        # the cross checking will also validate the references.
        await check_references_and_cross_check(
            client, doc, pos((0, 4)), [pos((0, 4)), pos((1, 8))]
        )
        await check_references_and_cross_check(
            client, doc, pos((1, 4)), [pos((1, 4))]
        )
        await check_references_and_cross_check(
            client, doc, pos((2, 4)), [pos((2, 4)), pos((4, 18))]
        )
        await check_references_and_cross_check(
            client, doc, pos((3, 9)), [pos((3, 9))]
        )
        await check_references_and_cross_check(
            client, doc, pos((4, 9)), [pos((3, 13)), pos((4, 9))]
        )
        await check_references_and_cross_check(
            client, doc, pos((7, 13)), [pos((7, 13)), pos((8, 17))]
        )
        await check_references_and_cross_check(
            client, doc, pos((8, 13)), [pos((8, 13))]
        )

        assert len(client.diagnostics) == 0


@pytest.mark.asyncio
async def test_list_references_across_files(client: LanguageClient):
    """
    Ensure that list-references works across multiple user-defined files.
    """

    fileA = """
            module A {
              var x = 42;
            }
            """
    fileB = """
            module B {
              use A;

              var y = x;
            }
            """
    fileC = """
            module C {
              import A.x;

              var z = x;
            }
            """
    with source_files(client, A=fileA, B=fileB, C=fileC) as docs:
        all_refs = [
            (docs("A"), pos((1, 6))),
            (docs("B"), pos((3, 10))),
            (docs("C"), pos((3, 10))),
        ]

        await check_references_and_cross_check(
            client, docs("A"), pos((1, 6)), all_refs
        )
        assert len(client.diagnostics) == 0


@pytest.mark.asyncio
@pytest.mark.xfail
async def test_list_references_standard(client: LanguageClient):
    """
    Doesn't seem to work now; would require loading and processing entire
    standard library to find all references to a standard symbol.
    """

    file = """
           use IO;
           type myType = ioMode;
           """

    with source_file(client, file) as doc:
        references = await client.text_document_references_async(
            params=ReferenceParams(
                text_document=doc,
                position=pos((1, 14)),
                context=ReferenceContext(include_declaration=True),
            )
        )

        assert references is not None
        assert len(references) > 10
