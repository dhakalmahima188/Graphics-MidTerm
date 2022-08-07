
#include "graphics.h"
#pragma comment(lib, "graphics.lib")
#include< sstream>
#include<math.h>
#include<vector>
#include<algorithm>
#include <iostream>
#include "2D.h"
using namespace std;

float CAM_DIR[3] = { 0,0,-1 };

float CAM_POS_PRP[3] = { 0,0,500 };
float CAM_POS_VP[3] = { 0,0,150 };

float LIGHT[3] = { 200,-200,200};



//------------3D-----------

//for indices matrix in 3d
struct Matpxn {
	float** m;
	Matpxn(int numberOfVertices, int pointsPerFace = 3) {
		m = new float* [numberOfVertices];
		for (int i = 0; i < numberOfVertices; i++) {
			m[i] = new float[pointsPerFace]();
		}
	}
};

//for transformation matrix in 3d
struct Mat4xn {
	float** m;
	Mat4xn(int n = 1, int points = 0) {
		m = new float* [4];
		for (int i = 0; i < 4; i++) {
			m[i] = new float[n]();
		}
		if (n >= 2)
			m[3][3] = 1;
		if (points == 1)
		{
			for (int i = 0; i < n; i++)
			{
				m[3][i] = 1;
			}
		}
	}
};

//fro 3d combining of transformations
Mat4xn combineTransformation(Mat4xn T, Mat4xn P, int n) {
	Mat4xn R(n, 1);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < n; j++) {
			R.m[i][j] = 0;
			for (int k = 0; k < 3; k++) {
				R.m[i][j] += T.m[i][k] * P.m[k][j];
			}
		}
	}
	return R;
}

//for 3d final multang
Mat4xn matMult3d(Mat4xn T, Mat4xn P, int n, int combineTransform = 0) {
	Mat4xn R(n, 0);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < n; j++) {
			R.m[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				R.m[i][j] += T.m[i][k] * P.m[k][j];
			}
		}
	}
	return R;
}

struct Vertex {
	float x, y, z;
};

struct Triangle {
	Vertex vertex[3];
	float intense;
	Triangle(Mat4xn P, float intensity) {
		vertex[0].x = P.m[0][0];
		vertex[0].y = P.m[1][0];
		vertex[0].z = P.m[2][0];
		vertex[1].x = P.m[0][1];
		vertex[1].y = P.m[1][1];
		vertex[1].z = P.m[2][1];
		vertex[2].x = P.m[0][2];
		vertex[2].y = P.m[1][2];
		vertex[2].z = P.m[2][2];
		intense = intensity;
	}
};

//left to implement
//int painter(int x0, int y0, int z0, int x1, int y1, int z1) {
//	point3d temp;
//	temp.x1 = x0;
//	temp.y1 = y0;
//	temp.z1 = z0;
//	temp.x2 = x1;
//	temp.y2 = y1;
//	temp.z2 = z1;
//
//	globalBuffer.push_back(temp);
//
//	return 0;
//}

Mat4xn calcNormal(Mat4xn V, Matpxn I, int faces) {
	Mat4xn N(faces);
	cout << " normal data ==================" << endl << endl;
	for (int i = 0; i < faces; i++) {
		int i1 = I.m[i][0];
		int i2 = I.m[i][1];
		int i3 = I.m[i][2];
		float U[3] = { V.m[0][i1] - V.m[0][i2],V.m[1][i1] - V.m[1][i2] ,V.m[2][i1] - V.m[2][i2] };
		float W[3] = { V.m[0][i1] - V.m[0][i3],V.m[1][i1] - V.m[1][i3] ,V.m[2][i1] - V.m[2][i3] };
		N.m[0][i] = -U[1] * W[2] + U[2] * W[1];
		N.m[1][i] = -U[2] * W[0] + U[0] * W[2];
		N.m[2][i] = -U[0] * W[1] + U[1] * W[0];
		float mag = sqrtf(N.m[0][i] * N.m[0][i] + N.m[1][i] * N.m[1][i] + N.m[2][i] * N.m[2][i]);
		N.m[0][i] /= mag;
		N.m[1][i] /= mag;
		N.m[2][i] /= mag;
		//cout << "face normals: " << N.m[0][i] << " , " << N.m[1][i] << " , " << N.m[2][i] << endl;
	}
	cout << "normal data end===============" << endl << endl;
	return N;
}

