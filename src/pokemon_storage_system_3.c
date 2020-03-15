#include "global.h"
#include "gflib.h"
#include "box_party_pokemon_dropdown.h"
#include "data.h"
#include "decompress.h"
#include "dynamic_placeholder_text_util.h"
#include "event_data.h"
#include "graphics.h"
#include "help_system.h"
#include "item.h"
#include "item_menu.h"
#include "mail_data.h"
#include "menu.h"
#include "naming_screen.h"
#include "new_menu_helpers.h"
#include "pc_screen_effect.h"
#include "pokemon_icon.h"
#include "pokemon_storage_system_internal.h"
#include "pokemon_summary_screen.h"
#include "quest_log.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "trig.h"
#include "constants/items.h"
#include "constants/help_system.h"
#include "constants/songs.h"
#include "constants/flags.h"
#include "constants/vars.h"

EWRAM_DATA struct PokemonStorageSystemData *sPSSData = NULL;
EWRAM_DATA bool8 sInPartyMenu = 0;
EWRAM_DATA u8 sCurrentBoxOption = 0;
EWRAM_DATA u8 gUnknown_20397B6 = 0;
EWRAM_DATA u8 sWhichToReshow = 0;
EWRAM_DATA u8 sLastUsedBox = 0;
EWRAM_DATA u16 gUnknown_20397BA = ITEM_NONE;

void Cb_InitPSS(u8 taskId);
void Cb_ShowPSS(u8 taskId);
void Cb_ReshowPSS(u8 taskId);
void Cb_MainPSS(u8 taskId);
void Cb_ShowPartyPokemon(u8 taskId);
void Cb_HidePartyPokemon(u8 taskId);
void Cb_OnSelectedMon(u8 taskId);
void Cb_MoveMon(u8 taskId);
void Cb_PlaceMon(u8 taskId);
void Cb_ShiftMon(u8 taskId);
void Cb_WithdrawMon(u8 taskId);
void Cb_DepositMenu(u8 taskId);
void Cb_ReleaseMon(u8 taskId);
void Cb_ShowMarkMenu(u8 taskId);
void Cb_TakeItemForMoving(u8 taskId);
void Cb_GiveMovingItemToMon(u8 taskId);
void Cb_ItemToBag(u8 taskId);
void Cb_SwitchSelectedItem(u8 taskId);
void Cb_ShowItemInfo(u8 taskId);
void Cb_HandleMovingMonFromParty(u8 taskId);
void Cb_PrintCantStoreMail(u8 taskId);
void Cb_HandleBoxOptions(u8 taskId);
void Cb_HandleWallpapers(u8 taskId);
void Cb_JumpBox(u8 taskId);
void Cb_NameBox(u8 taskId);
void Cb_ShowMonSummary(u8 taskId);
void Cb_GiveItemFromBag(u8 taskId);
void Cb_OnCloseBoxPressed(u8 taskId);
void Cb_OnBPressed(u8 taskId);
void Cb_ChangeScreen(u8 taskId);
void GiveChosenBagItem(void);
void FreePSSData(void);
void SetScrollingBackground(void);
void ScrollBackground(void);
void LoadPSSMenuGfx(void);
bool8 InitPSSWindows(void);
void LoadWaveformSpritePalette(void);
void sub_808F078(void);
void sub_808F0F4(void);
void sub_808F164(void);
void RefreshCursorMonData(void);
void BoxSetMosaic(void);
void SpriteCB_CursorMon_Mosaic(struct Sprite * sprite);
bool8 BoxGetMosaic(void);
void LoadCursorMonSprite(void);
void LoadCursorMonGfx(u16 species, u32 pid);
void PrintCursorMonInfo(void);
void sub_808F5E8(void);
void sub_808F68C(void);
void SetUpHidePartyMenu(void);
bool8 HidePartyMenu(void);
void sub_808F90C(bool8 species);
void sub_808F948(void);
void sub_808F974(void);
void sub_808F99C(void);
void sub_808F9FC(void);
void sub_808FA30(u8 pos, bool8 isPartyMon);
void sub_808FAA8(void);
void SetUpDoShowPartyMenu(void);
bool8 DoShowPartyMenu(void);
void sub_808FB68(void);
void PrintStorageActionText(u8 textId);
void ShowYesNoWindow(s8 species);
void ClearBottomWindow(void);
void AddWallpaperSetsMenu(void);
void AddWallpapersMenu(u8 wallpaperSet);
void sub_808FDFC(void);
void sub_808FE54(u8 species);
void sub_808FF70(void);

const u32 gPokemonStorageScrollingBGTileset[] = INCBIN_U32("graphics/interface/pss_unk_83CE438.4bpp.lz");
const u32 gPokemonStorageScrollingBGTilemap[] = INCBIN_U32("graphics/interface/pss_unk_83CE4D0.bin.lz");
const u16 gPokemonStorageScrollingBGPalette[] = INCBIN_U16("graphics/interface/pss_unk_83CE5DC.gbapal");
const u32 gUnknown_83CE5FC[] = INCBIN_U32("graphics/interface/pss_unk_83CE5FC.bin.lz");

const u16 gUnknown_83CE6F8[] = {
    0x0101, 0x0102, 0x0103, 0x0104, 0x0105, 0x0106, 0x0107, 0x0108,
    0x0111, 0x0112, 0x0113, 0x0114, 0x0115, 0x0116, 0x0117, 0x0118,
    0x2101, 0x2102, 0x2103, 0x2104, 0x2105, 0x2106, 0x2107, 0x2108,
    0x2111, 0x2112, 0x2113, 0x2114, 0x2115, 0x2116, 0x2117, 0x2118,
};

const u16 gUnknown_83CE738[] = INCBIN_U16("graphics/interface/pss_unk_83CE738.gbapal");
const u16 gUnknown_83CE758[] = INCBIN_U16("graphics/interface/pss_unk_83CE758.gbapal");

const u16 gUnknown_83CE778[] = {
    0x014c, 0x014d, 0x014e, 0x014f, 0x0170, 0x0171, 0x0172, 0x0173, 0x0174,
    0x015c, 0x015d, 0x015e, 0x015f, 0x0180, 0x0181, 0x0182, 0x0183, 0x0184,
    0x0175, 0x0176, 0x0177, 0x0178, 0x0179, 0x017a, 0x017b, 0x017c, 0x017d,
    0x0185, 0x0186, 0x0187, 0x0188, 0x0189, 0x018a, 0x018b, 0x018c, 0x018d,
};

const u16 gUnknown_83CE7C0[] = {
    0x1140, 0x1141, 0x1141, 0x1142,
    0x1150, 0x1151, 0x1151, 0x1152,
    0x1160, 0x1161, 0x1161, 0x1162,
};

const u16 gUnknown_83CE7D8[] = {
    0x1143, 0x1144, 0x1144, 0x1145,
    0x1153, 0x1154, 0x1154, 0x1155,
    0x1163, 0x1164, 0x1164, 0x1165,
};

const u16 gUnknown_83CE7F0[] = INCBIN_U16("graphics/interface/pss_unk_83CE810.gbapal");
const u16 gUnknown_83CE810[] = INCBIN_U16("graphics/interface/pss_unk_83CE810.4bpp");
const u16 gUnknown_83CE9D0[] = INCBIN_U16("graphics/interface/pss_unk_83CE9D0.gbapal");
const u16 gUnknown_83CEA10[] = INCBIN_U16("graphics/interface/pss_unk_83CEA10.gbapal");

const struct WindowTemplate gUnknown_83CEA30[] = {
    {
        .bg = 1,
        .tilemapLeft = 0,
        .tilemapTop = 11,
        .width = 9,
        .height = 7,
        .paletteNum = 3,
        .baseBlock = 0x0c0
    }, {
        .bg = 0,
        .tilemapLeft = 11,
        .tilemapTop = 17,
        .width = 18,
        .height = 2,
        .paletteNum = 13,
        .baseBlock = 0x014
    }, {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 12,
        .width = 25,
        .height = 8,
        .paletteNum = 15,
        .baseBlock = 0x014
    }, DUMMY_WIN_TEMPLATE
};

const struct BgTemplate gUnknown_83CEA50[] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }, {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0x100
    }, {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 27,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0x000
    }, {
        .bg = 3,
        .charBaseIndex = 3,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0x000
    }
};

const struct SpritePalette gWaveformSpritePalette = {
    gUnknown_83CE7F0, TAG_PAL_WAVEFORM
};

const struct SpriteSheet gWaveformSpriteSheet = {
    gUnknown_83CE810, 0x01c0, TAG_TILE_WAVEFORM
};

const struct OamData gUnknown_83CEB88;

