/**
 * @file  cmdIncludes.h
 * @author Tomas Opazo T - tomas.opazo.t@gmail.com
 * @author Carlos Gonzalez C - carlgonz@ug.uchile.cl
 * @date 2012
 * @copyright GNU GPL v3
 *
 * This header have general definitions about commands prototipes and related
 * data structures.
 */

#ifndef CMD_INCLUDES_H
#define CMD_INCLUDES_H

#include "SUCHAI_config.h"

/**
 *  Defines the prototype that every command must conform
 */
typedef int (*cmdFunction)( void * );


typedef void (*onResetFunction)( void );

/**
 * Structure that represents a command passed to executer. Contains a pointer of
 * type cmdFunction with the function to execute and one parameter for that
 * function
 */
typedef struct exec_command{
    int param;                  ///< Command parameter
    cmdFunction fnct;           ///< Command function
}ExeCmd;

/**
 * Structure that represent a command passed to dispatcher. Contains only a code
 * that represent the function to call, a paremeter and other command's metadata
 */
typedef struct ctrl_command{
    int cmdId;                  ///< Command id, represent the desired command
    int param;                  ///< Command parameter
    int idOrig;                 ///< Metadata: Id of sender subsystem
    int sysReq;                 ///< Metadata: Level of energylp the command requires
}DispCmd;


#define CMD_CMDNULL     (0xFFFF)    ///< Dummy command id. Represent a null command
#define CMD_STOP        (0xFFFE)    ///< Reserved id. Represent a stop or separation code

#define CMD_IDORIG_TCONSOLE         (0x1101)
#define CMD_IDORIG_THOUSEKEEPING    (0x1102)
#define CMD_IDORIG_TCOMUNICATIONS   (0x1103)
#define CMD_IDORIG_TFLIGHTPLAN      (0x1104)
#define CMD_IDORIG_TFLIGHTPLAN2     (0x1105)
#define CMD_IDORIG_TFLIGHTPLAN3     (0x1106)
#define CMD_IDORIG_TDEPLOYMENT      (0x1107)

#define CMD_SYSREQ_MIN      (1)     ///< Min energy level possible
#define CMD_SYSREQ_MAX      (9)     ///< Max energy level possible

#endif /* CMD_INCLUDES_H */
