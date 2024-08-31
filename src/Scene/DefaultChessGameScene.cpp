#include "DefaultChessGameScene.h"

void Chess_Game::DefaultChessScene::InitScene()
{
    // Setup White Pieces
    std::shared_ptr<Rook> m_white_team_rook_a = std::make_shared<Rook>(BoardPosition{ 'a', 1 });
    m_white_team_rook_a->AttachDrawable(std::make_shared<Drawable>(glm::vec2(-30, 0), glm::vec3(1, 1, 1)));  // White Rook A

    std::shared_ptr<Knight> m_white_team_knight_b = std::make_shared<Knight>(BoardPosition{ 'b', 1 });
    m_white_team_knight_b->AttachDrawable(std::make_shared<Drawable>(glm::vec2(-20, 0), glm::vec3(1, 1, 1)));  // White Knight B

    std::shared_ptr<Bishop> m_white_team_bishop_c = std::make_shared<Bishop>(BoardPosition{ 'c', 1 });
    m_white_team_bishop_c->AttachDrawable(std::make_shared<Drawable>(glm::vec2(-10, 0), glm::vec3(1, 1, 1)));  // White Bishop C

    std::shared_ptr<Queen> m_white_team_queen_d = std::make_shared<Queen>(BoardPosition{ 'd', 1 });
    m_white_team_queen_d->AttachDrawable(std::make_shared<Drawable>(glm::vec2(0, 0), glm::vec3(1, 1, 1)));  // White Queen D

    std::shared_ptr<King> m_white_team_king_e = std::make_shared<King>(BoardPosition{ 'e', 1 });
    m_white_team_king_e->AttachDrawable(std::make_shared<Drawable>(glm::vec2(10, 0), glm::vec3(1, 1, 1)));  // White King E

    std::shared_ptr<Bishop> m_white_team_bishop_f = std::make_shared<Bishop>(BoardPosition{ 'f', 1 });
    m_white_team_bishop_f->AttachDrawable(std::make_shared<Drawable>(glm::vec2(20, 0), glm::vec3(1, 1, 1)));  // White Bishop F

    std::shared_ptr<Knight> m_white_team_knight_g = std::make_shared<Knight>(BoardPosition{ 'g', 1 });
    m_white_team_knight_g->AttachDrawable(std::make_shared<Drawable>(glm::vec2(30, 0), glm::vec3(1, 1, 1)));  // White Knight G

    std::shared_ptr<Rook> m_white_team_rook_h = std::make_shared<Rook>(BoardPosition{ 'h', 1 });
    m_white_team_rook_h->AttachDrawable(std::make_shared<Drawable>(glm::vec2(40, 0), glm::vec3(1, 1, 1)));  // White Rook H

    // White Pawns
    std::vector<std::shared_ptr<ChessPiece>> white_pawns;
    for (char file = 'a'; file <= 'h'; ++file) {
        auto pawn = std::make_shared<Pawn>(BoardPosition{ file, 2 });
        pawn->AttachDrawable(std::make_shared<Drawable>(glm::vec2((file - 'a') * 10 - 30, -10), glm::vec3(1, 1, 1)));
        white_pawns.push_back(pawn);
    }

    // Assemble White Team
    std::vector<std::shared_ptr<ChessPiece>> white_team_vector = {
        m_white_team_rook_a,
        m_white_team_knight_b,
        m_white_team_bishop_c,
        m_white_team_queen_d,
        m_white_team_king_e,
        m_white_team_bishop_f,
        m_white_team_knight_g,
        m_white_team_rook_h
    };
    white_team_vector.insert(white_team_vector.end(), white_pawns.begin(), white_pawns.end());

    m_WhitePlayer = std::make_shared<ChessPlayer>(white_team_vector);

    // Setup Black Pieces
    std::shared_ptr<Rook> m_black_team_rook_a = std::make_shared<Rook>(BoardPosition{ 'a', 8 });
    m_black_team_rook_a->AttachDrawable(std::make_shared<Drawable>(glm::vec2(-30, 70), glm::vec3(0, 0, 0)));  // Black Rook A

    std::shared_ptr<Knight> m_black_team_knight_b = std::make_shared<Knight>(BoardPosition{ 'b', 8 });
    m_black_team_knight_b->AttachDrawable(std::make_shared<Drawable>(glm::vec2(-20, 70), glm::vec3(0, 0, 0)));  // Black Knight B

    std::shared_ptr<Bishop> m_black_team_bishop_c = std::make_shared<Bishop>(BoardPosition{ 'c', 8 });
    m_black_team_bishop_c->AttachDrawable(std::make_shared<Drawable>(glm::vec2(-10, 70), glm::vec3(0, 0, 0)));  // Black Bishop C

    std::shared_ptr<Queen> m_black_team_queen_d = std::make_shared<Queen>(BoardPosition{ 'd', 8 });
    m_black_team_queen_d->AttachDrawable(std::make_shared<Drawable>(glm::vec2(0, 70), glm::vec3(0, 0, 0)));  // Black Queen D

    std::shared_ptr<King> m_black_team_king_e = std::make_shared<King>(BoardPosition{ 'e', 8 });
    m_black_team_king_e->AttachDrawable(std::make_shared<Drawable>(glm::vec2(10, 70), glm::vec3(0, 0, 0)));  // Black King E

    std::shared_ptr<Bishop> m_black_team_bishop_f = std::make_shared<Bishop>(BoardPosition{ 'f', 8 });
    m_black_team_bishop_f->AttachDrawable(std::make_shared<Drawable>(glm::vec2(20, 70), glm::vec3(0, 0, 0)));  // Black Bishop F

    std::shared_ptr<Knight> m_black_team_knight_g = std::make_shared<Knight>(BoardPosition{ 'g', 8 });
    m_black_team_knight_g->AttachDrawable(std::make_shared<Drawable>(glm::vec2(30, 70), glm::vec3(0, 0, 0)));  // Black Knight G

    std::shared_ptr<Rook> m_black_team_rook_h = std::make_shared<Rook>(BoardPosition{ 'h', 8 });
    m_black_team_rook_h->AttachDrawable(std::make_shared<Drawable>(glm::vec2(40, 70), glm::vec3(0, 0, 0)));  // Black Rook H

    // Black Pawns
    std::vector<std::shared_ptr<ChessPiece>> black_pawns;
    for (char file = 'a'; file <= 'h'; ++file) {
        auto pawn = std::make_shared<Pawn>(BoardPosition{ file, 7 });
        pawn->AttachDrawable(std::make_shared<Drawable>(glm::vec2((file - 'a') * 10 - 30, 60), glm::vec3(0, 0, 0)));
        black_pawns.push_back(pawn);
    }

    // Assemble Black Team
    std::vector<std::shared_ptr<ChessPiece>> black_team_vector = {
        m_black_team_rook_a,
        m_black_team_knight_b,
        m_black_team_bishop_c,
        m_black_team_queen_d,
        m_black_team_king_e,
        m_black_team_bishop_f,
        m_black_team_knight_g,
        m_black_team_rook_h
    };
    black_team_vector.insert(black_team_vector.end(), black_pawns.begin(), black_pawns.end());

    m_BlackPlayer = std::make_shared<ChessPlayer>(black_team_vector);


    for (const auto& piece_attachable : m_WhitePlayer->GetPlayerPieces())
    {
        m_SceneObjects.push_back(piece_attachable->GetDrawable());
    }

    for (const auto& piece_attachable : m_BlackPlayer->GetPlayerPieces())
    {
        m_SceneObjects.push_back(piece_attachable->GetDrawable());
    }

    m_ChessGame = std::make_unique<ChessGame>(m_WhitePlayer, m_BlackPlayer);
    m_ChessBoard = std::make_shared<Drawable>(glm::vec2(0), glm::vec3(0, 1, 0));
    m_SceneObjects.push_back(m_ChessBoard);
}

void Chess_Game::DefaultChessScene::DrawScene()
{
    for (const auto& drawable_weak_ptr : m_SceneObjects)
    {
        if (auto drawable = drawable_weak_ptr.lock())
        {
            m_BatchRenderer.Push(drawable->GetPosition(), drawable->GetColor());
        }
    }
    m_BatchRenderer.Flush();
}

void Chess_Game::DefaultChessScene::OnUpdate()
{



}

void Chess_Game::DefaultChessScene::DestroyScene()
{
}
