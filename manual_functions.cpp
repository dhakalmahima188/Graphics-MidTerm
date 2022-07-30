////nachalne
//#include "olcConsoleGameEngine.h"
//#include<fstream>
//#include<strstream>
//#include<algorithm> //for sorting
//using namespace std;
//
//struct vec3d {
//	float x, y, z;
//};
//struct triangle {
//	vec3d p[3];
//	wchar_t sym; //16-bit wide character used to store Unicode
//	short col;  //short is 16 bits (int)
//
//
//};
//struct mesh {
//	vector<triangle>tris;
//	bool LoadFromObjectFile(string sFilename)
//	{
//		ifstream f(sFilename);
//		if (!f.is_open())
//			return false;
//
//		// Local cache of verts
//		vector<vec3d> verts;
//
//		while (!f.eof())
//		{
//			char line[128];
//			f.getline(line, 128);
//
//			strstream s;
//			s << line;
//
//			char junk; // to store v, vn ,f
//
//			if (line[0] == 'v')
//			{
//				vec3d v;
//				s >> junk >> v.x >> v.y >> v.z;
//				verts.push_back(v);
//			}
//
//			if (line[0] == 'f')
//			{
//				int f[3];
//				s >> junk >> f[0] >> f[1] >> f[2];
//				tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
//			}
//		}
//
//		return true;
//	}
//
//};
//
//struct mat4x4 {
//	float m[4][4] = { 0 };
//};
//
//
//class olcEngine3D : public olcConsoleGameEngine {
//
//public:
//	olcEngine3D() {
//		m_sAppName = L"3D Demo"; //unicode ko laagi
//
//	}
//private:
//	mesh meshCube;
//	mat4x4 matProj;
//	float fTheta;
//	vec3d vCamera;
//
//	void MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m)
//	{
//		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
//		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
//		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
//		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];
//
//		if (w != 0.0f)
//		{
//			o.x /= w; o.y /= w; o.z /= w;
//		}
//	}
//	CHAR_INFO GetColour(float lum)
//	{
//		short bg_col, fg_col;
//		wchar_t sym;
//		int pixel_bw = (int)(13.0f * lum);
//		switch (pixel_bw)
//		{
//		case 0: bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID; break;
//
//		case 1: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_QUARTER; break;
//		case 2: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_HALF; break;
//		case 3: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_THREEQUARTERS; break;
//		case 4: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_SOLID; break;
//
//		case 5: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_QUARTER; break;
//		case 6: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_HALF; break;
//		case 7: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_THREEQUARTERS; break;
//		case 8: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_SOLID; break;
//
//		case 9:  bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_QUARTER; break;
//		case 10: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_HALF; break;
//		case 11: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_THREEQUARTERS; break;
//		case 12: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_SOLID; break;
//		default:
//			bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID;
//		}
//
//		CHAR_INFO c;
//		c.Attributes = bg_col | fg_col; //bitwise or : returns 1 if any is 1
//		c.Char.UnicodeChar = sym;
//		return c;
//	}
//
//public:
//	bool OnUserCreate() override
//	{
//
//		meshCube.LoadFromObjectFile("man.obj");
//
//		float fNear = 0.1f;
//		float fFar = 1000.0f;
//		float fFov = 90.0f; //field of view 90 degree;
//		float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
//		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f); //degrees to radins 0.5 le khhai kina multiply gaero
//
//		matProj.m[0][0] = fAspectRatio * fFovRad;
//		matProj.m[1][1] = fFovRad;
//		matProj.m[2][2] = fFar / (fFar - fNear);
//		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
//		matProj.m[2][3] = 1.0f;
//		matProj.m[3][3] = 0.0f;
//
//		return  true;
//
//
//
//	}
//	bool OnUserUpdate(float fElapsedTime) override
//	{
//		// Clear Screen
//		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
//
//		// Set up rotation matrices
//		mat4x4 matRotZ, matRotX;
//		fTheta += 1.0f * fElapsedTime;
//
//		// Rotation Z
//		matRotZ.m[0][0] = cosf(fTheta);
//		matRotZ.m[0][1] = sinf(fTheta);
//		matRotZ.m[1][0] = -sinf(fTheta);
//		matRotZ.m[1][1] = cosf(fTheta);
//		matRotZ.m[2][2] = 1;
//		matRotZ.m[3][3] = 1;
//
//		// Rotation X
//		matRotX.m[0][0] = 1;
//		matRotX.m[1][1] = cosf(fTheta * 0.5f);
//		matRotX.m[1][2] = sinf(fTheta * 0.5f);
//		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
//		matRotX.m[2][2] = cosf(fTheta * 0.5f);
//		matRotX.m[3][3] = 1;
//
//		vector<triangle>vecTrianglesToRaster;
//
//		// Draw Triangles
//		for (auto tri : meshCube.tris)  //sabai cordinates ko laagi
//		{
//			triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;
//
//			// Rotate in Z-Axis
//			MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
//			MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
//			MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);
//
//			// Rotate in X-Axis
//			MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
//			MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
//			MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);
//
//			// Offset into the screen
//			//3 le shift
//			//natra screen ma dekhinna
//			//paxadi lageko
//			triTranslated = triRotatedZX;
//			triTranslated.p[0].z = triRotatedZX.p[0].z + 8.0f;
//			triTranslated.p[1].z = triRotatedZX.p[1].z + 8.0f;
//			triTranslated.p[2].z = triRotatedZX.p[2].z + 8.0f;
//
//			//calculating normals
//			vec3d normal, line1, line2;
//			line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
//			line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
//			line1.z = triTranslated.p[1].z - triTranslated.p[0].z;
//
//			line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
//			line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
//			line2.z = triTranslated.p[2].z - triTranslated.p[0].z;
//
//			normal.x = line1.y * line2.z - line1.z * line2.y;
//			normal.y = line1.z * line2.x - line1.x * line2.z;
//			normal.z = line1.x * line2.y - line1.y * line2.x;
//
//			float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
//			normal.x /= l; normal.y /= l; normal.z /= l;
//
//
//			/*		if (normal.z < 0) {*/
//					//surface dekhina lai normal ra hamro aakha ko sight opposite direction ma huna parxa
//
//			if (normal.x * (triTranslated.p[0].x - vCamera.x) +
//				normal.y * (triTranslated.p[0].y - vCamera.y) +
//				normal.z * (triTranslated.p[0].z - vCamera.z) < 0.0f)
//			{
//				//lighting
//				vec3d light_direction = { 0.0f,0.0f,-1.0f };
//				float l = sqrtf(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
//				light_direction.x /= l; light_direction.y /= l; light_direction.z /= l;
//				float dp = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;
//				CHAR_INFO c = GetColour(dp);
//				triTranslated.col = c.Attributes;
//				triTranslated.sym = c.Char.UnicodeChar;//idk rato kina aairaxa
//
//
//
//
//			// Project triangles from 3D --> 2D
//				MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
//				MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
//				MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);
//				triProjected.col = triTranslated.col;
//				triProjected.sym = triTranslated.sym;
//
//				// Scale into view
//				//1 le shift center tira lera aunxa
//				triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
//				triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
//				triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
//
//				triProjected.p[0].x *= 0.5f * (float)ScreenWidth();
//				triProjected.p[0].y *= 0.5f * (float)ScreenHeight();
//				triProjected.p[1].x *= 0.5f * (float)ScreenWidth();
//				triProjected.p[1].y *= 0.5f * (float)ScreenHeight();
//				triProjected.p[2].x *= 0.5f * (float)ScreenWidth();
//				triProjected.p[2].y *= 0.5f * (float)ScreenHeight();
//
//				// Rasterize triangle
//				//DrawTrianngle, FillTriangle
//				//store triangle for soorting
//				vecTrianglesToRaster.push_back(triProjected);
//
//			}
//
//			/*	DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
//				triProjected.p[1].x, triProjected.p[1].y,
//				triProjected.p[2].x, triProjected.p[2].y,
//				PIXEL_SOLID, FG_BLACK);*/
//
//		}
//		//sort from back to front
//
//		sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end()
//			, [](triangle& t1, triangle& t2) {
//
//				float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
//				float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
//				return z1 > z2;
//
//			});
//
//		for (auto& triProjected : vecTrianglesToRaster)
//		{
//			// Rasterize triangle
//			FillTriangle(triProjected.p[0].x, triProjected.p[0].y,
//				triProjected.p[1].x, triProjected.p[1].y,
//				triProjected.p[2].x, triProjected.p[2].y,
//				triProjected.sym, triProjected.col);
//
//			/*DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
//			triProjected.p[1].x, triProjected.p[1].y,
//			triProjected.p[2].x, triProjected.p[2].y,
//			PIXEL_SOLID, FG_BLACK);*/
//		}
//
//
//
//		return true;
//	}
//
//};
//
//int main() {
//	olcEngine3D demo;
//	if (demo.ConstructConsole(256, 240, 3, 3)) {
//		demo.Start();
//	}
//	return 0;
//}