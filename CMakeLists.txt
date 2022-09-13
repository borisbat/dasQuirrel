
IF ((NOT DAS_QUIRREL_INCLUDED) AND ((NOT ${DAS_QUIRREL_DISABLED}) OR (NOT DEFINED DAS_QUIRREL_DISABLED)))
SET(DAS_QUIRREL_INCLUDED TRUE)
MESSAGE(STATUS "dasQUIRREL module included.")


SET(DAS_QUIRREL_DIR ${PROJECT_SOURCE_DIR}/modules/dasQuirrel)

include(ExternalProject)
ExternalProject_Add(
	LIBQUIRREL
	SOURCE_DIR ${DAS_QUIRREL_DIR}/quirrel
	CMAKE_ARGS -DCMAKE_OSX_ARCHITECTURES="${CMAKE_OSX_ARCHITECTURES}" -DCMAKE_INSTALL_PREFIX=${DAS_QUIRREL_DIR}/libquirrel -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_LINKER=${CMAKE_LINKER}
	BINARY_DIR ${DAS_QUIRREL_DIR}/quirrel/build
)

IF(APPLE)
	SET(QUIRREL_LIBRARIES
	${DAS_QUIRREL_DIR}/libquirrel/lib/sqmodules_static.a
	${DAS_QUIRREL_DIR}/libquirrel/lib/sqstdlib_static.a
	${DAS_QUIRREL_DIR}/libquirrel/lib/squirrel_static.a
#		"-framework CoreFoundation"
#		"-framework Security"
	)
ELSEIF(UNIX)
	SET(QUIRREL_LIBRARIES
	${DAS_QUIRREL_DIR}/libquirrel/lib/sqmodules_static.a
	${DAS_QUIRREL_DIR}/libquirrel/lib/sqstdlib_static.a
	${DAS_QUIRREL_DIR}/libquirrel/lib/squirrel_static.a
	)
ELSE()
	SET(QUIRREL_LIBRARIES
		${DAS_QUIRREL_DIR}/libquirrel/lib/sqmodules_static.lib
		${DAS_QUIRREL_DIR}/libquirrel/lib/sqstdlib_static.lib
		${DAS_QUIRREL_DIR}/libquirrel/lib/squirrel_static.lib
	)
ENDIF()

LIST(APPEND CMAKE_MODULE_PATH ${DAS_QUIRREL_DIR})

# libDasModuleQUIRREL
SET(DAS_QUIRREL_MODULE_SRC
${DAS_QUIRREL_DIR}/src/cb_dasQUIRREL.h
#generated
${DAS_QUIRREL_DIR}/src/need_dasQUIRREL.h
${DAS_QUIRREL_DIR}/src/dasQUIRREL.h
${DAS_QUIRREL_DIR}/src/dasQUIRREL.cpp
# user include
${DAS_QUIRREL_DIR}/src/dasQUIRREL.main.cpp
# generated binding includes
${DAS_QUIRREL_DIR}/src/dasQUIRREL.alias.add.inc
${DAS_QUIRREL_DIR}/src/dasQUIRREL.dummy.add.inc
${DAS_QUIRREL_DIR}/src/dasQUIRREL.enum.add.inc
${DAS_QUIRREL_DIR}/src/dasQUIRREL.enum.class.inc
${DAS_QUIRREL_DIR}/src/dasQUIRREL.enum.decl.inc
${DAS_QUIRREL_DIR}/src/dasQUIRREL.enum.decl.cast.inc
${DAS_QUIRREL_DIR}/src/dasQUIRREL.struct.add.inc
${DAS_QUIRREL_DIR}/src/dasQUIRREL.struct.postadd.inc
${DAS_QUIRREL_DIR}/src/dasQUIRREL.struct.class.inc
${DAS_QUIRREL_DIR}/src/dasQUIRREL.struct.decl.inc
${DAS_QUIRREL_DIR}/src/dasQUIRREL.struct.impl.inc
${DAS_QUIRREL_DIR}/src/dasQUIRREL.struct.cast.inc
${DAS_QUIRREL_DIR}/src/dasQUIRREL.func.reg.inc
${DAS_QUIRREL_DIR}/src/dasQUIRREL.func.decl.inc
${DAS_QUIRREL_DIR}/src/dasQUIRREL.func.aot.inc
${DAS_QUIRREL_DIR}/src/dasQUIRREL.func.aot.decl.inc
# generated functions
${DAS_QUIRREL_DIR}/src/dasQUIRREL.func_1.cpp
${DAS_QUIRREL_DIR}/src/dasQUIRREL.func_2.cpp
${DAS_QUIRREL_DIR}/src/dasQUIRREL.func_3.cpp
${DAS_QUIRREL_DIR}/src/dasQUIRREL.func_4.cpp
${DAS_QUIRREL_DIR}/src/dasQUIRREL.func_5.cpp
${DAS_QUIRREL_DIR}/src/dasQUIRREL.func_6.cpp
${DAS_QUIRREL_DIR}/src/dasQUIRREL.func_7.cpp
${DAS_QUIRREL_DIR}/src/dasQUIRREL.func_8.cpp
${DAS_QUIRREL_DIR}/src/dasQUIRREL.func_9.cpp
)

ADD_MODULE_LIB(libDasModuleQuirrel)
ADD_MODULE_CPP(dasQUIRREL)
ADD_LIBRARY(libDasModuleQuirrel ${DAS_QUIRREL_MODULE_SRC} ${DAS_QUIRREL_MODULE_PLATFORM_SRC})
ADD_DEPENDENCIES(libDasModuleQuirrel LIBQUIRREL)
TARGET_INCLUDE_DIRECTORIES(libDasModuleQuirrel PUBLIC "${DAS_QUIRREL_DIR}/libquirrel/include")
TARGET_LINK_LIBRARIES(libDasModuleQuirrel ${QUIRREL_LIBRARIES})
SETUP_CPP11(libDasModuleQuirrel)

#ADD_MODULE_DAS(dashv dashv dashv_boost)

ENDIF()
