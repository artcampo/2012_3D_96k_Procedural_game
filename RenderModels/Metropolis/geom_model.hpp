#ifndef GEOM_MODEL
#define GEOM_MODEL

#include		<vector>
#include		<queue>
using namespace std;

#include "Math/Basics.hpp"
 
#include "Math/StaticMatrix.hpp"




class CylinderModel {

private:


public:
	vec<float,3>			p1;			//in world coordinates
	vec<float,3>			p2;

	float					r1;
	float					r2;

	CylinderModel() {};
};


///////////////////////////////////////////////////////////////////////////////
//		Convex hull
///////////////////////////////////////////////////////////////////////////////

bool	IsTriangleCounterClockWise	( vec<float,3> p1, vec<float,3> p2, vec<float,3> p3 );
bool	PointInPositiveHalfPlane	( vec<float,3> x, vec<float,3> p1, vec<float,3> p2, vec<float,3> p3 );
double	StaticMatrix_44_Det			( vec<float,3> p0, vec<float,3> p1, vec<float,3> p2, vec<float,3> p3);
double	StaticMatrix_33_Det			( vec<float,3> p0, vec<float,3> p1, vec<float,3> p2 );
double	Determinant					( double **a,int n );

#ifndef CG_DCEL_EDGE
#define CG_DCEL_EDGE
class DcelEdge {

public:
	int		id;
	int		vb, ve;	
	int		fl, fr;
	int		ep, en;

};
#endif


class EdgeFacetPair{
	public:
		int e;
		int f;
	};
class FacetsPair{
	public:
		int fl;
		int fr;
	};

class ConvexHullInc {
public:
	//	Size = n
	vector <vec<float,3>> Vertexes;
	vector <int>		Edge_of_Vertex;
	int					number_of_vertexes;
	int					max_number_of_vertexes;
	
	//	Maximum size = 3n
	vector <DcelEdge>	Edges;
	vector <bool>		EdgeIsValid;
	int					number_of_edges;
	int					max_number_of_edges;

	//	Maximum size = 2n
	vector <int>		Edge_of_Face;
	vector <bool>		FaceIsValid;
	vector <bool>		FaceColour;
	int					number_of_faces;
	int					max_number_of_faces;

	//	Conflict graph
	vector<queue <int>>			P_conflict;
	vector<queue <int>>			F_conflict;

	////////////////////////////////////////
	//	functions

	//Related to primary data
	int 	AddVertex 	( vec<float,3> p );
	int		AddFace		( int edge );
	int		AddEdge		();

	void	InitEdge	( int e, int vb, int ve, int fl, int fr, int ep, int en );
	void	DelFace		( int f );
	void	DelEdge		( int e );

	void 	Face_ListEdgesAndVertices	( int f, int e [3] , int v [3] );
	void	Vertex_ListEdgesAndFaces	( int v, vector<int> &e , vector<int> &f );

	void			UpdateEdgeWithNewFace	( int e, int old_face, int new_face, int updating_edge );
	vec<float,3>	FaceBarycenter			( int f );
	
	void			CheckIntegrity ( void ); 
	void			Compute_Edge_of_Vertex ();

	//Conflict list functions
	queue<int>		ConflictingFaces			( int v );
	void			ConflictGraphDeleteFaces	( queue<int> f );
	void			ConflictGraphDeleteVertex	( int v );
	void			CheckDataStructuresIntegrity( void );

public:

	ConvexHullInc 	();

	void 	Init 				( int num_vertexes , vec<float,3> points[] );
	void 	AddPoint 			( int v );
	void 	ChangeFaceColours 	( int v );
	
	bool	CheckAllPointsPosition	(void);
	void	PrepareForRender		(void);

	//void 			Vertex_ListFaces			( int v, queue<int> &f );

};


#endif