#ifndef ENGINE_H__
#define ENGINE_H__
#include "define.h"
#include "openglcontext.h"
#include "texture.h"
#include "player.h"
#include "shader.h"
#include "chunk.h"
#include "textureatlas.h"
#include "blockinfo.h"
#include "array2d.h"

class Engine : public OpenglContext
{
public:
    Engine();
    virtual ~Engine();
    virtual void Init();
    virtual void DeInit();
    virtual void LoadResource();
    virtual void UnloadResource();
    virtual void DrawHud(const float &elapsedTime);
    virtual void PrintText(unsigned int x, unsigned int y, const std::string& t);
    virtual void Render(float elapsedTime);
    virtual void KeyPressEvent(unsigned char key);
    virtual void KeyReleaseEvent(unsigned char key);
    virtual void MouseMoveEvent(int x, int y);
    virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y);
    virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y);

    template <class T>
    Chunk* ChunkAt(T x, T y, T z) const;

    template <class T>
    Chunk* ChunkAt(const Vector3<T>& pos) const;

    template <class T>
    BlockType BlockAt(T x, T y, T z, BlockType defaultBlockType) const;

    virtual void CheckCollision(const float& elapsedTime);


private:
    bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);

private:
    bool m_wireframe = false;

    Texture m_textureFloor;
    Texture m_textureCube;
    Texture m_textureGrass;
    Texture m_textureFont;
    Texture m_textureCrosshair;

    Shader m_shader01;
    
    Player m_player;

    bool m_keyF3 = true; //Show stats
    bool m_keyW = false;
    bool m_keyA = false;
    bool m_keyS = false;
    bool m_keyD = false;

    TextureAtlas m_textureAtlas;

    BlockInfo Terre;
    BlockInfo Gazon;
    BlockInfo Planche;
    BlockInfo Cobble;

    BlockInfo* m_BlockInfo[BTYPE_LAST];
    Array2d<Chunk*> m_chunkArray2d;

    float w;
    float h;
    float u;
    float v;

};

#endif // ENGINE_H__
