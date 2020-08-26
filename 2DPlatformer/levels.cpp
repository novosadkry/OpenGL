#include <vector>
#include "game.h"

namespace game
{
    std::vector<Level>* levels = new std::vector<Level>
    {
        // Level1
        []()
        {
            std::vector<shape::Shape*>* shapes = new std::vector<shape::Shape*>();

            // Shapes
            glm::vec3 groundPosition = { 0.0f, -0.9f, 0.0f };
            std::vector<glm::vec4> groundVertices =
            {
                {  -1.0f,  -0.1f,  0.0f,  1  },
                {   1.0f,  -0.1f,  0.0f,  1  },
                {   1.0f,   0.1f,  0.0f,  1  },
                {  -1.0f,   0.1f,  0.0f,  1  }
            };
            shape::color groundColor = { 1.0f, 1.0f, 1.0f };
            shape::Polygon* groundShape = new shape::Polygon(groundPosition, groundVertices, groundColor);

            glm::vec3 wall1Position = { 0.7f, -0.4f, 0.0f };
            std::vector<glm::vec4> wall1Vertices =
            {
                {  -0.3f,  -0.4f,  0.0f,  1  },
                {   0.3f,  -0.4f,  0.0f,  1  },
                {   0.3f,   0.2f,  0.0f,  1  },
                {  -0.3f,   0.2f,  0.0f,  1  }
            };
            shape::color wall1Color = { 1.0f, 1.0f, 1.0f };
            shape::Polygon* wall1Shape = new shape::Polygon(wall1Position, wall1Vertices, wall1Color);

            glm::vec3 wall2Position = { 0.1f, -0.6f, 0.0f };
            std::vector<glm::vec4> wall2Vertices =
            {
                {  -0.3f,  -0.2f,  0.0f,  1  },
                {   0.3f,  -0.2f,  0.0f,  1  },
                {   0.3f,   0.2f,  0.0f,  1  },
                {  -0.3f,   0.2f,  0.0f,  1  }
            };
            shape::color wall2Color = { 1.0f, 1.0f, 1.0f };
            shape::Polygon* wall2Shape = new shape::Polygon(wall2Position, wall2Vertices, wall2Color);

            glm::vec3 plane1Position = { -0.5f, 0.5f, 0.0f };
            std::vector<glm::vec4> plane1Vertices =
            {
                {  -0.3f,  -0.05f,  0.0f,  1  },
                {   0.3f,  -0.05f,  0.0f,  1  },
                {   0.3f,   0.05f,  0.0f,  1  },
                {  -0.3f,   0.05f,  0.0f,  1  }
            };
            shape::color plane1Color = { 1.0f, 1.0f, 1.0f };
            shape::Polygon* plane1Shape = new shape::Polygon(plane1Position, plane1Vertices, plane1Color);

            glm::vec3 plane2Position = { 0.2f, 0.2f, 0.0f };
            std::vector<glm::vec4> plane2Vertices =
            {
                {  -0.2f,  -0.05f,  0.0f,  1  },
                {   0.2f,  -0.05f,  0.0f,  1  },
                {   0.2f,   0.05f,  0.0f,  1  },
                {  -0.2f,   0.05f,  0.0f,  1  }
            };
            shape::color plane2Color = { 1.0f, 1.0f, 1.0f };
            shape::Polygon* plane2Shape = new shape::Polygon(plane2Position, plane2Vertices, plane2Color);

            glm::vec3 doorEntryPosition = { -0.8f, -0.7f, 0.0f };
            std::vector<glm::vec4> doorEntryVertices =
            {
                {  -0.06f,  -0.10f,  0.0f,  1  },
                {   0.06f,  -0.10f,  0.0f,  1  },
                {   0.06f,   0.10f,  0.0f,  1  },
                {  -0.06f,   0.10f,  0.0f,  1  }
            };
            shape::color doorEntryColor = { 1.0f, 1.0f, 0.2f };
            shape::Polygon* doorEntryShape = new shape::Polygon(doorEntryPosition, doorEntryVertices, doorEntryColor);
            doorEntryShape->canCollide(false);

            glm::vec3 doorExitPosition = { -0.65f, 0.65f, 0.0f };
            std::vector<glm::vec4> doorExitVertices =
            {
                {  -0.06f,  -0.10f,  0.0f,  1  },
                {   0.06f,  -0.10f,  0.0f,  1  },
                {   0.06f,   0.10f,  0.0f,  1  },
                {  -0.06f,   0.10f,  0.0f,  1  }
            };
            shape::color doorExitColor = { 0.2f, 1.0f, 0.2f };
            shape::Polygon* doorExitShape = new shape::Polygon(doorExitPosition, doorExitVertices, doorExitColor);
            doorExitShape->canCollide(false);

            glm::vec3 playerPosition = { -0.8f, -0.75f, 0.0f };
            std::vector<glm::vec4> playerVertices =
            {
                {  -0.05f,  -0.05f,  0.0f,  1  },
                {   0.05f,  -0.05f,  0.0f,  1  },
                {   0.05f,   0.05f,  0.0f,  1  },
                {  -0.05f,   0.05f,  0.0f,  1  }
            };
            shape::color playerColor = { 0.0f, 1.0f, 1.0f };
            shape::Polygon* playerShape = new shape::Polygon(playerPosition, playerVertices, playerColor);
            playerShape->canCollide(false);

            // Layers
            shapes->push_back(groundShape);
            shapes->push_back(wall1Shape);
            shapes->push_back(wall2Shape);
            shapes->push_back(plane1Shape);
            shapes->push_back(plane2Shape);
            shapes->push_back(doorEntryShape);
            shapes->push_back(doorExitShape);
            shapes->push_back(playerShape);

            game::player = new player::Player(playerShape);
            game::player->setMass(0.6f);

            game::gravity = { 0.0f, -10.0f, 0.0f };
            game::air = 5.0f;

            game::shapes = shapes;
            game::entry = doorEntryShape;
            game::exit = doorExitShape;
        },

        // Level2
        []()
        {
            std::vector<shape::Shape*>* shapes = new std::vector<shape::Shape*>();

            // Shapes
            glm::vec3 groundPosition = { 0.0f, -0.9f, 0.0f };
            std::vector<glm::vec4> groundVertices =
            {
                {  -1.0f,  -0.1f,  0.0f,  1  },
                {   1.0f,  -0.1f,  0.0f,  1  },
                {   1.0f,   0.1f,  0.0f,  1  },
                {  -1.0f,   0.1f,  0.0f,  1  }
            };
            shape::color groundColor = { 1.0f, 1.0f, 1.0f };
            shape::Polygon* groundShape = new shape::Polygon(groundPosition, groundVertices, groundColor);

            glm::vec3 wall1Position = { 0.7f, -0.4f, 0.0f };
            std::vector<glm::vec4> wall1Vertices =
            {
                {  -0.1f,  -0.4f,  0.0f,  1  },
                {   0.1f,  -0.4f,  0.0f,  1  },
                {   0.1f,   0.2f,  0.0f,  1  },
                {  -0.1f,   0.2f,  0.0f,  1  }
            };
            shape::color wall1Color = { 1.0f, 1.0f, 1.0f };
            shape::Polygon* wall1Shape = new shape::Polygon(wall1Position, wall1Vertices, wall1Color);

            glm::vec3 wall2Position = { 0.1f, -0.6f, 0.0f };
            std::vector<glm::vec4> wall2Vertices =
            {
                {  -0.1f,  -0.2f,  0.0f,  1  },
                {   0.1f,  -0.2f,  0.0f,  1  },
                {   0.1f,   0.2f,  0.0f,  1  },
                {  -0.1f,   0.2f,  0.0f,  1  }
            };
            shape::color wall2Color = { 1.0f, 1.0f, 1.0f };
            shape::Polygon* wall2Shape = new shape::Polygon(wall2Position, wall2Vertices, wall2Color);

            glm::vec3 lava1Position = { 0.4f, -0.7f, 0.0f };
            std::vector<glm::vec4> lava1Vertices =
            {
                {  -0.2f,  -0.1f,  0.0f,  1  },
                {   0.2f,  -0.1f,  0.0f,  1  },
                {   0.2f,   0.1f,  0.0f,  1  },
                {  -0.2f,   0.1f,  0.0f,  1  }
            };
            shape::color lava1Color = { 1.0f, 0.2f, 0.2f };
            shape::Polygon* lava1Shape = new shape::Polygon(lava1Position, lava1Vertices, lava1Color);
            lava1Shape->isDeadly(true);

            glm::vec3 plane1Position = { -0.5f, 0.5f, 0.0f };
            std::vector<glm::vec4> plane1Vertices =
            {
                {  -0.3f,  -0.05f,  0.0f,  1  },
                {   0.3f,  -0.05f,  0.0f,  1  },
                {   0.3f,   0.05f,  0.0f,  1  },
                {  -0.3f,   0.05f,  0.0f,  1  }
            };
            shape::color plane1Color = { 1.0f, 1.0f, 1.0f };
            shape::Polygon* plane1Shape = new shape::Polygon(plane1Position, plane1Vertices, plane1Color);

            glm::vec3 plane2Position = { 0.2f, 0.2f, 0.0f };
            std::vector<glm::vec4> plane2Vertices =
            {
                {  -0.2f,  -0.05f,  0.0f,  1  },
                {   0.2f,  -0.05f,  0.0f,  1  },
                {   0.2f,   0.05f,  0.0f,  1  },
                {  -0.2f,   0.05f,  0.0f,  1  }
            };
            shape::color plane2Color = { 1.0f, 1.0f, 1.0f };
            shape::Polygon* plane2Shape = new shape::Polygon(plane2Position, plane2Vertices, plane2Color);

            glm::vec3 doorEntryPosition = { -0.8f, -0.7f, 0.0f };
            std::vector<glm::vec4> doorEntryVertices =
            {
                {  -0.06f,  -0.10f,  0.0f,  1  },
                {   0.06f,  -0.10f,  0.0f,  1  },
                {   0.06f,   0.10f,  0.0f,  1  },
                {  -0.06f,   0.10f,  0.0f,  1  }
            };
            shape::color doorEntryColor = { 1.0f, 1.0f, 0.2f };
            shape::Polygon* doorEntryShape = new shape::Polygon(doorEntryPosition, doorEntryVertices, doorEntryColor);
            doorEntryShape->canCollide(false);

            glm::vec3 doorExitPosition = { -0.65f, 0.65f, 0.0f };
            std::vector<glm::vec4> doorExitVertices =
            {
                {  -0.06f,  -0.10f,  0.0f,  1  },
                {   0.06f,  -0.10f,  0.0f,  1  },
                {   0.06f,   0.10f,  0.0f,  1  },
                {  -0.06f,   0.10f,  0.0f,  1  }
            };
            shape::color doorExitColor = { 0.2f, 1.0f, 0.2f };
            shape::Polygon* doorExitShape = new shape::Polygon(doorExitPosition, doorExitVertices, doorExitColor);
            doorExitShape->canCollide(false);

            glm::vec3 playerPosition = { -0.8f, -0.75f, 0.0f };
            std::vector<glm::vec4> playerVertices =
            {
                {  -0.05f,  -0.05f,  0.0f,  1  },
                {   0.05f,  -0.05f,  0.0f,  1  },
                {   0.05f,   0.05f,  0.0f,  1  },
                {  -0.05f,   0.05f,  0.0f,  1  }
            };
            shape::color playerColor = { 0.0f, 1.0f, 1.0f };
            shape::Polygon* playerShape = new shape::Polygon(playerPosition, playerVertices, playerColor);
            playerShape->canCollide(false);

            // Layers
            shapes->push_back(groundShape);
            shapes->push_back(lava1Shape);
            shapes->push_back(wall1Shape);
            shapes->push_back(wall2Shape);
            shapes->push_back(plane1Shape);
            shapes->push_back(plane2Shape);
            shapes->push_back(doorEntryShape);
            shapes->push_back(doorExitShape);
            shapes->push_back(playerShape);

            game::player = new player::Player(playerShape);
            game::player->setMass(0.6f);

            game::gravity = { 0.0f, -10.0f, 0.0f };
            game::air = 5.0f;

            game::shapes = shapes;
            game::entry = doorEntryShape;
            game::exit = doorExitShape;
        },
    };
}