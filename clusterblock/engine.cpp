#include "engine.h"
#include "transformation.h"
#include "player.h"
#include "chunk.h"
#include "networking.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <new>
#include <string>

Engine::Engine() : m_player(Vector3f(5.f,30.f,5.f)), m_textureAtlas(4), Terre(BTYPE_DIRT, "terre"), 
                   Planche(BTYPE_PLANK, "planche"), Gazon(BTYPE_GRASS, "gazon"), Cobble(BTYPE_COBBLE, "roche"),
                    m_chunkArray2d(VIEW_DISTANCE * 2 / CHUNK_SIZE_X, VIEW_DISTANCE * 2 / CHUNK_SIZE_Z), m_network("127.0.0.1", 7777)
{
}

Engine::~Engine()
{
}

void Engine::Init()
{    


    for (int i = 0; i < m_chunkArray2d.GetRow(); i++)
    {
        for (int j = 0; j < m_chunkArray2d.GetCol(); j++)
        {
            m_chunkArray2d.Set(i,j,new Chunk(i * CHUNK_SIZE_X, j * CHUNK_SIZE_Z));
        }
        
    }
  
    
    GLenum glewErr = glewInit();
    if(glewErr != GLEW_OK)
    {
        std::cerr << "ERREUR GLEW: " << glewGetErrorString(glewErr) << std::endl;
        abort();
    }
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)Width() / (float)Height(), 0.0001f, 1000.0f);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);

    glEnable(GL_CULL_FACE);
    // Light
    GLfloat light0Pos[4] = { 0.0f, CHUNK_SIZE_Y, 0.0f, 1.0f };
    GLfloat light0Amb[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat light0Diff[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light0Spec[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);

    CenterMouse();
    HideCursor();
}

void Engine::DeInit()
{
}

void Engine::LoadResource()
{
    std::cout << "Loading and compiling shaders ..." << std::endl;
    if(!m_shader01.Load(SHADER_PATH "shader01.vert", SHADER_PATH "shader01.frag", true))
    {
        std::cout << "Failed to load shader" << std::endl;
        exit (1);
    }

    LoadTexture(m_textureCube, TEXTURE_PATH "joe.png");
    LoadTexture(m_textureFont, TEXTURE_PATH "font.bmp");
    LoadTexture(m_textureCrosshair, TEXTURE_PATH "cross.bmp");
    LoadTexture(m_textureArm, TEXTURE_PATH "arm.png");

    TextureAtlas :: TextureIndex texPlanche = m_textureAtlas.AddTexture(TEXTURE_PATH "Planche.jpg");
    m_textureAtlas.TextureIndexToCoord(texPlanche, u, v, w, h);
    m_BlockInfo[BTYPE_PLANK] = new BlockInfo(BTYPE_PLANK, "Planche");
    m_BlockInfo[BTYPE_PLANK] ->SetWHUV(w,h,u,v);
    m_BlockInfo[BTYPE_PLANK] ->SetDurability(15);
    TextureAtlas :: TextureIndex texTerre = m_textureAtlas.AddTexture(TEXTURE_PATH "textureTerre.jpg");
    m_textureAtlas.TextureIndexToCoord(texTerre, u, v, w, h);
    m_BlockInfo[BTYPE_DIRT] = new BlockInfo(BTYPE_DIRT, "Terre");
    m_BlockInfo[BTYPE_DIRT] ->SetWHUV(w,h,u,v);
    m_BlockInfo[BTYPE_DIRT] ->SetDurability(10);
    TextureAtlas :: TextureIndex texCobble = m_textureAtlas.AddTexture(TEXTURE_PATH "textureCobble.jpg");
    m_textureAtlas.TextureIndexToCoord(texCobble, u, v, w, h);
    m_BlockInfo[BTYPE_COBBLE] = new BlockInfo(BTYPE_COBBLE, "Pierre");
    m_BlockInfo[BTYPE_COBBLE] ->SetWHUV(w,h,u,v);
    m_BlockInfo[BTYPE_COBBLE] ->SetDurability(30);
    TextureAtlas :: TextureIndex texGazon = m_textureAtlas.AddTexture(TEXTURE_PATH "textureFloor.png");
    m_textureAtlas.TextureIndexToCoord(texGazon, u, v, w, h);
    m_BlockInfo[BTYPE_GRASS] = new BlockInfo(BTYPE_GRASS, "Gazon");
    m_BlockInfo[BTYPE_GRASS] ->SetWHUV(w,h,u,v);
    m_BlockInfo[BTYPE_GRASS] ->SetDurability(10);
    
    if(! m_textureAtlas.Generate (128, false))
    {
        std::cout << "Unable to generate texture atlas ..." << std::endl;
        abort();
    }
}

void Engine::UnloadResource()
{
}

void Engine::Render(float elapsedTime)
{
    static float gameTime = elapsedTime;

    gameTime += elapsedTime;

    m_network.ReceiveData();

    // Envoyer position player
    std::string msgPosition = std::to_string(m_player.GetPosition().x) + "x" + std::to_string(m_player.GetPosition().y) + "y" + std::to_string(m_player.GetPosition().z) + "z";
    char messageData[80] = "1|";
    strcat(messageData, msgPosition.c_str());
    m_network.SendPacket(messageData);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Transformations initiales
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    CheckCollision(elapsedTime);
    
    //Player
    Transformation t;
    m_player.ApplyTransformation(t);
    t.Use();
    std::map<int, PlayerData*> players = m_network.GetPlayers();
    std::cout << players.size() << std::endl;
    if (players.size() > 1)
    {
        for (std::map<int, PlayerData*>::iterator it = players.begin(); it != players.end(); ++it)
        {
            RenderOnlinePlayers(gameTime, it->second);
        }
    }
    
    
    //FIN

    m_textureAtlas.Bind();

    m_shader01.Use();
    for (int i = 0; i < m_chunkArray2d.GetRow(); i++)
    {
        for (int j = 0; j < m_chunkArray2d.GetCol(); j++)
        {
            if(m_chunkArray2d.Get(i,j)->IsDirty())
                m_chunkArray2d.Get(i,j)->Update(m_BlockInfo);
            m_chunkArray2d.Get(i,j)->Render();
        }
        
    }    
    Shader::Disable();

    DrawHud(elapsedTime);
    if(m_wireframe){
        glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
        glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);
    }
    GetBlocAtCursor();
}

