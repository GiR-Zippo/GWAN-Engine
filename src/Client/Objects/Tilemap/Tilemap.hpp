/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __TILEMAP_HPP__
#define __TILEMAP_HPP__

#include "Object.hpp"

enum MapProps
{
    MAP_PROP_NONE      = 0,
    MAP_PROP_COLLISION = 1,
    MAP_PROP_TELEPORT  = 2
};

struct TileSet
{
    std::string name;
    uint64 firstgid;
    uint16 tilewidth;
    uint16 tileheight;
    uint32 spacing;
    uint16 margin;
    std::string image;
    uint32 imagewidth;
    uint32 imageheight;
    float alpha;
    std::map<uint32, GLuint>         tileTextures;
};

struct TileLayer
{
    std::string name;
    uint64 firstgid;
    uint16 tilewidth;
    uint16 tileheight;
    uint32 spacing;
    uint16 margin;
    std::string tilename;
    uint32 imagewidth;
    uint32 imageheight;
    float alpha;
    std::map<int,std::map<int,int> > map;
};

class TiXmlElement;
class Tilemap : public Object
{
    public:
        Tilemap(uint16 id, float x, float y, float z, bool depth, std::string mapname);
        void Draw();
        bool IsPropCollosion(float x, float dx, float y, float dy);
    private:
        bool _LoadMap(std::string filename);
        void _ReadTileSet(TiXmlElement* elem);
        void _ReadTileLayer(TiXmlElement* elem);
        void _AssembleMap();

        GLuint                           _CallListNum;
        bool                             _MapAssembled;        //Check if we have our Map assembled
        std::map<int, TileSet*>          _tileSets;
        std::map<int, TileLayer*>        _tileLayers;
        std::map<int,std::map<int,int> > _propmap;             //The Prop-Map
        uint32                           _mapSize[2];
};
#endif
