#include "Mandelbrot.h"
#include <math.h>
//#include "utility.h"

CMandelBrot* CMandelBrot::s_pMandelBrotInstance = 0;

CMandelBrot::CMandelBrot()
{
}

CMandelBrot::~CMandelBrot()
{
}

CMandelBrot* CMandelBrot::GetInstance()
{
	if(s_pMandelBrotInstance == 0)
	{
		s_pMandelBrotInstance = new CMandelBrot();
	}
	return s_pMandelBrotInstance;
}

void CMandelBrot::Draw(HWND _hwnd, RECT _rect)
{
	HDC hdc = GetDC(_hwnd);
	//Do all drawing here
	
	//Get the Window Height and Width
	RECT _clientRect;
	GetClientRect(_hwnd, &_clientRect);
	int _iWindowWidth = _clientRect.right;
	int _iWindowHeight = _clientRect.bottom;

	ComplexNumber c;
	ComplexNumber Zn0,Zn1;
	int _iIteration = 0;

	float RealMin = -4.0f;
	float ImMin = -2.0f;
	float ImMax = 2.0f;
	//float RealMax = 2.0f;
	float scaleFactor = 1.0f;
	float RealMax = _iWindowWidth*((ImMax - ImMin)/static_cast<float>(_iWindowHeight)) + RealMin;
	
	//Iterate over the screen area starting from 0,0 to Width,Height.
	for(int _iScreenY = _rect.top; _iScreenY < _rect.bottom; _iScreenY++)
	{
		for(int _iScreenX = _rect.left; _iScreenX < _rect.right; _iScreenX++)
		{
			//int _iScreenY = 200; int _iScreenX = 200; 
			//Map the screen (x,y) coordinates to the complex-number values in the complex-plane
			c._fReal = scaleFactor*((static_cast<float>(_iScreenX) * (RealMax - RealMin))/_iWindowWidth + RealMin);
			c._fImag = scaleFactor*((static_cast<float>(_iScreenY) * (ImMin - ImMax))/_iWindowHeight + ImMax);
			Zn0._fReal = 0; Zn0._fImag = 0;
			bool bIsInside = true;
			int _iIteration = 0;
			for(; _iIteration < MAXITERATIONS; _iIteration++)
			{
				Zn1._fReal = (Zn0._fReal * Zn0._fReal) - (Zn0._fImag * Zn0._fImag) + c._fReal;
				Zn1._fImag = (2 * Zn0._fReal * Zn0._fImag) + c._fImag;
							
				// if |z|-squared exceeds 4 then the point is not in the set; break from the loop.
				if(((Zn1._fReal * Zn1._fReal) + (Zn1._fImag * Zn1._fImag)) > 4 )
				{
					bIsInside = false;
					break;
				}
				Zn0._fReal = Zn1._fReal; //assign z0 = z1 before the next iteration
				Zn0._fImag = Zn1._fImag;
			} //End of for iteration loop

			//If the point being tested never reached the limit of 2; within an iteration of 255; then the point is to be in the set, therefore color it black
			if(bIsInside)
			{
				//Point is in the set; Color it black!
				SetPixel(hdc, _iScreenX, _iScreenY, RGB(0, 0, 0));
			}
			else
			{
				//point is not in the set
				double z = sqrt( Zn1._fReal * Zn1._fReal + Zn1._fImag * Zn1._fImag );
				int brightness = static_cast<int>( 256 * log( 10.0 + _iIteration - log( z ) / log( double( MAXITERATIONS ) ) ) );
						
				//auto colColour = RGB( brightness, brightness, 255 );
				//color it in different shades of blue 
				SetPixel(hdc, _iScreenX, _iScreenY, RGB(brightness, brightness, 155));
				//Color it white
				//SetPixel(hdc, _iScreenX, _iScreenY, RGB( 255, 255, 255 ));
			}

		} //End of for X-coordinate
	} //End of for Y-coordinate
	ReleaseDC(_hwnd, hdc);
}
