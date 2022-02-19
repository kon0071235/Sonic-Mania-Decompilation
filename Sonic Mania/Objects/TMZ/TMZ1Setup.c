// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TMZ1Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectTMZ1Setup *TMZ1Setup;

void TMZ1Setup_Update(void)
{
    RSDK_THIS(TMZ1Setup);
    StateMachine_Run(self->state);
}

void TMZ1Setup_LateUpdate(void) {}

void TMZ1Setup_StaticUpdate(void)
{
    if (!(Zone->timer & 1))
        ++TMZ1Setup->bgPtr->deformationOffset;

    if (!(Zone->timer & 3)) {
        TMZ1Setup->aniTileFrame += 32;
        TMZ1Setup->aniTileFrame = TMZ1Setup->aniTileFrame & 0xFF;
        RSDK.DrawAniTiles(TMZ1Setup->aniTiles, 34, TMZ1Setup->aniTileFrame, 0, 32, 112);
    }

    if (!(Zone->timer & 1))
        RSDK.RotatePalette(0, 204, 207, false);

    if (!RSDK.GetEntityCount(TMZ1Setup->objectID, true)) {
        RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(2 * Zone->timer) >> 1) + 128, 192, 197);
        RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(2 * Zone->timer + 128) >> 1) + 128, 198, 203);
        RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(4 * Zone->timer + 128) >> 1) + 64, 220, 223);
        RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(Zone->timer) >> 1) + 128, 211, 213);
        RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(Zone->timer + 128) >> 1) + 128, 237, 239);
    }

    if (TMZ1Setup->aniTileDelay <= 0) {
        TMZ1Setup->angle += 32;
        TMZ1Setup->angle = TMZ1Setup->angle;
        RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(TMZ1Setup->angle) >> 1) + 128, 184, 190);
        if (TMZ1Setup->angle == 160) {
            TMZ1Setup->aniTileDelay = RSDK.Rand(2, 60);
        }
    }
    else {
        TMZ1Setup->aniTileDelay--;
    }

    if (TMZ1Setup->stageState < TMZ1_STAGESTATE_LIFT) {
        int32 posY = ScreenInfo->centerY + ScreenInfo->position.y;
        if (posY > 4528 && TMZ1Setup->stageState != TMZ1_STAGESTATE_CITY && !RSDK.GetEntityCount(TMZ1Setup->objectID, true)) {
            CREATE_ENTITY(TMZ1Setup, intToVoid(1), 0, 0);
        }
        else if (posY < 4192 && TMZ1Setup->stageState != TMZ1_STAGESTATE_SKY && !RSDK.GetEntityCount(TMZ1Setup->objectID, true)) {
            CREATE_ENTITY(TMZ1Setup, intToVoid(0), 0, 0);
        }
    }
    EntityActClear *actClear = RSDK_GET_ENTITY(SLOT_ACTCLEAR, ActClear);
    if (!TMZ1Setup->hasAchievement && actClear->objectID == ActClear->objectID && actClear->state == ActClear_State_EnterText && !Player->gotHit[0]) {
        API_UnlockAchievement("ACH_TMZ");
        TMZ1Setup->hasAchievement = true;
    }
}

void TMZ1Setup_Draw(void) {}

void TMZ1Setup_Create(void *data)
{
    RSDK_THIS(TMZ1Setup);
    self->active = ACTIVE_NORMAL;

    switch (TMZ1Setup->stageState) {
        default: self->state = TMZ1Setup_State_FadeOutCurrentBG; break;
        case TMZ1_STAGESTATE_NONE: self->state = TMZ1Setup_State_ShowOutsideBGs; break;
        case TMZ1_STAGESTATE_LIFT: self->state = TMZ1Setup_State_FadeToLift; break;
    }
}

