CeladonCity_Gym_MapScripts:: @ 816D060
    .byte 0

CeladonCity_Gym_EventScript_16D061:: @ 816D061
    setvar VAR_0x8004, 5
    setvar VAR_0x8005, 2
    special UpdatePickStateFromSpecialVar8005
    trainerbattle 1, 417, 0, Text_197114, Text_197260, EventScript_16D0A0
    checkflag FLAG_0x293
    goto_if 0, EventScript_16D0C6
    setvar VAR_0x8004, 5
    setvar VAR_0x8005, 4
    special SetFlavorTextFlagFromSpecialVars
    loadword 0, gUnknown_81972B8
    callstd 4
    release
    end

EventScript_16D0A0:: @ 816D0A0
    setvar VAR_0x8004, 5
    setvar VAR_0x8005, 1
    special SetFlavorTextFlagFromSpecialVars
    clearflag FLAG_0x09E
    setflag FLAG_0x4B3
    setflag 2083
    setvar VAR_0x8008, 4
    call EventScript_1A6B18
    goto EventScript_16D0C6
    end

EventScript_16D0C6:: @ 816D0C6
    loadword 0, gUnknown_8197327
    callstd 4
    checkitemspace ITEM_TM19, 1
    compare_var_to_value VAR_RESULT, 0
    goto_if 1, EventScript_16D107
    additem ITEM_TM19, 1
    loadword 0, gUnknown_81973BA
    setorcopyvar VAR_0x8000, 307
    setorcopyvar VAR_0x8001, 1
    setorcopyvar VAR_0x8002, 257
    callstd 9
    setflag FLAG_0x293
    loadword 0, gUnknown_81973D7
    callstd 4
    release
    end

EventScript_16D107:: @ 816D107
    loadword 0, gUnknown_819745D
    callstd 4
    release
    end

CeladonCity_Gym_EventScript_16D111:: @ 816D111
    trainerbattle 0, 132, 0, Text_19747C, Text_1974C4
    loadword 0, gUnknown_81974D6
    callstd 6
    end

CeladonCity_Gym_EventScript_16D128:: @ 816D128
    trainerbattle 0, 133, 0, Text_1976C5, Text_1976FF
    setvar VAR_0x8004, 5
    setvar VAR_0x8005, 2
    special SetFlavorTextFlagFromSpecialVars
    loadword 0, gUnknown_8197708
    callstd 6
    end

CeladonCity_Gym_EventScript_16D14C:: @ 816D14C
    trainerbattle 0, 160, 0, Text_197595, Text_1975BB
    loadword 0, gUnknown_81975D1
    callstd 6
    end

CeladonCity_Gym_EventScript_16D163:: @ 816D163
    trainerbattle 0, 265, 0, Text_1974FA, Text_19751C
    loadword 0, gUnknown_8197527
    callstd 6
    end

CeladonCity_Gym_EventScript_16D17A:: @ 816D17A
    trainerbattle 0, 266, 0, Text_19760B, Text_197660
    setvar VAR_0x8004, 5
    setvar VAR_0x8005, 3
    special SetFlavorTextFlagFromSpecialVars
    loadword 0, gUnknown_8197664
    callstd 6
    end

CeladonCity_Gym_EventScript_16D19E:: @ 816D19E
    trainerbattle 0, 267, 0, Text_197747, Text_19777A
    loadword 0, gUnknown_8197788
    callstd 6
    end

CeladonCity_Gym_EventScript_16D1B5:: @ 816D1B5
    trainerbattle 0, 402, 0, Text_1977E5, Text_197832
    loadword 0, gUnknown_819783E
    callstd 6
    end

CeladonCity_Gym_EventScript_16D1CC:: @ 816D1CC
    lockall
    checkflag 2083
    goto_if 1, EventScript_16D1E0
    loadword 0, gUnknown_8197874
    callstd 4
    releaseall
    end

EventScript_16D1E0:: @ 816D1E0
    loadword 0, gUnknown_81978AB
    callstd 4
    releaseall
    end