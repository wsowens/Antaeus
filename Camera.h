#include <complex>

#ifndef CAMERA_H
#define CAMERA_H

class Camera
{

	//aspect ratio is always such that 1 unit x : 1 unit
	private:
		//const int maxWidth;
		//const int maxHeight;
		double zoom;

	//coordinates of the top right of the camera field
	double x;
	double y;

	public:

		Camera(double startX, double startY, double startZoom);

		std::complex<double> transform(int inputX, int inputY);

		void incrementZoom();
		//TODO: overload the operators
};

#endif
