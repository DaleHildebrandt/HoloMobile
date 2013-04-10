//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Quaternion.h
//  Content:    The quaternion class.
//  $Id:        Quaternion.h 101 2010-03-01 20:43:38Z agray $
//
//////////////////////////////////////////////////////////////////////////////

// This file is based on material originally from:
// OGRE (Object-oriented Graphics Rendering Engine)
// Copyright (c) 2000-2009 Torus Knot Software Ltd
// Distributed under the MIT License.
// http://www.ogre3d.org
// http://www.ogre3d.org/licensing/licensing-faq

#ifndef __ZSPACE_QUATERNION_H__
#define __ZSPACE_QUATERNION_H__

#include "ZSpace/Common/CommonDefines.h"
#include "ZSpace/Common/Math/Math.h"
#include "ZSpace/Common/Error/Error.h"

ZSPACE_COMMON_NS_BEGIN

	// Pre-declare classes
	class Matrix3;
  class Vector3;

  /**
  * @addtogroup Common
  * @{
  *
  *   @addtogroup Math
  *   @{
  */

	/** Implementation of a quaternion
	*  @remarks
	*  For the definition of a quaternion, please see http://en.wikipedia.org/wiki/Quaternion.
    */
    class ZSPACE_COMMON_API Quaternion
    {
    public:
        inline Quaternion (
            float fW = 1.0,
            float fX = 0.0, float fY = 0.0, float fZ = 0.0)
		{
			w = fW;
			x = fX;
			y = fY;
			z = fZ;
		}
        /// Construct a quaternion from a rotation matrix
        inline Quaternion(const Matrix3& rot)
        {
            this->FromRotationMatrix(rot);
        }
        /// Construct a quaternion from an angle and axis
        inline Quaternion(const Radian& rfAngle, const Vector3& rkAxis)
        {
            this->FromAngleAxis(rfAngle, rkAxis);
        }
        /// Construct a quaternion from three orthonormal local axes
        inline Quaternion(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
        {
            this->FromAxes(xaxis, yaxis, zaxis);
        }
        /// Construct a quaternion from three orthonormal local axes
        inline Quaternion(const Vector3* akAxis)
        {
            this->FromAxes(akAxis);
        }
		/// Construct a quaternion from four manual w, x, y, and z values
		inline Quaternion(float* valptr)
		{
			memcpy(&w, valptr, sizeof(float)*4);
		}

		/** Exchange the contents of this quaternion with another. 
		*/
		inline void swap(Quaternion& other)
		{
			std::swap(w, other.w);
			std::swap(x, other.x);
			std::swap(y, other.y);
			std::swap(z, other.z);
		}

		/// Array accessor operator
		inline float operator [] ( const size_t i ) const
		{
			ZS_ASSERT(0x00000107, i < 4, "Out-of-bounds element requested from 4-element Quaternion.");

			return *(&w+i);
		}

		/// Array accessor operator
		inline float& operator [] ( const size_t i )
		{
			ZS_ASSERT(0x00000108, i < 4, "Out-of-bounds element requested from 4-element Quaternion.");

			return *(&w+i);
		}

		/// Pointer accessor for direct copying
		inline float* ptr()
		{
			return &w;
		}

		/// Pointer accessor for direct copying
		inline const float* ptr() const
		{
			return &w;
		}

    Vector3 computeEulerAngles() const;
		void FromRotationMatrix (const Matrix3& kRot);
        void ToRotationMatrix (Matrix3& kRot) const;
        void FromAngleAxis (const Radian& rfAngle, const Vector3& rkAxis);
        void ToAngleAxis (Radian& rfAngle, Vector3& rkAxis) const;
        inline void ToAngleAxis (Degree& dAngle, Vector3& rkAxis) const {
            Radian rAngle;
            ToAngleAxis ( rAngle, rkAxis );
            dAngle = rAngle;
        }
        void FromAxes (const Vector3* akAxis);
        void FromAxes (const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
        void ToAxes (Vector3* akAxis) const;
        void ToAxes (Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const;
        /// Get the local x-axis
        Vector3 xAxis(void) const;
        /// Get the local y-axis
        Vector3 yAxis(void) const;
        /// Get the local z-axis
        Vector3 zAxis(void) const;

        inline Quaternion& operator= (const Quaternion& rkQ)
		{
			w = rkQ.w;
			x = rkQ.x;
			y = rkQ.y;
			z = rkQ.z;
			return *this;
		}
        Quaternion operator+ (const Quaternion& rkQ) const;
        Quaternion operator- (const Quaternion& rkQ) const;
        Quaternion operator* (const Quaternion& rkQ) const;
        Quaternion operator* (float fScalar) const;
        ZSPACE_COMMON_API friend Quaternion operator* (float fScalar,
            const Quaternion& rkQ);
        Quaternion operator- () const;
        inline bool operator== (const Quaternion& rhs) const
		{
			return (rhs.x == x) && (rhs.y == y) &&
				(rhs.z == z) && (rhs.w == w);
		}
        inline bool operator!= (const Quaternion& rhs) const
		{
			return !operator==(rhs);
		}
        // functions of a quaternion
        float Dot (const Quaternion& rkQ) const;  // dot product
        float Norm () const;  // squared-length
        /// Normalises this quaternion, and returns the previous length
        float normalise(void); 
        Quaternion Inverse () const;  // apply to non-zero quaternion
        Quaternion UnitInverse () const;  // apply to unit-length quaternion
        Quaternion Exp () const;
        Quaternion Log () const;

        // rotation of a vector by a quaternion
        Vector3 operator* (const Vector3& rkVector) const;

   		/** Calculate the local roll element of this quaternion.
		@param reprojectAxis By default the method returns the intuitive result.
			That is, if you projected the local <i>Y</i> of the quaternion onto the <i>X</i> and
			<i>Y</i> axes, the angle between them is returned. If set to false though, the
			result is the actual yaw that will be used to implement the quaternion.
			That is the shortest possible path to get to the same orientation and 
			may involve less axial rotation. 
		*/
		Radian getRoll(bool reprojectAxis = true) const;
   		/** Calculate the local pitch element of this quaternion
		@param reprojectAxis By default the method returns the intuitive result.
			That is, if you projected the local <i>Z</i> of the quaternion onto the <i>X</i> and
			<i>Y</i> axes, the angle between them is returned. If set to true though, the
			result is the actual yaw that will be used to implement the quaternion.
			That is the shortest possible path to get to the same orientation and 
			may involve less axial rotation. 
		*/
		Radian getPitch(bool reprojectAxis = true) const;
   		/** Calculate the local yaw element of this quaternion
		@param reprojectAxis By default the method returns the intuitive result.
			That is, if you projected the local <i>Z</i> of the quaternion onto the <i>X</i> and
			<i>Z</i> axes, the angle between them is returned. If set to true though, the
			result is the actual yaw that will be used to implement the quaternion.
			That is the shortest possible path to get to the same orientation and 
			may involve less axial rotation. 
		*/
		Radian getYaw(bool reprojectAxis = true) const;		
		/// Equality with tolerance (tolerance is max angle difference)
    bool equals(const Quaternion& rhs, const Radian& tolerance = Radian(Math::SQRT_EPSILON())) const;
		
	    // spherical linear interpolation
        static Quaternion Slerp (float fT, const Quaternion& rkP,
            const Quaternion& rkQ, bool shortestPath = false);

        static Quaternion SlerpExtraSpins (float fT,
            const Quaternion& rkP, const Quaternion& rkQ,
            int iExtraSpins);

        // setup for spherical quadratic interpolation
        static void Intermediate (const Quaternion& rkQ0,
            const Quaternion& rkQ1, const Quaternion& rkQ2,
            Quaternion& rka, Quaternion& rkB);

        // spherical quadratic interpolation
        static Quaternion Squad (float fT, const Quaternion& rkP,
            const Quaternion& rkA, const Quaternion& rkB,
            const Quaternion& rkQ, bool shortestPath = false);

        // normalised linear interpolation, which is faster but less accurate (non-constant rotation velocity)
        static Quaternion nlerp(float fT, const Quaternion& rkP, 
            const Quaternion& rkQ, bool shortestPath = false);

        // special values
        static const Quaternion& ZERO();
        static const Quaternion& IDENTITY();

		float w, x, y, z;

		/// Check whether this quaternion contains valid values
		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z) || Math::isNaN(w);
		}

        /** Function for writing to a stream. Outputs "Quaternion(w, x, y, z)" with w,x,y,z
            being the member values of the quaternion.
        */
        inline ZSPACE_COMMON_API friend std::ostream& operator <<
            ( std::ostream& o, const Quaternion& q )
        {
            o << "Quaternion(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
            return o;
        }

    };

/** @} */   // End of group: Math
/** @} */   // End of group: Common

ZSPACE_COMMON_NS_END

#endif // __ZSPACE_QUATERNION_H__
