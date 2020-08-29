#include "material.h"

namespace materials
{
    Material none = {
        { 0.2f, 0.2f, 0.2f },
        { 1.0f, 1.0f, 1.0f },
        { 0.5f, 0.5f, 0.5f },
        32.0f
    };

    Material copper = {
        { 0.19125f, 0.0735f, 0.0225f },
        { 0.7038f, 0.27048f, 0.0828f },
        { 0.256777f, 0.137622f, 0.086014f },
        0.1f
    };

    Material white_plastic = {
        { 0.0f, 0.0f, 0.0f },
        { 0.55f, 0.55f, 0.55f },
        { 0.7f, 0.7f, 0.7f },
        0.25f
    };

    Material brass;
    Material pearl;
}