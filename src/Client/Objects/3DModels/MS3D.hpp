// Thanks to Brett Porter and Mete Ciragan for help with this MS3D model loading code

#ifndef MS3D_H
#define MS3D_H
#include "3DObject.hpp"

/*
	MS3D STRUCTURES
*/
#ifdef _MSC_VER							// byte-align structures
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#	define PATH_MAX _MAX_PATH
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#	include <limits.h>
#else
#	error you must byte-align these structures with the appropriate compiler directives
#endif

typedef unsigned char byte;
typedef unsigned short word;

struct MS3DHeader						// File Header
{
    char m_ID[10];
    int m_version;
} PACK_STRUCT;

struct MS3DVertex						// Vertex info
{
    byte m_flags;
    float m_vertex[3];
    char m_boneID;
    byte m_refCount;
} PACK_STRUCT;

struct MS3DTriangle						// Triangle info
{
    word m_flags;
    word m_vertexIndices[3];
    float m_vertexNormals[3][3];
    float m_s[3], m_t[3];
    byte m_smoothingGroup;
    byte m_groupIndex;
} PACK_STRUCT;

struct MS3DMaterial						// Material info
{
    char m_name[32];
    float m_ambient[4];
    float m_diffuse[4];
    float m_specular[4];
    float m_emissive[4];
    float m_shininess;					// 0.0f - 128.0f
    float m_transparency;				// 0.0f - 1.0f
    byte m_mode;						// 0, 1, 2 (unused now)
    char m_texture[128];
    char m_alphamap[128];
} PACK_STRUCT;

#ifdef _MSC_VER							// Default alignment
#	pragma pack( pop, packing )
#endif

#undef PACK_STRUCT




struct Mesh								//	Mesh struct
{
    int m_materialIndex;
    int m_numTriangles;
    int *m_pTriangleIndices;
};

struct Material							//	Material properties
{
    float m_ambient[4], m_diffuse[4], m_specular[4], m_emissive[4];
    float m_shininess;
    GLuint m_texture;
    char *m_pTextureFilename;
};

struct Triangle							//	Triangle struct
{
    float m_vertexNormals[3][3];
    float m_s[3], m_t[3];
    int m_vertexIndices[3];
};

struct Vertex							//	Vertex struct
{
    char m_boneID;
    float m_location[3];
};

class MS3D : public Model
{
    public:

        MS3D();
        virtual ~MS3D();
        int LoadModelData(const char *path, const char *filename);
        void Cleanup();
        void reloadTextures();

    protected:
        uint32 _data;
        std::string _path;
        int m_numMeshes;
        Mesh *m_pMeshes;

        int m_numMaterials;
        Material *m_pMaterials;

        int m_numTriangles;
        Triangle *m_pTriangles;

        int m_numVertices;
        Vertex *m_pVertices;
};
#endif