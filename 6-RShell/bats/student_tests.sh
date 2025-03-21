#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Exec Local Command Loop - Basic Execution" {
    run ./dsh <<EOF
ls
EOF
    [ "$status" -eq 0 ]
}

@test "Exec Local Command Loop - Exit Command" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

@test "Execute Multiple Commands" {
    run ./dsh <<EOF
echo first
echo second
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"first"* ]]
    [[ "$output" == *"second"* ]]
}

@test "Handle Long Output" {
    run ./dsh <<EOF
seq 1 1000
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"1000"* ]]
}

@test "Execute Background Command" {
    run ./dsh <<EOF
sleep 1 &
EOF
    [ "$status" -eq 0 ]
}

@test "Execute Multiple Background Commands" {
    run ./dsh <<EOF
sleep 2 & sleep 3 &
EOF
    [ "$status" -eq 0 ]
}

@test "Execute Chained Commands with AND" {
    run ./dsh <<EOF
echo First && echo Second
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"First"* ]]
    [[ "$output" == *"Second"* ]]
}

@test "File Append Redirection" {
    echo "First Line" > testfile
    run ./dsh <<EOF
echo Second Line >> testfile
cat testfile
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"First Line"* ]]
    [[ "$output" == *"Second Line"* ]]
}