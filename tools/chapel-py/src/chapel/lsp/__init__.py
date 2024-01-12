from lsprotocol.types import Position, Range


def location_to_range(location) -> Range:
    """
    Convert a Chapel location into a lsprotocol.types Range, which is
    used for e.g. reporting diagnostics.
    """

    start = location.start()
    end = location.end()
    # NOTE: we want the char number to be one extra, otherwise single character identifiers don't work right
    return Range(
        start=Position(start[0] - 1, start[1] - 1), end=Position(end[0] - 1, end[1])
    )
