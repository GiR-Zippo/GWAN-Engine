/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "Tilemap.hpp"
#include "splitstring.h"
#include "TextureLoader.hpp"
#include "tinyxml.h"

#include <math.h>
//uint16 id, float x, float y, float z, bool depth, uint16 tex, float zoom, float wide
Tilemap::Tilemap(uint16 id, float x, float y, float z, bool depth, std::string mapname):
    Object(id, x, y, z, 0, 0, 0, depth, false, false)
{
    _MapAssembled = false;
    _LoadMap(mapname);
    _AssembleMap();
}

void Tilemap::Draw()
{
    if (!_MapAssembled)
    {
        _AssembleMap();
        return;
    }

    // Enable depth testing if flag is set
    if (_depth)
    {
        glEnable(GL_DEPTH_TEST);
        glAlphaFunc (GL_GREATER, (float)0.1f);
        glEnable (GL_ALPHA_TEST);
    }
    else
        glDisable(GL_DEPTH_TEST);

    float zoom = _sx / 2;
    float width = _sy;
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
    glTranslatef(_x, _y, _z);
    glRotatef(_rotX, 1, 0, 0);
    glRotatef(_rotY, 0, 1, 0);
    glRotatef(_rotZ, 0, 0, 1);

    glCallList(_CallListNum);
    glPopMatrix();
    glDisable(GL_BLEND);

    if (_depth)
    {
        glDisable (GL_ALPHA_TEST) ;
        glDisable(GL_DEPTH_TEST);
    }
}

bool Tilemap::_LoadMap(string filename)
{
    int tile = 0;
    uint32 tilemapId = 0;
    TiXmlDocument doc(filename.c_str());
    bool loadOkay = doc.LoadFile();

    if (loadOkay)
    {
        printf("\n%s:\n", filename.c_str());
        TiXmlElement* root = doc.FirstChildElement();

        if (root == NULL)
        {
            doc.Clear();
            return false;
        }

        ///- MapSize lesen
        _mapSize[0] = atoi(root->Attribute("width"));
        _mapSize[1] = atoi(root->Attribute("height"));

        for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
        {
            string elemName = elem->Value();

            ///- TileSet laden
            if (elemName == "tileset")
                _ReadTileSet(elem);

            ///- Und nu die TileMaps
            else if (elemName == "layer")
                _ReadTileLayer(elem);
        }
    }
    else
    {
        printf("Failed to load file \"%s\"\n", filename.c_str());
    }

    return true;
}

void Tilemap::_ReadTileSet(TiXmlElement* elem)
{
    TileSet* t = new TileSet;
    t->firstgid = atoi(elem->Attribute("firstgid"));        /* Read first GID */
    t->name = elem->Attribute("name");                      /* Read TileSetName */
    t->tilewidth = atoi(elem->Attribute("tilewidth"));
    t->tileheight = atoi(elem->Attribute("tileheight"));

    ///- Imagedaten laden
    for (TiXmlElement* belem = elem->FirstChildElement(); belem != NULL; belem = belem->NextSiblingElement())
    {
        std::string tileData = belem->Value();

        if (tileData == "image")
        {
            std::string imagesource = belem->Attribute("source");
            imagesource.erase(std::remove(imagesource.begin(), imagesource.end(), '\"'), imagesource.end());
            Splitter split ( imagesource, "/");
            t->image = split[split.size() - 1];
            t->imageheight = atoi(belem->Attribute("height"));
            t->imagewidth = atoi(belem->Attribute("width"));
        }
    }

    ///- Texture laden, Set generieren und umwandeln
    if (t->name != "PropTex")
    {
        TextureLoader* tload = new TextureLoader();
        std::map<uint32, GLuint> temp = tload->LoadTiledTexture(GetFilePath(t->image), t->tilewidth, t->tileheight);
        delete tload;

        uint32 i = 0;

        for (std::map<uint32, GLuint>::const_iterator itr = temp.begin(); itr != temp.end(); itr++)
        {
            t->tileTextures[t->firstgid + i] = (*itr).second;
            i++;
        }
    }

    int lastID = 0;
    for (std::map<int, TileSet*>::const_iterator tile = _tileSets.begin(); tile != _tileSets.end(); tile++)
        lastID++;
    _tileSets[lastID] = t;    
}