int defTriad[3] = { 0,1,2 };

//float calIllum(Mat4xn V, Matpxn I, int faceNo, int triad[] = defTriad) {
//	float centroidX = 0;
//	float centroidY = 0;
//	float centroidZ = 0;
//	int i1 = I.m[faceNo][triad[0]];
//	int i2 = I.m[faceNo][triad[1]];
//	int i3 = I.m[faceNo][triad[2]];
//	P.m[0][0] = V.m[0][i1];
//	P.m[1][0] = V.m[1][i1];
//	P.m[2][0] = V.m[2][i1];
//	P.m[0][1] = V.m[0][i2];
//	P.m[1][1] = V.m[1][i2];
//	P.m[2][1] = V.m[2][i2];
//	P.m[0][2] = V.m[0][i3];
//	P.m[1][2] = V.m[1][i3];
//	P.m[2][2] = V.m[2][i3];
//	for (size_t i = 0; i < 3; i++) {
//		centroidX += P.m[0][i];
//		centroidY += P.m[1][i];
//		centroidZ += P.m[2][i];
//	}
//	centroidX /= 3.0f;
//	centroidY /= 3.0f;
//	centroidZ /= 3.0f;
//	float U[3] = { centroidX - LIGHT[0],centroidX - LIGHT[0],centroidX - LIGHT[0] };
//	float mag = sqrtf(U[0] * U[0] + U[1] * U[1] + U[2] * U[2]);
//	U[0] /= mag;
//	U[1] /= mag;
//	U[2] /= mag;
//	float dotVal = U[0] * U[0] + U[1] * LIGHT[1] + U[2] * LIGHT[2];
//	cout << "light: " << dotVal << endl;
//	return dotVal;
//}
float calcBack(Mat4xn P, float N[3]) {
	float centroidX = 0;
	float centroidY = 0;
	float centroidZ = 0;

	for (size_t i = 0; i < 3; i++) {
		centroidX += P.m[0][i];
		centroidY += P.m[1][i];
		centroidZ += P.m[2][i];
	}
	centroidX /= 3.0f;
	centroidY /= 3.0f;
	centroidZ /= 3.0f;
	float U[3] = { centroidX - CAM_POS_PRP[0], centroidY - CAM_POS_PRP[1], centroidZ - CAM_POS_PRP[2] };
	float mag = sqrtf(U[0] * U[0] + U[1] * U[1] + U[2] * U[2]);
	U[0] /= mag;
	U[1] /= mag;
	U[2] /= mag;
	/*for (size_t i = 0; i < 3; i++)
		cout << "triad :  "<< i<< " " << P.m[0][i] << " , " << P.m[1][i] << " , " << P.m[1][i] << endl;

	cout << "centroid:  " << centroidX<< " , " << centroidY << " , " << centroidZ << endl;
	cout << "camera normal:  " << U[0] << " , " << U[1] << " , " << U[2] << endl;*/
	float dotVal = U[0] * N[0] + U[1] * N[1] + U[2] * N[2];


	float fovCheck = U[0] * CAM_DIR[0] + U[1] * CAM_DIR[1] + U[2] * CAM_DIR[2];
	if (abs(acosf(fovCheck)) >= (3.1415f)) {
		dotVal = 1;
	}

	/*cout << "centroid:  " << centroidX << " , " << centroidY << " , " << centroidZ << endl;
	cout << "camera normal:  " << U[0] << " , " << U[1] << " , " << U[2] << endl;
	cout << "object normal:  " << N[0] << " , " << N[1] << " , " << N[2] << endl;

	cout << " backface value : " << dotVal << endl;*/
	return dotVal;
}


