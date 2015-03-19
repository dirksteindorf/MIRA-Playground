
MIRA_REQUIRE_PACKAGE(ChannelData)

###############################################################################

MIRA_ADD_DOCUMENTATION(ChannelData
	DIRS
		doc
		include
	DEPENDS
		#RobotDataTypes
)

###############################################################################

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/include)

###############################################################################

MIRA_ADD_LIBRARY(OwnChannelDataPub
	SHARED
	#PACKAGE $PACKAGE$
	SOURCE
		src/OwnChannelDataPub.C
		src/OwnChannelData.cpp

	LINK_LIBS
		MIRABase
		MIRAFramework
)

###############################################################################

MIRA_ADD_LIBRARY(OwnChannelDataSub
	SHARED
	#PACKAGE $PACKAGE$
	SOURCE
		src/OwnChannelDataSub.C
		src/OwnChannelData.cpp

	LINK_LIBS
		MIRABase
		MIRAFramework
)
