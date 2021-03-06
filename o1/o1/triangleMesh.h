#pragma once

#include <vector>

class triangleMesh
{
public:
	triangleMesh(void);
	triangleMesh(long,long);
	~triangleMesh(void);

	long nface;
	long npoint;
	double **point;
	long **face;
	double *pointValue;
	double *faceValue;

	double maxPointValue;
	double minPointValue;
	double maxFaceValue;
	double minFaceValue;
	bool loadPoint(double ** pt, long np);
	bool loadFace(long ** fc, long nf);
	bool loadPointV(double * ptv, long np);
	bool loadFaceV(double * fcv, long nf);
//	bool findLine(long * triangle, double v, double * p1, double * p2);
	// sort triangle vertex in face matrix according to point value
	void sortFaceVertex(void);
	long **faceNext;
	void getFaceNext(void);
	long isBelongToFace(long * edge, long * face, long nv);

	double rsl(double x1, double x2, double y1, double y2, double x);
//	double ** findOneContour(double v, bool *faceOfInterest, long * np);
//	long * faceAtVertex(long pt, long *nfav);
	//edge[][1..2] is two vertex
	long **edge;
	// generate edge list from faceNext matrix
	void genEdge(int isvolume);
	long nedge;
	// nface*3 matrix from face to edge
	long **faceEdge;
 //edge[][1..2] is two triangle
	long **edgeFace;
	// next edge list from edgei
//	long * nextEdgeFromEdge(long edgei, long *nne);

//	long * nextEdgeFromPoint(long, long pointi, long *nne);

	//std::vector<double> contourpx;
	//std::vector<double> contourpy;
	//std::vector<double> contourpz;
//	double * getNextPoint(long * edgelist, long nel, double v, long * edgeBelong);
//	void getPointFromOneEdge(long edgei, double v, double * p1, long *p1idx, double * p2, long *p2idx);
//	long * edgeLoopToPointLoop(long * edgelist, long nel);

	std::vector< std::vector<long> > point2face;
	void genPointToFaceMap(void);
	void findContourInOneTriangle(long triangleIndex, double v, double * pin, long * pinLocate, double * pout, long * poutLocate);
	void findContour(double v);

	std::vector< std::vector< std::vector<double> > > contourv;
	std::vector< std::vector< std::vector<long> > > contourv1;
	long isBelongToTriangle(long * edget, long * facet);

	std::vector<double> contourValue;
	void genContourMap(long contourNumber);


	double **faceCentroid;
	void genFaceCentroid(void);
	// point value interpolate from face value
	double *pointValueInterp;
	// interpolate point value 
	void interpPointV(void);
	// dimension
	long dim;
	double maxPointValueInterp;
	double minPointValueInterp;
	void genContourMapInterp(long contourNumber);
	void loadPointAndFace(double ** pt, long np, long ** fc, long nf, int isvolume);
};

