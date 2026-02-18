#include "SkillJSON.h"

void SkillJSON::loadJSONC() 
{
    for (auto& [familyKey, familyObj] : jsonFile.items()) 
    {
        FamilyData family;
        family.familyName = familyKey;
        
        family.animationPrefix = familyObj["animationPrefix"];
        
        for (auto& moveObj : familyObj["moves"]) 
        {
            MoveData move;
            move.name = moveObj["name"];
            move.baseDamage = moveObj["baseDamage"];
            move.stunRate = moveObj["stunRate"];
            std::string attackType = moveObj.value("attackType", "Light");
            if (attackType == "Light") move.attackType = AttackType::Light;
            else if (attackType == "Medium") move.attackType = AttackType::Medium;
            else if (attackType == "Heavy") move.attackType = AttackType::Heavy;
            
            move.role = moveObj["role"];
            move.bannedStatuses = moveObj["bannedStatuses"];

            family.moves.push_back(move);
        }

        familyVec.push_back(family);
    }
}