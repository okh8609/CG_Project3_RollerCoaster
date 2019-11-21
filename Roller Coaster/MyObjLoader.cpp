#include "MyObjLoader.h"


MyObjLoader::MyObjLoader(const string &filePath, int size, QVector3D origin) : _filePath(filePath), _origin(origin)
{
	fstream file(filePath, ios::in);
	if (!file.is_open())
		throw std::runtime_error("Failed to open: " + filePath);

	QVector3D boundsMin(1e9, 1e9, 1e9);
	QVector3D boundsMax(-1e9, -1e9, -1e9);

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
			QVector3D p;
			float t = 0.0f;

			line_iss >> t; p.setX(t);
			boundsMin.setX(std::min(boundsMin.x(), t));
			boundsMax.setX(std::max(boundsMax.x(), t));
			line_iss >> t; p.setY(t);
			boundsMin.setY(std::min(boundsMin.y(), t));
			boundsMax.setY(std::max(boundsMax.y(), t));
			line_iss >> t; p.setZ(t);
			boundsMin.setZ(std::min(boundsMin.z(), t));
			boundsMax.setZ(std::max(boundsMax.z(), t));

			all_vertex.push_back(p);
		}
		else if (type == "f")
		{
			vector<int> surface;

			string vertexSerialNumber;
			while (line_iss) {
				line_iss >> vertexSerialNumber;
				int vertexSerialNumber2 = 0;
				istringstream v_iss(vertexSerialNumber);
				v_iss >> vertexSerialNumber2;
				surface.push_back(vertexSerialNumber2 > 0 ? vertexSerialNumber2 - 1 : all_vertex.size() + vertexSerialNumber2);
			}

			all_surface.push_back(surface);
		}
	}

	const QVector3D bounds = boundsMax - boundsMin;
	scale = size / maximum(bounds.x(), bounds.y(), bounds.z());
}

void MyObjLoader::render() const
{
	for (auto surface : all_surface)
	{
		glBegin(GL_POLYGON);

		if (surface.size() >= 3)
		{
			const auto & a = all_vertex.at(surface.at(0));
			const auto & b = all_vertex.at(surface.at(1));
			const auto & c = all_vertex.at(surface.at(2));

			auto n = QVector3D::normal(b - a, c - a);
			glNormal3d(n.x(), n.y(), n.z());
		}

		for (size_t i = 0; i < surface.size(); i++)
			glVertexVec3(all_vertex.at(surface.at(i)) * scale + _origin);
		glEnd();
	}
}