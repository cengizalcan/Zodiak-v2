#pragma once
#pragma warning(disable: 6011) // dereferencing null pointer bla bla bla
#include "pch.h"

extern bool bEsp;
extern bool bAimbot;
extern bool NoSpread;
extern bool NoSpread2;
extern bool bOnlyEnemies;



void DataInitialize();
void DataLoop();