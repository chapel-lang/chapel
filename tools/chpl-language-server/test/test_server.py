import sys
import os
import tempfile
import typing
import json

from lsprotocol.types import ClientCapabilities
from lsprotocol.types import CompletionList
from lsprotocol.types import CompletionParams
from lsprotocol.types import DefinitionParams
from lsprotocol.types import DeclarationParams
from lsprotocol.types import ReferenceParams
from lsprotocol.types import ReferenceContext
from lsprotocol.types import InitializeParams
from lsprotocol.types import LocationLink
from lsprotocol.types import Location
from lsprotocol.types import Position
from lsprotocol.types import TextDocumentIdentifier

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
    def __init__(self, **files: str):
        self.tempdir = tempfile.TemporaryDirectory()

        commands = {}
        allfiles = []
        for name, contents in files.items():
            filepath = os.path.join(self.tempdir.name, name + ".chpl")
            with open(filepath, "w") as f:
                f.write(strip_leading_whitespace(contents))

            allfiles.append(filepath)
            commands[filepath] = {
                "module_dirs": [],
                "files": allfiles
            }

        commandspath = os.path.join(self.tempdir.name, ".cls-commands.json")
        with open(commandspath, "w") as f:
            json.dump(commands, f)

    def __enter__(self):
        self.tempdir.__enter__()
        return lambda name: TextDocumentIdentifier(
            uri=f"file://{os.path.join(self.tempdir.name, name + '.chpl')}"
        )

    def __exit__(self, *exc):
        return self.tempdir.__exit__(*exc)


class SourceFileContext:
    def __init__(self, main_file: str, **files: str):
        self.main_file = main_file
        self.source_files_context = SourceFilesContext(**files)

    def __enter__(self):
        return self.source_files_context.__enter__()(self.main_file)

    def __exit__(self, *exc):
        return self.source_files_context.__exit__(*exc)


def source_files(**files: str):
    return SourceFilesContext(**files)


def source_file(contents: str):
    return SourceFileContext("main", main=contents)


def pos(coord: typing.Tuple[int, int]):
    line, column = coord
    return Position(line=line, character=column)


def standard_module(name: str):
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


@pytest.mark.asyncio
async def test_global_completion(client: LanguageClient):
    # All completion is based entirely on global symbols right now; thus,
    # anywhere you hover, you should get the same results.

    positions = [pos((n, 0)) for n in range(1, 5)]
    file = """
           for i in 1..10 {
             for j in 1..10 {
               writeln("Hello, world!", (i, j));
             }
           }
           """

    global_symbols = ["here", "strideKind", "boundKind", "Locales"]

    with source_file(file) as doc:
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
    if isinstance(dst_actual, LocationLink):
        got_dst_pos = dst_actual.target_range.start
        got_dst_uri = dst_actual.target_uri
    else:
        got_dst_pos = dst_actual.range.start
        got_dst_uri = dst_actual.uri

    if isinstance(dst_expected, tuple):
        if got_dst_uri != dst_expected[0].uri:
            return False
        got_dst_pos = dst_expected[1]
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
) -> typing.List[Position]:
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

    return [ref.range.start for ref in references]


async def check_references_and_cross_check(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    src: Position,
    dsts: typing.List[DestinationTestType],
):
    references = await check_references(client, doc, src, dsts)
    for ref in references:
        await check_references(client, doc, ref, references)


@pytest.mark.asyncio
async def test_go_to_definition_simple(client: LanguageClient):
    file = """

           var x: int = 5;
           var y: int = x;
           var z: int = x + y;
           {
             var x = 10;
             var y = x;
           }
           """

    with source_file(file) as doc:
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
    file = """
           use IO;
           use List, Map;
           import Time;
           """

    mod_IO = standard_module("standard/IO.chpl")
    mod_List = standard_module("standard/List.chpl")
    mod_Map = standard_module("standard/Map.chpl")
    mod_Time = standard_module("standard/Time.chpl")

    with source_file(file) as doc:
        await check_goto_decl_def(client, doc, pos((0, 4)), mod_IO)
        await check_goto_decl_def(client, doc, pos((1, 4)), mod_List)
        await check_goto_decl_def(client, doc, pos((1, 10)), mod_Map)
        await check_goto_decl_def(client, doc, pos((2, 8)), mod_Time)

        assert len(client.diagnostics) == 0


@pytest.mark.asyncio
async def test_go_to_definition_standard_rename(client: LanguageClient):
    file = """
           use IO as OI;
           import IO.{ioMode as im};
           use List only list;
           """

    mod_IO = standard_module("standard/IO.chpl")
    mod_List = standard_module("standard/List.chpl")

    with source_file(file) as doc:
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
    file = """
           record myRec {}
           var x: myRec;
           var y = new myRec();
           """

    with source_file(file) as doc:
        await check_goto_decl_def(client, doc, pos((0, 7)), pos((0, 7)))
        await check_goto_decl_def(client, doc, pos((1, 7)), pos((0, 7)))
        await check_goto_decl_def(client, doc, pos((2, 12)), pos((0, 7)))

        assert len(client.diagnostics) == 0


@pytest.mark.asyncio
async def test_list_references(client: LanguageClient):
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

    with source_file(file) as doc:
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
