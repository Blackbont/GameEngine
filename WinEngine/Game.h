#ifndef _GAME_H
#define _GAME_H

#include "USystem.h"

struct GameData : public SystemData
{
	
};

class Game :public USystem
{
public:
	Game(const GameData &data);
	virtual ~Game(void);
};

#endif