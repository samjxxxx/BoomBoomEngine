#include "Mesh.h"

#include <GL/glew.h>

namespace Kawaii
{
	Mesh::Mesh(const std::vector<Vertex>& vert, const std::vector<unsigned int>& indi)
	{
		setupMesh(vert, indi);
	}

	Mesh::~Mesh()
	{
		clearMesh();
	}

	void Mesh::draw(bool instance, int amount) const
	{
		glBindVertexArray(m_vao);
		if (!instance)
			glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
		else
			glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
				0, amount);
		glBindVertexArray(0);
	}

	void Mesh::setupMesh(const std::vector<Vertex>& vert, const std::vector<unsigned int>& indi)
	{
		m_vertices = vert;
		m_indices = indi;

		// create buffers/arrays
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
			&m_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
			&m_indices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			static_cast<void*>(0));
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, normal)));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, texcoord)));
		// vertex colors
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, color)));
		// vertex tangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
		// vertex bitangent
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

		glBindVertexArray(0);
	}

	void Mesh::clearMesh()
	{
		std::vector<Vertex>().swap(m_vertices);
		std::vector<unsigned int>().swap(m_indices);
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
	}

	void Mesh::generateTangentAndBitangent(const glm::vec3& normal,
		glm::vec3& tangent, glm::vec3& bitangent)
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		if (normal == up)
			up = glm::vec3(1, 0, 0);
		tangent = glm::cross(normal, up);
		bitangent = glm::cross(tangent, normal);
	}

}