#include <sampgdk/interop.h>
#include <sampgdk/a_vehicles.h>

#include "MapFunctions.h"

AMX_NATIVE g_streamer_create_object;
AMX_NATIVE g_streamer_destroy_object;
AMX_NATIVE g_streamer_updateex;

void _initStreamerFunctions()
{
	static bool inited = false;
	if (inited) return;

	g_streamer_create_object = sampgdk_FindNative("CreateDynamicObject");
	g_streamer_destroy_object = sampgdk_FindNative("DestroyDynamicObject");
	g_streamer_updateex = sampgdk_FindNative("Streamer_UpdateEx");

	if (!g_streamer_create_object) // check the first one is enough.
	{
		std::cout << "Warning: Streamer plugin may not be loaded.\n";
	}
	else
	{
		inited = true;
	}
}

int createObject(const ObjectInfo& obj)
{
	_initStreamerFunctions();
	return sampgdk_InvokeNative(g_streamer_create_object, "iffffffiiiff",
		obj.model, obj.x, obj.y, obj.z, obj.rx, obj.ry, obj.rz,
		-1, -1, -1, 500.0f, 0.0f);
}

int destroyObject(int id)
{
	_initStreamerFunctions();
	return sampgdk_InvokeNative(g_streamer_destroy_object, "i", id);
}

int createVehicle(const VehicleInfo& veh)
{
	return CreateVehicle(veh.model, veh.x, veh.y, veh.z, veh.angle, veh.color1, veh.color2, 120);
}

int destroyVehicle(int id)
{
	return DestroyVehicle(id);
}

int updatePlayerObject(int playerid, float x, float y, float z)
{
	_initStreamerFunctions();
	return sampgdk_InvokeNative(g_streamer_updateex, "ifff", playerid, x, y, z);
}
