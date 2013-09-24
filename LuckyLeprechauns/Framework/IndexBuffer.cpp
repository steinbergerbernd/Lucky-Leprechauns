#include "IndexBuffer.h"


const char* IndexBuffer::getLockFailedException() const
{
	return Exceptions::indexBufferLockFailed;
}


const char* IndexBuffer::getUnlockFailedException() const
{
	return Exceptions::indexBufferUnlockFailed;
}


const char* IndexBuffer::getGetDescFailedException() const
{
	return Exceptions::indexBufferGetDescFailed;
}
