cmake_minimum_required(VERSION 3.5)
project(proyecto)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/lock_file/LockFile.cpp
        src/lock_file/LockFile.h
        src/logger/Log.cpp
        src/logger/Log.h
        src/msgqueue/MsgQueue.h
        src/signals/EventHandler.h
        src/signals/SIGINT_Handler.h
        src/signals/SignalHandler.cpp
        src/signals/SignalHandler.h)

set(CLIENT_FILES
        src/client/main.cpp)

set(MANAGER_FILES
        src/manager/main.cpp src/manager/Database.cpp src/manager/Database.h src/record.h src/request.h)

add_executable(manager ${SOURCE_FILES} ${MANAGER_FILES})
add_executable(client ${SOURCE_FILES} ${CLIENT_FILES})