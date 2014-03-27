/**
 * @file  SUCHAI_config.h
 * @author Tomas Opazo T
 * @author Carlos Gonzalez C
 * @date 04-01-2013
 * @copyright GNU Public License.
 *
 * El proposito de este header file es configurar las ditintas opciones de SUCHAI
 * en un solo archivo a traves de una serie de defines que son utilzadas por
 * el pre-procesador del compilador. Las configuraciones descritas aca, afectan
 * la forma en que funciona el sistema de vuelo del satelite SUCHAI, por ejemplo
 * activando o desactivando opciones de debug, hardware disponible y parametros
 * generales del sistema.
 */

#ifndef SUCHAI_CONFIG_H
#define	SUCHAI_CONFIG_H


/* Peripheral's config */
#define SCH_UART_DEBUG_PORT     (1) ///< 0-Habilita DB9, 1-Habilita USB
#define SCH_I2C1_ADDR           (0x02) ///< I2C 1 Slave address
#define SCH_I2C2_ADDR           (0xF3) ///< I2C 2 Slave address
#define SCH_I2C3_ADDR           (0xF4) ///< I2C 3 Slave address

/* Realtime config */
#define SCH_TASKDEPLOYMENT_SILENT_REALTIME  (0)  ///< 1=Realtime 0=Debugtime
#define SCH_TASKDEPLOYMENT_ANTENNA_REALTIME (0)  ///< 1=Realtime 0=Debugtime
#define SCH_TASKHOUSEKEEPING_REALTIME       (0)  ///< 1=Realtime 0=Debugtime
#define SCH_TASKFLIGHTPLAN_REALTIME         (0)  ///< 1=Realtime 0=Debugtime
#define SCH_FLIGHTPLAN2_REALTIME            (0)  ///< 1=Realtime 0=Debugtime

/* Bus Onboard */
#define SCH_SYSBUS_ONBOARD                  (0)  //< Onboard => I2C1 en uso
#define SCH_PAYBUS_ONBOARD                  (1)  ///< Onboard => I2C3 en uso
#define SCH_ANTENNA_ONBOARD                 (0) ///< Onboard => 3 pin(es) en modo Switch, 1 pin(es) en modo Check
#define SCH_TRX_ONBOARD                     (0) ///< 0=Ninguno, 2=HACLM12 y TRX Allspace, 1= TRX Allspace => I2C1 en uso, UART2 en uso (H1.17), 2 pin(es) en modo Check
#define SCH_RTC_ONBOARD                     (1) ///< 1 = Onboard 0 = Not Onboard
#define SCH_MEMEEPROM_ONBOARD               (1) ///< 1 = Onboard 0 = Not Onboard
#define SCH_EPS_ONBOARD                     (0) ///< Onboard => I2C1 en uso
#define SCH_MEMSD_ONBOARD                   (1) ///< Onboard => SPI3 en uso, 1 pin(es) en modo Switch (nSS)
#define SCH_MGN_ONBOARD                     (0)  ///< Onboard => 1 pin(es) en modo Switch

/* Payloads Onboard */
//#define SCH_PAYLOADS_ONBOARD                (0) ///< 0 = no payloads is onboard
#define SCH_PAY_LAGMUIR_ONBOARD             (1) ///< Ocupa UART3_en_H1.19_y_H1.20
#define SCH_PAY_FIS_ONBOARD                 (0)  ///<
#define SCH_PAY_GYRO_ONBOARD                (0)  ///< Onboard => SPI3 en uso, 1 pin(es) en modo Switch (nSS)
#define SCH_PAY_SENSTEMP_ONBOARD            (0)  ///<
#define SCH_PAY_GPS_ONBOARD                 (0)  ///< Onboard => UART4 en uso, 1 pin(es) en modo Switch
#define SCH_PAYCAM_nMEMFLASH_ONBOARD        (0)  ///< Onboard => SPI-1 asignado a la Camara. Offboard => SPI-1 asignado a memFlash
#define SCH_PAY_TEST1_ONBOARD               (0)  ///< Onboard =>
#define SCH_PAY_TEST2_ONBOARD               (0)  ///< Onboard =>

