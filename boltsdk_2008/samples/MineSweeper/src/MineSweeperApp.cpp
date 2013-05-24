#include "StdAfx.h"
#include ".\MineSweeperApp.h"

MineSweeperApp::MineSweeperApp(void)
{
}

MineSweeperApp::~MineSweeperApp(void)
{
}

void MineSweeperApp::Quit()
{
	::PostQuitMessage(0);
}

MineSweeperApp * MineSweeperApp::Instance()
{
	static MineSweeperApp ideApp;
	return &ideApp;
}