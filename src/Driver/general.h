#pragma once
#include "pch.h"

/* Offsets */
#define dwViewMatrix          0xEC9780
#define dwEntityList          0x12043CC
#define dwLocalTeam           0x100DF4
#define m_entityOrigin        0x184
#define m_stateValue          0x17C
#define m_entityModel         0x12C
#define m_entitySize          0x250

/* Settings */
#define MAX_ENTITIES          64
#define AIMBOT_KEY            0x1		// VK_LBUTTON

// change these if aimbot points below or higher than player head
// or want to make bigger esp box or something
#define AIMBOT_HEAD           aimposition		// Head position (relatively of the player origin)
#define ESP_BOX_BOTTOM        30.f				// Box bottom position (relatively of the player origin) 
#define ESP_BOX_TOP           25.f				// Box top position (relatively of the player origin)
#define AIMRIFLEKEY           0x70      // F1
#define AIMPISTOLKEY          0x71      // F2
#define AIMAWPKEY             0x72      // F3
#define AIMSMGKEY             0x73      // F4
#define AIMBOT_TOGGLE_KEY     0x75		// F6
#define ESP_TOGGLE_KEY        0x76		// F7
#define ONLY_ENEMIES_KEY      0x77		// F8
#define NOSPREAD_ONTOGGLE_KEY 0x78		// F9
#define NOSPREAD_ONTOGGLE_KEY2 0x2E		// DEL
#define NOSPREAD_OFF_KEY      0x79		// F10
#define SMOOTH_UP_KEY         0x21		// VK_PRIOR
#define SMOOTH_DOWN_KEY       0x22		// VK_NEXT
#define FOV_UP_KEY            0x24		// VK_HOME
#define FOV_DOWN_KEY          0x23		// VK_END
#define AIMHEADPOSITIONKEY    0x6D		// VK_SUBTRACT
#define AIMCHESTPOSITIONKEY   0x6B		// VK_ADD



static RGB box_color = { 0.0f, 255.0f, 255.0f };
static float aimbot_smooth = 5.f;
static int aimbot_fov = 50;
static float aimposition = 25.f;
static float faimposition = 25.f;

/* Global variables */
static CHAR* gProcessName = "hl.exe";
PEPROCESS gProcess;
ULONG64 gPid;

/* Others */
#define DBG 1
#if DBG
#define DebugPrint(x,...) DbgPrint("[zodiak] " x, __VA_ARGS__)
#else
#define DebugPrint(x,...)
#endif

// sb: There is no useless things in programming !
// this big foot nygga:
#define __FLTUSED__
__declspec(selectany) int _fltused = 1;
// virgin nerd in the corner: "fltused is important flag for compiler to allow support float !!!" 