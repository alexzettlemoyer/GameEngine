#!/bin/bash

folder1="/home/ubuntu/game/src"
folder2="/home/ubuntu/game/tetris/src"

compare_folders() {
    local dir1="$1"
    local dir2="$2"

    total_lines=0
    diff_lines=0

    # Traverse through all files in the first folder and its subfolders
    while IFS= read -r -d '' file1; do
        # Get the corresponding file path in the second folder
        file2="${dir2}/${file1#$dir1/}"

        # Check if the file exists in both folders
        if [ -f "$file2" ]; then
            # Compare the files using sdiff and count differing lines
            diff_count=$(sdiff -B -b -s "$file1" "$file2" | wc -l | tr -d ' ')
            total_count=$(wc -l < "$file1")

            if [ "$total_count" -ne 0 ]; then
                total_lines=$((total_lines + total_count))
                diff_lines=$((diff_lines + diff_count))
            fi
        else
            total_count=$(wc -l < "$file1")
            total_lines=$((total_lines + total_count))
        fi
    done < <(find "$dir1" -type f -print0)

    # Calculate percentage difference
    if [ "$total_lines" -gt 0 ]; then
        percentage_diff=$(awk "BEGIN {printf \"%.2f\", ($diff_lines / $total_lines) * 100}")
    else
        percentage_diff="0.00"
    fi

    echo "Percentage Difference in $dir1 and $dir2: $percentage_diff%"
}

# Compare files in both folders and their subfolders recursively
compare_folders "$folder1" "$folder2"
compare_folders "$folder2" "$folder1"