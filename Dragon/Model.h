#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Mesh
*	描　　述：代表一个GameEntity的外观、可视化
*
*/

#include <vector>
#include <map>
#include <GLM\glm.hpp>
#include "Mesh.h"
#include "CommonType.h"

using std::vector;
using std::map;
using glm::mat4;

/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Model
*	描　　述：模型类的抽象基类
*
*/

class Model
{
public:
	virtual void Move() = 0;

	virtual bool hasAnimation() = 0;

	virtual void AddTexture(string texturePath) = 0;
};



/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：StaticModel
*	描　　述：静态模型
*
*/
class StaticModel : public Model
{
private:
	vector<StaticMesh*> m_subMeshes;
	vector<Texture*> m_texutres;

public:
	StaticModel(vector<StaticMesh*> subMeshes, vector<Texture*> textures);

	virtual void Move();

	virtual bool hasAnimation() { return false; }

	int GetSubMeshCount();

	const StaticMesh* GetMesh(int index);

	void AddTexture(string texturePath);
};

/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：SkeletonMesh
*	描　　述：动态模型的子网格
*
*/
struct Key
{
	int m_key;
	float m_value;

public:
	Key(int key, float value) :m_key(key), m_value(value) {}
};

class BoneAnimation
{
private:
	vector<Key>* m_translation[3];			//Translate中X、Y、Z分量的Key
	vector<Key>* m_rotate[3];				//Rotate中X、Y、Z分量的Key
	vector<Key>* m_scale[3];				//Scale中X、Y、Z分量的Key
	vec3* m_prePostRotate;

public:
	enum FrameType { TX, TY, TZ, RX, RY, RZ, SX, SY, SZ };

public:
	BoneAnimation()
	{
		for (int i = 0; i < 3; i++)
		{
			m_translation[i] = nullptr;
			m_rotate[i] = nullptr;
			m_scale[i] = nullptr;
			m_prePostRotate = nullptr;
		}
	}
	int GetKeyCount(FrameType type)
	{
		switch (type)
		{
		case FrameType::TX:
		case FrameType::TY:
		case FrameType::TZ:
			m_translation[type]->size();
			break;
		case FrameType::RX:
		case FrameType::RY:
		case FrameType::RZ:
			m_rotate[type % 3]->size();
			break;
		case FrameType::SX:
		case FrameType::SY:
		case FrameType::SZ:
			m_scale[type % 3]->size();
			break;
		}
	}

	void AddFrame(vector<Key>* values, FrameType type)
	{
		switch (type)
		{
		case FrameType::TX:
		case FrameType::TY:
		case FrameType::TZ:
			m_translation[type] = values;
			break;
		case FrameType::RX:
		case FrameType::RY:
		case FrameType::RZ:
			m_rotate[type % 3] = values;
			break;
		case FrameType::SX:
		case FrameType::SY:
		case FrameType::SZ:
			m_scale[type % 3] = values;
			break;
		}
	}
	void AddFrame(vec3 *prePostRotate)
	{
		m_prePostRotate = prePostRotate;
	}

	mat4 GetTransform(int key);
	vec3 GetKeyValue(int key, vector<Key>* Keyvector[3]);
};

struct Bone
{
	string m_name;
	mat4 m_offsetMatrix;
	int m_childCount;
	Bone *m_child;
	BoneAnimation *m_animation;

	Bone(void) : m_child(nullptr), m_animation(nullptr) {}
};


struct BoneIndex
{
	Bone *bone;
	int index;
};

class SkeletonModel : public Model
{
	friend class FBXModelLoader;
private:
	vector<SkeletonMesh> m_meshes;			//SkeletonSubMesh
	vector<Texture*> m_texutres;			//Texture
	Bone *m_rootBone;						//骨骼信息
	map<string, BoneIndex> m_bonesMap;		//骨骼名-索引映射表
	vector<mat4> m_boneTransform;			//最终Transform矩阵
	
	float m_spf;							//每帧所需时间
	float m_curTime;						//当前帧积累时间
	int m_keyCount;							//动画帧总数
	int m_curKey;							//当前帧

public:
	class SkeletonModelException
	{
	private:
		string m_err;

	public:
		SkeletonModelException(string err) :m_err(err) {}
	};

public:
	SkeletonModel(void);

	virtual void Move(void);

	virtual bool hasAnimation(void) { return true; }
	
	virtual void AddTexture(string texturePath) {};

	int GetMeshCount() { return m_meshes.size(); };

	const SkeletonMesh* GetMesh(int index) { return &m_meshes[index]; };

	const vector<mat4>& GetBoneTransform(void) { return m_boneTransform; }

private:
	void SkeletonUpdate(Bone *node, mat4 &parentTransform);
};
