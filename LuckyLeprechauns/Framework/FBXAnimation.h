#pragma once

struct FBXAnimation
{
	FBXAnimation() : name(), start(0), length(1), originalFps(1), currentFps(1), action(-1), priority(0) {}
	FBXAnimation(const std::string& name, unsigned start, unsigned length, unsigned fps, unsigned action, unsigned priority) 
		: name(name), start(start), length(length), originalFps(fps), action(action), priority(priority), currentFps(fps) {}

	std::string name;

	unsigned start;
	unsigned length;
	unsigned originalFps;
	unsigned currentFps;
	unsigned action;
	unsigned priority;
};