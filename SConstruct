#--------------SConstruct per compilare BiKayaOS - Phase2----------------------
#                        - Bikaya OS - Phase 2 -
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

#---------------------------------------------------
#    DEFINE 
#---------------------------------------------------

S_EXT_LOW   ='.s'
S_EXT_HI    ='.S'
C_EXT       ='.c'
O_EXT       ='.o'
PRFX_I      = '-I'

# Definizione directory INCLUDE 
#-------------------------------------------------------
dir_h = './include'

# All INCLUDE dir
asl_h           = dir_h + '/asl'
handler_h       = dir_h + '/handler'
pcb_h           = dir_h + '/pcb'
scheduler_h     = dir_h + '/scheduler'
system_h        = dir_h + '/system'
sysshared_h     = system_h + '/shared'
utilities_h     = dir_h + '/utilities'
shared_device_h = sysshared_h + '/device'

# INCLUDE dir specifiche per architettura
uarm_h      = dir_h     + '/uarm'
umps_h      = dir_h     + '/umps'
sysuarm_h   = system_h  + '/uarm'
sysumps_h   = system_h  + '/umps'
uarm_handler_h = handler_h + '/uarm'
umps_handler_h = handler_h + '/umps'

# Definizione directory SOURCE 
#-------------------------------------------------------
dir_s = './src'

# All SOURCE dir
asl_s           = dir_s + '/asl'
pcb_s           = dir_s + '/pcb'
scheduler_s     = dir_s + '/scheduler'
system_s        = dir_s + '/system'
handler_s       = dir_s + '/handler'
sysshared_s     = system_s + '/shared'
device_s        = sysshared_s + '/device'
utilities_f     = dir_s + '/utilities'

# SOURCE dir specifiche per architettura
uarm_s           = dir_s     + '/uarm'
umps_s           = dir_s     + '/umps'
sysuarm_s        = system_s  + '/uarm'
sysumps_s        = system_s  + '/umps'
uarm_handler_s   = handler_s + '/uarm'
umps_handler_s   = handler_s + '/umps'

# Source files list (without extension)
#-------------------------------------------------------
main_f      = dir_s + '/main' 
p2test_f   = dir_s + '/p2test_bikaya_v0.2'
test_f      = dir_s + '/test'

# ASL Module
#--------------------
asl_f               = asl_s + '/asl'

# Handler Module
#--------------------
handler_f       = handler_s + '/shared'
##UARM dedicated
uarm_handler_f  = uarm_handler_s + '/handler'
##UMPS dedicated    
umps_handler_f  = umps_handler_s + '/handler'

# PCB Module
#--------------------
pcb_f               = pcb_s + '/pcb'
pcb_utils_f         = pcb_s + '/utils'

# Scheduler Module
#--------------------
scheduler_f         = scheduler_s + '/scheduler'

# Utilities Module
util_semaphore_f    = utilities_f + '/semaphore'
#System Module
#--------------------

##SHARED 
shared_device_f     = device_s + '/device'
##UARM dedicated
uarm_shared_f       = sysuarm_s + '/shared'
uarm_sysinit_f      = sysuarm_s + '/sysinit'

##UMPS dedicated
umps_shared_f       = sysumps_s + '/shared'
umps_sysinit_f      = sysumps_s + '/sysinit'

# File di architettura
#--------------------
##UARM dedicated
crtso_uarm  = uarm_s + '/crtso'
libuarm     = uarm_s + '/libuarm'
libdiv_uarm = uarm_s + '/libdiv'

##UMPS dedicated
crtso_umps  = umps_s + '/crtso'
libumps     = umps_s + '/libumps'

#---------------------------------------------------
# ENVIRONMENT
#---------------------------------------------------

# Opzioni disponibili
#-----------------------
option_active = ARGUMENTS.get('TARGET', 'help')

umps_mode   = (option_active=='umps' or option_active=='UMPS')
uarm_mode   = (option_active=='uarm' or option_active=='UARM')
clean_mode  = GetOption('clean')

if not umps_mode and not uarm_mode and not clean_mode:
    Help("Digita: 'scons TARGET=[UARM|uarm]' per buildare BiKaya per architetture uarm.\n")
    Help("Digita: 'scons TARGET=[UARM|umps]' per buildare BiKaya per architetture umps.\n")
    Help("Digita: 'scons -c' per pulire le directory.\n")
    Exit(2)

# Set Environments
#-----------------------

## Environment UMPS
umps_ENV = Environment(
    CC      = 'mipsel-linux-gnu-gcc',
    AS      = 'mipsel-linux-gnu-gcc',
    CFLAGS = '-Wall -O0 -ffreestanding -DTARGET_UMPS=1 -mips1 -mabi=32 -mno-gpopt -G 0 -mno-abicalls -fno-pic -mfp32 '
)

## Environment UARM
uarm_ENV = Environment(
    CC      = 'arm-none-eabi-gcc',
    AS      = 'arm-none-eabi-gcc',
    CFLAGS = '-Wall -O0 -mcpu=arm7tdmi -DTARGET_UARM=1 ',
)

