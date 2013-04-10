//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       PlaneBoundedVolume.h
//  Content:    The plane bounded volume class.
//  $Id:        PlaneBoundedVolume.h 101 2010-03-01 20:43:38Z agray $
//
//////////////////////////////////////////////////////////////////////////////

// This file is based on material originally from:
// OGRE (Object-oriented Graphics Rendering Engine)
// Copyright (c) 2000-2009 Torus Knot Software Ltd
// Distributed under the MIT License.
// http://www.ogre3d.org
// http://www.ogre3d.org/licensing/licensing-faq

#ifndef __ZSPACE_PLANE_BOUNDED_VOLUME_H__
#define __ZSPACE_PLANE_BOUNDED_VOLUME_H__

// Precompiler options
#include "ZSpace/Common/CommonDefines.h"

#include "ZSpace/Common/Math/AxisAlignedBox.h"
#include "ZSpace/Common/Math/Math.h"
#include "ZSpace/Common/Math/Plane.h"
#include "ZSpace/Common/Math/Sphere.h"

ZSPACE_COMMON_NS_BEGIN

/**
* @addtogroup Common
* @{
*
*   @addtogroup Math
*   @{
*/

	/** Represents a convex volume bounded by planes.
    */
    class ZSPACE_COMMON_API PlaneBoundedVolume
    {
    public:
		typedef std::vector<Plane> PlaneList;
        /// Publicly accessible plane list, you can modify this direct
        PlaneList planes;
        Plane::Side outside;

        PlaneBoundedVolume() :outside(Plane::NEGATIVE_SIDE) {}
        /** Constructor, determines which side is deemed to be 'outside' */
        PlaneBoundedVolume(Plane::Side theOutside) 
            : outside(theOutside) {}

        /** Intersection test with AABB
        @remarks May return false positives but will never miss an intersection.
        */
        inline bool intersects(const AxisAlignedBox& box) const
        {
            if (box.isNull()) return false;
            if (box.isInfinite()) return true;

            // Get center of the box
            Vector3 centre = box.getCenter();
            // Get the half-size of the box
            Vector3 halfSize = box.getHalfSize();
            
            PlaneList::const_iterator i, iend;
            iend = planes.end();
            for (i = planes.begin(); i != iend; ++i)
            {
                const Plane& plane = *i;

                Plane::Side side = plane.getSide(centre, halfSize);
                if (side == outside)
                {
                    // Found a splitting plane therefore return not intersecting
                    return false;
                }
            }

            // couldn't find a splitting plane, assume intersecting
            return true;

        }
        /** Intersection test with Sphere
        @remarks May return false positives but will never miss an intersection.
        */
        inline bool intersects(const Sphere& sphere) const
        {
            PlaneList::const_iterator i, iend;
            iend = planes.end();
            for (i = planes.begin(); i != iend; ++i)
            {
                const Plane& plane = *i;

                // Test which side of the plane the sphere is
                float d = plane.getDistance(sphere.getCenter());
                // Negate d if planes point inwards
                if (outside == Plane::NEGATIVE_SIDE) d = -d;

                if ( (d - sphere.getRadius()) > 0)
                    return false;
            }

            return true;

        }

        /** Intersection test with a Ray
        @returns std::pair of hit (bool) and distance
        @remarks May return false positives but will never miss an intersection.
        */
        inline std::pair<bool, float> intersects(const Ray& ray)
        {
            return Math::intersects(ray, planes, outside == Plane::POSITIVE_SIDE);
        }

    };

/** @} */   // End of group: Math
/** @} */   // End of group: Common

typedef std::vector<PlaneBoundedVolume> PlaneBoundedVolumeList;

ZSPACE_COMMON_NS_END

#endif // __ZSPACE_PLANE_BOUNDED_VOLUME_H__

