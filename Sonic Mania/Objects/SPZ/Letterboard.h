#ifndef OBJ_LETTERBOARD_H
#define OBJ_LETTERBOARD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxLetterTurn;
    uint16 sfxWin;
} ObjectLetterboard;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    bool32 controller;
    uint8 letterID;
    int32 spinSpeed;
    int32 timer;
    Animator animatorFront;
    Animator animatorBack;
} EntityLetterboard;

// Object Struct
extern ObjectLetterboard *Letterboard;

// Standard Entity Events
void Letterboard_Update(void);
void Letterboard_LateUpdate(void);
void Letterboard_StaticUpdate(void);
void Letterboard_Draw(void);
void Letterboard_Create(void* data);
void Letterboard_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Letterboard_EditorDraw(void);
void Letterboard_EditorLoad(void);
#endif
void Letterboard_Serialize(void);

// Extra Entity Functions
void Letterboard_State_Controller(void);
void Letterboard_State_CheckPlayerSpin(void);
void Letterboard_State_Spun(void);

#endif //!OBJ_LETTERBOARD_H
