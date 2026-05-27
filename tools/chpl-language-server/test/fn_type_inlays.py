"""
Test that function return type inlay hints
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
            "--resolver",
            "--fn-type-inlays",
            "--no-type-inlays",
            "--no-param-inlays",
            "--no-literal-arg-inlays",
            "--end-markers=none",
        ],
        client_factory=get_base_client,
    )
)
async def client(lsp_client: LanguageClient):
    params = InitializeParams(capabilities=ClientCapabilities())
    await lsp_client.initialize_session(params)
    yield
    await lsp_client.shutdown_session()


@pytest.mark.asyncio
async def test_fn_type_inlay_concrete_int(client: LanguageClient):
    """
    Ensure that a concrete function returning int gets an inlay.
    """
    file = """
            proc test() do return 42;
           """
    inlays = [(pos((0, 11)), "int(64)")]
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_fn_type_inlay_concrete_real(client: LanguageClient):
    """
    Ensure that a concrete function returning real gets an inlay.
    """
    file = """
            proc foo() {
              return 42.0;
            }
           """
    inlays = [(pos((0, 10)), "real(64)")]
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_fn_type_inlay_explicit_no_inlay(client: LanguageClient):
    """
    Ensure that a function with an explicit return type annotation gets no inlay.
    """
    file = """
            proc bar(): int do return 1;
           """
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(client, doc, rng((0, 0), endpos(file)), [])


@pytest.mark.asyncio
async def test_fn_type_inlay_generic_const_return(client: LanguageClient):
    """
    Ensure that a generic function with a concrete return value gets the
    common inlay when called with multiple types.
    """
    file = """
            proc idk2(x) { return 42; }
            idk2(10);
            idk2(10.0);
           """
    inlays = [(pos((0, 12)), "int(64)")]
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_fn_type_inlay_return_intent_type(client: LanguageClient):
    """
    Ensure return type annotations for functions with explicit return intents.
    """
    file = """
            proc baz() type { return int; }
           """
    inlays = [(pos((0, 15)), "int(64)")]
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_fn_type_inlay_where_clause(client: LanguageClient):
    """
    Ensure that a function with a 'where' clause gets an inlay in the right
    spot.
    """
    file = """
            proc boop() type where true { return int; }
           """
    inlays = [(pos((0, 16)), "int(64)")]
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_fn_type_inlay_per_instantiation(client: LanguageClient):
    """
    For functions whose return type depends on the instantiation (i.e.,
    is not common), ensure that clicking a particular instantiation shows
    an appropriate inlay for that instantiation.
    """
    file = """
            proc idk1(x) { return x; }
            idk1(10);
            idk1(10.0);
           """
    # 3 lenses: "Show Generic" + 2 instantiations
    expected_lens = (pos((0, 5)), 3)

    generic_inlays: EXPECTED_INLAYS = []
    int_inlays: EXPECTED_INLAYS = [(pos((0, 12)), ": int(64)", None)]
    real_inlays: EXPECTED_INLAYS = [(pos((0, 12)), ": real(64)", None)]
    all_inlays = [generic_inlays, int_inlays, real_inlays]

    await click_lenses_and_check_inlays(
        client, expected_lens, all_inlays, A=file
    )


@pytest.mark.asyncio
async def test_fn_type_inlay_header_variants(client: LanguageClient):
    """
    More cases for return inlays.
    """
    file = """
            proc f1() param { return 42; }
            proc f2() throws { return 42; }
            proc f3() throws where true do return 42;
            proc f4() where true do return 42;
            proc f5() param throws where true do return 42;
            proc f6() param where true do return 42;
           """
    inlays = [
        (pos((0, 15)), "int(64)"),
        (pos((1, 16)), "int(64)"),
        (pos((2, 16)), "int(64)"),
        (pos((3, 9)), "int(64)"),
        (pos((4, 22)), "int(64)"),
        (pos((5, 15)), "int(64)"),
    ]
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(client, doc, rng((0, 0), endpos(file)), inlays)
