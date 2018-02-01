#ifndef __BEHAVIOR_H_
#define __BEHAVIOR_H_

#include "Vector2.h"
#include "Tools.h"
#include "Obstacle.h"
#include <vector>
#include <random>

class Entity;

class Behavior
{
protected:
	Entity* m_entity;
	Vector2 m_desiredVelocity;
	Vector2 m_steering;
	Vector2 m_offset;
public:
	Behavior(Entity* _entity);
	void setOffset(Vector2& _offset) { m_offset = _offset; };
	virtual Vector2& Update(double _dT) = 0;
};

class Seek : public Behavior
{
	Entity* m_target;
public:
	Seek(Entity* _entity, Entity* _target) : Behavior(_entity), m_target(_target) { };
	Vector2& Update(double _dT);
};

class Flee : public Behavior
{
	Entity* m_target;
public:
	Flee(Entity* _entity, Entity* _target) : Behavior(_entity), m_target(_target) { };
	Vector2& Update(double _dT);
};

class Pursuit : public Behavior
{
	Entity* m_target;
	float m_fTmax;
public:
	Pursuit(Entity* _entity, Entity* _target, float _fTmax) : Behavior(_entity), m_target(_target), m_fTmax(_fTmax) { };
	Vector2& Update(double _dT);
};

class Evasion : public Behavior
{
	Entity* m_target;
	float m_fTmax;
public:
	Evasion(Entity* _entity, Entity* _target, float _fTmax) : Behavior(_entity), m_target(_target), m_fTmax(_fTmax) { };
	Vector2& Update(double _dT);
};

class Arrival : public Behavior
{
	Entity* m_target;
	float m_fSlowingDistance;
public:
	Arrival(Entity* _entity, Entity* _target, float _fSlowingDistance) : Behavior(_entity), m_target(_target), m_fSlowingDistance(_fSlowingDistance) { };
	Vector2& Update(double _dT);
};

class Wander : public Behavior
{
	double m_distance;
	double m_radius;
	double m_littleRadius;
public:
	Wander(Entity* _entity, double _distance, double _radius, double _littleRadius) 
		: Behavior(_entity), m_distance(_distance), m_radius(_radius), m_littleRadius(_littleRadius) { };
	Vector2& Update(double _dT);
};

class PathFollowing : public Behavior
{
	float m_fC;
	double m_radius;
	double m_farView;
	std::vector<Obstacle*>* m_obstacles;
public:
	PathFollowing(Entity* _entity, float _fC, double radius, double farView, std::vector<Obstacle*>* obstacles) 
		: Behavior(_entity), m_fC(_fC), m_radius(radius), m_farView(farView), m_obstacles(obstacles) { };
	Vector2& Update(double _dT);
};

class UnalignedCollisionAvoidance : public Behavior
{	
	double m_radius;
	std::vector<Entity*>* m_entities;
public:
	UnalignedCollisionAvoidance(Entity* _entity, double radius, std::vector<Entity*>* entities) 
		: Behavior(_entity), m_radius(radius), m_entities(entities) { };
	Vector2& Update(double _dT);
};

class ObstacleAvoidance : public Behavior
{
	double m_radius;
	double m_farView;
	std::vector<Obstacle*>* m_obstacles;
public:
	ObstacleAvoidance(Entity* _entity, double radius, double farView, std::vector<Obstacle*>* obstacles) 
		: Behavior(_entity), m_radius(radius), m_farView(farView), m_obstacles(obstacles) { };
	Vector2& Update(double _dT);
};

class Separation : public Behavior
{
	double m_distanceMax;
	std::vector<Entity*>* m_entities;
public:
	Separation(Entity* _entity, double distanceMax, std::vector<Entity*>* entities) 
		: Behavior(_entity), m_distanceMax(distanceMax), m_entities(entities) { };
	Vector2& Update(double _dT);
};

class Cohesion : public Behavior
{
	double m_distanceMax;
	std::vector<Entity*>* m_entities;
public:
	Cohesion(Entity* _entity, double distanceMax, std::vector<Entity*>* entities) 
		: Behavior(_entity), m_distanceMax(distanceMax), m_entities(entities) { };
	Vector2& Update(double _dT);
};