const struct SpriteTemplate sSpriteTemplate_CursorMon = {
    .tileTag = TAG_TILE_2,
    .paletteTag = TAG_PAL_DAC6,
    .oam = &gUnknown_83CEB88,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

const struct StorageAction gPCStorageActionTexts[] = {
    [PC_TEXT_EXIT_BOX] = {gText_ExitFromBox, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_WHAT_YOU_DO] = {gText_WhatDoYouWantToDo, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_PICK_A_THEME] = {gText_PleasePickATheme, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_PICK_A_WALLPAPER] = {gText_PickTheWallpaper, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_IS_SELECTED] = {gText_PkmnIsSelected, PC_TEXT_FMT_MON_NAME_1},
    [PC_TEXT_JUMP_TO_WHICH_BOX] = {gText_JumpToWhichBox, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_DEPOSIT_IN_WHICH_BOX] = {gText_DepositInWhichBox, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_WAS_DEPOSITED] = {gText_PkmnWasDeposited, PC_TEXT_FMT_MON_NAME_1},
    [PC_TEXT_BOX_IS_FULL] = {gText_BoxIsFull2, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_RELEASE_POKE] = {gText_ReleaseThisPokemon, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_WAS_RELEASED] = {gText_PkmnWasReleased, PC_TEXT_FMT_MON_NAME_4},
    [PC_TEXT_BYE_BYE] = {gText_ByeByePkmn, PC_TEXT_FMT_MON_NAME_6},
    [PC_TEXT_MARK_POKE] = {gText_MarkYourPkmn, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_LAST_POKE] = {gText_ThatsYourLastPkmn, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_PARTY_FULL] = {gText_YourPartysFull, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_HOLDING_POKE] = {gText_YoureHoldingAPkmn, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_WHICH_ONE_WILL_TAKE] = {gText_WhichOneWillYouTake, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_CANT_RELEASE_EGG] = {gText_YouCantReleaseAnEgg, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_CONTINUE_BOX] = {gText_ContinueBoxOperations, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_CAME_BACK] = {gText_PkmnCameBack, PC_TEXT_FMT_MON_NAME_1},
    [PC_TEXT_WORRIED] = {gText_WasItWorriedAboutYou, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_SURPRISE] = {gText_FourEllipsesExclamation, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_PLEASE_REMOVE_MAIL] = {gText_PleaseRemoveTheMail, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_IS_SELECTED2] = {gText_PkmnIsSelected, PC_TEXT_FMT_ITEM_NAME},
    [PC_TEXT_GIVE_TO_MON] = {gText_GiveToAPkmn, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_PLACED_IN_BAG] = {gText_PlacedItemInBag, PC_TEXT_FMT_ITEM_NAME},
    [PC_TEXT_BAG_FULL] = {gText_BagIsFull2, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_PUT_IN_BAG] = {gText_PutItemInBag, PC_TEXT_FMT_NORMAL},
    [PC_TEXT_ITEM_IS_HELD] = {gText_ItemIsNowHeld, PC_TEXT_FMT_ITEM_NAME},
    [PC_TEXT_CHANGED_TO_ITEM] = {gText_ChangedToNewItem, PC_TEXT_FMT_ITEM_NAME},
    [PC_TEXT_CANT_STORE_MAIL] = {gText_MailCantBeStored, PC_TEXT_FMT_NORMAL},
};

// Yes/No menu
const struct WindowTemplate sYesNoWindowTemplate = {
    .bg = 0,
    .tilemapLeft = 24,
    .tilemapTop = 11,
    .width = 5,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 0x05c
};

const struct OamData gUnknown_83CEB88 = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0x000,
    .priority = 0,
    .paletteNum = 0
};

// Waveform

const struct OamData gUnknown_83CEB90 = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x8),
    .tileNum = 0x000,
    .priority = 0,
    .paletteNum = 0
};

const union AnimCmd gUnknown_83CEB98[] = {
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_END
};

const union AnimCmd gUnknown_83CEBA0[] = {
    ANIMCMD_FRAME(2, 8),
    ANIMCMD_FRAME(4, 8),
    ANIMCMD_FRAME(6, 8),
    ANIMCMD_JUMP(0)
};

const union AnimCmd gUnknown_83CEBB0[] = {
    ANIMCMD_FRAME(8, 5),
    ANIMCMD_END
};

const union AnimCmd gUnknown_83CEBB8[] = {
    ANIMCMD_FRAME(10, 8),
    ANIMCMD_FRAME(4, 8),
    ANIMCMD_FRAME(12, 8),
    ANIMCMD_JUMP(0)
};

const union AnimCmd *const gUnknown_83CEBC8[] = {
    gUnknown_83CEB98,
    gUnknown_83CEBA0,
    gUnknown_83CEBB0,
    gUnknown_83CEBB8
};

const struct SpriteTemplate sSpriteTemplate_Waveform = {
    .tileTag = TAG_TILE_WAVEFORM,
    .paletteTag = TAG_PAL_WAVEFORM,
    .oam = &gUnknown_83CEB90,
    .anims = gUnknown_83CEBC8,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

void VblankCb_PSS(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    sub_8096BF8();
    TransferPlttBuffer();
    SetGpuReg(REG_OFFSET_BG2HOFS, sPSSData->bg2_X);
}

void Cb2_PSS(void)
{
    RunTasks();
    DoScheduledBgTilemapCopiesToVram();
    ScrollBackground();
    sub_808F99C();
    AnimateSprites();
    BuildOamBuffer();
}

void Cb2_EnterPSS(u8 boxOption)
{
    ResetTasks();
    sCurrentBoxOption = boxOption;
    sPSSData = Alloc(sizeof(struct PokemonStorageSystemData));
    if (sPSSData == NULL)
        SetMainCallback2(Cb2_ExitPSS);
    else
    {
        sPSSData->boxOption = boxOption;
        sPSSData->isReshowingPSS = FALSE;
        gUnknown_20397BA = 0;
        sPSSData->state = 0;
        sPSSData->taskId = CreateTask(Cb_InitPSS, 3);
        SetHelpContext(HELPCONTEXT_BILLS_PC);
        sLastUsedBox = StorageGetCurrentBox();
        SetMainCallback2(Cb2_PSS);
    }
}

void Cb2_ReturnToPSS(void)
{
    ResetTasks();
    sPSSData = Alloc(sizeof(struct PokemonStorageSystemData));
    if (sPSSData == NULL)
        SetMainCallback2(Cb2_ExitPSS);
    else
    {
        sPSSData->boxOption = sCurrentBoxOption;
        sPSSData->isReshowingPSS = TRUE;
        sPSSData->state = 0;
        sPSSData->taskId = CreateTask(Cb_InitPSS, 3);
        SetHelpContext(HELPCONTEXT_BILLS_PC);
        SetMainCallback2(Cb2_PSS);
    }
}

void ResetAllBgCoords(void)
{
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    SetGpuReg(REG_OFFSET_BG3HOFS, 0);
    SetGpuReg(REG_OFFSET_BG3VOFS, 0);
}

void sub_808CF10(void)
{
    ResetPaletteFade();
    ResetSpriteData();
    FreeSpriteTileRanges();
    FreeAllSpritePalettes();
    ClearDma3Requests();
    gReservedSpriteTileCount = 0x280;
    sub_8096BE4(&sPSSData->unk_0020, sPSSData->unk_0028, 8);
    gKeyRepeatStartDelay = 20;
    ClearScheduledBgCopiesToVram();
    AllocBoxPartyPokemonDropdowns(3);
    SetBoxPartyPokemonDropdownMap2(0, 1, gUnknown_83CE6F8, 8, 4);
    SetBoxPartyPokemonDropdownMap2Pos(0, 1, 0);
    sPSSData->unk_02C7 = FALSE;
}

void sub_808CF94(void)
{
    sub_8092B50();
    sInPartyMenu = sPSSData->boxOption == BOX_OPTION_DEPOSIT;
    gUnknown_20397B6 = 0;
}

void sub_808CFC4(void)
{
    if (sPSSData->boxOption == BOX_OPTION_MOVE_ITEMS)
    {
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(7, 11));
    }
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_1D_MAP);
}

void SetPSSCallback(TaskFunc newFunc)
{
    gTasks[sPSSData->taskId].func = newFunc;
    sPSSData->state = 0;
}

void Cb_InitPSS(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        SetVBlankCallback(NULL);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        sub_808CF10();
        if (sPSSData->isReshowingPSS)
        {
            switch (sWhichToReshow)
            {
            case 1:
                sub_8093660();
                break;
            case 0:
                sub_80937B4();
                break;
            case 2:
                GiveChosenBagItem();
                break;
            }
        }
        LoadPSSMenuGfx();
        LoadWaveformSpritePalette();
        break;
    case 1:
        if (!InitPSSWindows())
        {
            SetPSSCallback(Cb_ChangeScreen);
            return;
        }
        break;
    case 2:
        PutWindowTilemap(0);
        ClearWindowTilemap(1);
        CpuFill32(0, (void *)VRAM, 0x200);
        TextWindow_SetUserSelectedFrame(1, 0xB, 0xE0);
        break;
    case 3:
        ResetAllBgCoords();
        if (!sPSSData->isReshowingPSS)
            sub_808CF94();
        break;
    case 4:
        sub_808FFAC();
        if (!sPSSData->isReshowingPSS)
            sub_80922C0();
        else
            sub_8092340();
        break;
    case 5:
        if (!sub_8095050())
        {
            SetPSSCallback(Cb_ChangeScreen);
            return;
        }
        else
        {
            SetScrollingBackground();
            sub_808FB68();
        }
        break;
    case 6:
        sub_808F078();
        break;
    case 7:
        sub_808F68C();
        break;
    case 8:
        sub_80913DC(StorageGetCurrentBox());
        break;
    case 9:
        if (sub_809140C())
            return;

        if (sPSSData->boxOption != BOX_OPTION_MOVE_ITEMS)
        {
            sPSSData->field_DA4.baseTileTag = TAG_TILE_D;
            sPSSData->field_DA4.basePaletteTag = TAG_PAL_DACE;
            SetMonMarkingsMenuPointer(&sPSSData->field_DA4);
            LoadMonMarkingsFrameGfx();
        }
        else
        {
            sub_8095B5C();
            sub_808FDFC();
        }
        break;
    case 10:
        sub_808CFC4();
        if (!sPSSData->isReshowingPSS)
        {
            BlendPalettes(0xFFFFFFFF, 0x10, RGB_BLACK);
            SetPSSCallback(Cb_ShowPSS);
        }
        else
        {
            BlendPalettes(0xFFFFFFFF, 0x10, RGB_BLACK);
            SetPSSCallback(Cb_ReshowPSS);
        }
        SetVBlankCallback(VblankCb_PSS);
        return;
    default:
        return;
    }

    sPSSData->state++;
}

void Cb_ShowPSS(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        PlaySE(SE_PC_LOGIN);
        sub_80A0A48(0x14, 0, 1);
        sPSSData->state++;
        break;
    case 1:
        if (!sub_80A0A98())
            SetPSSCallback(Cb_MainPSS);
        break;
    }
}

