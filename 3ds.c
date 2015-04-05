#include <stdio.h>
#include "3ds.h"




obj3ds* load3ds(char* filename)
{
  unsigned char chunk[4];
  int length, curchunk, verticecount, facecount, count, countwo;
  vertex *vertlist;
  unsigned int *facelist;
  obj3ds *outobject;
  int foundverts = 0, foundfaces = 0;
  
    
   FILE* file3ds = fopen(filename, "rb");
   FILE* log = fopen("obj3ds.log", "w");
          
     fread(chunk, 1, 2, file3ds); 
           
       curchunk = (chunk[1]<<8) + chunk[0];
       
     fread(&length, 4, 1, file3ds);
       
       fprintf (log, "\nChunk: %x \nLength: %x bytes\n", curchunk, length);
       
    
       
while(1)
{
       
   fread(chunk, 1, 2, file3ds);       
   curchunk = (chunk[1]<<8) + chunk[0];
        
   fread(&length, 4, 1, file3ds);
       
       
       fprintf (log, "\nChunk: %x \nLength: %x bytes\n", curchunk, length);
       
     if((curchunk == 15677) || (curchunk == 16384))
     {
     
      if(curchunk == 16384)
      {
      
       while(1)
       {
        fread(chunk, 1, 1, file3ds);  
        if(chunk[0] == 0)
        break;
        }
      }  
      }
     
     if(curchunk == 16640) 
     {
      fprintf(log, "\nfound the triangles!\n");
     }
     
     if(curchunk == 16656)
     {
      fread(chunk, 1, 2, file3ds);  
       
      verticecount = (chunk[1]<<8) + chunk[0];
      fprintf(log, "\nverticecount: %x\n", verticecount);
      
      vertlist = (vertex*)malloc(verticecount*sizeof(vertex));
      
      for(count = 0; count < verticecount; count++)
      {
        float number;        
        fread(&number, 1, 4, file3ds);
        vertlist[count].x = number;
        fread(&number, 1, 4, file3ds);
        vertlist[count].y = number;
        fread(&number, 1, 4, file3ds); 
        vertlist[count].z = number;  
        
        fprintf(log, "\nvertex%i: (%f, %f, %f)\n", count,vertlist[count].x, vertlist[count].y, vertlist[count].z);      
      
      }
     
      foundverts = 1;
     
      if(foundfaces == 1 && foundverts == 1)
        break;
      
      }
      
     if(curchunk == 16672) 
     {     
     
     fread(chunk, 1, 4, file3ds);
     facecount = (chunk[3]<<24)+(chunk[2]<<16)+(chunk[1]<<8)+chunk[0];
            
      fprintf(log, "\nfacecount: %x\n", facecount);
      
      facelist = (int*)malloc(facecount*sizeof(unsigned int)*3);
      
     for(count = 0; count < facecount; count++)
     {
        fread(chunk, 1, 4, file3ds);
        facelist[(count*3)] = (chunk[3]<<24)+(chunk[2]<<16)+(chunk[1]<<8) + chunk[0];
        fread(chunk, 1, 4, file3ds);
        facelist[(count*3)+1] = (chunk[3]<<24)+(chunk[2]<<16)+(chunk[1]<<8) + chunk[0];
        fread(chunk, 1, 4, file3ds);      
        facelist[(count*3)+2] = (chunk[3]<<24)+(chunk[2]<<16)+(chunk[1]<<8) + chunk[0];
     
     }
           
     foundfaces = 1;
     
     if(foundfaces == 1 && foundverts == 1)
     {
     break;   
     }
     }
     
     if(curchunk != 16656 && curchunk != 15677 && curchunk != 16384 && curchunk != 16640 && curchunk != 16672)
     {
     for(count = 0; count < length - 6; count++)
      fread(chunk, 1, 1, file3ds);
      }
        
     
     
     }      

    outobject = (obj3ds*)malloc(sizeof(obj3ds)); 
    outobject->triangles = (poly3d*)malloc(facecount*sizeof(poly3d));
    outobject->triNumber = facecount;
    for(count = 0; count < facecount; count++)
    {
     outobject->triangles[count].a.x = vertlist[facelist[(count*3)]].x;
     outobject->triangles[count].a.y = vertlist[facelist[(count*3)]].y;
     outobject->triangles[count].a.z = vertlist[facelist[(count*3)]].z;
     outobject->triangles[count].b.x = vertlist[facelist[(count*3)+1]].x;
     outobject->triangles[count].b.y = vertlist[facelist[(count*3)+1]].y;
     outobject->triangles[count].b.z = vertlist[facelist[(count*3)+1]].z;
     outobject->triangles[count].c.x = vertlist[facelist[(count*3)+2]].x;
     outobject->triangles[count].c.y = vertlist[facelist[(count*3)+2]].y;
     outobject->triangles[count].c.z = vertlist[facelist[(count*3)+2]].z;
     fprintf(log, "\nface%i: vertex%i, vertex%i, vertex%i\n", count, facelist[(count*3)], facelist[(count*3)+1], facelist[(count*3)+2]);
    }
    
    free(vertlist);
    free(facelist);
    fclose(file3ds);
    fclose(log);
 
   return outobject;
}

/*
void drawPoly(poly3d triangle)
{
  glBegin(GL_TRIANGLES); 
     glVertex3f(triangle.a.x, triangle.a.y, triangle.a.z);
     glVertex3f(triangle.b.x, triangle.b.y, triangle.b.z);
     glVertex3f(triangle.c.x, triangle.c.y, triangle.c.z);   
  glEnd();
}
*/

void draw3ds(obj3ds* object) {
     
     int i; 
     for(i = 0; i < object->triNumber; i++)
     drawPoly(object->triangles[i]);
}


int jsonObj(obj3ds *object, char *jsonFileName) {

    int i;
    FILE *jsonFile = fopen(jsonFileName, "r");
    
    if(!jsonFile)
        return 0;

    fprintf(jsonFile, "{\"trinumber\":%d,\"triangles\":[", object->triNumber);
    
    for(i = 0; i < object->triNumber; i++) {
    
        if(i)
            fprintf(jsonFile, ",");
        
        fprintf(jsonFile, "{\"a\":{\"x\":%d,\"y\":%d\"z\":%d},", object->triangles[i].a.x, object->triangles[i].a.y, object->triangles[i].a.z);
        fprintf(jsonFile, "\"b\":{\"x\":%d,\"y\":%d\"z\":%d},", object->triangles[i].b.x, object->triangles[i].b.y, object->triangles[i].b.z);
        fprintf(jsonFile, "\"c\":{\"x\":%d,\"y\":%d\"z\":%d}}", object->triangles[i].c.x, object->triangles[i].c.y, object->triangles[i].c.z);
    }
    
    fprintf(jsonFile, "]}");
    fclose(jsonFile);
}