class Alignment : public Behavior
{
	double m_distanceMax;
	std::vector<Entity*>* m_entities;
public:
	Alignment(Entity* _entity, double distanceMax, std::vector<Entity*>* entities) 
		: Behavior(_entity), m_distanceMax(distanceMax), m_entities(entities) { };
	Vector2& Update(double _dT);
};

class LeadFollowing : public Behavior
{
	Entity* m_leader;
	double m_distance;	
	double m_angle;	
	double m_distanceFlee;
	double m_distanceArrive;	
public:
	LeadFollowing(Entity* _entity, Entity* _leader, double _distance, double _angle, double _distanceFlee, double _distanceArrive) 
		: Behavior(_entity), m_leader(_leader), m_distance(_distance), m_angle(_angle), m_distanceFlee(_distanceFlee), m_distanceArrive(_distanceArrive) { };
	Vector2& Update(double _dT);
};

class Swarming : public Behavior
{
	Entity* m_target;
	float m_fSwarmDistanceSquare;
public:
	Swarming(Entity* _entity, Entity* _target, float _fSwarmDistance) : Behavior(_entity), m_target(_target), m_fSwarmDistanceSquare(_fSwarmDistance*_fSwarmDistance) { };
	Vector2& Update(double _dT);
};

class FormationV : public Behavior
{
	Entity* m_leader;
	bool m_bUseLeaderOrientation;
	unsigned int m_id, m_maxId, m_nbInLine;
	double m_distanceMax, m_slowingDistance;
	double m_angle;

public:
	FormationV(Entity* _entity, Entity* _leader, bool _bUseLeaderOrientation, 
		unsigned int _nbInLine, unsigned int _id, unsigned int _maxId, 
		double _distanceMax, double _slowingDistance, 
		double _angle)
		: Behavior(_entity), m_leader(_leader), m_bUseLeaderOrientation(_bUseLeaderOrientation), 
		m_nbInLine(_nbInLine), m_id(_id), m_maxId(_maxId), 
		m_distanceMax(_distanceMax), m_slowingDistance(_slowingDistance), 
		m_angle(_angle) 
	{};
	Vector2& Update(double _dT);
};

class FormationCircle : public Behavior
{
	Entity* m_leader;
	bool m_bUseLeaderOrientation;
	unsigned int m_id, m_maxId, m_nbInCircle;
	double m_distanceMax, m_slowingDistance;
	double m_minAngle, m_maxAngle, m_minRadius;

public:
	FormationCircle(Entity* _entity, Entity* _leader, bool _bUseLeaderOrientation,
		unsigned int _nbInCircle, unsigned int _id, unsigned int _maxId,
		double _distanceMax, double _slowingDistance,
		double _minAngle, double _maxAngle, double _minRadius)
		: Behavior(_entity), m_leader(_leader), m_bUseLeaderOrientation(_bUseLeaderOrientation), 
		m_nbInCircle(_nbInCircle), m_id(_id), m_maxId(_maxId), 		
		m_distanceMax(_distanceMax), m_slowingDistance(_slowingDistance),
		m_minAngle(_minAngle), m_maxAngle(_maxAngle), m_minRadius(_minRadius)
	{};
	Vector2& Update(double _dT);
};

class FormationDynamic : public Behavior
{
	Entity* m_leader;
	bool m_bUseLeaderOrientation;
	unsigned int m_id, m_maxId, m_nbInCircle;
	double m_distanceMax, m_slowingDistance;
	double m_minAngle, m_maxAngle, m_minRadius;

	double m_angleStart;

public:
	FormationDynamic(Entity* _entity, Entity* _leader, bool _bUseLeaderOrientation,
		unsigned int _nbInCircle, unsigned int _id, unsigned int _maxId,
		double _distanceMax, double _slowingDistance,
		double _minAngle, double _maxAngle, double _minRadius)
		: Behavior(_entity), m_leader(_leader), m_bUseLeaderOrientation(_bUseLeaderOrientation),
		m_nbInCircle(_nbInCircle), m_id(_id), m_maxId(_maxId),
		m_distanceMax(_distanceMax), m_slowingDistance(_slowingDistance),
		m_minAngle(_minAngle), m_maxAngle(_maxAngle), m_minRadius(_minRadius)
	{
		m_angleStart = 0.0f;
	};
	Vector2& Update(double _dT);
};
#endif