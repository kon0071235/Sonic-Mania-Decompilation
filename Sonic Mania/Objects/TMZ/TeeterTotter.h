#ifndef OBJ_TEETERTOTTER_H
#define OBJ_TEETERTOTTER_H

#include "SonicMania.h"

// Object Class
struct ObjectTeeterTotter {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityTeeterTotter {
    RSDK_ENTITY
    StateMachine(state);
    uint8 color;
    uint32 length;
    Vector2 origin;
    int32 unused1; // never even set
    int32 playerIDs[4];
    int32 inactiveSegments;
    int32 fallPos;
    int32 fallVelocity;
    int32 unused2; // set, but never used
    int32 segmentPosition[32];
    int32 segmentVelocity[32];
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectTeeterTotter *TeeterTotter;

// Standard Entity Events
void TeeterTotter_Update(void);
void TeeterTotter_LateUpdate(void);
void TeeterTotter_StaticUpdate(void);
void TeeterTotter_Draw(void);
void TeeterTotter_Create(void* data);
void TeeterTotter_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void TeeterTotter_EditorDraw(void);
void TeeterTotter_EditorLoad(void);
#endif
void TeeterTotter_Serialize(void);

// Extra Entity Functions
int32 TeeterTotter_CheckPlayerCollisions(void);
void TeeterTotter_ProcessSegmentGravity(void);
void TeeterTotter_HandleSegmentPositions(void);

void TeeterTotter_State_Setup(void);
void TeeterTotter_State_Teeter(void);
void TeeterTotter_State_Fall(void);

#endif //!OBJ_TEETERTOTTER_H
