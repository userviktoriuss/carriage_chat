cmake_minimum_required(VERSION 3.25)
project(CarriageChat LANGUAGES CXX)

add_executable(
        CarriageChatClient
        ClientApp/ClientApp.cpp
        ClientApp/ClientApp.h)
target_link_libraries(CarriageChatClient wsock32 ws2_32)

add_executable(
        CarriageChatHost
        HostApp/HostApp.cpp
)
target_link_libraries(CarriageChatHost wsock32 ws2_32)
