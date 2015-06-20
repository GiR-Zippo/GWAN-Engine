#include "MS3D.hpp"
#include "TextureLoader.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////
//                                      The Model Class
/////////////////////////////////////////////////////////////////////////////////////////////////
MS3D::MS3D()
{
    _path = "";
    m_numMeshes = 0;
    m_pMeshes = NULL;
    m_numMaterials = 0;
    m_pMaterials = NULL;
    m_numTriangles = 0;
    m_pTriangles = NULL;
    m_numVertices = 0;
    m_pVertices = NULL;
}

MS3D::~MS3D()
{
    int i;
    for ( i = 0; i < m_numMeshes; i++ )
        delete[] m_pMeshes[i].m_pTriangleIndices;
    for ( i = 0; i < m_numMaterials; i++ )
        delete[] m_pMaterials[i].m_pTextureFilename;

    m_numMeshes = 0;
    if ( m_pMeshes != NULL )
    {
        delete[] m_pMeshes;
        m_pMeshes = NULL;
    }

    m_numMaterials = 0;
    if ( m_pMaterials != NULL )
    {
        delete[] m_pMaterials;
        m_pMaterials = NULL;
    }

    m_numTriangles = 0;
    if ( m_pTriangles != NULL )
    {
        delete[] m_pTriangles;
        m_pTriangles = NULL;
    }

    m_numVertices = 0;
    if ( m_pVertices != NULL )
    {
        delete[] m_pVertices;
        m_pVertices = NULL;
    }
}

int MS3D::LoadModelData(const char *path, const char *filename)
{
    _path = path;
    std::stringstream fn;
    fn << path << filename;
    ifstream inputFile(fn.str().c_str(), ios::in | ios::binary);
    if ( inputFile.fail())
        return false;

    char pathTemp[PATH_MAX+1];
    int pathLength;
    for ( pathLength = strlen( fn.str().c_str() ); pathLength--; ) 
        if ( fn.str().c_str()[pathLength] == '/' || fn.str().c_str()[pathLength] == '\\' ) 
            break;

    strncpy( pathTemp, fn.str().c_str(), pathLength );

    inputFile.seekg( 0, ios::end );
    long fileSize = inputFile.tellg();
    inputFile.seekg( 0, ios::beg );

    char *pBuffer = new char[fileSize];
    inputFile.read(pBuffer, fileSize );
    inputFile.close();

    const byte *pPtr = reinterpret_cast<unsigned char*>(pBuffer);
    MS3DHeader *pHeader = ( MS3DHeader* )pPtr;
    pPtr += sizeof( MS3DHeader );

    if ( strncmp( pHeader->m_ID, "MS3D000000", 10 ) != 0 )
    {
        printf("Not a valid Milkshape3D model file.\n");
        return -1; // "Not a valid Milkshape3D model file."
    }

    if ( pHeader->m_version < 3 || pHeader->m_version > 4 )
    {
        printf("Unhandled file version. Only Milkshape3D Version 1.3 and 1.4 is supported.\n");
        return -1; // "Unhandled file version. Only Milkshape3D Version 1.3 and 1.4 is supported." );
    }

    int nVertices = *( word* )pPtr;
    m_numVertices = nVertices;
    m_pVertices = new Vertex[nVertices];
    pPtr += sizeof( word );

    int i;
    for ( i = 0; i < nVertices; i++ )
    {
        MS3DVertex *pVertex = ( MS3DVertex* )pPtr;
        m_pVertices[i].m_boneID = pVertex->m_boneID;
        memcpy( m_pVertices[i].m_location, pVertex->m_vertex, sizeof( float )*3 );
        pPtr += sizeof( MS3DVertex );
    }

    int nTriangles = *( word* )pPtr;
    m_numTriangles = nTriangles;
    m_pTriangles = new Triangle[nTriangles];
    pPtr += sizeof( word );

    for ( i = 0; i < nTriangles; i++ )
    {
        MS3DTriangle *pTriangle = ( MS3DTriangle* )pPtr;
        int vertexIndices[3] = { pTriangle->m_vertexIndices[0], pTriangle->m_vertexIndices[1], pTriangle->m_vertexIndices[2] };
        float t[3] = { 1.0f-pTriangle->m_t[0], 1.0f-pTriangle->m_t[1], 1.0f-pTriangle->m_t[2] };
        memcpy( m_pTriangles[i].m_vertexNormals, pTriangle->m_vertexNormals, sizeof( float )*3*3 );
        memcpy( m_pTriangles[i].m_s, pTriangle->m_s, sizeof( float )*3 );
        memcpy( m_pTriangles[i].m_t, t, sizeof( float )*3 );
        memcpy( m_pTriangles[i].m_vertexIndices, vertexIndices, sizeof( int )*3 );
        pPtr += sizeof( MS3DTriangle );
    }



    int nGroups = *( word* )pPtr;
    m_numMeshes = nGroups;
    m_pMeshes = new Mesh[nGroups];
    pPtr += sizeof( word );
    for ( i = 0; i < nGroups; i++ )
    {
        pPtr += sizeof( byte );	// flags
        pPtr += 32;				// name

        word nTriangles = *( word* )pPtr;
        pPtr += sizeof( word );
        int *pTriangleIndices = new int[nTriangles];
        for ( int j = 0; j < nTriangles; j++ )
        {
            pTriangleIndices[j] = *( word* )pPtr;
            pPtr += sizeof( word );
        }

        char materialIndex = *( char* )pPtr;
        pPtr += sizeof( char );

        m_pMeshes[i].m_materialIndex = materialIndex;
        m_pMeshes[i].m_numTriangles = nTriangles;
        m_pMeshes[i].m_pTriangleIndices = pTriangleIndices;
    }



    int nMaterials = *( word* )pPtr;
    m_numMaterials = nMaterials;
    m_pMaterials = new Material[nMaterials];
    pPtr += sizeof( word );
    for ( i = 0; i < nMaterials; i++ )
    {
        MS3DMaterial *pMaterial = ( MS3DMaterial* )pPtr;
        memcpy( m_pMaterials[i].m_ambient, pMaterial->m_ambient, sizeof( float )*4 );
        memcpy( m_pMaterials[i].m_diffuse, pMaterial->m_diffuse, sizeof( float )*4 );
        memcpy( m_pMaterials[i].m_specular, pMaterial->m_specular, sizeof( float )*4 );
        memcpy( m_pMaterials[i].m_emissive, pMaterial->m_emissive, sizeof( float )*4 );
        m_pMaterials[i].m_shininess = pMaterial->m_shininess;
        if ( strncmp( pMaterial->m_texture, ".\\", 2 ) == 0 ) {
            // MS3D 1.5.x relative path
            strcpy( pathTemp + pathLength, pMaterial->m_texture + 1 );
            m_pMaterials[i].m_pTextureFilename = new char[strlen( pathTemp )+1];
            strcpy( m_pMaterials[i].m_pTextureFilename, pathTemp );
        }
        else {
            // MS3D 1.4.x or earlier - absolute path
            m_pMaterials[i].m_pTextureFilename = new char[strlen( pMaterial->m_texture )+1];
            strcpy( m_pMaterials[i].m_pTextureFilename, pMaterial->m_texture );
        }
        pPtr += sizeof( MS3DMaterial );
    }

    reloadTextures();
    delete[] pBuffer;

    int num;
    num=glGenLists(1);      //I generate a unique identifier for the list
    glNewList(num,GL_COMPILE);
    
    GLboolean texEnabled = glIsEnabled(GL_TEXTURE_2D);

    for ( int i = 0; i < m_numMeshes; i++ )                     // draw in groups
    {
        int materialIndex = m_pMeshes[i].m_materialIndex;
        if ( materialIndex >= 0 )
        {
            glMaterialfv( GL_FRONT, GL_AMBIENT,   m_pMaterials[materialIndex].m_ambient );
            glMaterialfv( GL_FRONT, GL_DIFFUSE,   m_pMaterials[materialIndex].m_diffuse );
            glMaterialfv( GL_FRONT, GL_SPECULAR,  m_pMaterials[materialIndex].m_specular );
            glMaterialfv( GL_FRONT, GL_EMISSION,  m_pMaterials[materialIndex].m_emissive );
            glMaterialf(  GL_FRONT, GL_SHININESS, m_pMaterials[materialIndex].m_shininess );

            if ( m_pMaterials[materialIndex].m_texture > 0 )
            {
                glBindTexture( GL_TEXTURE_2D, m_pMaterials[materialIndex].m_texture );
                glEnable(GL_TEXTURE_2D);
                glEnable(GL_BLEND);
            }
            else
            {
                glDisable(GL_BLEND);
                glDisable(GL_TEXTURE_2D);
            }
        }
        else
        {
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
        }

        glBegin( GL_TRIANGLES );
        {
            for ( int j = 0; j < m_pMeshes[i].m_numTriangles; j++ )
            {
                int triangleIndex = m_pMeshes[i].m_pTriangleIndices[j];
                const Triangle* pTri = &m_pTriangles[triangleIndex];

                for ( int k = 0; k < 3; k++ )
                {
                    int index = pTri->m_vertexIndices[k];

                    glNormal3fv( pTri->m_vertexNormals[k] );
                    glTexCoord2f( pTri->m_s[k], pTri->m_t[k] );
                    glVertex3fv( m_pVertices[index].m_location );
                }
            }
        }
        glEnd();
    }

    /*if ( texEnabled )
    {
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
    }
    else*/
    {
        glDisable(GL_BLEND);
        glDisable( GL_TEXTURE_2D );
    }
    glEndList();
    Cleanup();
    return num;
}

