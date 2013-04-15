//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Sphere.h
//  Content:    The sphere class.
//  $Id:        Sphere.h 101 2010-03-01 20:43:38Z agray $
//
//////////////////////////////////////////////////////////////////////////////

// This file is based on material originally from:
// OGRE (Object-oriented Graphics Rendering Engine)
// Copyright (c) 2000-2009 Torus Knot Software Ltd
// Distributed under the MIT License.
// http://www.ogre3d.org
// http://www.ogre3d.org/licensing/licensing-faq

#ifndef __ZSPACE_SPHERE_H__
#define __ZSPACE_SPHERE_H__

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

	/** A sphere primitive, mostly used for bounds checking. 
    @remarks
        A sphere in math texts is normally represented by the function
        <i>x^2 + y^2 + z^2 = r^2</i> (for spheres centered on the origin). Ogre stores spheres
        simply as a center point and a radius.
    */
    class ZSPACE_COMMON_API Sphere
    {
    protected:
        float mRadius;
        Vector3 mCenter;
    public:
        /** Standard constructor - creates a unit sphere around the origin.*/
        Sphere() : mRadius(1.0), mCenter(Vector3::ZERO()) {}
        /** Constructor allowing arbitrary spheres. 
            @param center The center point of the sphere.
            @param radius The radius of the sphere.
        */
        Sphere(const Vector3& center, float radius)
            : mRadius(radius), mCenter(center) {}

        /** Returns the radius of the sphere. */
        float getRadius(void) const { return mRadius; }

        /** Sets the radius of the sphere. */
        void setRadius(float radius) { mRadius = radius; }

        /** Returns the center point of the sphere. */
        const Vector3& getCenter(void) const { return mCenter; }

        /** Sets the center point of the sphere. */
        void setCenter(const Vector3& center) { mCenter = center; }

		/** Returns whether or not this sphere intersects another sphere. */
		bool intersects(const Sphere& s) const
		{
            return (s.mCenter - mCenter).squaredLength() <=
                Math::Sqr(s.mRadius + mRadius);
		}
		/** Returns whether or not this sphere intersects a box. */
		bool intersects(const AxisAlignedBox& box) const
		{
			return Math::intersects(*this, box);
		}
		/** Returns whether or not this sphere intersects a plane. */
		bool intersects(const Plane& plane) const
		{
			return Math::intersects(*this, plane);
		}
		/** Returns whether or not this sphere intersects a point. */
		bool intersects(const Vector3& v) const
		{
            return ((v - mCenter).squaredLength() <= Math::Sqr(mRadius));
		}
        

    };

/** @} */   // End of group: Math
/** @} */   // End of group: Common

ZSPACE_COMMON_NS_END

#endif // __ZSPACE_SPHERE_H__

