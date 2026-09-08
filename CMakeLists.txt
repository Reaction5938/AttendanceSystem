cmake_minimum_required(VERSION 3.20)

project(AttendanceSystem)

set(CMAKE_CXX_STANDARD 17)


add_compile_options(-finput-charset=UTF-8)

find_package(wxWidgets REQUIRED COMPONENTS core base)

find_package(PostgreSQL REQUIRED)

include(${wxWidgets_USE_FILE})

add_executable(AttendanceSystem
    main.cpp
    MainFrame.cpp
    CustomButton.cpp
    MenuPanel.cpp
    HomePanel.cpp
    HistoryPanel.cpp
    EditPanel.cpp
    VacationPanel.cpp
    OvertimePanel.cpp
    UserPanel.cpp
    Database.cpp
    LoginPanel.cpp
)

target_link_libraries(AttendanceSystem
    ${wxWidgets_LIBRARIES}
    PostgreSQL::PostgreSQL
)

target_compile_definitions(AttendanceSystem PRIVATE UNICODE _UNICODE)