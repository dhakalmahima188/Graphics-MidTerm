////chalne
//#include "olcConsoleGameEngine.h"
//#include<fstream>
//#include<strstream>
//#include<algorithm> //for sorting
//using namespace std;
//
//struct vec3d {
//	float x = 0, y = 0, z = 0, w = 1;
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
//		m_sAppName = L"MID-TERM"; //unicode ko laagi
//
//	}
//private:
//	mesh meshCube;
//	mat4x4 matProj;
//	float fTheta;
//	vec3d vCamera;
//
//	vec3d Matrix_MultiplyVector(mat4x4& m, vec3d& i)
//	{
//		vec3d v;
//		v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
//		v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
//		v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
//		v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
//		return v;
//	}
//
//	mat4x4 Matrix_MakeIdentity()
//	{
//		mat4x4 matrix;
//		matrix.m[0][0] = 1.0f;
//		matrix.m[1][1] = 1.0f;
//		matrix.m[2][2] = 1.0f;
//		matrix.m[3][3] = 1.0f;
//		return matrix;
//	}
//
//	mat4x4 Matrix_MakeRotationX(float fAngleRad)
//	{
//		mat4x4 matrix;
//		matrix.m[0][0] = 1.0f;
//		matrix.m[1][1] = cosf(fAngleRad);
//		matrix.m[1][2] = sinf(fAngleRad);
//		matrix.m[2][1] = -sinf(fAngleRad);
//		matrix.m[2][2] = cosf(fAngleRad);
//		matrix.m[3][3] = 1.0f;
//		return matrix;
//	}
//
//	mat4x4 Matrix_MakeRotationY(float fAngleRad)
//	{
//		mat4x4 matrix;
//		matrix.m[0][0] = cosf(fAngleRad);
//		matrix.m[0][2] = sinf(fAngleRad);
//		matrix.m[2][0] = -sinf(fAngleRad);
//		matrix.m[1][1] = 1.0f;
//		matrix.m[2][2] = cosf(fAngleRad);
//		matrix.m[3][3] = 1.0f;
//		return matrix;
//	}
//
//	mat4x4 Matrix_MakeRotationZ(float fAngleRad)
//	{
//		mat4x4 matrix;
//		matrix.m[0][0] = cosf(fAngleRad);
//		matrix.m[0][1] = sinf(fAngleRad);
//		matrix.m[1][0] = -sinf(fAngleRad);
//		matrix.m[1][1] = cosf(fAngleRad);
//		matrix.m[2][2] = 1.0f;
//		matrix.m[3][3] = 1.0f;
//		return matrix;
//	}
//
//	mat4x4 Matrix_MakeTranslation(float x, float y, float z)
//	{
//		mat4x4 matrix;
//		matrix.m[0][0] = 1.0f;
//		matrix.m[1][1] = 1.0f;
//		matrix.m[2][2] = 1.0f;
//		matrix.m[3][3] = 1.0f;
//		matrix.m[3][0] = x;
//		matrix.m[3][1] = y;
//		matrix.m[3][2] = z;
//		return matrix;
//	}
//
//	mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
//	{
//		float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
//		mat4x4 matrix;
//		matrix.m[0][0] = fAspectRatio * fFovRad;
//		matrix.m[1][1] = fFovRad;
//		matrix.m[2][2] = fFar / (fFar - fNear);
//		matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
//		matrix.m[2][3] = 1.0f;
//		matrix.m[3][3] = 0.0f;
//		return matrix;
//	}
//
//	mat4x4 Matrix_MultiplyMatrix(mat4x4& m1, mat4x4& m2)
//	{
//		mat4x4 matrix;
//		for (int c = 0; c < 4; c++)
//			for (int r = 0; r < 4; r++)
//				matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
//		return matrix;
//	}
//
//	mat4x4 Matrix_PointAt(vec3d& pos, vec3d& target, vec3d& up)
//	{
//		// Calculate new forward direction
//		vec3d newForward = Vector_Sub(target, pos);
//		newForward = Vector_Normalise(newForward);
//
//		// Calculate new Up direction
//		vec3d a = Vector_Mul(newForward, Vector_DotProduct(up, newForward));
//		vec3d newUp = Vector_Sub(up, a);
//		newUp = Vector_Normalise(newUp);
//
//		// New Right direction is easy, its just cross product
//		vec3d newRight = Vector_CrossProduct(newUp, newForward);
//
//		// Construct Dimensioning and Translation Matrix	
//		mat4x4 matrix;
//		matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
//		matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
//		matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
//		matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
//		return matrix;
//
//	}
//
//	mat4x4 Matrix_QuickInverse(mat4x4& m) // Only for Rotation/Translation Matrices
//	{
//		mat4x4 matrix;
//		matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
//		matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
//		matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
//		matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
//		matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
//		matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
//		matrix.m[3][3] = 1.0f;
//		return matrix;
//	}
//
//	vec3d Vector_Add(vec3d& v1, vec3d& v2)
//	{
//		return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
//	}
//
//	vec3d Vector_Sub(vec3d& v1, vec3d& v2)
//	{
//		return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
//	}
//
//	vec3d Vector_Mul(vec3d& v1, float k)
//	{
//		return { v1.x * k, v1.y * k, v1.z * k };
//	}
//
//	vec3d Vector_Div(vec3d& v1, float k)
//	{
//		return { v1.x / k, v1.y / k, v1.z / k };
//	}
//
//	float Vector_DotProduct(vec3d& v1, vec3d& v2)
//	{
//		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
//	}
//
//	float Vector_Length(vec3d& v)
//	{
//		return sqrtf(Vector_DotProduct(v, v));
//	}
//
//	vec3d Vector_Normalise(vec3d& v)
//	{
//		float l = Vector_Length(v);
//		return { v.x / l, v.y / l, v.z / l };
//	}
//
//	vec3d Vector_CrossProduct(vec3d& v1, vec3d& v2)
//	{
//		vec3d v;
//		v.x = v1.y * v2.z - v1.z * v2.y;
//		v.y = v1.z * v2.x - v1.x * v2.z;
//		v.z = v1.x * v2.y - v1.y * v2.x;
//		return v;
//	}
//
//	vec3d Vector_IntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd)
//	{
//		plane_n = Vector_Normalise(plane_n);
//		float plane_d = -Vector_DotProduct(plane_n, plane_p);
//		float ad = Vector_DotProduct(lineStart, plane_n);
//		float bd = Vector_DotProduct(lineEnd, plane_n);
//		float t = (-plane_d - ad) / (bd - ad);
//		vec3d lineStartToEnd = Vector_Sub(lineEnd, lineStart);
//		vec3d lineToIntersect = Vector_Mul(lineStartToEnd, t);
//		return Vector_Add(lineStart, lineToIntersect);
//	}
//
//	int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle& in_tri, triangle& out_tri1, triangle& out_tri2)
//	{
//		// Make sure plane normal is indeed normal
//		plane_n = Vector_Normalise(plane_n);
//
//		// Return signed shortest distance from point to plane, plane normal must be normalised
//		auto dist = [&](vec3d& p)
//		{
//			vec3d n = Vector_Normalise(p);
//			return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vector_DotProduct(plane_n, plane_p));
//		};
//
//		// Create two temporary storage arrays to classify points either side of plane
//		// If distance sign is positive, point lies on "inside" of plane
//		vec3d* inside_points[3];  int nInsidePointCount = 0;
//		vec3d* outside_points[3]; int nOutsidePointCount = 0;
//
//		// Get signed distance of each point in triangle to plane
//		float d0 = dist(in_tri.p[0]);
//		float d1 = dist(in_tri.p[1]);
//		float d2 = dist(in_tri.p[2]);
//
//		if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[0]; }
//		else { outside_points[nOutsidePointCount++] = &in_tri.p[0]; }
//		if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[1]; }
//		else { outside_points[nOutsidePointCount++] = &in_tri.p[1]; }
//		if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[2]; }
//		else { outside_points[nOutsidePointCount++] = &in_tri.p[2]; }
//
//		// Now classify triangle points, and break the input triangle into 
//		// smaller output triangles if required. There are four possible
//		// outcomes...
//
//		if (nInsidePointCount == 0)
//		{
//			// All points lie on the outside of plane, so clip whole triangle
//			// It ceases to exist
//
//			return 0; // No returned triangles are valid
//		}
//
//		if (nInsidePointCount == 3)
//		{
//			// All points lie on the inside of plane, so do nothing
//			// and allow the triangle to simply pass through
//			out_tri1 = in_tri;
//
//			return 1; // Just the one returned original triangle is valid
//		}
//
//		if (nInsidePointCount == 1 && nOutsidePointCount == 2)
//		{
//			// Triangle should be clipped. As two points lie outside
//			// the plane, the triangle simply becomes a smaller triangle
//
//			// Copy appearance info to new triangle
//			out_tri1.col = in_tri.col;
//			out_tri1.sym = in_tri.sym;
//
//			// The inside point is valid, so keep that...
//			out_tri1.p[0] = *inside_points[0];
//
//			// but the two new points are at the locations where the 
//			// original sides of the triangle (lines) intersect with the plane
//			out_tri1.p[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
//			out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);
//
//			return 1; // Return the newly formed single triangle
//		}
//
//		if (nInsidePointCount == 2 && nOutsidePointCount == 1)
//		{
//			// Triangle should be clipped. As two points lie inside the plane,
//			// the clipped triangle becomes a "quad". Fortunately, we can
//			// represent a quad with two new triangles
//
//			// Copy appearance info to new triangles
//			out_tri1.col = in_tri.col;
//			out_tri1.sym = in_tri.sym;
//
//			out_tri2.col = in_tri.col;
//			out_tri2.sym = in_tri.sym;
//
//			// The first triangle consists of the two inside points and a new
//			// point determined by the location where one side of the triangle
//			// intersects with the plane
//			out_tri1.p[0] = *inside_points[0];
//			out_tri1.p[1] = *inside_points[1];
//			out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
//
//			// The second triangle is composed of one of he inside points, a
//			// new point determined by the intersection of the other side of the 
//			// triangle and the plane, and the newly created point above
//			out_tri2.p[0] = *inside_points[1];
//			out_tri2.p[1] = out_tri1.p[2];
//			out_tri2.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);
//
//			return 2; // Return two newly formed triangles which form a quad
//		}
//	}
//
//
//
//
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
//		// Projection Matrix
//		matProj = Matrix_MakeProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);
//		return true;
//
//
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
//
//
//		matRotZ = Matrix_MakeRotationZ(fTheta * 0.5f);
//		matRotX = Matrix_MakeRotationX(fTheta);
//
//
//		mat4x4 matTrans;
//
//		matTrans = Matrix_MakeTranslation(0.0f, 0.0f, 16.0f);
//
//		mat4x4 matWorld;
//
//
//		matWorld = Matrix_MakeIdentity();	// Form World Matrix
//		matWorld = Matrix_MultiplyMatrix(matRotZ, matRotX); // Transform by rotation
//		matWorld = Matrix_MultiplyMatrix(matWorld, matTrans); // Transform by translation
//
//
//		vector<triangle>vecTrianglesToRaster;
//
//		// Draw Triangles
//		for (auto tri : meshCube.tris)  //sabai cordinates ko laagi
//		{
//			triangle triProjected, triTransformed, triViewed;
//
//
//			// World Matrix Transform
//			triTransformed.p[0] = Matrix_MultiplyVector(matWorld, tri.p[0]);
//			triTransformed.p[1] = Matrix_MultiplyVector(matWorld, tri.p[1]);
//			triTransformed.p[2] = Matrix_MultiplyVector(matWorld, tri.p[2]);
//
//			// Calculate triangle Normal
//			vec3d normal, line1, line2;
//
//			// Get lines either side of triangle
//			line1 = Vector_Sub(triTransformed.p[1], triTransformed.p[0]);
//			line2 = Vector_Sub(triTransformed.p[2], triTransformed.p[0]);
//
//			// Take cross product of lines to get normal to triangle surface
//			normal = Vector_CrossProduct(line1, line2);
//
//			// You normally need to normalise a normal!
//			normal = Vector_Normalise(normal);
//			// Get Ray from triangle to camera
//			vec3d vCameraRay = Vector_Sub(triTransformed.p[0], vCamera);
//
//
//			/*		if (normal.z < 0) {*/
//					//surface dekhina lai normal ra hamro aakha ko sight opposite direction ma huna parxa
//
//			if (Vector_DotProduct(normal, vCameraRay) < 0.0f)
//			{
//				//lighting
//				vec3d light_direction = { 0.0f,0.0f,-1.0f };
//				float l = sqrtf(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
//				light_direction.x /= l; light_direction.y /= l; light_direction.z /= l;
//				float dp = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;
//				CHAR_INFO c = GetColour(dp);
//				triTransformed.col = c.Attributes;
//				triTransformed.sym = c.Char.UnicodeChar; //idk rato kina aairaxa
//
//
//
//
//			// Project triangles from 3D --> 2D
//			// Convert World Space --> View Space
//				triProjected.p[0] = Matrix_MultiplyVector(matProj, triTransformed.p[0]);
//				triProjected.p[1] = Matrix_MultiplyVector(matProj, triTransformed.p[1]);
//				triProjected.p[2] = Matrix_MultiplyVector(matProj, triTransformed.p[2]);
//				triProjected.sym = triTransformed.sym;
//				triProjected.col = triTransformed.col;
//
//				triProjected.p[0] = Vector_Div(triProjected.p[0], triProjected.p[0].w);
//				triProjected.p[1] = Vector_Div(triProjected.p[1], triProjected.p[1].w);
//				triProjected.p[2] = Vector_Div(triProjected.p[2], triProjected.p[2].w);
//				// Scale into view
//				//1 le shift center tira lera aunxa
//				vec3d vOffsetView = { 1,1,0 };
//				triProjected.p[0] = Vector_Add(triProjected.p[0], vOffsetView);
//				triProjected.p[1] = Vector_Add(triProjected.p[1], vOffsetView);
//				triProjected.p[2] = Vector_Add(triProjected.p[2], vOffsetView);
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