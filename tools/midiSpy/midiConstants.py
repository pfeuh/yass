#!/usr/bin/env python
# coding: utf-8

import os, sys

if 1:
    MIDI_CMD_OFF  = 0x80 # note off
    MIDI_CMD_ON   = 0x90 # note on
    MIDI_CMD_CC   = 0xB0 # control change
    MIDI_CMD_PC   = 0xC0 # program change
    MIDI_CMD_PB   = 0xE0 # Pitchbend
    MIDI_CMD_SOX  = 0xF0 # (start of) exclusive
    MIDI_CMD_EOX  = 0xF7 # end of exclusive
    MIDI_RT_CLOCK = 0xF8 # midi clock 
    MIDI_RT_START = 0xFA # midi start 
    MIDI_RT_STOP  = 0xFC # midi stop
    MIDI_RT_CONT  = 0xFB # midi continue

if 1:
    MIDI_CHANNEL_1  = 0
    MIDI_CHANNEL_2  = 1
    MIDI_CHANNEL_3  = 2
    MIDI_CHANNEL_4  = 3
    MIDI_CHANNEL_5  = 4
    MIDI_CHANNEL_6  = 5
    MIDI_CHANNEL_7  = 6
    MIDI_CHANNEL_8  = 7
    MIDI_CHANNEL_9  = 8
    MIDI_CHANNEL_10 = 9
    MIDI_CHANNEL_11 = 10
    MIDI_CHANNEL_12 = 11
    MIDI_CHANNEL_13 = 12
    MIDI_CHANNEL_14 = 13
    MIDI_CHANNEL_15 = 14
    MIDI_CHANNEL_16 = 15

