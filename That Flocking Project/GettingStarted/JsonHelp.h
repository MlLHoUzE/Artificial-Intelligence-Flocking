#pragma once
#include <string>

#include "json\json.h"
//#include "../Classes/Vectors.h"
//#include "../Classes/Other Things.h"
#include "externals.h"
#include "structs.h"
#include "cPlayer.h"
#include "cAreaInfo.h"
#include "cCamera.h"
#include <vector>

namespace JsonHelp
{
	// helper: isNumeric()
	inline bool ValidNumeric(Json::Value& n0) { return n0.isNumeric(); }
	inline bool ValidNumeric(Json::Value& n0, Json::Value& n1) { return ValidNumeric(n0) && ValidNumeric(n1); }
	inline bool ValidNumeric(Json::Value& n0, Json::Value& n1, Json::Value& n2) { return ValidNumeric(n0, n1) && ValidNumeric(n2); }
	inline bool ValidNumeric(Json::Value& n0, Json::Value& n1, Json::Value& n2, Json::Value& n3) { return ValidNumeric(n0, n1) && ValidNumeric(n2, n3); }
	
	// helper: isObject()
	inline bool ValidObject(Json::Value& json0) { return json0.isObject(); };
	inline bool ValidObject(Json::Value& json0, Json::Value& json1) { return ValidObject(json0) && ValidObject(json1); };
	inline bool ValidObject(Json::Value& json0, Json::Value& json1, Json::Value& json2) { return ValidObject(json0, json1) && ValidObject(json2); };
	inline bool ValidObject(Json::Value& json0, Json::Value& json1, Json::Value& json2, Json::Value& json3) { return ValidObject(json0, json1) && ValidObject(json2, json3); };
	// helper: isString()
	inline bool ValidString(Json::Value& str0) { return str0.isString(); };
	inline bool ValidString(Json::Value& str0, Json::Value& str1) { return ValidString(str0) && str1.isString(); };
	
	// Load options
	bool Load(const char* fileName, Json::Value& jsonOut);
	bool Load(const char* fileName, Json::Value& jsonOut, std::string& errors);
	bool Load(const std::string& fileName, Json::Value& jsonOut);
	bool Load(const std::string& fileName, Json::Value& jsonOut, std::string& errors);
	
	// Set values for vectors
	bool Set(Json::Value& json, glm::vec2& vec);
	bool Set(Json::Value& json, glm::vec3& vec);
	bool Set(Json::Value& json, glm::vec4& vec);
	// I could see this being handy for colours? Maybe I'm trying to think too far ahead
	bool Set(Json::Value& json, glm::vec4& vec, float defaultW);
	// Set values for components
	bool Set(Json::Value& json, sRenderingInfo& comp);
	bool Set(Json::Value& json, sPhysicsInfo& comp);
	// Set values for an area
	bool Set(Json::Value& json, cAreaInfo& area);
	// Set values for a game object
	bool Set(Json::Value& json, cPlayer& obj);
	bool Set(Json::Value& json, cCamera& obj);
	bool Set(Json::Value& json, sEnemyDesc& obj);
	// Special Randomized group of enemies might be a good idea
	// group: output
	//bool EnemyGroup(Json::Value& json, std::vector<cGameObject*>& group, const cArea& area);
}