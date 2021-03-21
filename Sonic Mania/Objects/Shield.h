#ifndef OBJ_SHIELD_H
#define OBJ_SHIELD_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    ushort sfx_BlueShield;
    ushort sfx_BubbleShield;
    ushort sfx_FireShield;
    ushort sfx_LightningShield;
    ushort sfx_InstaShield;
    ushort sfx_BubbleBounce;
    ushort sfx_FireDash;
    ushort sfx_LightningJump;
} ObjectShield;

// Entity Class
typedef struct {
    RSDK_ENTITY
    EntityPlayer *player;
    void (*state)();
    int type;
    int timer;
    int frameID;
    int field_6C;
    AnimationData data;
    AnimationData altData;
} EntityShield;

// Object Struct
extern ObjectShield *Shield;

// Standard Entity Events
void Shield_Update();
void Shield_LateUpdate();
void Shield_StaticUpdate();
void Shield_Draw();
void Shield_Create(void* data);
void Shield_StageLoad();
void Shield_EditorDraw();
void Shield_EditorLoad();
void Shield_Serialize();

// Extra Entity Functions
void Shield_State_Update();
void Shield_State_Bubble();
void Shield_State_BubbleAlt();
void Shield_State_Fire();
void Shield_State_Lightning();
void Shield_State_Insta();

#endif //!OBJ_SHIELD_H
