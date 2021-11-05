#include "engine.h"
#include "transformation.h"
#include "player.h"
#include <algorithm>
#include <cmath>
#include <iostream>

Engine::Engine() : m_player(Vector3f(0,0,-5.f)), m_textureAtlas(4), Terre(BTYPE_DIRT, "terre"), 
                   Planche(BTYPE_PLANK, "planche"), Gazon(BTYPE_GRASS, "gazon"), Cobble(BTYPE_COBBLE, "roche"), m_chunkArray2d(VIEW_DISTANCE * 2 / CHUNK_SIZE_X, VIEW_DISTANCE * 2 / CHUNK_SIZE_X)
{
}

Engine::~Engine()
{
}

void Engine::Init()
{    
    std::cout << m_chunkArray2d.GetRow() << std::endl;
    std::cout << m_chunkArray2d.GetCol() << std::endl;
    

    for (int i = 0; i < m_chunkArray2d.GetRow(); i++)
    {
        for (int j = 0; j < m_chunkArray2d.GetCol(); j++)
        {
            // m_chunkArray2d.Set(i, j, new Chunk());
            std::cout << "New chunk created" << std::endl;
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

    LoadTexture(m_textureFont, TEXTURE_PATH "font.bmp");
    LoadTexture(m_textureCrosshair, TEXTURE_PATH "cross.bmp");

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
    m_player.CheckFallState(elapsedTime);
    m_player.CheckJump();
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

    if(m_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
        DrawHud(elapsedTime);
    if(m_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);
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
