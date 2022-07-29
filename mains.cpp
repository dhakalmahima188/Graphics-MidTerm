#include "olcConsoleGameEngine.h"
using namespace std;
struct vec3d {
	float x, y, z;
};
struct triangle {
	vec3d p[3];

}; 
struct mesh {
	vector<triangle>tris;
};



using namespace std;
 
class olcEngine3D : public olcConsoleGameEngine {
private:
	mesh meshCube;
public:
	olcEngine3D() {
		m_sAppName = L"3D Demo"; //unicode ko laagi

	}
public:
	bool OnUserCreate() override
	{
		meshCube.tris = {

		};
	}
	bool OnUserUpdate(float fElapsedTime) override {

		return true;

	}
};
int main(){
	olcEngine3D demo;
	if (demo.ConstructConsole(256, 240, 4, 4)) {
		demo.Start();
	}
	return 0;
	}