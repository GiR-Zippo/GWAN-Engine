#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__

#include "Object.hpp"

class Sprite: public Object
{
    public:
        Sprite(uint32 id, float x, float y, float z, std::string tileimage, uint32 tileX, uint32 tileY);
        ~Sprite();

        void Draw();
        void PostDraw();

    public:
        void SetAminamtionFrame(uint32 index);
        void SetIdleAnimation (bool left, bool right, bool up, bool down);
        void MoveUp();
        void MoveDown();
        void MoveLeft();
        void MoveRight();

    private:
        ///-Texturzeugs
        void _LoadTileTexture (std::string filename, uint32 x, uint32 y);       /* Dateiname des Bildes, Tilegröße X, Tilegröße Y */
        std::string                     _tileimage;                             /* Name des Tileimages */
        uint32                          _tileX;                                 /* Tilegröße X */
        uint32                          _tileY;                                 /* Tilegröße Y */
        bool                            _initalized;                            /* ist alles initialisiert? */
        uint8                           _animCounter;                           /* aktuelles Animationsframe */
        std::map<uint32, uint32>        _textureMap;                            /* Übersetzung Animationsframe zu Textur */
};
#endif
