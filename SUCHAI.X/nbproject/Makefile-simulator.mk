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
ifeq "$(wildcard nbproject/Makefile-local-simulator.mk)" "nbproject/Makefile-local-simulator.mk"
include nbproject/Makefile-local-simulator.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=simulator
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

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o ${OBJECTDIR}/_ext/1904510940/transceiver.o ${OBJECTDIR}/_ext/1904510940/console.o ${OBJECTDIR}/_ext/1904510940/eps_suchai.o ${OBJECTDIR}/_ext/1904510940/rtc_mb.o ${OBJECTDIR}/_ext/1904510940/memSD.o ${OBJECTDIR}/_ext/1904510940/memFlash.o ${OBJECTDIR}/_ext/1904510940/memEEPROM.o ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o ${OBJECTDIR}/_ext/1904510940/i2c_comm.o ${OBJECTDIR}/_ext/381897321/croutine.o ${OBJECTDIR}/_ext/381897321/list.o ${OBJECTDIR}/_ext/381897321/queue.o ${OBJECTDIR}/_ext/381897321/tasks.o ${OBJECTDIR}/_ext/381897321/timers.o ${OBJECTDIR}/_ext/91495067/csp_malloc.o ${OBJECTDIR}/_ext/91495067/csp_system.o ${OBJECTDIR}/_ext/91495067/csp_semaphore.o ${OBJECTDIR}/_ext/91495067/csp_time.o ${OBJECTDIR}/_ext/91495067/csp_thread.o ${OBJECTDIR}/_ext/91495067/csp_queue.o ${OBJECTDIR}/_ext/240167377/csp_xtea.o ${OBJECTDIR}/_ext/240167377/csp_hmac.o ${OBJECTDIR}/_ext/240167377/csp_sha1.o ${OBJECTDIR}/_ext/582537530/csp_i2c.o ${OBJECTDIR}/_ext/688460950/csp_if_i2c.o ${OBJECTDIR}/_ext/688460950/csp_if_lo.o ${OBJECTDIR}/_ext/1134435303/csp_rdp.o ${OBJECTDIR}/_ext/1134435303/csp_udp.o ${OBJECTDIR}/_ext/317875457/csp_service_handler.o ${OBJECTDIR}/_ext/317875457/csp_crc32.o ${OBJECTDIR}/_ext/317875457/csp_endian.o ${OBJECTDIR}/_ext/317875457/csp_buffer.o ${OBJECTDIR}/_ext/317875457/csp_services.o ${OBJECTDIR}/_ext/317875457/csp_io.o ${OBJECTDIR}/_ext/317875457/csp_conn.o ${OBJECTDIR}/_ext/317875457/csp_route.o ${OBJECTDIR}/_ext/317875457/csp_port.o ${OBJECTDIR}/_ext/317875457/csp_debug.o ${OBJECTDIR}/_ext/570733759/cmdPayload.o ${OBJECTDIR}/_ext/1325771184/camera.o ${OBJECTDIR}/_ext/1325771184/dig_gyro.o ${OBJECTDIR}/_ext/1325771184/fis_payload.o ${OBJECTDIR}/_ext/1325771184/sensTemp.o ${OBJECTDIR}/_ext/1325771184/langmuir.o ${OBJECTDIR}/_ext/1841791051/port.o ${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o ${OBJECTDIR}/_ext/167578668/heap_2.o ${OBJECTDIR}/_ext/1719123586/taskConsole.o ${OBJECTDIR}/_ext/1719123586/cmdPPC.o ${OBJECTDIR}/_ext/1719123586/cmdConsole.o ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o ${OBJECTDIR}/_ext/1719123586/cmdTRX.o ${OBJECTDIR}/_ext/1719123586/cmdDRP.o ${OBJECTDIR}/_ext/1719123586/taskComunications.o ${OBJECTDIR}/_ext/1719123586/taskDeployment.o ${OBJECTDIR}/_ext/1719123586/cmdEPS.o ${OBJECTDIR}/_ext/1719123586/cmdRTC.o ${OBJECTDIR}/_ext/1719123586/cmdTCM.o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1719123586/taskTest.o ${OBJECTDIR}/_ext/540993458/cmdRepository.o ${OBJECTDIR}/_ext/540993458/taskExecuter.o ${OBJECTDIR}/_ext/540993458/taskDispatcher.o ${OBJECTDIR}/_ext/540993458/dataRepository.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o.d ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o.d ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o.d ${OBJECTDIR}/_ext/1904510940/transceiver.o.d ${OBJECTDIR}/_ext/1904510940/console.o.d ${OBJECTDIR}/_ext/1904510940/eps_suchai.o.d ${OBJECTDIR}/_ext/1904510940/rtc_mb.o.d ${OBJECTDIR}/_ext/1904510940/memSD.o.d ${OBJECTDIR}/_ext/1904510940/memFlash.o.d ${OBJECTDIR}/_ext/1904510940/memEEPROM.o.d ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o.d ${OBJECTDIR}/_ext/1904510940/i2c_comm.o.d ${OBJECTDIR}/_ext/381897321/croutine.o.d ${OBJECTDIR}/_ext/381897321/list.o.d ${OBJECTDIR}/_ext/381897321/queue.o.d ${OBJECTDIR}/_ext/381897321/tasks.o.d ${OBJECTDIR}/_ext/381897321/timers.o.d ${OBJECTDIR}/_ext/91495067/csp_malloc.o.d ${OBJECTDIR}/_ext/91495067/csp_system.o.d ${OBJECTDIR}/_ext/91495067/csp_semaphore.o.d ${OBJECTDIR}/_ext/91495067/csp_time.o.d ${OBJECTDIR}/_ext/91495067/csp_thread.o.d ${OBJECTDIR}/_ext/91495067/csp_queue.o.d ${OBJECTDIR}/_ext/240167377/csp_xtea.o.d ${OBJECTDIR}/_ext/240167377/csp_hmac.o.d ${OBJECTDIR}/_ext/240167377/csp_sha1.o.d ${OBJECTDIR}/_ext/582537530/csp_i2c.o.d ${OBJECTDIR}/_ext/688460950/csp_if_i2c.o.d ${OBJECTDIR}/_ext/688460950/csp_if_lo.o.d ${OBJECTDIR}/_ext/1134435303/csp_rdp.o.d ${OBJECTDIR}/_ext/1134435303/csp_udp.o.d ${OBJECTDIR}/_ext/317875457/csp_service_handler.o.d ${OBJECTDIR}/_ext/317875457/csp_crc32.o.d ${OBJECTDIR}/_ext/317875457/csp_endian.o.d ${OBJECTDIR}/_ext/317875457/csp_buffer.o.d ${OBJECTDIR}/_ext/317875457/csp_services.o.d ${OBJECTDIR}/_ext/317875457/csp_io.o.d ${OBJECTDIR}/_ext/317875457/csp_conn.o.d ${OBJECTDIR}/_ext/317875457/csp_route.o.d ${OBJECTDIR}/_ext/317875457/csp_port.o.d ${OBJECTDIR}/_ext/317875457/csp_debug.o.d ${OBJECTDIR}/_ext/570733759/cmdPayload.o.d ${OBJECTDIR}/_ext/1325771184/camera.o.d ${OBJECTDIR}/_ext/1325771184/dig_gyro.o.d ${OBJECTDIR}/_ext/1325771184/fis_payload.o.d ${OBJECTDIR}/_ext/1325771184/sensTemp.o.d ${OBJECTDIR}/_ext/1325771184/langmuir.o.d ${OBJECTDIR}/_ext/1841791051/port.o.d ${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.d ${OBJECTDIR}/_ext/167578668/heap_2.o.d ${OBJECTDIR}/_ext/1719123586/taskConsole.o.d ${OBJECTDIR}/_ext/1719123586/cmdPPC.o.d ${OBJECTDIR}/_ext/1719123586/cmdConsole.o.d ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o.d ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o.d ${OBJECTDIR}/_ext/1719123586/cmdTRX.o.d ${OBJECTDIR}/_ext/1719123586/cmdDRP.o.d ${OBJECTDIR}/_ext/1719123586/taskComunications.o.d ${OBJECTDIR}/_ext/1719123586/taskDeployment.o.d ${OBJECTDIR}/_ext/1719123586/cmdEPS.o.d ${OBJECTDIR}/_ext/1719123586/cmdRTC.o.d ${OBJECTDIR}/_ext/1719123586/cmdTCM.o.d ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1719123586/taskTest.o.d ${OBJECTDIR}/_ext/540993458/cmdRepository.o.d ${OBJECTDIR}/_ext/540993458/taskExecuter.o.d ${OBJECTDIR}/_ext/540993458/taskDispatcher.o.d ${OBJECTDIR}/_ext/540993458/dataRepository.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o ${OBJECTDIR}/_ext/1904510940/transceiver.o ${OBJECTDIR}/_ext/1904510940/console.o ${OBJECTDIR}/_ext/1904510940/eps_suchai.o ${OBJECTDIR}/_ext/1904510940/rtc_mb.o ${OBJECTDIR}/_ext/1904510940/memSD.o ${OBJECTDIR}/_ext/1904510940/memFlash.o ${OBJECTDIR}/_ext/1904510940/memEEPROM.o ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o ${OBJECTDIR}/_ext/1904510940/i2c_comm.o ${OBJECTDIR}/_ext/381897321/croutine.o ${OBJECTDIR}/_ext/381897321/list.o ${OBJECTDIR}/_ext/381897321/queue.o ${OBJECTDIR}/_ext/381897321/tasks.o ${OBJECTDIR}/_ext/381897321/timers.o ${OBJECTDIR}/_ext/91495067/csp_malloc.o ${OBJECTDIR}/_ext/91495067/csp_system.o ${OBJECTDIR}/_ext/91495067/csp_semaphore.o ${OBJECTDIR}/_ext/91495067/csp_time.o ${OBJECTDIR}/_ext/91495067/csp_thread.o ${OBJECTDIR}/_ext/91495067/csp_queue.o ${OBJECTDIR}/_ext/240167377/csp_xtea.o ${OBJECTDIR}/_ext/240167377/csp_hmac.o ${OBJECTDIR}/_ext/240167377/csp_sha1.o ${OBJECTDIR}/_ext/582537530/csp_i2c.o ${OBJECTDIR}/_ext/688460950/csp_if_i2c.o ${OBJECTDIR}/_ext/688460950/csp_if_lo.o ${OBJECTDIR}/_ext/1134435303/csp_rdp.o ${OBJECTDIR}/_ext/1134435303/csp_udp.o ${OBJECTDIR}/_ext/317875457/csp_service_handler.o ${OBJECTDIR}/_ext/317875457/csp_crc32.o ${OBJECTDIR}/_ext/317875457/csp_endian.o ${OBJECTDIR}/_ext/317875457/csp_buffer.o ${OBJECTDIR}/_ext/317875457/csp_services.o ${OBJECTDIR}/_ext/317875457/csp_io.o ${OBJECTDIR}/_ext/317875457/csp_conn.o ${OBJECTDIR}/_ext/317875457/csp_route.o ${OBJECTDIR}/_ext/317875457/csp_port.o ${OBJECTDIR}/_ext/317875457/csp_debug.o ${OBJECTDIR}/_ext/570733759/cmdPayload.o ${OBJECTDIR}/_ext/1325771184/camera.o ${OBJECTDIR}/_ext/1325771184/dig_gyro.o ${OBJECTDIR}/_ext/1325771184/fis_payload.o ${OBJECTDIR}/_ext/1325771184/sensTemp.o ${OBJECTDIR}/_ext/1325771184/langmuir.o ${OBJECTDIR}/_ext/1841791051/port.o ${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o ${OBJECTDIR}/_ext/167578668/heap_2.o ${OBJECTDIR}/_ext/1719123586/taskConsole.o ${OBJECTDIR}/_ext/1719123586/cmdPPC.o ${OBJECTDIR}/_ext/1719123586/cmdConsole.o ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o ${OBJECTDIR}/_ext/1719123586/cmdTRX.o ${OBJECTDIR}/_ext/1719123586/cmdDRP.o ${OBJECTDIR}/_ext/1719123586/taskComunications.o ${OBJECTDIR}/_ext/1719123586/taskDeployment.o ${OBJECTDIR}/_ext/1719123586/cmdEPS.o ${OBJECTDIR}/_ext/1719123586/cmdRTC.o ${OBJECTDIR}/_ext/1719123586/cmdTCM.o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1719123586/taskTest.o ${OBJECTDIR}/_ext/540993458/cmdRepository.o ${OBJECTDIR}/_ext/540993458/taskExecuter.o ${OBJECTDIR}/_ext/540993458/taskDispatcher.o ${OBJECTDIR}/_ext/540993458/dataRepository.o


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
	${MAKE}  -f nbproject/Makefile-simulator.mk dist/${CND_CONF}/${IMAGE_TYPE}/SUCHAI.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ256GA110
MP_LINKER_FILE_OPTION=,--script="../p24FJ256GA110.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o: ../Drivers/interfaz_SPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/interfaz_SPI.c  -o ${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o: ../Drivers/pic_pc104_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/pic_pc104_config.c  -o ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/rs232_suchai.o: ../Drivers/rs232_suchai.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/rs232_suchai.c  -o ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/rs232_suchai.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/rs232_suchai.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/transceiver.o: ../Drivers/transceiver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/transceiver.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/transceiver.c  -o ${OBJECTDIR}/_ext/1904510940/transceiver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/transceiver.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/transceiver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/console.o: ../Drivers/console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/console.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/console.c  -o ${OBJECTDIR}/_ext/1904510940/console.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/console.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/console.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/eps_suchai.o: ../Drivers/eps_suchai.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/eps_suchai.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/eps_suchai.c  -o ${OBJECTDIR}/_ext/1904510940/eps_suchai.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/eps_suchai.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/eps_suchai.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/rtc_mb.o: ../Drivers/rtc_mb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/rtc_mb.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/rtc_mb.c  -o ${OBJECTDIR}/_ext/1904510940/rtc_mb.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/rtc_mb.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/rtc_mb.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/memSD.o: ../Drivers/memSD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memSD.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/memSD.c  -o ${OBJECTDIR}/_ext/1904510940/memSD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/memSD.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/memSD.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/memFlash.o: ../Drivers/memFlash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memFlash.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/memFlash.c  -o ${OBJECTDIR}/_ext/1904510940/memFlash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/memFlash.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/memFlash.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/memEEPROM.o: ../Drivers/memEEPROM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memEEPROM.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/memEEPROM.c  -o ${OBJECTDIR}/_ext/1904510940/memEEPROM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/memEEPROM.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/memEEPROM.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o: ../Drivers/interfaz_ADC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/interfaz_ADC.c  -o ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/i2c_comm.o: ../Drivers/i2c_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/i2c_comm.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/i2c_comm.c  -o ${OBJECTDIR}/_ext/1904510940/i2c_comm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/i2c_comm.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/i2c_comm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/croutine.o: ../FreeRTOS/Source/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/croutine.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/croutine.c  -o ${OBJECTDIR}/_ext/381897321/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/croutine.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/croutine.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/list.o: ../FreeRTOS/Source/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/list.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/list.c  -o ${OBJECTDIR}/_ext/381897321/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/list.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/list.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/queue.o: ../FreeRTOS/Source/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/queue.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/queue.c  -o ${OBJECTDIR}/_ext/381897321/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/queue.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/queue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/tasks.o: ../FreeRTOS/Source/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/tasks.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/tasks.c  -o ${OBJECTDIR}/_ext/381897321/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/tasks.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/tasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/timers.o: ../FreeRTOS/Source/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/timers.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/timers.c  -o ${OBJECTDIR}/_ext/381897321/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/timers.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/timers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/91495067/csp_malloc.o: ../Libcsp/src/arch/freertos/csp_malloc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/91495067 
	@${RM} ${OBJECTDIR}/_ext/91495067/csp_malloc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/arch/freertos/csp_malloc.c  -o ${OBJECTDIR}/_ext/91495067/csp_malloc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/91495067/csp_malloc.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/91495067/csp_malloc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/91495067/csp_system.o: ../Libcsp/src/arch/freertos/csp_system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/91495067 
	@${RM} ${OBJECTDIR}/_ext/91495067/csp_system.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/arch/freertos/csp_system.c  -o ${OBJECTDIR}/_ext/91495067/csp_system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/91495067/csp_system.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/91495067/csp_system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/91495067/csp_semaphore.o: ../Libcsp/src/arch/freertos/csp_semaphore.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/91495067 
	@${RM} ${OBJECTDIR}/_ext/91495067/csp_semaphore.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/arch/freertos/csp_semaphore.c  -o ${OBJECTDIR}/_ext/91495067/csp_semaphore.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/91495067/csp_semaphore.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/91495067/csp_semaphore.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/91495067/csp_time.o: ../Libcsp/src/arch/freertos/csp_time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/91495067 
	@${RM} ${OBJECTDIR}/_ext/91495067/csp_time.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/arch/freertos/csp_time.c  -o ${OBJECTDIR}/_ext/91495067/csp_time.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/91495067/csp_time.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/91495067/csp_time.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/91495067/csp_thread.o: ../Libcsp/src/arch/freertos/csp_thread.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/91495067 
	@${RM} ${OBJECTDIR}/_ext/91495067/csp_thread.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/arch/freertos/csp_thread.c  -o ${OBJECTDIR}/_ext/91495067/csp_thread.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/91495067/csp_thread.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/91495067/csp_thread.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/91495067/csp_queue.o: ../Libcsp/src/arch/freertos/csp_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/91495067 
	@${RM} ${OBJECTDIR}/_ext/91495067/csp_queue.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/arch/freertos/csp_queue.c  -o ${OBJECTDIR}/_ext/91495067/csp_queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/91495067/csp_queue.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/91495067/csp_queue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/240167377/csp_xtea.o: ../Libcsp/src/crypto/csp_xtea.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/240167377 
	@${RM} ${OBJECTDIR}/_ext/240167377/csp_xtea.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/crypto/csp_xtea.c  -o ${OBJECTDIR}/_ext/240167377/csp_xtea.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/240167377/csp_xtea.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/240167377/csp_xtea.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/240167377/csp_hmac.o: ../Libcsp/src/crypto/csp_hmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/240167377 
	@${RM} ${OBJECTDIR}/_ext/240167377/csp_hmac.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/crypto/csp_hmac.c  -o ${OBJECTDIR}/_ext/240167377/csp_hmac.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/240167377/csp_hmac.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/240167377/csp_hmac.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/240167377/csp_sha1.o: ../Libcsp/src/crypto/csp_sha1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/240167377 
	@${RM} ${OBJECTDIR}/_ext/240167377/csp_sha1.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/crypto/csp_sha1.c  -o ${OBJECTDIR}/_ext/240167377/csp_sha1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/240167377/csp_sha1.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/240167377/csp_sha1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/582537530/csp_i2c.o: ../Libcsp/src/drivers/i2c/csp_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/582537530 
	@${RM} ${OBJECTDIR}/_ext/582537530/csp_i2c.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/drivers/i2c/csp_i2c.c  -o ${OBJECTDIR}/_ext/582537530/csp_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/582537530/csp_i2c.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/582537530/csp_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/688460950/csp_if_i2c.o: ../Libcsp/src/interfaces/csp_if_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/688460950 
	@${RM} ${OBJECTDIR}/_ext/688460950/csp_if_i2c.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/interfaces/csp_if_i2c.c  -o ${OBJECTDIR}/_ext/688460950/csp_if_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/688460950/csp_if_i2c.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/688460950/csp_if_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/688460950/csp_if_lo.o: ../Libcsp/src/interfaces/csp_if_lo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/688460950 
	@${RM} ${OBJECTDIR}/_ext/688460950/csp_if_lo.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/interfaces/csp_if_lo.c  -o ${OBJECTDIR}/_ext/688460950/csp_if_lo.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/688460950/csp_if_lo.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/688460950/csp_if_lo.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1134435303/csp_rdp.o: ../Libcsp/src/transport/csp_rdp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1134435303 
	@${RM} ${OBJECTDIR}/_ext/1134435303/csp_rdp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/transport/csp_rdp.c  -o ${OBJECTDIR}/_ext/1134435303/csp_rdp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1134435303/csp_rdp.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1134435303/csp_rdp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1134435303/csp_udp.o: ../Libcsp/src/transport/csp_udp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1134435303 
	@${RM} ${OBJECTDIR}/_ext/1134435303/csp_udp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/transport/csp_udp.c  -o ${OBJECTDIR}/_ext/1134435303/csp_udp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1134435303/csp_udp.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1134435303/csp_udp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_service_handler.o: ../Libcsp/src/csp_service_handler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_service_handler.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_service_handler.c  -o ${OBJECTDIR}/_ext/317875457/csp_service_handler.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_service_handler.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_service_handler.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_crc32.o: ../Libcsp/src/csp_crc32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_crc32.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_crc32.c  -o ${OBJECTDIR}/_ext/317875457/csp_crc32.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_crc32.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_crc32.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_endian.o: ../Libcsp/src/csp_endian.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_endian.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_endian.c  -o ${OBJECTDIR}/_ext/317875457/csp_endian.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_endian.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_endian.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_buffer.o: ../Libcsp/src/csp_buffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_buffer.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_buffer.c  -o ${OBJECTDIR}/_ext/317875457/csp_buffer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_buffer.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_buffer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_services.o: ../Libcsp/src/csp_services.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_services.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_services.c  -o ${OBJECTDIR}/_ext/317875457/csp_services.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_services.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_services.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_io.o: ../Libcsp/src/csp_io.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_io.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_io.c  -o ${OBJECTDIR}/_ext/317875457/csp_io.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_io.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_io.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_conn.o: ../Libcsp/src/csp_conn.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_conn.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_conn.c  -o ${OBJECTDIR}/_ext/317875457/csp_conn.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_conn.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_conn.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_route.o: ../Libcsp/src/csp_route.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_route.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_route.c  -o ${OBJECTDIR}/_ext/317875457/csp_route.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_route.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_route.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_port.o: ../Libcsp/src/csp_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_port.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_port.c  -o ${OBJECTDIR}/_ext/317875457/csp_port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_port.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_debug.o: ../Libcsp/src/csp_debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_debug.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_debug.c  -o ${OBJECTDIR}/_ext/317875457/csp_debug.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_debug.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_debug.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/570733759/cmdPayload.o: ../Payloads/Cmd/cmdPayload.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/570733759 
	@${RM} ${OBJECTDIR}/_ext/570733759/cmdPayload.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Cmd/cmdPayload.c  -o ${OBJECTDIR}/_ext/570733759/cmdPayload.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/570733759/cmdPayload.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/570733759/cmdPayload.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/camera.o: ../Payloads/Drivers/camera.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/camera.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/camera.c  -o ${OBJECTDIR}/_ext/1325771184/camera.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/camera.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/camera.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/dig_gyro.o: ../Payloads/Drivers/dig_gyro.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/dig_gyro.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/dig_gyro.c  -o ${OBJECTDIR}/_ext/1325771184/dig_gyro.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/dig_gyro.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/dig_gyro.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/fis_payload.o: ../Payloads/Drivers/fis_payload.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/fis_payload.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/fis_payload.c  -o ${OBJECTDIR}/_ext/1325771184/fis_payload.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/fis_payload.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/fis_payload.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/sensTemp.o: ../Payloads/Drivers/sensTemp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/sensTemp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/sensTemp.c  -o ${OBJECTDIR}/_ext/1325771184/sensTemp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/sensTemp.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/sensTemp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/langmuir.o: ../Payloads/Drivers/langmuir.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/langmuir.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/langmuir.c  -o ${OBJECTDIR}/_ext/1325771184/langmuir.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/langmuir.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/langmuir.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1841791051/port.o: ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1841791051 
	@${RM} ${OBJECTDIR}/_ext/1841791051/port.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/port.c  -o ${OBJECTDIR}/_ext/1841791051/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1841791051/port.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1841791051/port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/167578668/heap_2.o: ../FreeRTOS/Source/portable/MemMang/heap_2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/167578668 
	@${RM} ${OBJECTDIR}/_ext/167578668/heap_2.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/portable/MemMang/heap_2.c  -o ${OBJECTDIR}/_ext/167578668/heap_2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/167578668/heap_2.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/167578668/heap_2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskConsole.o: ../System/taskConsole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskConsole.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskConsole.c  -o ${OBJECTDIR}/_ext/1719123586/taskConsole.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskConsole.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskConsole.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdPPC.o: ../System/cmdPPC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdPPC.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdPPC.c  -o ${OBJECTDIR}/_ext/1719123586/cmdPPC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdPPC.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdPPC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdConsole.o: ../System/cmdConsole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdConsole.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdConsole.c  -o ${OBJECTDIR}/_ext/1719123586/cmdConsole.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdConsole.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdConsole.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o: ../System/taskHousekeeping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskHousekeeping.c  -o ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o: ../System/taskFlightPlan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskFlightPlan.c  -o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdTRX.o: ../System/cmdTRX.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdTRX.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdTRX.c  -o ${OBJECTDIR}/_ext/1719123586/cmdTRX.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdTRX.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdTRX.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdDRP.o: ../System/cmdDRP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdDRP.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdDRP.c  -o ${OBJECTDIR}/_ext/1719123586/cmdDRP.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdDRP.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdDRP.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskComunications.o: ../System/taskComunications.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskComunications.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskComunications.c  -o ${OBJECTDIR}/_ext/1719123586/taskComunications.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskComunications.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskComunications.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskDeployment.o: ../System/taskDeployment.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskDeployment.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskDeployment.c  -o ${OBJECTDIR}/_ext/1719123586/taskDeployment.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskDeployment.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskDeployment.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdEPS.o: ../System/cmdEPS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdEPS.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdEPS.c  -o ${OBJECTDIR}/_ext/1719123586/cmdEPS.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdEPS.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdEPS.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdRTC.o: ../System/cmdRTC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdRTC.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdRTC.c  -o ${OBJECTDIR}/_ext/1719123586/cmdRTC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdRTC.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdRTC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdTCM.o: ../System/cmdTCM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdTCM.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdTCM.c  -o ${OBJECTDIR}/_ext/1719123586/cmdTCM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdTCM.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdTCM.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o: ../System/taskFlightPlan2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskFlightPlan2.c  -o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskTest.o: ../System/taskTest.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskTest.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskTest.c  -o ${OBJECTDIR}/_ext/1719123586/taskTest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskTest.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskTest.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/540993458/cmdRepository.o: ../System/Internal/cmdRepository.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/540993458 
	@${RM} ${OBJECTDIR}/_ext/540993458/cmdRepository.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/Internal/cmdRepository.c  -o ${OBJECTDIR}/_ext/540993458/cmdRepository.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/540993458/cmdRepository.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/540993458/cmdRepository.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/540993458/taskExecuter.o: ../System/Internal/taskExecuter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/540993458 
	@${RM} ${OBJECTDIR}/_ext/540993458/taskExecuter.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/Internal/taskExecuter.c  -o ${OBJECTDIR}/_ext/540993458/taskExecuter.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/540993458/taskExecuter.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/540993458/taskExecuter.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/540993458/taskDispatcher.o: ../System/Internal/taskDispatcher.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/540993458 
	@${RM} ${OBJECTDIR}/_ext/540993458/taskDispatcher.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/Internal/taskDispatcher.c  -o ${OBJECTDIR}/_ext/540993458/taskDispatcher.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/540993458/taskDispatcher.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/540993458/taskDispatcher.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/540993458/dataRepository.o: ../System/Internal/dataRepository.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/540993458 
	@${RM} ${OBJECTDIR}/_ext/540993458/dataRepository.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/Internal/dataRepository.c  -o ${OBJECTDIR}/_ext/540993458/dataRepository.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/540993458/dataRepository.o.d"        -g -D__DEBUG   -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/540993458/dataRepository.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o: ../Drivers/interfaz_SPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/interfaz_SPI.c  -o ${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/interfaz_SPI.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o: ../Drivers/pic_pc104_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/pic_pc104_config.c  -o ${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/pic_pc104_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/rs232_suchai.o: ../Drivers/rs232_suchai.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/rs232_suchai.c  -o ${OBJECTDIR}/_ext/1904510940/rs232_suchai.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/rs232_suchai.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/rs232_suchai.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/transceiver.o: ../Drivers/transceiver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/transceiver.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/transceiver.c  -o ${OBJECTDIR}/_ext/1904510940/transceiver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/transceiver.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/transceiver.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/console.o: ../Drivers/console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/console.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/console.c  -o ${OBJECTDIR}/_ext/1904510940/console.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/console.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/console.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/eps_suchai.o: ../Drivers/eps_suchai.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/eps_suchai.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/eps_suchai.c  -o ${OBJECTDIR}/_ext/1904510940/eps_suchai.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/eps_suchai.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/eps_suchai.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/rtc_mb.o: ../Drivers/rtc_mb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/rtc_mb.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/rtc_mb.c  -o ${OBJECTDIR}/_ext/1904510940/rtc_mb.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/rtc_mb.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/rtc_mb.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/memSD.o: ../Drivers/memSD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memSD.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/memSD.c  -o ${OBJECTDIR}/_ext/1904510940/memSD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/memSD.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/memSD.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/memFlash.o: ../Drivers/memFlash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memFlash.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/memFlash.c  -o ${OBJECTDIR}/_ext/1904510940/memFlash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/memFlash.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/memFlash.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/memEEPROM.o: ../Drivers/memEEPROM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/memEEPROM.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/memEEPROM.c  -o ${OBJECTDIR}/_ext/1904510940/memEEPROM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/memEEPROM.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/memEEPROM.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o: ../Drivers/interfaz_ADC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/interfaz_ADC.c  -o ${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/interfaz_ADC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1904510940/i2c_comm.o: ../Drivers/i2c_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1904510940 
	@${RM} ${OBJECTDIR}/_ext/1904510940/i2c_comm.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Drivers/i2c_comm.c  -o ${OBJECTDIR}/_ext/1904510940/i2c_comm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1904510940/i2c_comm.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1904510940/i2c_comm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/croutine.o: ../FreeRTOS/Source/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/croutine.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/croutine.c  -o ${OBJECTDIR}/_ext/381897321/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/croutine.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/croutine.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/list.o: ../FreeRTOS/Source/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/list.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/list.c  -o ${OBJECTDIR}/_ext/381897321/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/list.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/list.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/queue.o: ../FreeRTOS/Source/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/queue.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/queue.c  -o ${OBJECTDIR}/_ext/381897321/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/queue.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/queue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/tasks.o: ../FreeRTOS/Source/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/tasks.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/tasks.c  -o ${OBJECTDIR}/_ext/381897321/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/tasks.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/tasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/381897321/timers.o: ../FreeRTOS/Source/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/381897321 
	@${RM} ${OBJECTDIR}/_ext/381897321/timers.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/timers.c  -o ${OBJECTDIR}/_ext/381897321/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/381897321/timers.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/381897321/timers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/91495067/csp_malloc.o: ../Libcsp/src/arch/freertos/csp_malloc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/91495067 
	@${RM} ${OBJECTDIR}/_ext/91495067/csp_malloc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/arch/freertos/csp_malloc.c  -o ${OBJECTDIR}/_ext/91495067/csp_malloc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/91495067/csp_malloc.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/91495067/csp_malloc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/91495067/csp_system.o: ../Libcsp/src/arch/freertos/csp_system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/91495067 
	@${RM} ${OBJECTDIR}/_ext/91495067/csp_system.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/arch/freertos/csp_system.c  -o ${OBJECTDIR}/_ext/91495067/csp_system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/91495067/csp_system.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/91495067/csp_system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/91495067/csp_semaphore.o: ../Libcsp/src/arch/freertos/csp_semaphore.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/91495067 
	@${RM} ${OBJECTDIR}/_ext/91495067/csp_semaphore.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/arch/freertos/csp_semaphore.c  -o ${OBJECTDIR}/_ext/91495067/csp_semaphore.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/91495067/csp_semaphore.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/91495067/csp_semaphore.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/91495067/csp_time.o: ../Libcsp/src/arch/freertos/csp_time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/91495067 
	@${RM} ${OBJECTDIR}/_ext/91495067/csp_time.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/arch/freertos/csp_time.c  -o ${OBJECTDIR}/_ext/91495067/csp_time.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/91495067/csp_time.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/91495067/csp_time.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/91495067/csp_thread.o: ../Libcsp/src/arch/freertos/csp_thread.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/91495067 
	@${RM} ${OBJECTDIR}/_ext/91495067/csp_thread.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/arch/freertos/csp_thread.c  -o ${OBJECTDIR}/_ext/91495067/csp_thread.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/91495067/csp_thread.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/91495067/csp_thread.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/91495067/csp_queue.o: ../Libcsp/src/arch/freertos/csp_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/91495067 
	@${RM} ${OBJECTDIR}/_ext/91495067/csp_queue.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/arch/freertos/csp_queue.c  -o ${OBJECTDIR}/_ext/91495067/csp_queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/91495067/csp_queue.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/91495067/csp_queue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/240167377/csp_xtea.o: ../Libcsp/src/crypto/csp_xtea.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/240167377 
	@${RM} ${OBJECTDIR}/_ext/240167377/csp_xtea.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/crypto/csp_xtea.c  -o ${OBJECTDIR}/_ext/240167377/csp_xtea.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/240167377/csp_xtea.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/240167377/csp_xtea.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/240167377/csp_hmac.o: ../Libcsp/src/crypto/csp_hmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/240167377 
	@${RM} ${OBJECTDIR}/_ext/240167377/csp_hmac.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/crypto/csp_hmac.c  -o ${OBJECTDIR}/_ext/240167377/csp_hmac.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/240167377/csp_hmac.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/240167377/csp_hmac.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/240167377/csp_sha1.o: ../Libcsp/src/crypto/csp_sha1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/240167377 
	@${RM} ${OBJECTDIR}/_ext/240167377/csp_sha1.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/crypto/csp_sha1.c  -o ${OBJECTDIR}/_ext/240167377/csp_sha1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/240167377/csp_sha1.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/240167377/csp_sha1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/582537530/csp_i2c.o: ../Libcsp/src/drivers/i2c/csp_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/582537530 
	@${RM} ${OBJECTDIR}/_ext/582537530/csp_i2c.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/drivers/i2c/csp_i2c.c  -o ${OBJECTDIR}/_ext/582537530/csp_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/582537530/csp_i2c.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/582537530/csp_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/688460950/csp_if_i2c.o: ../Libcsp/src/interfaces/csp_if_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/688460950 
	@${RM} ${OBJECTDIR}/_ext/688460950/csp_if_i2c.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/interfaces/csp_if_i2c.c  -o ${OBJECTDIR}/_ext/688460950/csp_if_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/688460950/csp_if_i2c.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/688460950/csp_if_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/688460950/csp_if_lo.o: ../Libcsp/src/interfaces/csp_if_lo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/688460950 
	@${RM} ${OBJECTDIR}/_ext/688460950/csp_if_lo.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/interfaces/csp_if_lo.c  -o ${OBJECTDIR}/_ext/688460950/csp_if_lo.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/688460950/csp_if_lo.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/688460950/csp_if_lo.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1134435303/csp_rdp.o: ../Libcsp/src/transport/csp_rdp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1134435303 
	@${RM} ${OBJECTDIR}/_ext/1134435303/csp_rdp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/transport/csp_rdp.c  -o ${OBJECTDIR}/_ext/1134435303/csp_rdp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1134435303/csp_rdp.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1134435303/csp_rdp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1134435303/csp_udp.o: ../Libcsp/src/transport/csp_udp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1134435303 
	@${RM} ${OBJECTDIR}/_ext/1134435303/csp_udp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/transport/csp_udp.c  -o ${OBJECTDIR}/_ext/1134435303/csp_udp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1134435303/csp_udp.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1134435303/csp_udp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_service_handler.o: ../Libcsp/src/csp_service_handler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_service_handler.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_service_handler.c  -o ${OBJECTDIR}/_ext/317875457/csp_service_handler.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_service_handler.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_service_handler.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_crc32.o: ../Libcsp/src/csp_crc32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_crc32.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_crc32.c  -o ${OBJECTDIR}/_ext/317875457/csp_crc32.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_crc32.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_crc32.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_endian.o: ../Libcsp/src/csp_endian.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_endian.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_endian.c  -o ${OBJECTDIR}/_ext/317875457/csp_endian.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_endian.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_endian.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_buffer.o: ../Libcsp/src/csp_buffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_buffer.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_buffer.c  -o ${OBJECTDIR}/_ext/317875457/csp_buffer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_buffer.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_buffer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_services.o: ../Libcsp/src/csp_services.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_services.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_services.c  -o ${OBJECTDIR}/_ext/317875457/csp_services.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_services.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_services.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_io.o: ../Libcsp/src/csp_io.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_io.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_io.c  -o ${OBJECTDIR}/_ext/317875457/csp_io.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_io.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_io.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_conn.o: ../Libcsp/src/csp_conn.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_conn.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_conn.c  -o ${OBJECTDIR}/_ext/317875457/csp_conn.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_conn.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_conn.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_route.o: ../Libcsp/src/csp_route.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_route.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_route.c  -o ${OBJECTDIR}/_ext/317875457/csp_route.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_route.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_route.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_port.o: ../Libcsp/src/csp_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_port.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_port.c  -o ${OBJECTDIR}/_ext/317875457/csp_port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_port.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/317875457/csp_debug.o: ../Libcsp/src/csp_debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/317875457 
	@${RM} ${OBJECTDIR}/_ext/317875457/csp_debug.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Libcsp/src/csp_debug.c  -o ${OBJECTDIR}/_ext/317875457/csp_debug.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/317875457/csp_debug.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/317875457/csp_debug.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/570733759/cmdPayload.o: ../Payloads/Cmd/cmdPayload.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/570733759 
	@${RM} ${OBJECTDIR}/_ext/570733759/cmdPayload.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Cmd/cmdPayload.c  -o ${OBJECTDIR}/_ext/570733759/cmdPayload.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/570733759/cmdPayload.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/570733759/cmdPayload.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/camera.o: ../Payloads/Drivers/camera.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/camera.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/camera.c  -o ${OBJECTDIR}/_ext/1325771184/camera.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/camera.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/camera.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/dig_gyro.o: ../Payloads/Drivers/dig_gyro.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/dig_gyro.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/dig_gyro.c  -o ${OBJECTDIR}/_ext/1325771184/dig_gyro.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/dig_gyro.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/dig_gyro.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/fis_payload.o: ../Payloads/Drivers/fis_payload.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/fis_payload.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/fis_payload.c  -o ${OBJECTDIR}/_ext/1325771184/fis_payload.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/fis_payload.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/fis_payload.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/sensTemp.o: ../Payloads/Drivers/sensTemp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/sensTemp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/sensTemp.c  -o ${OBJECTDIR}/_ext/1325771184/sensTemp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/sensTemp.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/sensTemp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1325771184/langmuir.o: ../Payloads/Drivers/langmuir.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1325771184 
	@${RM} ${OBJECTDIR}/_ext/1325771184/langmuir.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Payloads/Drivers/langmuir.c  -o ${OBJECTDIR}/_ext/1325771184/langmuir.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1325771184/langmuir.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1325771184/langmuir.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1841791051/port.o: ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1841791051 
	@${RM} ${OBJECTDIR}/_ext/1841791051/port.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/port.c  -o ${OBJECTDIR}/_ext/1841791051/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1841791051/port.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1841791051/port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/167578668/heap_2.o: ../FreeRTOS/Source/portable/MemMang/heap_2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/167578668 
	@${RM} ${OBJECTDIR}/_ext/167578668/heap_2.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../FreeRTOS/Source/portable/MemMang/heap_2.c  -o ${OBJECTDIR}/_ext/167578668/heap_2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/167578668/heap_2.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/167578668/heap_2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskConsole.o: ../System/taskConsole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskConsole.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskConsole.c  -o ${OBJECTDIR}/_ext/1719123586/taskConsole.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskConsole.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskConsole.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdPPC.o: ../System/cmdPPC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdPPC.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdPPC.c  -o ${OBJECTDIR}/_ext/1719123586/cmdPPC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdPPC.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdPPC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdConsole.o: ../System/cmdConsole.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdConsole.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdConsole.c  -o ${OBJECTDIR}/_ext/1719123586/cmdConsole.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdConsole.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdConsole.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o: ../System/taskHousekeeping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskHousekeeping.c  -o ${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskHousekeeping.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o: ../System/taskFlightPlan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskFlightPlan.c  -o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskFlightPlan.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdTRX.o: ../System/cmdTRX.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdTRX.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdTRX.c  -o ${OBJECTDIR}/_ext/1719123586/cmdTRX.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdTRX.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdTRX.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdDRP.o: ../System/cmdDRP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdDRP.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdDRP.c  -o ${OBJECTDIR}/_ext/1719123586/cmdDRP.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdDRP.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdDRP.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskComunications.o: ../System/taskComunications.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskComunications.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskComunications.c  -o ${OBJECTDIR}/_ext/1719123586/taskComunications.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskComunications.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskComunications.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskDeployment.o: ../System/taskDeployment.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskDeployment.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskDeployment.c  -o ${OBJECTDIR}/_ext/1719123586/taskDeployment.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskDeployment.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskDeployment.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdEPS.o: ../System/cmdEPS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdEPS.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdEPS.c  -o ${OBJECTDIR}/_ext/1719123586/cmdEPS.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdEPS.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdEPS.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdRTC.o: ../System/cmdRTC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdRTC.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdRTC.c  -o ${OBJECTDIR}/_ext/1719123586/cmdRTC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdRTC.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdRTC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/cmdTCM.o: ../System/cmdTCM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/cmdTCM.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/cmdTCM.c  -o ${OBJECTDIR}/_ext/1719123586/cmdTCM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/cmdTCM.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/cmdTCM.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o: ../System/taskFlightPlan2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskFlightPlan2.c  -o ${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskFlightPlan2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1719123586/taskTest.o: ../System/taskTest.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1719123586 
	@${RM} ${OBJECTDIR}/_ext/1719123586/taskTest.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/taskTest.c  -o ${OBJECTDIR}/_ext/1719123586/taskTest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1719123586/taskTest.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1719123586/taskTest.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/540993458/cmdRepository.o: ../System/Internal/cmdRepository.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/540993458 
	@${RM} ${OBJECTDIR}/_ext/540993458/cmdRepository.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/Internal/cmdRepository.c  -o ${OBJECTDIR}/_ext/540993458/cmdRepository.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/540993458/cmdRepository.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/540993458/cmdRepository.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/540993458/taskExecuter.o: ../System/Internal/taskExecuter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/540993458 
	@${RM} ${OBJECTDIR}/_ext/540993458/taskExecuter.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/Internal/taskExecuter.c  -o ${OBJECTDIR}/_ext/540993458/taskExecuter.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/540993458/taskExecuter.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/540993458/taskExecuter.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/540993458/taskDispatcher.o: ../System/Internal/taskDispatcher.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/540993458 
	@${RM} ${OBJECTDIR}/_ext/540993458/taskDispatcher.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/Internal/taskDispatcher.c  -o ${OBJECTDIR}/_ext/540993458/taskDispatcher.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/540993458/taskDispatcher.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/540993458/taskDispatcher.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/540993458/dataRepository.o: ../System/Internal/dataRepository.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/540993458 
	@${RM} ${OBJECTDIR}/_ext/540993458/dataRepository.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../System/Internal/dataRepository.c  -o ${OBJECTDIR}/_ext/540993458/dataRepository.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/540993458/dataRepository.o.d"        -g -omf=elf -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -O0 -fomit-frame-pointer -I".." -I"../Drivers/include" -I"../System/include" -I"../FreeRTOS/Source/include" -I"../Payloads/Cmd/include" -I"../Payloads/Drivers/include" -DMPLAB_PIC24_PORT -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/540993458/dataRepository.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
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
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S  -o ${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.d"  -D__DEBUG   -omf=elf -I"../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/" -Wa,-MD,"${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.d" "${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o: ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1841791051 
	@${RM} ${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.d 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S  -o ${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.d"  -omf=elf -I"../FreeRTOS/Source/portable/MPLAB/PIC24_dsPIC/" -Wa,-MD,"${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.asm.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.d" "${OBJECTDIR}/_ext/1841791051/portasm_PIC24.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/SUCHAI.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../p24FJ256GA110.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/SUCHAI.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG   -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,$(MP_LINKER_FILE_OPTION),--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io$(MP_EXTRA_LD_POST) 
	
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
	${RM} -r build/simulator
	${RM} -r dist/simulator

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