void Cb_ReshowPSS(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        BeginNormalPaletteFade(0xFFFFFFFF, -1, 0x10, 0, RGB_BLACK);
        sPSSData->state++;
        break;
    case 1:
        if (!UpdatePaletteFade())
        {
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    }
}

void Cb_MainPSS(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        switch (HandleInput())
        {
        case 1:
            PlaySE(SE_SELECT);
            sPSSData->state = 1;
            break;
        case 5:
            if (sPSSData->boxOption != BOX_OPTION_MOVE_MONS && sPSSData->boxOption != BOX_OPTION_MOVE_ITEMS)
            {
                PrintStorageActionText(PC_TEXT_WHICH_ONE_WILL_TAKE);
                sPSSData->state = 3;
            }
            else
            {
                sub_8092B50();
                SetPSSCallback(Cb_ShowPartyPokemon);
            }
            break;
        case 6:
            if (sPSSData->boxOption == BOX_OPTION_MOVE_MONS)
            {
                if (IsMonBeingMoved() && ItemIsMail(sPSSData->cursorMonItem))
                    sPSSData->state = 5;
                else
                    SetPSSCallback(Cb_HidePartyPokemon);
            }
            else if (sPSSData->boxOption == BOX_OPTION_MOVE_ITEMS)
            {
                SetPSSCallback(Cb_HidePartyPokemon);
            }
            break;
        case 4:
            SetPSSCallback(Cb_OnCloseBoxPressed);
            break;
        case 19:
            SetPSSCallback(Cb_OnBPressed);
            break;
        case 7:
            PlaySE(SE_SELECT);
            SetPSSCallback(Cb_HandleBoxOptions);
            break;
        case 8:
            SetPSSCallback(Cb_OnSelectedMon);
            break;
        case 9:
            PlaySE(SE_SELECT);
            sPSSData->newCurrBoxId = StorageGetCurrentBox() + 1;
            if (sPSSData->newCurrBoxId >= TOTAL_BOXES_COUNT)
                sPSSData->newCurrBoxId = 0;
            if (sPSSData->boxOption != BOX_OPTION_MOVE_ITEMS)
            {
                SetUpScrollToBox(sPSSData->newCurrBoxId);
                sPSSData->state = 2;
            }
            else
            {
                sub_8094D60();
                sPSSData->state = 10;
            }
            break;
        case 10:
            PlaySE(SE_SELECT);
            sPSSData->newCurrBoxId = StorageGetCurrentBox() - 1;
            if (sPSSData->newCurrBoxId < 0)
                sPSSData->newCurrBoxId = TOTAL_BOXES_COUNT - 1;
            if (sPSSData->boxOption != BOX_OPTION_MOVE_ITEMS)
            {
                SetUpScrollToBox(sPSSData->newCurrBoxId);
                sPSSData->state = 2;
            }
            else
            {
                sub_8094D60();
                sPSSData->state = 10;
            }
            break;
        case 11:
            if (!CanMovePartyMon())
            {
                if (ItemIsMail(sPSSData->cursorMonItem))
                {
                    sPSSData->state = 5;
                }
                else
                {
                    PlaySE(SE_SELECT);
                    SetPSSCallback(Cb_DepositMenu);
                }
            }
            else
            {
                sPSSData->state = 4;
            }
            break;
        case 13:
            if (CanMovePartyMon())
            {
                sPSSData->state = 4;
            }
            else
            {
                PlaySE(SE_SELECT);
                SetPSSCallback(Cb_MoveMon);
            }
            break;
        case 14:
            if (!CanShiftMon())
            {
                sPSSData->state = 4;
            }
            else
            {
                PlaySE(SE_SELECT);
                SetPSSCallback(Cb_ShiftMon);
            }
            break;
        case 12:
            PlaySE(SE_SELECT);
            SetPSSCallback(Cb_WithdrawMon);
            break;
        case 15:
            PlaySE(SE_SELECT);
            SetPSSCallback(Cb_PlaceMon);
            break;
        case 16:
            PlaySE(SE_SELECT);
            SetPSSCallback(Cb_TakeItemForMoving);
            break;
        case 17:
            PlaySE(SE_SELECT);
            SetPSSCallback(Cb_GiveMovingItemToMon);
            break;
        case 18:
            PlaySE(SE_SELECT);
            SetPSSCallback(Cb_SwitchSelectedItem);
            break;
        case 20:
            PlaySE(SE_SELECT);
            sub_80950BC(0);
            sPSSData->state = 7;
            break;
        case 22:
            sub_80950BC(1);
            sPSSData->state = 8;
            break;
        case 21:
            PlaySE(SE_SELECT);
            sub_80950BC(2);
            sPSSData->state = 9;
            break;
        case 23:
            sub_80950BC(3);
            sPSSData->state = 7;
            break;
        case 25:
            PlaySE(SE_SELECT);
            sub_80950BC(4);
            sPSSData->state = 9;
            break;
        case 26:
            PlaySE(SE_SELECT);
            sub_808FE54(3);
            sub_80950BC(5);
            sPSSData->state = 7;
            break;
        case 24:
            PlaySE(SE_HAZURE);
            break;
        }
        break;
    case 1:
        if (!sub_80924A8())
        {
            if (IsCursorOnCloseBox())
                sub_808F948();
            else
                sub_808F974();

            if (sPSSData->setMosaic)
                BoxSetMosaic();
            sPSSData->state = 0;
        }
        break;
    case 2:
        if (!ScrollToBox())
        {
            SetCurrentBox(sPSSData->newCurrBoxId);
            if (!sInPartyMenu && !IsMonBeingMoved())
            {
                sub_8092F54();
                BoxSetMosaic();
            }

            if (sPSSData->boxOption == BOX_OPTION_MOVE_ITEMS)
            {
                sub_8094D84();
                sPSSData->state = 11;
            }
            else
            {
                sPSSData->state = 0;
            }
        }
        break;
    case 3:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            ClearBottomWindow();
            sPSSData->state = 0;
        }
        break;
    case 4:
        PlaySE(SE_HAZURE);
        PrintStorageActionText(PC_TEXT_LAST_POKE);
        sPSSData->state = 6;
        break;
    case 5:
        PlaySE(SE_HAZURE);
        PrintStorageActionText(PC_TEXT_PLEASE_REMOVE_MAIL);
        sPSSData->state = 6;
        break;
    case 6:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            ClearBottomWindow();
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    case 7:
        if (!sub_80950D0())
            sPSSData->state = 0;
        break;
    case 8:
        if (!sub_80950D0())
            SetPSSCallback(Cb_MoveMon);
        break;
    case 9:
        if (!sub_80950D0())
        {
            if (sPSSData->setMosaic)
                BoxSetMosaic();
            sPSSData->state = 0;
        }
        break;
    case 10:
        if (!sub_809610C())
        {
            SetUpScrollToBox(sPSSData->newCurrBoxId);
            sPSSData->state = 2;
        }
        break;
    case 11:
        if (!sub_809610C())
            sPSSData->state = 0;
        break;
    }
}

void Cb_ShowPartyPokemon(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        SetUpDoShowPartyMenu();
        sPSSData->state++;
        break;
    case 1:
        if (!DoShowPartyMenu())
            SetPSSCallback(Cb_MainPSS);
        break;
    }
}

void Cb_HidePartyPokemon(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        PlaySE(SE_SELECT);
        SetUpHidePartyMenu();
        sPSSData->state++;
        break;
    case 1:
        if (!HidePartyMenu())
        {
            sub_8092B3C(sub_8092B70());
            sPSSData->state++;
        }
        break;
    case 2:
        if (!sub_80924A8())
        {
            if (sPSSData->setMosaic)
                BoxSetMosaic();
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    }
}

void Cb_OnSelectedMon(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        if (!BoxGetMosaic())
        {
            PlaySE(SE_SELECT);
            if (sPSSData->boxOption != BOX_OPTION_MOVE_ITEMS)
                PrintStorageActionText(PC_TEXT_IS_SELECTED);
            else if (IsActiveItemMoving() || sPSSData->cursorMonItem != 0)
                PrintStorageActionText(PC_TEXT_IS_SELECTED2);
            else
                PrintStorageActionText(PC_TEXT_GIVE_TO_MON);

            AddMenu();
            sPSSData->state = 1;
        }
        break;
    case 1: // debug?
        if (!sub_8094F90())
            sPSSData->state = 2;
        break;
    case 2:
        switch (sub_8094F94())
        {
        case -1:
        case  0:
            ClearBottomWindow();
            SetPSSCallback(Cb_MainPSS);
            break;
        case 3:
            if (CanMovePartyMon())
            {
                sPSSData->state = 3;
            }
            else
            {
                PlaySE(SE_SELECT);
                ClearBottomWindow();
                SetPSSCallback(Cb_MoveMon);
            }
            break;
        case 5:
            PlaySE(SE_SELECT);
            ClearBottomWindow();
            SetPSSCallback(Cb_PlaceMon);
            break;
        case 4:
            if (!CanShiftMon())
            {
                sPSSData->state = 3;
            }
            else
            {
                PlaySE(SE_SELECT);
                ClearBottomWindow();
                SetPSSCallback(Cb_ShiftMon);
            }
            break;
        case 2:
            PlaySE(SE_SELECT);
            ClearBottomWindow();
            SetPSSCallback(Cb_WithdrawMon);
            break;
        case 1:
            if (CanMovePartyMon())
            {
                sPSSData->state = 3;
            }
            else if (ItemIsMail(sPSSData->cursorMonItem))
            {
                sPSSData->state = 4;
            }
            else
            {
                PlaySE(SE_SELECT);
                ClearBottomWindow();
                SetPSSCallback(Cb_DepositMenu);
            }
            break;
        case 7:
            if (CanMovePartyMon())
            {
                sPSSData->state = 3;
            }
            else if (sPSSData->cursorMonIsEgg)
            {
                sPSSData->state = 5; // Cannot release an Egg.
            }
            else if (ItemIsMail(sPSSData->cursorMonItem))
            {
                sPSSData->state = 4;
            }
            else
            {
                PlaySE(SE_SELECT);
                SetPSSCallback(Cb_ReleaseMon);
            }
            break;
        case 6:
            PlaySE(SE_SELECT);
            SetPSSCallback(Cb_ShowMonSummary);
            break;
        case 8:
            PlaySE(SE_SELECT);
            SetPSSCallback(Cb_ShowMarkMenu);
            break;
        case 12:
            PlaySE(SE_SELECT);
            SetPSSCallback(Cb_TakeItemForMoving);
            break;
        case 13:
            PlaySE(SE_SELECT);
            SetPSSCallback(Cb_GiveMovingItemToMon);
            break;
        case 16:
            SetPSSCallback(Cb_ItemToBag);
            break;
        case 15:
            SetPSSCallback(Cb_SwitchSelectedItem);
            break;
        case 14:
            SetPSSCallback(Cb_GiveItemFromBag);
            break;
        case 17:
            SetPSSCallback(Cb_ShowItemInfo);
            break;
        }
        break;
    case 3:
        PlaySE(SE_HAZURE);
        PrintStorageActionText(PC_TEXT_LAST_POKE);
        sPSSData->state = 6;
        break;
    case 5:
        PlaySE(SE_HAZURE);
        PrintStorageActionText(PC_TEXT_CANT_RELEASE_EGG);
        sPSSData->state = 6;
        break;
    case 4:
        PlaySE(SE_HAZURE);
        PrintStorageActionText(PC_TEXT_PLEASE_REMOVE_MAIL);
        sPSSData->state = 6;
        break;
    case 6:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            ClearBottomWindow();
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    }
}

