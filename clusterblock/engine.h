#ifndef ENGINE_H__
#define ENGINE_H__
#include "define.h"
#include "networking.h"
#include "openglcontext.h"
#include "texture.h"
#include "player.h"
#include "shader.h"
#include "chunk.h"
#include "textureatlas.h"
#include "blockinfo.h"
#include "array2d.h"
#include "networking.h"

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
    virtual void GetBlocAtCursor();

    template <class T>
    Chunk* ChunkAt(T x, T y, T z) const;

    template <class T>
    Chunk* ChunkAt(const Vector3<T>& pos) const;

    template <class T>
    BlockType BlockAt(T x, T y, T z, BlockType defaultBlockType) const;

    template <class T>
    static bool EqualWithEpsilon(const T& v1, const T& v2, T epsilon = T(0.0001));

    template <class T>
    static bool InRangeWithEpsilon(const T& v, const T& vinf, const T& vsup, T epsilon = T(0.0001));

    virtual void CheckCollision(const float& elapsedTime);

    void RenderOnlinePlayers(const float gameTime, const PlayerData* player);


private:
    bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);
    void CreateClientCon();

private:
    bool m_wireframe = false;

    Texture m_textureFloor;
    Texture m_textureCube;
    Texture m_textureGrass;
    Texture m_textureFont;
    Texture m_textureCrosshair;
    Texture m_textureArm;

    Shader m_shader01;

    Player m_player;

    Vector3f m_currentBlock;
    Vector3f m_currentFaceNormal;

    Networking m_network;

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

template <class T>
bool Engine::EqualWithEpsilon(const T& v1, const T& v2, T epsilon)
    {
    return (fabs(v2 - v1) < epsilon);
    }

template <class T>
bool Engine::InRangeWithEpsilon(const T& v, const T& vinf, const T& vsup, T epsilon)
    {
    return (v >= vinf - epsilon && v <= vsup + epsilon);
    }


#endif // ENGINE_H__
