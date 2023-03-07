#pragma once

class GrassSystem
{
public:
	GrassSystem(GrassSystem const&) = delete;
	void operator=(GrassSystem const&) = delete;

	static GrassSystem& Instance()
	{
		static GrassSystem instance;

		return instance;
	}

private:
	GrassSystem() {}
};