void MS3D::reloadTextures()
{
    for ( int i = 0; i < m_numMaterials; i++ )
    {
        if ( strlen( m_pMaterials[i].m_pTextureFilename ) > 0 )
        {
            std::stringstream fn;
            std::string file = m_pMaterials[i].m_pTextureFilename;
            file = file.substr(0, file.length() - 4);
            fn << _path << file << ".png";
            TextureLoader *tload = new TextureLoader();
            m_pMaterials[i].m_texture = tload->LoadTexture( fn.str().c_str() );
            delete tload;
        }
        else
        {
            m_pMaterials[i].m_texture = 0;
        }
    }
}

void MS3D::Cleanup()
{
    int i;
    for ( i = 0; i < m_numMeshes; i++ )
        delete[] m_pMeshes[i].m_pTriangleIndices;
    for ( i = 0; i < m_numMaterials; i++ )
        delete[] m_pMaterials[i].m_pTextureFilename;

    m_numMeshes = 0;
    if ( m_pMeshes != NULL )
    {
        delete[] m_pMeshes;
        m_pMeshes = NULL;
    }

    m_numMaterials = 0;
    if ( m_pMaterials != NULL )
    {
        delete[] m_pMaterials;
        m_pMaterials = NULL;
    }

    m_numTriangles = 0;
    if ( m_pTriangles != NULL )
    {
        delete[] m_pTriangles;
        m_pTriangles = NULL;
    }

    m_numVertices = 0;
    if ( m_pVertices != NULL )
    {
        delete[] m_pVertices;
        m_pVertices = NULL;
    }
}