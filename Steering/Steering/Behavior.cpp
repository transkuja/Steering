#include "Behavior.h"
#include "Entity.h"
#include <iostream>

Behavior::Behavior(Entity* _entity) 
	: m_entity(_entity), m_desiredVelocity(ORIGIN2), m_steering(ORIGIN2), m_offset(ORIGIN2)
{
};

Vector2& Seek::Update( double _dT )
{
	m_desiredVelocity = m_target->getPosition() - m_entity->getPosition();
	m_desiredVelocity.Normalize();
	m_desiredVelocity *= m_entity->getMaxSpeed();
	m_steering = m_desiredVelocity - m_entity->getVelocity();
	return m_steering;
}

Vector2& Flee::Update( double _dT )
{
	m_desiredVelocity =  m_entity->getPosition() - m_target->getPosition();
	m_desiredVelocity.Normalize();
	m_desiredVelocity *= m_entity->getMaxSpeed();
	m_steering = m_desiredVelocity - m_entity->getVelocity();
	return m_steering;
}

Vector2& Pursuit::Update( double _dT )
{
	Vector2 targetOffset = m_entity->getPosition() - m_target->getPosition();
	double fT = Tools::min(m_fTmax, targetOffset.Length());
	Vector2 prediction = m_target->getPosition() + m_target->getVelocity() * fT;
	m_desiredVelocity = prediction - m_entity->getPosition();
	m_desiredVelocity.Normalize();
	m_desiredVelocity *= m_entity->getMaxSpeed();
	m_steering = m_desiredVelocity - m_entity->getVelocity();
	return m_steering;
}

Vector2& Evasion::Update( double _dT )
{
	Vector2 targetOffset = m_entity->getPosition() - m_target->getPosition();
	double fT = Tools::min(m_fTmax, targetOffset.Length());
	Vector2 prediction = m_target->getPosition() + m_target->getVelocity() * fT;
	m_desiredVelocity = m_entity->getPosition() - prediction;
	m_desiredVelocity.Normalize();
	m_desiredVelocity *= m_entity->getMaxSpeed();
	m_steering = m_desiredVelocity - m_entity->getVelocity();
	return m_steering;
}

Vector2& Arrival::Update( double _dT )
{
	Vector2 targetOffset = m_target->getPosition() - m_entity->getPosition();
	double distance = targetOffset.Length();
	double rampedSpeed = m_entity->getMaxSpeed() * distance / m_fSlowingDistance;
	double clippedSpeed = Tools::min(rampedSpeed, m_entity->getMaxSpeed());
	m_desiredVelocity = targetOffset * (clippedSpeed / distance);
	m_steering = m_desiredVelocity - m_entity->getVelocity();
	return m_steering;	
}

Vector2& Wander::Update( double _dT )
{
	Vector2 direction = m_entity->getVelocity();
	direction.Normalize();
	
	Vector2 center = direction * (m_entity->getMaxForce() - m_radius);

	double value = (double)(rand() %360);
	Vector2 r(sin(value*PI/180.0f) * m_littleRadius, cos(value*PI/180.0f) * m_littleRadius);
	Vector2 R = m_entity->getLastR();
	R += r;
	R.Normalize();
	R *= m_radius;
	m_steering = center + R;
	m_entity->setLastR(R);

	return m_steering;
}

Vector2& PathFollowing::Update( double _dT )
{
	Vector2 position = m_entity->getPosition() + m_entity->getVelocity() * m_fC;
	double distMin = INFINITY;
	std::vector<Obstacle*>::iterator j;

	// if no target yet
	if (m_entity->pTarget == NULL)
	{
		// Compute closest waypoints
		for(std::vector<Obstacle*>::iterator i = m_obstacles->begin(); i != m_obstacles->end(); i++)
		{
			Vector2 obstaclePosition = (*i)->m_position;
			double dist = (position - obstaclePosition).Length();

			if(dist < distMin){
				distMin = dist;
				// take next as target (if one exist)
				j = i;
				j++;
				if (j != m_obstacles->end())
					m_entity->pTarget = *j;
				else
					m_entity->pTarget = *i;
			}
		}
	}

	if ((m_entity->pTarget->m_position - m_entity->getPosition()).Length() < m_entity->pTarget->m_radius)
	{
		// Find next waypoint
		for(std::vector<Obstacle*>::iterator i = m_obstacles->begin(); i != m_obstacles->end(); i++)
		{
			if(*i == m_entity->pTarget)
			{
				// take next as target (if one exist)
				j = i;
				j++;
				if (j != m_obstacles->end())
					m_entity->pTarget = *j;
				else
					m_entity->pTarget = *i;
				break;
			}
		}
	}
	
	// Seek on target
	m_desiredVelocity = m_entity->pTarget->m_position - m_entity->getPosition();
	m_desiredVelocity.Normalize();
	m_desiredVelocity *= m_entity->getMaxSpeed();
	m_steering = m_desiredVelocity - m_entity->getVelocity();

	return m_steering;
}

