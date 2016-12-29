#include "SphereModel.hpp"

using namespace std;

SphereModel::SphereModel( int numberVerticesPerEdge)
{
  mModel = new DxMeshModelBatch<VertexPos>( sizeof(VertexPos) , D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  
  std::vector<VertexPos> points = sphere(numberVerticesPerEdge);

  DxMeshModel<VertexPos> *model  = new DxMeshModel<VertexPos>( sizeof(VertexPos), points.size(), &points[0] );
  mModel->addMeshModel( model );
  mModel->createVB();

}

SphereModel::~SphereModel()
{
}

void SphereModel::display()
{
  mModel->DisplaySansShader();
}


std::vector<VertexPos> SphereModel::sphere(int size) {

    int vertex_face             = size*size;
    int num_vertex              = 6*vertex_face;
    int triangles_face          = 2*(size-1)*(size-1);
    int vertex_triangles_cube   = 3*6*triangles_face;
    
    vector<VertexPos>              ret( num_vertex );
    vector<VertexPos>              ver( vertex_triangles_cube );
    
    for (int i=0; i<vertex_face; i++) {
        float u = i/size;     // 0 <= u < size
        float v = i-size*u;   // 0 <= v < size
        float w = 1;
        
        u = 1 - u*2/float(size-1); // 0 <= u <= 1
        v = 1 - v*2/float(size-1); // 0 <= v <= 1
        
        float module = sqrt(u*u+v*v+1);

        float factor = module*module*module;

        float t1_u = (u*u-module*module)/factor;
        float t1_v = (v*u)/factor;
        float t1_w = u/factor;
        
        float t2_u = (u*v)/factor;
        float t2_v = (v*v-module*module)/factor;
        float t2_w = v/factor;
        //Analitally checked: n = t1 x t2 = (u, v, w)/module^2

        u /= module;
        v /= module;
        w /= module;

        //For each side
        //x=0
        ret[i].pos.x                  = -w; 
        ret[i].pos.y                  =  u;
        ret[i].pos.z                  =  v;
/*
        ret[i].nor.x                  = -w; 
        ret[i].nor.y                  =  u;
        ret[i].nor.z                  =  v;*/
        //If you make ret[i].pos = ret[i].nor you can have a cool image.

        //x=1
        ret[i+vertex_face].pos.x      =  w; 
        ret[i+vertex_face].pos.y      =  u;
        ret[i+vertex_face].pos.z      =  v;
   /*
        ret[i+vertex_face].nor.x      =  w; 
        ret[i+vertex_face].nor.y      =  u;
        ret[i+vertex_face].nor.z      =  v;*/

        
        //y=0
        ret[i+2*vertex_face].pos.x    =  u; 
        ret[i+2*vertex_face].pos.y    = -w;
        ret[i+2*vertex_face].pos.z    =  v;
  /*
        ret[i+2*vertex_face].nor.x    =  u; 
        ret[i+2*vertex_face].nor.y    = -w;
        ret[i+2*vertex_face].nor.z    =  v;*/


        //y=1
        ret[i+3*vertex_face].pos.x    =  u; 
        ret[i+3*vertex_face].pos.y    =  w;
        ret[i+3*vertex_face].pos.z    =  v;
/*
        ret[i+3*vertex_face].nor.x    =  u; 
        ret[i+3*vertex_face].nor.y    =  w;
        ret[i+3*vertex_face].nor.z    =  v;*/


        //z=0
        ret[i+4*vertex_face].pos.x    =  u; 
        ret[i+4*vertex_face].pos.y    =  v;
        ret[i+4*vertex_face].pos.z    = -w;
  /*
        ret[i+4*vertex_face].nor.x    =  u; 
        ret[i+4*vertex_face].nor.y    =  v;
        ret[i+4*vertex_face].nor.z    = -w;*/


        //z=1
        ret[i+5*vertex_face].pos.x    =  u; 
        ret[i+5*vertex_face].pos.y    =  v;
        ret[i+5*vertex_face].pos.z    =  w;
/*
        ret[i+5*vertex_face].nor.x    =  u; 
        ret[i+5*vertex_face].nor.y    =  v;
        ret[i+5*vertex_face].nor.z    =  w;
*/

    }
    
    int l = 0;
    for (int k=0; k<6; k++) {
        int start = k*size*size;
        for (int i=0; i<size-1; i++) {
            for (int j=0; j<size-1; j++) {
                if(k == 3 || k == 4 || k == 0) {
                    ver[l]      = ret[  start + i + j*size      ];
                    ver[l+1]    = ret[  start + i + j*size + 1  ];
                    ver[l+2]    = ret[  start + i + (j+1)*size  ];
                    
                    ver[l+5]    = ver[l+1];
                    ver[l+4]    = ver[l+2];
                    ver[l+3]    = ret[  start + i + (j+1)*size + 1  ];
                } else {
                    ver[l+2]  = ret[  start + i + j*size      ];
                    ver[l+1]  = ret[  start + i + j*size + 1  ];
                    ver[l]    = ret[  start + i + (j+1)*size  ];
                    
                    ver[l+3]    = ret[  start + i + j*size + 1  ];
                    ver[l+4]    = ret[  start + i + (j+1)*size  ];
                    ver[l+5]    = ret[  start + i + (j+1)*size + 1  ];
                }
                l += 6;
            }
        }
    }

    return ver;
}