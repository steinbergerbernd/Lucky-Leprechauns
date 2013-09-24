#include "StateBlock.h"


void StateBlock::apply() const
{
	Validator::validate(resource->Apply(), Exceptions::stateBlockApplyFailed);
}