float calcIllum(Mat4xn P, float N[3]) {
	float centroidX = 0;
	float centroidY = 0;
	float centroidZ = 0;

	for (size_t i = 0; i < 3; i++) {
		centroidX += P.m[0][i];
		centroidY += P.m[1][i];
		centroidZ += P.m[2][i];
	}
	centroidX /= 3.0f;
	centroidY /= 3.0f;
	centroidZ /= 3.0f;
	float U[3] = { centroidX - LIGHT[0],centroidY - LIGHT[1],centroidZ - LIGHT[2] };
	float mag = sqrtf(U[0] * U[0] + U[1] * U[1] + U[2] * U[2]);
	U[0] /= mag;
	U[1] /= mag;
	U[2] /= mag;
	float diffused = (U[0] * N[0] + U[1] * N[1] + U[2] * N[2]) * .8f;
	diffused = min(diffused, 0);
	////spec
	//float reflect_dir = normal * (normal * 2.f).dot(-light_direction) + light_direction;
	float reflect_dir[3];
	float dot = (N[0] * 2.f) * (-U[0]) + (N[1] * 2.f) * (-U[1]) + (N[2] * 2.f) * (-U[2]);
	reflect_dir[0] = N[0] * dot + U[0];
	reflect_dir[1] = N[1] * dot + U[1];
	reflect_dir[2] = N[2] * dot + U[2];

	//V3 view_dir = look_dir;

	//V3 view_dir = (triangle_center - prp).normal()
	float view_dir[3] = { centroidX - CAM_POS_PRP[0],centroidY - CAM_POS_PRP[1],centroidZ - CAM_POS_PRP[2] };
	mag = sqrtf(view_dir[0] * view_dir[0] + view_dir[1] * view_dir[1] + view_dir[2] * view_dir[2]);
	view_dir[0] /= mag;
	view_dir[1] /= mag;
	view_dir[2] /= mag;

	//r32 spec = powf(MAX(view_dir.dot(reflect_dir), 0.0), shininess);
	float viewDotRef = view_dir[0] * U[0] + view_dir[1] * U[1] + view_dir[2] * U[2];
	float spec = powf(max(viewDotRef, 0.0), 2);

	float ambient = -.05f;
	diffused *= .75f;
	spec *= -.9;
	float dotVal = ambient + diffused + spec;
	dotVal = max(dotVal, -1.0f);
	cout << "  ambient: " << ambient << endl;
	cout << "  diffused:  " << diffused << endl;
	cout << "  spec:    " << spec << endl;
	cout << "+++++++++++++++ LIGHT: " << dotVal << endl;
	return dotVal;
}
//for flood fill
void fill_triangle(Mat4xn P3, int color)
{
	if (P3.m[1][0] == P3.m[1][1] && P3.m[1][0] == P3.m[1][2]) return; // dont care about degenerate triangles
	if (P3.m[1][0] > P3.m[1][1]) {
		swap(P3.m[0][0], P3.m[0][1]);
		swap(P3.m[1][0], P3.m[1][1]);
	}
	if (P3.m[1][0] > P3.m[1][2]) {
		swap(P3.m[1][0], P3.m[1][2]);
		swap(P3.m[0][0], P3.m[0][2]);
	}
	if (P3.m[1][1] > P3.m[1][2]) {
		swap(P3.m[1][1], P3.m[1][2]);
		swap(P3.m[0][1], P3.m[0][2]);
	}
	int total_height = int(P3.m[1][2] - P3.m[1][0]);

	for (int i = 0; i < total_height; i++) {
		bool second_half = i > P3.m[1][1] - P3.m[1][0] || P3.m[1][1] == P3.m[1][0];
		int segment_height = (int)(second_half ? P3.m[1][2] - P3.m[1][1] : P3.m[1][1] - P3.m[1][0]);
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? P3.m[1][1] - P3.m[1][0] : 0)) / segment_height; // be careful: with above conditions no division by zero here
		float ax = P3.m[0][0] + (P3.m[0][2] - P3.m[0][0]) * alpha;
		float ay = P3.m[1][0] + (P3.m[1][2] - P3.m[1][0]) * alpha;
		float bx = second_half ? P3.m[0][1] + (P3.m[0][2] - P3.m[0][1]) * beta : P3.m[0][0] + (P3.m[0][1] - P3.m[0][0]) * beta;
		float by = second_half ? P3.m[1][1] + (P3.m[1][2] - P3.m[1][1]) * beta : P3.m[1][0] + (P3.m[1][1] - P3.m[1][0]) * beta;
		if (ax > bx) {
			swap(ax, bx);
			swap(ay, by);
		}
		for (int j = (int)ax; j <= bx; j++) {
			putpixel(j + GLOBAL_OFFSET_XY[0], (int)P3.m[1][0] + i + GLOBAL_OFFSET_XY[1], color); // attention, due to int casts t0.y + i != a.y
		}
	}
}

