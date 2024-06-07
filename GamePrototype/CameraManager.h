#pragma once

class Texture;

class CameraManager final
{
public:
	CameraManager();

	void Update(const Point2f& playerPos, const Rectf& viewPort);

	Point2f& GetCameraPos();

private:
	Point2f m_Position;
	Point2f m_PreviousPosition;
};