void Cb_MoveMon(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        InitMonPlaceChange(0);
        sPSSData->state++;
        break;
    case 1:
        if (!DoMonPlaceChange())
        {
            if (sInPartyMenu)
                SetPSSCallback(Cb_HandleMovingMonFromParty);
            else
                SetPSSCallback(Cb_MainPSS);
        }
        break;
    }
}

void Cb_PlaceMon(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        sub_808FE54(1);
        InitMonPlaceChange(1);
        sPSSData->state++;
        break;
    case 1:
        if (!DoMonPlaceChange())
        {
            if (sInPartyMenu)
                SetPSSCallback(Cb_HandleMovingMonFromParty);
            else
                SetPSSCallback(Cb_MainPSS);
        }
        break;
    }
}

void Cb_ShiftMon(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        sub_808FE54(0);
        InitMonPlaceChange(2);
        sPSSData->state++;
        break;
    case 1:
        if (!DoMonPlaceChange())
        {
            BoxSetMosaic();
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    }
}

void Cb_WithdrawMon(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        if (CalculatePlayerPartyCount() == PARTY_SIZE)
        {
            PrintStorageActionText(PC_TEXT_PARTY_FULL);
            sPSSData->state = 1;
        }
        else
        {
            sub_8092B5C();
            InitMonPlaceChange(0);
            sPSSData->state = 2;
        }
        break;
    case 1:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            ClearBottomWindow();
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    case 2:
        if (!DoMonPlaceChange())
        {
            SetMovingMonPriority(1);
            SetUpDoShowPartyMenu();
            sPSSData->state++;
        }
        break;
    case 3:
        if (!DoShowPartyMenu())
        {
            sub_808FE54(1);
            InitMonPlaceChange(1);
            sPSSData->state++;
        }
        break;
    case 4:
        if (!DoMonPlaceChange())
        {
            sub_808FAA8();
            sPSSData->state++;
        }
        break;
    case 5:
        SetPSSCallback(Cb_HidePartyPokemon);
        break;
    }
}

void Cb_DepositMenu(u8 taskId)
{
    u8 boxId;

    switch (sPSSData->state)
    {
    case 0:
        PrintStorageActionText(PC_TEXT_DEPOSIT_IN_WHICH_BOX);
        sub_808C854(&sPSSData->field_1E5C, TAG_TILE_A, TAG_PAL_DAC7, 3, FALSE);
        sub_808C940(gUnknown_20397B6);
        sPSSData->state++;
        break;
    case 1:
        boxId = HandleBoxChooseSelectionInput();
        if (boxId == 200)
        {
            // no box chosen yet
        }
        else if (boxId == 201)
        {
            ClearBottomWindow();
            sub_808C950();
            sub_808C8FC();
            SetPSSCallback(Cb_MainPSS);
        }
        else
        {
            if (TryStorePartyMonInBox(boxId))
            {
                gUnknown_20397B6 = boxId;
                sub_808FE54(2);
                ClearBottomWindow();
                sub_808C950();
                sub_808C8FC();
                sPSSData->state = 2;
            }
            else
            {
                PrintStorageActionText(PC_TEXT_BOX_IS_FULL);
                sPSSData->state = 4;
            }
        }
        break;
    case 2:
        CompactPartySlots();
        sub_80909F4();
        sPSSData->state++;
        break;
    case 3:
        if (!sub_8090A60())
        {
            sub_8093174();
            BoxSetMosaic();
            sub_808FAA8();
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    case 4:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            PrintStorageActionText(PC_TEXT_DEPOSIT_IN_WHICH_BOX);
            sPSSData->state = 1;
        }
        break;
    }
}

void Cb_ReleaseMon(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        PrintStorageActionText(PC_TEXT_RELEASE_POKE);
        ShowYesNoWindow(1);
        sPSSData->state++;
        // fallthrough
    case 1:
        switch (Menu_ProcessInputNoWrapClearOnChoose())
        {
        case MENU_B_PRESSED:
        case  1:
            ClearBottomWindow();
            SetPSSCallback(Cb_MainPSS);
            break;
        case  0:
            ClearBottomWindow();
            InitCanReleaseMonVars();
            sub_8093194();
            sPSSData->state++;
            break;
        }
        break;
    case 2:
        RunCanReleaseMon();
        if (!sub_80931EC())
        {
            while (1)
            {
                s8 r0 = RunCanReleaseMon();
                if (r0 == 1)
                {
                    sPSSData->state++;
                    break;
                }
                else if (r0 == 0)
                {
                    sPSSData->state = 8; // Can't release the mon.
                    break;
                }
            }
        }
        break;
    case 3:
        ReleaseMon();
        RefreshCursorMonData();
        PrintStorageActionText(PC_TEXT_WAS_RELEASED);
        sPSSData->state++;
        break;
    case 4:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            PrintStorageActionText(PC_TEXT_BYE_BYE);
            sPSSData->state++;
        }
        break;
    case 5:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            ClearBottomWindow();
            if (sInPartyMenu)
            {
                CompactPartySlots();
                sub_80909F4();
                sPSSData->state++;
            }
            else
            {
                sPSSData->state = 7;
            }
        }
        break;
    case 6:
        if (!sub_8090A60())
        {
            sub_8092F54();
            BoxSetMosaic();
            sub_808FAA8();
            sPSSData->state++;
        }
        break;
    case 7:
        SetPSSCallback(Cb_MainPSS);
        break;
    case 8:
        PrintStorageActionText(PC_TEXT_WAS_RELEASED);
        sPSSData->state++;
        break;
    case 9:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            PrintStorageActionText(PC_TEXT_SURPRISE);
            sPSSData->state++;
        }
        break;
    case 10:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            ClearBottomWindow();
            sub_8091114();
            sPSSData->state++;
        }
        break;
    case 11:
        if (!sub_8091150())
        {
            sub_8093264();
            PrintStorageActionText(PC_TEXT_CAME_BACK);
            sPSSData->state++;
        }
        break;
    case 12:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            PrintStorageActionText(PC_TEXT_WORRIED);
            sPSSData->state++;
        }
        break;
    case 13:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            ClearBottomWindow();
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    }
}

void Cb_ShowMarkMenu(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        PrintStorageActionText(PC_TEXT_MARK_POKE);
        sPSSData->field_DA4.markings = sPSSData->cursorMonMarkings;
        DrawMonMarkingsMenu(sPSSData->cursorMonMarkings, 0xb0, 0x10);
        sPSSData->state++;
        break;
    case 1:
        if (!MonMarkingsHandleInput())
        {
            TeardownMonMarkingsMenu();
            ClearBottomWindow();
            SetMonMarkings(sPSSData->field_DA4.markings);
            RefreshCursorMonData();
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    }
}

void Cb_TakeItemForMoving(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        if (!ItemIsMail(sPSSData->cursorMonItem))
        {
            ClearBottomWindow();
            sPSSData->state++;
        }
        else
        {
            SetPSSCallback(Cb_PrintCantStoreMail);
        }
        break;
    case 1:
        sub_8094D14(2);
        Item_FromMonToMoving((sInPartyMenu != FALSE) ? CURSOR_AREA_IN_PARTY : CURSOR_AREA_IN_BOX, GetBoxCursorPosition());
        sPSSData->state++;
        break;
    case 2:
        if (!sub_809610C())
        {
            sub_8094D14(3);
            ClearBottomWindow();
            sub_8092F54();
            PrintCursorMonInfo();
            sPSSData->state++;
        }
        break;
    case 3:
        if (!IsDma3ManagerBusyWithBgCopy())
            SetPSSCallback(Cb_MainPSS);
        break;
    }
}

