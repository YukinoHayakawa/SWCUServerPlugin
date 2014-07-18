/*
 * Copyright 2014 Yukino Hayakawa<tennencoll@gmail.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>

#include "MapInfo.h"

std::ostream& operator<<(std::ostream& out, const ObjectInfo& obj)
{
	out << "Object(" << obj.model << ", " << obj.x << ", " << obj.y << ", " << obj.z << ", "
		<< obj.rx << ", " << obj.ry << ", " << obj.rz << ")";
	return out;
}

std::ostream& operator<<(std::ostream& out, const VehicleInfo& veh)
{
	out << "Vehicle(" << veh.model << ", " << veh.x << ", " << veh.y << ", " << veh.z << ", "
		<< veh.angle << ", " << veh.color1 << ", " << veh.color2 << ")";
	return out;
}

std::ostream& operator<<(std::ostream& out, const MapInfo& map)
{
	out << "Map(" << map.name << ", objects: " << map.objects.size() << ", vehicles: " << map.vehicles.size() << ", autoload: " << map.autoload << ")";
	return out;
}
