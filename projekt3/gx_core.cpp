#include "global.h"

void renderQuads(int m, int n, VertList& l, VertList& nor){
    for(int y = 0;y < n - 1; y++)
	{
		glBegin(GL_QUADS);
        for(int x = 0;x < m - 1; x++)
		{
            int i = y*m + x;
 
            glNormal3dv(nor.at(i).v);
            glVertex3dv(l.at(i).v);
			
           	glNormal3dv(nor.at(i+1).v);
            glVertex3dv(l.at(i+1).v);
		    
            glNormal3dv(nor.at(i+1+m).v);
			glVertex3dv(l.at(i+1+m).v);
			
            glNormal3dv(nor.at(i+m).v);
            glVertex3dv(l.at(i+m).v);  
        }
        glEnd();
    }

}

void renderField(int m, int n, VertList& l){
    if (l.empty()) return;
    
    for(int y = 0;y < n - 1; y++)
	{
		glBegin(GL_TRIANGLES);
        for(int x = 0;x < m - 1; x++)
		{
            int i = y*m + x;
           
            GX_VEC3 n1 = (l.at(i+1)-l.at(i)) * (l.at(i+1+m)-l.at(i));
            GX_VEC3 n2 = (l.at(i+1+m)-l.at(i)) * (l.at(i+m)-l.at(i));
            n1.normalize();
            n2.normalize();
            
            glColor3f(l.at(i).fi,l.at(i).fi,l.at(i).fi);
            glNormal3dv( n1.v );
            glVertex3dv(l.at(i).v);
			glVertex3dv(l.at(i+1).v);
			glVertex3dv(l.at(i+1+m).v);
	
            glNormal3dv( n2.v );
            glVertex3dv(l.at(i).v);
        	glVertex3dv(l.at(i+1+m).v);
		    glVertex3dv(l.at(i+m).v); 
             
        }
        glEnd();
    }

}

void renderNormals(VertList& list, VertList& normals)
{
		glBegin(GL_LINES);
				for (int i = 0; i < static_cast<int>(list.size()); i++) {
				glVertex3dv(list.at(i).v);
				glVertex3f(list.at(i).x+normals.at(i).x, 
							list.at(i).y+normals.at(i).y, 
							list.at(i).z+normals.at(i).z);
				}
		glEnd();
}

// Punktliste rendern
void renderPlist(VertPointerList& list)
{
		glBegin(GL_POINTS);
				for (int i = 0; i < static_cast<int>(list.size()); i++) {
				glVertex3f(list.at(i)->x, 
							list.at(i)->y, 
							list.at(i)->z);
				}
		glEnd();
}

void renderPoint(GX_VEC3 p)
{
		glBegin(GL_POINTS);
    
                
				glVertex3f(p.x,	p.y, p.z);
			
		glEnd();
}

void renderList(VertList& list)
{
		glBegin(GL_POINTS);
				for (int i = 0; i < static_cast<int>(list.size()); i++) {
                    glColor3f(list.at(i).fi, list.at(i).fi,list.at(i).fi);
				    glNormal3fv(list.at(i).n);
                    glVertex3dv(list.at(i).v);
				}
		glEnd();

}

void renderValues(VertList& list)
{
		glBegin(GL_POINTS);
				for (int i = 0; i < static_cast<int>(list.size()); i++) {
				    float fi = list.at(i).fi;
				    glNormal3fv(list.at(i).n);
                    if (fi>0.0f) glColor3f(0.0f, fi*.07f+0.1f,0.0f);
                    if (fi<=0.0f) glColor3f(fi*.5f, 1.0f,1.0f);
                    glVertex3dv(list.at(i).v);
				}
		glEnd();
}

void renderTris(VertList& list)
{
		glBegin(GL_TRIANGLES);
				for (int i = 0; i < static_cast<int>(list.size()); i +=3) {
                    //GX_VEC3 n =  (list.at(i+1)-list.at(i)) * (list.at(i+2)-list.at(i));
                    //n.normalize();
                    //glNormal3dv( n.v );
                    glNormal3fv(list.at(i).n);
                    glVertex3dv(list.at(i).v);
                    glNormal3fv(list.at(i+1).n);
					glVertex3dv(list.at(i+1).v);
					glNormal3fv(list.at(i+2).n);
					glVertex3dv(list.at(i+2).v);
                            
				}
		glEnd();
}