void Cb_GiveMovingItemToMon(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        ClearBottomWindow();
        sPSSData->state++;
        break;
    case 1:
        sub_8094D14(2);
        Item_GiveMovingToMon((sInPartyMenu != FALSE) ? CURSOR_AREA_IN_PARTY : CURSOR_AREA_IN_BOX, GetBoxCursorPosition());
        sPSSData->state++;
        break;
    case 2:
        if (!sub_809610C())
        {
            sub_8094D14(0);
            sub_8092F54();
            PrintCursorMonInfo();
            PrintStorageActionText(PC_TEXT_ITEM_IS_HELD);
            sPSSData->state++;
        }
        break;
    case 3:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            ClearBottomWindow();
            sPSSData->state++;
        }
        break;
    case 4:
        if (!IsDma3ManagerBusyWithBgCopy())
            SetPSSCallback(Cb_MainPSS);
        break;
    }
}

void Cb_ItemToBag(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        if (!AddBagItem(sPSSData->cursorMonItem, 1))
        {
            PlaySE(SE_HAZURE);
            PrintStorageActionText(PC_TEXT_BAG_FULL);
            sPSSData->state = 3;
        }
        else
        {
            PlaySE(SE_SELECT);
            Item_TakeMons((sInPartyMenu != FALSE) ? CURSOR_AREA_IN_PARTY : CURSOR_AREA_IN_BOX, GetBoxCursorPosition());
            sPSSData->state = 1;
        }
        break;
    case 1:
        if (!sub_809610C())
        {
            PrintStorageActionText(PC_TEXT_PLACED_IN_BAG);
            sPSSData->state = 2;
        }
        break;
    case 2:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            ClearBottomWindow();
            sub_8092F54();
            PrintCursorMonInfo();
            sPSSData->state = 4;
        }
        break;
    case 4:
        if (!IsDma3ManagerBusyWithBgCopy())
            SetPSSCallback(Cb_MainPSS);
        break;
    case 3:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            ClearBottomWindow();
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    }
}

void Cb_SwitchSelectedItem(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        if (!ItemIsMail(sPSSData->cursorMonItem))
        {
            ClearBottomWindow();
            sPSSData->state++;
        }
        else
        {
            SetPSSCallback(Cb_PrintCantStoreMail);
        }
        break;
    case 1:
        sub_8094D14(2);
        Item_SwitchMonsWithMoving((sInPartyMenu != FALSE) ? CURSOR_AREA_IN_PARTY : CURSOR_AREA_IN_BOX, GetBoxCursorPosition());
        sPSSData->state++;
        break;
    case 2:
        if (!sub_809610C())
        {
            sub_8094D14(3);
            sub_8092F54();
            PrintCursorMonInfo();
            PrintStorageActionText(PC_TEXT_CHANGED_TO_ITEM);
            sPSSData->state++;
        }
        break;
    case 3:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            ClearBottomWindow();
            sPSSData->state++;
        }
        break;
    case 4:
        if (!IsDma3ManagerBusyWithBgCopy())
            SetPSSCallback(Cb_MainPSS);
        break;
    }
}

void Cb_ShowItemInfo(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        ClearBottomWindow();
        sPSSData->state++;
        break;
    case 1:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            PlaySE(SE_WIN_OPEN);
            PrintItemDescription();
            sub_80966F4();
            sPSSData->state++;
        }
        break;
    case 2:
        if (!sub_8096728())
            sPSSData->state++;
        break;
    case 3:
        if (!IsDma3ManagerBusyWithBgCopy())
            sPSSData->state++;
        break;
    case 4:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            PlaySE(SE_WIN_OPEN);
            sPSSData->state++;
        }
        break;
    case 5:
        if (!sub_80967C0())
            sPSSData->state++;
        break;
    case 6:
        if (!IsDma3ManagerBusyWithBgCopy())
            SetPSSCallback(Cb_MainPSS);
        break;
    }
}

void Cb_CloseBoxWhileHoldingItem(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        PlaySE(SE_SELECT);
        PrintStorageActionText(PC_TEXT_PUT_IN_BAG);
        ShowYesNoWindow(0);
        sPSSData->state = 1;
        break;
    case 1:
        switch (Menu_ProcessInputNoWrapClearOnChoose())
        {
        case MENU_B_PRESSED:
        case 1:
            ClearBottomWindow();
            SetPSSCallback(Cb_MainPSS);
            break;
        case 0:
            if (AddBagItem(sPSSData->movingItem, 1) == TRUE)
            {
                ClearBottomWindow();
                sPSSData->state = 3;
            }
            else
            {
                PrintStorageActionText(PC_TEXT_BAG_FULL);
                sPSSData->state = 2;
            }
            break;
        }
        break;
    case 2:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            ClearBottomWindow();
            sPSSData->state = 5;
        }
        break;
    case 3:
        sub_8096088();
        sPSSData->state = 4;
        break;
    case 4:
        if (!sub_809610C())
        {
            sub_8094D14(0);
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    case 5:
        if (!IsDma3ManagerBusyWithBgCopy())
            SetPSSCallback(Cb_MainPSS);
        break;
    }
}

void Cb_HandleMovingMonFromParty(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        CompactPartySlots();
        sub_80909F4();
        sPSSData->state++;
        break;
    case 1:
        if (!sub_8090A60())
        {
            sub_808FAA8();
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    }
}

void Cb_PrintCantStoreMail(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        PrintStorageActionText(PC_TEXT_CANT_STORE_MAIL);
        sPSSData->state++;
        break;
    case 1:
        if (!IsDma3ManagerBusyWithBgCopy())
            sPSSData->state++;
        break;
    case 2:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            ClearBottomWindow();
            sPSSData->state++;
        }
        break;
    case 3:
        if (!IsDma3ManagerBusyWithBgCopy())
            SetPSSCallback(Cb_MainPSS);
        break;
    }
}

void Cb_HandleBoxOptions(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        PrintStorageActionText(PC_TEXT_WHAT_YOU_DO);
        AddMenu();
        sPSSData->state++;
        break;
    case 1:
        if (sub_8094F90())
            return;
        sPSSData->state++;
    case 2:
        switch (sub_8094F94())
        {
        case -1:
        case  0:
            sub_80920FC(TRUE);
            ClearBottomWindow();
            SetPSSCallback(Cb_MainPSS);
            break;
        case 11:
            PlaySE(SE_SELECT);
            SetPSSCallback(Cb_NameBox);
            break;
        case 10:
            PlaySE(SE_SELECT);
            ClearBottomWindow();
            SetPSSCallback(Cb_HandleWallpapers);
            break;
        case 9:
            PlaySE(SE_SELECT);
            ClearBottomWindow();
            SetPSSCallback(Cb_JumpBox);
            break;
        }
        break;
    }
}

void Cb_HandleWallpapers(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        AddWallpaperSetsMenu();
        PrintStorageActionText(PC_TEXT_PICK_A_THEME);
        sPSSData->state++;
        break;
    case 1:
        if (!sub_8094F90())
            sPSSData->state++;
        break;
    case 2:
        sPSSData->wallpaperSetId = sub_8094F94();
        switch (sPSSData->wallpaperSetId)
        {
        case -1:
            sub_80920FC(TRUE);
            ClearBottomWindow();
            SetPSSCallback(Cb_MainPSS);
            break;
        case 18 ... 21:
            PlaySE(SE_SELECT);
            sub_8095024();
            sPSSData->wallpaperSetId -= 18;
            sPSSData->state++;
            break;
        }
        break;
    case 3:
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            AddWallpapersMenu(sPSSData->wallpaperSetId);
            PrintStorageActionText(PC_TEXT_PICK_A_WALLPAPER);
            sPSSData->state++;
        }
        break;
    case 4:
        sPSSData->wallpaperId = sub_8094F94();
        switch (sPSSData->wallpaperId)
        {
        case MENU_NOTHING_CHOSEN:
            break;
        case MENU_B_PRESSED:
            ClearBottomWindow();
            sPSSData->state = 0;
            break;
        default:
            PlaySE(SE_SELECT);
            ClearBottomWindow();
            sPSSData->wallpaperId -= 22;
            SetWallpaperForCurrentBox(sPSSData->wallpaperId);
            sPSSData->state++;
            break;
        }
        break;
    case 5:
        if (!DoWallpaperGfxChange())
        {
            sub_80920FC(TRUE);
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    }
}

void Cb_JumpBox(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        PrintStorageActionText(PC_TEXT_JUMP_TO_WHICH_BOX);
        sub_808C854(&sPSSData->field_1E5C, TAG_TILE_A, TAG_PAL_DAC7, 3, FALSE);
        sub_808C940(StorageGetCurrentBox());
        sPSSData->state++;
        break;
    case 1:
        sPSSData->newCurrBoxId = HandleBoxChooseSelectionInput();
        switch (sPSSData->newCurrBoxId)
        {
        case 200:
            break;
        default:
            ClearBottomWindow();
            sub_808C950();
            sub_808C8FC();
            if (sPSSData->newCurrBoxId == 201 || sPSSData->newCurrBoxId == StorageGetCurrentBox())
            {
                sub_80920FC(TRUE);
                SetPSSCallback(Cb_MainPSS);
            }
            else
            {
                sPSSData->state++;
            }
            break;
        }
        break;
    case 2:
        SetUpScrollToBox(sPSSData->newCurrBoxId);
        sPSSData->state++;
        break;
    case 3:
        if (!ScrollToBox())
        {
            SetCurrentBox(sPSSData->newCurrBoxId);
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    }
}

void Cb_NameBox(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        sub_8093630();
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        sPSSData->state++;
        break;
    case 1:
        if (!UpdatePaletteFade())
        {
            sWhichToReshow = 1;
            sPSSData->screenChangeType = SCREEN_CHANGE_NAME_BOX;
            SetPSSCallback(Cb_ChangeScreen);
        }
        break;
    }
}

void Cb_ShowMonSummary(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        sub_80936B8();
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        sPSSData->state++;
        break;
    case 1:
        if (!UpdatePaletteFade())
        {
            sWhichToReshow = 0;
            sPSSData->screenChangeType = SCREEN_CHANGE_SUMMARY_SCREEN;
            SetPSSCallback(Cb_ChangeScreen);
        }
        break;
    }
}

