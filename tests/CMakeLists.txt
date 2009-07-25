
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SIGC++_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})


set(DSPSRCDIR ../ext-dspboard/src)
set(DSPSRCHWDIR ${DSPSRCDIR}/host)

include_directories(${DSPSRCDIR})
include_directories(${DSPSRCHWDIR})

link_directories(
  ${Boost_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

add_executable(test_simple
  test_simple.cc
  runner.cc
)

add_executable(test_acqdatasource
  test_acqdatasource.cc
  runner.cc)

add_executable(test_eventdispatch
  test_eventdispatch.cc
  runner.cc)

set(testlist
  test_simple
  test_acqdatasource
  test_eventdispatch)

foreach(testname ${testlist})
    target_link_libraries(${testname}
      mockdsp
      ${PROJECT_NAME}_test
      somanetwork)
  
  ADD_TEST(${testname} ${testname})

endforeach(testname)


add_executable(runtest
  runtest.cc
)

target_link_libraries(runtest
  ${PROJECT_NAME}_test

  somanetwork)

