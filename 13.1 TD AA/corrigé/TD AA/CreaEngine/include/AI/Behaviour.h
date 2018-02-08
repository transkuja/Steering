#ifndef _BEHAVIOUR_H
#define _BEHAVIOUR_H

namespace crea
{
	class CREAENGINE_API Behaviour
	{
	protected:
		Entity* m_entity;
		Vector2f m_desiredVelocity;
		Vector2f m_steering;
		Vector2f m_offset;
		short m_poids;

	public:
		Behaviour(Entity* _entity, short _poids = 1);
		~Behaviour();
		void setOffset(Vector2f& _offset) { m_offset = _offset; };
		virtual Vector2f& Update(double _dT) = 0;
		inline Vector2f GetSteering() { return m_steering; };
		inline void UpdatePoids(short _newPoids) { m_poids = _newPoids; };
	};

	class CREAENGINE_API Seek : public Behaviour
	{
		Entity* m_target;
	public:
		Seek(Entity* _entity, Entity* _target, short _poids = 1) : Behaviour(_entity, _poids), m_target(_target) { };
		Vector2f& Update(double _dT);
	};

	class CREAENGINE_API Flee : public Behaviour
	{
		Entity* m_target;
	public:
		Flee(Entity* _entity, Entity* _target, short _poids = 1) : Behaviour(_entity, _poids), m_target(_target) { };
		Vector2f& Update(double _dT);
	};

	class CREAENGINE_API Pursuit : public Behaviour
	{
		Entity* m_target;
		float m_fTmax;
	public:
		Pursuit(Entity* _entity, Entity* _target, float _fTmax, short _poids = 1) : Behaviour(_entity, _poids), m_target(_target), m_fTmax(_fTmax) { };
		Vector2f& Update(double _dT);
	};

	class CREAENGINE_API Evasion : public Behaviour
	{
		Entity* m_target;
		float m_fTmax;
	public:
		Evasion(Entity* _entity, Entity* _target, float _fTmax, short _poids = 1) : Behaviour(_entity, _poids), m_target(_target), m_fTmax(_fTmax) { };
		Vector2f& Update(double _dT);
	};

	class CREAENGINE_API Arrival : public Behaviour
	{
		Entity* m_target;
		float m_fSlowingDistance;
	public:
		Arrival(Entity* _entity, Entity* _target, float _fSlowingDistance, short _poids = 1) : Behaviour(_entity, _poids), m_target(_target), m_fSlowingDistance(_fSlowingDistance) { };
		Vector2f& Update(double _dT);
	};

	class CREAENGINE_API ObstacleAvoidance : public Behaviour
	{
		double m_radius;
		double m_farView;
		std::vector<Collider*>* m_obstacles;
	public:
		ObstacleAvoidance(Entity* _entity, double radius, double farView, std::vector<Collider*>* obstacles, short _poids = 1)
			: Behaviour(_entity, _poids), m_radius(radius), m_farView(farView), m_obstacles(obstacles) { };
		Vector2f& Update(double _dT);
	};

	class CREAENGINE_API Wander : public Behaviour
	{
		double m_radius;
		double m_littleRadius;
		Vector2f m_R;
	public:
		Wander(Entity* _entity, double _radius, double _littleRadius, short _poids = 1)
			: Behaviour(_entity, _poids), m_radius(_radius), m_littleRadius(_littleRadius) { };
		Vector2f& Update(double _dT);
	};

	class CREAENGINE_API PathFollowing : public Behaviour
	{
		float m_fC;
		
		std::vector<Collider*>* m_obstacles;
	public:
		bool m_isArrived = false;
		PathFollowing(Entity* _entity, float _fC, std::vector<Collider*>* obstacles, short _poids = 1)
			: Behaviour(_entity, _poids), m_fC(_fC), m_obstacles(obstacles) { };
		Vector2f& Update(double _dT);
	};

	//class CREAENGINE_API UnalignedCollisionAvoidance : public Behaviour
	//{
	//	double m_radius;
	//	std::vector<Entity*>* m_entities;
	//public:
	//	UnalignedCollisionAvoidance(Entity* _entity, double radius, std::vector<Entity*>* entities)
	//		: Behavior(_entity), m_radius(radius), m_entities(entities) { };
	//	Vector2f& Update(double _dT);
	//};

	//class CREAENGINE_API Separation : public Behaviour
	//{
	//	double m_distanceMax;
	//	std::vector<Entity*>* m_entities;
	//public:
	//	Separation(Entity* _entity, double distanceMax, std::vector<Entity*>* entities)
	//		: Behavior(_entity), m_distanceMax(distanceMax), m_entities(entities) { };
	//	Vector2f& Update(double _dT);
	//};

	//class CREAENGINE_API Cohesion : public Behaviour
	//{
	//	double m_distanceMax;
	//	std::vector<Entity*>* m_entities;
	//public:
	//	Cohesion(Entity* _entity, double distanceMax, std::vector<Entity*>* entities)
	//		: Behavior(_entity), m_distanceMax(distanceMax), m_entities(entities) { };
	//	Vector2f& Update(double _dT);
	//};