void Cb_GiveItemFromBag(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        sPSSData->state++;
        break;
    case 1:
        if (!UpdatePaletteFade())
        {
            sWhichToReshow = 2;
            sPSSData->screenChangeType = SCREEN_CHANGE_ITEM_FROM_BAG;
            SetPSSCallback(Cb_ChangeScreen);
        }
        break;
    }
}

void Cb_OnCloseBoxPressed(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        if (IsMonBeingMoved())
        {
            PlaySE(SE_HAZURE);
            PrintStorageActionText(PC_TEXT_HOLDING_POKE);
            sPSSData->state = 1;
        }
        else if (IsActiveItemMoving())
        {
            SetPSSCallback(Cb_CloseBoxWhileHoldingItem);
        }
        else
        {
            PlaySE(SE_SELECT);
            PrintStorageActionText(PC_TEXT_EXIT_BOX);
            ShowYesNoWindow(0);
            sPSSData->state = 2;
        }
        break;
    case 1:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            ClearBottomWindow();
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    case 2:
        switch (Menu_ProcessInputNoWrapClearOnChoose())
        {
        case MENU_B_PRESSED:
        case 1:
            ClearBottomWindow();
            SetPSSCallback(Cb_MainPSS);
            break;
        case 0:
            PlaySE(SE_PC_OFF);
            ClearBottomWindow();
            sPSSData->state++;
            break;
        }
        break;
    case 3:
        sub_80A0A70(0x14, 0, 1);
        sPSSData->state++;
        break;
    case 4:
        if (!sub_80A0AAC())
        {
            sub_808FF70();
            gPlayerPartyCount = CalculatePlayerPartyCount();
            sPSSData->screenChangeType = SCREEN_CHANGE_EXIT_BOX;
            SetPSSCallback(Cb_ChangeScreen);
        }
        break;
    }
}

void Cb_OnBPressed(u8 taskId)
{
    switch (sPSSData->state)
    {
    case 0:
        if (IsMonBeingMoved())
        {
            PlaySE(SE_HAZURE);
            PrintStorageActionText(PC_TEXT_HOLDING_POKE);
            sPSSData->state = 1;
        }
        else if (IsActiveItemMoving())
        {
            SetPSSCallback(Cb_CloseBoxWhileHoldingItem);
        }
        else
        {
            PlaySE(SE_SELECT);
            PrintStorageActionText(PC_TEXT_CONTINUE_BOX);
            ShowYesNoWindow(0);
            sPSSData->state = 2;
        }
        break;
    case 1:
        if (JOY_NEW(A_BUTTON | B_BUTTON | DPAD_ANY))
        {
            ClearBottomWindow();
            SetPSSCallback(Cb_MainPSS);
        }
        break;
    case 2:
        switch (Menu_ProcessInputNoWrapClearOnChoose())
        {
        case 0:
            ClearBottomWindow();
            SetPSSCallback(Cb_MainPSS);
            break;
        case 1:
        case MENU_B_PRESSED:
            PlaySE(SE_PC_OFF);
            ClearBottomWindow();
            sPSSData->state++;
            break;
        }
        break;
    case 3:
        sub_80A0A70(0x14, 0, 0);
        sPSSData->state++;
        break;
    case 4:
        if (!sub_80A0AAC())
        {
            sub_808FF70();
            gPlayerPartyCount = CalculatePlayerPartyCount();
            sPSSData->screenChangeType = SCREEN_CHANGE_EXIT_BOX;
            SetPSSCallback(Cb_ChangeScreen);
        }
        break;
    }
}

void Cb_ChangeScreen(u8 taskId)
{
    struct Pokemon * partyMon;
    u8 mode, monIndex, maxMonIndex;
    u8 screenChangeType = sPSSData->screenChangeType;

    if (sPSSData->boxOption == BOX_OPTION_MOVE_ITEMS && IsActiveItemMoving() == TRUE)
        gUnknown_20397BA = GetMovingItem();
    else
        gUnknown_20397BA = ITEM_NONE;

    switch (screenChangeType)
    {
    case SCREEN_CHANGE_EXIT_BOX:
    default:
        FreePSSData();
        SetMainCallback2(Cb2_ExitPSS);
        break;
    case SCREEN_CHANGE_SUMMARY_SCREEN:
        partyMon = sPSSData->field_218C.mon;
        monIndex = sPSSData->field_2187;
        maxMonIndex = sPSSData->field_2186;
        mode = sPSSData->field_2188;
        FreePSSData();
        ShowPokemonSummaryScreen(partyMon, monIndex, maxMonIndex, Cb2_ReturnToPSS, mode);
        break;
    case SCREEN_CHANGE_NAME_BOX:
        FreePSSData();
        DoNamingScreen(NAMING_SCREEN_BOX, GetBoxNamePtr(StorageGetCurrentBox()), 0, 0, 0, Cb2_ReturnToPSS);
        break;
    case SCREEN_CHANGE_ITEM_FROM_BAG:
        FreePSSData();
        GoToBagMenu(ITEMMENULOCATION_PCBOX, OPEN_BAG_ITEMS, Cb2_ReturnToPSS);
        break;
    }

    DestroyTask(taskId);
}

void GiveChosenBagItem(void)
{
    u16 item = gSpecialVar_ItemId;

    if (item != 0)
    {
        u8 id = GetBoxCursorPosition();

        if (sInPartyMenu)
            SetMonData(&gPlayerParty[id], MON_DATA_HELD_ITEM, &item);
        else
            SetCurrentBoxMonData(id, MON_DATA_HELD_ITEM, &item);

        RemoveBagItem(item, 1);
    }
}

void FreePSSData(void)
{
    FreeBoxPartyPokemonDropdowns();
    sub_80950A4();
    FREE_AND_SET_NULL(sPSSData);
    FreeAllWindowBuffers();
}

// ******************************************************************
// Graphics util
// ******************************************************************

void SetScrollingBackground(void)
{
    SetGpuReg(REG_OFFSET_BG3CNT, BGCNT_PRIORITY(3) | BGCNT_CHARBASE(3) | BGCNT_16COLOR | BGCNT_SCREENBASE(31));
    DecompressAndLoadBgGfxUsingHeap(3, gPokemonStorageScrollingBGTileset, 0, 0, 0);
    LZ77UnCompVram(gPokemonStorageScrollingBGTilemap, (void *)BG_SCREEN_ADDR(31));
}

void ScrollBackground(void)
{
    ChangeBgX(3, 128, 1);
    ChangeBgY(3, 128, 2);
}

void LoadPSSMenuGfx(void)
{
    InitBgsFromTemplates(0, gUnknown_83CEA50, NELEMS(gUnknown_83CEA50));
    DecompressAndLoadBgGfxUsingHeap(1, gPSSMenu_Gfx, 0, 0, 0);
    LZ77UnCompWram(gUnknown_83CE5FC, sPSSData->field_5AC4);
    SetBgTilemapBuffer(1, sPSSData->field_5AC4);
    ShowBg(1);
    ScheduleBgCopyTilemapToVram(1);
}

bool8 InitPSSWindows(void)
{
    if (!InitWindows(gUnknown_83CEA30))
    {
        return FALSE;
    }
    else
    {
        DeactivateAllTextPrinters();
        return TRUE;
    }
}

void LoadWaveformSpritePalette(void)
{
    LoadSpritePalette(&gWaveformSpritePalette);
}

void sub_808F078(void)
{
    LoadPalette(gUnknown_8E9C3F8, 0, 0x20);
    LoadPalette(gUnknown_8E9C418, 0x20, 0x20);
    LoadPalette(gUnknown_83CEA10, 0xF0, 0x20);
    if (sPSSData->boxOption != BOX_OPTION_MOVE_ITEMS)
        LoadPalette(gUnknown_83CE738, 0x30, 0x20);
    else
        LoadPalette(gUnknown_83CE758, 0x30, 0x20);

    SetGpuReg(REG_OFFSET_BG1CNT, BGCNT_PRIORITY(1) | BGCNT_CHARBASE(1) | BGCNT_16COLOR | BGCNT_SCREENBASE(30));
    LoadCursorMonSprite();
    sub_808F0F4();
    sub_808F164();
    RefreshCursorMonData();
}

void sub_808F0F4(void)
{
    sPSSData->field_D94 = CreateMonMarkingSprite_AllOff(TAG_TILE_10, TAG_PAL_DAC8, NULL);
    sPSSData->field_D94->oam.priority = 1;
    sPSSData->field_D94->subpriority = 1;
    sPSSData->field_D94->pos1.x = 40;
    sPSSData->field_D94->pos1.y = 150;
    sPSSData->field_DA0 = (void *)OBJ_VRAM0 + 32 * GetSpriteTileStartByTag(TAG_TILE_10);
}

void sub_808F164(void)
{
    u16 i;
    struct SpriteSheet sheet = gWaveformSpriteSheet;

    LoadSpriteSheet(&sheet);
    for (i = 0; i < 2; i++)
    {
        u8 spriteId = CreateSprite(&sSpriteTemplate_Waveform, i * 63 + 8, 9, 2);
        sPSSData->field_D98[i] = &gSprites[spriteId];
    }
}

void RefreshCursorMonData(void)
{
    LoadCursorMonGfx(sPSSData->cursorMonSpecies, sPSSData->cursorMonPersonality);
    PrintCursorMonInfo();
    sub_808F5E8();
    ScheduleBgCopyTilemapToVram(0);
}

