#ifndef __MANDELBROT_H__
#define __MANDELBROT_H__

#include <Windows.h>

#define MAXITERATIONS 255

typedef struct tagComplexNumber
{
	float _fReal;
	float _fImag;
} ComplexNumber;

class CMandelBrot
{
public:
	~CMandelBrot();
	static CMandelBrot* GetInstance();
	void Draw(HWND, RECT _rect);
private:
	CMandelBrot();

	// Singleton Instance
	static CMandelBrot* s_pMandelBrotInstance;
};

#endif