#include <complex>
#include <iostream>
#ifndef CAMERA_H
#define CAMERA_H

class Camera
{

	//aspect ratio is always square
	private:
		int width;
		int height;

		double zoom;

	//coordinates of the top right of the camera field
	double x;
	double y;

	public:

		Camera(double minX, double minY, int width, int height);

		std::complex<double> transform(int inputX, int inputY);

		void modZoom(float zoomLevel);
		void panX(int amount);
		void panY(int amount);
};

#endif
