#ifndef DEFINE_H__
#define DEFINE_H__

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#ifdef _WIN32
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#else
#include <GL/glew.h>
#endif

#define SHADER_PATH "../clusterblock/media/shaders/"
#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 128
#define CHUNK_SIZE_Z 16
#define FALLSPEED 1
#define JUMP_HEIGHT 1
#define WALK_SPEED 8
#define MAX_SELECTION_DISTANCE 5

typedef uint8_t BlockType;
enum BLOCK_TYPE {BTYPE_AIR, BTYPE_DIRT, BTYPE_GRASS, BTYPE_PLANK, BTYPE_COBBLE, BTYPE_SABLE, BTYPE_BEDROCK, BTYPE_BOIS, BTYPE_FEUILLE, BTYPE_LAST};


#define TEXTURE_PATH        "../clusterblock/media/textures/"
#define VIEW_DISTANCE       128

#endif // DEFINE_H__
