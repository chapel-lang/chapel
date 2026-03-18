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
import typing
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


EXPECTED_INLAY = tuple[Position, str, typing.Optional[InlayHintKind]]
EXPECTED_INLAYS = typing.Sequence[EXPECTED_INLAY]


async def click_lenses_and_check_inlays(
    client: LanguageClient,
    expected_lens: tuple[Position, int],
    all_inlays: typing.Sequence[EXPECTED_INLAYS],
    expected_lens_file = None,
    **files: str,
):
    if expected_lens_file is None:
        assert len(files) == 1
        expected_lens_file = next(iter(files.keys()))

    async with source_files(client, **files) as docs:
        doc = docs(expected_lens_file)
        file = files[expected_lens_file]
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
                        and len(command.arguments) == 4
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

    await click_lenses_and_check_inlays(client, expected_lens, all_inlays, A=file)


@pytest.mark.asyncio
async def test_lenses_default_rect(client: LanguageClient):
    """
    Test that we can show default-rectangular substitutions for functions
    that are only generic because of their array formals.
    """

    file = """
            proc foo(arr: [] int) {
              param rank = arr.rank;
              type eltType = arr.eltType;
              param arrayType = arr.type : string;
            }
            // Note: no calls; auto-instantiated.
            """

    expected_lens = (pos((0, 5)), 2)

    generic_inlays = []
    default_inlays = [
        (pos((1, 12)), "param value is 1", None),
        (pos((1, 12)), ": int(64)", None),
        (pos((2, 14)), ": int(64)", None),
        (
            pos((3, 17)),
            'param value is "[domain(1, int(64), strideKind.one)] int(64)"',
            None,
        ),
        (pos((3, 17)), ": string", None),
    ]
    all_inlays = [
        generic_inlays,
        default_inlays,
    ]

    await click_lenses_and_check_inlays(client, expected_lens, all_inlays, A=file)


@pytest.mark.asyncio
async def test_lenses_default_rect_rank1(client: LanguageClient):
    """
    Test that we can show default-rectangular substitutions for functions
    that are only generic because of their array formals.

    Here, the array has a specific rank, but it matches our default
    substitution, so the tests match.
    """

    file = """
            proc foo(arr: [1..10] int) {
              param rank = arr.rank;
              type eltType = arr.eltType;
              param arrayType = arr.type : string;
            }
            // Note: no calls; auto-instantiated.
            """

    expected_lens = (pos((0, 5)), 2)

    generic_inlays = []
    default_inlays = [
        (pos((1, 12)), "param value is 1", None),
        (pos((1, 12)), ": int(64)", None),
        (pos((2, 14)), ": int(64)", None),
        (
            pos((3, 17)),
            'param value is "[domain(1, int(64), strideKind.one)] int(64)"',
            None,
        ),
        (pos((3, 17)), ": string", None),
    ]
    all_inlays = [
        generic_inlays,
        default_inlays,
    ]

    await click_lenses_and_check_inlays(client, expected_lens, all_inlays, A=file)


@pytest.mark.asyncio
async def test_lenses_default_rect_rank2(client: LanguageClient):
    """
    Test that we can show default-rectangular substitutions for functions
    that are only generic because of their array formals.

    Here, the array has a non-default rank, so we should show an instantiation
    with a two-dimensional array.
    """

    file = """
            proc foo(arr: [1..10, 1..10] int) {
              param rank = arr.rank;
              type eltType = arr.eltType;
              param arrayType = arr.type : string;
            }
            // Note: no calls; auto-instantiated.
            """

    expected_lens = (pos((0, 5)), 2)

    generic_inlays = []
    default_inlays = [
        (pos((1, 12)), "param value is 2", None),
        (pos((1, 12)), ": int(64)", None),
        (pos((2, 14)), ": int(64)", None),
        (
            pos((3, 17)),
            'param value is "[domain(2, int(64), strideKind.one)] int(64)"',
            None,
        ),
        (pos((3, 17)), ": string", None),
    ]
    all_inlays = [
        generic_inlays,
        default_inlays,
    ]

    await click_lenses_and_check_inlays(client, expected_lens, all_inlays, A=file)


@pytest.mark.asyncio
async def test_lenses_default_rect_where_1(client: LanguageClient):
    """
    Test that we can show default-rectangular substitutions for functions
    that are only generic because of their array formals.

    Here, the procedure has a 'where' clause that constrains the array
    to be a specific rank. The rank matches, so we should be able to show
    the default substitution still.
    """

    file = """
            proc foo(arr: [] int) where arr.rank == 1 {
              param rank = arr.rank;
              type eltType = arr.eltType;
              param arrayType = arr.type : string;
            }
            // Note: no calls; auto-instantiated.
            """

    expected_lens = (pos((0, 5)), 2)

    generic_inlays = []
    default_inlays = [
        (pos((1, 12)), "param value is 1", None),
        (pos((1, 12)), ": int(64)", None),
        (pos((2, 14)), ": int(64)", None),
        (
            pos((3, 17)),
            'param value is "[domain(1, int(64), strideKind.one)] int(64)"',
            None,
        ),
        (pos((3, 17)), ": string", None),
    ]
    all_inlays = [
        generic_inlays,
        default_inlays,
    ]

    await click_lenses_and_check_inlays(client, expected_lens, all_inlays, A=file)