/* Software configs */
#define SCH_TASKDISPATCHER_CHECK_IF_EXECUTABLE  (0) ///< 1=Activo ( EPS montada ) 0= Inactivo ( EPS no montada SOC se lleva a MAX_SOC )

/* TRX configs */
#define SCH_COMM_NO_TC_DAYS                (1)      ///< Dias sin TC para pasar a modo RSII
#define SCH_COMM_RSSI_MIN_TIME             (10)     ///< [s] Segundos minimos que debe haber senal para considerar RSSI
#define SCH_COMM_RSSI_MAX_TIME             (20)     ///< [s] Segundos maximos que debe haber senal para considerar RSSI
#define SCH_COMM_RSSI_CNT_MAX              (60)     ///< Segundos que RSSI_CNT puede pasar sin que se actualice RSSI_MEAN
#define SCH_TRX_BEACON_PERIOD              (4*60)   ///< [s] Periodo del beacon en segundos (UINT16)
#define SCH_TRX_BEACON_WPM                 (20)     ///< Velocidad del beacon en palabras por minuto 1-255
#define SCH_TRX_BEACON_BAT_LVL             (550)    ///< Nivel de bateria minimo para transmitir beacon
#define SCH_TRX_BEACON_MODE                (2)      ///< Tipo de beacon 0=CW, 1=Cw+FM, 2=FM
#define SCH_TRX_BEACON_POSPONE             (30*60)  ///< [s] Periodo de inactividad del beacon luego de cada encendido (UINT16)
#define SCH_TRX_RX_BAUD                    (48)     /// RX Baurade 12=1200bps, 24=2400bps, 48=4800bps [48 default]
#define SCH_TRX_TX_BAUD                    (48)     /// TX Baurade 12=1200bps, 24=2400bps, 48=4800bps [48 default]
#define SCH_TRX_NODE_GND                   (10)     /// Nodo de la estacion terrena
#define SCH_TRX_PORT_TM                    (10)     /// Puerto correspondiente a telemetria (en la estacion terrena)
#define SCH_TRX_PORT_TC                    (10)     /// Puerto correspondiente a telemetria (en el software de vuelo)

#define SCH_TRX_BEACON_PWR                 (4)      ///< @deprecated Potencia beacon 0 - 24
#define SCH_TRX_TELEMETRY_PWR              (8)      ///< @deprecated Potencia telemetria 0 - 24 q
#define SCH_CMDTRX_INIT_MODE               (0x10)   ///< @deprecated Modo no beacon, si TM

#define SCH_TASKDISPATCHER_CHECK_IF_EXECUTABLE  (0)     ///< 1=Activo ( EPS montada ) 0= Inactivo ( EPS no montada SOC se lleva a MAX_SOC )
#define SCH_FLIGHTPLAN_TYPE                     (0)    ///< 0=ninguno, 1=taskFligthPlan 2=taskFlightPlan2 3=ambos
#define SCH_FLIGHTPLAN_RESOLUTION               (10)    ///< [MINUTES] Resolucion de tiempo del flight plan
#define SCH_FLIGHTPLAN_N_CMD                    ((24 * 60) / SCH_FLIGHTPLAN_RESOLUTION) ///< Total de comandos en el flight plan 24[hrs]*60[min] / RES[min]
#define SCH_FLIGHTPLAN_MEMORY                   (0)     ///< (0) buffer en  RAM, (1) en memoria SD
#define SCH_USE_FLIGHTPLAN2                     (0) ///< 1 = usar, 0 = no usar

/**
 * @def SCH_DATAREPOSITORY_MAX_BUFF_TELECMD
 * Cantidad de telecomandos en cola para ser procesados por el SUCHAI
 * Max internal (not Transceiver mem) TeleCMD buffer length considering TC in
 * format [0xTCID|0xARGM|.....|0xSTOP] in frames of 63 bytes each one. 
 * So 15 commands plus 15 args, that means 62 bytes (15 cmd + 15 param + 1 stop)
 */
