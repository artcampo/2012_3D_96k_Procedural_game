#ifndef DX_METARM_MODEL
#define DX_METARM_MODEL


#include		<queue>
#include		<vector>
#include    "dx_misc.hpp"
#include    "dx_model_basic.hpp"

#define DP_DEBUG_ON_LINES

/////////////////////////////////////////////////////////////////////////////////
//		PRIVATE DATA


//SIZE OF FINAL CUBES

#define DP_METROPOLIS_ARM_GROUPS	int(4)


/////////////////////////////////////////////////////////
//  These that follow are for LINEAR ARMS

//khan pieces
#define DP_MET_CUB_RAND_X_VALUES	int (4)
const float	cube_size_x	[4]	=	{ 5, 5, 5, 5 };
const float	cube_size_z	[4]	=	{ 5, 5, 5, 5 };
//const int		cube_size_x__num = 4;

const float	cube_size_y_min			= 5.0f;
const float	cube_size_y_rand		= 1.0f;

//parameter of the arm itself
const float	cube_separation_from_center_y = 4.5f;

//Final separation = cube_separation_min + [0,1]*cube_separation_rand
//On axis X, of tangential frame basis to (p1,p2)
//first correspond to cubes that have a small x size

const float	cube_separation_x_min [2]		= { 0.03f, 0.02f };
const float	cube_separation_x_rand[2]		= { 0.02f, 0.01f };
//cuidado, es para los cubos pequeños, no pasarse
const float	cube_separation_z_min [1]		= { 0.03f };
const float	cube_separation_z_rand[1]		= { 0.01f };

/////////////////////////////////////////////////////////
//  These that follow are for CURVED ARMS

const float	arch_cube_size_x	= 20.0f;
const float	arch_cube_size_x_separation	= 0.45f;

// arch_cube_size_z_min>=1
const float arch_cube_size_z_min  = 10.5f;
const float arch_cube_size_z_rand = 2.75f;

// arch_cube_size_y_min>=1
const float arch_cube_size_y_min  = 10.5f;
const float arch_cube_size_y_rand = 2.75f;


/////////////////////////////////////////////////////////////////////////////////
//	Any one arm, composed of many cubes. Might be liner or curved.

class MetArmModel {

public:

  DxCubeModelPool         *dx_cube_pool;
	MetArmModel				() {};

	MetArmModel ( D3DXVECTOR3 point1, D3DXVECTOR3 point2);
	MetArmModel ( D3DXVECTOR3 point1, D3DXVECTOR3 point2, D3DXVECTOR3 _c1, D3DXVECTOR3 _c2, D3DXVECTOR3 _binormal );

  int   getNumberCubes() {return number_cubes;};
	
	void	display	();
private:
	D3DXVECTOR3		p1, p2;

	bool			curved;
  D3DXVECTOR3		c1, c2;   //extra points to define curve
  D3DXVECTOR3    binormal; //Binormal for TNB matrix  

	int						number_cubes;

  //  Cubes for linear arm
  std::vector <CubeModel>		cubes;
	std::vector <DxCubeModel>	dx_cubes;

  //  Cubes for curved arm
  std::vector <DxArchCubeModel>	dx_archCubes;
  DxArchCubeModelPool*      dx_archCubesPool;
  
  // shade me (better yet learn design patterns)
  

	//Private functions
	//void linear_arm_face ( vec TBN_matrix[3] );
  void buildCurvedArm();
  void dxRemoveZeroes ( D3DXVECTOR3* v );
  void dxCatmullFrameBasis( 
           D3DXMATRIX *m , 
           const D3DXVECTOR3* c0,
           const D3DXVECTOR3* c1,
           const D3DXVECTOR3* c2,
           const D3DXVECTOR3* c3,
           float t,
           int side,
           const D3DXVECTOR3* _binormal);

  void	met_build_model		();

};

void CheckNegativeZeroes( D3DXVECTOR3 v );


/////////////////////////////////////////////////////////////////////////////////
//	Used while building arm
class arm_cube_lengths{
	public:
		float			x_length;
		float			y_length;
		float			z_length;
		D3DXVECTOR3	center;			//WCORD
		D3DXVECTOR3	displacement;	//OCORD
};


#endif