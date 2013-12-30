typedef struct 
{  
   float x, y, z;     
}vertex;


typedef struct 
{ 
  vertex a, b, c;
}poly3d;

typedef struct 
{
  poly3d triangles[100];
  int triNumber; 
}obj3ds;



typedef struct {
	float u,v;
}mapcoord;

void draw3ds(obj3ds* object);





obj3ds* load3ds(char* filename);

