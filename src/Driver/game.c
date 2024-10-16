#include "game.h"


bool get_hw_base()
{
	hw_dll = GetModuleBase32(gPid, L"\\hw.dll");

	if (hw_dll == 0)
	{
		return false;
	}
	return true;
}

bool get_client_base()
{
	client_dll = GetModuleBase32(gPid, L"\\client.dll");

	if (client_dll == 0)
	{
		return false;
	}
	return true;
}

bool read_view_matrix()
{
	if (!read(hw_dll + dwViewMatrix, &view_matrix, sizeof(view_matrix_t)))
	{
		return false; // used as a check to see if the game is running
	}
	return true;
}

void clear_entity_list()
{
	memset(entity_list, 0, sizeof(entity_list));
}

bool entity_dead(int i)
{
	/*
		Making dead check by entity state
		If player is alive and in draw distance state always increment, we are checking old state and compering with new one
		Why not using 0x178 offset for dead check?
		Its because when player is not in draw distance, it will continuing drawing the last position
	*/
	float state = get_entity_state(i);

	if (!state)
	{
		return true;
	}

	if (!state_list[i])
	{
		state_list[i] = state;
		return true;
	}

	if (state == state_list[i])
	{
		return true;
	}

	state_list[i] = state;

	return false;
}

bool entity_screen(int i, BOX* box)
{
	Vector3 origin = get_entity_origin(i);

	if (origin.x == 0.0f || origin.y == 0.0f || origin.z == 0.0f) {
		return false;
	}

	Vector3 origin_bottom = origin;
	Vector3 origin_top = origin;
	Vector3 origin_head = origin;

	origin_bottom.z -= ESP_BOX_BOTTOM;
	origin_top.z += ESP_BOX_TOP;
	origin_head.z += AIMBOT_HEAD; // should point on the head

	Vector2 screen_bottom, screen_top, screen_head;

	if (!w2s(screen_size, origin_bottom, &screen_bottom, view_matrix) ||
		!w2s(screen_size, origin_top, &screen_top, view_matrix) ||
		!w2s(screen_size, origin_head, &screen_head, view_matrix))
	{
		return false;
	}

	int height = (int)(screen_bottom.y - screen_top.y);
	int width = (int)(height / 2.4f);
	screen_top.x -= width / 2;

	if (screen_top.x <= 0.0f || screen_top.y <= 0.0f ||
		screen_bottom.x <= 0.0f || screen_bottom.y <= 0.0f)
	{
		return false;
	}

	box->h = height;
	box->w = width;
	box->x = screen_top.x;
	box->y = screen_top.y;

	entity_list[i] = screen_head;

	return true;
}

int get_player_team()
{
	int team;
	read(client_dll + dwLocalTeam, &team, sizeof(int));

	return team;
}

bool entity_teammate(int i)
{
	int team = get_player_team();
	char* model = get_entity_model(i);

	/* CT */
	if (team == 2) {
		if (strstr(model, "gign") != NULL ||
			strstr(model, "gsg9") != NULL ||
			strstr(model, "urban") != NULL ||
			strstr(model, "sas") != NULL)
		{
			return true;
		}
	}

	/* T */
	else if (team == 1) {
		if (strstr(model, "terror") != NULL ||
			strstr(model, "leet") != NULL ||
			strstr(model, "arctic") != NULL ||
			strstr(model, "guerilla") != NULL)
		{
			return true;
		}
	}

	return false;
}

Vector2 get_closest_entity()
{
	Vector2 closest_enemy = { 0 };
	double closestDist = 999999.f;

	Vector2 center = { screen_size.x / 2, screen_size.y / 2 };

	int fovHalf = aimbot_fov / 2;

	int lowerX = (int)center.x - fovHalf;
	int upperX = (int)center.x + fovHalf;
	int lowerY = (int)center.y - fovHalf;
	int upperY = (int)center.y + fovHalf;

	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		if (entity_list[i].x || entity_list[i].y)
		{
			if (entity_list[i].x > lowerX && entity_list[i].x < upperX && entity_list[i].y > lowerY && entity_list[i].y < upperY)
			{
				double dist = distance((int)entity_list[i].x, (int)entity_list[i].y, (int)center.x, (int)center.y);
				if (dist < closestDist)
				{
					closestDist = dist;
					closest_enemy = entity_list[i];
				}
			}
		}
	}

	return closest_enemy;
}

