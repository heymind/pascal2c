# If pkg-config is not installed on the system, then the CHECK_INSTALL_DIR var
# must be set to the install location of Check. For example, on Windows, this
# may be: C:/Program Files/check
# set(CHECK_INSTALL_DIR "C:/Program Files/check")

find_package(Check REQUIRED)

include_directories(${CHECK_INCLUDE_DIRS})
include_directories(../src)
link_directories(${CHECK_LIBRARY_DIRS})


# Since Check uses Threads to paralelize the tests, it's mandatory
# add pthread as a dependency, alongside the Check libraries.
add_executable(test_ast test_ast.c ../src/ast.c ../src/ast.h ../3rd_parties/sds.c)
target_link_libraries(test_ast ${CHECK_LIBRARIES} pthread)

# Create testing target and redirect its output to `Testing` folder
add_test(NAME test_sample COMMAND test_sample WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(test_sample PROPERTIES TIMEOUT 30)

