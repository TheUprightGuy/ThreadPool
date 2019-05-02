#include <iostream>
#include <thread>

#include "Task.h"
#include "Mandelbrot.h"

CTask::CTask()
	:m_ivalueX(0)
{

}

CTask::CTask(HWND _hwnd, int _valueX, int _valueY)
	: m_ivalueX(_valueX),
	m_ivalueY(_valueY),
	m_hwnd(_hwnd)
{

}

CTask::~CTask()
{

}

void CTask::operator()() const
{
	//Sleep to simulate work being done
	RECT _clientRect;
	GetClientRect(m_hwnd, &_clientRect);
	const int _iWindowWidth = _clientRect.right;
	const int _iWindowHeight = _clientRect.bottom;

	const int iXMax = 16;
	const int iYMax = 8;

	const int iXDivided = _iWindowWidth / iXMax;
	const int iYDivided = _iWindowHeight / iYMax;


	RECT drawRect = { m_ivalueX * iXDivided, m_ivalueY * iYDivided, (m_ivalueX + 1) * iXDivided, (m_ivalueY + 1) * iYDivided };
	CMandelBrot::GetInstance()->Draw(m_hwnd, drawRect);

	//std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 101));
}

int CTask::getValue() const
{
	return m_ivalueX;
}