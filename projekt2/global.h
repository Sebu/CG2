#pragma once

#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__) || defined(__MINGW32__) 
    #include <windows.h>		// damit gl.h sauber vom windows gefressen wird
#endif

#include <GL/glut.h>    
#include <GL/gl.h> 

//NEWMAT
#include <include.h>
#include <newmat.h>
#include <newmatap.h>

#include <math.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>			// fuer STL vector
#include <algorithm>		// fuer STL sort
using namespace std;

#include "gx_math.h"
#include "gx_core.h"
#include "gx_aabb.h"
#include "gx_kdtree.h"
#include "gx_model.h"
