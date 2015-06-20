#ifndef __OBJ_HPP__
#define __OBJ_HPP__
#include "3DModel.hpp"
#include <math.h>
#include <vector>
#include <string>

#include "Common.h"

//This struct contain 3 floats and a constructor, it's used for vertexes and normal vectors
struct coordinate{
        float x,y,z;
        coordinate(float a,float b,float c);
};
 
//This structure is store every property of a face
struct face{
        int facenum;    //the number of the face (it's start from 1 not 0, so if you use it as an index, subtract 1 from it), it's used for the normal vectors
        bool four;              //if true, than it's a quad else it's a triangle
        int faces[4];   //indexes for every vertex, which makes the face (it's start from 1 not 0, so if you use it as an index, subtract 1 from it)
        int texcoord[4];        //indexes for every texture coorinate that is in the face (it's start from 1 not 0, so if you use it as an index, subtract 1 from it)
        int mat;                                        //the index for the material, which is used by the face
        face(int facen,int f1,int f2,int f3,int t1,int t2,int t3,int m);        //constuctor for triangle
        face(int facen,int f1,int f2,int f3,int f4,int t1,int t2,int t3,int t4,int m);  //-"- for quad
};
 
//this is a structure, which contain one material
struct material{
        std::string name;       //the name of the material
        float alpha,ns,ni;      //some property, alpha, shininess, and some other, which we not used
        float dif[3],amb[3],spec[3];    //the color property (diffuse, ambient, specular)
        int illum;      //illum - we not use it
        int texture;    //the id for the texture, if there is no texture than -1
        material(const char* na,float al,float n,float ni2,float* d,float* a,float* s,int i,int t);
};
 
//texture coorinate (UV coordinate), nothing to explain here
struct texcoord{
        float u,v;
        texcoord(float a,float b);
};
 
//the main class for the object loader
class OBJModel : public Model
{
        std::vector<std::string*> coord;        //every line of code from the obj file
        std::vector<coordinate*> vertex;        //all vertexes
        std::vector<face*> faces;                                       //all faces
        std::vector<coordinate*> normals;       //all normal vectors
        std::vector<unsigned int> texture;//the id for all the textures (so we can delete the textures after use it)
        std::vector<unsigned int> lists;        //the id for all lists (so we can delete the lists after use it)
        std::vector<material*> materials;       //all materials
        std::vector<texcoord*> texturecoordinate;       //all texture coorinate (UV coordinate)
        bool ismaterial,isnormals,istexture;    //obvious
     
    public:    
        int LoadModelData(const char *path, const char* filename); //private load texture function
        void Cleanup();   //free all of the used memory
       
    public:
        OBJModel();   
        ~OBJModel();   //free the textures and lists
        int load(const char* filename); //the main model load function
    
    private:
        std::string _path;
        uint32 _data;
};
#endif
