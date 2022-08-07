#pragma once
static int WINDOW_SIZE[2] = { 800,800};
int GLOBAL_OFFSET_XY[2] = { 100,100 };


void DDA(int x0, int y0, int xl, int yl, int color) {
	int delx = xl - x0;
	int dely = yl - y0;
	float x_inc = 0;
	float y_inc = 0;
	float x = x0, y = y0;
	int steps = 0;
	if (fabs(dely) <= fabs(delx)) {
		x_inc = delx / fabs(delx);
		y_inc = (float)dely / delx * x_inc;
		steps = fabs(delx);
	}
	else {
		y_inc = dely / fabs(dely);
		x_inc = (float)delx / dely * y_inc;
		steps = fabs(dely);
	}
	for (int i = 0; i <= steps; i++) {
		putpixel(floor(x + .5f), floor(y + .5f), color);
		x += x_inc;
		y += y_inc;
	}
	return;
}

void BLA(int x0, int y0, int xl, int yl, int color) {
	int delx = fabs(xl - x0);
	int dely = fabs(yl - y0);
	int a = 0, b = 0;
	int p = 0;
	a = ((xl - x0) > 0) ? 1 : -1;
	b = ((yl - y0) > 0) ? 1 : -1;
	if (delx > dely) {
		p = 2 * dely - delx;
		for (int i = 0; i <= delx; i++) {
			putpixel(x0 + GLOBAL_OFFSET_XY[0], y0 + GLOBAL_OFFSET_XY[1], color);
			x0 += a;
			if (p <= 0)
				p += 2 * dely;
			else {
				p += 2 * dely - 2 * delx;
				y0 += b;
			}
		}
	}
	else {
		p = 2 * delx - dely;
		for (int i = 0; i <= dely; i++) {
			putpixel(x0 + GLOBAL_OFFSET_XY[0], y0 + GLOBAL_OFFSET_XY[1], color);
			y0 += b;
			if (p <= 0)
				p += 2 * delx;
			else {
				p += 2 * delx - 2 * dely;
				x0 += a;
			}
		}
	}
}

void Circle(int x0, int y0, int r, int color) {
	int x = 0, y = r;
	int p = 1 - r;
	while (y >= x) {
		putpixel((x + x0), (y + y0), color);
		putpixel((y + x0), (x + y0), color);

		putpixel((-x + x0), (y + y0), color);
		putpixel((-y + x0), (x + y0), color);

		putpixel((x + x0), (-y + y0), color);
		putpixel((y + x0), (-x + y0), color);

		putpixel((-x + x0), (-y + y0), color);
		putpixel((-y + x0), (-x + y0), color);

		if (p >= 0) {
			y -= 1;
			p -= 2 * y;
		}
		x += 1;
		p += 2 * x + 1;
	}
}

void Ellipse(int x0, int y0, int rx, int ry, int color) {
	int x = 0, y = ry;
	int p = ry * ry - rx * rx * ry + ((float)1 / 4) * (float)rx * rx;

	while (rx * rx * y >= ry * ry * x) {
		putpixel((x + x0), (y + y0), color);
		putpixel((-x + x0), (y + y0), color);
		putpixel((x + x0), (-y + y0), color);
		putpixel((-x + x0), (-y + y0), color);
		x += 1;
		if (p < 0)
			p += 2 * ry * ry * x + ry * ry;
		else {
			y -= 1;
			p += 2 * ry * ry * x + ry * ry - 2 * rx * rx * y;
		}
	}

	p = ry * ry * (x + .5f) * (x + .5f) + rx * rx * (y - 1) * (y - 1) - rx * rx * ry * ry;

	while (y >= 0) {
		putpixel((x + x0), (y + y0), color);
		putpixel((-x + x0), (y + y0), color);
		putpixel((x + x0), (-y + y0), color);
		putpixel((-x + x0), (-y + y0), color);
		y -= 1;
		if (p > 0)
			p += rx * rx - 2 * rx * rx * y;
		else {
			x += 1;
			p += rx * rx - 2 * rx * rx * y + 2 * ry * ry * x;
		}
	}
}

//for 2d
struct Mat3xn {
	float** m;
	Mat3xn(int n = 1, int points = 0) {
		m = new float* [3];
		for (int i = 0; i < 3; i++) {
			m[i] = new float[n]();
		}
		if (n >= 2)
			m[2][2] = 1;
		if (points == 1)
		{
			for (int i = 0; i < n; i++)
			{
				m[2][i] = 1;
			}
		}
	}
};