char* get_entity_model(int i)
{
	static char model[10];
	read(hw_dll + dwEntityList + (i * m_entitySize) + m_entityModel, &model, sizeof(model));

	return model;
}

float get_entity_state(int i)
{
	float state;
	read(hw_dll + dwEntityList + (i * m_entitySize) + m_stateValue, &state, sizeof(float));

	return state;
}

Vector3 get_entity_origin(int i)
{
	Vector3 origin;
	read(hw_dll + dwEntityList + (i * m_entitySize) + m_entityOrigin, &origin, sizeof(Vector3));

	return origin;
}


void HandleKeyInputs()
{
	{
		/* Aimbot on/off */
		static count = 0;
		if (NtUserGetAsyncKeyState(AIMBOT_TOGGLE_KEY)) // F6
		{
			count++;
			if (count == 1)
			{
				bAimbot = !bAimbot;
			}
		}
		else
		{
			count = 0;
		}
	}

	{
		/* Esp on/off */
		static count = 0;
		if (NtUserGetAsyncKeyState(ESP_TOGGLE_KEY)) // F7
		{
			count++;
			if (count == 1)
			{
				bEsp = !bEsp;
			}
		}
		else
		{
			count = 0;
		}
	}

	{
		/* Esp Push on/off */
		static count = 0;
		if (NtUserGetAsyncKeyState(ESP_PUSH_ONOFF_KEY)) // RIGHT SHIFT and use LEFT SHIFT FOR PUSH ESP
		{
			count++;
			if (count == 1)
			{
				bEspush = !bEspush;
			}
		}
		else
		{
			count = 0;
		}
	}

	{
		/* Only Enemies on/off */
		static count = 0;
		if (NtUserGetAsyncKeyState(ONLY_ENEMIES_KEY)) // F8
		{
			count++;
			if (count == 1)
			{
				bOnlyEnemies = !bOnlyEnemies;
			}
		}
		else
		{
			count = 0;
		}
	}

	{
		/* NoSpread on/off */
		static count = 0;
		if (NtUserGetAsyncKeyState(NOSPREAD_ONTOGGLE_KEY)) // F9
		{
			count++;
			if (count == 1)
			{
				AutoBot = !AutoBot;
				PistolBot = !PistolBot;
				AWPBot = !AWPBot;
				DefaultBot = !DefaultBot;
			}
		}
		else
		{
			count = 0;
		}
	}

	/* Aimbot */
	if (bAimbot)
	{
		if (NtUserGetAsyncKeyState(AIMBOT_KEY)) //MOUSE1
		{
			Vector2 closest_enemy = get_closest_entity();

			if (closest_enemy.x != 0 && closest_enemy.y != 0)
			{
				aim(closest_enemy, screen_size, aimbot_smooth);
			}
		}
	}

	/* Default Bot Active */
	if (DefaultBot)
	{
		if (NtUserGetAsyncKeyState(AIMBOT_KEY)) //MOUSE1
		{
			aimposition = faimposition;
			aimbot_fov = faimbot_fov;
			aimbot_smooth = faimbot_smooth;
		}
	}

	/* Auto Bot Active */
	if (AutoBot)
	{
		if (NtUserGetAsyncKeyState(AIMBOT_KEY)) //MOUSE1
		{
			if (aimbot_fov < 60)
			{
				aimbot_fov += 5;
			}
			if (aimposition > 20.f)
			{
				aimposition -= 0.25f;
			}
			else
			{
				if (aimposition > -20.f)
				{
					aimposition -= 2.f;
				}
			}
			aimbot_smooth = faimbot_smooth;
		}
		else
		{
			aimposition = faimposition;
			aimbot_fov = faimbot_fov;
		}
	}

	/* Pistol SMG Bot Active */
	if (PistolBot)
	{
		if (NtUserGetAsyncKeyState(AIMBOT_KEY)) //MOUSE1
		{
			if (aimbot_fov < 100)
			{
				aimbot_fov += 5;
			}
			if (aimposition > 15.f)
			{
				aimposition -= 0.25f;
			}
			else
			{
				if (aimposition > 0.f)
				{
					aimposition -= 0.5f;
				}
			}
			aimbot_smooth = faimbot_smooth;
		}
		else
		{
			aimbot_fov = faimbot_fov;
			aimposition = faimposition;
		}
	}

	/* AWP Bot Active */
	if (AWPBot)
	{
		if (NtUserGetAsyncKeyState(AIMBOT_KEY)) //MOUSE1
		{
			if (aimbot_fov < 100)
			{
				aimbot_fov += 5;
			}
			aimposition = faimposition;
			aimbot_smooth = faimbot_smooth;
		}
		else
		{
			aimbot_fov = faimbot_fov;
		}
	}

	{
		/* Aim Default Settings */
		static count = 0;
		if (NtUserGetAsyncKeyState(AIMDEFAULTKEY)) // INSERT
		{
			count++;
			if (count == 1)
			{
				AutoBot = false;
				PistolBot = false;
				AWPBot = false;
				DefaultBot = true;
				aimposition = 22.5f;
				faimposition = aimposition;
				aimbot_fov = 100;
				faimbot_fov = aimbot_fov;
				aimbot_smooth = 2.5f;
				faimbot_smooth = aimbot_smooth;
			}
		}
		else
		{
			count = 0;
		}
	}

	{
		/* Aim Auto Settings */
		static count = 0;
		if (NtUserGetAsyncKeyState(AIMAUTOKEY) || NtUserGetAsyncKeyState(AIMAUTOKEY2)) // F10 or C
		{
			count++;
			if (count == 1)
			{
				AutoBot = true;
				PistolBot = false;
				AWPBot = false;
				DefaultBot = false;
				aimposition = 25.f;
				faimposition = aimposition;
				aimbot_fov = 30;
				faimbot_fov = aimbot_fov;
				aimbot_smooth = 2.5f;
				faimbot_smooth = aimbot_smooth;
			}
		}
		else
		{
			count = 0;
		}
	}

	{
		/* Aim Pistol Settings */
		static count = 0;
		if (NtUserGetAsyncKeyState(AIMPISTOLKEY) || NtUserGetAsyncKeyState(AIMPISTOLKEY2)) // F11 or F
		{
			count++;
			if (count == 1)
			{
				AutoBot = false;
				PistolBot = true;
				AWPBot = false;
				DefaultBot = false;
				aimposition = 22.5f;
				faimposition = aimposition;
				aimbot_fov = 50;
				faimbot_fov = aimbot_fov;
				aimbot_smooth = 2.5f;
				faimbot_smooth = aimbot_smooth;
			}
		}
		else
		{
			count = 0;
		}
	}

	{
		/* Aim AWP Settings */
		static count = 0;
		if (NtUserGetAsyncKeyState(AIMAWPKEY) || NtUserGetAsyncKeyState(AIMAWPKEY2)) // F12 or V
		{
			count++;
			if (count == 1)
			{
				AutoBot = false;
				PistolBot = false;
				AWPBot = true;
				DefaultBot = false;
				aimposition = 10.f;
				faimposition = aimposition;
				aimbot_fov = 50;
				faimbot_fov = aimbot_fov;
				aimbot_smooth = 2.f;
				faimbot_smooth = aimbot_smooth;
			}
		}
		else
		{
			count = 0;
		}
	}

	{
		/* Smooth Increase */
		static count = 0;
		if (NtUserGetAsyncKeyState(SMOOTH_UP_KEY)) // PG_UP
		{
			count++;
			if (count == 1) 
			{
				aimbot_smooth += 0.5f;
				faimbot_smooth = aimbot_smooth;
			}
		}
		else
		{
			count = 0;
		}
	}
	
	{
		/* Smooth Decrease */
		static count = 0;
		if (NtUserGetAsyncKeyState(SMOOTH_DOWN_KEY)) // PG_DOWN
		{
			count++;
			if (count == 1)
			{
				if (aimbot_smooth > 0.f)
				{
					aimbot_smooth -= 0.5f;
					faimbot_smooth = aimbot_smooth;
				}
			}
		}
		else
		{
			count = 0;
		}
	}

	{
		/* Fov Increase */
		static count = 0;
		if (NtUserGetAsyncKeyState(FOV_UP_KEY)) // Home
		{
			count++;
			if (count == 1)
			{
				aimbot_fov += 5;
				faimbot_fov = aimbot_fov;
			}
		}
		else
		{
			count = 0;
		}
	}

	{
		/* Fov Decrease */
		static count = 0;
		if (NtUserGetAsyncKeyState(FOV_DOWN_KEY)) // END
		{
			count++;
			if (count == 1)
			{
				if (aimbot_fov > 0)
				{
					aimbot_fov -= 5;
					faimbot_fov = aimbot_fov;
				}
			}
		}
		else
		{
			count = 0;
		}
	}

	{
		/* Aim Position Upper */
		static count = 0;
		if (NtUserGetAsyncKeyState(POSITION_UP_KEY)) // VK_ADD
		{
			count++;
			if (count == 1)
			{
				aimposition += 1.f;
				faimposition = aimposition;
			}
		}
		else
		{
			count = 0;
		}
	}

	{
		/* Aim Position Downer */
		static count = 0;
		if (NtUserGetAsyncKeyState(POSITION_DOWN_KEY)) // VK_SUBTRACT
		{
			count++;
			if (count == 1)
			{
				aimposition -= 1.f;
				faimposition = aimposition;
			}
		}
		else
		{
			count = 0;
		}
	}
}


