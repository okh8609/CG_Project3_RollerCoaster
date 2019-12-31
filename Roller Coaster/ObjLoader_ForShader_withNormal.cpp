#include "ObjLoader_ForShader_withNormal.h"

ObjLoader_ForShader_withNormal::ObjLoader_ForShader_withNormal()
{
}

ObjLoader_ForShader_withNormal::ObjLoader_ForShader_withNormal(const string &filePath, int size) : _filePath(filePath), _size(size)
{
	fstream file(filePath, ios::in);
	if (!file.is_open())
		throw std::runtime_error("Failed to open: " + filePath);

	while (file)
	{
		string line;
		getline(file, line);
		if (line.empty()) continue;

		istringstream line_iss(line);
		string type;
		line_iss >> type;

		if (type == "v")
		{
			QVector3D v;
			float t = 0.0f;

			line_iss >> t; v.setX(t);
			boundsMin.setX(std::min(boundsMin.x(), t));
			boundsMax.setX(std::max(boundsMax.x(), t));
			line_iss >> t; v.setY(t);
			boundsMin.setY(std::min(boundsMin.y(), t));
			boundsMax.setY(std::max(boundsMax.y(), t));
			line_iss >> t; v.setZ(t);
			boundsMin.setZ(std::min(boundsMin.z(), t));
			boundsMax.setZ(std::max(boundsMax.z(), t));

			all_v.push_back(v);
		}
		else if (type == "vn")
		{
			QVector3D vn;
			float t = 0.0f;

			line_iss >> t; vn.setX(t);
			line_iss >> t; vn.setY(t);
			line_iss >> t; vn.setZ(t);

			all_vn.push_back(vn);
		}
		else if (type == "f")
		{
			vector<pair<int, int>> f;

			string str;
			for (size_t i = 0; i < 3; i++)
			{
				int Serial01;
				int Serial02;

				char c;

				line_iss >> str;
				istringstream v_iss(str);

				v_iss >> Serial01 >> c >> c >> Serial02;
				f.push_back(make_pair(Serial01 - 1, Serial02 - 1));
			}

			all_f.push_back(f);
		}
	}

	//正規化成他要的大小
	const QVector3D bounds = boundsMax - boundsMin;
	for (size_t i = 0; i < all_v.size(); ++i)
	{
		all_v.at(i) *= (size / maximum(bounds.x(), bounds.y(), bounds.z()));
		//all_v.at(i) += QVector3D(0, 3, 0);
	}

	// 整理輸出
	all_data = (float*)malloc(sizeof(float) * all_f.size() * 3 * 3 * 2);
	for (size_t i = 0; i < all_f.size(); ++i)
	{
		int v1s = all_f.at(i).at(0).first;
		int v2s = all_f.at(i).at(1).first;
		int v3s = all_f.at(i).at(2).first;

		float * base = all_data + i * 9;

		base[0] = all_v.at(v1s).x();
		base[1] = all_v.at(v1s).y();
		base[2] = all_v.at(v1s).z();

		base[3] = all_v.at(v2s).x();
		base[4] = all_v.at(v2s).y();
		base[5] = all_v.at(v2s).z();

		base[6] = all_v.at(v3s).x();
		base[7] = all_v.at(v3s).y();
		base[8] = all_v.at(v3s).z();

		//====================================

		int nv1s = all_f.at(i).at(0).second;
		int nv2s = all_f.at(i).at(1).second;
		int nv3s = all_f.at(i).at(2).second;

		float * nvBase = all_data + ((all_f.size() + i) * 9);

		nvBase[0] = all_v.at(nv1s).x();
		nvBase[1] = all_v.at(nv1s).y();
		nvBase[2] = all_v.at(nv1s).z();

		nvBase[3] = all_v.at(nv2s).x();
		nvBase[4] = all_v.at(nv2s).y();
		nvBase[5] = all_v.at(nv2s).z();

		nvBase[6] = all_v.at(nv3s).x();
		nvBase[7] = all_v.at(nv3s).y();
		nvBase[8] = all_v.at(nv3s).z();
	}
}

int ObjLoader_ForShader_withNormal::getDataSize()
{
	return sizeof(float) * all_f.size() * 3 * 3 * 2;
}

int ObjLoader_ForShader_withNormal::getVertexCount()
{
	return all_f.size() * 3;
}
