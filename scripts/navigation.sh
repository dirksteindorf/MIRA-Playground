#!/bin/bash

# all maps have to be in a separate folder in /localhome/demo/map/

if [ -z "$1" ] # no parameter was given 
	then
		echo "usage: ./navigation.sh mapFolderName"
		echo "for example: ./navigation.sh test"
	else
		mapDirectory="/localhome/demo/map/"$1
		cd $mapDirectory
	
		# get the .mcf-file in this directory (should be only one - the loop returns only the last one)
		# not very pretty...I know
		for i in *.mcf; do
			mapName=$i
		done	
		mapLocation="$mapDirectory/$mapName"

	# open miracenter with the map file in the given directory
#	miracenter -c /opt/MIRA-commercial/domains/robot/SCITOSConfigs/etc/SCITOS-Pilot.xml --variables MCFFile="$mapLocation"
	miracenter -c /opt/MIRA-commercial/domains/robot/SCITOSConfigs/etc/SCITOS-Pilot.xml -v MCFFile="$mapLocation"
#	miracenter -c /localhome/demo/navConfig.xml --variables MCFFile="$mapLocation"
fi
