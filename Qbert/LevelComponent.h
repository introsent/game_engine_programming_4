#pragma once
#include "Component.h"
#include "Level.h"
#include <vector>
#include <memory>
#include "utils.h"
#include <unordered_map>

class LevelComponent final : public dae::Component {
public:
    explicit LevelComponent(dae::GameObject* pOwner, int levelIndex);
    ~LevelComponent() override = default;

    void Update(float deltaTime) override;
    void Render() const override {}

    const glm::ivec2 UP_LEFT{ -1, -1 };
    const glm::ivec2 UP_RIGHT{ 0, -1 };
    const glm::ivec2 DOWN_LEFT{ 0, 1 };
    const glm::ivec2 DOWN_RIGHT{ 1, 1 };

private:
    std::unique_ptr<Level> m_pLevel;
    std::vector<std::shared_ptr<dae::GameObject>> m_TileGOs;
    std::shared_ptr<dae::GameObject> m_pQBertGO;
    std::vector<std::shared_ptr<dae::GameObject>> m_DiscGOs;

    std::vector<std::pair<glm::ivec2, std::shared_ptr<dae::GameObject>>> m_DiscList;

    //Atlas functions
    void SpawnTiles();
    void OnTileColored(const Tile& tile) const;

	//Qbert functions
    void SpawnQBert();
    void BindCommands() const;

    void SpawnDiscs();
 

    // Texture parameters
    static constexpr char const* TileAtlasFile = "../Data/Qbert Cubes.png";
    static constexpr glm::vec2 MapFrameSize = { 32.f, 32.f };

    static constexpr char const* QbertFile = "../Data/Qbert P1 Spritesheet.png";
    static constexpr glm::vec2 QbertFrameSize = { 17.f, 16.f };

    static constexpr char const* DiscFile = "../Data/Disk Spritesheet.png";
    static constexpr glm::vec2 DiscFrameSize = { 16.f, 10.f };

    static constexpr int AtlasColumns = 6;
    static constexpr int AtlasRows = 3;
};