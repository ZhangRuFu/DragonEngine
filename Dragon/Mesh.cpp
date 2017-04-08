#include "Mesh.h"

StaticMesh::StaticMesh(vector<Vertex> *vertices, vector<Texture*>* textures, vector<unsigned int>* indices) :
	m_vertices(vertices), m_textures(*textures), m_indices(*indices)
{
}

int StaticMesh::GetVertexCount() const
{
	return m_vertices->size();
}

int StaticMesh::GetIndexCount() const
{
	return m_indices.size();
}

void * StaticMesh::GetVertex() const
{
	return m_vertices->data();
}

void * StaticMesh::GetIndex() const
{
	return (void*)m_indices.data();
}

int StaticMesh::GetTextureCount() const
{
	return m_textures.size();
}

const Texture* StaticMesh::GetTexture(int index) const
{
	return m_textures[index];
}

void StaticMesh::AddTexture(Texture * tex)
{
	m_textures.push_back(tex);
}


//===================================================VertexWeight====================================================
VertexWeight::VertexWeight(void)
{
	for (int i = 0; i < BONES_COUNT; i++)
	{
		m_boneIndex[i] = 0;
		m_weight[i] = 0;
	}
}

void VertexWeight::AddVertexBoneInfo(int boneIndex, float weight)
{
	for (int i = 0; i < BONES_COUNT; i++)
	{
		if (m_weight[i] != 0)
			continue;
		m_boneIndex[i] = boneIndex;
		m_weight[i] = weight;
		break;
	}
}