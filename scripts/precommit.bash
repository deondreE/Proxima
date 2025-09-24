#!/bin/bash

# This script is used as a pre-commit hook to automatically format code
# using clang-format based on a .clang-format file.

# --- Configuration ---
# Specify the path to clang-format executable if it's not in your PATH
CLANG_FORMAT_CMD="clang-format"

# Specify the types of files to format
FILE_PATTERNS="*.cpp *.hpp *.h *.c"

# Directory to start searching for files (usually project root)
SEARCH_DIR="." 

# --- Script Logic ---

echo "Running clang-format for staged files..."

# Find all files matching patterns recursively from SEARCH_DIR
find "$SEARCH_DIR" -type f \( $(echo $FILE_PATTERNS | sed 's/ / -o -name /g; s/^/ -name /') \) > files_to_format.txt

if [ ! -s files_to_format.txt ]; then
    echo "No files found to format."
    exit 0
fi

FORMATTED_FILES=""
HAS_CHANGES=false

# Loop through each file and format it
while IFS= read -r file; do
    if [ -f "$file" ]; then
        # Check if clang-format exists
        if ! command -v "$CLANG_FORMAT_CMD" &> /dev/null; then
            echo "ERROR: clang-format command not found. Please install clang-format or set CLANG_FORMAT_CMD."
            exit 1
        fi

        # Run clang-format -i (in-place formatting)
        # We check if it modified the file afterwards.
        # The output of clang-format itself (warnings/errors) can be captured if needed.
        if "$CLANG_FORMAT_CMD" -i "$file"; then
            # Check if the file was actually changed
            if git diff --quiet "$file"; then
                # File was NOT changed by clang-format
                : # No-op
            else
                # File WAS changed by clang-format
                echo "Formatted: $file"
                FORMATTED_FILES="$FORMATTED_FILES\n$file"
                HAS_CHANGES=true
            fi
        else
            echo "ERROR: clang-format failed for $file"
            # Decide if you want to exit or continue on error
            # exit 1 
        fi
    fi
done < files_to_format.txt

# Clean up the temporary file list
rm files_to_format.txt

if [ "$HAS_CHANGES" = true ]; then
    echo ""
    echo "--- clang-format detected changes. Staging them. ---"
    echo -e "$FORMATTED_FILES"
    
    # Add the formatted files to the staging area
    # git add --update : adds changes from the index, or staged files if index is empty
    # The following command should stage all modified files by clang-format
    git add $FORMATTED_FILES
    
    echo ""
    echo "Files staged for commit. Please review and commit."
    exit 1 # Exit with 1 to indicate that the commit is being paused for review
else
    echo "No files needed formatting."
    exit 0 # Exit cleanly, no changes made
fi