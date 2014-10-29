/**
 * @file  cmdPayload.h
 * @author Tomas Opazo T
 * @date 2013
 * @copyright GNU Public License.
 *
 * Este header contiene las definiciones de comandos asociados a los diferentes
 * payloads. La logica de trabajo de los payloads es: tomar datos y guardarlos
 * en memoria externa de manera periodica hasta que la memoria se llena. Una vez
 * que su buffer se vacia, producto de enviar la informacion como telemtria,
 * retoma su funcionamiento.
 * Los comandos permiten controlar la logica de la maquina de estados de cada
 * payload, funciones de debugm asi como ejecutar de manera independiente cada uno.
 */

#ifndef CMDPAYLOAD_H
#define	CMDPAYLOAD_H

#include "FreeRTOS.h"
#include "queue.h"

#include "langmuir.h"
#include "fis_payload.h"
#include "camera.h"
//#include "CameraC329.h"
#include "dig_gyro.h"
#include "sensTemp.h"

#include "console.h"

#include "cmdIncludes.h"
#include "dataRepository.h"
#include "statusRepository.h"

/**
 * Lista de comandos disponibles.
 */
typedef enum{
    pay_id_test_dataRepo=((unsigned int)(SCH_CMD_PAY)*0x100), ///< @cmd_first

    pay_id_FSM_default, ///< @cmd

    pay_id_isAlive_tmEstado, ///< @cmd
    pay_id_get_state_tmEstado, ///< @cmd
    pay_id_init_tmEstado, ///< @cmd
    pay_id_take_tmEstado, ///< @cmd
    pay_id_stop_tmEstado, ///< @cmd

    pay_id_isAlive_sensTemp, ///< @cmd
    pay_id_get_state_sensTemp, ///< @cmd
    pay_id_init_sensTemp, ///< @cmd
    pay_id_take_sensTemp, ///< @cmd
    pay_id_stop_sensTemp, ///< @cmd
    pay_id_debug_sensTemp, ///< @cmd

    pay_id_isAlive_gyro, ///< @cmd
    pay_id_get_state_gyro, ///< @cmd
    pay_id_init_gyro, ///< @cmd
    pay_id_take_gyro, ///< @cmd
    pay_id_stop_gyro, ///< @cmd
    pay_id_debug_gyro, ///< @cmd

    pay_id_isAlive_camera, ///< @cmd
    pay_id_get_state_camera, ///< @cmd
    pay_id_init_camera, ///< @cmd
    pay_id_take_camera, ///< @cmd
    pay_id_stop_camera, ///< @cmd
    pay_id_takePhoto_camera, ///< @cmd
    pay_id_debug_camera, ///< @cmd

    pay_id_isAlive_gps, ///< @cmd
    pay_id_get_state_gps, ///< @cmd
    pay_id_init_gps, ///< @cmd
    pay_id_take_gps, ///< @cmd
    pay_id_stop_gps, ///< @cmd

    pay_id_isAlive_expFis, ///< @cmd
    pay_id_get_state_expFis, ///< @cmd
    pay_id_init_expFis, ///< @cmd
    pay_id_take_expFis, ///< @cmd
    pay_id_stop_expFis, ///< @cmd
    pay_id_debug_expFis, ///< @cmd

    pay_id_isAlive_battery, ///< @cmd
    pay_id_get_state_battery, ///< @cmd
    pay_id_init_battery, ///< @cmd
    pay_id_take_battery, ///< @cmd
    pay_id_stop_battery, ///< @cmd

    pay_id_isAlive_debug, ///< @cmd
    pay_id_get_state_debug, ///< @cmd
    pay_id_init_debug, ///< @cmd
    pay_id_take_debug, ///< @cmd
    pay_id_stop_debug, ///< @cmd
    
    pay_id_isAlive_lagmuirProbe, ///< @cmd
    pay_id_get_state_lagmuirProbe, ///< @cmd
    pay_id_init_lagmuirProbe, ///< @cmd
    pay_id_take_lagmuirProbe, ///< @cmd
    pay_id_stop_lagmuirProbe, ///< @cmd
    pay_id_send_to_lagimur, ///< @cmd
    pay_id_debug_langmuirProbe, ///< @cmd
            
    //*********************
    pay_id_last_one    //Elemento sin sentido, solo se utiliza para marcar el largo del arreglo
}PAY_CmdIndx;

