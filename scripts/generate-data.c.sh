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

normalize_path() {
    local path="${1##*"$PROJECT_NAME"}"
    printf "%s" "$path"
}

quotes() {
    local data_dir_path="$1"
    local data_h_path="$2"
    shopt -s nullglob

    for file in "$project_path"/data/*quote*.txt; do
        local data_name="${file##*/}"; data_name="${data_name%%.txt*}"
        local var_name="${data_name}_data"
        local c_file="$data_dir_path/$data_name.c"
        {
            printf "%s\n\n" '#include "data.h"'
            printf "const mcmt_Quote %s[] = {\n" "$var_name"

            while IFS= read -r line || [[ -n "$line" ]]; do
                line="${line//\"/\\\"}"
                printf "    {\"%s\"},\n" "${line/;;;/\", \"}"
            done < "$file"

            printf "};"
        } > "$c_file"
        printf "extern const mcmt_Quote %s[];\n\n" "$var_name" >> "$data_h_path"
        printf "Added %s contents to %s\n" "$(normalize_path "$file")" "$(normalize_path "$c_file")"
    done
}

words() {
    local data_dir_path="$1"
    local data_h_path="$2"

    for file in "$project_path"/data/*.txt; do
        if [[ $file =~ quote ]]; then continue; fi

        local data_name="${file##*/}"; data_name="${data_name%%.txt*}"
        local var_name="${data_name}_data"
        local c_file="$data_dir_path/$data_name.c"
        {
            printf "%s\n\n" '#include "data.h"'
            printf "const int n%s = %s;\n" "$var_name" "$(wc -l "$file" | cut -d ' ' -f1)"
            printf "const char* %s[] = {\n" "$var_name"

            while IFS= read -r line || [[ -n "$line" ]]; do
                printf "    \"%s\",\n" "$line"
            done < "$file"

            printf "};"
        } > "$c_file"
        printf "extern const int n%s;\n" "$var_name" >> "$data_h_path"
        printf "extern const char* %s[];\n\n" "$var_name" >> "$data_h_path"
        printf "Added %s contents to %s\n" "$(normalize_path "$file")" "$(normalize_path "$c_file")"
    done
}

main() {
    local project_path="$(get_project_path)"
    local data_dir_path="$(find "$project_path/src" -path '*data')"
    local data_h_path="$(find "$project_path/src" -path '*data*' -name 'data.h')"

    {
        printf "%s\n" "#ifndef MCMT_DATA_H"
        printf "%s\n\n" "#define MCMT_DATA_H"
        printf "%s\n" "typedef struct {"
        printf "    %s\n" "char* quote;"
        printf "    %s\n" "char* source;"
        printf "%s\n\n" "} mcmt_Quote;"
    } > "$data_h_path"

    quotes "$data_dir_path" "$data_h_path"
    words "$data_dir_path" "$data_h_path"

    printf "%s" "#endif //MCMT_DATA_H" >> "$data_h_path"
}

main