Vector2& UnalignedCollisionAvoidance::Update( double _dT )
{
	Vector2 nextPosition = m_entity->getPosition() +  m_entity->getVelocity() / 2.0f;
	Vector2 nextPosition2, diff;
	m_steering = ORIGIN2;
	for(std::vector<Entity*>::iterator i = m_entities->begin(); i != m_entities->end(); i++)
	{
		if((*i) != m_entity)
		{
			nextPosition2 = (*i)->getPosition() + (*i)->getVelocity() / 2.0f;
			diff = nextPosition - nextPosition2;
			if( diff.Length() < m_radius)
			{
				m_steering += diff;
			}
		}
	}
	return m_steering;
}

Vector2& ObstacleAvoidance::Update( double _dT )
{
	Vector2 position = m_entity->getPosition();
	Vector2 direction = m_entity->getVelocity();
	direction.Normalize();

	double distance = INFINITY;
	double fleeDirection;

	for (std::vector<Obstacle*>::iterator i = m_obstacles->begin(); i != m_obstacles->end(); i++)
	{
		Vector2 positionToObstacle = (*i)->m_position - position;

		// Projection du vecteur positionToObstacle sur le vecteur direction normalisé (r) et son vecteur orthogonal (s)
		double r = positionToObstacle.x * direction.x + positionToObstacle.y * direction.y;
		double s = positionToObstacle.x * direction.y - positionToObstacle.y * direction.x;

		if (r > 0
			&& r - (*i)->m_radius < m_farView * m_entity->getVelocity().Length() / m_entity->getMaxSpeed()
			&& r + (*i)->m_radius < distance
			&& s < (m_radius + (*i)->m_radius)
			&& s > -(m_radius + (*i)->m_radius)){
			distance = r - (*i)->m_radius;
			fleeDirection = s;
		}
	}
	if (distance == INFINITY)
	{
		m_steering = ORIGIN2;
	}
	else
	{
		direction *= m_entity->getMaxForce();
		if (fleeDirection > 0)
		{
			m_steering = Vector2(-direction.y, direction.x);
		}
		else
		{
			m_steering = Vector2(direction.y, -direction.x);
		}
	}
	return m_steering;
}
Vector2& Separation::Update( double _dT )
{
	Vector2 direction;
	double distance;
	m_steering = ORIGIN2;
	for(std::vector<Entity*>::iterator i = m_entities->begin(); i != m_entities->end(); i++)
	{
		if((*i) != m_entity)
		{
			direction = m_entity->getPosition() - (*i)->getPosition();
			distance = direction.Length();
			if(distance <= m_distanceMax && distance != 0.0f)
			{
				direction.Normalize();
				m_steering += direction * 1 / distance;
			}
		}
	}
	m_steering.Normalize();
	m_steering *= m_entity->getMaxForce();
	return m_steering;
}

Vector2& Cohesion::Update( double _dT )
{
	Vector2 direction, center = ORIGIN2;
	double distance;
	int nbNeighbors = 0;
	for(std::vector<Entity*>::iterator i = m_entities->begin(); i != m_entities->end(); i++)
	{
		if((*i) != m_entity)
		{
			direction = m_entity->getPosition() - (*i)->getPosition();
			distance = direction.Length();
			if(distance <= m_distanceMax)
			{
				center += (*i)->getPosition();
				nbNeighbors++;
			}
		}
	}
	if (nbNeighbors)
	{
		center /= nbNeighbors;
		m_steering = center - m_entity->getPosition();
	}

	return m_steering;
}