	//class CREAENGINE_API Alignment : public Behaviour
	//{
	//	double m_distanceMax;
	//	std::vector<Entity*>* m_entities;
	//public:
	//	Alignment(Entity* _entity, double distanceMax, std::vector<Entity*>* entities)
	//		: Behavior(_entity), m_distanceMax(distanceMax), m_entities(entities) { };
	//	Vector2f& Update(double _dT);
	//};

	//class CREAENGINE_API Swarming : public Behaviour
	//{
	//	Entity* m_target;
	//	float m_fSwarmDistanceSquare;
	//public:
	//	Swarming(Entity* _entity, Entity* _target, float _fSwarmDistance) : Behavior(_entity), m_target(_target), m_fSwarmDistanceSquare(_fSwarmDistance*_fSwarmDistance) { };
	//	Vector2f& Update(double _dT);
	//};

	//class CREAENGINE_API LeadFollowing : public Behaviour
	//{
	//	Entity* m_leader;
	//	double m_distance;
	//	double m_angle;
	//	double m_distanceFlee;
	//	double m_distanceArrive;
	//public:
	//	LeadFollowing(Entity* _entity, Entity* _leader, double _distance, double _angle, double _distanceFlee, double _distanceArrive)
	//		: Behavior(_entity), m_leader(_leader), m_distance(_distance), m_angle(_angle), m_distanceFlee(_distanceFlee), m_distanceArrive(_distanceArrive) { };
	//	Vector2f& Update(double _dT);
	//};

	//class CREAENGINE_API FormationV : public Behaviour
	//{
	//	Entity* m_leader;
	//	bool m_bUseLeaderOrientation;
	//	unsigned int m_id, m_maxId, m_nbInLine;
	//	double m_distanceMax, m_slowingDistance;
	//	double m_angle;

	//public:
	//	FormationV(Entity* _entity, Entity* _leader, bool _bUseLeaderOrientation,
	//		unsigned int _nbInLine, unsigned int _id, unsigned int _maxId,
	//		double _distanceMax, double _slowingDistance,
	//		double _angle)
	//		: Behavior(_entity), m_leader(_leader), m_bUseLeaderOrientation(_bUseLeaderOrientation),
	//		m_nbInLine(_nbInLine), m_id(_id), m_maxId(_maxId),
	//		m_distanceMax(_distanceMax), m_slowingDistance(_slowingDistance),
	//		m_angle(_angle)
	//	{};
	//	Vector2f& Update(double _dT);
	//};

	//// TODO: add formation line

	//class CREAENGINE_API FormationCircle : public Behaviour
	//{
	//	Entity* m_leader;
	//	bool m_bUseLeaderOrientation;
	//	unsigned int m_id, m_maxId, m_nbInCircle;
	//	double m_distanceMax, m_slowingDistance;
	//	double m_minAngle, m_maxAngle, m_minRadius;

	//public:
	//	FormationCircle(Entity* _entity, Entity* _leader, bool _bUseLeaderOrientation,
	//		unsigned int _nbInCircle, unsigned int _id, unsigned int _maxId,
	//		double _distanceMax, double _slowingDistance,
	//		double _minAngle, double _maxAngle, double _minRadius)
	//		: Behavior(_entity), m_leader(_leader), m_bUseLeaderOrientation(_bUseLeaderOrientation),
	//		m_nbInCircle(_nbInCircle), m_id(_id), m_maxId(_maxId),
	//		m_distanceMax(_distanceMax), m_slowingDistance(_slowingDistance),
	//		m_minAngle(_minAngle), m_maxAngle(_maxAngle), m_minRadius(_minRadius)
	//	{};
	//	Vector2f& Update(double _dT);
	//};

	//// TODO: add formation 2lvl
	//// TODO: add formation of formations

	//class CREAENGINE_API FormationDynamic : public Behaviour
	//{
	//	Entity* m_leader;
	//	bool m_bUseLeaderOrientation;
	//	unsigned int m_id, m_maxId, m_nbInCircle;
	//	double m_distanceMax, m_slowingDistance;
	//	double m_minAngle, m_maxAngle, m_minRadius;

	//	double m_angleStart;

	//public:
	//	FormationDynamic(Entity* _entity, Entity* _leader, bool _bUseLeaderOrientation,
	//		unsigned int _nbInCircle, unsigned int _id, unsigned int _maxId,
	//		double _distanceMax, double _slowingDistance,
	//		double _minAngle, double _maxAngle, double _minRadius)
	//		: Behavior(_entity), m_leader(_leader), m_bUseLeaderOrientation(_bUseLeaderOrientation),
	//		m_nbInCircle(_nbInCircle), m_id(_id), m_maxId(_maxId),
	//		m_distanceMax(_distanceMax), m_slowingDistance(_slowingDistance),
	//		m_minAngle(_minAngle), m_maxAngle(_maxAngle), m_minRadius(_minRadius)
	//	{
	//		m_angleStart = 0.0f;
	//	};
	//	Vector2f& Update(double _dT);
	//};
}
#endif
