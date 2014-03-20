#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SUCHAI.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SUCHAI.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../Drivers/interfaz_SPI.c ../Drivers/pic_pc104_config.c ../Drivers/rs232_suchai.c ../Drivers/transceiver.c ../Drivers/console.c ../Drivers/eps_suchai.c ../Drivers/rtc_mb.c ../Drivers/memSD.c ../Drivers/memFlash.c ../Drivers/memEEPROM.c ../Drivers/interfaz_ADC.c ../Drivers/i2c_comm.c ../FreeRTOS/Source/croutine.c ../FreeRTOS/Source/list.c ../FreeRTOS/Source/queue.c ../FreeRTOS/Source/tasks.c ../FreeRTOS/Source/timers.c ../Payloads/Cmd/cmdPayload.c ../Payloads/Drivers/camera.c ../Payloads/Drivers/dig_gyro.c ../Payloads/Drivers/fis_payload.c ../Payloads/Drivers/sensTemp.c ../Payloads/Drivers/langmuir.c ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/port.c ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S ../FreeRTOS/Source/portable/MemMang/heap_2.c ../System/taskConsole.c ../System/taskDispatcher.c ../System/taskExecuter.c ../System/cmdPPC.c ../System/cmdConsole.c ../System/taskHousekeeping.c ../System/taskFlightPlan.c ../System/dataRepository.c ../System/cmdRepository.c ../System/cmdTRX.c ../System/cmdDRP.c ../System/taskComunications.c ../System/taskDeployment.c ../System/cmdEPS.c ../System/cmdRTC.c ../System/cmdTCM.c ../System/taskFlightPlan2.c ../main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o ${OBJECTDIR}/_ext/1904510940/transceiver.o ${OBJECTDIR}/_ext/1904510940/console.o ${OBJECTDIR}/_ext/1904510940/eps_suchai.o ${OBJECTDIR}/_ext/1904510940/rtc_mb.o ${OBJECTDIR}/_ext/1904510940/memSD.o ${OBJECTDIR}/_ext/1904510940/memFlash.o ${OBJECTDIR}/_ext/1904510940/memEEPROM.o ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o ${OBJECTDIR}/_ext/1904510940/i2c_comm.o ${OBJECTDIR}/_ext/381897321/croutine.o ${OBJECTDIR}/_ext/381897321/list.o ${OBJECTDIR}/_ext/381897321/queue.o ${OBJECTDIR}/_ext/381897321/tasks.o ${OBJECTDIR}/_ext/381897321/timers.o ${OBJECTDIR}/_ext/570733759/cmdPayload.o ${OBJECTDIR}/_ext/1325771184/camera.o ${OBJECTDIR}/_ext/1325771184/dig_gyro.o ${OBJECTDIR}/_ext/1325771184/fis_payload.o ${OBJECTDIR}/_ext/1325771184/sensTemp.o ${OBJECTDIR}/_ext/1325771184/langmuir.o ${OBJECTDIR}/_ext/1841791051/port.o ${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o ${OBJECTDIR}/_ext/167578668/heap_2.o ${OBJECTDIR}/_ext/1719123586/taskConsole.o ${OBJECTDIR}/_ext/1719123586/taskDispatcher.o ${OBJECTDIR}/_ext/1719123586/taskExecuter.o ${OBJECTDIR}/_ext/1719123586/cmdPPC.o ${OBJECTDIR}/_ext/1719123586/cmdConsole.o ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o ${OBJECTDIR}/_ext/1719123586/dataRepository.o ${OBJECTDIR}/_ext/1719123586/cmdRepository.o ${OBJECTDIR}/_ext/1719123586/cmdTRX.o ${OBJECTDIR}/_ext/1719123586/cmdDRP.o ${OBJECTDIR}/_ext/1719123586/taskComunications.o ${OBJECTDIR}/_ext/1719123586/taskDeployment.o ${OBJECTDIR}/_ext/1719123586/cmdEPS.o ${OBJECTDIR}/_ext/1719123586/cmdRTC.o ${OBJECTDIR}/_ext/1719123586/cmdTCM.o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o ${OBJECTDIR}/_ext/1472/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o.d ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o.d ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o.d ${OBJECTDIR}/_ext/1904510940/transceiver.o.d ${OBJECTDIR}/_ext/1904510940/console.o.d ${OBJECTDIR}/_ext/1904510940/eps_suchai.o.d ${OBJECTDIR}/_ext/1904510940/rtc_mb.o.d ${OBJECTDIR}/_ext/1904510940/memSD.o.d ${OBJECTDIR}/_ext/1904510940/memFlash.o.d ${OBJECTDIR}/_ext/1904510940/memEEPROM.o.d ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o.d ${OBJECTDIR}/_ext/1904510940/i2c_comm.o.d ${OBJECTDIR}/_ext/381897321/croutine.o.d ${OBJECTDIR}/_ext/381897321/list.o.d ${OBJECTDIR}/_ext/381897321/queue.o.d ${OBJECTDIR}/_ext/381897321/tasks.o.d ${OBJECTDIR}/_ext/381897321/timers.o.d ${OBJECTDIR}/_ext/570733759/cmdPayload.o.d ${OBJECTDIR}/_ext/1325771184/camera.o.d ${OBJECTDIR}/_ext/1325771184/dig_gyro.o.d ${OBJECTDIR}/_ext/1325771184/fis_payload.o.d ${OBJECTDIR}/_ext/1325771184/sensTemp.o.d ${OBJECTDIR}/_ext/1325771184/langmuir.o.d ${OBJECTDIR}/_ext/1841791051/port.o.d ${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.d ${OBJECTDIR}/_ext/167578668/heap_2.o.d ${OBJECTDIR}/_ext/1719123586/taskConsole.o.d ${OBJECTDIR}/_ext/1719123586/taskDispatcher.o.d ${OBJECTDIR}/_ext/1719123586/taskExecuter.o.d ${OBJECTDIR}/_ext/1719123586/cmdPPC.o.d ${OBJECTDIR}/_ext/1719123586/cmdConsole.o.d ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o.d ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o.d ${OBJECTDIR}/_ext/1719123586/dataRepository.o.d ${OBJECTDIR}/_ext/1719123586/cmdRepository.o.d ${OBJECTDIR}/_ext/1719123586/cmdTRX.o.d ${OBJECTDIR}/_ext/1719123586/cmdDRP.o.d ${OBJECTDIR}/_ext/1719123586/taskComunications.o.d ${OBJECTDIR}/_ext/1719123586/taskDeployment.o.d ${OBJECTDIR}/_ext/1719123586/cmdEPS.o.d ${OBJECTDIR}/_ext/1719123586/cmdRTC.o.d ${OBJECTDIR}/_ext/1719123586/cmdTCM.o.d ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o.d ${OBJECTDIR}/_ext/1472/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o ${OBJECTDIR}/_ext/1904510940/transceiver.o ${OBJECTDIR}/_ext/1904510940/console.o ${OBJECTDIR}/_ext/1904510940/eps_suchai.o ${OBJECTDIR}/_ext/1904510940/rtc_mb.o ${OBJECTDIR}/_ext/1904510940/memSD.o ${OBJECTDIR}/_ext/1904510940/memFlash.o ${OBJECTDIR}/_ext/1904510940/memEEPROM.o ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o ${OBJECTDIR}/_ext/1904510940/i2c_comm.o ${OBJECTDIR}/_ext/381897321/croutine.o ${OBJECTDIR}/_ext/381897321/list.o ${OBJECTDIR}/_ext/381897321/queue.o ${OBJECTDIR}/_ext/381897321/tasks.o ${OBJECTDIR}/_ext/381897321/timers.o ${OBJECTDIR}/_ext/570733759/cmdPayload.o ${OBJECTDIR}/_ext/1325771184/camera.o ${OBJECTDIR}/_ext/1325771184/dig_gyro.o ${OBJECTDIR}/_ext/1325771184/fis_payload.o ${OBJECTDIR}/_ext/1325771184/sensTemp.o ${OBJECTDIR}/_ext/1325771184/langmuir.o ${OBJECTDIR}/_ext/1841791051/port.o ${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o ${OBJECTDIR}/_ext/167578668/heap_2.o ${OBJECTDIR}/_ext/1719123586/taskConsole.o ${OBJECTDIR}/_ext/1719123586/taskDispatcher.o ${OBJECTDIR}/_ext/1719123586/taskExecuter.o ${OBJECTDIR}/_ext/1719123586/cmdPPC.o ${OBJECTDIR}/_ext/1719123586/cmdConsole.o ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o ${OBJECTDIR}/_ext/1719123586/dataRepository.o ${OBJECTDIR}/_ext/1719123586/cmdRepository.o ${OBJECTDIR}/_ext/1719123586/cmdTRX.o ${OBJECTDIR}/_ext/1719123586/cmdDRP.o ${OBJECTDIR}/_ext/1719123586/taskComunications.o ${OBJECTDIR}/_ext/1719123586/taskDeployment.o ${OBJECTDIR}/_ext/1719123586/cmdEPS.o ${OBJECTDIR}/_ext/1719123586/cmdRTC.o ${OBJECTDIR}/_ext/1719123586/cmdTCM.o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o ${OBJECTDIR}/_ext/1472/main.o