void BoxSetMosaic(void)
{
    RefreshCursorMonData();
    if (sPSSData->cursorMonSprite)
    {
        sPSSData->cursorMonSprite->oam.mosaic = TRUE;
        sPSSData->cursorMonSprite->data[0] = 10;
        sPSSData->cursorMonSprite->data[1] = 1;
        sPSSData->cursorMonSprite->callback = SpriteCB_CursorMon_Mosaic;
        SetGpuReg(REG_OFFSET_MOSAIC, (sPSSData->cursorMonSprite->data[0] << 12) | (sPSSData->cursorMonSprite->data[0] << 8));
    }
}

u8 BoxGetMosaic(void)
{
    return sPSSData->cursorMonSprite->oam.mosaic;
}

void SpriteCB_CursorMon_Mosaic(struct Sprite *sprite)
{
    sprite->data[0] -= sprite->data[1];
    if (sprite->data[0] < 0)
        sprite->data[0] = 0;
    SetGpuReg(REG_OFFSET_MOSAIC, (sprite->data[0] << 12) | (sprite->data[0] << 8));
    if (sprite->data[0] == 0)
    {
        sprite->oam.mosaic = FALSE;
        sprite->callback = SpriteCallbackDummy;
    }
}

void LoadCursorMonSprite(void)
{
    u16 i;
    u16 tileStart;
    u8 palSlot;
    u8 spriteId;
    struct SpriteSheet sheet = {sPSSData->field_22C4, 0x800, TAG_TILE_2};
    struct SpritePalette palette = {sPSSData->field_2244, TAG_PAL_DAC6};
    struct SpriteTemplate template = sSpriteTemplate_CursorMon;

    for (i = 0; i < 0x800; i++)
        sPSSData->field_22C4[i] = 0;
    for (i = 0; i < 0x10; i++)
        sPSSData->field_2244[i] = 0;

    sPSSData->cursorMonSprite = NULL;

    do
    {
        tileStart = LoadSpriteSheet(&sheet);
        if (tileStart == 0)
            break;

        palSlot = LoadSpritePalette(&palette);
        if (palSlot == 0xFF)
            break;

        spriteId = CreateSprite(&template, 40, 48, 0);
        if (spriteId == MAX_SPRITES)
            break;

        sPSSData->cursorMonSprite = &gSprites[spriteId];
        sPSSData->field_223A = palSlot * 16 + 0x100;
        sPSSData->field_223C = (void *)OBJ_VRAM0 + tileStart * 32;
    } while (0);

    if (sPSSData->cursorMonSprite == NULL)
    {
        FreeSpriteTilesByTag(TAG_TILE_2);
        FreeSpritePaletteByTag(TAG_PAL_DAC6);
    }
}

void LoadCursorMonGfx(u16 species, u32 pid)
{
    if (sPSSData->cursorMonSprite == NULL)
        return;

    if (species != SPECIES_NONE)
    {
        HandleLoadSpecialPokePic(&gMonFrontPicTable[species], sPSSData->field_22C4, species, pid);
        LZ77UnCompWram(sPSSData->cursorMonPalette, sPSSData->field_2244);
        CpuCopy32(sPSSData->field_22C4, sPSSData->field_223C, 0x800);
        LoadPalette(sPSSData->field_2244, sPSSData->field_223A, 0x20);
        sPSSData->cursorMonSprite->invisible = FALSE;
    }
    else
    {
        sPSSData->cursorMonSprite->invisible = TRUE;
    }
}

void PrintCursorMonInfo(void)
{
    u16 i;
    u16 y;
    FillWindowPixelBuffer(0, PIXEL_FILL(1));
    if (sPSSData->boxOption != BOX_OPTION_MOVE_ITEMS)
    {
        for (i = 0, y = 0; i < 3; i++, y += 14)
        {
            AddTextPrinterParameterized(0, 2, sPSSData->cursorMonTexts[i], i == 2 ? 10 : 6, y, TEXT_SPEED_FF, NULL);
        }
        AddTextPrinterParameterized(0, 0, sPSSData->cursorMonTexts[3], 6, y + 2, TEXT_SPEED_FF, NULL);
    }
    else
    {
        AddTextPrinterParameterized(0, 0, sPSSData->cursorMonTexts[3], 6, 0, TEXT_SPEED_FF, NULL);
        for (i = 0, y = 15; i < 3; i++, y += 14)
        {
            AddTextPrinterParameterized(0, 2, sPSSData->cursorMonTexts[i], i == 2 ? 10 : 6, y, TEXT_SPEED_FF, NULL);
        }
    }

    CopyWindowToVram(0, 2);
    if (sPSSData->cursorMonSpecies != SPECIES_NONE)
    {
        sub_80BEBD0(sPSSData->cursorMonMarkings, sPSSData->field_DA0);
        sPSSData->field_D94->invisible = FALSE;
    }
    else
    {
        sPSSData->field_D94->invisible = TRUE;
    }
}

void sub_808F5E8(void)
{
    u16 i;

    if (sPSSData->cursorMonSpecies != SPECIES_NONE)
    {
        SetBoxPartyPokemonDropdownMap2Rect(0, 0, 0, 8, 2);
        for (i = 0; i < 2; i++)
            StartSpriteAnimIfDifferent(sPSSData->field_D98[i], i * 2 + 1);
    }
    else
    {
        SetBoxPartyPokemonDropdownMap2Rect(0, 0, 2, 8, 2);
        for (i = 0; i < 2; i++)
            StartSpriteAnim(sPSSData->field_D98[i], i * 2);
    }

    CopyBoxPartyPokemonDropdownToBgTilemapBuffer(0);
    ScheduleBgCopyTilemapToVram(1);
}

void sub_808F68C(void)
{
    LZ77UnCompWram(gUnknown_8E9CAEC, sPSSData->field_B0);
    LoadPalette(gPSSMenu_Pal, 0x10, 0x20);
    SetBoxPartyPokemonDropdownMap2(1, 1, sPSSData->field_B0, 12, 22);
    SetBoxPartyPokemonDropdownMap2(2, 1, gUnknown_83CE778, 9, 4);
    SetBoxPartyPokemonDropdownMap2Pos(1, 10, 0);
    SetBoxPartyPokemonDropdownMap2Pos(2, 21, 0);
    sub_808F9FC();
    if (sInPartyMenu)
    {
        sub_808F90C(TRUE);
        CreatePartyMonsSprites(TRUE);
        CopyBoxPartyPokemonDropdownToBgTilemapBuffer(2);
        CopyBoxPartyPokemonDropdownToBgTilemapBuffer(1);
    }
    else
    {
        SetBoxPartyPokemonDropdownMap2Rect(1, 0, 20, 12, 2);
        sub_808F90C(TRUE);
        CopyBoxPartyPokemonDropdownToBgTilemapBuffer(1);
        CopyBoxPartyPokemonDropdownToBgTilemapBuffer(2);
    }

    ScheduleBgCopyTilemapToVram(1);
    sPSSData->unk_02C7 = FALSE;
}

void SetUpShowPartyMenu(void)
{
    sPSSData->field_2C0 = 20;
    sPSSData->field_2C2 = 2;
    sPSSData->field_2C5 = 0;
    CreatePartyMonsSprites(FALSE);
}

bool8 ShowPartyMenu(void)
{
    if (sPSSData->field_2C5 == 20)
        return FALSE;

    sPSSData->field_2C0--;
    sPSSData->field_2C2++;
    AdjustBoxPartyPokemonDropdownPos(1, 3, 1);
    CopyBoxPartyPokemonDropdownToBgTilemapBuffer(1);
    ScheduleBgCopyTilemapToVram(1);
    sub_8090B98(8);
    if (++sPSSData->field_2C5 == 20)
    {
        sInPartyMenu = TRUE;
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

void SetUpHidePartyMenu(void)
{
    sPSSData->field_2C0 = 0;
    sPSSData->field_2C2 = 22;
    sPSSData->field_2C5 = 0;
    if (sPSSData->boxOption == BOX_OPTION_MOVE_ITEMS)
        sub_80960C0();
}

bool8 HidePartyMenu(void)
{
    if (sPSSData->field_2C5 != 20)
    {
        sPSSData->field_2C0++;
        sPSSData->field_2C2--;
        AdjustBoxPartyPokemonDropdownPos(1, 3, -1);
        CopyBoxPartyPokemonDropdownToBgTilemapBuffer(1);
        FillBgTilemapBufferRect_Palette0(1, 0x100, 10, sPSSData->field_2C2, 12, 1);
        sub_8090B98(-8);
        if (++sPSSData->field_2C5 != 20)
        {
            ScheduleBgCopyTilemapToVram(1);
            return TRUE;
        }
        else
        {
            sInPartyMenu = FALSE;
            DestroyAllPartyMonIcons();
            CompactPartySlots();
            SetBoxPartyPokemonDropdownMap2Rect(2, 0, 0, 9, 2);
            CopyBoxPartyPokemonDropdownToBgTilemapBuffer(2);
            ScheduleBgCopyTilemapToVram(1);
            return FALSE;
        }
    }

    return FALSE;
}

void sub_808F90C(bool8 arg0)
{
    if (arg0)
        SetBoxPartyPokemonDropdownMap2Rect(2, 0, 0, 9, 2);
    else
        SetBoxPartyPokemonDropdownMap2Rect(2, 0, 2, 9, 2);

    CopyBoxPartyPokemonDropdownToBgTilemapBuffer(2);
    ScheduleBgCopyTilemapToVram(1);
}

void sub_808F948(void)
{
    sPSSData->unk_02C7 = TRUE;
    sPSSData->unk_02C8 = 30;
    sPSSData->unk_02C9 = TRUE;
}

void sub_808F974(void)
{
    if (sPSSData->unk_02C7)
    {
        sPSSData->unk_02C7 = FALSE;
        sub_808F90C(TRUE);
    }
}

void sub_808F99C(void)
{
    if (sPSSData->unk_02C7 && ++sPSSData->unk_02C8 > 30)
    {
        sPSSData->unk_02C8 = 0;
        sPSSData->unk_02C9 = (sPSSData->unk_02C9 == FALSE);
        sub_808F90C(sPSSData->unk_02C9);
    }
}

void sub_808F9FC(void)
{
    u8 i;

    for (i = 1; i < PARTY_SIZE; i++)
    {
        s32 species = GetMonData(gPlayerParty + i, MON_DATA_SPECIES);
        sub_808FA30(i, (species != SPECIES_NONE));
    }
}

void sub_808FA30(u8 pos, bool8 isPartyMon)
{
    u16 i, j, index;
    const u16 *data;

    if (isPartyMon)
        data = gUnknown_83CE7C0;
    else
        data = gUnknown_83CE7D8;

    index = 3 * (3 * (pos - 1) + 1);
    index *= 4;
    index += 7;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            sPSSData->field_B0[index + j] = data[j];
        }
        data += 4;
        index += 12;
    }
}

