#pragma once

class BoltFoxApp
{
public:
	BoltFoxApp(void);
	~BoltFoxApp(void);

	static BoltFoxApp* GetInstance();

	bool Init();
	bool Uinit();

	bool Quit(int exitCode);


private:
};
