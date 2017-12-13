from ROOT import *
import time
from pcaspy import Driver, SimpleServer
import threading
from threading import Thread, Event
import sys
import zmq # for socket communication
import numpy as np
import math
import random
import os  
import pwd

#from pybar.daq.readout_utils import is_data_from_channel
#from pybar.analysis.RawDataConverter.data_interpreter import PyDataInterpreter
#from pybar.analysis.RawDataConverter.data_histograming import PyDataHistograming

#from pybar_fei4_interpreter.data_interpreter import PyDataInterpreter
#from pybar_fei4_interpreter.data_histograming import PyDataHistograming


MAX_POINTS    = 2400
MIN_UPDATE_TIME = 1


# TODO:
#   Fix prescaling so it's actually a minimum update time
#   Locate occasional segfault
#   Fix mangled tracks (prob. client side--zero nonfilled entries!)

user = pwd.getpwuid(os.getuid()).pw_name

if user == 'beastdaq':
    prefix = "BEAST:TPCDAQSTER:"
    print "I can tell you're on beastdaq"
elif user == 'TPCMaster':
    prefix = "BEAST:TPCMASTER:"
    print "I can tell you're on TPCmaster"
else:
    print "Unable to determine your host, assuming beastdaq"
    prefix = "BEAST:TPCDAQSTER:"

pvdb = {
    'Run' : { 
        'type' : 'enum',
        'enums': ['STOP', 'RUN']   
    },
    'UpdateTime' : { 
        'prec' : 3, 
        'unit' : 's', 
        'value' : 0.5     
    },
    'PACKED_TRACK' : { 
        'count': MAX_POINTS
    },
    'PACKED_TRACK.NORD' : {
        'type' : 'int',
        'value': MAX_POINTS
    },
    'PACKED_TRACK.FTVL' : {
        'type' : 'str', 
        'value': 'USHORT'
    },
} 
#
## TODO: some very light online PID (based on nentries? random pre-sampling?)
## timestamp doesn't work
## packed_track is NEVER written to CA right now

def loopServer(rate):
    while True:
        server.process(rate)

## MyDriver is a pcaspy module member 
class myDriver(Driver):
    def __init__(self):
        Driver.__init__(self)
  #      self.eid = threading.Event()
  #      self.tid = threading.Thread(target = self.pushWaveforms) 
  #      self.tid.setDaemon(True)
  #      self.tid.start()
  # I don't think we need to thread this. Just call "pushWaveforms" with the appropriate data. 
     
    def pushWaveforms(self, event):
  #      run = self.getParam('Run')
  #      updateTime = self.getParam('UpdateTime')
  #      if run:
  #          self.eid.wait(updateTime)
  #      else:
  #          self.eid.wait()
  #      run = self.getParam('Run')
  #      if not run: continue
        print "pushWaveforms with ", event[0], event[1], event[2], event[3]
	self.setParam('PACKED_TRACK',  event)
        self.setParam('PACKED_TRACK.NORD', event[2])
        self.updatePVs()

