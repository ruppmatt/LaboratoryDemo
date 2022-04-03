project(LaboratoryDemoLib VERSION 2.02.00)
configure_file(Config.h.in Config.h)

# Request the required wxWidgets libs
# Turn off wxWidgets own precompiled header system, since
# it doesn't seem to work. The CMake version works much better.
set(wxBUILD_PRECOMP OFF)
find_package(wxWidgets COMPONENTS core base xrc html xml REQUIRED)

# Include the wxWidgets use file to initialize various settings
include(${wxWidgets_USE_FILE})

# Define a variable containing a list of source files for the project
#set(SOURCE_FILES
set(SOURCE_FILES pch.h ILaboratoryIsolator.h
        LaboratoryDemoBaseApp.h LaboratoryDemoBaseApp.cpp
        LaboratoryDemoMainFrame.cpp LaboratoryDemoMainFrame.h
        ControlPanel.cpp ControlPanel.h
        LaboratoryView.cpp LaboratoryView.h
        AboutDialog.cpp AboutDialog.h
        Controller.cpp Controller.h)


add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)