void Engine::RenderOnlinePlayers(float gameTime, const PlayerData* player)
{
    std::cout << "Render player" << std::endl;
    //Other players
    m_textureCube.Bind();

//===============================================================
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(player->GetPosition().x - 0.5f, player->GetPosition().y - 0.5f, player->GetPosition().z + 0.5f);

    glTexCoord2f(1, 0);
    glVertex3f(player->GetPosition().x + 0.5f, player->GetPosition().y - 0.5f, player->GetPosition().z + 0.5f);

    glTexCoord2f(1, 1);
    glVertex3f(player->GetPosition().x + 0.5f, player->GetPosition().y + 0.5f, player->GetPosition().z + 0.5f);

    glTexCoord2f(0, 1);
    glVertex3f(player->GetPosition().x - 0.5f, player->GetPosition().y + 0.5f, player->GetPosition().z + 0.5f);
    glEnd();
//=================================================================
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);

    glTexCoord2f(0, 0);
    glVertex3f(player->GetPosition().x + 0.5f, player->GetPosition().y - 0.5f, player->GetPosition().z + 0.5f);

    glTexCoord2f(1, 0);
    glVertex3f(player->GetPosition().x + 0.5f, player->GetPosition().y - 0.5f, player->GetPosition().z - 0.5f);

    glTexCoord2f(1, 1);
    glVertex3f(player->GetPosition().x + 0.5f, player->GetPosition().y + 0.5f, player->GetPosition().z - 0.5f);

    glTexCoord2f(0, 1);
    glVertex3f(player->GetPosition().x + 0.5f, player->GetPosition().y + 0.5f, player->GetPosition().z + 0.5f);
    glEnd();
//================================================================
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);

    glTexCoord2f(0, 0);
    glVertex3f(player->GetPosition().x + 0.5f, player->GetPosition().y - 0.5f, player->GetPosition().z - 0.5f);

    glTexCoord2f(1, 0);
    glVertex3f(player->GetPosition().x - 0.5f, player->GetPosition().y - 0.5f, player->GetPosition().z - 0.5f);

    glTexCoord2f(1, 1);
    glVertex3f(player->GetPosition().x - 0.5f, player->GetPosition().y + 0.5f, player->GetPosition().z - 0.5f);

    glTexCoord2f(0, 1);
    glVertex3f(player->GetPosition().x + 0.5f, player->GetPosition().y + 0.5f, player->GetPosition().z - 0.5f);
    
    glEnd();
