typedef struct vertex
{  
   float x, y, z;     
}vertex;


typedef struct poly3d
{ 
  vertex a, b, c;
}poly3d;

typedef struct obj3ds
{
  poly3d* triangles;
  int triNumber; 
}obj3ds;

typedef struct mapcoord{
	float u,v;
}mapcoord;

void draw3ds(obj3ds* object);
obj3ds* load3ds(char* filename);

