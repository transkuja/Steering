/***************************************************/
/* Nom:	Messages.h
/* Description: Messages
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __Messages_H_
#define __Messages_H_

//Add new messages here
typedef enum {
	MSG_NULL,
	MSG_Randomize,
	MSG_Timeout,
	MSG_ChangeState,

	// Steering msg
	MSG_Seek,
	MSG_Flee,
	MSG_Pursuit,
	MSG_Evasion,

	MSG_Arrival,
	MSG_ObstacleAvoidance,
	MSG_Wander,
	MSG_PathFollowing,

	MSG_UnalignedCollisionAvoidance,
	MSG_Separation,
	MSG_Cohesion,
	MSG_Alignment,

	MSG_Flocking,
	MSG_LeadFollowing,
	MSG_FormationV,
	MSG_FormationLine,

	MSG_FormationCircle,
	MSG_Formation2Lvl,
	MSG_FormationOfFormations,
	MSG_FormationDynamic,

	MSG_Kill,
	MSG_Stop,
	MSG_GoToWithGold,
	MSG_GoToWithLumber,
	MSG_GoTo,
	MSG_Build,
	MSG_Mine,
	MSG_Harvest,
	MSG_Die,
	MSG_Boost,
	MSG_Hit,
	MSG_HitStop,
} MSG_Name;

#endif
