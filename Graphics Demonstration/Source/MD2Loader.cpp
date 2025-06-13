
#include "MD2Loader.h"

#include <iostream>
#include <fstream>

using namespace std;

// IF Windows.h IS NOT INCLUDED
typedef unsigned char BYTE; 


const int MD2Ident = (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I');


const int MD2Version = 8;

struct Md2Header
{
		int indent;               
		int version;              
		int sknWidth;            
		int sknHeight;           
		int frmSize;            
		int numSkins;             
		int numVertices;		  
		int numTexCoords;		  
		int numTriangles;		  
		int numGlCommands;        
		int numFrames;			  
		int offsetSkins;		  
		int offsetTexcoord;	  
		int offsetTriangles;	  
		int offsetFrames;		  
		int offsetGlcommands;	  
		int offsetEnd;            
};

struct Md2Triangle
{
	short vertexIndex[3];   
	short uvIndex[3];       
};

struct Md2Vertex
{
	BYTE v[3];               
	BYTE lightnormalIndex;    
};

struct Md2Frame
{
	float       scale[3];       
	float       translate[3];  
	char        name[16];       
	Md2Vertex   verts[1];       
};

bool MD2Loader::LoadModel(const char* md2Filename, Model& model)
{
	ifstream   file;           
	Md2Header header;

	// OPEN MODEL FILE
	file.open(md2Filename, ios::in | ios::binary);
	if (file.fail())
	{
		return false;
	}
	
	file.read(reinterpret_cast<char*>(&header), sizeof(Md2Header));
		
	// FILE VERIFICATION
	if ((header.indent != MD2Ident) && (header.version != MD2Version))
	{
		
		file.close();
		return false;
	}

	Md2Triangle* triangles = new Md2Triangle[header.numTriangles];
	BYTE* frameBuffer = new BYTE[header.frmSize];
	Md2Frame* frame = reinterpret_cast<Md2Frame*>(frameBuffer);

	file.seekg(header.offsetTriangles, ios::beg);
	file.read(reinterpret_cast<char*>(triangles), sizeof(Md2Triangle) * header.numTriangles);	
		
	file.seekg(header.offsetFrames, ios::beg);
	file.read(reinterpret_cast<char*>(frame), header.frmSize);	
		
	file.close();

	
	model.Clear();
	// ARRAY OF POLYGONS
	for ( int i = 0; i < header.numTriangles; ++i )
	{
		// ADD NEW POLYGON
		model.AddTriangle(triangles[i].vertexIndex[0], triangles[i].vertexIndex[1], triangles[i].vertexIndex[2]);
	}

	// VERTEX ARRAY
	for( int i = 0; i < header.numVertices; ++i )
	{
		
		model.AddVertex(static_cast<float>((frame->verts[i].v[0] * frame->scale[0]) + frame->translate[0]),
					    static_cast<float>((frame->verts[i].v[2] * frame->scale[2]) + frame->translate[2]),
						static_cast<float>((frame->verts[i].v[1] * frame->scale[1]) + frame->translate[1]));
	}

	delete [] triangles; 
	triangles = 0;

	delete [] frameBuffer;
	frameBuffer = 0;
	frame = 0;

	return true;
}
