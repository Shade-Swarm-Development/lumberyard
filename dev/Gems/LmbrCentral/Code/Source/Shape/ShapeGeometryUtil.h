/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

#include <AzCore/Math/Vector2.h>
#include <AzCore/Math/Vector3.h>
#include <Cry_Math.h>

namespace LmbrCentral
{
    struct ShapeDrawParams;

    /**
     * Buffers used for rendering Shapes.
     * Generated from shape properties.
     */
    struct ShapeMesh
    {
        AZStd::vector<Vec3> m_vertexBuffer; ///< Vertices of the shape.
        AZStd::vector<vtx_idx> m_indexBuffer; ///< Indices of the shape.
        AZStd::vector<Vec3> m_lineBuffer; ///< Lines of the shape.
    };

    /**
     * Writes 3 indices (1 tri) to the buffer and returns a pointer to the next index.
     */
    vtx_idx* WriteTriangle(AZ::u32 a, AZ::u32 b, AZ::u32 c, vtx_idx* indices);

    /**
     * Writes a vertex to the buffer and returns a pointer to the next vertex.
     */
    Vec3* WriteVertex(const AZ::Vector3& vertex, Vec3* vertices);

    /**
     * Draw a ShapeMesh (previously generated vertices, indices and lines).
     */
    void DrawShape(
        const ShapeDrawParams& shapeDrawParams, const ShapeMesh& shapeMesh);

    /**
     * Return a vector of vertices representing a list of triangles to render (CCW).
     * This is implemented using the Ear Clipping method:
     *  (https://www.gamedev.net/articles/programming/graphics/polygon-triangulation-r3334/)
     * @param vertices List of vertices to process (pass by value as vertices is
     * modified inside the function so must be copied).
     */
    template<typename V3>
    AZStd::vector<V3> GenerateTriangles(AZStd::vector<AZ::Vector2> vertices);

    /**
     * Determine if a list of ordered vertices have clockwise winding order.
     * http://blog.element84.com/polygon-winding.html
     */
    bool ClockwiseOrder(const AZStd::vector<AZ::Vector2>& vertices);

    namespace CapsuleTubeUtil
    {
        /**
         * Generates all indices, assumes index pointer is valid.
         */
        void GenerateSolidMeshIndices(
            AZ::u32 sides, AZ::u32 segments, AZ::u32 capSegments, vtx_idx* indices);

        /**
         * Generate verts to be used when drawing triangles for the start cap.
         */
        Vec3* GenerateSolidStartCap(
            const AZ::Transform& worldFromLocal, const AZ::Vector3& localPosition, float radius,
            const AZ::Vector3& direction, const AZ::Vector3& side, AZ::u32 sides, AZ::u32 capSegments,
            Vec3* vertices);

        /**
         * Generate verts to be used when drawing triangles for the end cap.
         */
        Vec3* GenerateSolidEndCap(
            const AZ::Transform& worldFromLocal, const AZ::Vector3& localPosition, float radius,
            const AZ::Vector3& direction, const AZ::Vector3& side, AZ::u32 sides, AZ::u32 capSegments,
            Vec3* vertices);

        /**
         * Generate vertices to be used for a loop of a segment along a tube or capsule (for use with index buffer).
         */
        Vec3* GenerateSegmentVertices(
            const AZ::Vector3& point,
            const AZ::Vector3& axis,
            const AZ::Vector3& normal,
            float radius,
            AZ::u32 sides,
            const AZ::Transform& transform,
            Vec3* vertices);

        /**
         * Generate a circle/loop for a given segment along the capsule/tube - Produces a series of begin/end
         * line segments to draw in DrawLines.
         */
        Vec3* GenerateWireLoop(
            const AZ::Transform& worldFromLocal, const AZ::Vector3& position, const AZ::Vector3& direction,
            const AZ::Vector3& side, AZ::u32 sides, float radius, Vec3* vertices);

        /**
         * Generate a series of lines to be drawn, arcing around the end of a capsule/tube
         * Two arcs, one horizontal and one vertical, arcing 180 degrees of a sphere.
         */
        Vec3* GenerateWireCap(
            const AZ::Transform& worldFromLocal, const AZ::Vector3& localPosition,
            float radius, const AZ::Vector3& direction, const AZ::Vector3& side,
            AZ::u32 capSegments, Vec3* vertices);

        /**
         * Given a position, forward axis, side axis and angle (radians), calculate
         * the position of a final point on a sphere by summing the rotation of those
         * two axis from their starting orientation.
         */
        AZ::Vector3 CalculatePositionOnSphere(
            const AZ::Transform& worldFromLocal, const AZ::Vector3& localPosition,
            float radius, const AZ::Vector3& forwardAxis, const AZ::Vector3& sideAxis,
            float angle);
    }
}