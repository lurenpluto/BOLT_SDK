#include "StdAfx.h"
#include ".\BoltFoxApp.h"

BoltFoxApp::BoltFoxApp(void)
{
}

BoltFoxApp::~BoltFoxApp(void)
{

}



bool BoltFoxApp::Init()
{
	
	return true;
}

bool BoltFoxApp::Uinit()
{
	

	return true;
}

BoltFoxApp* BoltFoxApp::GetInstance()
{
	static BoltFoxApp s_instance;

	return &s_instance;
}

bool BoltFoxApp::Quit(int exitCode)
{
	::PostQuitMessage(exitCode);

	return true;
}
