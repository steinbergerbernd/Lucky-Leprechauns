#include "VertexBuffer.h"


const char* VertexBuffer::getLockFailedException() const
{
	return Exceptions::vertexBufferLockFailed;
}


const char* VertexBuffer::getUnlockFailedException() const
{
	return Exceptions::vertexBufferUnlockFailed;
}


const char* VertexBuffer::getGetDescFailedException() const
{
	return Exceptions::vertexBufferGetDescFailed;
}
