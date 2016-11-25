import sys

cmd_init = {'SCH_CMD_PPC':0x1000,
            'SCH_CMD_CON':0x2000,
            'SCH_CMD_TRX':0x3000,
            'SCH_CMD_EPS':0x4000,
            'SCH_CMD_DRP':0x5000,
            'SCH_CMD_PAY':0x6000,
            'SCH_CMD_RTC':0x7000,
            'SCH_CMD_TCM':0x8000,
            'SCH_CMD_SRP':0x9000,
            'SCH_CMD_THK':0xA000
            }

f_name = sys.argv[1]
f = open(f_name, 'r')
cmd = 0

log = open("cmd_list.txt", 'a')

for line in f:
    if line.startswith("//"):
        continue
    
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
                    log.write("{0},{1}\n".format(str(hex(cmd)), enum.strip()))
                    cmd +=1
    else:        
        if line.find('@cmd') != -1:
            line = line.replace('@cmd', str(hex(cmd)))
            line = line.replace('_id_','_')
            enum = line[:line.find(',')]
            enum = enum.replace('_id_','_')
            line = line[:-1] + ', referencia: @ref ' + enum + '\n'
            log.write("{0},{1}\n".format(str(hex(cmd)), enum.strip()))
            cmd +=1
    
    print line[:-1]

f.close()
log.close()