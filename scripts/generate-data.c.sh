#!/usr/bin/env bash

#This script takes each data/*.txt file and converts it, line-by-line,
#to an array of char* values in the data.c file in src. Each array is
#the filename (remove .txt) followed by _data (i.e. 'quotes_data').

source "$(dirname "$0")/globals.sh"

get_project_path() {
    local path="$(pwd)"
    path="${path%"$PROJECT_NAME"*}$PROJECT_NAME"
    printf "%s" "$path"
}

main() {
    #Download and build data/*.txt files
    ./"$project_path"/scripts/build-data-source-files.py

    #Fill src/**/data.c with text file strings
    local project_path="$(get_project_path)"
    local data_c_file="$(find "$project_path/src" -path '*data*' -name '*.c')"

    for file in "$project_path"/data/*.txt; do
        local var_name="${file##*/}"; var_name="${var_name%%.txt*}_data"
        printf "const char* const %s[] = {\n" "$var_name" > "$data_c_file"

        while IFS= read -r line || [[ -n "$line" ]]; do
            line="${line//\"/\\\"}"
            printf "    \"%s\",\n" "$line" >> "$data_c_file"
        done < "$file"

        printf "};" >> "$data_c_file"
    done
}

main
