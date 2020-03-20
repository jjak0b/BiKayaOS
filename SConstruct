#--------------SConstruct per compilare BiKayaOS - Phase1.5----------------------
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

#---------------------------------------------------
#    DEFINE 
#---------------------------------------------------

S_EXT_LOW ='.s'
S_EXT_HI  ='.S'
C_EXT ='.c'
O_EXT ='.o'
PRFX_I = '-I'

# Definizione directory INCLUDE 
#-------------------------------------------------------
dir_h = './include'

# All INCLUDE dir
asl_h           = dir_h + '/asl'
devices_h       = dir_h + '/devices'
handler_h       = dir_h + '/handler'
interrupts_h    = dir_h + '/interrupts'
pcb_h           = dir_h + '/pcb'
scheduler_h     = dir_h + '/scheduler'
system_h        = dir_h + '/system'
sysshared_h     = system_h + '/shared'
utilities_h     = dir_h + '/utilities'

# INCLUDE dir specifiche per architettura
uarm_h      = dir_h     + '/uarm'
umps_h      = dir_h     + '/umps'
sysuarm_h   = system_h  + '/uarm'
sysumps_h   = system_h  + '/umps'

# Definizione directory SOURCE 
#-------------------------------------------------------
dir_s = './src'

# All SOURCE dir
asl_s           = dir_s + '/asl'
devices_s       = dir_s + '/devices'
handler_s       = dir_s + '/handler'
interrupts_s    = dir_s + '/interrupts'
pcb_s           = dir_s + '/pcb'
scheduler_s     = dir_s + '/scheduler'
system_s        = dir_s + '/system'
utilities_s     = dir_s + '/utilities'

# SOURCE dir specifiche per architettura
uarm_s      = dir_s     + '/uarm'
umps_s      = dir_s     + '/umps'
sysuarm_s   = system_s  + '/uarm'
sysumps_s   = system_s  + '/umps'

# SOURCE dir specifiche per architettura
uarm_d  = dir_s + '/uarm'
umps_d  = dir_s + '/umps'

# Source files list (without extension)
#-------------------------------------------------------
main_f      = dir_s + '/main' 
p15test_f   = dir_s + '/p1.5test_bikaya_v0'
test_f      = dir_s + '/test'

# ASL Module
#--------------------
asl_f               = asl_s + '/asl'

# Devices Module
#--------------------
device_f            = devices_s + '/device'
printer_f           = devices_s + '/printer'
terminal_f          = devices_s + '/terminal'

# Handler Module
#--------------------
handler_f           = handler_s + '/handler'

# Interrupts Module
#--------------------
#

# PCB Module
#--------------------
pcb_f               = pcb_s + '/pcb'

# Scheduler Module
#--------------------
scheduler_f         = scheduler_s + '/scheduler'

# Utilities Module
#--------------------
shared_f            = utilities_s + '/shared'

#System Module
#--------------------
##UARM dedicated
uarm_shared_f       = sysuarm_s + '/shared'
uarm_sysinit_f      = sysuarm_s + '/sysinit'

##UMPS dedicated
umps_shared_f       = sysumps_s + '/shared'
umps_sysinit_f      = sysumps_s + '/sysinit'

# File di architettura
#--------------------
##UARM dedicated
crtso_uarm  = uarm_d + '/crtso'
libuarm     = uarm_d + '/libuarm'

##UMPS dedicated
crtso_umps  = umps_d + '/crtso'
libumps     = umps_d + '/libumps'

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
shared_headers_list = [dir_h, system_h, sysshared_h, asl_h, devices_h, handler_h, interrupts_h, pcb_h, scheduler_h, utilities_h]
uarm_headers_list   = [sysuarm_h]
umps_headers_list   = [sysumps_h]

for i,x in enumerate(shared_headers_list):
    shared_headers_list[i] = PRFX_I+x

for i,x in enumerate(uarm_headers_list):
    uarm_headers_list[i] = PRFX_I+x

for i,x in enumerate(umps_headers_list):
    umps_headers_list[i] = PRFX_I+x

# Source (NOEXT) lists
#-------------------
shared_noext_list = [main_f, p15test_f, test_f, shared_f, scheduler_f, pcb_f, handler_f, terminal_f, printer_f, device_f, asl_f]

# Per favore, lascia i file crtso____ e lib_____ per ultimi
uarm_noext_list   = [uarm_shared_f, uarm_sysinit_f, crtso_uarm, libuarm]
umps_noext_list   = [umps_shared_f, umps_sysinit_f, crtso_umps, libumps]

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
    LDFLAGS = ' -G 0 -nostdlib -T '+uarm_d+'/elf32ltsarm.h.uarmcore.x '
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
    LDFLAGS = ' -G 0 -nostdlib -T '+umps_d+'/umpscore.ldscript '
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
