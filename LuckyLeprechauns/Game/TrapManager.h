#pragma once
#include "LLGameComponent.h"
#include "LuckyLeprechauns.h"
#include "TrapController.h"

class TrapManager : public LLGameComponent
{
public:
	typedef std::map<unsigned, TrapController*> TrapMap;
	typedef std::map<unsigned, TrapMap> RemoteTrapMap;
	typedef std::list<Vector3> ExplosionList;

	TrapManager(LuckyLeprechauns* game);
	~TrapManager();

	void init();

	void addTrap(const Vector3& position);
	void addTrap(TrapController* trap, unsigned playerId, unsigned trapId);

	void removeTrap(unsigned playerId, unsigned trapId);

	const TrapMap& getLocalTraps() const { return localTraps; }

	const RemoteTrapMap& getRemoteTraps() const { return remoteTraps; }
	RemoteTrapMap& getRemoteTraps() { return remoteTraps; }

	const ExplosionList& getExplosions() const { return explosions; }
	void clearExplosions();

	TrapController* createTrap();
private:
	unsigned trapId;

	float trapScale;

	TrapMap localTraps;
	RemoteTrapMap remoteTraps;
	ExplosionList explosions;
};

