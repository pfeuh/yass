#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys

def writeStateSkeleton(fsm_label, state_label):
    sys.stdout.write("\n\n")
    
    sys.stdout.write("void %s::setState%s(byte state_num, void(*change_data_hook(byte direction))\n{\n"%(fsm_label, state_label.capitalize()))
    sys.stdout.write("    state = %s_STATE_%s;\n"%(fsm_label, state_label.upper()))
    sys.stdout.write("    changeDataHook = change_data_hook;\n")
    sys.stdout.write("}\n\n")
    
    




if __name__ == "__main__":
    
    #~ fsmLabel = input("Enter fsm name")
    #~ stateLabel = input("Enter state name")
    
    for state in ("tempo", "channelin", "channelout"):
        writeStateSkeleton("YASS_CONF_FSM", state)
    