void sub_808FAA8(void)
{
    sub_808F9FC();
    SetBoxPartyPokemonDropdownMap2Rect(1, 0, 0, 12, 22);
    CopyBoxPartyPokemonDropdownToBgTilemapBuffer(1);
    ScheduleBgCopyTilemapToVram(1);
}

void SetUpDoShowPartyMenu(void)
{
    sPSSData->showPartyMenuState = 0;
    PlaySE(SE_WIN_OPEN);
    SetUpShowPartyMenu();
}

bool8 DoShowPartyMenu(void)
{
    switch (sPSSData->showPartyMenuState)
    {
    case 0:
        if (!ShowPartyMenu())
        {
            sub_8092AE4();
            sPSSData->showPartyMenuState++;
        }
        break;
    case 1:
        if (!sub_80924A8())
        {
            if (sPSSData->setMosaic)
                BoxSetMosaic();
            sPSSData->showPartyMenuState++;
        }
        break;
    case 2:
        return FALSE;
    }
    return TRUE;
}

void sub_808FB68(void)
{
    SetGpuReg(REG_OFFSET_BG0CNT, BGCNT_PRIORITY(0) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(29));
    TextWindow_SetStdFrame0_WithPal(1, 2, 208);
    FillBgTilemapBufferRect(0, 0, 0, 0, 32, 20, 17);
    CopyBgTilemapBufferToVram(0);
}

void PrintStorageActionText(u8 id)
{
    u8 *txtPtr;

    DynamicPlaceholderTextUtil_Reset();
    switch (gPCStorageActionTexts[id].format)
    {
    case PC_TEXT_FMT_NORMAL:
        break;
    case PC_TEXT_FMT_MON_NAME_1:
    case PC_TEXT_FMT_MON_NAME_2:
    case PC_TEXT_FMT_MON_NAME_3:
        DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, sPSSData->cursorMonNick);
        break;
    case PC_TEXT_FMT_MON_NAME_4:
    case PC_TEXT_FMT_MON_NAME_5:
    case PC_TEXT_FMT_MON_NAME_6:
        DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, sPSSData->field_21E0);
        break;
    case PC_TEXT_FMT_ITEM_NAME:
        if (IsActiveItemMoving())
            txtPtr = StringCopy(sPSSData->itemName, GetMovingItemName());
        else
            txtPtr = StringCopy(sPSSData->itemName, sPSSData->cursorMonTexts[3]);

        while (*(txtPtr - 1) == CHAR_SPACE)
            txtPtr--;

        *txtPtr = EOS;
        DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, sPSSData->itemName);
        break;
    }

    DynamicPlaceholderTextUtil_ExpandPlaceholders(sPSSData->field_2190, gPCStorageActionTexts[id].text);
    FillWindowPixelBuffer(1, PIXEL_FILL(1));
    AddTextPrinterParameterized(1, 1, sPSSData->field_2190, 0, 2, TEXT_SPEED_FF, NULL);
    DrawTextBorderOuter(1, 2, 13);
    PutWindowTilemap(1);
    CopyWindowToVram(1, 2);
    ScheduleBgCopyTilemapToVram(0);
}

void ShowYesNoWindow(s8 cursorPos)
{
    CreateYesNoMenu(&sYesNoWindowTemplate, 1, 0, 2, 0x00b, 14, 1);
    Menu_MoveCursorNoWrapAround(cursorPos);
}

void ClearBottomWindow(void)
{
    ClearStdWindowAndFrameToTransparent(1, FALSE);
    ScheduleBgCopyTilemapToVram(0);
}

void AddWallpaperSetsMenu(void)
{
    InitMenu();
    SetMenuText(PC_TEXT_SCENERY1);
    SetMenuText(PC_TEXT_SCENERY2);
    SetMenuText(PC_TEXT_SCENERY3);
    SetMenuText(PC_TEXT_ETCETERA);
    AddMenu();
}

void AddWallpapersMenu(u8 wallpaperSet)
{
    InitMenu();
    switch (wallpaperSet)
    {
    case 0:
        SetMenuText(PC_TEXT_FOREST);
        SetMenuText(PC_TEXT_CITY);
        SetMenuText(PC_TEXT_DESERT);
        SetMenuText(PC_TEXT_SAVANNA);
        break;
    case 1:
        SetMenuText(PC_TEXT_CRAG);
        SetMenuText(PC_TEXT_VOLCANO);
        SetMenuText(PC_TEXT_SNOW);
        SetMenuText(PC_TEXT_CAVE);
        break;
    case 2:
        SetMenuText(PC_TEXT_BEACH);
        SetMenuText(PC_TEXT_SEAFLOOR);
        SetMenuText(PC_TEXT_RIVER);
        SetMenuText(PC_TEXT_SKY);
        break;
    case 3:
        SetMenuText(PC_TEXT_POLKADOT);
        SetMenuText(PC_TEXT_POKECENTER);
        SetMenuText(PC_TEXT_MACHINE);
        SetMenuText(PC_TEXT_SIMPLE);
        break;
    }
    AddMenu();
}

u8 GetCurrentBoxOption(void)
{
    return sCurrentBoxOption;
}

void sub_808FDFC(void)
{
    if (!IsCursorOnBox())
    {
        if (sInPartyMenu)
            sub_8095C84(CURSOR_AREA_IN_PARTY, GetBoxCursorPosition());
        else
            sub_8095C84(CURSOR_AREA_IN_BOX, GetBoxCursorPosition());
    }

    if (gUnknown_20397BA != ITEM_NONE)
    {
        sub_8095E2C(gUnknown_20397BA);
        sub_8094D14(3);
    }
}

void sub_808FE54(u8 action)
{
    u16 event;
    u8 fromBox = sub_8094D34();
    u16 species = sPSSData->cursorMonSpecies;
    u16 species2;
    u8 toBox;
    struct PssQuestLogBuffer * qlogBuffer;
    if (sInPartyMenu)
    {
        toBox = TOTAL_BOXES_COUNT;
        species2 = GetMonData(&gPlayerParty[GetBoxCursorPosition()], MON_DATA_SPECIES2);
    }
    else
    {
        toBox = StorageGetCurrentBox();
        species2 = GetCurrentBoxMonData(GetBoxCursorPosition(), MON_DATA_SPECIES2);
    }
    qlogBuffer = &sPSSData->qlogBuffer;

    switch (action)
    {
    default:
        return;
    case 0:
        if (sInPartyMenu)
        {
            if (fromBox == TOTAL_BOXES_COUNT)
                return;
            else
                event = QL_EVENT_SWITCHED_PARTY_MON_FOR_PC_MON;
        }
        else
        {
            if (fromBox == TOTAL_BOXES_COUNT)
                // Should upmerge but doesn't
                event = QL_EVENT_SWITCHED_PARTY_MON_FOR_PC_MON;
            else
                event = fromBox != toBox ? QL_EVENT_SWITCHED_MONS_BETWEEN_BOXES : QL_EVENT_SWITCHED_MONS_WITHIN_BOX;
        }
        qlogBuffer->species = species;
        qlogBuffer->species2 = species2;
        qlogBuffer->fromBox = fromBox;
        qlogBuffer->toBox = toBox;
        break;
    case 1:
        qlogBuffer->species = species;
        qlogBuffer->species2 = SPECIES_NONE;
        qlogBuffer->fromBox = fromBox;
        qlogBuffer->toBox = 0xFF;
        if (sInPartyMenu)
        {
            if (fromBox == TOTAL_BOXES_COUNT)
                return;
            else
                event = QL_EVENT_WITHDREW_MON_PC;
        }
        else
        {
            if (fromBox == TOTAL_BOXES_COUNT)
            {
                event = QL_EVENT_DEPOSITED_MON_PC;
                qlogBuffer->fromBox = toBox;
            }
            else if (fromBox != toBox)
            {
                event = QL_EVENT_MOVED_MON_BETWEEN_BOXES;
                qlogBuffer->toBox = toBox;
            }
            else
                event = QL_EVENT_MOVED_MON_WITHIN_BOX;
        }
        break;
    case 2:
        event = QL_EVENT_DEPOSITED_MON_PC;
        qlogBuffer->species = species;
        qlogBuffer->species2 = SPECIES_NONE;
        qlogBuffer->fromBox = gUnknown_20397B6;
        qlogBuffer->toBox = 0xFF;
        break;
    case 3:
        event = QL_EVENT_SWITCHED_MULTIPLE_MONS;
        qlogBuffer->species = SPECIES_NONE;
        qlogBuffer->species2 = SPECIES_NONE;
        qlogBuffer->fromBox = fromBox;
        qlogBuffer->toBox = toBox;
        break;
    }
    SetQuestLogEvent(event, (const void *)qlogBuffer);
}

void sub_808FF70(void)
{
    if (sLastUsedBox != StorageGetCurrentBox())
    {
        FlagClear(FLAG_SHOWN_BOX_WAS_FULL_MESSAGE);
        VarSet(VAR_PC_BOX_TO_SEND_MON, StorageGetCurrentBox());
    }
}
