"""
Test that show generic code lens show up properly and work.
Also tests that using the code lens shows the appropriate inlays.
Requires `--resolver`.
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
            "--param-inlays",
            "--type-inlays",
            "--literal-arg-inlays",
            "--end-markers=none",
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
async def test_lenses_show(client: LanguageClient):
    """
    Test that the code lenses for generics show
    """

    file = """
            proc foo(type t) { }
            foo(int);

            proc bar(type t) { }
            bar(int);
            bar(real);

            proc baz() { } // concrete, no lenses
            baz();
            """
    # the number of lenses is the number of instances + 1 (for "show generic")
    lenses = [(pos((0, 5)), 2), (pos((3, 5)), 3)]

    async with source_file(client, file) as doc:
        await check_generic_code_lenses(client, doc, lenses)


@pytest.mark.asyncio
async def test_lenses_switch(client: LanguageClient):
    """
    Test that using the code lens for generics switches the type, and that show
    generic code lens show up properly and work.
    """

    file = """
            record R { type t = int; }

            proc foo(type t, param p = 1) param do
              return t == int && p == 2;

            foo(R(real), false);
            foo(R);
            foo(R(?));
            foo(int, p=3);
            """

    expected_lens = (pos((2, 5)), 5)

    generic_inlays = [(pos((5, 13)), "p = ", None)]
    R_real_inlays = [
        (pos((2, 15)), ": R(real(64))", None),
        (pos((2, 24)), "param value is false", None),
        (pos((2, 24)), ": bool", None),
        (pos((5, 13)), "p = ", None),
    ]
    R_inlays = [
        (pos((2, 15)), ": R(int(64))", None),
        (pos((2, 24)), "param value is 1", None),
        (pos((2, 24)), ": int(64)", None),
        (pos((5, 13)), "p = ", None),
    ]
    R_generic_inlays = [
        (pos((2, 15)), ": R", None),
        (pos((2, 24)), "param value is 1", None),
        (pos((2, 24)), ": int(64)", None),
        (pos((5, 13)), "p = ", None),
    ]
    int_inlays = [
        (pos((2, 15)), ": int(64)", None),
        (pos((2, 24)), "param value is 3", None),
        (pos((2, 24)), ": int(64)", None),
        (pos((5, 13)), "p = ", None),
    ]
    all_inlays = [
        generic_inlays,
        R_real_inlays,
        R_inlays,
        R_generic_inlays,
        int_inlays,
    ]

    async with source_file(client, file) as doc:
        actual_lenses = await check_generic_code_lenses(
            client, doc, [expected_lens]
        )
        key = depos(expected_lens[0])
        assert len(actual_lenses.keys()) == 1 and key in actual_lenses
        lenses = actual_lenses[key]

        # for each lens, apply the command and check the inlays
        # cycle through this twice to ensure that the lenses are not being
        # broken by other lenses
        for _ in range(2):
            for lens in lenses:
                # each lens should have a command. The 3rd argument to the command is the index of the inlays to check
                # apply the command, and then check the inlays
                command = lens.command
                assert command is not None
                if command.title == "Show Generic":
                    idx = 0
                else:
                    assert (
                        command.arguments is not None
                        and len(command.arguments) == 3
                    )
                    idx = command.arguments[2]
                    assert idx is not None
                    idx = int(idx) + 1
                # apply the command and get the inlays to check
                await execute_command(client, command)
                inlays = all_inlays[idx]

                # check the inlays
                await check_inlay_hints(
                    client, doc, rng((0, 0), endpos(file)), inlays
                )
