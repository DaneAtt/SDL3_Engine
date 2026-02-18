    #pragma once
    #include "EngineAPI.h"
    #include <iostream>
    #include <fstream>
    #include <nlohmann/json.hpp>
    #include "JSONTemplate.h"

    using json = nlohmann::json;

    class ENGINE_API SkillJSON : public JSONTemplate
    {
    public:
        enum AttackType { Light, Medium, Heavy };

        struct MoveData {
            std::string name;
            double stunRate;
            AttackType attackType;
            double baseDamage;
            std::string role;  // "apply", "amplify", "none"
            std::vector<std::string> bannedStatuses;
        };

        struct FamilyData {
            std::string familyName;
            std::string animationPrefix;
            std::vector<MoveData> moves;
        };

        std::vector<FamilyData> transfer() { return std::move(familyVec); }

        void loadJSONC() override;

        const char* getClassName() const override { return "SkillJson"; }

    private:
        std::vector<FamilyData> familyVec;
    };