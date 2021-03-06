
set(dspiolib_SRCS
  dspcontrol.cc
  acqdatasource.cc
  tspikesink.cc
  wavesink.cc
  eventcodec.cc
  eventpreddisp.cc
  mockdspboard.cc
  logging.cc
 )

set(somadspio_HEADERS
  dspcontrol.h eventcodec.h const.h types.h
  acqdatasource.h tspikesink.h wavesink.h eventpreddisp.h
  logging.h mockdspboard.h
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SIGC++_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})


set(DSPSRCDIR dspboard)
set(DSPSRCHWDIR ${DSPSRCDIR}/host)

include_directories(${DSPSRCDIR})
include_directories(${DSPSRCHWDIR})

add_library(mockdsp
  mockdspboard.cc
  ${DSPSRCDIR}/systemtimer.cc 
  ${DSPSRCDIR}/benchmark.cc 
  ${DSPSRCDIR}/echoproc.cc 
  ${DSPSRCDIR}/audiomon.cc 
  ${DSPSRCHWDIR}/hostdataout.cc 
  ${DSPSRCDIR}/eventdispatch.cc 
  ${DSPSRCHWDIR}/hw/eventtx.cc 
  ${DSPSRCHWDIR}/hw/acqserial.cc 
  ${DSPSRCDIR}/sinks/tspikesink.cc 
  ${DSPSRCDIR}/sinks/wavesink.cc 
  ${DSPSRCHWDIR}/filter.cc 
  ${DSPSRCDIR}/filterlinks/availablefirs.cc 
  ${DSPSRCDIR}/mainloops/rawmainloop.cc 
  ${DSPSRCDIR}/filterlinkcontroller.cc 
  ${DSPSRCDIR}/filterlinks/fir.cc 
  ${DSPSRCDIR}/acqstatecontrol.cc 
  ${DSPSRCDIR}/acqdatasourcecontrol.cc 
  ${DSPSRCDIR}/acqdatasource.cc 
  ${DSPSRCDIR}/audiomon.cc 
  ${DSPSRCDIR}/eventutil.cc 
  ${DSPSRCDIR}/acqboardif.cc 
  ${DSPSRCDIR}/sinks/rawsink.cc 
  ${DSPSRCDIR}/mainloops/somamainloop.cc)

link_directories(
  ${Boost_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

ADD_LIBRARY(${PROJECT_NAME} SHARED
  ${dspiolib_SRCS})

target_link_libraries(${PROJECT_NAME} 
  mockdsp
  ${Boost_LIBRARIES} 
  ${SIGC++_LIBRARIES})


ADD_LIBRARY(${PROJECT_NAME}_test
  ${dspiolib_SRCS})

target_link_libraries(${PROJECT_NAME}_test
  ${Boost_LIBRARIES} 
  ${SIGC++_LIBRARIES})

SET_TARGET_PROPERTIES(${PROJECT_NAME}
  PROPERTIES VERSION ${LIBRARY_VERSION}
  SOVERSION ${LIBRARY_SOVERSION})

install( TARGETS ${PROJECT_NAME} LIBRARY
  DESTINATION lib)

install(FILES ${somadspio_HEADERS}
  DESTINATION
  include/${PROJECT_NAME}-${LIBRARY_VERSION}/${PROJECT_NAME})

# this is a horrible hack to get these to install in the 
# correct location

install(FILES dspboard/event.h dspboard/acqboardif.h dspboard/host/types.h 
  DESTINATION
  include/${PROJECT_NAME}-${LIBRARY_VERSION}/${PROJECT_NAME}/dspboard/)

install(FILES dspboard/host/hw/acqserial.h 
  DESTINATION
  include/${PROJECT_NAME}-${LIBRARY_VERSION}/${PROJECT_NAME}/dspboard/)


