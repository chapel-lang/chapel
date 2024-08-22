"""
Test that end markers show up properly.
"""

import sys

from lsprotocol.types import ClientCapabilities
from lsprotocol.types import InitializeParams
import pytest
import pytest_lsp
from pytest_lsp import ClientServerConfig, LanguageClient

from util.utils import *
from util.config import CLS_PATH


@pytest_lsp.fixture(
    config=ClientServerConfig(
        server_command=[
            sys.executable,
            CLS_PATH(),
            "--end-markers=all",
            "--end-marker-threshold=2",
        ],
        client_factory=get_base_client,
    )
)
async def client(lsp_client: LanguageClient):
    # Setup
    params = InitializeParams(capabilities=ClientCapabilities())
    await lsp_client.initialize_session(params)

    yield

    # Teardown
    await lsp_client.shutdown_session()


@pytest.mark.asyncio
async def test_end_markers_functions(client: LanguageClient):
    """
    Test that end markers show up properly for functions.
    """
    file = """
            proc foo() {


            }
            proc bar() {
              proc nested() {


              }
            } //
            proc foobar () do ;
            proc foobar2 () {

            }
            """

    inlays = [
        (pos((3, 1)), "proc foo()"),
        (pos((8, 3)), "proc nested()"),
    ]

    async with source_file(client, file) as doc:
        await check_end_marker_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_end_markers_named_decl(client: LanguageClient):
    """
    Test that end markers show up properly for named decls.
    """
    file = """
            module M {
              record R {


              }
              class C {


              }
              enum E {
                a, b

              }
              union U {
                var a: int;
                var b: int;
              }
            }
            """

    inlays = [
        (pos((4, 3)), "record R"),
        (pos((8, 3)), "class C"),
        (pos((12, 3)), "enum E"),
        (pos((16, 3)), "union U"),
        (pos((17, 1)), "module M"),
    ]

    async with source_file(client, file) as doc:
        await check_end_marker_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_end_markers_select(client: LanguageClient):
    """
    Test that end markers show up properly for select/when.
    """
    file = """
            var x = 10;
            select x {
              when 10 {


              } when 20 {


              }
            }
            """

    inlays = [(pos((8, 3)), "when 20"), (pos((9, 1)), "select x")]

    async with source_file(client, file) as doc:
        await check_end_marker_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_end_markers_loops(client: LanguageClient):
    """
    Test that end markers show up properly for loops.
    """
    file = """
            for i in 1..10 {


            }
            for i in 1..10 do ;
            [(i, j) in zip(1..10, 1..10)] {
              ;
              ;
            }
            while false {


            }
            var a: int;
            forall i in 1..10 with (ref a) {


            }
            """

    inlays = [
        (pos((3, 1)), "for i in 1..10"),
        (pos((8, 1)), "(i, j) in zip(1..10, 1..10)"),
        (pos((12, 1)), "while false"),
        (pos((17, 1)), "forall i in 1..10"),
    ]

    async with source_file(client, file) as doc:
        await check_end_marker_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_end_markers_blocks(client: LanguageClient):
    """
    Test that end markers show up properly for blocks like `on`.
    """
    file = """
            on Locale[0] {


            }
            """

    inlays = [(pos((3, 1)), "on Locale[0]")]

    async with source_file(client, file) as doc:
        await check_end_marker_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )
