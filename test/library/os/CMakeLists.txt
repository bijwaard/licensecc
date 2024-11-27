set(CMAKE_CXX_STANDARD 17)

if(MSVC)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT /NODEFAULTLIB:LIBCMTD /NODEFAULTLIB:MSVCRTD")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
# set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT /NODEFAULTLIB:LIBCMTD /NODEFAULTLIB:MSVCRTD")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  set(Boost_USE_STATIC_RUNTIME ON)
endif()

add_executable( test_network
 network_test.cpp
 "network_test.cpp")

target_link_libraries( test_network
 licensecc_static
 Boost::unit_test_framework 
 Boost::filesystem
 Boost::system
)

ADD_TEST(NAME test_network COMMAND test_network)

IF(NOT DEFINED MINGW)
	add_executable( test_dmi_info dmi_info_test.cpp "dmi_info_test.cpp")
	target_link_libraries( test_dmi_info
 		licensecc_static
 		Boost::unit_test_framework 
 		Boost::filesystem
 		Boost::system
	)
	ADD_TEST(NAME test_dmi_info COMMAND test_dmi_info)
ENDIF(NOT DEFINED MINGW)

add_executable( test_execution_environment
 	execution_environment_test.cpp
)

target_link_libraries( test_execution_environment
 licensecc_static
 Boost::unit_test_framework 
 Boost::filesystem
 Boost::system
)

ADD_TEST(NAME test_execution_environment COMMAND test_execution_environment)

if(CODE_COVERAGE AND UNIX)
	IF(NOT DEFINED MINGW)
		target_compile_options(test_dmi_info PUBLIC -O0 -g --coverage)
	ENDIF(NOT DEFINED MINGW)
	
	target_compile_options(test_execution_environment PUBLIC -O0 -g --coverage)
	target_compile_options(test_network PUBLIC -O0 -g --coverage)
endif(CODE_COVERAGE AND UNIX)




