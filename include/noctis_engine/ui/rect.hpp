#pragma once


namespace NoctisEngine
{
    
/// @brief A rect in screen space coordinates
struct Rect
{
    /// @brief The width, in the range [-1, 1], where -1 is the leftmost position on 
    /// the screen, and 1 the rightmost position on the screen
    float width;

    /// @brief The height, in the range [-1, 1], where -1 is the most down position on 
    /// the screen, and 1 the upmost position on the screen
    float height;
};

} // namespace NoctisEngine