MIDI_NOTE = {}
if 1:
    MIDI_NOTE["C-2"] = 0
    MIDI_NOTE["C#-2"] = 1
    MIDI_NOTE["Db-2"] = 1
    MIDI_NOTE["D-2"] = 2
    MIDI_NOTE["D#-2"] = 3
    MIDI_NOTE["Eb-2"] = 3
    MIDI_NOTE["E-2"] = 4
    MIDI_NOTE["F-2"] = 5
    MIDI_NOTE["F#-2"] = 6
    MIDI_NOTE["Gb-2"] = 6
    MIDI_NOTE["G-2"] = 7
    MIDI_NOTE["G#-2"] = 8
    MIDI_NOTE["Ab-2"] = 8
    MIDI_NOTE["A-2"] = 9
    MIDI_NOTE["A#-2"] = 10
    MIDI_NOTE["Bb-2"] = 10
    MIDI_NOTE["B-2"] = 11
    MIDI_NOTE["C-1"] = 12
    MIDI_NOTE["C#-1"] = 13
    MIDI_NOTE["Db-1"] = 13
    MIDI_NOTE["D-1"] = 14
    MIDI_NOTE["D#-1"] = 15
    MIDI_NOTE["Eb-1"] = 15
    MIDI_NOTE["E-1"] = 16
    MIDI_NOTE["F-1"] = 17
    MIDI_NOTE["F#-1"] = 18
    MIDI_NOTE["Gb-1"] = 18
    MIDI_NOTE["G-1"] = 19
    MIDI_NOTE["G#-1"] = 20
    MIDI_NOTE["Ab-1"] = 20
    MIDI_NOTE["A-1"] = 21
    MIDI_NOTE["A#-1"] = 22
    MIDI_NOTE["Bb-1"] = 22
    MIDI_NOTE["B-1"] = 23
    MIDI_NOTE["C0"] = 24
    MIDI_NOTE["C#0"] = 25
    MIDI_NOTE["Db0"] = 25
    MIDI_NOTE["D0"] = 26
    MIDI_NOTE["D#0"] = 27
    MIDI_NOTE["Eb0"] = 27
    MIDI_NOTE["E0"] = 28
    MIDI_NOTE["F0"] = 29
    MIDI_NOTE["F#0"] = 30
    MIDI_NOTE["Gb0"] = 30
    MIDI_NOTE["G0"] = 31
    MIDI_NOTE["G#0"] = 32
    MIDI_NOTE["Ab0"] = 32
    MIDI_NOTE["A0"] = 33
    MIDI_NOTE["A#0"] = 34
    MIDI_NOTE["Bb0"] = 34
    MIDI_NOTE["B0"] = 35
    MIDI_NOTE["C1"] = 36
    MIDI_NOTE["C#1"] = 37
    MIDI_NOTE["Db1"] = 37
    MIDI_NOTE["D1"] = 38
    MIDI_NOTE["D#1"] = 39
    MIDI_NOTE["Eb1"] = 39
    MIDI_NOTE["E1"] = 40
    MIDI_NOTE["F1"] = 41
    MIDI_NOTE["F#1"] = 42
    MIDI_NOTE["Gb1"] = 42
    MIDI_NOTE["G1"] = 43
    MIDI_NOTE["G#1"] = 44
    MIDI_NOTE["Ab1"] = 44
    MIDI_NOTE["A1"] = 45
    MIDI_NOTE["A#1"] = 46
    MIDI_NOTE["Bb1"] = 46
    MIDI_NOTE["B1"] = 47
    MIDI_NOTE["C2"] = 48
    MIDI_NOTE["C#2"] = 49
    MIDI_NOTE["Db2"] = 49
    MIDI_NOTE["D2"] = 50
    MIDI_NOTE["D#2"] = 51
    MIDI_NOTE["Eb2"] = 51
    MIDI_NOTE["E2"] = 52
    MIDI_NOTE["F2"] = 53
    MIDI_NOTE["F#2"] = 54
    MIDI_NOTE["Gb2"] = 54
    MIDI_NOTE["G2"] = 55
    MIDI_NOTE["G#2"] = 56
    MIDI_NOTE["Ab2"] = 56
    MIDI_NOTE["A2"] = 57
    MIDI_NOTE["A#2"] = 58
    MIDI_NOTE["Bb2"] = 58
    MIDI_NOTE["B2"] = 59
    MIDI_NOTE["C3"] = 60
    MIDI_NOTE["C#3"] = 61
    MIDI_NOTE["Db3"] = 61
    MIDI_NOTE["D3"] = 62
    MIDI_NOTE["D#3"] = 63
    MIDI_NOTE["Eb3"] = 63
    MIDI_NOTE["E3"] = 64
    MIDI_NOTE["F3"] = 65
    MIDI_NOTE["F#3"] = 66
    MIDI_NOTE["Gb3"] = 66
    MIDI_NOTE["G3"] = 67
    MIDI_NOTE["G#3"] = 68
    MIDI_NOTE["Ab3"] = 68
    MIDI_NOTE["A3"] = 69
    MIDI_NOTE["A#3"] = 70
    MIDI_NOTE["Bb3"] = 70
    MIDI_NOTE["B3"] = 71
    MIDI_NOTE["C4"] = 72
    MIDI_NOTE["C#4"] = 73
    MIDI_NOTE["Db4"] = 73
    MIDI_NOTE["D4"] = 74
    MIDI_NOTE["D#4"] = 75
    MIDI_NOTE["Eb4"] = 75
    MIDI_NOTE["E4"] = 76
    MIDI_NOTE["F4"] = 77
    MIDI_NOTE["F#4"] = 78
    MIDI_NOTE["Gb4"] = 78
    MIDI_NOTE["G4"] = 79
    MIDI_NOTE["G#4"] = 80
    MIDI_NOTE["Ab4"] = 80
    MIDI_NOTE["A4"] = 81
    MIDI_NOTE["A#4"] = 82
    MIDI_NOTE["Bb4"] = 82
    MIDI_NOTE["B4"] = 83
    MIDI_NOTE["C5"] = 84
    MIDI_NOTE["C#5"] = 85
    MIDI_NOTE["Db5"] = 85
    MIDI_NOTE["D5"] = 86
    MIDI_NOTE["D#5"] = 87
    MIDI_NOTE["Eb5"] = 87
    MIDI_NOTE["E5"] = 88
    MIDI_NOTE["F5"] = 89
    MIDI_NOTE["F#5"] = 90
    MIDI_NOTE["Gb5"] = 90
    MIDI_NOTE["G5"] = 91
    MIDI_NOTE["G#5"] = 92
    MIDI_NOTE["Ab5"] = 92
    MIDI_NOTE["A5"] = 93
    MIDI_NOTE["A#5"] = 94
    MIDI_NOTE["Bb5"] = 94
    MIDI_NOTE["B5"] = 95
    MIDI_NOTE["C6"] = 96
    MIDI_NOTE["C#6"] = 97
    MIDI_NOTE["Db6"] = 97
    MIDI_NOTE["D6"] = 98
    MIDI_NOTE["D#6"] = 99
    MIDI_NOTE["Eb6"] = 99
    MIDI_NOTE["E6"] = 100
    MIDI_NOTE["F6"] = 101
    MIDI_NOTE["F#6"] = 102
    MIDI_NOTE["Gb6"] = 102
    MIDI_NOTE["G6"] = 103
    MIDI_NOTE["G#6"] = 104
    MIDI_NOTE["Ab6"] = 104
    MIDI_NOTE["A6"] = 105
    MIDI_NOTE["A#6"] = 106
    MIDI_NOTE["Bb6"] = 106
    MIDI_NOTE["B6"] = 107
    MIDI_NOTE["C7"] = 108
    MIDI_NOTE["C#7"] = 109
    MIDI_NOTE["Db7"] = 109
    MIDI_NOTE["D7"] = 110
    MIDI_NOTE["D#7"] = 111
    MIDI_NOTE["Eb7"] = 111
    MIDI_NOTE["E7"] = 112
    MIDI_NOTE["F7"] = 113
    MIDI_NOTE["F#7"] = 114
    MIDI_NOTE["Gb7"] = 114
    MIDI_NOTE["G7"] = 115
    MIDI_NOTE["G#7"] = 116
    MIDI_NOTE["Ab7"] = 116
    MIDI_NOTE["A7"] = 117
    MIDI_NOTE["A#7"] = 118
    MIDI_NOTE["Bb7"] = 118
    MIDI_NOTE["B7"] = 119
    MIDI_NOTE["C8"] = 120
    MIDI_NOTE["C#8"] = 121
    MIDI_NOTE["Db8"] = 121
    MIDI_NOTE["D8"] = 122
    MIDI_NOTE["D#8"] = 123
    MIDI_NOTE["Eb8"] = 123
    MIDI_NOTE["E8"] = 124
    MIDI_NOTE["F8"] = 125
    MIDI_NOTE["F#8"] = 126
    MIDI_NOTE["Gb8"] = 126
    MIDI_NOTE["G8"] = 127