void TMZ1Setup_StageLoad(void)
{
    TMZ1Setup->aniTiles     = RSDK.LoadSpriteSheet("TMZ1/AniTiles.gif", SCOPE_STAGE);
    TMZ1Setup->aniTileDelay = RSDK.Rand(2, 60);
    TMZ1Setup->angle        = 0xC0;
    TMZ1Setup->stageState   = TMZ1_STAGESTATE_NONE;
    TMZ1Setup->bgPtr        = RSDK.GetSceneLayer(0);

    int32 ang = 0;
    for (int32 i = 0; i < 0x200; ++i) {
        TMZ1Setup->bgPtr->deformationData[i] = (4 * RSDK.Sin1024(ang)) >> 10;
        ang += 16;
    }
    memcpy(TMZ1Setup->bgPtr->deformationData + 0x200, TMZ1Setup->bgPtr->deformationData, 0x200 * sizeof(int32));

    RSDK.GetSceneLayer(2)->scrollPos = -0x1000000;
    RSDK.GetSceneLayer(3)->scrollPos = -0x1000000;

    if (!TMZ1Setup->paletteInit) {
#if RETRO_USE_PLUS
        if (SceneInfo->filter & FILTER_ENCORE) {
            RSDK.LoadPalette(0, "EncoreTMZ1.act", 0b0000000011111111);
            RSDK.LoadPalette(1, "EncoreTMZ1d.act", 0b0000000011111111);
            RSDK.LoadPalette(2, "EncoreTMZ1l.act", 0b0000000011111111);
        }
#endif
        for (int32 i = 0; i < 256; ++i) RSDK.SetPaletteEntry(5, i, 0x000000);
        RSDK.CopyPalette(0, 0, 4, 0, 255);
        TMZ1Setup->paletteInit = true;
    }
    RSDK.SetLimitedFade(3, 3, 4, 128, 0, 256);
    RSDK.SetLimitedFade(6, 0, 5, 192, 0, 256);

    Animals->animalTypes[0]     = ANIMAL_TOCKY;
    Animals->animalTypes[1]     = ANIMAL_PICKY;

    // BGSwitch is used for Sky <-> Lift BGs, this object manages the city & sky BG changes itself
    BGSwitch->switchCallback[0] = TMZ1Setup_BGCallback_ShowSky;
    BGSwitch->switchCallback[1] = TMZ1Setup_BGCallback_ShowLift;

    RSDK.SetDrawLayerProperties(Zone->hudDrawOrder - 1, false, 0);
    RSDK.SetDrawLayerProperties(Zone->hudDrawOrder, false, 0);

    if (isMainGameMode() && PlayerHelpers_CheckAct1())
        Zone->stageFinishCallback = TMZ1Setup_StageFinishCB;

    if (PlayerHelpers_CheckStageReload())
        TMZ1Setup->hasAchievement = true;
}

void TMZ1Setup_BGCallback_ShowSky(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(4)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    foreach_active(MonarchBG, monarch) { monarch->visible = true; }
    TMZ1Setup->stageState = TMZ1_STAGESTATE_SKY;
}

void TMZ1Setup_BGCallback_ShowLift(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(4)->drawLayer[BGSwitch->screenID] = 0;
    foreach_active(MonarchBG, monarch) { monarch->visible = false; }
    TMZ1Setup->stageState = TMZ1_STAGESTATE_LIFT;
}

void TMZ1Setup_StageFinishCB(void)
{
    foreach_active(CrimsonEye, crimsonEye)
    {
        if (crimsonEye->state == CrimsonEye_StateContainer_HandleElevator) {
            crimsonEye->state = CrimsonEye_StateContainer_Explode;
            CREATE_ENTITY(TMZ1Outro, NULL, crimsonEye->position.x, crimsonEye->position.y);
        }
    }
}

void TMZ1Setup_State_FadeToLift(void)
{
    RSDK_THIS(TMZ1Setup);
    if (self->timer < 256) {
        self->timer += 8;
        RSDK.SetLimitedFade(0, 4, 3, self->timer, 128, 207);
    }
    else {
        destroyEntity(self);
    }
}