//=================================================================
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);

    glTexCoord2f(0, 0);
    glVertex3f(player->GetPosition().x - 0.5f, player->GetPosition().y - 0.5f, player->GetPosition().z - 0.5f);

    glTexCoord2f(1, 0);
    glVertex3f(player->GetPosition().x - 0.5f, player->GetPosition().y - 0.5f, player->GetPosition().z + 0.5f);

    glTexCoord2f(1, 1);
    glVertex3f(player->GetPosition().x - 0.5f, player->GetPosition().y + 0.5f, player->GetPosition().z + 0.5f);

    glTexCoord2f(0, 1);
    glVertex3f(player->GetPosition().x - 0.5f, player->GetPosition().y + 0.5f, player->GetPosition().z - 0.5f);
    
    glEnd();

    //===================================================================
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);

    glTexCoord2f(0, 0);
    glVertex3f(player->GetPosition().x - 0.5f, player->GetPosition().y + 0.5f, player->GetPosition().z + 0.5f);

    glTexCoord2f(1, 0);
    glVertex3f(player->GetPosition().x + 0.5f, player->GetPosition().y + 0.5f, player->GetPosition().z + 0.5f);

    glTexCoord2f(1, 1);
    glVertex3f(player->GetPosition().x + 0.5f, player->GetPosition().y + 0.5f, player->GetPosition().z - 0.5f);

    glTexCoord2f(0, 1);
    glVertex3f(player->GetPosition().x - 0.5f, player->GetPosition().y + 0.5f, player->GetPosition().z - 0.5f);
    
    glEnd();

    //===================================================================
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);

    glTexCoord2f(0, 0);
    glVertex3f(player->GetPosition().x + 0.5f, player->GetPosition().y - 0.5f, player->GetPosition().z + 0.5f);

    glTexCoord2f(1, 0);
    glVertex3f(player->GetPosition().x - 0.5f, player->GetPosition().y - 0.5f, player->GetPosition().z + 0.5f);

    glTexCoord2f(1, 1);
    glVertex3f(player->GetPosition().x - 0.5f, player->GetPosition().y - 0.5f, player->GetPosition().z - 0.5f);

    glTexCoord2f(0, 1);
    glVertex3f(player->GetPosition().x + 0.5f, player->GetPosition().y - 0.5f, player->GetPosition().z - 0.5f);
    
    glEnd();
}

void Engine::KeyPressEvent(unsigned char key)
{
    switch(key)
    {
        case 36: // ESC
            Stop();
            break;
        case 94: // F10
            SetFullscreen(!IsFullscreen());
            break;
        case 22: // W
            m_keyW = true;
            break;
        case 0:  // A
            m_keyA = true;
            break;
        case 18: // S
            m_keyS = true;
            break;
        case 3:  // D
            m_keyD = true;
            break;
        case 5: //F
            m_player.ToggleisFly();
            break;
        case 87:
            m_keyF3 = !m_keyF3;
            break;
        case 57:
            m_player.Jump();
            break;
        case 38:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(50.0f, (float)Width() / (float)Height(), 0.0001f, 1000.0f);
            m_player.SetSpeed(WALK_SPEED + 5.f);
            break;
        default:
            std::cout << "Unhandled key: " << (int)key << std::endl;
    }
}

void Engine::KeyReleaseEvent(unsigned char key)
{
    switch(key)
    {
        case 24: // Y
            m_wireframe = !m_wireframe;
            if(m_wireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case 22: // W
            m_keyW = false;
            break;
        case 0:  // A
            m_keyA = false;
            break;
        case 18: // S
            m_keyS = false;
            break;
        case 3:  // D
            m_keyD = false;
            break;
        case 38:
            m_player.SetSpeed(WALK_SPEED);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45.0f, (float)Width() / (float)Height(), 0.0001f, 1000.0f);
            break;
    }
}