MIDI_GM_PROGS = ['AcousticGrandPiano',
    'BrightAcousticPiano',
    'ElectricGrandPiano',
    'Honky-tonkPiano',
    'ElectricPiano1',
    'ElectricPiano2',
    'Harpsichord',
    'Clavi',
    'Celesta',
    'Glockenspiel',
    'MusicBox',
    'Vibraphone',
    'Marimba',
    'Xylophone',
    'TubularBells',
    'Dulcimer',
    'DrawbarOrgan',
    'PercussiveOrgan',
    'RockOrgan',
    'ChurchOrgan',
    'ReedOrgan',
    'Accordion',
    'Harmonica',
    'TangoAccordion',
    'AcousticGuitar(nylon)',
    'AcousticGuitar(steel)',
    'ElectricGuitar(jazz)',
    'ElectricGuitar(clean)',
    'ElectricGuitar(muted)',
    'OverdrivenGuitar',
    'DistortionGuitar',
    'Guitarharmonics',
    'AcousticBass',
    'ElectricBass(finger)',
    'ElectricBass(pick)',
    'FretlessBass',
    'SlapBass1',
    'SlapBass2',
    'SynthBass1',
    'SynthBass2',
    'Violin',
    'Viola',
    'Cello',
    'Contrabass',
    'TremoloStrings',
    'PizzicatoStrings',
    'OrchestralHarp',
    'Timpani',
    'StringEnsemble1',
    'StringEnsemble2',
    'SynthStrings1',
    'SynthStrings2',
    'ChoirAahs',
    'VoiceOohs',
    'SynthVoice',
    'OrchestraHit',
    'Trumpet',
    'Trombone',
    'Tuba',
    'MutedTrumpet',
    'FrenchHorn',
    'BrassSection',
    'SynthBrass1',
    'SynthBrass2',
    'SopranoSax',
    'AltoSax',
    'TenorSax',
    'BaritoneSax',
    'Oboe',
    'EnglishHorn',
    'Bassoon',
    'Clarinet',
    'Piccolo',
    'Flute',
    'Recorder',
    'PanFlute',
    'BlownBottle',
    'Shakuhachi',
    'Whistle',
    'Ocarina',
    'Lead1(square)',
    'Lead2(sawtooth)',
    'Lead3(calliope)',
    'Lead4(chiff)',
    'Lead5(charang)',
    'Lead6(voice)',
    'Lead7(fifths)',
    'Lead8(bass+lead)',
    'Pad1(newage)',
    'Pad2(warm)',
    'Pad3(polysynth)',
    'Pad4(choir)',
    'Pad5(bowed)',
    'Pad6(metallic)',
    'Pad7(halo)',
    'Pad8(sweep)',
    'FX1(rain)',
    'FX2(soundtrack)',
    'FX3(crystal)',
    'FX4(atmosphere)',
    'FX5(brightness)',
    'FX6(goblins)',
    'FX7(echoes)',
    'FX8(sci-fi)',
    'Sitar',
    'Banjo',
    'Shamisen',
    'Koto',
    'Kalimba',
    'Bagpipe',
    'Fiddle',
    'Shanai',
    'TinkleBell',
    'Agogo',
    'SteelDrums',
    'Woodblock',
    'TaikoDrum',
    'MelodicTom',
    'SynthDrum',
    'ReverseCymbal',
    'GuitarFretNoise',
    'BreathNoise',
    'Seashore',
    'BirdTweet',
    'TelephoneRing',
    'Helicopter',
    'Applause',
    'Gunshot']


