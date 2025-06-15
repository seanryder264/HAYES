#!/bin/bash

# This script runs the testbench
# Usage: ./doit.sh <file1.cpp> <file2.cpp>

# === Constants ===
SCRIPT_DIR=$(dirname "$(realpath "$0")")
TEST_FOLDER=$(realpath "$SCRIPT_DIR/tests")
RTL_FOLDER=$(realpath "$SCRIPT_DIR/../rtl")
GREEN=$(tput setaf 2)
RED=$(tput setaf 1)
YELLOW=$(tput setaf 3)
RESET=$(tput sgr0)

# === Counters ===
passes=0
fails=0

# === Collect test files ===
if [[ $# -eq 0 ]]; then
    files=("$TEST_FOLDER"/*.cpp)
else
    for arg in "$@"; do
        if [[ -f "$arg" ]]; then
            files+=("$arg")
        elif [[ -f "$TEST_FOLDER/$arg" ]]; then
            files+=("$TEST_FOLDER/$arg")
        else
            echo "${RED}Could not find file: $arg or $TEST_FOLDER/$arg${RESET}"
        fi
    done
fi

cd "$SCRIPT_DIR" || exit 1

# === Clean old output ===
rm -rf test_out/*
mkdir -p test_out

# === Iterate through tests ===
for file in "${files[@]}"; do
    echo "${YELLOW}Testing: $file${RESET}"
    
    if [[ ! -f "$file" ]]; then
        echo "${RED}File not found: $file${RESET}"
        ((fails++))
        continue
    fi

    name=$(basename "$file" _tb.cpp | cut -f1 -d-)
    
    # Special case override
    [[ "$name" == "wrapper.cpp" ]] && name="pixel_generator"

    python3 "$RTL_FOLDER/log_lut_gen.py" "$RTL_FOLDER/log_lut.mem"
    python3 "$RTL_FOLDER/atan_lut_gen.py" "$RTL_FOLDER/atan_lut.mem"

    # Compile with Verilator
    verilator -Wall -sv -trace \
        -cc "$RTL_FOLDER/$name.v" \
        --exe "$file" \
        -y "$RTL_FOLDER" \
        --prefix "Vdut" \
        -o Vdut \
        -LDFLAGS "-lgtest -lgtest_main -lpthread"

    if [[ $? -ne 0 ]]; then
        echo "${RED}❌ Verilation failed for $name${RESET}"
        ((fails++))
        continue
    fi

    make -j -C obj_dir/ -f Vdut.mk
    if [[ $? -ne 0 ]]; then
        echo "${RED}❌ Build failed for $name${RESET}"
        ((fails++))
        continue
    fi

    cp "$RTL_FOLDER/log_lut.mem" obj_dir/
    cp "$RTL_FOLDER/atan_lut.mem" obj_dir/

    # Run simulation from inside obj_dir so $readmemb("log_lut.mem") works
    cd obj_dir/
    ./Vdut
    cd ..

    if [[ $? -eq 0 ]]; then
        echo "${GREEN}✅ Passed${RESET}"
        ((passes++))
    else
        echo "${RED}❌ Failed${RESET}"
        ((fails++))
    fi
done

# === Final stats ===
mv obj_dir test_out/
echo -e "\n${GREEN}Passed: $passes${RESET} | ${RED}Failed: $fails${RESET}"
