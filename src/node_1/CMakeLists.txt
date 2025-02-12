cmake_minimum_required(VERSION 3.8)
project(node_1)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#set(CMAKE_PREFIX_PATH "/usr/include" ${CMAKE_PREFIX_PATH})
set(CMAKE_PREFIX_PATH "~/program/opencv_first_try/install" ${CMAKE_PREFIX_PATH})

# find dependencies
find_package(ament_cmake_auto REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
ament_auto_find_build_dependencies()

include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(include ${OpenCV_INCLUDE_DIRS})
include_directories(include)

ament_auto_add_executable(${PROJECT_NAME}_1
        src/node_1.cpp
)
target_link_libraries(${PROJECT_NAME}_1 ${OpenCV_LIBS})

ament_auto_add_executable(${PROJECT_NAME}_2
        src/node_2.cpp
)
target_link_libraries(${PROJECT_NAME}_2 ${OpenCV_LIBS})

ament_auto_add_executable(${PROJECT_NAME}_3
        src/node_3.cpp
)
target_link_libraries(${PROJECT_NAME}_3 ${OpenCV_LIBS})

ament_auto_add_executable(${PROJECT_NAME}_4
        src/node_4.cpp
)
target_link_libraries(${PROJECT_NAME}_4 ${OpenCV_LIBS})

ament_auto_add_executable(${PROJECT_NAME}_5
        src/node_5.cpp
)
target_link_libraries(${PROJECT_NAME}_5 ${OpenCV_LIBS})

ament_auto_add_executable(${PROJECT_NAME}_6
        src/node_6.cpp
)
target_link_libraries(${PROJECT_NAME}_6 ${OpenCV_LIBS})

ament_auto_add_executable(${PROJECT_NAME}_7
        src/node_7.cpp
)
target_link_libraries(${PROJECT_NAME}_7 ${OpenCV_LIBS})

ament_auto_add_executable(${PROJECT_NAME}_8
        src/node_8.cpp
)
target_link_libraries(${PROJECT_NAME}_8 ${OpenCV_LIBS})

ament_auto_add_executable(${PROJECT_NAME}_try
        src/try_1.cpp
)
target_link_libraries(${PROJECT_NAME}_try ${OpenCV_LIBS})

ament_auto_add_executable(${PROJECT_NAME}_program_1
        src/program_1.cpp
)
target_link_libraries(${PROJECT_NAME}_program_1 ${OpenCV_LIBS})

ament_auto_add_executable(${PROJECT_NAME}_test_1
        src/test_1.cpp
)
target_link_libraries(${PROJECT_NAME}_test_1 ${OpenCV_LIBS})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_auto_package()
