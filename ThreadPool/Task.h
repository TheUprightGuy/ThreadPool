// A functor class

#ifndef __CTASK_H__
#define __CTASK_H__

#include <Windows.h>

class CTask
{
public:
	CTask();
	CTask(HWND _hwnd, int _valueX, int _valueY);
	~CTask();
	void operator()() const;
	int getValue() const;
private:
	int m_ivalueX;
	int m_ivalueY;

	HWND m_hwnd;
};

#endif



