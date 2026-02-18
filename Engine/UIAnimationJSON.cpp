#include "UIAnimationJSON.h"

void UIAnimationJSON::loadJSONC()
{
    for(auto& [AtlasName, UIList] : jsonFile.items())
    {
        texID = AtlasName;
        for (auto& [UIName, Infos] : UIList.items())
        {
            UIAnimInfo uiInfo;
            uiInfo.textureID = texID;
            uiInfo.fixedFrame = Infos["fixedFrame"];
            uiInfo.loop = Infos["loop"];
            uiInfo.atlasPosition.x = Infos["atlasPosition"]["srcX"];
            uiInfo.atlasPosition.y = Infos["atlasPosition"]["srcY"];

            if (uiInfo.fixedFrame)
            {
                uiInfo.size.w = Infos["size"];
                uiInfo.size.h = Infos["size"];
            }
            else
            {
                uiInfo.size.w = Infos["width"];
                uiInfo.size.h = Infos["height"];
            }

            for (auto& info : Infos["frames"])
            {
                Vector2D srcRect;
                srcRect.x = info["srcX"];
                srcRect.y = info["srcY"];
                uiInfo.frames.push_back(std::move(srcRect));
            }
            UIAnimMap[UIName] = uiInfo;
        }
    }
}

UIAnimInfo* UIAnimationJSON::search(const std::string& name)
{
    auto it = UIAnimMap.find(name);
    if (it == UIAnimMap.end())
    {
        std::cout << "UI Animation '" << name << "' not found\n";
        return nullptr;
    }
    return &it->second;
}