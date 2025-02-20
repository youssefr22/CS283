#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

# Test 1: Simple command
@test "Run ls without errors" {
    run ./dsh <<EOF
ls
EOF
    [ "$status" -eq 0 ]
}

# Test 2: Command with arguments
@test "Run ls with -l flag" {
    run ./dsh <<EOF
ls -l
EOF
    [ "$status" -eq 0 ]
}

# Test 3: Quoted argument with spaces
@test "Echo with quoted spaces" {
    run ./dsh <<EOF
echo "hello   world"
EOF
    [[ "$output" == *"hello   world"* ]]
    [ "$status" -eq 0 ]
}

# Test 4: Multiple spaces between arguments
@test "Multiple spaces between arguments" {
    run ./dsh <<EOF
echo    hello        world
EOF
    [[ "$output" == *"hello world"* ]]
    [ "$status" -eq 0 ]
}

# Test 5: Empty input
@test "Empty input should not crash" {
    run ./dsh <<EOF

EOF
    [ "$status" -eq 0 ]
}

# Test 6: Change directory and print it
@test "Change directory and check pwd" {
    current=$(pwd)
    run ./dsh <<EOF
cd /tmp
pwd
EOF
    [[ "$output" == *"/tmp"* ]]
    [ "$status" -eq 0 ]
    cd "$current" # Restore working directory
}

# Test 7: Echo empty string
@test "Echo empty string" {
    run ./dsh <<EOF
echo ""
EOF
    [[ "$output" == *""* ]] # Expect an empty line or nothing after prompt
    [ "$status" -eq 0 ]
}

# Test 8: Handling tab characters in input
@test "Echo with tabs" {
    run ./dsh <<EOF
echo "hello   	world"
EOF
    [[ "$output" == *"hello   	world"* ]]
    [ "$status" -eq 0 ]
}