/* Converted w2s that I have used in cs2 cheat */
bool w2s(Vector2 screen, Vector3 position, Vector2* out, view_matrix_t matrix)
{
	/*
		Issue with CS 1.6 that view matrix in game is stored in columns, when in newer CS2 is stored with rows
		cs2    -> [0][0]  [0][1]  [0][2]  [0][3]  // First row
		cs 1.6 -> [0][0]  [1][0]  [2][0]  [3][0]  // First column
	*/
	out->x = matrix[0][0] * position.x + matrix[1][0] * position.y + matrix[2][0] * position.z + matrix[3][0];
	out->y = matrix[0][1] * position.x + matrix[1][1] * position.y + matrix[2][1] * position.z + matrix[3][1];
	float w = matrix[0][3] * position.x + matrix[1][3] * position.y + matrix[2][3] * position.z + matrix[3][3];

	if (w < 0.01f)
	{
		return false;
	}

	float inv_w = 1.0f / w;
	out->x *= inv_w;
	out->y *= inv_w;

	out->x = (out->x + 1.0f) * 0.5f * screen.x;
	out->y = (1.0f - out->y) * 0.5f * screen.y;

	return true;
}

void aim(Vector2 position, Vector2 screen, float SmoothAmount)
{
	int TargetX = 0, TargetY = 0;

	if (position.x != 0.0f)
	{
		if (position.x > screen.x / 2)
		{
			TargetX = -(int)((screen.x / 2 - position.x) / SmoothAmount);
			if (TargetX + screen.x / 2 > screen.x) TargetX = 0;
		}

		if (position.x < screen.x / 2)
		{
			TargetX = (int)((position.x - screen.x / 2) / SmoothAmount);
			if (TargetX + screen.x / 2 < 0) TargetX = 0;
		}
	}

	if (position.y != 0.0f)
	{
		if (position.y > screen.y / 2)
		{
			TargetY = -(int)((screen.y / 2 - position.y) / SmoothAmount);
			if (TargetY + screen.y / 2 > screen.y) TargetY = 0;
		}

		if (position.y < screen.y / 2)
		{
			TargetY = (int)((position.y - screen.y / 2) / SmoothAmount);
			if (TargetY + screen.y / 2 < 0) TargetY = 0;
		}
	}

	if (TargetX != 0 || TargetY != 0)
	{
		mouse_move((LONG)(TargetX), (LONG)(TargetY), 0);
	}
}

double distance(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	return sqrt(dx * dx + dy * dy);
}
