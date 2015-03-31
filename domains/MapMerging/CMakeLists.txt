
MIRA_REQUIRE_PACKAGE(MapMerging)

###############################################################################

MIRA_ADD_DOCUMENTATION(MapMerging
	DIRS
		doc
		include
	DEPENDS
		#RobotDataTypes
)

###############################################################################

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/include)

###############################################################################

MIRA_ADD_BINARY(sensorPublisher
    SOURCE
        src/sensorPublisher.cpp

    LINK_LIBS
        MIRAFramework
)