void Engine::MouseMoveEvent(int x, int y)
{
    // Centrer la souris seulement si elle n'est pas d�j� centr�e
    // Il est n�cessaire de faire la v�rification pour �viter de tomber
    // dans une boucle infinie o� l'appel � CenterMouse g�n�re un
    // MouseMoveEvent, qui rapelle CenterMouse qui rapelle un autre
    // MouseMoveEvent, etc

    if(x == (Width() / 2) && y == (Height() / 2))
        return;

    MakeRelativeToCenter(x,y);
    m_player.TurnLeftRight(x * 0.05);
    m_player.TurnTopBottom(y * 0.05);
    CenterMouse();
}

void Engine::MousePressEvent(const MOUSE_BUTTON& button, int x, int y)
{
    switch (button)
    {
    case MOUSE_BUTTON_LEFT:
        if (m_currentBlock.x >= 0 && m_currentBlock.y >= 0 && m_currentBlock.z >= 0)
        {
            int bx = (int)m_currentBlock.x % CHUNK_SIZE_X;
            int by = (int)m_currentBlock.y % CHUNK_SIZE_Y;
            int bz = (int)m_currentBlock.z % CHUNK_SIZE_Z;

            ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z )->RemoveBlock(bx, by, bz);
        }
        
        break;
    case MOUSE_BUTTON_RIGHT:
        if (m_currentBlock.x >= 0 && m_currentBlock.y >= 0 && m_currentBlock.z >= 0)
        {
            int bx = (int)m_currentBlock.x % CHUNK_SIZE_X;
            int by = (int)m_currentBlock.y % CHUNK_SIZE_Y;
            int bz = (int)m_currentBlock.z % CHUNK_SIZE_Z;

            std::cout << "x: " << m_currentFaceNormal.x << "y: " << m_currentFaceNormal.y << "z: " << m_currentFaceNormal.z << std::endl;

            if (m_currentFaceNormal.x == 1)
            {
                ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(bx + 1, by, bz, BTYPE_DIRT);
            }
            else if (m_currentFaceNormal.x == -1)
            {
                ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(bx - 1, by, bz, BTYPE_DIRT);
            }
            else if (m_currentFaceNormal.y == 1)
            {
                ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(bx, by + 1, bz, BTYPE_DIRT);
            }
            else if (m_currentFaceNormal.y == -1)
            {
                ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(bx, by - 1, bz, BTYPE_DIRT);
            }
            else if (m_currentFaceNormal.z == 1 )
            {
                ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(bx, by, bz + 1, BTYPE_DIRT);
            }
            else if (m_currentFaceNormal.z == -1)
            {
                ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(bx, by, bz - 1, BTYPE_DIRT);
            }
        }
        break;
    default:
        break;
    }
}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON& button, int x, int y)
{

}



