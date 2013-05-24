#pragma once

class MineSweeperApp
{
public:
	MineSweeperApp(void);
	~MineSweeperApp(void);

	void Quit();

	static MineSweeperApp *Instance();
};
