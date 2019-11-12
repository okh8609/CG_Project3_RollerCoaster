/************************************************************************
     File:        Point3f.H

     Author:     
                  Michael Gleicher, gleicher@cs.wisc.edu

     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu
     
     Comment:     
						simple 3D point class

						Note: for the longest time, we avoided giving out 
						basic data
						structure classes. Everyone likes to make their 
						own, and everyone
						makes different choices in how to use them.

						This one is really basic. It is evolved from the 
						old "Pnt3"
						class that Mike wrote in grad school (circa 1993). 
						It is hard coded for floats (since we didn't want 
						to get started
						with templates). 

						We've only put in a few functions. You can add more 
						if you want.

     Platform:    Visio Studio.Net 2003/2005

*************************************************************************/
#pragma once


class Point3f {
	public:

		// Constructor
		// if we have 1, we need the default 
		Point3f();			
		// say where
		explicit Point3f(const float x,const float y,const float z);	
		// from an array 
		Point3f(const float*);			
		// copy constructor created by default
		// Pnt3f(const Pnt3f&);		

	public:
		// if you want to treat this thing as a C vector (point to float)
		float* v();

		// some useful operators - not the complete set, but just some basic ones
		Point3f operator * (const Point3f&) const;	/* cross product */
		Point3f operator * (const float)	const;  /* scale by scalar */
		Point3f operator + (const Point3f&) const;	/* create a temp */

		// make sure that we're unit length - vertical in the error case (0 length)
		void normalize();

		// note - the operators above are for Pnt3f*scalar, here we have scalar*Pnt3f
		friend Point3f operator * (const float s, const Point3f& p );

	public:
		// for simplicity, we just make this public so everything can access
		// it. real software engineers would make the internal data private.
		float x;			/* isn't this obvious */
		float y;
		float z;
};
//*****************************************************************************
//
// inline definitions
//
//*****************************************************************************

//*****************************************************************************
//
// *
//=============================================================================
inline float* Point3f::
v()
//=============================================================================
{
	return &x;
}

//*****************************************************************************
//
// *
//=============================================================================
inline Point3f Point3f::
operator * (const float s) const 
//=============================================================================
{
	return Point3f(x*s,y*s,z*s);
};

//*****************************************************************************
//
// *
//=============================================================================
inline Point3f operator * (const float s, const Point3f& p)  
//=============================================================================
{
	return Point3f(s*p.x,s*p.y,s*p.z);
};

//*****************************************************************************
//
// *
//=============================================================================
inline Point3f Point3f::
operator + (const Point3f& p) const 
//=============================================================================
{
	return Point3f(x+p.x, y+p.y, z+p.z);
};

//*****************************************************************************
//
// * cross product
//=============================================================================
inline Point3f Point3f::
operator*(const Point3f& p) const 
//=============================================================================
{
	Point3f q;
	q.x = p.z * y - p.y * z;
	q.y = p.x * z - p.z * x;
	q.z = p.y * x - p.x * y;
	return q;
};