# Source Files
SOURCEFILES=../Drivers/interfaz_SPI.c ../Drivers/pic_pc104_config.c ../Drivers/rs232_suchai.c ../Drivers/transceiver.c ../Drivers/console.c ../Drivers/eps_suchai.c ../Drivers/rtc_mb.c ../Drivers/memSD.c ../Drivers/memFlash.c ../Drivers/memEEPROM.c ../Drivers/interfaz_ADC.c ../Drivers/i2c_comm.c ../FreeRTOS/Source/croutine.c ../FreeRTOS/Source/list.c ../FreeRTOS/Source/queue.c ../FreeRTOS/Source/tasks.c ../FreeRTOS/Source/timers.c ../Payloads/Cmd/cmdPayload.c ../Payloads/Drivers/camera.c ../Payloads/Drivers/dig_gyro.c ../Payloads/Drivers/fis_payload.c ../Payloads/Drivers/sensTemp.c ../Payloads/Drivers/langmuir.c ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/port.c ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S ../FreeRTOS/Source/portable/MemMang/heap_2.c ../System/taskConsole.c ../System/taskDispatcher.c ../System/taskExecuter.c ../System/cmdPPC.c ../System/cmdConsole.c ../System/taskHousekeeping.c ../System/taskFlightPlan.c ../System/dataRepository.c ../System/cmdRepository.c ../System/cmdTRX.c ../System/cmdDRP.c ../System/taskComunications.c ../System/taskDeployment.c ../System/cmdEPS.c ../System/cmdRTC.c ../System/cmdTCM.c ../System/taskFlightPlan2.c ../main.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/SUCHAI.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ256GA110
MP_LINKER_FILE_OPTION=,--script="../p24FJ256GA110.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o: ../Drivers/interfaz_SPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/interfaz_SPI.c  -o ${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o: ../Drivers/pic_pc104_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/pic_pc104_config.c  -o ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/rs232_suchai.o: ../Drivers/rs232_suchai.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/rs232_suchai.c  -o ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/rs232_suchai.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/rs232_suchai.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/transceiver.o: ../Drivers/transceiver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/transceiver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/transceiver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/transceiver.c  -o ${OBJECTDIR}/_ext/1904510940/transceiver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/transceiver.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/transceiver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/console.o: ../Drivers/console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/console.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/console.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/console.c  -o ${OBJECTDIR}/_ext/1904510940/console.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/console.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/console.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/eps_suchai.o: ../Drivers/eps_suchai.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/eps_suchai.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/eps_suchai.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/eps_suchai.c  -o ${OBJECTDIR}/_ext/1904510940/eps_suchai.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/eps_suchai.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/eps_suchai.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/rtc_mb.o: ../Drivers/rtc_mb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/rtc_mb.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/rtc_mb.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/rtc_mb.c  -o ${OBJECTDIR}/_ext/1904510940/rtc_mb.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/rtc_mb.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/rtc_mb.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/memSD.o: ../Drivers/memSD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memSD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memSD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/memSD.c  -o ${OBJECTDIR}/_ext/1904510940/memSD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/memSD.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/memSD.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/memFlash.o: ../Drivers/memFlash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memFlash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memFlash.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/memFlash.c  -o ${OBJECTDIR}/_ext/1904510940/memFlash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/memFlash.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/memFlash.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/memEEPROM.o: ../Drivers/memEEPROM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memEEPROM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memEEPROM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/memEEPROM.c  -o ${OBJECTDIR}/_ext/1904510940/memEEPROM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/memEEPROM.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/memEEPROM.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o: ../Drivers/interfaz_ADC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/interfaz_ADC.c  -o ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/i2c_comm.o: ../Drivers/i2c_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/i2c_comm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/i2c_comm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/i2c_comm.c  -o ${OBJECTDIR}/_ext/1904510940/i2c_comm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/i2c_comm.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/i2c_comm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/croutine.o: ../FreeRTOS/Source/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/381897321/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/croutine.c  -o ${OBJECTDIR}/_ext/381897321/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/croutine.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/croutine.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/list.o: ../FreeRTOS/Source/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/381897321/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/list.c  -o ${OBJECTDIR}/_ext/381897321/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/list.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/list.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/queue.o: ../FreeRTOS/Source/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/381897321/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/queue.c  -o ${OBJECTDIR}/_ext/381897321/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/queue.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/queue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/tasks.o: ../FreeRTOS/Source/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/381897321/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/tasks.c  -o ${OBJECTDIR}/_ext/381897321/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/tasks.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/tasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/timers.o: ../FreeRTOS/Source/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/381897321/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/timers.c  -o ${OBJECTDIR}/_ext/381897321/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/timers.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/timers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/570733759/cmdPayload.o: ../Payloads/Cmd/cmdPayload.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/570733759 
	@${RM} ${OBJECTDIR}/_ext/570733759/cmdPayload.o.d 
	@${RM} ${OBJECTDIR}/_ext/570733759/cmdPayload.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Cmd/cmdPayload.c  -o ${OBJECTDIR}/_ext/570733759/cmdPayload.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/570733759/cmdPayload.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/570733759/cmdPayload.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/camera.o: ../Payloads/Drivers/camera.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/camera.o.d 
	@${RM} ${OBJECTDIR}/_ext/1325771184/camera.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/camera.c  -o ${OBJECTDIR}/_ext/1325771184/camera.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/camera.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/camera.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/dig_gyro.o: ../Payloads/Drivers/dig_gyro.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/dig_gyro.o.d 
	@${RM} ${OBJECTDIR}/_ext/1325771184/dig_gyro.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/dig_gyro.c  -o ${OBJECTDIR}/_ext/1325771184/dig_gyro.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/dig_gyro.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/dig_gyro.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/fis_payload.o: ../Payloads/Drivers/fis_payload.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/fis_payload.o.d 
	@${RM} ${OBJECTDIR}/_ext/1325771184/fis_payload.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/fis_payload.c  -o ${OBJECTDIR}/_ext/1325771184/fis_payload.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/fis_payload.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/fis_payload.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/sensTemp.o: ../Payloads/Drivers/sensTemp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/sensTemp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1325771184/sensTemp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/sensTemp.c  -o ${OBJECTDIR}/_ext/1325771184/sensTemp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/sensTemp.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/sensTemp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/langmuir.o: ../Payloads/Drivers/langmuir.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/langmuir.o.d 
	@${RM} ${OBJECTDIR}/_ext/1325771184/langmuir.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/langmuir.c  -o ${OBJECTDIR}/_ext/1325771184/langmuir.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/langmuir.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/langmuir.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1841791051/port.o: ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1841791051 
	@${RM} ${OBJECTDIR}/_ext/1841791051/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1841791051/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/port.c  -o ${OBJECTDIR}/_ext/1841791051/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1841791051/port.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1841791051/port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/167578668/heap_2.o: ../FreeRTOS/Source/portable/MemMang/heap_2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/167578668 
	@${RM} ${OBJECTDIR}/_ext/167578668/heap_2.o.d 
	@${RM} ${OBJECTDIR}/_ext/167578668/heap_2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/portable/MemMang/heap_2.c  -o ${OBJECTDIR}/_ext/167578668/heap_2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/167578668/heap_2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/167578668/heap_2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskConsole.o: ../System/taskConsole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskConsole.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskConsole.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskConsole.c  -o ${OBJECTDIR}/_ext/1719123586/taskConsole.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskConsole.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskConsole.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskDispatcher.o: ../System/taskDispatcher.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskDispatcher.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskDispatcher.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskDispatcher.c  -o ${OBJECTDIR}/_ext/1719123586/taskDispatcher.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskDispatcher.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskDispatcher.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskExecuter.o: ../System/taskExecuter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskExecuter.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskExecuter.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskExecuter.c  -o ${OBJECTDIR}/_ext/1719123586/taskExecuter.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskExecuter.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskExecuter.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdPPC.o: ../System/cmdPPC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdPPC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdPPC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdPPC.c  -o ${OBJECTDIR}/_ext/1719123586/cmdPPC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdPPC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdPPC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdConsole.o: ../System/cmdConsole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdConsole.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdConsole.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdConsole.c  -o ${OBJECTDIR}/_ext/1719123586/cmdConsole.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdConsole.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdConsole.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o: ../System/taskHousekeeping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskHousekeeping.c  -o ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o: ../System/taskFlightPlan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskFlightPlan.c  -o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/dataRepository.o: ../System/dataRepository.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/dataRepository.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/dataRepository.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/dataRepository.c  -o ${OBJECTDIR}/_ext/1719123586/dataRepository.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/dataRepository.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/dataRepository.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdRepository.o: ../System/cmdRepository.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdRepository.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdRepository.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdRepository.c  -o ${OBJECTDIR}/_ext/1719123586/cmdRepository.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdRepository.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdRepository.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdTRX.o: ../System/cmdTRX.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdTRX.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdTRX.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdTRX.c  -o ${OBJECTDIR}/_ext/1719123586/cmdTRX.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdTRX.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdTRX.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdDRP.o: ../System/cmdDRP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdDRP.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdDRP.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdDRP.c  -o ${OBJECTDIR}/_ext/1719123586/cmdDRP.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdDRP.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdDRP.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskComunications.o: ../System/taskComunications.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskComunications.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskComunications.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskComunications.c  -o ${OBJECTDIR}/_ext/1719123586/taskComunications.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskComunications.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskComunications.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskDeployment.o: ../System/taskDeployment.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskDeployment.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskDeployment.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskDeployment.c  -o ${OBJECTDIR}/_ext/1719123586/taskDeployment.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskDeployment.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskDeployment.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdEPS.o: ../System/cmdEPS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdEPS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdEPS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdEPS.c  -o ${OBJECTDIR}/_ext/1719123586/cmdEPS.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdEPS.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdEPS.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdRTC.o: ../System/cmdRTC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdRTC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdRTC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdRTC.c  -o ${OBJECTDIR}/_ext/1719123586/cmdRTC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdRTC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdRTC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdTCM.o: ../System/cmdTCM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdTCM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdTCM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdTCM.c  -o ${OBJECTDIR}/_ext/1719123586/cmdTCM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdTCM.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdTCM.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o: ../System/taskFlightPlan2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskFlightPlan2.c  -o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o: ../Drivers/interfaz_SPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/interfaz_SPI.c  -o ${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o: ../Drivers/pic_pc104_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/pic_pc104_config.c  -o ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/rs232_suchai.o: ../Drivers/rs232_suchai.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/rs232_suchai.c  -o ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/rs232_suchai.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/rs232_suchai.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/transceiver.o: ../Drivers/transceiver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/transceiver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/transceiver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/transceiver.c  -o ${OBJECTDIR}/_ext/1904510940/transceiver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/transceiver.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/transceiver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/console.o: ../Drivers/console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/console.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/console.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/console.c  -o ${OBJECTDIR}/_ext/1904510940/console.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/console.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/console.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/eps_suchai.o: ../Drivers/eps_suchai.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/eps_suchai.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/eps_suchai.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/eps_suchai.c  -o ${OBJECTDIR}/_ext/1904510940/eps_suchai.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/eps_suchai.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/eps_suchai.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/rtc_mb.o: ../Drivers/rtc_mb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/rtc_mb.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/rtc_mb.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/rtc_mb.c  -o ${OBJECTDIR}/_ext/1904510940/rtc_mb.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/rtc_mb.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/rtc_mb.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/memSD.o: ../Drivers/memSD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memSD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memSD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/memSD.c  -o ${OBJECTDIR}/_ext/1904510940/memSD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/memSD.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/memSD.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/memFlash.o: ../Drivers/memFlash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memFlash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memFlash.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/memFlash.c  -o ${OBJECTDIR}/_ext/1904510940/memFlash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/memFlash.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/memFlash.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/memEEPROM.o: ../Drivers/memEEPROM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memEEPROM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memEEPROM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/memEEPROM.c  -o ${OBJECTDIR}/_ext/1904510940/memEEPROM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/memEEPROM.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/memEEPROM.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o: ../Drivers/interfaz_ADC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/interfaz_ADC.c  -o ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/i2c_comm.o: ../Drivers/i2c_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/i2c_comm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1904510940/i2c_comm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/i2c_comm.c  -o ${OBJECTDIR}/_ext/1904510940/i2c_comm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/i2c_comm.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/i2c_comm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/croutine.o: ../FreeRTOS/Source/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/381897321/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/croutine.c  -o ${OBJECTDIR}/_ext/381897321/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/croutine.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/croutine.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/list.o: ../FreeRTOS/Source/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/381897321/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/list.c  -o ${OBJECTDIR}/_ext/381897321/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/list.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/list.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/queue.o: ../FreeRTOS/Source/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/381897321/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/queue.c  -o ${OBJECTDIR}/_ext/381897321/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/queue.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/queue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/tasks.o: ../FreeRTOS/Source/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/381897321/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/tasks.c  -o ${OBJECTDIR}/_ext/381897321/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/tasks.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/tasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/timers.o: ../FreeRTOS/Source/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/381897321/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/timers.c  -o ${OBJECTDIR}/_ext/381897321/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/timers.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/timers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/570733759/cmdPayload.o: ../Payloads/Cmd/cmdPayload.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/570733759 
	@${RM} ${OBJECTDIR}/_ext/570733759/cmdPayload.o.d 
	@${RM} ${OBJECTDIR}/_ext/570733759/cmdPayload.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Cmd/cmdPayload.c  -o ${OBJECTDIR}/_ext/570733759/cmdPayload.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/570733759/cmdPayload.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/570733759/cmdPayload.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/camera.o: ../Payloads/Drivers/camera.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/camera.o.d 
	@${RM} ${OBJECTDIR}/_ext/1325771184/camera.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/camera.c  -o ${OBJECTDIR}/_ext/1325771184/camera.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/camera.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/camera.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/dig_gyro.o: ../Payloads/Drivers/dig_gyro.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/dig_gyro.o.d 
	@${RM} ${OBJECTDIR}/_ext/1325771184/dig_gyro.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/dig_gyro.c  -o ${OBJECTDIR}/_ext/1325771184/dig_gyro.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/dig_gyro.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/dig_gyro.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/fis_payload.o: ../Payloads/Drivers/fis_payload.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/fis_payload.o.d 
	@${RM} ${OBJECTDIR}/_ext/1325771184/fis_payload.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/fis_payload.c  -o ${OBJECTDIR}/_ext/1325771184/fis_payload.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/fis_payload.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/fis_payload.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/sensTemp.o: ../Payloads/Drivers/sensTemp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/sensTemp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1325771184/sensTemp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/sensTemp.c  -o ${OBJECTDIR}/_ext/1325771184/sensTemp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/sensTemp.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/sensTemp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/langmuir.o: ../Payloads/Drivers/langmuir.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/langmuir.o.d 
	@${RM} ${OBJECTDIR}/_ext/1325771184/langmuir.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/langmuir.c  -o ${OBJECTDIR}/_ext/1325771184/langmuir.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/langmuir.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/langmuir.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1841791051/port.o: ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1841791051 
	@${RM} ${OBJECTDIR}/_ext/1841791051/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1841791051/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/port.c  -o ${OBJECTDIR}/_ext/1841791051/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1841791051/port.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1841791051/port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/167578668/heap_2.o: ../FreeRTOS/Source/portable/MemMang/heap_2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/167578668 
	@${RM} ${OBJECTDIR}/_ext/167578668/heap_2.o.d 
	@${RM} ${OBJECTDIR}/_ext/167578668/heap_2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/portable/MemMang/heap_2.c  -o ${OBJECTDIR}/_ext/167578668/heap_2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/167578668/heap_2.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/167578668/heap_2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskConsole.o: ../System/taskConsole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskConsole.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskConsole.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskConsole.c  -o ${OBJECTDIR}/_ext/1719123586/taskConsole.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskConsole.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskConsole.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskDispatcher.o: ../System/taskDispatcher.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskDispatcher.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskDispatcher.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskDispatcher.c  -o ${OBJECTDIR}/_ext/1719123586/taskDispatcher.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskDispatcher.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskDispatcher.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskExecuter.o: ../System/taskExecuter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskExecuter.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskExecuter.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskExecuter.c  -o ${OBJECTDIR}/_ext/1719123586/taskExecuter.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskExecuter.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskExecuter.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdPPC.o: ../System/cmdPPC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdPPC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdPPC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdPPC.c  -o ${OBJECTDIR}/_ext/1719123586/cmdPPC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdPPC.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdPPC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdConsole.o: ../System/cmdConsole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdConsole.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdConsole.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdConsole.c  -o ${OBJECTDIR}/_ext/1719123586/cmdConsole.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdConsole.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdConsole.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o: ../System/taskHousekeeping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskHousekeeping.c  -o ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o: ../System/taskFlightPlan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskFlightPlan.c  -o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/dataRepository.o: ../System/dataRepository.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/dataRepository.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/dataRepository.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/dataRepository.c  -o ${OBJECTDIR}/_ext/1719123586/dataRepository.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/dataRepository.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/dataRepository.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdRepository.o: ../System/cmdRepository.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdRepository.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdRepository.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdRepository.c  -o ${OBJECTDIR}/_ext/1719123586/cmdRepository.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdRepository.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdRepository.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdTRX.o: ../System/cmdTRX.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdTRX.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdTRX.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdTRX.c  -o ${OBJECTDIR}/_ext/1719123586/cmdTRX.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdTRX.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdTRX.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdDRP.o: ../System/cmdDRP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdDRP.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdDRP.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdDRP.c  -o ${OBJECTDIR}/_ext/1719123586/cmdDRP.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdDRP.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdDRP.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskComunications.o: ../System/taskComunications.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskComunications.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskComunications.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskComunications.c  -o ${OBJECTDIR}/_ext/1719123586/taskComunications.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskComunications.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskComunications.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskDeployment.o: ../System/taskDeployment.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskDeployment.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskDeployment.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskDeployment.c  -o ${OBJECTDIR}/_ext/1719123586/taskDeployment.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskDeployment.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskDeployment.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdEPS.o: ../System/cmdEPS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdEPS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdEPS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdEPS.c  -o ${OBJECTDIR}/_ext/1719123586/cmdEPS.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdEPS.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdEPS.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdRTC.o: ../System/cmdRTC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdRTC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdRTC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdRTC.c  -o ${OBJECTDIR}/_ext/1719123586/cmdRTC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdRTC.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdRTC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdTCM.o: ../System/cmdTCM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdTCM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdTCM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdTCM.c  -o ${OBJECTDIR}/_ext/1719123586/cmdTCM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdTCM.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdTCM.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o: ../System/taskFlightPlan2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskFlightPlan2.c  -o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"      -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o: ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1841791051 
	@${RM} ${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.d 
	@${RM} ${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S  -o ${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.d"  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -I"../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/" -Wa,-MD,"${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.d" "${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o: ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1841791051 
	@${RM} ${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.d 
	@${RM} ${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S  -o ${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.d"  -omf=elf -I"../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/" -Wa,-MD,"${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.asm.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.d" "${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/SUCHAI.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../p24FJ256GA110.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/SUCHAI.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/SUCHAI.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../p24FJ256GA110.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/SUCHAI.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/SUCHAI.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
