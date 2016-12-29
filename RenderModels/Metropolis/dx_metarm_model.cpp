#include  "dx_metarm_model.hpp"
#include  "Geometry/DxMathHelpers.hpp"
using namespace std;
#include  "Global.hpp"
#include  "Noise/RandomNumberGenerator.hpp"

MetArmModel::MetArmModel ( D3DXVECTOR3 point1, D3DXVECTOR3 point2 )
{
	p1		= point1;
	p2		= point2;
	curved	= false;
  met_build_model();
}

MetArmModel::MetArmModel( D3DXVECTOR3 point1, D3DXVECTOR3 point2, D3DXVECTOR3 _c1, D3DXVECTOR3 _c2, D3DXVECTOR3 _binormal )
{
	p1		    = point1;
	p2		    = point2;
  c1        = _c1;
  c2        = _c2;
  binormal  = _binormal;
	curved	  = true;
  met_build_model();
}

///////////////////////////////////////////////////////////////////////////////
//		MetArmModel::met_build_model
///////////////////////////////////////////////////////////////////////////////

void	MetArmModel::met_build_model		()
{

if (!curved)
{
	D3DXVECTOR3 arm			= p2 - p1;
  D3DXVECTOR3 arm_director;
  D3DXVec3Normalize( &arm_director, &arm);

	//init random based on cube position
  float length = D3DXVec3Length( &arm );
	

	//Choose cube lengths for the 4 sides


	//For each side, store its final lengths
	//equals total length / minimum cube x-length
	//int	max_pieces = int ( length / cube_size_x[0] );
		
	
	vector<queue<arm_cube_lengths>> p(4);
	number_cubes = 0;			//number of final cubes that will compose arm

  Rand::RandGen r(20);
  

	for ( int side = 0; side < 4 ; ++side )
	{
		D3DXVECTOR3 pos = p1;

		float		covered_length = 0.0f;		//space already taken by cubes in x direction
		//cube_size_x[0] == minimum cube size in x direction
		//WARNING: es adecuado? o pondremos mas lejos de lo debido?
		while ( (length - covered_length) > cube_size_x[0] )
		{
			//Choose random group
			//int		r_index		= (rand() % DP_METROPOLIS_ARM_GROUPS);
			int		r_index		= (rand() % 3);
			float	x_length;

			r_index = 2;
			switch ( r_index )
			{
			case 0:
				//GROUP 1: one cube of length 0.5 or 1.0
				r_index		= (rand() % 2);
				x_length	= cube_size_x[r_index];

				if ( (covered_length + x_length) <= length )
				{
				
					//Assign rest of x-length to the last of the cubes
					if ((length - ( covered_length + x_length)) <= cube_size_x[0])
						x_length += (length - ( covered_length + x_length));

					//Add this cube
					arm_cube_lengths a;
					a.x_length		= x_length;
					a.x_length		-= ( cube_separation_x_min[1] + cube_separation_x_rand[1]*r.randFloat_0_1() );
					a.z_length		= cube_size_z[1];
					a.y_length		= cube_size_y_min		+ cube_size_y_rand*r.randFloat_0_1();
					
					a.center		= pos + (arm_director * ((x_length * 0.5f) ));
					a.displacement	= D3DXVECTOR3 ( 0.0f, (a.y_length*0.5f) + cube_separation_from_center_y, 0.0f );

					//Store new cube into queue
					p[side].push(a);
					a = p[side].front();
					++number_cubes;

				}
				break;
			case 1:
				//GROUP 2a: two cubes of big length
				x_length	= cube_size_x[1];

				if ( (covered_length + x_length) <= length )
				{
					//Assign rest of x-length to the last of the cubes
					if ((length - ( covered_length + x_length)) <= cube_size_x[0])
						x_length += (length - ( covered_length + x_length));

					//Add this cube
					arm_cube_lengths a;
					a.x_length		= x_length;
					a.x_length		-= ( cube_separation_x_min[1] + cube_separation_x_rand[1]*r.randFloat_0_1() );

					//Make first cube
					a.z_length		= cube_size_z[0];
					a.z_length		-= ( cube_separation_z_min[0] + cube_separation_z_rand[0]*r.randFloat_0_1() );
					a.y_length		= cube_size_y_min		+ cube_size_y_rand*r.randFloat_0_1();
					
					a.center		= pos + (arm_director * (x_length*0.5f ));
					a.displacement	= D3DXVECTOR3 ( 0.0f, (a.y_length*0.5f) + cube_separation_from_center_y, cube_size_z[0]*0.5f );

					//Store new cube into queue
					p[side].push(a);
					a = p[side].front();
					++number_cubes;

					//Make second cube
					a.x_length		-= ( cube_separation_x_min[1] + cube_separation_x_rand[1]*r.randFloat_0_1() );
					a.z_length		= cube_size_z[0];
					a.z_length		-= ( cube_separation_z_min[0] + cube_separation_z_rand[0]*r.randFloat_0_1() );
					a.y_length		= cube_size_y_min		+ cube_size_y_rand*r.randFloat_0_1();
					a.center		= pos + (arm_director * (x_length*0.5f) );
					a.displacement	= D3DXVECTOR3 ( 0.0f, (a.y_length*0.5f) + cube_separation_from_center_y, (cube_size_z[0]*(-0.5f)) );

					//Store new cube into queue
					p[side].push(a);
					a = p[side].front();
					++number_cubes;

					//Store new cube into queue
					p[side].push(a);
					a = p[side].front();
					++number_cubes;
				}
				break;

			case 2:
				//GROUP 2b: one of length 1.0 (left), two of length 0.5 (right)
				x_length	= cube_size_x[1];

				if ( (covered_length + x_length) <= length )
				{
					
					//Assign rest of x-length to the last of the cubes
					if ((length - ( covered_length + x_length)) < cube_size_x[0])
						x_length += (length - ( covered_length + x_length));

					//Add this cube
					arm_cube_lengths a;
					a.x_length		= x_length;
					
					//Make large cube
					a.x_length		-= ( cube_separation_x_min[1] + cube_separation_x_rand[1]*r.randFloat_0_1() );
					a.z_length		= cube_size_z[0];
					a.z_length		-= ( cube_separation_z_min[0] + cube_separation_z_rand[0]*r.randFloat_0_1() );
					a.y_length		= cube_size_y_min		+ cube_size_y_rand*r.randFloat_0_1();			
					a.center		= pos + (arm_director * (x_length*0.5f));
					a.displacement	= D3DXVECTOR3 ( 0.0f, (a.y_length*0.5f) + cube_separation_from_center_y, cube_size_z[0]*0.5f );
					//Store new cube into queue
					p[side].push(a);
					a = p[side].front();
					++number_cubes;

					//Make small cube 1
					a.x_length		= x_length * 0.5f ;
					a.x_length		-= ( cube_separation_x_min[0] + cube_separation_x_rand[0]*r.randFloat_0_1() );
					a.z_length		= cube_size_z[0];
					a.z_length		-= ( cube_separation_z_min[0] + cube_separation_z_rand[0]*r.randFloat_0_1() );
					a.y_length		= cube_size_y_min		+ cube_size_y_rand*r.randFloat_0_1();
					a.center		= pos + (arm_director * (x_length * 0.5f + x_length * 0.25f) );
					a.displacement	= D3DXVECTOR3 ( 0.0f, (a.y_length*0.5f) + cube_separation_from_center_y, (cube_size_z[0]*(-0.5f)) );
					//Store new cube into queue
					p[side].push(a);
					a = p[side].front();
					++number_cubes;

					//Make small cube 2
					a.x_length		= x_length * 0.5f ;
					a.x_length		-= ( cube_separation_x_min[0] + cube_separation_x_rand[0]*r.randFloat_0_1() );
					a.z_length		= cube_size_z[0];
					a.z_length		-= ( cube_separation_z_min[0] + cube_separation_z_rand[0]*r.randFloat_0_1() );
					a.y_length		= cube_size_y_min		+ cube_size_y_rand*r.randFloat_0_1();
					a.center		= pos + (arm_director * (x_length * 0.5f - x_length * 0.25f) );
					a.displacement	= D3DXVECTOR3 ( 0.0f, (a.y_length*0.5f) + cube_separation_from_center_y, (cube_size_z[0]*(-0.5f)) );
					//Store new cube into queue
					p[side].push(a);
					a = p[side].front();
					++number_cubes;

					//Store new cube into queue
					p[side].push(a);
					a = p[side].front();
					++number_cubes;

				}
				break;

			}
			//Update position
			covered_length	+= x_length;
			pos				+= (arm_director * x_length);

		}

	}



	//Allocate all cubes
	++number_cubes;				//cube of the interior
	cubes		= vector <CubeModel>(number_cubes);
	dx_cubes	= vector <DxCubeModel>(number_cubes);
	int cube_i	= 0;		//cube to create




	//Construct cubes
	//Make TBN matrix
  std::vector<D3DXVECTOR3> TBNvec (3);

	//WARNING: failed at exactly (0,0,1) direction!
	//because of unproper perpendicular vector! (-y,x,z)
  D3DXVECTOR3 armNormalized;
  D3DXVec3Normalize( &armNormalized, &arm );

	if ( arm.x == 1.0f && arm.y == 0.0f && arm.z == 0.0f )
	{
		TBNvec[0] = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
		TBNvec[1] = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		TBNvec[2] = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	}
	else
	{
		TBNvec[0] = armNormalized;
    TBNvec[1] = DxMath::alignToTangents( D3DXVECTOR3( 1.0f, 0.0f, 0.0f ), armNormalized, D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
		TBNvec[2] = DxMath::alignToTangents( D3DXVECTOR3( 1.0f, 0.0f, 0.0f ), armNormalized, D3DXVECTOR3( 0.0f, 0.0f, 1.0f ) );
	}


  D3DXMATRIX FrameBasisMatrix = DxMath::basisMatrix( TBNvec );

	//Put first cube on the interior of the arm
	cubes[0].center		= p1 + ( arm_director * (D3DXVec3Length ( &arm )*0.5f) );
	cubes[0].frameBasis	= FrameBasisMatrix;
	cubes[0].xsize		= D3DXVec3Length ( &arm );
	cubes[0].ysize		= cube_separation_from_center_y * 2.0f;
	cubes[0].zsize		= cube_separation_from_center_y * 2.0f;
	cube_i = 1;

	for ( int side = 0; side < 4 ; ++side )
	{
		D3DXVECTOR3 displacement;

		//TODO: StaticMatrix<float,3,3> TBN( TBNvec[0], TBNvec[1], TBNvec[2] );		

		arm_cube_lengths a;
		while ( !p[side].empty() )
		{
			
			a = p[side].front();
			p[side].pop();

			//Compute cube displacement in object coordinates
			
			switch(side){
				case 0: 
					displacement = D3DXVECTOR3 ( 0.0f, a.displacement.y, a.displacement.z );
					cubes[cube_i].ysize		= a.y_length;
					cubes[cube_i].zsize		= a.z_length;
					break;
				case 1: 
					displacement = D3DXVECTOR3 ( 0.0f, -a.displacement.y, -a.displacement.z );
					cubes[cube_i].ysize		= a.y_length;
					cubes[cube_i].zsize		= a.z_length;
					break;
				case 2: 
					displacement = D3DXVECTOR3 ( 0.0f, a.displacement.z, -a.displacement.y );
					cubes[cube_i].ysize		= a.z_length;
					cubes[cube_i].zsize		= a.y_length;
					break;

				case 3: 
					displacement = D3DXVECTOR3 ( 0.0f, -a.displacement.z, a.displacement.y );
					cubes[cube_i].ysize		= a.z_length;
					cubes[cube_i].zsize		= a.y_length;
					break;
				}
				

			//Compute cube center
			cubes[cube_i].center	= a.center;
			cubes[cube_i].frameBasis= FrameBasisMatrix;
			cubes[cube_i].xsize		= a.x_length;


			D3DXMATRIX Transposed;
      D3DXMatrixInverse( &Transposed, NULL, &FrameBasisMatrix );    
      D3DXVECTOR3 r;
      D3DXVec3TransformCoord( &r, &displacement, &Transposed );
			cubes[cube_i].center				+= r;

			++cube_i;
		}
	}

	//Pasar todos los cubos de CubeModel a DxCubeModel
	
  dx_cube_pool = new DxCubeModelPool( );
	for ( int i = 0; i < number_cubes ; ++i )
		{
			dx_cubes[i].ConvertToDx ( cubes[i] );
      dx_cube_pool->addCubeModel ( &dx_cubes[i] );
		}
  dx_cube_pool->createVB();
//End of build arm, linear 
}
if (curved)
{
  buildCurvedArm();


}

//End of build arm, linear 
}

void MetArmModel::buildCurvedArm()
{
  D3DXVECTOR3 _p1 = D3DXVECTOR3( p1.x, p1.y, p1.z ) ;
  D3DXVECTOR3 _p2 = D3DXVECTOR3( p2.x, p2.y, p2.z ) ;

  D3DXVECTOR3 _c1 = D3DXVECTOR3( c1.x, c1.y, c1.z ) ;
  D3DXVECTOR3 _c2 = D3DXVECTOR3( c2.x, c2.y, c2.z ) ;


  //for ( int side = 2; side < 3; ++side )
  //for ( int side = 3; side < 4; ++side )
  //for ( int side = 2; side < 4; ++side )
  for ( int side = 0; side < 4; ++side )
  {
    bool  finished = false;
    float totalLength = 0.0f;
    float catmullTa = 0.01f;
    float catmullTb = 0.0f;
    float catmullTIncrement = 0.01f;

    int cubos_hechos = 0;

    while (! finished)
    {
      D3DXVECTOR3 pointA, pointB;     // points in the curve
      D3DXVec3CatmullRom( &pointA , &_c1, &_p1, &_p2, &_c2, catmullTa );
      float actualLength = 0.0f;
      while ( actualLength < arch_cube_size_x && !finished )
      {
        catmullTb += catmullTIncrement;
        if (catmullTb > 1.0f) 
        {
          catmullTb = 1.0f;
          finished  = true;
        }
        D3DXVec3CatmullRom( &pointB , &_c1, &_p1, &_p2, &_c2, catmullTb );

        actualLength = D3DXVec3Length ( &( pointA - pointB ) );
      }

      //  Now given starting position and ending position build ArchCube
      D3DXMATRIX paBasis, pbBasis;
      dxCatmullFrameBasis( &paBasis, &_c1, &_p1, &_p2, &_c2, catmullTa, side, (const D3DXVECTOR3 *)&binormal);
      dxCatmullFrameBasis( &pbBasis, &_c1, &_p1, &_p2, &_c2, catmullTb, side, (const D3DXVECTOR3 *)&binormal );

      float coordY, CoordZ;   //local coordinates
      coordY = arch_cube_size_y_min + arch_cube_size_y_rand*0;            
      CoordZ = arch_cube_size_z_min + arch_cube_size_z_rand*0;
      if (side == 2 || side ==3 )
      {
        float temp  = coordY;
        coordY      = CoordZ;
        CoordZ      = temp;

      }

      DxArchCubeModel archCube(
        &pointA,
        &pointB,
        &paBasis,
        &pbBasis,
        coordY,
        CoordZ,
        side
        );

      //if ( (cubos_hechos% 8) == 1 )
      dx_archCubes.push_back ( archCube );

      //  Advance on the curve
      catmullTa = catmullTb;

      //Leave some separation
      float catmullTc = catmullTa;
      float catmullTIncrementSeparation = 0.001f;
      D3DXVECTOR3 pointC;
      
      float separationLength = 0.0f;
      float actualDesiredSeparation = arch_cube_size_x_separation * 0;
      while ( separationLength < actualDesiredSeparation && !finished  )
      {
        catmullTc += catmullTIncrementSeparation;
        if ( catmullTc >= 1.0f )
          finished = true;

        D3DXVec3CatmullRom( &pointC , &_c1, &_p1, &_p2, &_c2, catmullTc );

        separationLength = D3DXVec3Length ( &( pointA - pointC ) );
      }
      catmullTa = catmullTc;

      ++cubos_hechos;
      /*
      if (cubos_hechos == 1)
        finished = true;*/
    }
    //this side is finished
  }


  dx_archCubesPool = new DxArchCubeModelPool(  );
  for ( unsigned int i = 0 ; i < dx_archCubes.size() ; ++i )
    dx_archCubesPool->addArchCubeModel( &dx_archCubes[i] );

  dx_archCubesPool->createVB();
}

void MetArmModel::dxRemoveZeroes ( D3DXVECTOR3* v )
{
  if( v->x == (-0.0f) ) v->x = 0.0f;
  if( v->y == (-0.0f) ) v->y = 0.0f;
  if( v->z == (-0.0f) ) v->z = 0.0f;
}

// TODO: t=0 => tangent = (0,0,0) !!!
void MetArmModel::dxCatmullFrameBasis( 
           D3DXMATRIX *m , 
           const D3DXVECTOR3* c0,
           const D3DXVECTOR3* c1,
           const D3DXVECTOR3* c2,
           const D3DXVECTOR3* c3,
           float t,
           int side,
           const D3DXVECTOR3* _binormal)
{
  D3DXVECTOR3 tangent;
  D3DXVECTOR3 normal;
  D3DXVECTOR3 binormal = *_binormal;

  D3DXVECTOR3 _c0 = *c0;
  D3DXVECTOR3 _c1 = *c1;
  D3DXVECTOR3 _c2 = *c2;
  D3DXVECTOR3 _c3 = *c3;

  float a0 = -3.0f*t*t +  4.0f*t - 1.0f;
  float a1 =  9.0f*t*t - 10.0f*t;
  float a2 = -9.0f*t*t +  8.0f*t + 1.0f;
  float a3 =  3.0f*t*t -  2.0f*t;

  tangent = 0.5f * ( a0*_c0 + a1*_c1 + a2*_c2 + a3*_c3 );
  D3DXVec3Normalize ( &tangent, &tangent );


  
  D3DXVec3Cross ( &normal, &tangent, &binormal );

  
  D3DXVec3Normalize ( &normal, &normal );
  D3DXVec3Normalize ( &binormal, &binormal );
  
  dxRemoveZeroes ( &normal );
  dxRemoveZeroes ( &binormal );
  
  if (side == 0)
    *m = D3DXMATRIX(
      tangent.x,    tangent.y,    tangent.z,    0.0f, 
      normal.x,     normal.y,     normal.z,     0.0f, 
      binormal.x,  binormal.y,  binormal.z,  0.0f, 
      0.0f,         0.0f,         0.0f,         1.0f
      );

  if (side == 1)
    *m = D3DXMATRIX(
      tangent.x,    tangent.y,    tangent.z,    0.0f, 
      -normal.x,    -normal.y,    -normal.z,    0.0f, 
      binormal.x,  binormal.y,  binormal.z,  0.0f, 
      0.0f,         0.0f,         0.0f,         1.0f
      );

  if (side == 2)
    *m = D3DXMATRIX(
      tangent.x,    tangent.y,    tangent.z,    0.0f, 
      normal.x,     normal.y,     normal.z,     0.0f,       
      binormal.x,   binormal.y,   binormal.z,   0.0f, 
      0.0f,         0.0f,         0.0f,         1.0f
      );

  if (side == 3)
    *m = D3DXMATRIX(
      tangent.x,    tangent.y,    tangent.z,    0.0f, 
      normal.x,     normal.y,     normal.z,     0.0f,       
      -binormal.x,   -binormal.y,   -binormal.z,   0.0f,       
      0.0f,         0.0f,         0.0f,         1.0f
      );

}

///////////////////////////////////////////////////////////////////////////////
//		MetArmModel::display
///////////////////////////////////////////////////////////////////////////////


void	MetArmModel::display	()
{
	//Render linear arm
	if( ! curved )
	{
    dx_cube_pool->Display();
	}

	//Render curved arm
	if( curved )
	{
    dx_archCubesPool->Display();
	}

}



