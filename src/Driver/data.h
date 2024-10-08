#pragma once
#pragma warning(disable: 6011) // dereferencing null pointer bla bla bla
#include "pch.h"

extern bool bEsp;
extern bool bAimbot;
extern bool bOnlyEnemies;
extern bool DefaultBot;
extern bool AutoBot;
extern bool PistolBot;
extern bool AWPBot;




void DataInitialize();
void DataLoop();