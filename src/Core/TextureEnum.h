#pragma once
namespace Chess_Game
{
    enum TextureName_
    {
        TextureName_kWhiteTexture,
        TextureName_kBoard,
        TextureName_kPawn,
        TextureName_kKing,
        TextureName_kKnight,
        TextureName_kRook,
        TextureName_kBishop,
        TextureName_kQueen,
        TextureName_kButton,
        TextureName_kPauseButton,
        TextureName_kGameLogo,
        TextureName_kUIGroupBackground
    };

    struct TextureRegion
    {
        glm::vec2 start = glm::vec2(0.0f);
        glm::vec2 end = glm::vec2(1.0f);
    };

    struct Texture
    {
        GLuint texture_handle;
        TextureRegion texture_region;
    };


    //typedef unsigned int Texture;
}