#---------------------------------------------------
# COMPILE PHASE
#---------------------------------------------------

# Headers lists
#-------------------
shared_headers_list = [dir_h, system_h, sysshared_h, asl_h, handler_h, pcb_h, scheduler_h, utilities_h, shared_device_h ]
uarm_headers_list   = [uarm_h, sysuarm_h, uarm_handler_h]
umps_headers_list   = [umps_h, sysumps_h, umps_handler_h]

for i,x in enumerate(shared_headers_list):
    shared_headers_list[i] = PRFX_I+x

for i,x in enumerate(uarm_headers_list):
    uarm_headers_list[i] = PRFX_I+x

for i,x in enumerate(umps_headers_list):
    umps_headers_list[i] = PRFX_I+x

# Source (NOEXT) lists
#-------------------
shared_noext_list = [main_f, p2test_f, test_f, handler_f, scheduler_f, pcb_f, pcb_utils_f, asl_f, shared_device_f, util_semaphore_f ]

# Per favore, lascia i file crtso____ e lib_____ per ultimi
uarm_noext_list   = [uarm_shared_f, uarm_handler_f, uarm_sysinit_f, crtso_uarm, libuarm, libdiv_uarm]
umps_noext_list   = [umps_handler_f, umps_shared_f, umps_sysinit_f, crtso_umps, libumps]

# Source .C lists
#-------------------
shared_c_list   = []
uarm_c_list     = []
umps_c_list     = []

for x in shared_noext_list:
    shared_c_list.append(x+C_EXT)

for x in uarm_noext_list:
    uarm_c_list.append(x+C_EXT)
uarm_c_list[-1] = uarm_c_list[-1].replace(C_EXT, S_EXT_LOW)
uarm_c_list[-2] = uarm_c_list[-2].replace(C_EXT, S_EXT_LOW)
uarm_c_list[-3] = uarm_c_list[-3].replace(C_EXT, S_EXT_LOW)

for x in umps_noext_list:
    umps_c_list.append(x+C_EXT)
umps_c_list[-1] = umps_c_list[-1].replace(C_EXT, S_EXT_HI)
umps_c_list[-2] = umps_c_list[-2].replace(C_EXT, S_EXT_HI)

# Source .O lists
#-------------------
shared_o_list   = []
uarm_o_list     = []
umps_o_list     = []

for x in shared_noext_list:
    shared_o_list.append(x+O_EXT)
for x in uarm_noext_list:
    uarm_o_list.append(x+O_EXT)
for x in umps_noext_list:
    umps_o_list.append(x+O_EXT)

# Compilazione UARM
#---------------------
if uarm_mode:
    shared_headers_list.extend(uarm_headers_list)
    shared_o_list.extend(uarm_o_list)
    shared_c_list.extend(uarm_c_list)

    uarm_lib = shared_headers_list #WRAP
    uarm_obj = shared_o_list #WRAP
    uarm_src = shared_c_list #WRAP

    # Settings
    #-----------------
    LDFLAGS = ' -G 0 -nostdlib -T '+uarm_s+'/elf32ltsarm.h.uarmcore.x '
    uarm_ENV.Append(CFLAGS = ' '.join(uarm_lib))
    uarm_ENV.Replace(ASFLAGS = uarm_ENV['CFLAGS']+' -c')
    uarm_ENV.Replace(LINKCOM = 'arm-none-eabi-ld -o kernel '+' '.join(uarm_obj)+LDFLAGS)
    
    # Compilazione
    #----------------
    print("Compilazione kernel: uarm_mode attivo.")
    uarm_ENV.Program('kernel',uarm_src)

# Compilazione UMPS
#---------------------
if umps_mode:
    shared_headers_list.extend(umps_headers_list)
    shared_o_list.extend(umps_o_list)
    shared_c_list.extend(umps_c_list)

    umps_lib = shared_headers_list #WRAP
    umps_obj = shared_o_list #WRAP
    umps_src = shared_c_list # WRAP

    # Settings
    #-----------------
    LDFLAGS = ' -G 0 -nostdlib -T '+umps_s+'/umpscore.ldscript '
    umps_ENV.Append(CFLAGS = ' '.join(umps_lib))
    umps_ENV.Replace(ASFLAGS = umps_ENV['CFLAGS']+' -c')
    umps_ENV.Replace(LINKCOM = 'mipsel-linux-gnu-ld -o kernel '+' '.join(umps_obj)+LDFLAGS)
    
    # Compilazione
    #-----------------
    print("Compilazione kernel: umps_mode attivo.")
    main = umps_ENV.Program('kernel',umps_src)
    elf  = umps_ENV.Command('elf',None,'umps2-elf2umps -k kernel')
    Depends(elf, main)

#---------------------------------------------------
# CLEAN PHASE
#---------------------------------------------------
clean_list = ['kernel', 'kernel.core.umps','kernel.stab.umps']
clean_list.extend(shared_o_list)
clean_list.extend(uarm_o_list)
clean_list.extend(umps_o_list)

uarm_ENV.Clean(' ',clean_list)
