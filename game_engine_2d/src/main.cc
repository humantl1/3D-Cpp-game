#include <iostream>
#include <memory>

#include "constants.h"
#include "game.h"

int main() {
    std::unique_ptr<Game> game {new Game()};
  
  game->Initialize(kWindowWidth, kWindowHeight);

  while (game->IsRunning()) {
    game->ProcessInput();
    game->Update();
    game->Render();
  }

  game->Destroy();

  std::cout << "Game is running..." <<std::endl;
}