Vector2& Alignment::Update( double _dT )
{
	Vector2 direction;
	double distance;
	int nbNeighbors = 0;
	m_desiredVelocity = ORIGIN2;
	for(std::vector<Entity*>::iterator i = m_entities->begin(); i != m_entities->end(); i++)
	{
		if((*i) != m_entity)
		{
			direction = m_entity->getPosition() - (*i)->getPosition();
			distance = direction.Length();
			if(distance <= m_distanceMax)
			{
				m_desiredVelocity += (*i)->getVelocity();
				nbNeighbors++;
			}
		}
	}
	if (nbNeighbors)
	{
		m_desiredVelocity /= nbNeighbors;
		m_steering = m_desiredVelocity - m_entity->getVelocity();
	}

	return m_steering;
}

Vector2& LeadFollowing::Update( double _dT )
{
	Vector2 entityPosition = m_entity->getPosition();
	Vector2 entityToLeader = entityPosition - m_leader->getPosition();
	Vector2 leadDirection = m_leader->getVelocity();
	leadDirection.Normalize();

	float ang = leadDirection.angle(entityToLeader);
	if (ang < m_angle && entityToLeader.Length()<m_distance)
	{
		// Flee in front of leader
		Vector2 frontOfLeader = ( m_leader->getPosition() + leadDirection * m_distanceFlee );
		m_desiredVelocity =  entityPosition - frontOfLeader;
		m_desiredVelocity.Normalize();
		m_desiredVelocity *= m_entity->getMaxSpeed();
		m_steering = m_desiredVelocity - m_entity->getVelocity();	

	}
	else
	{
		// Arrive behind leader
		Vector2 backOfLeader = ( m_leader->getPosition() - leadDirection * m_distanceArrive );
		double velocitylength = m_entity->getVelocity().Length();
		double slowingDistance = velocitylength * velocitylength / (  m_entity->getMaxForce() / m_entity->getMass());
		Vector2 targetOffset = backOfLeader - m_entity->getPosition();
		double distance = targetOffset.Length();
		double rampedSpeed = m_entity->getMaxSpeed() * ( distance / slowingDistance );
		double clippedSpeed = Tools::min(rampedSpeed, m_entity->getMaxSpeed());
		m_desiredVelocity = targetOffset * (clippedSpeed / distance );
		m_steering = m_desiredVelocity - m_entity->getVelocity();
		m_steering.Normalize();
		m_steering *= m_entity->getMaxForce();
	}

	return m_steering;
}

Vector2& Swarming::Update( double _dT )
{
	Vector2 velocity = m_entity->getVelocity();
	velocity.Normalize();
	Vector2 velocityTangent(-velocity.y, velocity.x);
	Vector2 vEntityToTarget = m_target->getPosition() - m_entity->getPosition();
	if (vEntityToTarget.LengthSquare() > m_fSwarmDistanceSquare)
	{
		// Outer zone

		// Increase speed to maximum
		m_steering = velocity * m_entity->getMaxForce();

		float fAngle = vEntityToTarget.angle(velocity);
		if (fAngle < 0.1f)
		{
			// Vary the steering as a function of the index of the entity
			double dRandTurn = (Tools::global_counter - m_entity->getIndex()) * 0.01f;
			m_steering += velocityTangent * dRandTurn;
		}
		else
		{
			if (vEntityToTarget.isLeft(velocity))
			{
				m_steering = velocityTangent * -100;
			}
			else
			{
				m_steering = velocityTangent * 100;
			}
		}
	}
	else
	{
		// Inner zone

		if (vEntityToTarget.isLeft(velocity))
		{
			m_steering = velocityTangent * -100;
		}
		else
		{
			m_steering = velocityTangent * 100;
		}
	}
	return m_steering;
}