//void fillTriangle(Mat4xn P, int intense) {
//	int points[6] = { 0 };
//	points[0] = P.m[0][0] + GLOBAL_OFFSET_XY[0];
//	points[1] = P.m[1][0] + GLOBAL_OFFSET_XY[1];
//	points[2] = P.m[0][1] + GLOBAL_OFFSET_XY[0];
//	points[3] = P.m[1][1] + GLOBAL_OFFSET_XY[1];
//	points[4] = P.m[0][2] + GLOBAL_OFFSET_XY[0];
//	points[5] = P.m[1][2] + GLOBAL_OFFSET_XY[1];
//	setcolor(COLOR(intense, intense, intense));
//	setfillstyle(1, COLOR(intense, intense, intense));
//	fillpoly(3, points);
//}
void fillTriangle(Vertex V[3], int intense) {
	int points[6] = { 0 };
	points[0] = V[0].x + GLOBAL_OFFSET_XY[0];
	points[1] = V[0].y + GLOBAL_OFFSET_XY[1];
	points[2] = V[1].x + GLOBAL_OFFSET_XY[0];
	points[3] = V[1].y + GLOBAL_OFFSET_XY[1];
	points[4] = V[2].x + GLOBAL_OFFSET_XY[0];
	points[5] = V[2].y + GLOBAL_OFFSET_XY[1];
	setcolor(COLOR(intense, intense, intense));
	setfillstyle(1, COLOR(intense, intense, intense));
	fillpoly(3, points);
}

Mat4xn calcProjection(Mat4xn P) {

	for (size_t i = 0; i < 3; i++) {
		P.m[0][i] = P.m[0][i] * (CAM_POS_PRP[2] - CAM_POS_VP[2]) / (CAM_POS_PRP[2] - P.m[2][i]) - CAM_POS_PRP[0] * (CAM_POS_VP[2] - P.m[2][i]) / (CAM_POS_PRP[2] - P.m[2][i]);
		P.m[1][i] = P.m[1][i] * (CAM_POS_PRP[2] - CAM_POS_VP[2]) / (CAM_POS_PRP[2] - P.m[2][i]) - CAM_POS_PRP[1] * (CAM_POS_VP[2] - P.m[2][i]) / (CAM_POS_PRP[2] - P.m[2][i]);
	}
	return P;
}