MIDI_GM_DRUM_SOUNDS = {'000',
    '001',
    '002',
    '003',
    '004',
    '005',
    '006',
    '007',
    '008',
    '009',
    '010',
    '011',
    '012',
    '013',
    '014',
    '015',
    '016',
    '017',
    '018',
    '019',
    '020',
    '021',
    '022',
    '023',
    '024',
    '025',
    '026',
    '027',
    '028',
    '029',
    '030',
    '031',
    '032',
    '033',
    '034',
    'AcousticBassDrum',
    'BassDrum1',
    'SideStick',
    'AcousticSnare',
    'HandClap',
    'ElectricSnare',
    'LowFloorTom',
    'ClosedHiHat',
    'HighFloorTom',
    'PedalHi-Hat',
    'LowTom',
    'OpenHi-Hat',
    'Low-MidTom',
    'Hi-MidTom',
    'CrashCymbal1',
    'HighTom',
    'RideCymbal1',
    'ChineseCymbal',
    'RideBell',
    'Tambourine',
    'SplashCymbal',
    'Cowbell',
    'CrashCymbal2',
    'Vibraslap',
    'RideCymbal2',
    'HiBongo',
    'LowBongo',
    'MuteHiConga',
    'OpenHiConga',
    'LowConga',
    'HighTimbale',
    'LowTimbale',
    'HighAgogo',
    'LowAgogo',
    'Cabasa',
    'Maracas',
    'ShortWhistle',
    'LongWhistle',
    'ShortGuiro',
    'LongGuiro',
    'Claves',
    'HiWoodBlock',
    'LowWoodBlock',
    'MuteCuica',
    'OpenCuica',
    'MuteTriangle',
    'OpenTriangle',
    '082',
    '083',
    '084',
    '085',
    '086',
    '087',
    '088',
    '089',
    '090',
    '091',
    '092',
    '093',
    '094',
    '095',
    '096',
    '097',
    '098',
    '099',
    '100',
    '101',
    '102',
    '103',
    '104',
    '105',
    '106',
    '107',
    '108',
    '109',
    '110',
    '111',
    '112',
    '113',
    '114',
    '115',
    '116',
    '117',
    '118',
    '119',
    '120',
    '121',
    '122',
    '123',
    '124',
    '125',
    '126',
    '127',}
    
if __name__ == "__main__":

    print "The file %s should be imported and used by another python script.""" % os.path.basename(sys.argv[0])
