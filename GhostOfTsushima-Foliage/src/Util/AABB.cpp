#include "pch.h"
#include "AABB.h"

AABB::AABB(const glm::vec3& min, const glm::vec3& max)
	: m_Center((max + min) * 0.5f), m_Extents(max.x - m_Center.x, max.y - m_Center.y, max.z - m_Center.z)
{}

std::array<glm::vec3, 8> AABB::GetVertices() const
{
	std::array<glm::vec3, 8> vertices;
	vertices[0] = { m_Center.x - m_Extents.x, m_Center.y - m_Extents.y, m_Center.z - m_Extents.z };
	vertices[1] = { m_Center.x + m_Extents.x, m_Center.y - m_Extents.y, m_Center.z - m_Extents.z };
	vertices[2] = { m_Center.x - m_Extents.x, m_Center.y + m_Extents.y, m_Center.z - m_Extents.z };
	vertices[3] = { m_Center.x + m_Extents.x, m_Center.y + m_Extents.y, m_Center.z - m_Extents.z };
	vertices[4] = { m_Center.x - m_Extents.x, m_Center.y - m_Extents.y, m_Center.z + m_Extents.z };
	vertices[5] = { m_Center.x + m_Extents.x, m_Center.y - m_Extents.y, m_Center.z + m_Extents.z };
	vertices[6] = { m_Center.x - m_Extents.x, m_Center.y + m_Extents.y, m_Center.z + m_Extents.z };
	vertices[7] = { m_Center.x + m_Extents.x, m_Center.y + m_Extents.y, m_Center.z + m_Extents.z };
	return vertices;
}

bool AABB::IsOnOrForwardPlane(const Plane& plane) const
{
	const float r = m_Extents.x * std::abs(plane.normal.x) + m_Extents.y * std::abs(plane.normal.y) +
		m_Extents.z * std::abs(plane.normal.z);

	return -r <= plane.getSignedDistanceToPlane(m_Center);
}

bool AABB::IsOnFrustum(const Frustum& camFrustum) const
{
	return (IsOnOrForwardPlane(camFrustum.leftFace) &&
		IsOnOrForwardPlane(camFrustum.rightFace) &&
		IsOnOrForwardPlane(camFrustum.topFace) &&
		IsOnOrForwardPlane(camFrustum.bottomFace) &&
		IsOnOrForwardPlane(camFrustum.nearFace) &&
		IsOnOrForwardPlane(camFrustum.farFace));
}
