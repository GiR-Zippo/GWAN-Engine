#include "Sprite.hpp"
#include "TextureLoader.hpp"

Sprite::Sprite(uint32 id, float x, float y, float z, std::string tileimage, uint32 tileX, uint32 tileY): 
Object(id, x, y, z, 0,0,0, false, false, false), _tileimage(tileimage), _tileX(tileX), _tileY(tileY)
{
    _initalized =false;
}

Sprite::~Sprite()
{}

void Sprite::Draw()
{
    if (!_initalized)
        return;

    // Enable depth testing if flag is set
    if (_depth)
    {
        glEnable(GL_DEPTH_TEST);
        glAlphaFunc (GL_GREATER, 0.1f);
        glEnable (GL_ALPHA_TEST);
    }
    else
        glDisable(GL_DEPTH_TEST);

    float zoom = _sx / 2;
    float width = _sy;
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glPushMatrix();
    glTranslatef(_x, -_y, _z);
    glRotatef(_rotX, 1, 0, 0);
    glRotatef(_rotY, 0, 1, 0);
    glRotatef(_rotZ, 0, 0, 1);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0);
    glVertex3f(0.0f, 0.0f, 0.1f);

    glTexCoord2d(1.0, 0.0);
    glVertex3f(1.0f, 0.0f, 0.1f);

    glTexCoord2d(1.0, 1.0);
    glVertex3f(1.0f, 1.0f, 0.1f);

    glTexCoord2d(0.0, 1.0);
    glVertex3f(0.0f, 1.0f, 0.1f);
    glEnd();
    glPopMatrix();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    if (_depth)
    {
        glDisable (GL_ALPHA_TEST) ;
        glDisable(GL_DEPTH_TEST);
    }
}

void Sprite::PostDraw()
{
    if (!_initalized)
    {
        _LoadTileTexture(_tileimage, _tileX, _tileY);
        _texture = _textureMap[0];
        _initalized = true;
    }
}

///- Da ein paar Funktionen
void Sprite::SetAminamtionFrame(uint32 index)
{
    if (index > _textureMap.size())
        return;
    _texture = _textureMap[index];
}

void Sprite::SetIdleAnimation(bool left, bool right, bool up, bool down)
{
    if (left)
        _animCounter = 4;
    else if (right)
        _animCounter = 7;
    else if (up)
        _animCounter = 10;
    else if (down)
        _animCounter = 1;
    _texture = _textureMap[_animCounter];
}

void Sprite::MoveUp()
{
    if (_animCounter != 9)
        _animCounter = 9;
    else if (_animCounter != 11)
        _animCounter = 11;
    _texture = _textureMap[_animCounter];
    return;
}

void Sprite::MoveDown()
{
    if (_animCounter != 2)
        _animCounter = 2;
    else if (_animCounter != 0)
        _animCounter = 0;
    _texture = _textureMap[_animCounter];
    return;
}

void Sprite::MoveLeft()
{
    if (_animCounter != 5)
        _animCounter = 5;
    else if (_animCounter != 3)
        _animCounter = 3;
    _texture = _textureMap[_animCounter];
    return;
}

void Sprite::MoveRight()
{
    if (_animCounter != 6)
        _animCounter = 6;
    else if (_animCounter != 8)
        _animCounter = 8;
    _texture = _textureMap[_animCounter];
    return;
}

///- So hier alles mal private
void Sprite::_LoadTileTexture(std::string filename, uint32 x, uint32 y)
{
    TextureLoader* ltext = new TextureLoader();
    std::map<uint32, uint32> temp = ltext->LoadTiledTexture(GetFilePath(filename), x , y);
    uint32 count = 0;
    for (std::map<uint32, uint32>::const_iterator itr = temp.begin(); itr != temp.end(); itr++)
    {
        _textureMap[count] = (*itr).second;
        count++;
    }
    delete ltext;
}