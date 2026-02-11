#
# Copyright 2024-2026 Hewlett Packard Enterprise Development LP
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

"""
=============
LSP Utilities
=============

This module contains utility functions for working with Chapel compiler types
and the Language Server Protocol.
"""

from lsprotocol.types import (
    Position,
    Range,
    Diagnostic,
    DiagnosticSeverity,
)
import typing
import chapel


def location_to_range(location) -> Range:
    """
    Convert a Chapel location into a lsprotocol.types Range, which is
    used for e.g. reporting diagnostics.
    """

    start = location.start()
    end = location.end()
    return Range(
        start=Position(max(start[0] - 1, 0), max(start[1] - 1, 0)),
        end=Position(max(0, end[0] - 1), max(end[1] - 1, 0)),
    )


def error_to_location_and_info(
    error,
) -> typing.Tuple[
    chapel.Location,
    typing.Optional[str],
]:
    location = error.location()
    type_ = error.type()
    new_message = None
    if isinstance(error, chapel.NoMatchingCandidates):
        (call, call_info, app_results, _) = error.info()

        # Check if all candidates were rejected due to a particular candidate.
        # In that case, highlight the specific candidate as the location of the error.
        actuals_set = set()

        # question args violate the correspondence between indices into the
        # call and indices into the call_info. The former are needed to get
        # an AST for the location, while the latter are provided by
        # app_results. Thus, do not include the indexing.
        if call_info.has_question_arg():
            return (location, new_message)

        for app_result in app_results:
            idx = app_result.actual_idx()

            # 'this' formals are inserted into the call info but aren't in the
            # call's actual list, so the reported indices are one higher than
            # we should use to index into the call's actuals.
            if app_result.candidate_is_method():
                idx -= 1

            if app_result.candidate_failure_reason() != "FAIL_CANNOT_PASS":
                idx = -1

            actuals_set.add(idx)

        if (
            len(actuals_set) == 1
            and -1 not in actuals_set
            and isinstance(call, chapel.FnCall)
        ):
            # All candidates were rejected due to the same candidate, so highlight that candidate.
            location = call.actual(actuals_set.pop()).location()
            assert type_ is not None
            new_message = "{}: [{}]: this actual could not be passed to a corresponding formal".format(
                error.kind().capitalize(), type_
            )
    elif isinstance(error, chapel.AsWithUseExcept):
        (_, as_) = error.info()
        if as_ is not None:
            location = as_.location()
    elif isinstance(error, chapel.TertiaryUseImportUnstable):
        (_, node, _, _, _) = error.info()
        if node is not None:
            location = node.location()
    elif isinstance(error, chapel.TupleExpansionNamedArgs):
        (tup, _) = error.info()
        if tup is not None:
            location = tup.location()
    elif isinstance(error, chapel.TupleExpansionNonTuple):
        (_, tup, _) = error.info()
        if tup is not None:
            location = tup.location()
    elif isinstance(error, chapel.MultipleQuestionArgs):
        (_, arg1, arg2) = error.info()
        assert arg1 is not None and arg2 is not None

        # TODO: it would be nice to use additional related info to highlight
        # both arguments, but we currently don't assume a URI encoding
        # scheme so can't construct a Position := Tuple[URI, Range] for the related info.
        location = arg2.location()

    return (location, new_message)


def error_to_diagnostic(error) -> Diagnostic:
    """
    Convert a Chapel error into a lsprotocol.types Diagnostic
    """
    kind_to_severity = {
        "error": DiagnosticSeverity.Error,
        "syntax": DiagnosticSeverity.Error,
        "note": DiagnosticSeverity.Information,
        "warning": DiagnosticSeverity.Warning,
    }

    type_ = error.type()
    if type_ is not None:
        message = "{}: [{}]: {}".format(
            error.kind().capitalize(), type_, error.message()
        )
    else:
        message = "{}: {}".format(error.kind().capitalize(), error.message())

    (location, new_message) = error_to_location_and_info(error)
    related_info = None
    if new_message is not None:
        message = new_message

    diagnostic = Diagnostic(
        range=location_to_range(location),
        message=message,
        severity=kind_to_severity[error.kind()],
        related_information=related_info,
    )
    return diagnostic