void render_ray(Point& a,Vector& v)
{

	float x2 = a.x + v.x*1000.0f;
	float y2 = a.y + v.y*1000.0f;
	float z2 = a.z + v.z*1000.0f;

	glLineWidth(1.0f);
	glBegin(GL_LINES);
		glColor3f( 1.0f, 1.0f, 1.0f);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(x2, y2, z2);
	glEnd();
}

// nehe.gamedev.net basic code GLInit
GLvoid init ( GLvoid )   
{
 
	glShadeModel(GL_SMOOTH);							
	glClearDepth(1.0f);				
	glEnable(GL_DEPTH_TEST);				
	glDepthFunc(GL_LEQUAL);				
	glEnable ( GL_COLOR_MATERIAL );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(.4f, 0.4f, .4f, 0.0f);
	glCullFace(GL_FRONT);
	//glEnable(GL_CULL_FACE);
	
}




// nehe.gamedev.net basic code resphape func
GLvoid reshape ( int w, int h )  
{
	glViewport     ( 0, 0, w, h );
	glMatrixMode   ( GL_PROJECTION );  
	glLoadIdentity ( );               
	if ( h==0 )  
		gluPerspective ( 90, ( float ) w, 0.1f, 5000.0f );
	else
		gluPerspective ( 90, ( float ) w / ( float ) h, 0.1f, 5000.0f );
	glMatrixMode   ( GL_MODELVIEW );
	glLoadIdentity ( );  

}

void writeScreen(char *filename, int ScrWidth, int ScrHeight, unsigned char *image)
{
    FILE *File;
    BITMAPFILEHEADER bitmapFileHeader;
    BITMAPINFOHEADER bitmapInfoHeader;
    int    imageIdx;
    unsigned char tempRGB;
    //unsigned char *image;

    //image = (unsigned char*)malloc(ScrWidth*ScrHeight*3);
    //memset(image, 0, ScrWidth*ScrHeight*3);

    //glReadPixels(0, 0, (ScrWidth-1), (ScrHeight-1), GL_RGB, GL_UNSIGNED_BYTE, image);

    File = fopen(filename, "wb");
    if (!File)
    { 
        return; 
    }

    bitmapFileHeader.bfSize = sizeof(BITMAPFILEHEADER);
    bitmapFileHeader.bfType = 0x4D42;
    bitmapFileHeader.bfReserved1 = 0;
    bitmapFileHeader.bfReserved2 = 0;
    bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfoHeader.biPlanes = 1;
    bitmapInfoHeader.biBitCount = 24;
    bitmapInfoHeader.biCompression = BI_RGB;
    bitmapInfoHeader.biSizeImage = ScrWidth * abs(ScrHeight) * 3;
    bitmapInfoHeader.biXPelsPerMeter = 0;
    bitmapInfoHeader.biYPelsPerMeter = 0;
    bitmapInfoHeader.biClrUsed = 0;
    bitmapInfoHeader.biClrImportant = 0;
    bitmapInfoHeader.biWidth = ScrWidth;
    bitmapInfoHeader.biHeight = ScrHeight;

    /*
    for (imageIdx = 0; imageIdx < (int)bitmapInfoHeader.biSizeImage; imageIdx+=3)
    {
        tempRGB = image[imageIdx];
        image[imageIdx] = image[imageIdx + 2];
        image[imageIdx + 2] = tempRGB;
    }
    */
    fwrite(&bitmapFileHeader, 1, sizeof(BITMAPFILEHEADER), File);
    fwrite(&bitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), File);
    fwrite(image, 1, bitmapInfoHeader.biSizeImage, File);

    fclose(File);
    free(image);

}

void save_tris(const char* filename, VertList t)
{
    fstream fs;
    
    VertList v;
    std::vector<GX_FACE> f;
    
    reduceData(t,v,f);
    
    int nverts = v.size();
    int nfaces = f.size();
    
    
	cout << "SCHREIBE MODELDATEN ...";

	fs.open(filename, fstream::out);

	fs << "NOFF" << endl;
	fs << nverts << " " << nfaces << " " << "0" << endl;


	for(long i = 0; i< nverts; i++)
	{
		fs << v[i].x << " " << v[i].y << " " << v[i].z << " " << v[i].nx << " " << v[i].ny << " " << v[i].nz << endl;
		
	}

	for(long i = 0; i< nfaces; i++)
	{
		fs << 3 << " " << f[i].v1 << " " << f[i].v2 << " " << f[i].v3 << endl;
		
	}

	fs.close();

    cout << "OK" << endl;
}
