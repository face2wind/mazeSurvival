#pragma once

#include "explorer.h"

class Runner : public Explorer
{
public:
	Runner(Scene *scene);
	~Runner();

	virtual void Thinking(long long interval);
	
protected:

};