void TMZ1Setup_State_ShowOutsideBGs(void)
{
    RSDK_THIS(TMZ1Setup);
    if (++self->timer >= 2) {
        if (ScreenInfo->position.y + ScreenInfo->centerY > 4192)
            self->state = TMZ1Setup_State_ShowCityBG;
        else
            self->state = TMZ1Setup_State_ShowSkyBG;
    }
}

void TMZ1Setup_State_FadeOutCurrentBG(void)
{
    RSDK_THIS(TMZ1Setup);
    if (self->timer >= 256) {
        if (TMZ1Setup->stageState) {
            TMZ1Setup->stageState               = TMZ1_STAGESTATE_SKY;
            RSDK.GetSceneLayer(0)->drawLayer[0] = 0;
            RSDK.GetSceneLayer(1)->drawLayer[0] = 0;
            RSDK.GetSceneLayer(2)->drawLayer[0] = DRAWLAYER_COUNT;
            RSDK.GetSceneLayer(3)->drawLayer[0] = DRAWLAYER_COUNT;
        }
        else {
            TMZ1Setup->stageState               = TMZ1_STAGESTATE_CITY;
            RSDK.GetSceneLayer(0)->drawLayer[0] = DRAWLAYER_COUNT;
            RSDK.GetSceneLayer(1)->drawLayer[0] = DRAWLAYER_COUNT;
            RSDK.GetSceneLayer(2)->drawLayer[0] = 0;
            RSDK.GetSceneLayer(3)->drawLayer[0] = 0;
        }
        self->state = TMZ1Setup_State_FadeIntoOutsideBGs;
    }
    else {
        self->timer += 8;
        if (TMZ1Setup->stageState)
            RSDK.SetLimitedFade(0, 3, 4, self->timer >> 1, 128, 207);
        else
            RSDK.SetLimitedFade(0, 4, 3, self->timer >> 1, 128, 207);
        RSDK.SetLimitedFade(0, 4, 5, self->timer, 208, 256);
    }
}

void TMZ1Setup_State_ShowCityBG(void)
{
    RSDK_THIS(TMZ1Setup);
    TMZ1Setup->stageState               = TMZ1_STAGESTATE_CITY;
    RSDK.GetSceneLayer(0)->drawLayer[0] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(1)->drawLayer[0] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(2)->drawLayer[0] = 0;
    RSDK.GetSceneLayer(3)->drawLayer[0] = 0;
    RSDK.CopyPalette(3, 128, 0, 128, 80);
    destroyEntity(self);
}

void TMZ1Setup_State_ShowSkyBG(void)
{
    RSDK_THIS(TMZ1Setup);
    TMZ1Setup->stageState               = TMZ1_STAGESTATE_SKY;
    RSDK.GetSceneLayer(0)->drawLayer[0] = 0;
    RSDK.GetSceneLayer(1)->drawLayer[0] = 0;
    RSDK.GetSceneLayer(2)->drawLayer[0] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(3)->drawLayer[0] = DRAWLAYER_COUNT;
    RSDK.CopyPalette(4, 128, 0, 128, 80);
    destroyEntity(self);
}

void TMZ1Setup_State_FadeIntoOutsideBGs(void)
{
    RSDK_THIS(TMZ1Setup);
    if (self->timer <= 0) {
        if (TMZ1Setup->stageState)
            RSDK.CopyPalette(4, 208, 0, 208, 48);
        else
            RSDK.CopyPalette(4, 128, 0, 128, 128);

        destroyEntity(self);
    }
    else {
        self->timer -= 8;
        if (TMZ1Setup->stageState)
            RSDK.SetLimitedFade(0, 3, 4, self->timer >> 1, 128, 207);
        else
            RSDK.SetLimitedFade(0, 4, 3, self->timer >> 1, 128, 207);

        RSDK.SetLimitedFade(0, 4, 5, self->timer, 208, 256);
    }
}

#if RETRO_INCLUDE_EDITOR
void TMZ1Setup_EditorDraw(void) {}

void TMZ1Setup_EditorLoad(void) {}
#endif

void TMZ1Setup_Serialize(void) {}
