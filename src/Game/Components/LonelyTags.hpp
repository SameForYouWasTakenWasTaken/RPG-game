#pragma once
/*
    Lonely tags -> Tags of general entities that don't serve any other person than to be a tag. 
    Non-lonely tags -> Tags of components that are dependent for other components, usually written directly next to the
    ... actual component as a struct.
    
    Lonely tags are used when they just serve for systems that may want a more precise iteration

    Non-lonely tag examples are Dead. Dead is a state for the Humanoid, so it's dependent of the humanoid component 
    Lonely tag examples are Spatial. Spatial makes iteration more precise for the spatial grid.
*/
namespace Game::Components::LonelyTags
{
    struct Spatial {}; // For spatial grids
}