//for 2d multang
Mat3xn matMult(Mat3xn T, Mat3xn P, int n) {
	Mat3xn R(n, 1);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < n; j++) {
			R.m[i][j] = 0;
			for (int k = 0; k < 3; k++) {
				R.m[i][j] += T.m[i][k] * P.m[k][j];
			}
		}
	}
	return R;
}

//2d render
void Draw2d(Mat3xn P, int n, int color) {
	if (n == 1)
		putpixel(P.m[0][0], P.m[1][0], color);
	for (size_t i = 0; i < n - 1; i++) {
		BLA(P.m[0][i], P.m[1][i], P.m[0][i + 1], P.m[1][i + 1], color);

	}
	BLA(P.m[0][n - 1], P.m[1][n - 1], P.m[0][0], P.m[1][0], color);

}

//2d transformations
Mat3xn Translate2d(Mat3xn polygonMat, int order, float x, float y) {
	Mat3xn T(3);
	T.m[0][2] = x;
	T.m[1][2] = y;
	for (int i = 0; i < 3; i++)
		T.m[i][i] = 1;
	Mat3xn R(order, 1);
	R = matMult(T, polygonMat, order);
	for (int i = 0; i < 3; i++)
		R.m[2][i] = 1;

	return R;
}
//2d transformations
Mat3xn Rotate2d(Mat3xn polygonMat, int order, float theta, float x = 0, float y = 0) {
	Mat3xn T(3);
	theta = theta / 180 * 3.1415f;
	T.m[0][0] = T.m[1][1] = cos(theta);
	T.m[0][1] = -sin(theta);
	T.m[0][2] = x * (1 - cos(theta)) + y * sin(theta);
	T.m[1][0] = sin(theta);
	T.m[1][2] = y * (1 - cos(theta)) - x * sin(theta);
	T.m[2][2] = 1;
	Mat3xn R(order, 1);
	R = matMult(T, polygonMat, order);
	for (int i = 0; i < order; i++)
		R.m[2][i] = 1;
	return R;
}
//2d transformations
Mat3xn Shear2d(Mat3xn polygonMat, int order, float sx = 1, float sy = 1, float x = 0, float y = 0)
{
	Mat3xn R(order, 1);
	Mat3xn shr(3);
	for (int i = 0; i < 3; i++)
		shr.m[i][i] = 1;
	shr.m[0][1] = sx;
	shr.m[0][2] = -y * sx;
	shr.m[1][0] = sy;
	shr.m[1][2] = -x * sy;
	R = matMult(shr, polygonMat, order);
	Draw2d(R, order, GREEN);
	return R;
}
//2d transformations
Mat3xn Scale2d(Mat3xn polygonMat, int order, float sx = 1, float sy = 1, float x = 0, float y = 0)
{
	Mat3xn R(order, 1);
	Mat3xn scal(3);
	scal.m[0][0] = sx;
	scal.m[0][2] = -x * sx + x;
	scal.m[1][1] = sy;
	scal.m[1][2] = -y * sy + y;
	scal.m[2][2] = 1;
	R = matMult(scal, polygonMat, order);
	return R;
}
//2d transformations
Mat3xn Reflect2d(Mat3xn polygonMat, int order, float m, float c = 0)
{
	Mat3xn R(order, 1);
	Mat3xn ref(3);
	ref.m[0][0] = 1 - m * m;
	ref.m[1][1] = -ref.m[0][0];
	ref.m[0][1] = ref.m[1][0] = 2 * m;
	ref.m[0][2] = -2 * c * m;
	ref.m[1][2] = 2 * c;
	ref.m[2][2] = 1;

	for (int i = 0; i < 2; i++) {
		for (size_t j = 0; j < 3; j++)
			ref.m[i][j] /= 1 + m * m;
	}
	int x = 0;
	int y = 0;
	for (size_t i = 0; i < 200; i++) {
		if (fabs(m) <= 1) {
			y = m * x + c;
			putpixel(x, y, GREEN);
			x++;
		}
		else {
			x = (y - c) / m;
			putpixel(x, y, GREEN);
			y++;
		}
	}
	R = matMult(ref, polygonMat, order);
	return R;
}
