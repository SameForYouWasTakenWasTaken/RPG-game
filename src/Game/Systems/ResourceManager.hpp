#pragma once

#include "Engine/Systems/ISystem.hpp"
#include "Global/Helpers.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <exception>
#include <filesystem>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include "Global/Types.hpp"

namespace Game::Systems
{
    class ResourceManager : public Core::Systems::ISystem
    {
        static inline std::unordered_map<Global::Types::TextureID, sf::Texture> m_TexturesHolder;
    public:
        ResourceManager() = default;

        static Global::Types::TextureHandle LoadTexture(const std::filesystem::path& path)
        {
            std::string absolutePath = std::filesystem::absolute(path).string();

            auto id = Global::Hash<Global::Types::TextureID>(absolutePath);
            auto [it, inserted] = m_TexturesHolder.try_emplace(id);
            
            if (inserted)
            {
                if (!it->second.loadFromFile(path))
                    return Global::Types::TextureHandle{
                            0, 
                            Global::Types::TextureStatus::NoFile
                        };
            }

            return Global::Types::TextureHandle{id, Global::Types::TextureStatus::Valid};
        };

        static sf::Texture& GetTexture(Global::Types::TextureHandle handle)
        {
            if (!handle.IsValid())
                throw std::runtime_error("Invalid texture!");

            return m_TexturesHolder.at(handle.id);
        }
    };
}