void Draw3dSimple(Mat4xn V, Matpxn I, int faces, int pointsPerFace, int color = CYAN, Mat4xn N = NULL) {
	float dotVal = 0;
	/*Mat4xn N1(faces);
	N1 = calcNormal(V, I, faces);*/
	vector<Triangle> renderQueue;
	Mat4xn P(3);

	for (int j = 0; j < faces; j++) {
		/*cout << endl << "========= FACE NO: ========== " << j << endl;*/
		int s = 0;
		int s2 = 0;
		int f = I.m[j][0];



		for (int i = 0; i < pointsPerFace - 1; i++) {
			s = I.m[j][i];
			s2 = I.m[j][i + 1];
			BLA(V.m[0][s], V.m[1][s], V.m[0][s2], V.m[1][s2], color);
		}
		BLA(V.m[0][s2], V.m[1][s2], V.m[0][f], V.m[1][f], color);
	}


}
//For 3d render of object using vertex and index matrix
void Draw3d(Mat4xn V, Matpxn I, int faces, int pointsPerFace, int color = CYAN, Mat4xn N = NULL) {
	float dotVal = 0;
	/*Mat4xn N1(faces);
	N1 = calcNormal(V, I, faces);*/
	vector<Triangle> renderQueue;
	Mat4xn P(3);

	for (int j = 0; j < faces; j++) {
		/*cout << endl << "========= FACE NO: ========== " << j << endl;*/
		int s = 0;
		int s2 = 0;
		int f = I.m[j][0];
		int i1 = I.m[j][0];
		int i2 = I.m[j][1];
		int i3 = I.m[j][2];
		P.m[0][0] = V.m[0][i1];
		P.m[1][0] = V.m[1][i1];
		P.m[2][0] = V.m[2][i1];
		P.m[0][1] = V.m[0][i2];
		P.m[1][1] = V.m[1][i2];
		P.m[2][1] = V.m[2][i2];
		P.m[0][2] = V.m[0][i3];
		P.m[1][2] = V.m[1][i3];
		P.m[2][2] = V.m[2][i3];
		float triNormal[3] = { N.m[0][j] ,N.m[1][j] ,N.m[2][j] };
		dotVal = calcBack(P, triNormal);

		//uncomment for backface culling
		if (dotVal > 0)
		{
			cout << endl << "Back face detected with vertices: " << endl;
			for (size_t i = 0; i < 4; i++)
			{
				int index = I.m[j][i];
				cout << "( " << V.m[0][index] << " , " << V.m[1][index] << " , " << V.m[2][index] << " ) " << endl;

			}
			continue;
		}

		if (dotVal < 0) {
			//dotVal = projectedNormal.m[0][j] * LIGHT[0] + projectedNormal.m[1][j] * LIGHT[1] + projectedNormal.m[2][j] * LIGHT[2];
			int inten = -calcIllum(P, triNormal) * 255;
			P = calcProjection(P);

			Triangle T(P, inten);

			renderQueue.emplace_back(T);
		}
		i1 = I.m[j][2];
		i2 = I.m[j][3];
		i3 = I.m[j][0];

		P.m[0][0] = V.m[0][i1];
		P.m[1][0] = V.m[1][i1];
		P.m[2][0] = V.m[2][i1];
		P.m[0][1] = V.m[0][i2];
		P.m[1][1] = V.m[1][i2];
		P.m[2][1] = V.m[2][i2];
		P.m[0][2] = V.m[0][i3];
		P.m[1][2] = V.m[1][i3];
		P.m[2][2] = V.m[2][i3];
		for (size_t k = 0; k < 3; k++)
			triNormal[k] = N.m[k][j];
		dotVal = calcBack(P, triNormal);
		if (dotVal < 0) {
			//dotVal = projectedNormal.m[0][j] * LIGHT[0] + projectedNormal.m[1][j] * LIGHT[1] + projectedNormal.m[2][j] * LIGHT[2];
			int inten = -calcIllum(P, triNormal) * 255;

			P = calcProjection(P);
			Triangle T(P, inten);
			renderQueue.emplace_back(T);
		}
		//fillTriangle(P, inten);
		//fill_triangle(P, color);

		/*for (int i = 0; i < pointsPerFace - 1; i++) {
			s = I.m[j][i];
			s2 = I.m[j][i + 1];
			BLA(V.m[0][s], V.m[1][s], V.m[0][s2], V.m[1][s2], color);
		}
		BLA(V.m[0][s2], V.m[1][s2], V.m[0][f], V.m[1][f], color);*/
	}

	//painter
	std::sort(renderQueue.begin(), renderQueue.end(), [](Triangle& t1, Triangle& t2) {
		float midZ1 = (t1.vertex[0].z + t1.vertex[1].z + t1.vertex[2].z) / 3.0f;
		float midZ2 = (t2.vertex[0].z + t2.vertex[1].z + t2.vertex[2].z) / 3.0f;
		return midZ1 < midZ2;
		});

	for (size_t i = 0; i < renderQueue.size(); i++) {
		fillTriangle(renderQueue[i].vertex, renderQueue[i].intense);
	}


}

