
class PVcreator:

    """ Class for creating a dictionary containing all the PVs """

    def __init__(self):
        self.pv_dict = {}
        self.pv_count = 0

    def __str__(self):
        for i in self.pv_dict:
            print( i + '+' + self.pv_dict[i])

    def addPV( 
            self, 
            name, 
            overload={}, 
            value=0.0,
            unit='kUnicorns',
            asyn=True,
            asg='' ) :
        
        possible_keys = {
                'low' : True,
                'high' : True,
                'count' : True,
                'enums' : True,
                'value' : True,
                'unit' : True,
                'type' : True,
                'prec' : True,
                'lolo' : True,
                'hihi' : True,
                'asyn' : True,
                'asg' : True,
                'scan' : True }

        try:
            for i in overload:
                possible_keys[i]
        except KeyError:
            print ('Yeah, you are trying to add a property: ' + str(i) + ' to a pv that is not allowed!')
            assert False


        if 'value' in overload:
            value=overload['value']
        
        if name in self.pv_dict:
            for key in self.pv_dict:
                print (key)
            print ('PV already created ' + str(name) + ' , so doing a shit')
            return
        

        elif isinstance(value, float):
            self.pv_dict[name]={'value' : value, 'type' : 'float', 'prec' : 4, 'unit' : unit, 'asyn' : asyn, 'asg' : asg }
            self.pv_dict[name].update(overload)
            print ("Adding double PV: " + name + " value: " + str(value))
            return


        elif isinstance(value, str):
            self.pv_dict[name]={'type' : 'string', 'value' : value}
            self.pv_dict[name].update(overload)
            print ('Adding str PV:    ' + name + ' value: '+ value)
            return
        

        elif isinstance(value, bool):
            self.pv_dict[name]={'type' : 'enum', 'enums' : ['False', 'True'], 'value' : value and 1 or 0}
            self.pv_dict[name].update(overload)
            print ("Adding bool PV:   " + name + " value: " + str(value))
            return
        

        elif isinstance(value, list):
            if isinstance(value[0], str):
                self.pv_dict[name]={'type' : 'enum', 'enums' : value, 'value' : 0, 'asyn' : asyn }
                self.pv_dict[name].update(overload)
                print('Adding enum PV:   ' + name + 'value: ' + str(value))
                return
                
            elif isinstance(value[0], int):
                self.pv_dict[name]={'value' : value, 'type' : 'int', 'unit' : unit, 'asyn' : asyn, 'asg' : asg }
                self.pv_dict[name].update(overload)
                print('Adding int PV:   ' + name + 'value: ' + str(value))
                return
            else:
                print ('List variable type not implemented yet')
                return
        elif isinstance(value, int):
            self.pv_dict[name]={'value' : value, 'type' : 'int', 'unit' : unit, 'asyn' : asyn, 'asg' : asg }
            print('Adding int PV:   ' + name + 'value: ' + str(value))
            return

        else:
            print("Swoopsi whoopsi, can't add PV because input is not usable")
            

        self.pv_count += 1
#        print ('PV created')
        
    def getPVs(self):
        return self.pv_dict

    def getNPVs(self):
        return self.pv_count
        

    def load_claws_variables(self):
        self.addPV('RC:STATUS', {}, ['IDLE','INTERMEDIATE','PHYSICS','OFFLINE'])
        self.addPV('GRAPH',{'count' : 27, 'unit' : '16bit int', 'scan' : 1.})
        self.addPV('GRAPH2D',{'count' : 6, 'unit' : '16bit int', 'scan' : 1.})
        self.addPV('',{'prec' : 3})
        self.addPV('CH2',{'prec' : 3})
        self.addPV('CH3',{'prec' : 3})

