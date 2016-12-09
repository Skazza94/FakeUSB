#
# This file is part of USBProxy.
#

project(usbproxy_devices C)
set(PACKAGE usbproxy_devices)

set(CMAKE_C_FLAGS "$ENV{CFLAGS} -pthread -Wall")
set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} $ENV{CPPFLAGS} -std=gnu++11 -pthread -pedantic -Wall -felide-constructors -fno-exceptions")

include_directories(${libusbproxy_SOURCE_DIR})
LIST(APPEND PLUGINS_LINK_LIBS USBProxy)

LIST(APPEND PLUGINS DeviceProxy_Emulation)

foreach(plugin ${PLUGINS})
	add_library(${plugin} SHARED ${plugin}.cpp)
	set_source_files_properties(${plugin}.cpp PROPERTIES COMPILE_FLAGS  "${CMAKE_CPP_FLAGS}")
	set_target_properties(${plugin} PROPERTIES PREFIX "")
	target_link_libraries(${plugin} ${PLUGINS_LINK_LIBS})
	install(TARGETS ${plugin}
        LIBRARY DESTINATION lib/USBProxy${LIB_SUFFIX}
        COMPONENT sharedlibs
        )
endforeach(plugin)