// how to do as per sirs need
void projectPers(Mat4xn V, Matpxn I, int faces, int pointsPerFace, int noOfVertices, int color, float xprp, float yprp, float zprp, float zvp) {
	//normal calc loop
	Mat4xn N(faces);
	N = calcNormal(V, I, faces);
	/*for (size_t i = 0; i < noOfVertices; i++) {
		V.m[0][i] = V.m[0][i] * (zprp - zvp) / (zprp - V.m[2][i]) - xprp * (zvp - V.m[2][i]) / (zprp - V.m[2][i]);
		V.m[1][i] = V.m[1][i] * (zprp - zvp) / (zprp - V.m[2][i]) - yprp * (zvp - V.m[2][i]) / (zprp - V.m[2][i]);
	}*/
	Draw3d(V, I, faces, pointsPerFace, color, N);
}


//just do nothing
void projectOrtho(Mat4xn V, Matpxn I, int faces, int pointsPerFace, int color = CYAN) {

	Draw3dSimple(V, I, faces, pointsPerFace, color, NULL);

}
//works but idk why
void projectOblique(Mat4xn V, Matpxn I, int faces, int pointsPerFace, int noOfVertices, float zvp = 0, float alpha = 30, float phi = 30, int color = CYAN) {
	phi = phi / 180 * 3.1415f;
	alpha = alpha / 180 * 3.1415f;
	Mat4xn T(4);
	T.m[0][0] = 1;
	T.m[0][2] = 1 * (1.0f / tanf(alpha)) * cosf(phi);
	T.m[0][2] = -1 * zvp * (1.0f / tanf(alpha)) * cosf(phi);
	T.m[1][1] = 1;
	T.m[1][2] = 1 * (1.0f / tanf(alpha)) * sinf(phi);
	T.m[1][2] = -1 * zvp * (1.0f / tanf(alpha)) * sinf(phi);
	V = matMult3d(T, V, noOfVertices);

	Draw3dSimple(V, I, faces, pointsPerFace, color, NULL);
}

//void projectPerspective(Mat4xn V, Matpxn I, int faces, int pointsPerFace, int noOfVertices, int color, float fov, float aspect, float near_plane, float far_plane)
//{
//	float fov_rad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.1415f);
//	Mat4xn T(4);
//	T.m[0][0] = fov_rad;
//	T.m[1][1] = aspect * fov_rad;
//	T.m[2][2] = far_plane / (far_plane - near_plane);
//	T.m[3][2] = (-far_plane * near_plane) / (far_plane - near_plane);
//	T.m[2][3] = 1.0f;
//	T.m[3][3] = 0.0f;
//	V = matMult3d(T, V, noOfVertices);
//	Draw3d(V, I, faces, pointsPerFace, color);
//}



//3d trasnformations
Mat4xn Translate3d(Mat4xn polygonMat, int noOfVertices, float x = 0, float y = 0, float z = 0, int ReturnTransformMat = 0) {
	Mat4xn T(4);
	T.m[0][3] = x;
	T.m[1][3] = y;
	T.m[2][3] = z;
	for (int i = 0; i < 4; i++)
		T.m[i][i] = 1;
	if (ReturnTransformMat == 1)
		return T; //return transformation matrix

	Mat4xn R(noOfVertices, 1);
	R = matMult3d(T, polygonMat, noOfVertices);

	return R;
}

