#!/bin/bash

# geninfo obj/*.gcno -o out
lcov -d obj/ --capture -o ./code_coverage.info -rc lcov_branch_coverage=1
genhtml code_coverage.info --branch-coverage --output-directory ./code_coverage_report/