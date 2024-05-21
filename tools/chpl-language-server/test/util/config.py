import functools
import os


@functools.lru_cache(maxsize=None)
def CHPL_HOME():
    chpl_home = os.environ.get("CHPL_HOME")
    if not chpl_home:
        raise ValueError(
            "The Language Server tests require the CHPL_HOME environment variable to be set."
        )
    return chpl_home


@functools.lru_cache(maxsize=None)
def CLS_PATH():
    return os.path.join(
        CHPL_HOME(),
        "tools",
        "chpl-language-server",
        "src",
        "chpl-language-server.py",
    )
