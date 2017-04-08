#include "Model.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "Time.h"

StaticModel::StaticModel(vector<StaticMesh*> subMeshes, vector<Texture*> textures) : m_subMeshes(subMeshes), m_texutres(textures) {}

void StaticModel::Move()
{

}

int StaticModel::GetSubMeshCount()
{
	return m_subMeshes.size();
}

const StaticMesh * StaticModel::GetMesh(int index)
{
	return m_subMeshes[index];
}

void StaticModel::AddTexture(string texturePath)
{
	unsigned int texID = TextureManager::LoadTexture(texturePath, false);
	Texture *tex = new Texture();
	tex->m_id = texID;
	tex->m_path = texturePath;
	tex->m_type = "texDiffuse";
	m_subMeshes[0]->AddTexture(tex);
	m_texutres.push_back(tex);
}



//==================================================SkeletonMesh================================================
SkeletonModel::SkeletonModel(void)
{
	m_spf = 1.0 / 30 * 1000;		//ms
	m_curTime = 0;
	m_rootBone = nullptr;
	m_curKey = m_keyCount = 0;
}

void SkeletonModel::Move(void)
{
	m_curTime += Time::GetDeltaTime();
	//=================================为了画面流畅，应该帧数保留小数=================================
	if (m_curTime < m_spf)
		return;
	m_curTime = 0;
	m_curKey++;
	m_curKey %= m_keyCount;
	SkeletonUpdate(m_rootBone, mat4());
}

void SkeletonModel::SkeletonUpdate(Bone * node, mat4 & parentTransform)
{
	string boneName = node->m_name;
	mat4 transform;
	if (m_bonesMap.find(boneName) != m_bonesMap.end())
	{
		const BoneIndex &boneIndex = m_bonesMap[boneName];
		BoneAnimation &boneAnimation = *boneIndex.bone->m_animation;
		transform = boneAnimation.GetTransform(m_curKey);
		m_boneTransform[boneIndex.index] = parentTransform * transform * boneIndex.bone->m_offsetMatrix;
	}
	for (int i = 0; i < node->m_childCount; i++)
		SkeletonUpdate(node->m_child + i, parentTransform * transform);
}