void Tilemap::_ReadTileLayer(TiXmlElement* elem)
{
    std::string elemValue = elem->Attribute("name");

    ///- PropMap wird nicht angezeigt und ist extra
    if (elemValue == "PropMap")
    {
        int firstgid;
        int mapid ;

        for (std::map<int, TileSet*>::const_iterator itr = _tileSets.begin(); itr != _tileSets.end(); itr++)
        {
            if ((*itr).second->name == "PropTex")
            {
                firstgid = ((*itr).second->firstgid) - 1;
                mapid = (*itr).first;
                break;
            }
        }

        for (TiXmlElement* belem = elem->FirstChildElement(); belem != NULL; belem = belem->NextSiblingElement())
        {
            std::string belemValue = belem->Value();

            if (belemValue == "data")
            {
                belemValue = belem->GetText();
                belemValue.erase(std::remove(belemValue.begin(), belemValue.end(), '\n'), belemValue.end());
                belemValue.erase(std::remove(belemValue.begin(), belemValue.end(), '\r'), belemValue.end());
                Splitter split ( belemValue, "," );
                uint64 i = 0;

                for (uint32 y = 0; y < _mapSize[1]; y++)
                    for (uint32 x = 0; x < _mapSize[0];  x++ )
                    {
                        if ((atoi(split[i].c_str()) - firstgid) <= 0)
                            _propmap[y][x] = 0;
                        else
                            _propmap[y][x] = atoi(split[i].c_str()) - firstgid;

                        i++;
                    }
            }
        }
    }///-ende
    else
    {
        TileLayer* t = new TileLayer;

        if (elem->Attribute("opacity"))
            t->alpha = atof(elem->Attribute("opacity"));
        else
            t->alpha = 0.0f;

        t->name = elem->Attribute("name");

        for (TiXmlElement* belem = elem->FirstChildElement(); belem != NULL; belem = belem->NextSiblingElement())
        {
            std::string belemValue = belem->Value();

            if (belemValue == "properties")
            {
                for (TiXmlElement* celem = belem->FirstChildElement(); celem != NULL; celem = celem->NextSiblingElement())
                {
                    std::string celemValue = celem->Value();

                    if (celemValue == "property")
                    {
                        std::string attrib = celem->Attribute("name");

                        if (attrib.find("TileSet") != std::string::npos)
                            t->tilename = celem->Attribute("value");
                    }
                }
            }
            else if (belemValue == "data")
            {
                belemValue = belem->GetText();
                belemValue.erase(std::remove(belemValue.begin(), belemValue.end(), '\n'), belemValue.end());
                belemValue.erase(std::remove(belemValue.begin(), belemValue.end(), '\r'), belemValue.end());
                Splitter split ( belemValue, "," );
                uint64 i = 0;

                for (uint32 y = 0; y < _mapSize[1]; y++)
                    for (uint32 x = 0; x < _mapSize[0];  x++ )
                    {
                        t->map[y][x] = atoi(split[i].c_str());
                        i++;
                    }
            }
        }

        int lastID = 0;
        for (std::map<int, TileLayer*>::const_iterator layer = _tileLayers.begin(); layer != _tileLayers.end(); layer++)
            lastID++;
        _tileLayers[lastID] = t;
    }
}

void Tilemap::_AssembleMap()
{
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 1.0, 0.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0f);
    int num = glGenLists(1);    //generate a unique identifier for the list
    glNewList(num, GL_COMPILE);

    for (std::map<int, TileLayer*>::reverse_iterator layer = _tileLayers.rbegin(); layer != _tileLayers.rend(); ++layer)
    {
        for (std::map<int, TileSet*>::const_iterator tile = _tileSets.begin(); tile != _tileSets.end(); tile++)
        {
            if ((*tile).second->name == "PropTex")
                continue;
            else if ((*layer).second->tilename  != (*tile).second->name)
                continue;

            for (int i = 0; i < (int)_mapSize[1]; i++) //loop through the height of the map
            {
                for (int j = 0; j < (int)_mapSize[0]; j++) //loop through the width of the map
                {
                    if ((*tile).second->tileTextures[(*layer).second->map[i][j]] == 0)
                        continue;

                    glEnable(GL_TEXTURE_2D);
                    {
                        glEnable(GL_BLEND);
                        {
                            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                            glBindTexture(GL_TEXTURE_2D, (*tile).second->tileTextures[(*layer).second->map[i][j]]); //get the texture for the actual quad
                            glPushMatrix();
                            {
                                glTranslatef((float)j, (float) - i, 0.0f); //translate the tile to where it should belong
                                glBegin(GL_QUADS);
                                {
                                    glTexCoord2d(0.0, 0.0);
                                    glVertex3f(0.0, 0.0, 0.0);

                                    glTexCoord2d(1.0, 0.0);
                                    glVertex3f(1.0, 0.0, 0.0);

                                    glTexCoord2d(1.0, 1.0);
                                    glVertex3f(1.0, 1.0, 0.0);

                                    glTexCoord2d(0.0, 1.0);
                                    glVertex3f(0.0, 1.0, 0.0);
                                }
                                glEnd();
                            }
                            glPopMatrix(); //pop the matrix
                        }
                        glDisable(GL_BLEND);
                    }
                    glDisable(GL_TEXTURE_2D);
                }
            }
        }
    }

    glEndList();
    _CallListNum = num;
    _MapAssembled = true;
}

bool Tilemap::IsPropCollosion(float x, float dx, float y, float dy)
{
    float px = x - _x;
    float py = -(y - _y);

    // std::cout << "px : " << px << ", py : " << py << "\n\n";

    if (dx != 0.0f)
    {
        int yu = floor(py);
        int yd = floor(py + 1.0f);

        int xn = floor((dx > 0.0f ? 1.0f : 0.0f) + px + dx);

        if (_propmap[yu][xn] == MAP_PROP_COLLISION)
            return true;
        else if (yu != yd && static_cast<float>(yd) != (py + 1.0f))
            return _propmap[yd][xn] == MAP_PROP_COLLISION;
    }

    if (dy != 0.0f)
    {
        int xl = floor(px);
        int xr = floor(px + 1.0f);

        int yn = floor((dy > 0.0f ? 1.0f : 0.0f) + py + dy);

        if (dy > 0.0f)
        {
            if (yn == 1.0f + py + dy)
                yn -= 1.0f;
        }

        if (_propmap[yn][xl] == MAP_PROP_COLLISION)
            return true;
        else if (xl != xr && static_cast<float>(xr) != (px + 1.0f))
            return _propmap[yn][xr] == MAP_PROP_COLLISION;
    }

    return false;
}

