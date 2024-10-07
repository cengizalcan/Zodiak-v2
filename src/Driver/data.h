#pragma once
#pragma warning(disable: 6011) // dereferencing null pointer bla bla bla
#include "pch.h"

extern bool bEsp;
extern bool bAimbot;
extern bool bOnlyEnemies;
extern bool RifleBot;
extern bool SMGBot;
extern bool PistolBot;
extern bool AWPBot;
extern bool DefaultBot;



void DataInitialize();
void DataLoop();