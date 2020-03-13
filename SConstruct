# SConstruct per compilare BiKayaOS - Phase1.5


option_active = ARGUMENTS.get('TARGET', 'help')

umps_mode = option_active=='umps'
uarm_mode = option_active=='uarm'
clean_mode = GetOption('clean')

if not umps_mode and not uarm_mode and not clean_mode:
    Help("Digita: 'scons TARGET=uarm' per buildare BiKaya per architetture uarm.\n")
    Help("Digita: 'scons TARGET=umps' per buildare BiKaya per architetture umps.\n")
    Help("Digita: 'scons -c' per pulire le directory.\n")
    Exit(2)

# Definizione directory INCLUDE
dir_h       = './include'
asl_h       = dir_h + '/asl'
pcb_h       = dir_h + '/pcb'
system_h    = dir_h + '/system'
uarm_h      = dir_h + '/uarm'
umps_h      = dir_h + '/umps'

PRFX_I = '-I'
uarm_headers = [PRFX_I+dir_h, PRFX_I+asl_h, PRFX_I+pcb_h, PRFX_I+system_h, PRFX_I+uarm_h]
umps_headers = [PRFX_I+dir_h, PRFX_I+asl_h, PRFX_I+pcb_h, PRFX_I+system_h, PRFX_I+umps_h]

# Definizione directory uarm e umps (source)
src     = './src'
uarm_d  = src+'/uarm'
umps_d  = src+'/umps'

# Environment UMPS
umps_ENV = Environment(
    CC      = 'mipsel-linux-gnu-gcc',
    AS      = 'mipsel-linux-gnu-gcc',
    CFLAGS = '-Wall -O0 -ffreestanding -DTARGET_UMPS=1 -mips1 -mabi=32 -mno-gpopt -G 0 -mno-abicalls -fno-pic -mfp32 '
)

# Environment UARM
uarm_ENV = Environment(
    CC      = 'arm-none-eabi-gcc',
    AS      = 'arm-none-eabi-gcc',
    CFLAGS = '-Wall -O0 -mcpu=arm7tdmi -DTARGET_UARM=1 ',
)

# Last settings 
umps_ENV.Append(CFLAGS = ' '.join(umps_headers))
umps_ENV.Replace(ASFLAGS = umps_ENV['CFLAGS']+' -c')

uarm_ENV.Append(CFLAGS = ' '.join(uarm_headers))
uarm_ENV.Replace(ASFLAGS = uarm_ENV['CFLAGS']+' -c')

S_EXT='.s'
C_EXT='.c'
O_EXT='.o'

## Path sorgenti C
pcb_s = src+'/pcb/pcb'
asl_s = src+'/asl/asl'
test_s = src+'/p1test_bikaya_v0'
## Relativi ad UARM
crtso_uarm = uarm_d+'/crtso'
libuarm = uarm_d+'/libuarm'
## Relativi ad UMPS
crtso_umps = umps_d+'/crtso'
libumps = umps_d+'/libumps'

##Compilazione UARM
if uarm_mode:
    print("Compilazione kernel: uarm_mode attivo.")
    # Linking
    LDFLAGS = ' -G 0 -nostdlib -T '+uarm_d+'/elf32ltsarm.h.uarmcore.x '
    uarm_ENV.Replace(LINKCOM = 'arm-none-eabi-ld -o kernel '+' '.join([test_s+O_EXT, pcb_s+O_EXT, asl_s+O_EXT, crtso_uarm+O_EXT, libuarm+O_EXT])+LDFLAGS)
    # Compilazione
    uarm_ENV.Program('kernel',[test_s+C_EXT, pcb_s+C_EXT, asl_s+C_EXT, crtso_uarm+S_EXT, libuarm+S_EXT])
##Compilazione UMPS
if umps_mode:
    print("Compilazione kernel: umps_mode attivo.")
    # Linking
    LDFLAGS = ' -G 0 -nostdlib -T '+umps_d+'/umpscore.ldscript '
    umps_ENV.Replace(LINKCOM = 'mipsel-linux-gnu-ld -o kernel '+' '.join([test_s+O_EXT, pcb_s+O_EXT, asl_s+O_EXT, crtso_umps+O_EXT, libumps+O_EXT])+LDFLAGS)
    # Compilazione
    main = umps_ENV.Program('kernel',[test_s+C_EXT, pcb_s+C_EXT, asl_s+C_EXT, crtso_umps+'.S', libumps+'.S'])
    elf  = umps_ENV.Command('elf',None,'umps2-elf2umps -k kernel')
    Depends(elf, main)

#Target CLEAN
uarm_ENV.Clean(' ',['kernel', 'kernel.core.umps','kernel.stab.umps', test_s+O_EXT, pcb_s+O_EXT, asl_s+O_EXT, crtso_umps+O_EXT, libumps+O_EXT, crtso_uarm+O_EXT, libuarm+O_EXT])
