import argparse
import os
import json
import requests
try:
    import serial
except ModuleNotFoundError as ERR:
    print("Serial module is missing for python - {}".format(ERR))
    exit(1)
from time import sleep

class BootModeSwitch():
    def __init__(self,verbose=False,BootMode="flash"):
        self.mode = BootMode
        self.verbose = verbose
        self.ReadConfig()
        self.SerialDeviceInit()


        self.SetBootMode()

        #self.SerialWrite("pin0 1")
        #self.SerialWrite("pin0 0")
        self.PDUStateTurn(State=0)
    def SerialDeviceInit(self):
        try:
           self.device = serial.Serial(port=self.SerialDev,baudrate=self.Baudrate,timeout=1)
        except serial.serialutil.SerialException as ERR:
            print("Error during connection to serial device - {}".format(ERR))
            exit(1)
        self.log("serial.isOpen() - {}".format(self.device.isOpen()))
  
    def SetBootMode(self):
        if "flash" in self.mode:
            self.log("Flash mode - relay will be off")
            #TURN OFF PDU here
            self.SerialWrite("pin0 0")
            #Turn ON PDU here
        elif "jtag" in self.mode:
            self.log("Jtag mode - relay will be on")
            #Turn OFF PDU here
            self.SerialWrite("pin0 1")
            #Turn ON PDU here
        print(self.PINStatus)

    def SerialWrite(self,Message):
        self.device.write("")
        sleep(2)
        self.device.write("{}\n".format(Message))
        answer = ""
        print("write")
        for x in range(0,3):
            a = self.device.read(2)
            answer += a
        self.log("Answer: {}".format(answer))
        if "ok" in answer.lower().strip():
            self.log("Serial answer is OK")
            self.PINStatus =  True
        else:
            self.log("Serial answer is wrong")
            self.PINStatus = False
        return self.PINStatus


    def ReadConfig(self):
        self.log("Looking for config.json")
        self.log(os.listdir("."))    
        try:
            with open("config.json") as File:
                data = json.load(File)

            self.log(data)
            self.PDU = data["PDU"]        
            self.BANK = data["BANK"]
            self.SerialDev = data["SerialDevice"]
            self.Baudrate  = data["Baudrate"]

        except Exception as ERR:
            print("Error - {}".format(ERR))
            exit(1)

    def PDUStateTurn(self,State):
        URL = "http://www.wp.pl"
        a = requests.get(URL)
        print(a.status_code)
        print("PDU - {} Bank - {} State - {}".format(self.PDU,self.BANK,State))

    def log(self,Message):
        if self.verbose:
            print(Message)



if __name__ == "__main__":
    print("dupaa")


parser = argparse.ArgumentParser()
parser.add_argument("-v","--verbose",help="enable extra debugs",action="store_true")
parser.add_argument("mode",type=str,help="Define which bootmode should be enabled: JTAG/FLASH")
args = parser.parse_args()
if (args.verbose):
    print("Args: {}".format(args))
BootMode = str(args.mode).lower()
if  (not "flash" in BootMode) and  (not "jtag" in BootMode):
    print("Wrong bootmode. exit")
    exit(1)


du = BootModeSwitch(verbose=args.verbose,BootMode=BootMode)
