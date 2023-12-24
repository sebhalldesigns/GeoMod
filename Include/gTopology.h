#ifndef GTOPOLOGY_H
#define GTOPOLOGY_H

// a zero-dimensional point
class gVertex;

// a one-dimensional line/curve bounded by two vertices
class gEdge;

// a set of connected edges that can be either open or closed (a chain or a loop)
class gPath;

// an area bounded by a closed path, can be flat or curved
class gFace;

// a set of connected faces that can either be open or closed (a shell or a case)
class gSurface;

// a volume bounded by a closed surface
class gSolid;


// an flat, infinite surface - sketches must be on planes
class gPlane;


#endif // GTOPOLOGY_H