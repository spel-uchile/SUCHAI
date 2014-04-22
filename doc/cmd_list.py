import sys

cmd_init = {'CMD_PPC':0x1000,
            'CMD_CON':0x2000,
            'CMD_TRX':0x3000,
            'CMD_EPS':0x4000,
            'CMD_DRP':0x5000,
            'CMD_PAY':0x6000,
            'CMD_RTC':0x7000,
            'CMD_TCM':0x8000,
            }

f_name = sys.argv[1]
f = open(f_name, 'r')
cmd = 0

log = open("cmd_log.txt", 'a')

for line in f:
    if not cmd:
        if line.find('@cmd_first') != -1:
            for _key in cmd_init.keys():
                if line.find(_key) != -1:
                    cmd = cmd_init[_key]
                    line = line.replace('@cmd_first',str(hex(cmd)))
                    line = line.replace('_id_','_')
                    enum = line[:line.find('=')]
                    enum = enum.replace('_id_','_')
                    line = line[:-1] + ', referencia: @ref ' + enum + '\n'
                    cmd +=1
                    log.write("{0},{1}\n".format(str(hex(cmd)), enum.strip()))
    else:        
        if line.find('@cmd') != -1:
            line = line.replace('@cmd', str(hex(cmd)))
            line = line.replace('_id_','_')
            enum = line[:line.find(',')]
            enum = enum.replace('_id_','_')
            line = line[:-1] + ', referencia: @ref ' + enum + '\n'
            cmd +=1
            log.write("{0},{1}\n".format(str(hex(cmd)), enum.strip()))
    
    print line[:-1]

f.close()
log.close()