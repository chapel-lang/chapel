#!/bin/bash
# Auto-execute e4s-cl profile commands for Chapel/Arkouda dependencies
# This script runs the generate-e4s-cl-profile.sh, lets the user choose
# an e4s-cl profile (or create a new one), and executes all valid commands
# while avoiding duplicate binds in that profile.

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GENERATOR_SCRIPT="$SCRIPT_DIR/generate-e4s-cl-profile.sh"

if [ ! -x "$GENERATOR_SCRIPT" ]; then
    echo "Error: Generator script not found or not executable: $GENERATOR_SCRIPT"
    exit 1
fi

if ! command -v e4s-cl >/dev/null 2>&1; then
    echo "Error: e4s-cl not found in PATH. Load the appropriate module first."
    exit 1
fi

NON_INTERACTIVE=0
if [ "$1" = "--auto" ]; then
    NON_INTERACTIVE=1
fi

get_selected_profile_name() {
    # Try to get the currently selected profile name; return empty if none
    local name
    name=$(e4s-cl profile show 2>/dev/null | awk -F': ' '/^Profile name/ {print $2; exit}' || true)
    echo "$name"
}

select_or_create_profile() {
    local non_interactive="$1"
    local active name

    active=$(get_selected_profile_name)

    if [ -n "$active" ] && [ "$non_interactive" -eq 1 ]; then
        echo "Using currently selected e4s-cl profile: $active"
        PROFILE_NAME="$active"
        return
    fi

    if [ -n "$active" ]; then
        echo "Currently selected e4s-cl profile: $active"
        if [ "$non_interactive" -eq 0 ]; then
            read -p "Use this profile? (Y/n): " -r reply
            echo ""
            if [[ ! "$reply" =~ ^[Nn]$ ]]; then
                PROFILE_NAME="$active"
                return
            fi
        else
            PROFILE_NAME="$active"
            return
        fi
    else
        echo "No e4s-cl profile is currently selected."
    fi

    if [ "$non_interactive" -eq 1 ]; then
        echo "Error: No selected profile and --auto specified."
        echo "       Please run this script interactively once to create/select a profile."
        exit 1
    fi

    while true; do
        read -p "Enter profile name to use (existing will be selected, new will be created): " name
        echo ""
        if [ -n "$name" ]; then
            break
        fi
        echo "Profile name cannot be empty."
    done

    if e4s-cl profile show "$name" >/dev/null 2>&1; then
        echo "Selecting existing e4s-cl profile: $name"
    else
        echo "Creating new e4s-cl profile: $name"
        if ! e4s-cl profile create "$name"; then
            echo "Error: Failed to create profile '$name'." >&2
            exit 1
        fi
    fi

    if ! e4s-cl profile select "$name"; then
        echo "Error: Failed to select profile '$name'." >&2
        exit 1
    fi

    PROFILE_NAME="$name"
}

echo "=== Determining target e4s-cl profile for Chapel/Arkouda ==="
echo ""

PROFILE_NAME=""
select_or_create_profile "$NON_INTERACTIVE"

echo "Using e4s-cl profile: $PROFILE_NAME"
echo ""

echo "=== Generating e4s-cl profile edit commands for Chapel/Arkouda ==="
echo ""

# Check if e4s-cl is available
if ! command -v e4s-cl &> /dev/null; then
    echo "Warning: e4s-cl command not found. Please ensure E4S-CL is installed and available in PATH."
    exit 1
fi

# Check if an e4s-cl profile exists
if ! e4s-cl profile show &> /dev/null; then
    echo "Warning: No e4s-cl profile found or e4s-cl profile is not properly configured."
    echo "Please create and configure an e4s-cl profile first using:"
    echo "  e4s-cl profile create <profile-name>"
    echo "  e4s-cl profile select <profile-name>"
    exit 1
fi

# Generate commands and filter only the executable ones
COMMANDS=$($GENERATOR_SCRIPT | grep '^e4s-cl profile edit')

if [ -z "$COMMANDS" ]; then
    echo "No valid e4s-cl profile edit commands were generated."
    echo "This could mean:"
    echo "  - Required Chapel/Arkouda libraries are not available in the expected locations"
    echo "  - The generator script failed to find compatible library installations"
    echo "  - Libraries are installed but not in standard paths"
    echo ""
    echo "Please check:"
    echo "  1. Chapel installation and CHPL_HOME environment variable"
    echo "  2. Arkouda installation and dependencies"
    echo "  3. Library paths in the generator script"
    exit 1
fi

echo "Found $(echo "$COMMANDS" | wc -l) library/directory entries to consider."
echo ""

# Fetch existing bindings from the selected profile to avoid duplicate adds
EXISTING_LIBS=()
EXISTING_FILES=()

PROFILE_JSON=$(e4s-cl profile dump "$PROFILE_NAME" 2>/dev/null | awk 'BEGIN{found=0} /^[[:space:]]*[\[{]/{found=1} found{print}')

