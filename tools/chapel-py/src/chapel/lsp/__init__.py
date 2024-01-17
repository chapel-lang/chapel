#
# Copyright 2024-2024 Hewlett Packard Enterprise Development LP
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

from lsprotocol.types import Position, Range, Diagnostic, DiagnosticSeverity


def location_to_range(location) -> Range:
    """
    Convert a Chapel location into a lsprotocol.types Range, which is
    used for e.g. reporting diagnostics.
    """

    start = location.start()
    end = location.end()
    # NOTE: we want the char number to be one extra, otherwise single character identifiers don't work right
    return Range(
        start=Position(start[0] - 1, start[1] - 1),
        end=Position(end[0] - 1, end[1]),
    )

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

    diagnostic = Diagnostic(
        range=location_to_range(error.location()),
        message="{}: [{}]: {}".format(
            error.kind().capitalize(), error.type(), error.message()
        ),
        severity=kind_to_severity[error.kind()],
    )
    return diagnostic
