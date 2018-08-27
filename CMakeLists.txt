cmake_minimum_required(VERSION 3.1)
project(wav2mp3)

#add source tree
set(sources src/lame_encoder.cpp src/mp3.cpp src/mymutex.cpp src/pcm.cpp src/thread.cpp src/wave_files.cpp src/wave_header.cpp lame/lame.cmake)
set(headers src/lame_encoder.h src/lame_encoder_exception.h src/lock_guard.h src/monitor.h src/mp3.h src/mp3_encoder.h src/mymutex.h src/path.h src/pcm.h src/thread.h src/wave_files.h src/wave_format_exception.h src/wave_header.h)
set(tests src/monitor.test.cpp src/mymutex.test.cpp src/thread.test.cpp gtest/gtest.cmake)

#add external library
include(gtest/gtest.cmake)
include(lame/lame.cmake)

#config external library in different platforms
if(CMAKE_HOST_WIN32)
  include(pthreads-win32/pthreads-win32.cmake)
  list(APPEND sources pthreads-win32/pthreads-win32.cmake)
  set(libraries lame pthreads-win32)
elseif(CMAKE_HOST_APPLE)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  link_directories(/usr/local/opt/llvm/lib)
  #set(libraries lame Threads::Threads stdc++fs c++experimental)
  set(libraries lame Threads::Threads c++experimental)
  set(CMAKE_C_COMPILER /usr/local/opt/llvm/bin/clang)
  set(CMAKE_CXX_COMPILER /usr/local/opt/llvm/bin/clang++)
else() #other Unix-lile or Linux
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  set(libraries lame Threads::Threads stdc++fs)
endif()

add_executable(${PROJECT_NAME} ${sources} ${headers} src/main.cpp)
add_executable(${PROJECT_NAME}-unit-tests ${sources} ${headers} ${tests} src/main.test.cpp)

target_link_libraries(${PROJECT_NAME} ${libraries})
target_link_libraries(${PROJECT_NAME}-unit-tests gtest ${libraries})

if(CMAKE_HOST_APPLE)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
  set_property(TARGET ${PROJECT_NAME}-unit-tests PROPERTY CXX_STANDARD 14)
else()
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
  set_property(TARGET ${PROJECT_NAME}-unit-tests PROPERTY CXX_STANDARD 14)
endif()

if(CMAKE_HOST_WIN32)
  get_property(pthreads-win32-location TARGET pthreads-win32 PROPERTY LOCATION)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${pthreads-win32-location}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
  add_custom_command(TARGET ${PROJECT_NAME}-unit-tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${pthreads-win32-location}" "$<TARGET_FILE_DIR:${PROJECT_NAME}-unit-tests>")
endif()

#Prepare Test Examole
foreach(channels m s)
  foreach(samplerate 08 11 22 44)
    foreach(bitspersample 08 16)
	  set(file pcm${bitspersample}${samplerate}${channels}.wav)
      if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/samples/${file}")
        message(STATUS "Downloading sample file: ${file}")
        file(DOWNLOAD http://www.mauvecloud.net/sounds/${file} "${CMAKE_CURRENT_BINARY_DIR}/samples/${file}" SHOW_PROGRESS)
      endif()
    endforeach()
  endforeach()
endforeach()

enable_testing()
add_test(${PROJECT_NAME} ${PROJECT_NAME} "${CMAKE_CURRENT_BINARY_DIR}/samples")
add_test(${PROJECT_NAME}-unit-tests ${PROJECT_NAME}-unit-tests)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
source_group("External project Files" REGULAR_EXPRESSION "[.]cmake$")