Mat4xn Scale3d(Mat4xn polygonMat, int noOfVertices, float sx = 0, float sy = 0, float sz = 0, int ReturnTransformMat = 0) {
	Mat4xn T(4);
	T.m[0][0] = sx;
	T.m[1][1] = sy;
	T.m[2][2] = sz;
	T.m[3][3] = 1;
	if (ReturnTransformMat == 1)
		return T; //return transformation matrix
	Mat4xn R(noOfVertices, 1);
	R = matMult3d(T, polygonMat, noOfVertices);
	return R;
}

Mat4xn RotateZ3d(Mat4xn polygonMat, int order, float theta, float x = 0, float y = 0, int ReturnTransformMat = 0) {
	Mat4xn T(4);
	theta = theta / 180 * 3.1415f;
	T.m[0][0] = T.m[1][1] = cos(theta);
	T.m[0][1] = -sin(theta);
	T.m[1][0] = sin(theta);
	T.m[3][3] = T.m[2][2] = 1;
	if (ReturnTransformMat == 1)
		return T;
	T = matMult3d(T, Translate3d(NULL, 0, -x, -y, 0, 1), 4);
	T = matMult3d(Translate3d(NULL, 0, x, y, 0, 1), T, 4);
	//T= matMult3d(T,Translate3d(NULL,0,-x,-y,-z))
	Mat4xn R(order, 1);
	R = matMult3d(T, polygonMat, order);
	return R;
}

Mat4xn RotateX3d(Mat4xn polygonMat, int order, float theta, float y = 0, float z = 0, int ReturnTransformMat = 0) {
	Mat4xn T(4);
	theta = theta / 180 * 3.1415f;
	T.m[2][2] = T.m[1][1] = cos(theta);
	T.m[2][1] = -sin(theta);
	T.m[1][2] = sin(theta);
	T.m[3][3] = T.m[0][0] = 1;
	if (ReturnTransformMat == 1)
		return T;
	T = matMult3d(T, Translate3d(NULL, 0, 0, -y, -z, 1), 4);
	T = matMult3d(Translate3d(NULL, 0, 0, y, z, 1), T, 4);
	//T= matMult3d(T,Translate3d(NULL,0,-x,-y,-z))
	Mat4xn R(order, 1);
	R = matMult3d(T, polygonMat, order);
	return R;
}

Mat4xn RotateY3d(Mat4xn polygonMat, int order, float theta, float x = 0, float z = 0, int ReturnTransformMat = 0) {
	Mat4xn T(4);
	theta = theta / 180 * 3.1415f;
	T.m[2][2] = T.m[0][0] = cos(theta);
	T.m[2][0] = -sin(theta);
	T.m[0][2] = sin(theta);
	T.m[3][3] = T.m[1][1] = 1;
	if (ReturnTransformMat == 1)
		return T;
	T = matMult3d(T, Translate3d(NULL, 0, -x, 0, -z, 1), 4);
	T = matMult3d(Translate3d(NULL, 0, x, 0, z, 1), T, 4);
	//T= matMult3d(T,Translate3d(NULL,0,-x,-y,-z))
	Mat4xn R(order, 1);
	R = matMult3d(T, polygonMat, order);
	return R;
}

void tranlatesOrigin(int offsetX = 100, int offsetY = 100) {
	GLOBAL_OFFSET_XY[0] = offsetX;
	GLOBAL_OFFSET_XY[1] = offsetY;
	cout << "Origin Shifted to: " << offsetX << ", " << offsetY << endl;
}
void drawAxis(int endpointx = 500, int endpointy = 500) {
	BLA(-GLOBAL_OFFSET_XY[0], 0, endpointx, 0, RED);

	BLA(0, -GLOBAL_OFFSET_XY[1], 0, endpointy, RED);
}

