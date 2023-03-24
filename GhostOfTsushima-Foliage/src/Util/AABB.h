#pragma once

class AABB
{
public:
	AABB(const glm::vec3& min, const glm::vec3& max);

	std::array<glm::vec3, 8> GetVertices() const;
	bool IsOnOrForwardPlane(const Plane& plane) const;
	bool IsOnFrustum(const Frustum& camFrustum) const;

private:
	glm::vec3 m_Center;
	glm::vec3 m_Extents;
};