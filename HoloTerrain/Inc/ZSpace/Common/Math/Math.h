//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Math.h
//  Content:    Radian, Degree, Angle classes and static Math class
//  $Id:        Math.h 101 2010-03-01 20:43:38Z agray $
//
//////////////////////////////////////////////////////////////////////////////

// This file is based on material originally from:
// OGRE (Object-oriented Graphics Rendering Engine)
// Copyright (c) 2000-2009 Torus Knot Software Ltd
// Distributed under the MIT License.
// http://www.ogre3d.org
// http://www.ogre3d.org/licensing/licensing-faq

#ifndef __ZSPACE_MATH_H__
#define __ZSPACE_MATH_H__

#include "ZSpace/Common/CommonDefines.h"
#include "ZSpace/Common/Error/Error.h"

// required for inlines
#include <limits>
#include <list>
#include <ostream>
#include <vector>

ZSPACE_COMMON_NS_BEGIN

// Pre-declare classes
class AxisAlignedBox;
class Degree;
class Matrix3;
class Matrix4;
class Plane;
class PlaneBoundedVolume;
class Quaternion;
class Radian;
class Ray;
class Sphere;
class Vector2;
class Vector3;
class Vector4;

/**
* @addtogroup Common
* @{
*
*   @addtogroup Math
*   @{
*/

	/** Wrapper class which indicates a given angle value is in radians.
    @remarks
        Radian values are interchangeable with Degree values and conversions
        will be done automatically between them.
    */
	class ZSPACE_COMMON_API Radian
	{
		float mRad;

	public:
		explicit Radian ( float r=0 ) : mRad(r) {}
		Radian ( const Degree& d );
		Radian& operator = ( const float& f ) { mRad = f; return *this; }
		Radian& operator = ( const Radian& r ) { mRad = r.mRad; return *this; }
		Radian& operator = ( const Degree& d );

		float valueDegrees() const; // see bottom of this file
		float valueRadians() const { return mRad; }

        const Radian& operator + () const { return *this; }
		Radian operator + ( const Radian& r ) const { return Radian ( mRad + r.mRad ); }
		Radian operator + ( const Degree& d ) const;
		Radian& operator += ( const Radian& r ) { mRad += r.mRad; return *this; }
		Radian& operator += ( const Degree& d );
		Radian operator - () const { return Radian(-mRad); }
		Radian operator - ( const Radian& r ) const { return Radian ( mRad - r.mRad ); }
		Radian operator - ( const Degree& d ) const;
		Radian& operator -= ( const Radian& r ) { mRad -= r.mRad; return *this; }
		Radian& operator -= ( const Degree& d );
		Radian operator * ( float f ) const { return Radian ( mRad * f ); }
        Radian operator * ( const Radian& f ) const { return Radian ( mRad * f.mRad ); }
		Radian& operator *= ( float f ) { mRad *= f; return *this; }
		Radian operator / ( float f ) const { return Radian ( mRad / f ); }
		Radian& operator /= ( float f ) { mRad /= f; return *this; }

		bool operator <  ( const Radian& r ) const { return mRad <  r.mRad; }
		bool operator <= ( const Radian& r ) const { return mRad <= r.mRad; }
		bool operator == ( const Radian& r ) const { return mRad == r.mRad; }
		bool operator != ( const Radian& r ) const { return mRad != r.mRad; }
		bool operator >= ( const Radian& r ) const { return mRad >= r.mRad; }
		bool operator >  ( const Radian& r ) const { return mRad >  r.mRad; }

		inline ZSPACE_COMMON_API friend std::ostream& operator <<
			( std::ostream& o, const Radian& v )
		{
			o << "Radian(" << v.valueRadians() << ")";
			return o;
		}
	};

    /** Wrapper class which indicates a given angle value is in degrees.
    @remarks
        Degree values are interchangeable with Radian values and conversions
        will be done automatically between them.
    */
	class ZSPACE_COMMON_API Degree
	{
		float mDeg;

	public:
		explicit Degree ( float d=0 ) : mDeg(d) {}
		Degree ( const Radian& r ) : mDeg(r.valueDegrees()) {}
		Degree& operator = ( const float& f ) { mDeg = f; return *this; }
		Degree& operator = ( const Degree& d ) { mDeg = d.mDeg; return *this; }
		Degree& operator = ( const Radian& r ) { mDeg = r.valueDegrees(); return *this; }

		float valueDegrees() const { return mDeg; }
		float valueRadians() const; // see bottom of this file

		const Degree& operator + () const { return *this; }
		Degree operator + ( const Degree& d ) const { return Degree ( mDeg + d.mDeg ); }
		Degree operator + ( const Radian& r ) const { return Degree ( mDeg + r.valueDegrees() ); }
		Degree& operator += ( const Degree& d ) { mDeg += d.mDeg; return *this; }
		Degree& operator += ( const Radian& r ) { mDeg += r.valueDegrees(); return *this; }
		Degree operator - () const { return Degree(-mDeg); }
		Degree operator - ( const Degree& d ) const { return Degree ( mDeg - d.mDeg ); }
		Degree operator - ( const Radian& r ) const { return Degree ( mDeg - r.valueDegrees() ); }
		Degree& operator -= ( const Degree& d ) { mDeg -= d.mDeg; return *this; }
		Degree& operator -= ( const Radian& r ) { mDeg -= r.valueDegrees(); return *this; }
		Degree operator * ( float f ) const { return Degree ( mDeg * f ); }
        Degree operator * ( const Degree& f ) const { return Degree ( mDeg * f.mDeg ); }
		Degree& operator *= ( float f ) { mDeg *= f; return *this; }
		Degree operator / ( float f ) const { return Degree ( mDeg / f ); }
		Degree& operator /= ( float f ) { mDeg /= f; return *this; }

		bool operator <  ( const Degree& d ) const { return mDeg <  d.mDeg; }
		bool operator <= ( const Degree& d ) const { return mDeg <= d.mDeg; }
		bool operator == ( const Degree& d ) const { return mDeg == d.mDeg; }
		bool operator != ( const Degree& d ) const { return mDeg != d.mDeg; }
		bool operator >= ( const Degree& d ) const { return mDeg >= d.mDeg; }
		bool operator >  ( const Degree& d ) const { return mDeg >  d.mDeg; }

		inline ZSPACE_COMMON_API friend std::ostream& operator <<
			( std::ostream& o, const Degree& v )
		{
			o << "Degree(" << v.valueDegrees() << ")";
			return o;
		}
	};

	inline Radian::Radian(const Degree &d) : mRad(d.valueRadians()) { }

    /** Class to provide access to common mathematical functions.
        @remarks
            Most of the math functions are aliased versions of the C runtime
            library functions. They are aliased here to provide future
            optimisation opportunities, either from faster RTLs or custom
            math approximations.
        @note
            <br>This is based on MgcMath.h from
            <a href="http://www.geometrictools.com/">Wild Magic</a>.
    */
    class ZSPACE_COMMON_API Math 
    {
   public:
       /** These are the angular units used by the API. This functionality is now deprecated
		   in favor of discreet angular unit types (see Degree and Radian above). The only
		   place where this functionality is still used is when parsing files. Search for
		   usage of the Angle class for those instances.
       */

    protected:
        /// Size of the trig tables is determined by constructor.
        static int mTrigTableSize;

		/// Radian -> index factor value <i>(</i> mTrigTableSize <i>/ 2 * PI )</i>
        static float mTrigTableFactor;
        static float* mSinTable;
        static float* mTanTable;

        /** Private function to build trig tables.
        */
        void buildTrigTables();

		static float SinTable (float fValue);
		static float TanTable (float fValue);
    public:
        /** Default constructor.
            @param
                trigTableSize Optional parameter to set the size of the
                tables used to implement Sin, Cos, Tan
        */
        Math(unsigned int trigTableSize = 4096);

        /** Default destructor.
        */
        ~Math();

		static inline int IAbs (int iValue) { return ( iValue >= 0 ? iValue : -iValue ); }
		static inline int ICeil (float fValue) { return int(ceil(fValue)); }
		static inline int IFloor (float fValue) { return int(floor(fValue)); }
        static int ISign (int iValue);

		static inline float Abs (float fValue) { return float(fabs(fValue)); }
		static inline Degree Abs (const Degree& dValue) { return Degree(fabs(dValue.valueDegrees())); }
		static inline Radian Abs (const Radian& rValue) { return Radian(fabs(rValue.valueRadians())); }
		static Radian ACos (float fValue);
		static Radian ASin (float fValue);
		static inline Radian ATan (float fValue) { return Radian(atan(fValue)); }
		static inline Radian ATan2 (float fY, float fX) { return Radian(atan2(fY,fX)); }
		static inline float Ceil (float fValue) { return float(ceil(fValue)); }
		static inline bool isNaN(float f)
		{
			// std::isnan() is C99, not supported by all compilers
			// However NaN always fails this next test, no other number does.
			return f != f;
		}

        /** Cosine function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation which is faster but less accurate.
        */
        static inline float Cos (const Radian& fValue, bool useTables = false) {
			return (!useTables) ? float(cos(fValue.valueRadians())) : SinTable(fValue.valueRadians() + HALF_PI());
		}
        /** Cosine function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation which is faster but less accurate.
        */
        static inline float Cos (float fValue, bool useTables = false) {
			return (!useTables) ? float(cos(fValue)) : SinTable(fValue + HALF_PI());
		}

		static inline float Exp (float fValue) { return float(exp(fValue)); }

		static inline float Floor (float fValue) { return float(floor(fValue)); }

		static inline float Log (float fValue) { return float(log(fValue)); }

		/// Stored value of log(2) for frequent use
		static float LOG2();

		static inline float Log2 (float fValue) { return float(log(fValue)/LOG2()); }

		static inline float LogN (float base, float fValue) { return float(log(fValue)/log(base)); }

		static inline float Pow (float fBase, float fExponent) { return float(pow(fBase,fExponent)); }

        static float Sign (float fValue);
		static inline Radian Sign ( const Radian& rValue )
		{
			return Radian(Sign(rValue.valueRadians()));
		}
		static inline Degree Sign ( const Degree& dValue )
		{
			return Degree(Sign(dValue.valueDegrees()));
		}

        /** Sine function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation which is faster but less accurate.
        */
        static inline float Sin (const Radian& fValue, bool useTables = false) {
			return (!useTables) ? float(sin(fValue.valueRadians())) : SinTable(fValue.valueRadians());
		}
        /** Sine function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation which is faster but less accurate.
        */
        static inline float Sin (float fValue, bool useTables = false) {
			return (!useTables) ? float(sin(fValue)) : SinTable(fValue);
		}

		static inline float Sqr (float fValue) { return fValue*fValue; }

		static inline float Sqrt (float fValue) { return float(sqrt(fValue)); }

        static inline Radian Sqrt (const Radian& fValue) { return Radian(sqrt(fValue.valueRadians())); }

        static inline Degree Sqrt (const Degree& fValue) { return Degree(sqrt(fValue.valueDegrees())); }

        /** Inverse square root, that is, 1 / Sqrt(x), which is good for vector
            normalisation.
        */
		static float InvSqrt(float fValue);

        static float UnitRandom ();  // in [0,1]

        static float RangeRandom (float fLow, float fHigh);  // in [fLow,fHigh]

        static float SymmetricRandom ();  // in [-1,1]

        /** Tangent function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation which is faster but less accurate.
        */
		static inline float Tan (const Radian& fValue, bool useTables = false) {
			return (!useTables) ? float(tan(fValue.valueRadians())) : TanTable(fValue.valueRadians());
		}
        /** Tangent function.
            @param
                fValue Angle in radians
            @param
                useTables If true, uses lookup tables rather than
                calculation which is faster but less accurate.
        */
		static inline float Tan (float fValue, bool useTables = false) {
			return (!useTables) ? float(tan(fValue)) : TanTable(fValue);
		}

		static inline float DegreesToRadians(float degrees) { return degrees * fDeg2Rad(); }
        static inline float RadiansToDegrees(float radians) { return radians * fRad2Deg(); }

       /** Checks whether a given point is inside a triangle, in a
            two-dimensional (Cartesian) space.
            @remarks
                The vertices of the triangle must be given in either
                trigonometrical (counterclockwise) or inverse trigonometrical
                (clockwise) order.
            @param
                p The point.
            @param
                a The triangle's first vertex.
            @param
                b The triangle's second vertex.
            @param
                c The triangle's third vertex.
            @returns
                If the point resides in the triangle, <b>true</b> is
                returned.
            @par
                If the point is outside the triangle, <b>false</b> is
                returned.
        */
        static bool pointInTri2D(const Vector2& p, const Vector2& a, 
			const Vector2& b, const Vector2& c);

       /** Checks whether a given 3D point is inside a triangle.
       @remarks
            The vertices of the triangle must be given in either
            trigonometrical (counterclockwise) or inverse trigonometrical
            (clockwise) order, and the point must be guaranteed to be in the
			same plane as the triangle
        @param
            p The point.
        @param
            a The triangle's first vertex.
        @param
            b The triangle's second vertex.
        @param
            c The triangle's third vertex.
		@param 
			normal The triangle plane's normal (passed in rather than calculated
				on demand since the caller may already have it)
        @returns
            If the point resides in the triangle, <b>true</b> is
            returned.
        @par
            If the point is outside the triangle, <b>false</b> is
            returned.
        */
        static bool pointInTri3D(const Vector3& p, const Vector3& a, 
			const Vector3& b, const Vector3& c, const Vector3& normal);
        /** Ray / plane intersection, returns boolean result and distance. */
        static std::pair<bool, float> intersects(const Ray& ray, const Plane& plane);

        /** Ray / sphere intersection, returns boolean result and distance. */
        static std::pair<bool, float> intersects(const Ray& ray, const Sphere& sphere, 
            bool discardInside = true);
        
        /** Ray / box intersection, returns boolean result and distance. */
        static std::pair<bool, float> intersects(const Ray& ray, const AxisAlignedBox& box);

        /** Ray / box intersection, returns boolean result and two intersection distance.
        @param
            ray The ray.
        @param
            box The box.
        @param
            d1 A real pointer to retrieve the near intersection distance
                from the ray origin, maybe <b>null</b> which means the near intersection distance does not matter.
        @param
            d2 A real pointer to retrieve the far intersection distance
                from the ray origin, maybe <b>null</b> which means the far intersection distance does not matter.
        @returns
            If the ray is intersects the box, <b>true</b> is returned, and
            the near intersection distance is return by <i>d1</i>, the
            far intersection distance is return by <i>d2</i>. Guarantee
            <b>0</b> <= <i>d1</i> <= <i>d2</i>.
        @par
            If the ray isn't intersects the box, <b>false</b> is returned, and
            <i>d1</i> and <i>d2</i> is unmodified.
        */
        static bool intersects(const Ray& ray, const AxisAlignedBox& box,
            float* d1, float* d2);

        /** Ray / triangle intersection, returns boolean result and distance.
        @param
            ray The ray.
        @param
            a The triangle's first vertex.
        @param
            b The triangle's second vertex.
        @param
            c The triangle's third vertex.
		@param 
			normal The triangle plane's normal (passed in rather than calculated
				on demand since the caller may already have it) which does not need
                to be normalised since it does not matter in this context.
        @param
            positiveSide Intersect with positive side of the triangle
        @param
            negativeSide Intersect with negative side of the triangle
        @returns
            If the ray is intersects the triangle, a pair of <b>true</b> and the
            distance between intersection point and ray origin gets returned.
        @par
            If the ray isn't intersects the triangle, a pair of <b>false</b> and
            <b>0</b> get returned.
        */
        static std::pair<bool, float> intersects(const Ray& ray, const Vector3& a,
            const Vector3& b, const Vector3& c, const Vector3& normal,
            bool positiveSide = true, bool negativeSide = true);

        /** Ray / triangle intersection, returns boolean result and distance.
        @param
            ray The ray.
        @param
            a The triangle's first vertex.
        @param
            b The triangle's second vertex.
        @param
            c The triangle's third vertex.
        @param
            positiveSide Intersect with positive side of the triangle
        @param
            negativeSide Intersect with negative side of the triangle
        @returns
            If the ray is intersects the triangle, a pair of <b>true</b> and the
            distance between intersection point and ray origin get returned.
        @par
            If the ray isn't intersects the triangle, a pair of <b>false</b> and
            <b>0</b> get returned.
        */
        static std::pair<bool, float> intersects(const Ray& ray, const Vector3& a,
            const Vector3& b, const Vector3& c,
            bool positiveSide = true, bool negativeSide = true);

        /** Sphere / box intersection test. */
        static bool intersects(const Sphere& sphere, const AxisAlignedBox& box);

        /** Plane / box intersection test. */
        static bool intersects(const Plane& plane, const AxisAlignedBox& box);

        /** Ray / convex plane list intersection test. 
        @param ray The ray to test with
        @param plaeList List of planes which form a convex volume
        @param normalIsOutside Does the normal point outside the volume
        */
       static std::pair<bool, float> intersects(
			const Ray& ray, const std::vector<Plane>& planeList, 
            bool normalIsOutside);
        /** Ray / convex plane list intersection test. 
        @param ray The ray to test with
        @param plaeList List of planes which form a convex volume
        @param normalIsOutside Does the normal point outside the volume
        */
       static std::pair<bool, float> intersects(
			const Ray& ray, const std::list<Plane>& planeList, 
            bool normalIsOutside);

        /** Sphere / plane intersection test. 
        @remarks Note: just do a plane.getDistance(sphere.getCenter()) for more detail
        */
        static bool intersects(const Sphere& sphere, const Plane& plane);

        /** Compare two reals, using tolerance for inaccuracies.
        */
        static bool floatEqual(float a, float b, float tolerance = Math::EPSILON());

        /** Calculates the tangent space vector for a given set of positions and texture coordinates. */
        static Vector3 calculateTangentSpaceVector(
            const Vector3& position1, const Vector3& position2, const Vector3& position3,
            float u1, float v1, float u2, float v2, float u3, float v3);

        /** Build a reflection matrix for the passed in plane. */
        static Matrix4 buildReflectionMatrix(const Plane& p);
        /** Calculate a face normal, including the w component which is the offset from the origin. */
        static Vector4 calculateFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
        /** Calculate a face normal, no w-information. */
        static Vector3 calculateBasicFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
        /** Calculate a face normal without normalize, including the w component which is the offset from the origin. */
        static Vector4 calculateFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);
        /** Calculate a face normal without normalize, no w-information. */
        static Vector3 calculateBasicFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);

		/** Generates a value based on the Gaussian (normal) distribution function
			with the given offset and scale parameters.
		*/
		static float gaussianDistribution(float x, float offset = 0.0f, float scale = 1.0f);

		/** Clamp a value within an inclusive range. */
		template <typename T>
		static T Clamp(T val, T minval, T maxval)
		{
			ZS_ASSERT(0x000000e1, minval < maxval, "Invalid clamp range");
			return (val < minval) ? minval : (val > maxval) ? maxval : val;
			// ASG: this compiles in ZSMath project but fails if ZSMath.h included in another proj
			//return std::max(std::min(val, maxval), minval);
		}

		static Matrix4 makeViewMatrix(const Vector3& position, const Quaternion& orientation, 
			const Matrix4* reflectMatrix = 0);

		/** Get a bounding radius value from a bounding box. */
		static float boundingRadiusFromAABB(const AxisAlignedBox& aabb);

    static float EPSILON();
    static float SQRT_EPSILON();
    static float POS_INFINITY();
    static float NEG_INFINITY();
    static float PI();
    static float TWO_PI();
    static float HALF_PI();

		static float fDeg2Rad();
		static float fRad2Deg();

    };

/** @} */   // End of group: Math
/** @} */   // End of group: Common

	// these functions must be defined down here, because they rely on the
	// angle unit conversion functions in class Math:

	inline float Radian::valueDegrees() const
	{
		return Math::RadiansToDegrees ( mRad );
	}

	inline float Degree::valueRadians() const
	{
		return Math::DegreesToRadians ( mDeg );
	}

	inline Radian operator * ( float a, const Radian& b )
	{
		return Radian ( a * b.valueRadians() );
	}

	inline Radian operator / ( float a, const Radian& b )
	{
		return Radian ( a / b.valueRadians() );
	}

	inline Degree operator * ( float a, const Degree& b )
	{
		return Degree ( a * b.valueDegrees() );
	}

	inline Degree operator / ( float a, const Degree& b )
	{
		return Degree ( a / b.valueDegrees() );
	}

ZSPACE_COMMON_NS_END

#endif // __ZSPACE_MATH_H__
