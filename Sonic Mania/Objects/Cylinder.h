#ifndef OBJ_CYLINDER_H
#define OBJ_CYLINDER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[24]; //= { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13 };
    AnimationData value2;
    ushort value3;
} ObjectCylinder;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCylinder;

// Object Struct
extern ObjectCylinder *Cylinder;

// Standard Entity Events
void Cylinder_Update();
void Cylinder_LateUpdate();
void Cylinder_StaticUpdate();
void Cylinder_Draw();
void Cylinder_Create(void* data);
void Cylinder_StageLoad();
void Cylinder_EditorDraw();
void Cylinder_EditorLoad();
void Cylinder_Serialize();

// Extra Entity Functions


#endif //!OBJ_CYLINDER_H
