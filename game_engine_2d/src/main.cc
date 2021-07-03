#include "./Constants.h"
#include "./Game.h"

int main() {
  Game* game = new Game();

  game->Initialize(kWindowWidth, kWindowHeight)

  while (game->IsRunning()) {
    game->ProcessInput();
    game->Update();
    game->Render();
  }

  game->Destroy();

  std::cout << "Game is running..." <<std::endl;
}