void Engine::DrawHud(const float &elaspedTime){
    glDisable(GL_LIGHTING);
    glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();
    glOrtho(0, Width (), 0, Height (), -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix ();
    // Bind de la texture pour le font
    if (m_keyF3)
    {
        m_textureFont.Bind();
        std:: ostringstream ss;
        ss << "Fps: " << 1/elaspedTime;
        PrintText (10, Height () - 25, ss.str());
        ss.str ("");
        ss << "Is Falling: " << m_player.GetIsFalling();
        PrintText (10, 20, ss.str());
        ss.str ("");
        ss << "Position: " << m_player.GetPosition(); // IMPORTANT: on utilise l’operateur << pour afficher la position
        PrintText (10, 10, ss.str());
    }
    // Affichage du crosshair
    m_textureCrosshair.Bind();
    static const int crossSize = 32;
    glLoadIdentity ();
    glTranslated(Width () / 2 - crossSize / 2, Height () / 2 - crossSize / 2, 0);
    glBegin(GL_QUADS);
    glTexCoord2f (0, 0);
    glVertex2i (0, 0);
    glTexCoord2f (1, 0);
    glVertex2i(crossSize , 0);
    glTexCoord2f (1, 1);
    glVertex2i(crossSize , crossSize);
    glTexCoord2f (0, 1);
    glVertex2i (0, crossSize);
    glEnd();

    m_textureArm.Bind();
    static const int armSize = Width()/4;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity ();
    glTranslated(Width() - (armSize + 10), 0, 0);
    glBegin(GL_QUADS);
    glTexCoord2f (0, 0);
    glVertex2i (0, 0);
    glTexCoord2f (1, 0);
    glVertex2i(armSize , 0);
    glTexCoord2f (1, 1);
    glVertex2i(armSize , armSize);
    glTexCoord2f (0, 1);
    glVertex2i (0, armSize);
    glEnd();

    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix ();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix ();
}

void Engine::PrintText(unsigned int x, unsigned int y, const std::string& t){
    glLoadIdentity ();
    glTranslated(x, y, 0);
    for(unsigned int i=0; i<t.length (); ++i)
    {
        float left = (float)((t[i] - 32) % 16) / 16.0f;
        float top = (float)((t[i] - 32) / 16) / 16.0f;
        top += 0.5f;
        glBegin(GL_QUADS);
        glTexCoord2f(left , 1.0f - top - 0.0625f);
        glVertex2f (0, 0);
        glTexCoord2f(left + 0.0625f, 1.0f - top - 0.0625f);
        glVertex2f (12, 0);
        glTexCoord2f(left + 0.0625f, 1.0f - top);
        glVertex2f (12, 12);
        glTexCoord2f(left , 1.0f - top);
        glVertex2f (0, 12);
        glEnd();
        glTranslated (8, 0, 0);
    }
}

bool Engine::LoadTexture(Texture& texture, const std::string& filename, bool stopOnError)
{
    texture.Load(filename);
    if(!texture.IsValid())
    {
        std::cerr << "Unable to load texture (" << filename << ")" << std::endl;
        if(stopOnError)
            Stop();

        return false;
    }

    return true;
}

template <class T>
Chunk* Engine::ChunkAt(T x, T y, T z) const
{
    int cx = (int)x / CHUNK_SIZE_X;
    int cz = (int)z / CHUNK_SIZE_Z;

    if(x < 0 || y < 0 || z < 0){
        return 0;
    }

    return m_chunkArray2d.Get(cx, cz);
}

template <class T>
Chunk* Engine::ChunkAt(const Vector3<T>& pos) const
{
    return ChunkAt(pos.x, pos.y, pos.z);
}

template <class T>
BlockType Engine::BlockAt(T x, T y, T z, BlockType defaultBlockType) const
{
    Chunk* c = ChunkAt(x, y, z);

    if (!c)
        return defaultBlockType;
    
    int bx = (int)x % CHUNK_SIZE_X;
    int by = (int)y % CHUNK_SIZE_Y;
    int bz = (int)z % CHUNK_SIZE_Z;

    return c->GetBlock(bx, by, bz);
}

void Engine::CheckCollision(const float &elapsedTime)
{
    Vector3f pos = m_player.GetPosition();
    Vector3f delta = m_player.SimulateMove(m_keyW, m_keyS, m_keyA, m_keyD, elapsedTime);
    BlockType bt1, bt2, bt3;
    if (!m_player.GetIsFlying())
    {
        
        bt1 = BlockAt(std::round(pos.x + delta.x), std::round(pos.y), std::round(pos.z), BTYPE_AIR);
        bt2 = BlockAt(std::round(pos.x + delta.x), std::round(pos.y - 0.9f), std::round(pos.z), BTYPE_AIR);
        bt3 = BlockAt(std::round(pos.x + delta.x), std::round(pos.y - 1.2f), std::round(pos.z), BTYPE_AIR);
        if(bt1 != BTYPE_AIR || bt2 != BTYPE_AIR || bt3 != BTYPE_AIR){
            delta.x = 0;
        }

        bt1 = BlockAt(std::round(pos.x), std::round(pos.y), std::round(pos.z + delta.z), BTYPE_AIR);
        bt2 = BlockAt(std::round(pos.x), std::round(pos.y - 0.9f), std::round(pos.z + delta.z), BTYPE_AIR);
        bt3 = BlockAt(std::round(pos.x), std::round(pos.y - 1.2f), std::round(pos.z + delta.z), BTYPE_AIR);
        if(bt1 != BTYPE_AIR || bt2 != BTYPE_AIR || bt3 != BTYPE_AIR)
        {
            delta.z = 0;
        }

        bt1 = BlockAt(std::round(pos.x + delta.x), pos.y - 1.2f, std::round(pos.z + delta.z), BTYPE_AIR);
        if(bt1 != BTYPE_AIR){
            if (delta.y < 0)
            {
                delta.y = 0;
                m_player.SetIsFalling(false);
                m_player.SetFallTime(0);
                m_player.SetIsJumping(false);
            }
        }
        else
        {
            m_player.SetIsFalling(true);
        }
        

        bt1 = BlockAt(std::round(pos.x + delta.x), pos.y + delta.y, std::round(pos.z + delta.z), BTYPE_AIR);
        bt2 = BlockAt(std::round(pos.x + delta.x), pos.y - 1.2f, std::round(pos.z + delta.z), BTYPE_AIR);
        if (bt1 != BTYPE_AIR && bt2 == BTYPE_AIR)
        {
            delta.y = 0;
            m_player.SetIsJumping(false);
        }
    }

    pos.y += delta.y;
    pos.x += delta.x;
    pos.z += delta.z;

    m_player.SetPosition(pos);
    
}

void Engine::GetBlocAtCursor(){
    int x = Width() / 2;
    int y = Height() / 2;

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    

    posX += .5f;
    posY += .5f;
    posZ += .5f;

    // Le cast vers int marche juste pour les valeurs entiere, utiliser une fonction de la libc si besoin
    // de valeurs negatives
    int px = (int)(posX);
    int py = (int)(posY);
    int pz = (int)(posZ);

    
    bool found = false;

    if((m_player.GetPosition() - Vector3f((float)posX, (float)posY, (float)posZ)).Length() < MAX_SELECTION_DISTANCE)
    {
        // Apres avoir determine la position du bloc en utilisant la partie entiere du hit
        // point retourne par opengl, on doit verifier de chaque cote du bloc trouve pour trouver
        // le vrai bloc. Le vrai bloc peut etre different a cause d'erreurs de precision de nos
        // nombres flottants (si z = 14.999 par exemple, et qu'il n'y a pas de blocs a la position
        // 14 (apres arrondi vers l'entier) on doit trouver et retourner le bloc en position 15 s'il existe
        // A cause des erreurs de precisions, ils arrive que le cote d'un bloc qui doit pourtant etre a la
        // position 15 par exemple nous retourne plutot la position 15.0001
        for(int x = px - 1; !found && x <= px + 1; ++x)
        {
            for(int y = py - 1; !found && x >= 0 && y <= py + 1; ++y)
            {
                for(int z = pz - 1; !found && y >= 0 && z <= pz + 1; ++z)
                {
                    if(z >= 0)
                    {
                        BlockType bt = BlockAt((float)x,(float)y, (float)z, BTYPE_AIR);
                        if(bt == BTYPE_AIR)
                            continue;

                        // Skip water blocs
                        //if(bloc->Type == BT_WATER)
                        //    continue;
                        
                        

                        m_currentBlock.x = x;
                        m_currentBlock.y = y;
                        m_currentBlock.z = z;

                        if(InRangeWithEpsilon<float>((float)posX, (float)x, (float)x + 1.f, 0.05f) && InRangeWithEpsilon<float>((float)posY, (float)y, (float)y + 1.f, 0.05f) && InRangeWithEpsilon<float>((float)posZ, (float)z, (float)z + 1.f, 0.05f))
                        {
                            found = true;
                        }
                    }
                }
            }
        }
    }

    if(!found)
    {
        m_currentBlock.x = -1;
    }
    else
    {
        // Find on which face of the bloc we got an hit
        m_currentFaceNormal.Zero();
        const float epsilon = 0.03f;

        // Front et back:
        if(EqualWithEpsilon<float>((float)posZ, (float)m_currentBlock.z, epsilon))
            m_currentFaceNormal.z = -1;
        else if(EqualWithEpsilon<float>((float)posZ, (float)m_currentBlock.z + 1.f, epsilon))
            m_currentFaceNormal.z = 1;
        else if(EqualWithEpsilon<float>((float)posX, (float)m_currentBlock.x, epsilon))
            m_currentFaceNormal.x = -1;
        else if(EqualWithEpsilon<float>((float)posX, (float)m_currentBlock.x + 1.f, epsilon))
            m_currentFaceNormal.x = 1;
        else if(EqualWithEpsilon<float>((float)posY, (float)m_currentBlock.y, epsilon))
            m_currentFaceNormal.y = -1;
        else if(EqualWithEpsilon<float>((float)posY, (float)m_currentBlock.y + 1.f, epsilon))
            m_currentFaceNormal.y = 1;
    }
}

