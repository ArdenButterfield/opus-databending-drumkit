# "GLOBS ARE BAD" is brittle and silly dev UX, sorry CMake!
file(GLOB_RECURSE ExplorationFiles CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/exploration/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/exploration/*.h")

# Organize the test source in the Tests/ folder in Xcode
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/exploration PREFIX "" FILES ${ExplorationFiles})

# Use Catch2 v3 on the devel branch
Include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
    GIT_TAG v3.4.0)
FetchContent_MakeAvailable(Catch2) # find_package equivalent

# Setup the test executable, again C++20 please
add_executable(Exploration ${ExplorationFiles})
target_compile_features(Exploration PRIVATE cxx_std_20)

# Our test executable also wants to know about our plugin code...
target_include_directories(Exploration PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

# Copy over compile definitions from our plugin target so it has all the JUCEy goodness
target_compile_definitions(Exploration PRIVATE $<TARGET_PROPERTY:${PROJECT_NAME},COMPILE_DEFINITIONS>)

# And give tests access to our shared code
target_link_libraries(Exploration PRIVATE SharedCode Catch2::Catch2WithMain)

# Make an Xcode Scheme for the test executable so we can run tests in the IDE
set_target_properties(Exploration PROPERTIES XCODE_GENERATE_SCHEME ON)

# When running Tests we have specific needs
target_compile_definitions(Exploration PUBLIC
    JUCE_MODAL_LOOPS_PERMITTED=1 # let us run Message Manager in tests
    RUN_PAMPLEJUCE_TESTS=1 # also run tests in other module .cpp files guarded by RUN_PAMPLEJUCE_TESTS
)

# Load and use the .cmake file provided by Catch2
# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md
# We have to manually provide the source directory here for now
include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)
catch_discover_tests(Exploration)
