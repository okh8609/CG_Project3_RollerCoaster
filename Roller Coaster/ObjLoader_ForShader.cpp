#include "ObjLoader_ForShader.h"



ObjLoader_ForShader::ObjLoader_ForShader()
{
}

ObjLoader_ForShader::ObjLoader_ForShader(const string &filePath, int size) : _filePath(filePath), _size(size)
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
		else if (type == "f")
		{
			vector<int> f;

			string str;
			for (size_t i = 0; i < 3; i++)
			{
				int Serial;

				line_iss >> str;
				istringstream v_iss(str);

				v_iss >> Serial;
				f.push_back(Serial - 1);
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
	all_vertex = (float*)malloc(sizeof(float) * all_f.size() * 3 * 3);
	for (size_t i = 0; i < all_f.size(); ++i)
	{
		int v1s = all_f.at(i).at(0);
		int v2s = all_f.at(i).at(1);
		int v3s = all_f.at(i).at(2);

		float * base = all_vertex + i * 9;

		base[0] = all_v.at(v1s).x();
		base[1] = all_v.at(v1s).y();
		base[2] = all_v.at(v1s).z();

		base[3] = all_v.at(v2s).x();
		base[4] = all_v.at(v2s).y();
		base[5] = all_v.at(v2s).z();

		base[6] = all_v.at(v3s).x();
		base[7] = all_v.at(v3s).y();
		base[8] = all_v.at(v3s).z();
	}
}

int ObjLoader_ForShader::getVertexSize()
{
	return sizeof(float) * all_f.size() * 3 * 3;
}

int ObjLoader_ForShader::getVertexCount()
{
	return all_f.size() * 3;
}
