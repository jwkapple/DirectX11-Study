#include <iostream>
#include <fstream>
using namespace std;

typedef struct
{
	float x, y, z;
}VertexType;

typedef struct
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
}FaceType;

// Function Properties
void GetModelFilename(char*);
bool ReadFileCounts(char*, int&, int&, int&, int&);
bool LoadDataStructures(char*, int, int, int, int);

int main()
{
	bool result;
	char filename[256];
	int vertexCount, textureCount, faceCount, normalCount;
	char garbage;

	// Read in the name of the model file.
	GetModelFilename(filename);

	// Read in the number of vertices, tex coords,and faces so that the data structures can be intializxed with the exact sizes needed.
	result = ReadFileCounts(filename,vertexCount,textureCount,normalCount,faceCount);
	if (!result)
	{
		return -1;
	}

	// Display the counts to the screen for information purposes.
	cout << endl;
	cout << "Vertices:" << vertexCount << endl;
	cout << "UVs:" << textureCount << endl;
	cout << "Normals:" << normalCount << endl;
	cout << "Faces: " << faceCount << endl;

	// Now read the dtaa from the file into the data structures and then output it in our model format.
	result = LoadDataStructures(filename, vertexCount,textureCount, normalCount, faceCount);
	if (!result)
	{
		return -1;
	}

	// Notify the user the model has been converted.
	cout << "\nFile has been converted" << endl;
	cout << "\nDo you wish to exit (y/n)?" << endl;
	cin >> garbage;

	return 0;
}

void GetModelFilename(char* filename)
{
	bool done;
	ifstream fin;

	//Loop Until we have file name.
	done = false;
	while (!done)
	{
		// Ask the user for the filename.
		cout << "Enter model filename:";

		// Read in the filename.
		cin >> filename;

		// Attempt to open the file
		fin.open(filename);

		if (fin.good())
		{
			// if hte file exists and there are no problems then exit since we have the file name.
			done = true;
		}
		else
		{
			// If the file does not exist or there was an issue opening it then notify the user and repeat the process.
			fin.clear();
			cout << endl;
			cout << "File " << filename << " could not be opened" << endl << endl;
		}
	}
	
	return;
}

bool ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	ifstream fin;
	char input;

	// Initialize counts.
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		// If the line starts with 'v' then oucnt either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		if (input == 'I')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line;
		fin.get(input);
	}

	fin.close();

	return true;
}

bool LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	VertexType* vertices, *texcoords, *normals;
	FaceType *faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	// Initialize the four data structres.
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new VertexType[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new VertexType[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	fin.open(filename);
	if (fin.fail() == true)
	{
		return false;
	}

	// Read
	// Important : Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				// Invert Z vertex to change to left hand system.
				vertices[vertexCount].z = vertices[vertexCount].z * -1.0f;
				vertexIndex++;
			}

			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				// invert the V texture
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// invert the Z normal to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		while (input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);

	}

	fin.close();
	fout.close();
	fout.open("model.txt");

	fout << "Vertex Count: " << (faceCount * 3) << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;

	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			 << texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			 << normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;
	}

	fout.close();

	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}

	if (normals)
	{
		delete[] normals;
		normals = 0;
	}

	if (faces)
	{
		delete[] faces;
		faces = 0;
	}

	return true;
}