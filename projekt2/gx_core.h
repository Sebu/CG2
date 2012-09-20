#include "global.h" 

GLvoid init ( GLvoid );
GLvoid reshape ( int w, int h );
void renderPlist(VertPointerList& list);
void renderList(VertList& list);
void renderNormals(VertList& list, VertList& normals);
void renderPoint(GX_VEC3 p);
void renderQuads(int m, int n, VertList& l, VertList& nor);
void renderField(int m, int n, VertList& l);
