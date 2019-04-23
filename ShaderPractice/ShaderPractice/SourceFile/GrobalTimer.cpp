#include "stdafx.h"
#include "GrobalTimer.h"


GrobalTimer::GrobalTimer():
	m_ElapsedTick(0),
	m_TotalTick(0),
	m_LeftOverTick(0),
	m_FlameCount(0),
	m_FlamePerSecond(0),
	m_FlameThisSecond(0),
	m_QPCSecCouter(0),
	m_IsFixedStep(false),
	m_TergetTick(TickPerSecond / 120)
{
	QueryPerformanceFrequency(&m_QPCFrec);
	QueryPerformanceCounter(&m_QPCLastTime);
	m_MaxDelta = m_QPCFrec.QuadPart / 10;
}


GrobalTimer::~GrobalTimer()
{
}

double	GrobalTimer::TickToSecond(uint64 Tick) 
{
	return static_cast<double>(Tick) / TickPerSecond; 
}

uint64	GrobalTimer::SecondToTick(double Second)
{
	return static_cast<unsigned long long>(Second)*TickPerSecond; 
}

uint64	GrobalTimer::GetElapsedTick() const
{
	return m_ElapsedTick; 
}

double	GrobalTimer::GetElapsedSecond()const
{
	return TickToSecond(m_ElapsedTick); 
}

uint64	GrobalTimer::GetTotalTick() const 
{
	return m_TotalTick; 
}

double	GrobalTimer::GetTotalSecond() const
{
	return TickToSecond(m_TotalTick); 
}

uint32	GrobalTimer::GetTotalFrameCount() const
{
	return m_FlameCount; 
}

uint32	GrobalTimer::GetFramePerSecond() const
{
	return m_FlamePerSecond; 
}

void	GrobalTimer::SetFixedStepTime(bool IsFixed)
{
	m_IsFixedStep = IsFixed;
}

void	GrobalTimer::SetElapsedSecond(double Second)
{
	m_ElapsedTick = SecondToTick(Second); 
}

void	GrobalTimer::SetElapsedTick(uint64 Tick)
{
	m_ElapsedTick = Tick; 
}

void GrobalTimer::ResetElapsedTime()
{
	if (!QueryPerformanceCounter(&m_QPCLastTime))
	{
		
	}

	m_LeftOverTick = 0;
	m_FlamePerSecond = 0;
	m_FlameThisSecond = 0;
	m_QPCSecCouter = 0;
}