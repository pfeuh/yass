 #!/usr/bin/python
# -*- coding: utf-8 -*-

import cStringIO

TAB = "    "
CR = '\n'
SEQUENCE_NB_NOTES = 16

DATA = """    // Billie Jean 
    8, YASS_SEQUENCE_CROCHE ,YASS_SEQUENCE_GATE_LONG,
    50, 45, 48, 50, 48, 45, 43, 45, 0, 0, 0, 0, 0, 0, 0, 0, // should always be 16 bytes
    // Midnight Express
    16, YASS_SEQUENCE_CROCHE, YASS_SEQUENCE_GATE_NORMAL,
    50, 50, 50, 50, 48, 50, 53, 50, 43, 45, 45, 45, 43, 45, 48, 52, // should always be 16 bytes
    // Blade runner
    4, YASS_SEQUENCE_DOUBLE_CROCHE, YASS_SEQUENCE_GATE_NORMAL,
    50, 50, 52, 53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // should always be 16 bytes
    // Dady Cool
    16, YASS_SEQUENCE_CROCHE, YASS_SEQUENCE_GATE_NORMAL,
    50, 50, 45, 50, 49, 49, 44, 49, 48, 48, 43, 48, 49, 49, 44, 49, // should always be 16 bytes
    // Marcia Bailar
    16, YASS_SEQUENCE_CROCHE, YASS_SEQUENCE_GATE_LONG,
    50, 0, 0, 54, 55, 0, 0, 51, 52, 0, 0, 56, 57, 56, 57, 52,  // should always be 16 bytes
    // Take Five
    10, YASS_SEQUENCE_CROCHE, YASS_SEQUENCE_GATE_LONG,
    50, 0, 0, 50, 0, 0, 45, 0, 45, 0, 0, 0, 0, 0, 0, 0, // should always be 16 bytes
    // Latin Bolero
    8, YASS_SEQUENCE_CROCHE, YASS_SEQUENCE_GATE_LONG,
    50, YASS_SEQUENCE_TIE, YASS_SEQUENCE_TIE, YASS_SEQUENCE_TIE, 57, YASS_SEQUENCE_REST, 45, YASS_SEQUENCE_REST, 0, 0, 0, 0, 0, 0, 0, 0, 
    // Boogie woogie
    16, YASS_SEQUENCE_CROCHE_SWING, YASS_SEQUENCE_GATE_NORMAL,
    50, 50, 54, 54, 57, 57, 59, 59, 62, 62, 59, 59, 57, 57, 54, 54, // should always be 16 bytes
    // Tubular bells
    16, YASS_SEQUENCE_DOUBLE_CROCHE, YASS_SEQUENCE_GATE_NORMAL,
    50, 50, 50, 128 , 55, 57, 62, 128, 62, 60, 57, 55, 59, 60, 53, 52, // should always be 16 bytes
    // Minor arpeggio
    12, YASS_SEQUENCE_TRIOLET_DOUBLE_CROCHE, YASS_SEQUENCE_GATE_NORMAL,
    50, 53, 57, 62 , 57, 53, 50, 45, 41, 38, 0, 0, 0, 0, 0, 0, // should always be 16 bytes
    // Major arpeggio
    12, YASS_SEQUENCE_TRIOLET_DOUBLE_CROCHE, YASS_SEQUENCE_GATE_NORMAL,
    50, 54, 57, 62 , 57, 54, 50, 45, 42, 38, 0, 0, 0, 0, 0, 0, // should always be 16 bytes
    // Diminued arpeggio
    12, YASS_SEQUENCE_TRIOLET_DOUBLE_CROCHE, YASS_SEQUENCE_GATE_NORMAL,
    50, 53, 56, 59 , 62, 65, 62, 59, 56, 53, 50, 0, 0, 0, 0, 0, // should always be 16 bytes
    // Oxygene4
    8, YASS_SEQUENCE_CROCHE_SWING, YASS_SEQUENCE_GATE_NORMAL,
    50, 45, 48, 50 , YASS_SEQUENCE_TIE, 45, 48, 45, 0, 0, 0, 0, 0, 0, 0, 0, // should always be 16 bytes"""
    
def getLines(string_file):
    return [line.strip() for line in cStringIO.StringIO(string_file).readlines()]

def getCode(string_file):
    lines = getLines(string_file)
    code_lines = []
    for line_num, line in enumerate(lines):
        line_type = line_num % 3
        if line_type == 0:
            comment = TAB + "/* ---*** %s ***--- */"%(line[3:])
        if line_type == 1:
            words = [word.strip() for word in line.split(',')]
            groove = words[1] + ","
            gateMode = words[2] + ","
            last_note = "0x%02x"%(int(words[0]) - 1) +  ","
            dataMode = "YASS_SEQUENCE_DATA_MODE_VELOCITY" +  ","
            controlChange = "0x7f,"
        if line_type == 2:
            words  = [word.strip() for word in line.split(',')][:SEQUENCE_NB_NOTES]
            notes = []
            for word in words:
                try:
                    value = int(word)
                    if value == 0:
                        word = "REST, "
                    else:
                        word  = "0x%02x, "%int(word)
                    notes.append(word)
                except:
                    word = word.replace("YASS_SEQUENCE_", "")
                    while len(word) < 4:word += ' '
                    notes.append(word + ', ' )
            data = ["0x7f, "] * SEQUENCE_NB_NOTES
            
            assert(len(notes) == SEQUENCE_NB_NOTES)
            assert(len(data) == SEQUENCE_NB_NOTES)
            
            # let's built the code
            code_lines.append(comment + CR)
            code_lines.append(TAB + "/* notes     */ " + "".join(notes) + CR)
            code_lines.append(TAB + "/* veloci.   */ " + "".join(data) + CR)
            code_lines.append(TAB + "/* groove    */ " + groove + CR)
            code_lines.append(TAB + "/* gate mode */ " + gateMode + CR)
            code_lines.append(TAB + "/* last note */ " + last_note + CR)
            code_lines.append(TAB + "/* data mode */ " + dataMode + CR)
            code_lines.append(TAB + "/* cc num    */ " + controlChange + CR)           
            code_lines.append(CR)
    return "".join(code_lines)

if __name__ == "__main__":
    
    print(getCode(DATA))
    
    
    
    
    