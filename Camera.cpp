#include "Camera.h"

Camera::Camera(double maxX, double maxY, int width, int height)
{
	this->width = width;
	this->height = height;

	x = maxX;
	y = maxY;

	//zoom calculation done off of width: graph assumed square, centered at origin
	zoom = x * 2 / width;
	std::cout << zoom << std::endl;
}

std::complex<double> Camera::transform(int inputX, int inputY)
{
	double a, b;
	a = zoom * inputX - x;
	b = y - zoom * inputY;
	return {a, b};
}

void Camera::modZoom(float zoomLevel)
{
	double oldZoom = zoom;
	zoom *= zoomLevel;
	x = x - width * (oldZoom - zoom) / 2;
	y = y - height * (oldZoom - zoom) / 2;
}

void Camera::panX(int amount)
{
	x += amount * zoom;
}

void Camera::panY(int amount)
{
	y += amount * zoom;
}