@pytest.mark.asyncio
async def test_lenses_default_rect_where_2(client: LanguageClient):
    """
    Test that we can show default-rectangular substitutions for functions
    that are only generic because of their array formals.

    Here, the procedure has a 'where' clause that constrains the array
    to be a specific rank. The rank doesn't matches, and we're not smart
    enough to parse the 'where' clause to figure out to try something
    else. We should end with no lenses.
    """

    file = """
            proc foo(arr: [] int) where arr.rank == 2 {
              param rank = arr.rank;
              type eltType = arr.eltType;
              param arrayType = arr.type : string;
            }
            // Note: no calls; auto-instantiated.
            """

    async with source_file(client, file) as doc:
        actual_lenses = await check_generic_code_lenses(client, doc, [])
        assert len(actual_lenses.keys()) == 0


@pytest.mark.asyncio
async def test_lenses_default_rect_twoargs(client: LanguageClient):
    """
    Test that we can show default-rectangular substitutions for functions
    that are only generic because of their array formals.

    Here, there are two formals; they should both be instantiated.
    """

    file = """
            proc foo(arr1: [1..10] int, arr2: [1..10] int) {
              param rank = arr1.rank + arr2.rank;
              type eltType = (arr1.eltType, arr2.eltType);
              param arrayType = arr1.type : string;
            }
            // Note: no calls; auto-instantiated.
            """

    expected_lens = (pos((0, 5)), 2)

    generic_inlays = []
    default_inlays = [
        (pos((1, 12)), "param value is 2", None),
        (pos((1, 12)), ": int(64)", None),
        (pos((2, 14)), ": (int(64), int(64))", None),
        (
            pos((3, 17)),
            'param value is "[domain(1, int(64), strideKind.one)] int(64)"',
            None,
        ),
        (pos((3, 17)), ": string", None),
    ]
    all_inlays = [
        generic_inlays,
        default_inlays,
    ]

    await click_lenses_and_check_inlays(client, file, expected_lens, all_inlays)


@pytest.mark.asyncio
async def test_lenses_default_rect_other_args(client: LanguageClient):
    """
    Test that we can show default-rectangular substitutions for functions
    that are only generic because of their array formals.

    Here, there are additional concrete formals; they should not interfere
    with the instantiation of the array formal, and we should still show
    inlays the function.
    """

    file = """
            proc foo(arr: [] int, x: int, y: real, z: string) {
              param rank = arr.rank;
              type eltType = arr.eltType;
              param arrayType = arr.type : string;
            }
            // Note: no calls; auto-instantiated.
            """

    expected_lens = (pos((0, 5)), 2)

    generic_inlays = []
    default_inlays = [
        (pos((1, 12)), "param value is 1", None),
        (pos((1, 12)), ": int(64)", None),
        (pos((2, 14)), ": int(64)", None),
        (
            pos((3, 17)),
            'param value is "[domain(1, int(64), strideKind.one)] int(64)"',
            None,
        ),
        (pos((3, 17)), ": string", None),
    ]
    all_inlays = [
        generic_inlays,
        default_inlays,
    ]

    await click_lenses_and_check_inlays(client, file, expected_lens, all_inlays)


@pytest.mark.asyncio
async def test_lenses_default_rect_with_calls(client: LanguageClient):
    """
    Test that we can show default-rectangular substitutions for functions
    that are only generic because of their array formals.

    Here there _are_ calls to the function; they should be the same
    instantiation as the one produced with the default-rectangular code lens,
    and we should show the same inlays for all of them.
    """

    file = """
            proc foo(arr: [] int) {
              param rank = arr.rank;
              type eltType = arr.eltType;
              param arrayType = arr.type : string;
            }
            foo([1,2,3]);
            foo([4,5,6]);
            """

    expected_lens = (pos((0, 5)), 2)

    generic_inlays = []
    default_inlays = [
        (pos((1, 12)), "param value is 1", None),
        (pos((1, 12)), ": int(64)", None),
        (pos((2, 14)), ": int(64)", None),
        (
            pos((3, 17)),
            'param value is "[domain(1, int(64), strideKind.one)] int(64)"',
            None,
        ),
        (pos((3, 17)), ": string", None),
    ]
    all_inlays = [
        generic_inlays,
        default_inlays,
    ]

    await click_lenses_and_check_inlays(client, file, expected_lens, all_inlays)


@pytest.mark.asyncio
async def test_lenses_switch_crossfile(client: LanguageClient):
    """
    Test that using the code lens for generics switches the type, and that show
    generic code lens show up properly and work.
    """

    file1 = """
            module A {
              record R { type t = int; }

              proc foo(type t, param p = 1) param do
                return t == int && p == 2;
            }
            """
    file2 = """
            module B {
              use A;

              foo(R(real), false);
              foo(R);
              foo(R(?));
              foo(int, p=3);
            }
            """

    expected_lens = (pos((3, 7)), 5)

    generic_inlays = []
    R_real_inlays = [
        (pos((3, 17)), ": R(real(64))", None),
        (pos((3, 26)), "param value is false", None),
        (pos((3, 26)), ": bool", None),
    ]
    R_inlays = [
        (pos((3, 17)), ": R(int(64))", None),
        (pos((3, 26)), "param value is 1", None),
        (pos((3, 26)), ": int(64)", None),
    ]
    R_generic_inlays = [
        (pos((3, 17)), ": R", None),
        (pos((3, 26)), "param value is 1", None),
        (pos((3, 26)), ": int(64)", None),
    ]
    int_inlays = [
        (pos((3, 17)), ": int(64)", None),
        (pos((3, 26)), "param value is 3", None),
        (pos((3, 26)), ": int(64)", None),
    ]
    all_inlays = [
        generic_inlays,
        R_real_inlays,
        R_inlays,
        R_generic_inlays,
        int_inlays,
    ]

    await click_lenses_and_check_inlays(client, expected_lens, all_inlays, expected_lens_file="A", A=file1, B=file2)
