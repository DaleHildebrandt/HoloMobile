//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Plane.h
//  Content:    The plane class.
//  $Id:        Plane.h 101 2010-03-01 20:43:38Z agray $
//
//////////////////////////////////////////////////////////////////////////////

// This file is based on material originally from:
// OGRE (Object-oriented Graphics Rendering Engine)
// Copyright (c) 2000-2009 Torus Knot Software Ltd
// Distributed under the MIT License.
// http://www.ogre3d.org
// http://www.ogre3d.org/licensing/licensing-faq

#ifndef __ZSPACE_PLANE_H__
#define __ZSPACE_PLANE_H__

#include "ZSpace/Common/CommonDefines.h"

#include "ZSpace/Common/Math/Vector3.h"

ZSPACE_COMMON_NS_BEGIN

/**
* @addtogroup Common
* @{
*
*   @addtogroup Math
*   @{
*/

	/** Defines a plane in 3D space.
        @remarks
            A plane is defined in 3D space by the equation
            <i>Ax + By + Cz + D = 0</i>
        @par
            This equates to a vector (the normal of the plane, whose <i>x</i>, <i>y</i>
            and <i>z</i> components equate to the coefficients <i>A</i>, <i>B</i> and <i>C</i>
            respectively) and a constant (<i>D</i>) which is the distance along
            the normal you have to go to move the plane back to the origin.
     */
    class ZSPACE_COMMON_API Plane
    {
    public:
        /** Default constructor that sets everything to 0.
        */
        Plane ();
        Plane (const Plane& rhs);
        /** Construct a plane through a normal and a distance to move the plane along the normal.*/
        Plane (const Vector3& rkNormal, float fConstant);
		/** Construct a plane using the four constants directly **/
		Plane (float a, float b, float c, float d);
        Plane (const Vector3& rkNormal, const Vector3& rkPoint);
        Plane (const Vector3& rkPoint0, const Vector3& rkPoint1,
            const Vector3& rkPoint2);

        /** The POSITIVE_SIDE of the plane is the half of space to which the
            plane normal points. The NEGATIVE_SIDE is the other half of
            space. The flag NO_SIDE indicates the plane itself.
        */
        enum Side
        {
            NO_SIDE,
            POSITIVE_SIDE,
            NEGATIVE_SIDE,
            BOTH_SIDE
        };

        Side getSide (const Vector3& rkPoint) const;

        /**
        This returns the side where the aligneBox is. The flag BOTH_SIDE indicates an intersecting box.
        One corner ON the plane is sufficient to consider the box and the plane intersecting.
        */
        Side getSide (const AxisAlignedBox& rkBox) const;

        /** This returns on which side of the plane the given box lies.
        @param centre The centre of the box.
        @param halfSize The half-size of the box.
        @returns
            POSITIVE_SIDE if the box complete lies on the "positive side" of the plane.
            NEGATIVE_SIDE if the box complete lies on the "negative side" of the plane.
            BOTH_SIDE if the box intersects the plane.
        */
        Side getSide (const Vector3& centre, const Vector3& halfSize) const;

        /** This is a pseudodistance. 
		@remarks The sign of the return value is
            positive if the point is on the positive side of the plane,
            negative if the point is on the negative side, and zero if the
            point is on the plane.
            @par
            The absolute value of the return value is the true distance only
            when the plane normal is a unit length vector.
        */
        float getDistance (const Vector3& rkPoint) const;

        /** Redefine this plane based on three points. */
        void redefine(const Vector3& rkPoint0, const Vector3& rkPoint1,
            const Vector3& rkPoint2);

		/** Redefine this plane based on a normal and a point. */
		void redefine(const Vector3& rkNormal, const Vector3& rkPoint);

		/** Project a vector onto the plane. 
		@remarks This gives you the element of the input vector that is perpendicular 
			to the normal of the plane. You can get the element which is parallel
			to the normal of the plane by subtracting the result of this method
			from the original vector, since parallel + perpendicular = original.
		@param v The input vector
		*/
		Vector3 projectVector(const Vector3& v) const;

        /** This normalises the plane.
            @remarks
                This method normalises the plane's normal and what the length scale of D
                is as well.
            @note
                This function will not crash for zero-sized vectors, but there
                will be no changes made to their components.
            @returns The previous length of the plane's normal.
        */
        float normalise(void);

		Vector3 normal;
        float d;

        /// Comparison operator
        bool operator==(const Plane& rhs) const
        {
            return (rhs.d == d && rhs.normal == normal);
        }
        bool operator!=(const Plane& rhs) const
        {
            return (rhs.d != d && rhs.normal != normal);
        }

        ZSPACE_COMMON_API friend std::ostream& operator<< (std::ostream& o, const Plane& p);
    };

/** @} */   // End of group: Math
/** @} */   // End of group: Common

typedef std::vector<Plane> PlaneList;

ZSPACE_COMMON_NS_END

#endif // __ZSPACE_PLANE_H__
