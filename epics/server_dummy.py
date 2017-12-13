#!/usr/bin/env python

from pcaspy import Driver, SimpleServer
import random

prefix = 'MTEST:'
pvdb = {
    'VAR1' : {
        'prec' : 3,
#        'scan' : 1,
    },
    'RUN' : {
        'type' : 'enum',
        'enums' : ['STOP', 'RUN'],
    },
}



class myDriver(Driver):
    def  __init__(self):
        super(myDriver, self).__init__()

#    def read(self, reason):
#        if reason == 'RAND':
#            value = random.random()
#        else:
#            value = self.getParam(reason)
#        return value


if __name__ == '__main__':
    server = SimpleServer()
    server.createPV(prefix, pvdb)
    driver = myDriver()

    # process CA transactions
    while True:
        server.process(0.1)