if [ -n "$PROFILE_JSON" ]; then
    if ! mapfile -t EXISTING_LIBS < <(printf '%s\n' "$PROFILE_JSON" | python3 -c '
import sys, json
text = sys.stdin.read().strip()
if not text:
    raise SystemExit(1)
data = json.loads(text)
if isinstance(data, list) and data:
    prof = data[0]
elif isinstance(data, dict):
    prof = data
else:
    prof = {}
for p in prof.get("libraries", []):
    print(p)
'); then
        EXISTING_LIBS=()
    fi

    if ! mapfile -t EXISTING_FILES < <(printf '%s\n' "$PROFILE_JSON" | python3 -c '
import sys, json
text = sys.stdin.read().strip()
if not text:
    raise SystemExit(1)
data = json.loads(text)
if isinstance(data, list) and data:
    prof = data[0]
elif isinstance(data, dict):
    prof = data
else:
    prof = {}
for p in prof.get("files", []):
    print(p)
'); then
        EXISTING_FILES=()
    fi
fi

path_in_list() {
    local needle="$1"; shift
    if [ "$#" -eq 0 ]; then
        return 1
    fi
    # Reason: avoid set -e exiting on a non-match from grep
    if printf '%s\n' "$@" | grep -Fxq "$needle"; then
        return 0
    fi
    return 1
}

show_next_steps() {
    echo ""
    echo "Next steps: Configure the container image and backend for this profile:"
    echo "  e4s-cl profile edit --image <path-to-container.sif> '$PROFILE_NAME'"
    echo "  e4s-cl profile edit --backend apptainer '$PROFILE_NAME'"
    echo ""
    echo "Example:"
    echo "  e4s-cl profile edit --image /path/to/chapel-arkouda.sif '$PROFILE_NAME'"
}

FILTERED_COMMANDS=()
NEW_LIBS=()
NEW_FILES=()
SKIP_COUNT=0

while IFS= read -r cmd; do
    # Determine what path this command is trying to add
    type=""
    if [[ "$cmd" == *"--add-libraries"* ]]; then
        type="lib"
    elif [[ "$cmd" == *"--add-files"* ]]; then
        type="file"
    fi

    path="$(echo "$cmd" | sed -E 's/.*"([^"]+)".*/\1/')"

    if [ -n "$type" ] && [ -n "$path" ]; then
        if [ "$type" = "lib" ]; then
            if path_in_list "$path" "${EXISTING_LIBS[@]}" "${NEW_LIBS[@]}"; then
                ((SKIP_COUNT++)) || true
                continue
            fi
            NEW_LIBS+=("$path")
        else
            if path_in_list "$path" "${EXISTING_FILES[@]}" "${NEW_FILES[@]}"; then
                ((SKIP_COUNT++)) || true
                continue
            fi
            NEW_FILES+=("$path")
        fi
    fi

    FILTERED_COMMANDS+=("$cmd")
done <<< "$COMMANDS"

if [ "${#FILTERED_COMMANDS[@]}" -eq 0 ]; then
    echo "No changes applied. All requested paths already exist in profile '$PROFILE_NAME'."
    echo "Skipped (already present): $SKIP_COUNT"
    echo "Nothing to add; verify with: e4s-cl profile show '$PROFILE_NAME'"
    show_next_steps
    exit 0
fi

# Ask for confirmation unless --auto flag is provided
if [ "$NON_INTERACTIVE" -eq 0 ]; then
    echo "Commands to execute (after skipping already-bound paths):"
    printf '%s\n' "${FILTERED_COMMANDS[@]}"
    echo ""
    read -p "Execute these commands? (y/N): " -n 1 -r
    echo ""
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        echo "Aborted."
        exit 0
    fi
fi

echo "Executing e4s-cl profile commands against profile: $PROFILE_NAME"
echo ""

# Execute only known-safe generated command shapes without eval.
run_generated_command() {
    local cmd="$1"
    local path

    if [[ "$cmd" =~ ^e4s-cl[[:space:]]+profile[[:space:]]+edit[[:space:]]+--add-libraries[[:space:]]+\"([^\"]+)\"$ ]]; then
        path="${BASH_REMATCH[1]}"
        e4s-cl profile edit --add-libraries "$path"
        return
    fi

    if [[ "$cmd" =~ ^e4s-cl[[:space:]]+profile[[:space:]]+edit[[:space:]]+--add-files[[:space:]]+\"([^\"]+)\"$ ]]; then
        path="${BASH_REMATCH[1]}"
        e4s-cl profile edit --add-files "$path"
        return
    fi

    echo "  [FAIL] Unsupported generated command format: $cmd" >&2
    return 2
}

# Execute each command with error handling
SUCCESS_COUNT=0
FAIL_COUNT=0

for cmd in "${FILTERED_COMMANDS[@]}"; do
    echo "Running: $cmd"
    if run_generated_command "$cmd"; then
        echo "  [OK] Success"
        ((SUCCESS_COUNT++)) || true
    else
        echo "  [FAIL] Failed (exit code: $?)"
        ((FAIL_COUNT++)) || true
    fi
    echo ""
done

echo "=== Summary for profile '$PROFILE_NAME' ==="
echo "Successful adds: $SUCCESS_COUNT"
echo "Skipped (already present): $SKIP_COUNT"
echo "Failed: $FAIL_COUNT"
echo ""

if [ $FAIL_COUNT -eq 0 ]; then
    echo "All requested libraries/files processed for e4s-cl profile '$PROFILE_NAME'."
    echo "Verify with: e4s-cl profile show '$PROFILE_NAME'"
    show_next_steps
else
    echo "Some commands failed. Check the output above for details."
    exit 1
fi
