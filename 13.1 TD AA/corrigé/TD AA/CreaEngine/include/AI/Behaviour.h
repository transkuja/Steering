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
	};

	class CREAENGINE_API Seek : public Behaviour
	{
		Entity* m_target;
	public:
		Seek(Entity* _entity, Entity* _target) : Behaviour(_entity), m_target(_target) { };
		Vector2f& Update(double _dT);
	};
}
#endif
