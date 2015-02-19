
MIRA_REQUIRE_PACKAGE(sandbox)

###############################################################################

MIRA_ADD_DOCUMENTATION(sandbox
	DIRS
		doc
		include
	DEPENDS
		#RobotDataTypes
)

###############################################################################

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/include)

###############################################################################

MIRA_ADD_LIBRARY(GamepadController
	SHARED
	#PACKAGE $PACKAGE$
	SOURCE
		src/GamepadController.C
		src/gamepad.cc
		src/gamepadEvent.cc

	LINK_LIBS
		MIRABase
		MIRAFramework
)

###############################################################################

MIRA_ADD_LIBRARY(kukaParking
	SHARED
	#PACKAGE $PACKAGE$
	SOURCE
		src/kukaParking.C

	LINK_LIBS
		MIRABase
		MIRAFramework
)

###############################################################################

MIRA_ADD_LIBRARY(LaserData
	SHARED
	#PACKAGE $PACKAGE$
	SOURCE
		src/LaserData.C

	LINK_LIBS
		MIRABase
		MIRAFramework
)

###############################################################################

MIRA_ADD_LIBRARY(MapMerger
	SHARED
	#PACKAGE $PACKAGE$
	SOURCE
		src/MapMerger.C

	LINK_LIBS
		MIRABase
		MIRAFramework
)

###############################################################################

MIRA_ADD_LIBRARY(MapPublisher
	SHARED
	#PACKAGE $PACKAGE$
	SOURCE
		src/MapPublisher.C

	LINK_LIBS
		MIRABase
		MIRAFramework
)

###############################################################################

MIRA_ADD_LIBRARY(remoteOrders
	SHARED
	#PACKAGE $PACKAGE$
	SOURCE
		src/remoteOrders.C

	LINK_LIBS
		MIRABase
		MIRAFramework
)

###############################################################################

MIRA_ADD_LIBRARY(RPCExample
	SHARED
	#PACKAGE $PACKAGE$
	SOURCE
		src/RPCExample.C

	LINK_LIBS
		MIRABase
		MIRAFramework
)

###############################################################################

MIRA_ADD_LIBRARY(RPCTests
	SHARED
	#PACKAGE $PACKAGE$
	SOURCE
		src/RPCTests.C

	LINK_LIBS
		MIRABase
		MIRAFramework
)

###############################################################################

MIRA_ADD_LIBRARY(singleKukaParking
	SHARED
	#PACKAGE $PACKAGE$
	SOURCE
		src/singleKukaParking.C

	LINK_LIBS
		MIRABase
		MIRAFramework
)
