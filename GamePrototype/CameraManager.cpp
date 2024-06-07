#include "pch.h"
#include "CameraManager.h"
#include "Texture.h"
#include "iostream"

CameraManager::CameraManager()
	:	m_Position{},
		m_PreviousPosition{}
{
}

void CameraManager::Update(const Point2f& playerPos, const Rectf& viewPort)
{
	Point2f newPosition = Point2f{
				playerPos.x - viewPort.width / 4,
				playerPos.y - viewPort.height / 4 };

	if (	newPosition.x > 0 
		&&	newPosition.x < 549)
	{
		m_Position.x = newPosition.x;
	}
	else
	{
		m_Position = m_PreviousPosition;
	}
	if (	newPosition.y > 0
		&&	newPosition.y < 314)
	{
		m_Position.y = newPosition.y;
	}
	else
	{
		m_Position.y = m_PreviousPosition.y;
	}

	m_PreviousPosition = m_Position;
}

Point2f& CameraManager::GetCameraPos()
{
	return m_Position;
}