#define PAY_NCMD ((unsigned char)pay_id_last_one)


void pay_onResetCmdPAY(void);


typedef enum{
    pay_state_pre_init=0,   ///< estado de partida (condicion inicial), no hace cosas
    pay_state_init,         ///< estado de config del Payload, lo inicializa (init)
    pay_state_take,         ///< estado de operacion nominal del Payload, toma datos
    pay_state_stop,         ///< estado de termino del ciclo del Payload,
                            ///< configuraciones de termino y post procesamiento
}PAY_State;

PAY_State pay_nextStateLogic(PAY_State pay_sem, DAT_PayloadBuff pay_i);
void pay_currentStateLogic(PAY_State pay_sem_state, DAT_PayloadBuff pay_i);

//Comandos
//Debug
int pay_test_dataRepo(void *param);
//comandos para inicializar/realizar los experimientos
int pay_FSM_default(void *param);
//battery
int pay_isAlive_battery(void *param);
int pay_get_state_battery(void *param);
int pay_init_battery(void *param);
int pay_take_battery(void *param);
int pay_stop_battery(void *param);
//debug
int pay_isAlive_debug(void *param);
int pay_get_state_debug(void *param);
int pay_init_debug(void *param);
int pay_take_debug(void *param);
int pay_stop_debug(void *param);
//expFis
int pay_isAlive_expFis(void *param);
int pay_get_state_expFis(void *param);
int pay_init_expFis(void *param);
int pay_take_expFis(void *param);
int pay_stop_expFis(void *param);
int pay_debug_expFis(void *param);
//sensTemp
int pay_isAlive_sensTemp(void *param);
int pay_get_state_sensTemp(void *param);
int pay_init_sensTemp(void *param);
int pay_take_sensTemp(void *param);
int pay_stop_sensTemp(void *param);
int pay_debug_sensTemp(void *param);
//LagmiurProbe
int pay_isAlive_lagmuirProbe(void *param);
int pay_get_state_lagmuirProbe(void *param);
int pay_init_lagmuirProbe(void *param);
int pay_take_lagmuirProbe(void *param);
int pay_stop_lagmuirProbe(void *param);
int pay_send_to_lagimur(void *param);
int pay_debug_langmuir(void *param);
BOOL pay_deploy_langmuirProbe(void);
//Gyro
int pay_isAlive_gyro(void *param);
int pay_get_state_gyro(void *param);
int pay_take_gyro(void *param);
int pay_init_gyro(void *param);
int pay_stop_gyro(void *param);
int pay_debug_gyro(void *param);
//tmEstado
int pay_isAlive_tmEstado(void *param);
int pay_get_state_tmEstado(void *param);
int pay_init_tmEstado(void *param);
int pay_take_tmEstado(void *param);
int pay_stop_tmEstado(void *param);
//Camera
int pay_isAlive_camera(void *param);
int pay_get_state_camera(void *param);
int pay_init_camera(void *param);
int pay_take_camera(void *param);
int pay_stop_camera(void *param);
int pay_debug_camera(void *param);
int pay_takePhoto_camera(void *param);
//GPS
int pay_isAlive_gps(void *param);
int pay_get_state_gps(void *param);
int pay_take_gps(void *param);
int pay_init_gps(void *param);
int pay_stop_gps(void *param);

//aux functions
BOOL pay_cam_takeAndSave_photo(int resolution, int qual, int pic_type, int mode);

#endif	/* CMDPAYLOAD_H */

