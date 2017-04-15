#include "JsonHelp.h"
#include <iostream>
#include <fstream>

namespace JsonHelp
{
	bool Load(const char* fileName, Json::Value& jsonOut)
	{
		std::string errors;
		if (Load(fileName, jsonOut, errors))
		{
			return true;
		}
		else
		{
			std::cout << "WARNING: json file '" << fileName << "' failed to load." << std::endl;
			std::cout << "\t" << "Errors: " << errors << std::endl;
			return false;
		}
	}
	bool Load(const char* fileName, Json::Value& jsonOut, std::string& errors)
	{
		std::ifstream jsonFile(fileName, std::ifstream::binary);
		Json::CharReaderBuilder charReaderBuilder;
		std::string errs;
		return Json::parseFromStream(charReaderBuilder, jsonFile, &jsonOut, &errors);
	}
	bool Load(const std::string& fileName, Json::Value& jsonOut)
	{
		return Load(fileName.c_str(), jsonOut);
	}
	bool Load(const std::string& fileName, Json::Value& jsonOut, std::string& errors)
	{
		return Load(fileName.c_str(), jsonOut, errors);
	}

	bool Set(Json::Value& json, glm::vec2& vec)
	{
		if (!ValidObject(json)) return false;
		Json::Value x = json["x"];
		Json::Value y = json["y"];
		if (!ValidNumeric(x, y)) return false;
		vec.x = x.asFloat();
		vec.y = y.asFloat();
		return true;
	}
	bool Set(Json::Value& json, float& value)
	{
		if (!ValidObject(json)) return false;
		Json::Value x = json["Value"];
		value = x.asFloat();
		return true;
	}
	bool Set(Json::Value& json, int& value)
	{
		if (!ValidObject(json)) return false;
		Json::Value x = json["Value"];
		value = x.asInt();
		return true;
	}
	bool Set(Json::Value& json, glm::vec3& vec)
	{
		if (!ValidObject(json)) return false;
		Json::Value x = json["x"];
		Json::Value y = json["y"];
		Json::Value z = json["z"];
		if (!ValidNumeric(x, y, z)) return false;
		vec.x = x.asFloat();
		vec.y = y.asFloat();
		vec.z = z.asFloat();
		return true;
	}
	bool Set(Json::Value& json, glm::vec4& vec)
	{
		if (!ValidObject(json)) return false;
		Json::Value x = json["x"];
		Json::Value y = json["y"];
		Json::Value z = json["z"];
		Json::Value w = json["w"];
		if (!ValidNumeric(x, y, z, w)) return false;
		vec.x = x.asFloat();
		vec.y = y.asFloat();
		vec.z = z.asFloat();
		vec.w = w.asFloat();
		return true;
	}
	bool Set(Json::Value& json, glm::quat& quat)
	{
		if (!ValidObject(json)) return false;
		Json::Value x = json["x"];
		Json::Value y = json["y"];
		Json::Value z = json["z"];
		Json::Value w = json["w"];
		if (!ValidNumeric(x, y, z, w)) return false;
		quat.x = x.asFloat();
		quat.y = y.asFloat();
		quat.z = z.asFloat();
		quat.w = w.asFloat();
		return true;
	}
	bool Set(Json::Value& json, glm::vec4& vec, float defaultW)
	{
		if (!ValidObject(json)) return false;
		Json::Value x = json["x"];
		Json::Value y = json["y"];
		Json::Value z = json["z"];
		Json::Value w = json["w"];
		if (!ValidNumeric(x, y, z)) return false;
		vec.x = x.asFloat();
		vec.y = y.asFloat();
		vec.z = z.asFloat();
		if (ValidNumeric(w)) vec.w = w.asFloat();
		else vec.w = defaultW;
		return true;
	}
	bool Set(Json::Value& json, sRenderingInfo& comp)
	{
		if (!ValidObject(json)) return false;
		// name the mesh! that's really the only important thing here
		Json::Value meshName = json["MeshName"];
		if (!ValidString(meshName)) return false;
		comp.MeshName = meshName.asString();
		Json::Value scale = json["Scale"];
		Json::Value position = json["Position"];
		Json::Value colour = json["Colour"];
		Set(scale, comp.scale);  // dun curr if it's not there
		Set(colour, comp.Color, 1.f); // hey that was useful!
		return true;
	}
	bool Set(Json::Value& json, sPhysicsInfo& comp)
	{
		if (!ValidObject(json)) return false;
		// position is the only absolutely necessary thing for me
		Json::Value position = json["Position"];
		if (!Set(position, comp.Position)) return false;
		Json::Value rotation = json["Rotation"];
		Set(rotation, comp.Orientation);
		Json::Value velocity = json["Velocity"];
		Set(velocity, comp.Velocity);
		Json::Value speed = json["Speed"];
		Set(speed, comp.speed);
		// I'll assume here that acceleration is never set initially
		return true;
	}
	bool Set(Json::Value& json, cAreaInfo& area)
	{
		if (!ValidObject(json)) return false;
		Json::Value maxBounds = json["MaxBounds"];
		Json::Value minBounds = json["MinBounds"];
		return Set(maxBounds, area.Max) && Set(minBounds, area.Min);
	}
	bool Set(Json::Value& json, cPlayer& obj)
	{
		if (!ValidObject(json)) return false;
		// let's say physics aren't necesarily required
		Json::Value physics = json["Physics"];
		Set(physics, obj.physicsInfo); // we don't care about the result
		
		return true;
	}
	bool Set(Json::Value& json, cCamera& obj)
	{
		if (!ValidObject(json)) return false;
		// let's say physics aren't necesarily required
		Json::Value offset = json["Offset"];
		Set(offset, obj.mOffset); // we don't care about the result

		return true;
	}
	bool Set(Json::Value& json, sEnemyDesc& obj)
	{
		if (!ValidObject(json)) return false;
		// let's say physics aren't necesarily required
		Json::Value numberToSpawn = json["Count"];
		obj.numberOfEnemies = numberToSpawn.asInt();
		Json::Value minSpeed = json["MinSpeed"];
		obj.minSpeed = minSpeed.asFloat();
		Json::Value maxSpeed = json["MaxSpeed"];
		obj.maxSpeed = maxSpeed.asFloat();

		return true;
	}
}