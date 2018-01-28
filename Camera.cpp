#include "Camera.h"

Camera::Camera(double startX, double startY, double startZoom)
{
	x = startX;
	y = startY;
	zoom = startZoom;
}

std::complex<double> Camera::transform(int inputX, int inputY)
{
	double a, b;
	a = zoom * inputX - x;
	b = y - zoom * inputY;
	return {a, b};
}
