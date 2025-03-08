#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

#!/usr/bin/env bats

# File: student_tests.sh

# Existing tests

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Pipe command: ls | grep .c" {
    run ./dsh <<EOF
ls | grep .c
EOF
    [[ "$output" == *".c"* ]]
    [ "$status" -eq 0 ]
}

@test "Multiple pipes: ls | grep .c | sort" {
    run ./dsh <<EOF
ls | grep .c | sort
EOF
    [[ "$output" == *".c"* ]]
    [ "$status" -eq 0 ]
}

@test "Pipe command with word count: ls | wc -l" {
    expectedOutput=$(ls | wc -l)  
    run ./dsh <<EOF
ls | wc -l
EOF
    cleanedOutput=$(echo "$output" | grep -o '^[0-9]\+')
    [ "$cleanedOutput" -eq "$expectedOutput" ]
    [ "$status" -eq 0 ]
}

@test "Pipe command with cat: echo 'hello' | cat" {
    run ./dsh <<EOF
echo "hello" | cat
EOF
    expectedOutput="hello"
    cleanedOutput=$(echo "$output" | sed -n '1p')  
    [ "$cleanedOutput" = "$expectedOutput" ]
    [ "$status" -eq 0 ]
}

@test "Exit command" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

@test "Background execution: sleep 2 &" {
    run ./dsh <<EOF
sleep 2 &
EOF
    echo "Output: $output"
    [ "$status" -eq 0 ]
}

@test "SIGINT (Ctrl+C) handling: sleep 5" {
    run timeout 2 ./dsh <<EOF
sleep 5
EOF
    [ "$status" -ne 0 ]  # Expecting it to be interrupted
}

@test "Handling empty command" {
    run ./dsh <<EOF

EOF
    [ "$status" -eq 0 ]
}

@test "Empty input command" {
    run ./dsh <<EOF

EOF
    [ "$status" -eq 0 ]
}

@test "Background execution" {
    run ./dsh <<EOF
sleep 3 &
EOF
    # Check for successful background execution
    [ "$status" -eq 0 ]
}