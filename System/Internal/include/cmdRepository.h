/**
 * @file  cmdRepoitory.h
 * @author Tomas Opazo T - tomas.opazo.t@gmail.com
 * @author Carlos Gonzalez C - carlgonz@ug.uchile.cl
 * @date 2012
 * @copyright GNU GPL v3
 *
 * Command repository
 */

#ifndef CMD_REPO_H
#define CMD_REPO_H

#include "cmdIncludes.h"
/* Add files with commands */
#include "cmdTCM.h"
#include "cmdConsole.h"
#include "cmdPPC.h"
#include "cmdTRX.h"
#include "cmdEPS.h"
#include "cmdRTC.h"
#include "cmdDRP.h"

#include "cmdPayload.h"


int cmdNULL(void *param);

cmdFunction repo_getCmd(int cmdID);
int repo_getsysReq(int cmdID);

int repo_setCmd(int cmdID, cmdFunction function);
int repo_onResetCmdRepo(void);

#endif /* CMD_REPO_H */
