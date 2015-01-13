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

#include "cmdIncludes.h"    //for CmdFunction
#include "DebugIncludes.h"
#include "SUCHAI_config.h"

/*
extern cmdFunction tcmFunction[];
extern int trx_sysReq[];
 */

typedef struct{
    unsigned char cmdOwn;
    unsigned int nCmd;
    cmdFunction *p_xxxFunction;
    int *p_xxxSysReq;
    onResetFunction xxx_onReset;

}CmdRepo_cmdXXX_handler;


#define CMD_BUFF_CMDXX_LEN SCH_NUM_CMDXXX
void repo_set_cmdXXX_hanlder(CmdRepo_cmdXXX_handler cmdPPC_handler);
cmdFunction repo_getFunction(int cmdID);

int cmdNULL(void *param);

cmdFunction repo_getCmd(int cmdID);
int repo_getsysReq(int cmdID);

int repo_setCmd(int cmdID, cmdFunction function);
int repo_onResetCmdRepo(void);

#endif /* CMD_REPO_H */
