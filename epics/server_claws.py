#!/usr/bin/env python

from pcaspy import Driver, SimpleServer
import random
import claws_pv as cPV



class myDriver(Driver):
    def  __init__(self):
        super(myDriver, self).__init__()

#    def read(self, reason):
#        if reason == 'RAND':
#            value = random.random()
#        else:
#            value = self.getParam(reason)
#        return value

def printBanner(text):
    ANSI_BOLD = '\033[1m'
    ANSI_RED = '\033[31m'
    ANSI_GREEN = '\033[32m'
    ANSI_RESET = '\033[0m'

    print ("#"*80)
    print ('###'+' '*28 + ANSI_BOLD + ANSI_RED + text + ANSI_RESET  + ' '*21 + '###' )
    print ('#'*80)

if __name__ == '__main__':
    printBanner('Initializing Claws IOC...')
    server = SimpleServer()
    pvdb = cPV.PVcreator()
    pvdb.load_claws_variables()



    system_prefix = 'BEAST:'
    host_prefix = 'CLAWS:'
    server.createPV( system_prefix + host_prefix, pvdb.getPVs())
    driver = myDriver()

    # process CA transactions
    try:
        while True:
            server.process(0.1)
    except KeyboardInterrupt:
        print( '\nBye!')
