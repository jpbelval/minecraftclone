#include "engine.h"
#include "transformation.h"
#include "player.h"
#include <algorithm>
#include <cmath>
#include <iostream>

Engine::Engine() : m_player(Vector3f(0,0,-5.f)), m_textureAtlas(4), Terre(BTYPE_DIRT, "terre"), 
                   Planche(BTYPE_PLANK, "planche"), Gazon(BTYPE_GRASS, "gazon"), Cobble(BTYPE_COBBLE, "roche") 
{
}

Engine::~Engine()
{
}

void Engine::Init()
{
    GLenum glewErr = glewInit();
    if(glewErr != GLEW_OK)
    {
        std::cerr << "ERREUR GLEW: " << glewGetErrorString(glewErr) << std::endl;
        abort();
    }
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    // glEnable(GL_CULL_FACE);
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

    TextureAtlas :: TextureIndex texPlanche = m_textureAtlas.AddTexture(TEXTURE_PATH "Planche.jpg");
    m_textureAtlas.TextureIndexToCoord(texPlanche, u, v, w, h);
    m_BlockInfo[BTYPE_PLANK] = new BlockInfo(BTYPE_PLANK, "Planche");
    m_BlockInfo[BTYPE_PLANK] ->SetWHUV(w,h,u,v);
    TextureAtlas :: TextureIndex texTerre = m_textureAtlas.AddTexture(TEXTURE_PATH "textureTerre.jpg");
    m_textureAtlas.TextureIndexToCoord(texTerre, u, v, w, h);
    m_BlockInfo[BTYPE_DIRT] = new BlockInfo(BTYPE_DIRT, "Terre");
    m_BlockInfo[BTYPE_DIRT] ->SetWHUV(w,h,u,v);
    TextureAtlas :: TextureIndex texCobble = m_textureAtlas.AddTexture(TEXTURE_PATH "textureCobble.jpg");
    m_textureAtlas.TextureIndexToCoord(texCobble, u, v, w, h);
    m_BlockInfo[BTYPE_COBBLE] = new BlockInfo(BTYPE_COBBLE, "Pierre");
    m_BlockInfo[BTYPE_COBBLE] ->SetWHUV(w,h,u,v);
    TextureAtlas :: TextureIndex texGazon = m_textureAtlas.AddTexture(TEXTURE_PATH "textureFloor.png");
    m_textureAtlas.TextureIndexToCoord(texGazon, u, v, w, h);
    m_BlockInfo[BTYPE_GRASS] = new BlockInfo(BTYPE_GRASS, "Gazon");
    m_BlockInfo[BTYPE_GRASS] ->SetWHUV(w,h,u,v);
    
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Transformations initiales
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    m_player.Move(m_keyW, m_keyS, m_keyA, m_keyD, elapsedTime);
    m_player.CheckFallState();
    //Player
    Transformation t;
    m_player.ApplyTransformation(t);
    t.Use();

    // Plancher
    // Les vertex doivent etre affiches dans le sens anti-horaire (CCW)
    m_textureAtlas.Bind();
    float nbRep = 50.f;
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0); // Normal vector
    glTexCoord2f(0, 0);
    glVertex3f(-50.f, -2.f, 50.f);
    glTexCoord2f(nbRep, 0);
    glVertex3f(50.f, -2.f, 50.f);
    glTexCoord2f(nbRep, nbRep);
    glVertex3f(50.f, -2.f, -50.f);
    glTexCoord2f(0, nbRep);
    glVertex3f(-50.f, -2.f, -50.f);
    glEnd();

    //if(m_testChunk.IsDirty ())
    //    m_testChunk.Update ();
    //m_shader01.Use();
    //m_testChunk.Render();
    //Shader::Disable ();
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
}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON& button, int x, int y)
{
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
