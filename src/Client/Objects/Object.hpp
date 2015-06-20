/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__
#include "wrapper.h"
#include <main.h>

class DreiDObject;
class PlanarObject;
class Player;
class LoadingObject;
class Sprite;
class TextObject;
class Textbox;
class Tilemap;
class WorldObject;

struct RGBA
{
    float r;
    float g;
    float b;
    float a;
};

class Object
{
    friend class DreiDObject;
    friend class PlanarObject;
    friend class Player;
    friend class LoadingObject;
    friend class Sprite;
    friend class TextObject;
    friend class Textbox;
    friend class Tilemap;
    friend class WorldObject;
    public:
        Object(uint16 id, float x, float y, float z, float sx, float sy, 
               uint16 tex, bool depth, bool ortho, bool clickable);
        ~Object();

        virtual void Draw();
        virtual void PostDraw();
        virtual void KeyPressed(uint8 key);
        virtual float GetPositionX() { return _x; }
        virtual float GetPositionY() { return _y; }
        void SetPositionX(float X) {  _x = X; }
        void SetPositionY(float Y) { _y = Y; }

        //Accessor
        DreiDObject*  To3D()         { return reinterpret_cast<DreiDObject*>(this);}
        PlanarObject* ToPlanar()     { return reinterpret_cast<PlanarObject*>(this);}
        Sprite*       ToSprite()     { return reinterpret_cast<Sprite*>(this);}
        TextObject*   ToText()       { return reinterpret_cast<TextObject*>(this);}
        Textbox*      ToTextbox()    { return reinterpret_cast<Textbox*>(this);}
        Tilemap*      ToTilemap()    { return reinterpret_cast<Tilemap*>(this);}
        WorldObject*  ToWorldObject(){ return reinterpret_cast<WorldObject*>(this);}

        void MoveXY(float x, float y)
        {
            _x =x;
            _y=y;
            
        }
        void MoveXYZ(float x, float y, float z) { _x =x; _y=y; _z =z; }
        void SetRotationXYZ(float x, float y, float z) { _rotX =x; _rotY=y; _rotZ=z; }
        void SetObjectRGBA(float r, float g, float b, float a);
        virtual bool IsMouseHit(float x, float y);
        
        uint16 GetId() { return _id; }
        
        void SetRotX(float x) { _rotX = x; }
        void SetRotY(float y) { _rotY = y; }
        void SetRotZ(float z) { _rotZ = z; }

        void SetSizeX(float x) { _sx = x; }
        void SetSizeY(float y) { _sy = y; }

        void pushScreenCoordinateMatrix();
        void pop_projection_matrix();

    private:
        GLfloat _matrix[16];

        uint16 _id;
        RGBA  *_color;
        ///- X Coords vom Objekt
        float _x;
        ///- Y Coords vom Objekt
        float _y;
        ///- Z Coords vom Objekt
        float _z;
        ///- Endcoords
        float _sx, _sy;
        ///- Texture ID
        int   _texture;
        ///- Z-Buffer
        bool  _depth;
        ///- Object mit Ortho-Graphic
        bool _ortho;
        ///- Gibt an, ob das Object klickbar ist
        bool _click;
        ///- Rotation
        float _rotX, _rotY, _rotZ;
};
#endif
