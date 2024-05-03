import sys
import os
import tempfile
import typing

from lsprotocol.types import ClientCapabilities
from lsprotocol.types import CompletionList
from lsprotocol.types import CompletionParams
from lsprotocol.types import InitializeParams
from lsprotocol.types import Position
from lsprotocol.types import TextDocumentIdentifier

import pytest
import pytest_lsp
from pytest_lsp import ClientServerConfig
from pytest_lsp import LanguageClient

class SourceFilesContext:
    def __init__(self, **files: str):
        self.tempdir = tempfile.TemporaryDirectory()
        for (name, contents) in files.items():
            with open(os.path.join(self.tempdir.name, name + ".chpl"), "w") as f:
                f.write(contents)

    def __enter__(self):
        return lambda name: f"file://{os.path.join(self.tempdir.name, name + '.chpl')}"

    def __exit__(self, *exc):
        return False

class SourceFileContext():
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

CHPL_HOME = os.environ.get("CHPL_HOME")
if not CHPL_HOME:
    raise ValueError("The Language Server tests require the CHPL_HOME environment variable to be set.")

@pytest_lsp.fixture(
    config=ClientServerConfig(server_command=[sys.executable, os.path.join(CHPL_HOME, "tools", "chpl-language-server", "src", "chpl-language-server.py")]),
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

    positions = [ (n, 0) for n in range(1, 5) ]
    file = """ for i in 1..10 {
                 for j in 1..10 {
                   writeln("Hello, world!", (i, j));
                 }
               }
           """

    global_symbols = ["here", "strideKind", "boundKind", "Locales"]

    with source_file(file) as uri:
        for (line, column) in positions:
            results = await client.text_document_completion_async(
                params=CompletionParams(
                    position=Position(line=line, character=column),
                    text_document=TextDocumentIdentifier(uri=uri),
                )
            )

            # spec: it could be a completion item list, or a CompletionList,
            # or null. We care about the list of items.
            assert results is not None
            if isinstance(results, CompletionList):
                results = results.items
            result_names = [ r.label for r in results ]

            for symbol in global_symbols:
                assert symbol in result_names

    pass
