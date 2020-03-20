#!/usr/bin/bash

#--------------Script per compilare BiKayaOS - Phase1.5----------------------
#                        - Bikaya OS - Phase 1.5 -
#    
#	Welcome to BiKaya OS!
#	To test kernel, just read README.md 
#	Enjoy using BiKaya OS. :)
#
#	Copyright (c) 2020 lso20az15. All rights reserved.
#	This work is licensed under the terms of the MIT license.
#	For a copy, see LICENSE.
# 	 
#	@credit: 
#   Stefano De Santis, Cristiano Guidotti, Iacopo Porcedda, Jacopo Rimediotti
#--------------------------------------------------------------------------------

usage()
{
    echo "usage: compile.sh [uarm|UARM|umps|UMPS|clean|CLEAN]"
}

#### Main

if [ "$1" == "" ]; then
    usage
    exit 1
fi

case $1 in
    "uarm"  | "UARM"    )   scons TARGET=uarm
                            ;;
    
    "umps"  | "UMPS"    )   scons TARGET=umps
                            ;;

    "clean" | "CLEAN"   )   scons -c
                            ;;

    *                   )   usage
                            exit 1
esac