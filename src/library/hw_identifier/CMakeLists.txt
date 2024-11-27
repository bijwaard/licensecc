set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_library(hw_identifier OBJECT
	hw_identifier_facade.cpp
	default_strategy.cpp
	ethernet.cpp
	disk_strategy.cpp
	ethernet_and_disk_strategy.cpp
	identification_strategy.cpp
	hw_identifier.cpp
)

if(CODE_COVERAGE AND UNIX)
	  target_compile_options(hw_identifier PUBLIC -O0 -g --coverage)
endif(CODE_COVERAGE AND UNIX)
