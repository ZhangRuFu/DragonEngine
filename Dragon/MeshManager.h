#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������MeshManager
*	�衡������Mesh��������������Model������̬�Ͷ�̬���м��غ͹������ṩ��������Mesh���д���
			��������DragonEngine���г�ʼ��
*
*/
#include <vector>
#include <string>
#include <map>
#include "Model.h"
#include "CommonType.h"

using std::string;
using std::vector;
using std::map;

class MeshManager
{
private:
	static MeshManager *m_instance;

private:
	vector<Model*> m_meshes;							//ע��������Ϸ�е�Model����ԴΨһ)
	vector<Model*> m_basicMeshes;						//�����Ļ���������
	vector<Model*> m_animationMesh;						//���ж�����������Ҫ����Move
	map<string, int> m_meshMap;							//Model·��-���� ӳ���

public:
	Model* _LoadModel(string meshPath);					//�����Զ����ģ���ļ�
	Model* _LoadModel(BasicMesh basicMesh);				//���ػ���������
	
	static Model* LoadModel(string meshPath);
	static Model* LoadModel(BasicMesh basicMesh);
	static MeshManager* GetInstance(void);

	void Move(void);
private:
	MeshManager(void);
	void CreateBasicMesh(void);
};