Vector2& FormationV::Update( double _dT )
{
	Vector2 forward(1.0f, 0.0f);
	Vector2 right(0.0f, 1.0f);
	if (m_bUseLeaderOrientation)
	{
		forward = m_leader->getVelocity().Normalized();
		right = Vector2(forward.y, -forward.x);
	}

	// Add 1 unit per line to have an odd number 
	if (m_nbInLine % 2 == 0)
	{
		m_nbInLine++; 
	}
	// Use minimum between nbInLine and max units
	if (m_nbInLine > m_maxId)
	{
		m_nbInLine = m_maxId;
	}
	
	int idRight = (m_id%m_nbInLine) - m_nbInLine / 2;
	int idBack = m_id / m_nbInLine;
	double fX = idRight * m_distanceMax;
	double fY = abs(fX) * (double) tan(m_angle) + idBack * m_distanceMax; // Fleche + recul
	Vector2 arrivalOffset = right * fX - forward * fY;
	Vector2 arrivalPos = m_leader->getPosition() + arrivalOffset;

	//Arrival
	Vector2 target_offset = arrivalPos - m_entity->getPosition();
	double distance = target_offset.Length();
	double max_speed = m_entity->getMaxSpeed();
	double ramped_speed = max_speed * (distance / m_slowingDistance);
	double clipped_speed = (ramped_speed < max_speed) ? ramped_speed : max_speed;
	Vector2 desired_velocity = target_offset * (clipped_speed / distance);
	m_steering = desired_velocity - m_entity->getVelocity();

	return m_steering;
}

Vector2& FormationCircle::Update( double _dT )
{
	Vector2 forward(1.0f, 0.0f);
	Vector2 right(0.0f, 1.0f);
	if (m_bUseLeaderOrientation)
	{
		forward = m_leader->getVelocity().Normalized();
		right = Vector2(forward.y, -forward.x);
	}

	int idInCircle = m_id%m_nbInCircle;
	int iCircle = m_id/m_nbInCircle;
	int idMax = (m_maxAngle - m_minAngle) == 360.0f ? m_nbInCircle : m_nbInCircle - 1;
	double angle = Tools::lerp(Tools::degreetoradian(m_minAngle), Tools::degreetoradian(m_maxAngle), ((double)(idInCircle) / (double)(idMax)));
	double distanceToLeader = m_minRadius + iCircle * m_distanceMax;
	double fX = sin(angle) * distanceToLeader;
	double fY = cos(angle) * distanceToLeader;
	Vector2 arrivalOffset = right * fX + forward * fY;
	Vector2 arrivalPos = m_leader->getPosition() + arrivalOffset;

	//Arrival
	Vector2 target_offset = arrivalPos - m_entity->getPosition();
	double distance = target_offset.Length();
	double max_speed = m_entity->getMaxSpeed();
	double ramped_speed = max_speed * (distance / m_slowingDistance);
	double clipped_speed = (ramped_speed < max_speed) ? ramped_speed : max_speed;
	Vector2 desired_velocity = target_offset * (clipped_speed / distance);
	m_steering = desired_velocity - m_entity->getVelocity();

	return m_steering;
}

Vector2& FormationDynamic::Update(double _dT)
{
	Vector2 forward(1.0f, 0.0f);
	Vector2 right(0.0f, 1.0f);
	if (m_bUseLeaderOrientation)
	{
		forward = m_leader->getVelocity().Normalized();
		right = Vector2(forward.y, -forward.x);
	}

	//Move slots
	m_angleStart += _dT;

	//Circle
	int idInCircle = m_id%m_nbInCircle;
	int iCircle = m_id / m_nbInCircle;
	int idMax = (m_maxAngle - m_minAngle) == 360.0f ? m_nbInCircle : m_nbInCircle - 1;
	double angle = m_angleStart + Tools::lerp(Tools::degreetoradian(m_minAngle), Tools::degreetoradian(m_maxAngle), ((double)(idInCircle) / (double)(idMax)));
	double distanceToLeader = m_minRadius + iCircle * m_distanceMax;
	double fX = sin(angle) * distanceToLeader;
	double fY = cos(angle) * distanceToLeader;
	Vector2 arrivalOffset = right * fX + forward * fY;
	Vector2 arrivalPos = m_leader->getPosition() + arrivalOffset;

	//Arrival
	Vector2 target_offset = arrivalPos - m_entity->getPosition();
	double distance = target_offset.Length();
	double max_speed = m_entity->getMaxSpeed();
	double ramped_speed = max_speed * (distance / m_slowingDistance);
	double clipped_speed = (ramped_speed < max_speed) ? ramped_speed : max_speed;
	Vector2 desired_velocity = target_offset * (clipped_speed / distance);
	m_steering = desired_velocity - m_entity->getVelocity();

	return m_steering;
}