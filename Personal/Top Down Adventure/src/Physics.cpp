#include "Physics.h"
#include "Components.h"

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	try
	{
		Vec2 aSize = a->getComponent<CBoundingBox>()->halfSize;
		Vec2 aPos = a->getComponent<CTransform>()->pos;
		Vec2 bSize = b->getComponent<CBoundingBox>()->halfSize;
		Vec2 bPos = b->getComponent<CTransform>()->pos;

		float dx = std::abs(aPos.x - bPos.x);
		float dy = std::abs(aPos.y - bPos.y);

		return Vec2(bSize.x + aSize.x - dx, bSize.y + aSize.y - dy);
	}
	catch (...)
	{
		return Vec2(0, 0);
	}
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	try
	{
		Vec2 aSize = a->getComponent<CBoundingBox>()->halfSize;
		Vec2 aPos = a->getComponent<CTransform>()->prevPos;
		Vec2 bSize = b->getComponent<CBoundingBox>()->halfSize;
		Vec2 bPos = b->getComponent<CTransform>()->prevPos;

		float dx = std::abs(aPos.x - bPos.x);
		float dy = std::abs(aPos.y - bPos.y);

		return Vec2(bSize.x + aSize.x - dx, bSize.y + aSize.y - dy);
	}
	catch (...)
	{
		return Vec2(0, 0);
	}
}

Intersect Physics::LineIntersect(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d)
{    
	Vec2 r = b-a;
	Vec2 s = d-c;
	float rxs = r.cross(s);
	Vec2 cma = c-a;
	float t = (cma.cross(s)) / rxs;
	float u = (cma.cross(r)) / rxs;
	if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
	{
		return { true, Vec2(a.x + t * r.x, a.y + t * r.y) };
	}
	else
	{
		return { false, Vec2(0,0) };
	}
}

bool Physics::EntityIntersect(const Vec2 & a, const Vec2 & b, std::shared_ptr<Entity> e)
{




    return false;
}

float Physics::FollowDist(std::shared_ptr<Entity> a, const Vec2 & b)
{
	try
	{
		Vec2 aPos = a->getComponent<CTransform>()->pos;

		float dx = std::abs(aPos.x - b.x);
		float dy = std::abs(aPos.y - b.y);

		return std::sqrt(dx*dx + dy * dy);
	}
	catch (...)
	{
		return 0;
	}
}