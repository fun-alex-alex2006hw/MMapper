INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/../src/clock
	${CMAKE_CURRENT_SOURCE_DIR}/../src/expandoracommon
	${CMAKE_CURRENT_SOURCE_DIR}/../src/parser
)

# Clock
SET(clock_SRCS ../src/clock/mumeclock.cpp ../src/clock/mumemoment.cpp)
SET(TestClock_SRCS testclock.cpp)
ADD_EXECUTABLE(TestClock ${TestClock_SRCS} ${clock_SRCS})
TARGET_LINK_LIBRARIES(TestClock Qt5::Test)
ADD_TEST(NAME TestClock COMMAND TestClock)

# Expandora
FILE(GLOB_RECURSE expandoracommon_SRCS ../src/expandoracommon/*.cpp)
SET(TestExpandoraCommon_SRCS testexpandoracommon.cpp)
ADD_EXECUTABLE(TestExpandoraCommon ${TestExpandoraCommon_SRCS} ${expandoracommon_SRCS})
TARGET_LINK_LIBRARIES(TestExpandoraCommon Qt5::Test)
ADD_TEST(NAME TestExpandoraCommon COMMAND TestExpandoraCommon)

# Parser
SET(parser_SRCS ../src/parser/parserutils.cpp)
SET(TestParser_SRCS testparser.cpp)
ADD_EXECUTABLE(TestParser ${TestParser_SRCS} ${parser_SRCS})
TARGET_LINK_LIBRARIES(TestParser Qt5::Test)
ADD_TEST(NAME TestParser COMMAND TestParser)
