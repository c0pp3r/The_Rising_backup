/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef GMPLAYCLIENTEFFECTCOMMAND_H_
#define GMPLAYCLIENTEFFECTCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/packets/object/PlayClientEffectObjectMessage.h"
#include "server/zone/packets/scene/PlayClientEffectLocMessage.h"
#include "server/zone/packets/scene/PlayClientEventObjectMessage.h"

class GmPlayClientEffectCommand : public QueueCommand {
public:

	GmPlayClientEffectCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if( !creature->isPlayerCreature() )
			return GENERALERROR;

		StringTokenizer args(arguments.toString());

		ManagedReference<PlayerObject*> ghost = creature->getPlayerObject();

		if (ghost == NULL || !ghost->isPrivileged()) {
			creature->sendSystemMessage("@error_message:insufficient_permissions"); //You do not have sufficient permissions to perform the requested action.
			return INSUFFICIENTPERMISSION;
		}

		if( !args.hasMoreTokens() ){
			creature->sendSystemMessage("format is /gmPlayClientEffect clienteffect/int_camshake_light.cef");
			return INVALIDPARAMETERS;
		}
		//Translate String
		String clienteffect;
		args.getStringToken(clienteffect);

		//Search for CreatureObject
		ManagedReference<SceneObject*> targetObject = server->getZoneServer()->getObject(creature->getTargetID());
		ManagedReference<CreatureObject*> targetCreature = NULL;
		if (targetObject != NULL && targetObject->isCreature()) {
			targetCreature = cast<CreatureObject*>( targetObject.get());
			targetCreature->playEffect(clienteffect, "head");
		} else {
			creature->playEffect(clienteffect, "head");
		}
		return SUCCESS;
	}
};

#endif //GMPLAYCLIENTEFFECTCOMMAND_H_