#define SCH_DATAREPOSITORY_MAX_BUFF_TELECMD (10*2)
#define SCH_DAT_TC_BUFF_MEMSD               (0)   ///< 1=buffer en la memSD 0=buff en mem RAM
#define SCH_TCM_SYS_REQ                     (3)   ///< Nivel de SOC sobre SYS_REQ_MIN que requieren los comandos de telemetria
#define SCH_BCN_SYS_REQ                     (2)   ///< Nivel de SOC sobre SYS_REQ_MIN que requieren los comandos de beacon

/* Drivers config VERBOSE */
#define SCH_CONSOLE_VERBOSE                 (1)   ///< 1=verboso 0=austero
#define SCH_EPS_SUCHAI_VERBOSE              (1)   ///< 1=verboso 0=austero
#define SCH_I2C_SUCHAI_VERBOSE              (1)   ///< 1=verboso 0=austero
#define SCH_INTERFAZ_SPI_VERBOSE            (1)   ///< 1=verboso 0=austero
#define SCH_MEMFLASH_VERBOSE                (1)   ///< 1=verboso 0=austero
#define SCH_MEMSD_VERBOSE                   (1)   ///< 1=verboso 0=austero
#define SCH_PIC_PC104_CONFIG_VERBOSE        (1)   ///< 1=verboso 0=austero
#define SCH_RS232_SUCHAI_VERBOSE            (1)   ///< 1=verboso 0=austero
#define SCH_RTC_MB_VERBOSE                  (1)   ///< 1=verboso 0=austero
#define SCH_TRASNCEIVER_VERBOSE             (1)   ///< 1=verboso 0=austero
#define SCH_CAMERA_VERBOSE                  (1)   ///< 1=verboso 0=austero
#define SCH_GYRO_VERBOSE                    (1)   ///< 1=verboso 0=austero
#define SCH_FISICA_VERBOSE                  (1)   ///< 1=verboso 0=austero
#define SCH_LANGMUIR_VERBOSE                (1)   ///< 1=verboso 0=austero
#define SCH_SENS_TEMP_VERBOSE               (1)   ///< 1=verboso 0=austero

//System config VERBOSE:
#define SCH_CMDCONSOLE_VERBOSE              (1)  ///< 1=verboso 0=austero
#define SCH_CMDDRP_VERBOSE                  (1)  ///< 1=verboso 0=austero
#define SCH_CMDEPS_VERBOSE                  (1)  ///< 1=verboso 0=austero
#define SCH_CMDPAYLOAD_VERBOSE              (1)  ///< 1=verboso 0=austero
#define SCH_CMDPPC_VERBOSE                  (1)  ///< 1=verboso 0=austero
#define SCH_CMDREPOSITORY_VERBOSE           (1)  ///< 1=verboso 0=austero
#define SCH_CMDRTC_VERBOSE                  (1)  ///< 1=verboso 0=austero
#define SCH_CMDTRX_VERBOSE                  (1)  ///< 1=verboso 0=austero
#define SCH_CMDTCM_VERBOSE                  (1)  ///< 1=verboso 0=austero
#define SCH_DATAREPOSITORY_VERBOSE          (1)  ///< 1=verboso 0=austero
#define SCH_TASKCOMUNICATIONS_VERBOSE       (1)  ///< 1=verboso 0=austero
#define SCH_TASKCONSOLE_VERBOSE             (1)  ///< 1=verboso 0=austero
#define SCH_TASKDEPLOYMENT_VERBOSE          (2)  ///< 1=verboso 0=austero
#define SCH_TASKDISPATCHER_VERBOSE          (0)  ///< 1=verboso 0=austero
#define SCH_TASKEXECUTER_VERBOSE            (0)  ///< 1=verboso 0=austero
#define SCH_TASKFLIGHTPLAN_VERBOSE          (1)  ///< 1=verboso 0=austero
#define SCH_FLIGHTPLAN2_VERBOSE             (1)  ///< 1=verboso 0=austero
#define SCH_TASKHOUSEKEEPING_VERBOSE        (1)  ///< 1=verboso 0=austero

#endif	/* SUCHAI_CONFIG_H */