int main()
{
	int gd = DETECT, gm, color;
	//initgraph(&gd, &gm, NULL);
	initwindow(WINDOW_SIZE[0], WINDOW_SIZE[1]);
	setbkcolor(BLACK);
	int x;
	int y;
	int z;
	int	vertices = 8;
	int faces = 6;
	int pointsPerFace = 4;
	stringstream VertexBuffer("0 0 0 0 1 0 1 1 0 1 0 0 0 0 1 0 1 1 1 1 1 1 0 1");
	stringstream indexBuffer("1 4 3 2 1 2 6 5 5 6 7 8 3 4 8 7 1 5 8 4 2 3 7 6");
	//stringstream normalBuffer("0 0 1 -1 0 0 0 0 -1 1 0 0 0 1 0 0 -1 0");

	Mat4xn vertexMatrix(vertices, 1);
	for (int i = 0; i < vertices; i++) {
		VertexBuffer >> x >> y >> z;
		vertexMatrix.m[0][i] = x;
		vertexMatrix.m[1][i] = y;
		vertexMatrix.m[2][i] = z;
		vertexMatrix.m[3][i] = 1;
	};

	Matpxn indexMatrix(vertices, pointsPerFace);
	for (size_t j = 0; j < faces; j++) {
		for (int i = 0; i < pointsPerFace; i++) {
			indexBuffer >> x;
			indexMatrix.m[j][i] = x - 1;
		};
	}

	Mat4xn Q(vertices, 1);

	Q = Scale3d(vertexMatrix, vertices, 250, 250, 250);
	Q = Translate3d(Q, vertices, -150, -100, -125);
	Q = RotateX3d(Q, vertices, 30, 0, 0);
	Q = RotateY3d(Q, vertices, 60, 0, 0);


	cout << "The transformation is as follows: " << endl;
	for (size_t i = 0; i < vertices; i++)
	{
		cout << " (" << vertexMatrix.m[0][i] << "," << vertexMatrix.m[1][i] << "," << vertexMatrix.m[2][i] << ")  to: ";
		cout << " (" << Q.m[0][i] << "," << Q.m[1][i] << "," << Q.m[2][i] << ")" << endl;
	}


	tranlatesOrigin(WINDOW_SIZE[0] / 2, WINDOW_SIZE[1] / 2);
	drawAxis(WINDOW_SIZE[0] / 2, WINDOW_SIZE[1] / 2);

	//projectOrtho(Q, indexMatrix, faces, pointsPerFace, RED);
	//projectOblique(Q, indexMatrix, faces, pointsPerFace, vertices, 80, 89, 20, WHITE);
	projectPers(Q, indexMatrix, faces, pointsPerFace, vertices, WHITE, CAM_POS_PRP[0], CAM_POS_PRP[1], CAM_POS_PRP[2], CAM_POS_VP[2]);


	setcolor(COLOR(255, 255, 255));
	setfillstyle(1, COLOR(255, 255, 255));
	int lights[8] = { LIGHT[0] + GLOBAL_OFFSET_XY[0],LIGHT[1] + GLOBAL_OFFSET_XY[1],  LIGHT[0] + 20 + GLOBAL_OFFSET_XY[0], LIGHT[1] + GLOBAL_OFFSET_XY[1], LIGHT[0] + 20 + GLOBAL_OFFSET_XY[0], LIGHT[1] + 20 + GLOBAL_OFFSET_XY[1], LIGHT[0] + GLOBAL_OFFSET_XY[0], LIGHT[1] + 20 + GLOBAL_OFFSET_XY[1] };
	fillpoly(4, lights);


	//porjectOrtho(Q, indexMatrix, faces, pointsPerFace, CYAN);
	getch();
	cleardevice();



	closegraph();
	return 0;

}
//4 100 100 100 200 200 200 200 100

/* vertex buffer
1 = 10 10 10
2 = 10 100 10
3 = 100 100 10
4 = 100 10 10

5 = 10 10 100
6 = 10 100 100
7 = 100 100 100
8 = 100 10 100
*/

/* index buffer (uses order of vertex, keep in clockwise :( )
1 4 3 2
1 2 6 5
5 6 7 8
3 4 8 7
1 5 8 4
2 3 7 6
*/

/* normal buffer
 0  0  1
-1  0  0
 0  0 -1
 1  0  0
 0  1  0
 0 -1  0
*/