# Get data from socket, taken from pyBar online monitor
class DataWorker(Thread):
    def __init__(self, socket_addr):
        Thread.__init__(self)
        self.driver = myDriver()
        self.connect(socket_addr)
        self._stop_readout = Event()
        self.setup_raw_data_analysis()
        self.kill_received = False

    def setup_raw_data_analysis(self):
        print "Setting up pyBar data interpreter"
        self.interpreter = PyDataInterpreter()
        self.interpreter.set_warning_output(False)
        self.interpreter.set_trig_count(255)
        self.interpreter.align_at_trigger(True)        # LINUX VERSION
  #      self.interpreter.use_trigger_number(True)     # USE THIS IN WINDOWS
  #      self.interpreter.use_trigger_time_stamp(True) # USE THIS IN WINDOWS
        self.interpreter.set_FEI4B(True)

    def connect(self, socket_addr):
        print "Connect to socket", socket_addr
	self.socket_addr = socket_addr
        self.context = zmq.Context()
        self.socket_pull = self.context.socket(zmq.SUB)  # subscriber
        self.socket_pull.setsockopt(zmq.SUBSCRIBE, '')  # do not filter any data
        self.socket_pull.connect(self.socket_addr)

        # Old version
        #self.socket_pull = self.context.socket(zmq.PULL)
        #self.socket_pull.connect(self.socket_addr)
        #self.poller = zmq.Poller()  # poll needed to be able to return QThread
        #self.poller.register(self.socket_pull, zmq.POLLIN)

    def analyze_raw_data(self, raw_data, channel, ts):
        self.interpreter.interpret_raw_data(raw_data)
	data_sarray = self.interpreter.get_hits()

	print "Packing track"
        # Pack the data
	if data_sarray.shape[0] != 0 : 
		data_array =  (0x000F & np.uint64(data_sarray['tot']))          << 0  # 4 bit
		data_array += (0x01FF & np.uint64(data_sarray['row']))          << 4  # 8 bit
		data_array += (0x00FF & np.uint64(data_sarray['column']))       << 13 # 9 bit
       	        #data_array += (0x00FF & np.uint64(data_sarray['relativeBCID'])) << 21 
                data_array += (0x00FF & np.uint64(data_sarray['BCID'])) << 21 
		#obtain number of hits in the event
                nhits = len(data_sarray)

                # Add TPC number and timestamp of track (in integer ms from "now") to first two entries
                data_array=np.insert(data_array,0,ts)
                data_array=np.insert(data_array,0,channel) 
                data_array=np.insert(data_array,0,nhits)

                print data_array[0], data_array[1], data_array[2], data_array[3]
        #        print "===>", data_array
	#	print "=>", data_array.dtype
                self.driver.pushWaveforms(data_array)

        #self.histograming.add_hits(self.interpreter.get_hits())

    def run(self):
        #while(not self._stop_readout.wait(0.001)):  # use wait(), do not block here
        while not self.kill_received:
            try:
                meta_data = self.socket_pull.recv_json(flags=zmq.NOBLOCK)
                # TODO: PIPE timestamps FROM METADATA INTO WF 
		#print "meta", meta_data
                print "Trigger recieved"
            except zmq.Again:
                pass
            else:
                if meta_data:
                    name = meta_data.pop('name')
                    if name == 'ReadoutData':
                        data = self.socket_pull.recv()
                    # reconstruct numpy array
                        buf = buffer(data)
                        dtype = meta_data.pop('dtype')
                        shape = meta_data.pop('shape')
                        ts = int(math.floor((meta_data.pop('timestamp_start')-1455662200.)*1000))
                        data_array = np.frombuffer(buf, dtype=dtype).reshape(shape)
                        channel_id = 0
                        if len(data_array)>1:
                            for channel in range(1,3):
				is_data_from_ch = is_data_from_channel(channel)
				data_from_channel = data_array[is_data_from_ch(data_array)]
				if len(data_from_channel) >1:
					channel_id = channel
    # only call this 10% of the time
                                if random.random() > 0.5:
                                        self.analyze_raw_data(data_array, channel_id, ts)
 

if __name__ == '__main__':
    # Initialize CA server
    server = SimpleServer()
    server.createPV(prefix, pvdb)

  #  while True:
    serverThread = Thread(target = loopServer, args = [0.1])
    serverThread.daemon = True
    serverThread.start()

   # Open socket to pyBar process
    socket_addr='tcp://127.0.0.1:5678'
    skimThread = DataWorker(socket_addr)
    skimThread.start()
    
    while skimThread.isAlive():
        try :
            skimThread.join(1)
        except KeyboardInterrupt:
            print "killed"
            skimThread.kill_received = True

    print "Terminatored"

    # process CA transactions
   # while True:

