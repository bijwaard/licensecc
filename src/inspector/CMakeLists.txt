set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

ADD_EXECUTABLE(lccinspector inspector.cpp )

target_link_libraries(lccinspector 
     licensecc_static
     Boost::unit_test_framework 
     Boost::filesystem
     Boost::system)


install(TARGETS lccinspector RUNTIME DESTINATION bin/${LCC_PROJECT_NAME})
