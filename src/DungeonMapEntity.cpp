#include <iostream>
#include <sstream>
#include "WitchBlastGame.h"
#include "DungeonMapEntity.h"
#include "Constants.h"
#include "sfml_game/ImageManager.h"

DungeonMapEntity::DungeonMapEntity() : GameEntity (0.0f, 0.0f)
{
  this->z = -1.0f;

  type = 0;
  hasChanged = true;

  DungeonMapEntityPost* post = new DungeonMapEntityPost(this);
  post->setZ(-0.2f);
  post->setType(0);

  DungeonMapEntityOverlay* over = new DungeonMapEntityOverlay(this);
  over->setZ(1000);
  over->setType(0);

  overlaySprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_OVERLAY));
  roomType = roomTypeStarting;
  keyRoomEffect.delay = -1.0f;
  randomSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_RANDOM_DUNGEON));
  doorSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_DOORS));
  doorSprite.setOrigin(96, 32);
  keyStoneSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_DOORS));
  keyStoneSprite.setOrigin(96, 32);
  doorWallSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_TILES));
  doorWallSprite.setOrigin(96, 32);

  shadowType = ShadowTypeStandard;
}

void DungeonMapEntity::animate(float delay)
{
  age += delay;
  bool needCompute = getChanged() || game().getCurrentMap()->getChanged();
  if (needCompute)
  {
    computeVertices();
    computeOverVertices();
    computeShadowVertices();
  }

  // blood
  bool moving = false;
  for (unsigned int i = 0; i < blood.size(); i++)
  {
    if (blood[i].moving)
    {
      moving = true;
      if (checkFalling(blood[i], 16, 16))
      {
        SpriteEntity* spriteEntity
          = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_BLOOD),
                             blood[i].x + 8,
                             blood[i].y + 8,
                             16, 16, 6);
        spriteEntity->setAge(0.0f);
        spriteEntity->setLifetime(3.0f);
        spriteEntity->setShrinking(true);
        spriteEntity->setFading(true);
        spriteEntity->setFrame(blood[i].frame);
        spriteEntity->setScale(blood[i].scale, blood[i].scale);

        blood.erase(blood.begin() + i);
      }
      else
      {
        animateParticle(blood[i], delay, 0.95f);

        if ((game().getParameters().bloodSpread && blood[i].frame < 12) || blood[i].frame >= 36)
        {
          if (blood[i].velocity.x * blood[i].velocity.x + blood[i].velocity.y * blood[i].velocity.y > 80
              && rand() % 4 == 0)
          {
            addBlood(blood[i].x, blood[i].y, blood[i].frame, blood[i].scale);
            blood[i].scale *= 0.85f;

            bool xPos = blood[i].velocity.x > 0;
            bool yPos = blood[i].velocity.y > 0;

            float norm = sqrtf(blood[i].velocity.x * blood[i].velocity.x + blood[i].velocity.y * blood[i].velocity.y);
            blood[i].velocity = Vector2D(norm);
            if (xPos && blood[i].velocity.x < 0)  blood[i].velocity.x = - blood[i].velocity.x;
            else if (!xPos && blood[i].velocity.x > 0)  blood[i].velocity.x = - blood[i].velocity.x;
            if (yPos && blood[i].velocity.y < 0)  blood[i].velocity.y = - blood[i].velocity.y;
            else if (!yPos && blood[i].velocity.y > 0)  blood[i].velocity.y = - blood[i].velocity.y;
          }
        }
      }
    }
  }
  if (moving) computeBloodVertices();

  // corpses
  int CorpsesBox = 38, CorpsesLargeBox = 76;
  moving = false;
  for (unsigned int i = 0; i < corpses.size(); i++)
  {
    if (corpses[i].moving)
    {
      moving = true;
      if (corpses[i].frame != FRAME_CORPSE_SLIME_VIOLET
          && collideWithWall(corpses[i], CorpsesBox, CorpsesBox))
      {
        if (checkFalling(corpses[i], 48, 48))
        {
          SpriteEntity* spriteEntity
            = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_CORPSES),
                               corpses[i].x,
                               corpses[i].y,
                               64, 64, 10);
          spriteEntity->setAge(0.0f);
          spriteEntity->setLifetime(3.0f);
          spriteEntity->setShrinking(true);
          spriteEntity->setFading(true);
          spriteEntity->setFrame(corpses[i].frame);

          corpses.erase(corpses.begin() + i);
        }
        else
        {
          if (corpses[i].velocity.x < 15.0f && corpses[i].velocity.x > -15.0f
              && corpses[i].velocity.y < 15.0f && corpses[i].velocity.y > -15.0f)
            autoSpeed(corpses[i], 200);

          animateParticle(corpses[i], delay, 1.0f);
        }
      }
      else
      {
        float oldx = corpses[i].x;
        float oldy = corpses[i].y;
        animateParticle(corpses[i], delay, 0.8f);
        if (corpses[i].frame != FRAME_CORPSE_SLIME_VIOLET
            && collideWithWall(corpses[i], CorpsesBox, CorpsesBox))
        {
          corpses[i].x = oldx;
          corpses[i].y = oldy;
          moving = false;
        }
      }
    }
  }
  for (unsigned int i = 0; i < corpsesLarge.size(); i++)
  {
    if (corpsesLarge[i].moving)
    {
      moving = true;
      if (collideWithWall(corpsesLarge[i], CorpsesLargeBox, CorpsesLargeBox))
      {
        if (corpsesLarge[i].velocity.x < 15.0f && corpsesLarge[i].velocity.x > -15.0f
            && corpsesLarge[i].velocity.y < 15.0f && corpsesLarge[i].velocity.y > -15.0f)
          autoSpeed(corpsesLarge[i], 200);

        animateParticle(corpsesLarge[i], delay, 1.0f);
      }
      else
      {
        float oldx = corpsesLarge[i].x;
        float oldy = corpsesLarge[i].y;
        animateParticle(corpsesLarge[i], delay, 0.8f);
        if (collideWithWall(corpsesLarge[i], CorpsesLargeBox, CorpsesLargeBox))
        {
          corpsesLarge[i].x = oldx;
          corpsesLarge[i].y = oldy;
          moving = false;
        }
      }
    }
  }
  for (unsigned int i = 0; i < corpsesLarge.size(); i++)
  {
    if (corpsesLarge[i].moving)
    {
      moving = true;
      animateParticle(corpsesLarge[i], delay, 0.95f);
    }
  }
  if (moving) computeCorpsesVertices();

  if (game().getCurrentMap()->getRoomType() == roomTypeKey && !game().getCurrentMap()->isCleared())
  {
    keyRoomEffect.delay -= delay;
    if (keyRoomEffect.delay <= 0.0f)
    {
      keyRoomEffect.delay = KeyRoomFXDelay;
      keyRoomEffect.amplitude = 120 + rand() % 100;
      keyRoomEffect.isBlinking = false;
    }
  }
}

void DungeonMapEntity::animateParticle(displayEntityStruct &particle, float delay, float viscosity)
{
  particle.velocity.x *= viscosity;
  particle.velocity.y *= viscosity;

  if (particle.velocity.x < -5 || particle.velocity.x > 5
      || particle.velocity.y < -5 || particle.velocity.y > 5)
  {
    particle.x += delay * particle.velocity.x;
    particle.y += delay * particle.velocity.y;
  }
  else
    particle.moving = false;
}

bool DungeonMapEntity::collideWithWall(displayEntityStruct &particle, int boxWidth, int boxHeight)
{
  float x0 = particle.x - boxWidth / 2;
  float xf = particle.x + boxWidth / 2;
  float y0 = particle.y - boxHeight / 2;
  float yf = particle.y + boxHeight / 2;

  if (particle.x < TILE_WIDTH && particle.velocity.x < -1.0f) particle.velocity.x = -particle.velocity.x;
  else if (particle.x > TILE_WIDTH * (MAP_WIDTH - 2) && particle.velocity.x > 1.0f) particle.velocity.x = -particle.velocity.x;

  if (particle.y < TILE_HEIGHT && particle.velocity.y < -1.0f) particle.velocity.y = -particle.velocity.y;
  else if (particle.y > TILE_HEIGHT * (MAP_HEIGHT - 2) && particle.velocity.y > 1.0f) particle.velocity.y = -particle.velocity.y;

  collide[NordWest] = !game().getCurrentMap()->isWalkable(x0 / TILE_WIDTH, y0 / TILE_HEIGHT);
  collide[SudWest] = !game().getCurrentMap()->isWalkable(x0 / TILE_WIDTH, yf / TILE_HEIGHT);
  collide[NordEast] = !game().getCurrentMap()->isWalkable(xf / TILE_WIDTH, y0 / TILE_HEIGHT);
  collide[SudEast] = !game().getCurrentMap()->isWalkable(xf / TILE_WIDTH, yf / TILE_HEIGHT);

  return collide[NordWest] || collide[SudWest] || collide[NordEast] || collide[SudEast];
}

bool DungeonMapEntity::checkFalling(displayEntityStruct &particle, int boxWidth, int boxHeight)
{
  int tilex = (particle.x + boxWidth / 2) / TILE_WIDTH;
  int tiley = (particle.y + boxHeight / 2) / TILE_HEIGHT;
  if (game().getCurrentMap()->getLogicalTile(tilex, tiley) != LogicalHole) return false;

  tilex = (particle.x + boxWidth / 2 - boxWidth / 3) / TILE_WIDTH;
  tiley = (particle.y + boxHeight / 2 - boxHeight / 3) / TILE_HEIGHT;
  if (game().getCurrentMap()->getLogicalTile(tilex, tiley) != LogicalHole) return false;

  tilex = (particle.x + boxWidth / 2 + boxWidth / 3) / TILE_WIDTH;
  tiley = (particle.y + boxHeight / 2 + boxHeight / 3) / TILE_HEIGHT;
  if (game().getCurrentMap()->getLogicalTile(tilex, tiley) != LogicalHole) return false;

  return true;
}

void DungeonMapEntity::autoSpeed(displayEntityStruct &particle, float speed)
{
  if (!collide[NordWest] && !collide[NordEast])
  {
    particle.velocity.x = 0;
    particle.velocity.y = -speed;
  }
  else if (!collide[SudWest] && !collide[SudEast])
  {
    particle.velocity.x = 0;
    particle.velocity.y = speed;
  }
  else if (!collide[SudWest] && !collide[NordWest])
  {
    particle.velocity.x = -speed;
    particle.velocity.y = 0;
  }
  else if (!collide[SudEast] && !collide[NordEast])
  {
    particle.velocity.x = speed;
    particle.velocity.y = 0;
  }
  else if (!collide[NordWest])
  {
    particle.velocity.x = -speed * 0.7f;
    particle.velocity.y = -speed * 0.7f;
  }
  else if (!collide[NordEast])
  {
    particle.velocity.x = speed * 0.7f;
    particle.velocity.y = -speed * 0.7f;
  }
  else if (!collide[SudWest])
  {
    particle.velocity.x = -speed * 0.7f;
    particle.velocity.y = speed * 0.7f;
  }
  else if (!collide[SudEast])
  {
    particle.velocity.x = speed * 0.7f;
    particle.velocity.y = speed * 0.7f;
  }
  else
    particle.velocity = Vector2D(speed);
}

bool DungeonMapEntity::getChanged()
{
  bool result = hasChanged;
  hasChanged = false;
  return result;
}

void DungeonMapEntity::render(sf::RenderTarget* app)
{
  app->draw(vertices, ImageManager::getInstance().getImage(IMAGE_TILES));

  // doors
  renderDoors(app);

  // random tile
  if ( game().getCurrentMap()->getRandomTileElement().type > -1) app->draw(randomSprite);

  // over tiles
  app->draw(overVertices, ImageManager::getInstance().getImage(IMAGE_DUNGEON_OBJECTS));

  if (game().getCurrentMap()->getRoomType() == roomTypeTemple)
  {
    for (int i = 1; i < MAP_WIDTH - 2 ; i++)
      for (int j = 1; j < MAP_WIDTH - 2 ; j++)
        if (game().getCurrentMap()->getTile(i, j) >= MAP_TEMPLE
            && game().getCurrentMap()->getTile(i, j) < MAP_TEMPLE + 10)
        {
          sf::Sprite tile;
          tile.setTexture(*ImageManager::getInstance().getImage(IMAGE_TILES));
          tile.setPosition(i * TILE_WIDTH, j * TILE_HEIGHT);
          tile.setTextureRect(sf::IntRect((game().getCurrentMap()->getTile(i, j) % 24) * TILE_WIDTH,
                                          (1 + game().getCurrentMap()->getTile(i, j) / 24) * TILE_HEIGHT,
                                          TILE_WIDTH, TILE_HEIGHT));
          int fade = 127 + 127 * (cosf(6.0f * game().getAbsolutTime()));
          tile.setColor(sf::Color(255, 255, 255, fade));
          app->draw(tile);
        }
  }
  else if (game().getCurrentMap()->getRoomType() == roomTypeKey && !game().getCurrentMap()->isCleared())
  {
    int fade = 0;
    if (keyRoomEffect.isBlinking)
    {
      fade = (int)(25.0f * game().getAbsolutTime()) % 2 == 0 ? 0 : 255;
    }
    else
    {
      if (keyRoomEffect.delay > KeyRoomFXDelay / 2)
        fade = keyRoomEffect.amplitude - keyRoomEffect.amplitude * (keyRoomEffect.delay - KeyRoomFXDelay / 2) / KeyRoomFXDelay * 2;
      else
        fade = keyRoomEffect.amplitude - keyRoomEffect.amplitude * (KeyRoomFXDelay / 2 - keyRoomEffect.delay) / KeyRoomFXDelay * 2;
    }

    sf::Sprite tiles;
    tiles.setTexture(*ImageManager::getInstance().getImage(IMAGE_KEY_AREA));
    tiles.setPosition((MAP_WIDTH / 2 - 1) * TILE_WIDTH, (MAP_HEIGHT / 2 - 1) * TILE_HEIGHT);
    tiles.setTextureRect(sf::IntRect(0, 0, 3 * TILE_WIDTH, 3 * TILE_HEIGHT));
    app->draw(tiles);

    tiles.setColor(sf::Color(255, 255, 255, fade));
    tiles.setTextureRect(sf::IntRect(3 * TILE_WIDTH, 0, 3 * TILE_WIDTH, 3 * TILE_HEIGHT));
    app->draw(tiles);
  }
}

void DungeonMapEntity::renderKeyStone(sf::RenderTarget* app)
{
  int x = game().getFloorX();
  int y = game().getFloorY();

  int doorType = 0;
  int doorStypeStandard = DoorStandard_0 + (game().getLevel() - 1) % 5;

  if (game().getCurrentMap()->getNeighbourUp() || game().getCurrentMap()->getRoomType() == roomTypeExit)
  {
    if (game().getCurrentMap()->getRoomType() == roomTypeBoss
        || game().getCurrentFloor()->getRoom(x, y - 1) == roomTypeBoss)
      doorType = int (DoorBoss);
    else if (game().getCurrentMap()->getRoomType() == roomTypeChallenge
             || game().getCurrentFloor()->getRoom(x, y - 1) == roomTypeChallenge)
      doorType = int (DoorChallenge);
    else
      doorType = doorStypeStandard;

    keyStoneSprite.setTextureRect(sf::IntRect(64, 128 + 192 * doorType, 192, 64));
    keyStoneSprite.setPosition(TILE_WIDTH * MAP_WIDTH / 2, TILE_HEIGHT / 2);
    keyStoneSprite.setRotation(0);
    app->draw(keyStoneSprite);
  }

  if (game().getCurrentMap()->getNeighbourDown())
  {
    if (game().getCurrentMap()->getRoomType() == roomTypeBoss
        || game().getCurrentFloor()->getRoom(x, y + 1) == roomTypeBoss)
      doorType = int (DoorBoss);
    else if (game().getCurrentMap()->getRoomType() == roomTypeChallenge
             || game().getCurrentFloor()->getRoom(x, y + 1) == roomTypeChallenge)
      doorType = int (DoorChallenge);
    else
      doorType = doorStypeStandard;

    keyStoneSprite.setTextureRect(sf::IntRect(64, 128 + 192 * doorType, 192, 64));
    keyStoneSprite.setPosition(TILE_WIDTH * MAP_WIDTH / 2, TILE_HEIGHT * MAP_HEIGHT - TILE_HEIGHT / 2);
    keyStoneSprite.setRotation(180);
    app->draw(keyStoneSprite);
  }

  if (game().getCurrentMap()->getNeighbourLeft())
  {
    if (game().getCurrentMap()->getRoomType() == roomTypeBoss
        || game().getCurrentFloor()->getRoom(x - 1, y) == roomTypeBoss)
      doorType = int (DoorBoss);
    else if (game().getCurrentMap()->getRoomType() == roomTypeChallenge
             || game().getCurrentFloor()->getRoom(x - 1, y) == roomTypeChallenge)
      doorType = int (DoorChallenge);
    else
      doorType = doorStypeStandard;

    keyStoneSprite.setTextureRect(sf::IntRect(64, 128 + 192 * doorType, 192, 64));
    keyStoneSprite.setPosition(TILE_WIDTH / 2, TILE_HEIGHT * MAP_HEIGHT / 2);
    keyStoneSprite.setRotation(270);
    app->draw(keyStoneSprite);
  }

  if (game().getCurrentMap()->getNeighbourRight())
  {
    if (game().getCurrentMap()->getRoomType() == roomTypeBoss
        || game().getCurrentFloor()->getRoom(x + 1, y) == roomTypeBoss)
      doorType = int (DoorBoss);
    else if (game().getCurrentMap()->getRoomType() == roomTypeChallenge
             || game().getCurrentFloor()->getRoom(x + 1, y) == roomTypeChallenge)
      doorType = int (DoorChallenge);
    else
      doorType = doorStypeStandard;

    keyStoneSprite.setTextureRect(sf::IntRect(64, 128 + 192 * doorType, 192, 64));
    keyStoneSprite.setPosition(TILE_WIDTH * MAP_WIDTH - TILE_WIDTH / 2, TILE_HEIGHT * MAP_HEIGHT / 2);
    keyStoneSprite.setRotation(90);
    app->draw(keyStoneSprite);
  }
}

void DungeonMapEntity::renderDoors(sf::RenderTarget* app)
{
  int x = game().getFloorX();
  int y = game().getFloorY();

  int doorType = 0;
  int doorStypeStandard = DoorStandard_0 + (game().getLevel() - 1) % 5;

  // fading from doors
  doorWallSprite.setTextureRect(sf::IntRect(DOOR_SHADOW_SPRITE_X, DOOR_SHADOW_SPRITE_Y, 192, 64));
  if (game().getCurrentMap()->hasNeighbourUp())
  {
    doorWallSprite.setPosition(TILE_WIDTH * MAP_WIDTH / 2, TILE_HEIGHT / 2);
    doorWallSprite.setRotation(0);
    app->draw(doorWallSprite);
  }

  if (game().getCurrentMap()->hasNeighbourDown())
  {
    doorWallSprite.setPosition(TILE_WIDTH * MAP_WIDTH / 2, TILE_HEIGHT * MAP_HEIGHT - TILE_HEIGHT / 2);
    doorWallSprite.setRotation(180);
    app->draw(doorWallSprite);
  }

  if (game().getCurrentMap()->hasNeighbourLeft())
  {
    doorWallSprite.setPosition(TILE_WIDTH / 2, TILE_HEIGHT * MAP_HEIGHT / 2);
    doorWallSprite.setRotation(270);
    app->draw(doorWallSprite);
  }

  if (game().getCurrentMap()->hasNeighbourRight())
  {
    doorWallSprite.setPosition(TILE_WIDTH * MAP_WIDTH - TILE_WIDTH / 2, TILE_HEIGHT * MAP_HEIGHT / 2);
    doorWallSprite.setRotation(90);
    app->draw(doorWallSprite);
  }

  game().renderDoors();
  if (game().getCurrentMap()->hasNeighbourUp()
      || game().getCurrentMap()->getRoomType() == roomTypeExit)
  {
    if (game().getCurrentMap()->getRoomType() == roomTypeBoss
        || game().getCurrentFloor()->getRoom(x, y - 1) == roomTypeBoss)
    {
      doorType = int (DoorBoss);
    }
    else if (game().getCurrentMap()->getRoomType() == roomTypeChallenge
             || game().getCurrentFloor()->getRoom(x, y - 1) == roomTypeChallenge)
    {
      doorType = DoorChallenge;
    }
    else
      doorType = doorStypeStandard;

    doorWallSprite.setTextureRect(sf::IntRect(DOOR_WALL_SPRITE_X, game().getCurrentMap()->getWallType() * 64 +  DOOR_WALL_SPRITE_Y, 192, 64));
    doorWallSprite.setPosition(TILE_WIDTH * MAP_WIDTH / 2, TILE_HEIGHT / 2);
    doorWallSprite.setRotation(0);
    app->draw(doorWallSprite);

    if (game().getCurrentMap()->getRoomType() == roomTypeExit)
    {
      doorWallSprite.setTextureRect(sf::IntRect(DOOR_STAIRS_SPRITE_X, DOOR_STAIRS_SPRITE_Y, 192, 64));
      app->draw(doorWallSprite);
    }

    doorSprite.setTextureRect(sf::IntRect(64, 64 + 192 * doorType, 192, 64));
    doorSprite.setPosition(TILE_WIDTH * MAP_WIDTH / 2, TILE_HEIGHT / 2);
    doorSprite.setRotation(0);
    app->draw(doorSprite);
  }
  if (game().getCurrentMap()->hasNeighbourDown()
      || (game().getLevel() > 1 && game().getCurrentMap()->getRoomType() == roomTypeStarting))
  {
    if (game().getCurrentMap()->getRoomType() == roomTypeBoss
        || game().getCurrentFloor()->getRoom(x, y + 1) == roomTypeBoss)
    {
      doorType = int (DoorBoss);
    }
    else if (game().getCurrentMap()->getRoomType() == roomTypeChallenge
             || game().getCurrentFloor()->getRoom(x, y + 1) == roomTypeChallenge)
    {
      doorType = DoorChallenge;
    }
    else
      doorType = doorStypeStandard;

    doorWallSprite.setPosition(TILE_WIDTH * MAP_WIDTH / 2, TILE_HEIGHT * MAP_HEIGHT - TILE_HEIGHT / 2);
    doorWallSprite.setRotation(180);
    doorWallSprite.setTextureRect(sf::IntRect(DOOR_WALL_SPRITE_X, game().getCurrentMap()->getWallType() * 64 +  DOOR_WALL_SPRITE_Y, 192, 64));
    app->draw(doorWallSprite);

    if (game().getLevel() > 1 && game().getCurrentMap()->getRoomType() == roomTypeStarting)
    {
      doorWallSprite.setTextureRect(sf::IntRect(DOOR_GRID_SPRITE_X, DOOR_GRID_SPRITE_Y, 192, 64));
      app->draw(doorWallSprite);
    }
    doorSprite.setPosition(TILE_WIDTH * MAP_WIDTH / 2, TILE_HEIGHT * MAP_HEIGHT - TILE_HEIGHT / 2);
    doorSprite.setRotation(180);
    doorSprite.setTextureRect(sf::IntRect(64, 64 + 192 * doorType, 192, 64));
    app->draw(doorSprite);
  }
  if (game().getCurrentMap()->hasNeighbourLeft())
  {
    if (game().getCurrentMap()->getRoomType() == roomTypeBoss
        || game().getCurrentFloor()->getRoom(x - 1, y) == roomTypeBoss)
    {
      doorType = int (DoorBoss);
    }
    else if (game().getCurrentMap()->getRoomType() == roomTypeChallenge
             || game().getCurrentFloor()->getRoom(x - 1, y) == roomTypeChallenge)
    {
      doorType = DoorChallenge;
    }
    else
      doorType = doorStypeStandard;

    doorWallSprite.setPosition(TILE_WIDTH / 2, TILE_HEIGHT * MAP_HEIGHT / 2);
    doorWallSprite.setRotation(270);
    doorWallSprite.setTextureRect(sf::IntRect(DOOR_WALL_SPRITE_X, game().getCurrentMap()->getWallType() * 64 +  DOOR_WALL_SPRITE_Y, 192, 64));
    app->draw(doorWallSprite);

    doorSprite.setTextureRect(sf::IntRect(64, 64 + 192 * doorType, 192, 64));
    doorSprite.setPosition(TILE_WIDTH / 2, TILE_HEIGHT * MAP_HEIGHT / 2);
    doorSprite.setRotation(270);
    app->draw(doorSprite);
  }
  if (game().getCurrentMap()->hasNeighbourRight())
  {
    if (game().getCurrentMap()->getRoomType() == roomTypeBoss
        || game().getCurrentFloor()->getRoom(x + 1, y) == roomTypeBoss)
    {
      doorType = int (DoorBoss);
    }
    else if (game().getCurrentMap()->getRoomType() == roomTypeChallenge
             || game().getCurrentFloor()->getRoom(x + 1, y) == roomTypeChallenge)
    {
      doorType = DoorChallenge;
    }
    else
      doorType = doorStypeStandard;

    doorWallSprite.setPosition(TILE_WIDTH * MAP_WIDTH - TILE_WIDTH / 2, TILE_HEIGHT * MAP_HEIGHT / 2);
    doorWallSprite.setRotation(90);
    doorWallSprite.setTextureRect(sf::IntRect(DOOR_WALL_SPRITE_X, game().getCurrentMap()->getWallType() * 64 +  DOOR_WALL_SPRITE_Y, 192, 64));
    app->draw(doorWallSprite);

    doorSprite.setTextureRect(sf::IntRect(64, 64 + 192 * doorType, 192, 64));
    doorSprite.setPosition(TILE_WIDTH * MAP_WIDTH - TILE_WIDTH / 2, TILE_HEIGHT * MAP_HEIGHT / 2);
    doorSprite.setRotation(90);
    app->draw(doorSprite);
  }
}

void DungeonMapEntity::renderPost(sf::RenderTarget* app)
{
  displayBlood(app);
  displayCorpses(app);
  switch (shadowType)
  {
  case ShadowTypeStandard:
    app->draw(shadowVertices, ImageManager::getInstance().getImage(IMAGE_TILES_SHADOW));
    break;
  case ShadowTypeCorner:
    app->draw(shadowVertices, ImageManager::getInstance().getImage(IMAGE_TILES_SHADOW_CORNER));
    break;
  case ShadowTypeSmall:
    app->draw(shadowVertices, ImageManager::getInstance().getImage(IMAGE_TILES_SHADOW_SMALL));
    break;
  case ShadowTypeMedium:
    app->draw(shadowVertices, ImageManager::getInstance().getImage(IMAGE_TILES_SHADOW_MEDIUM));
    break;
  }
}

void DungeonMapEntity::renderOverlay(sf::RenderTarget* app)
{
  renderKeyStone(app);
  app->draw(overlaySprite);
}

std::vector <displayEntityStruct> DungeonMapEntity::getBlood()
{
  return blood;
}

std::vector <displayEntityStruct> DungeonMapEntity::getCorpses()
{
  auto result = corpses;
  result.insert( result.end(), corpsesLarge.begin(), corpsesLarge.end() );
  return result;
}

void DungeonMapEntity::displayBlood(sf::RenderTarget* app)
{
  app->draw(bloodVertices, ImageManager::getInstance().getImage(IMAGE_BLOOD));
}

void DungeonMapEntity::displayCorpses(sf::RenderTarget* app)
{
  app->draw(corpsesVertices, ImageManager::getInstance().getImage(IMAGE_CORPSES));
  app->draw(corpsesLargeVertices, ImageManager::getInstance().getImage(IMAGE_CORPSES_BIG));
}

void DungeonMapEntity::refreshMap()
{
  hasChanged = true;

  blood.clear();
  corpses.clear();
  corpsesLarge.clear();

  computeBloodVertices();
  computeCorpsesVertices();
}

bool DungeonMapEntity::shouldBeTransformed(int part)
{
  part -= game().getCurrentMap()->getWallType() * 24;

  if (part >= MAP_WALL_8 && part < MAP_WALL_8 + 24) return true;

  else return false;
}

void DungeonMapEntity::computeVertices()
{
  DungeonMap* gameMap = game().getCurrentMap();
  int tilesProLine = 24;
  int tileWidth = 64;
  int tileHeight = 64;
  int tileBoxWidth = 64;
  int tileBoxHeight = 64;

  vertices.setPrimitiveType(sf::Quads);
  vertices.resize(gameMap->getWidth() * gameMap->getHeight() * 4);

  for (int i = 0; i < gameMap->getWidth(); i++)
    for (int j = 0; j < gameMap->getHeight(); j++)
    {
      int nx = gameMap->getTile(i, j) % tilesProLine;
      int ny = gameMap->getTile(i, j) / tilesProLine;

      sf::Vertex* quad = &vertices[(i + j * gameMap->getWidth()) * 4];

      if (shouldBeTransformed(gameMap->getTile(i, j)))
      {
        if (j == 0 && i <= MAP_WIDTH / 2)
        {
          quad[0].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[1].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[2].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[3].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
        else if (j == 0 && i > MAP_HEIGHT / 2)
        {
          quad[1].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[0].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[3].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[2].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
        else if (j == MAP_HEIGHT - 1 && i <= MAP_WIDTH / 2)
        {
          quad[3].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[2].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[1].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[0].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
        else if (j == MAP_HEIGHT - 1 && i > MAP_WIDTH / 2)
        {
          quad[2].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[3].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[0].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[1].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
        else if (i == 0 && j <= MAP_HEIGHT / 2)
        {
          quad[0].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[3].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[2].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[1].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
        else if (i == 0 && j > MAP_HEIGHT / 2)
        {
          quad[1].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[2].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[3].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[0].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
        else if (i == MAP_WIDTH - 1 && j <= MAP_HEIGHT / 2)
        {
          quad[3].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[0].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[1].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[2].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
        else if (i == MAP_WIDTH - 1 && j > MAP_HEIGHT / 2)
        {
          quad[2].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[1].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[0].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[3].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
        // corridors
        else if (i < MAP_WIDTH / 2 && j <= MAP_HEIGHT / 2 && gameMap->getTile(i, j - 1) != game().getCurrentMap()->getWallType() * 24 + MAP_WALL_X)
        {
          quad[0].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[3].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[2].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[1].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
        else if (i < MAP_WIDTH / 2 && j > MAP_HEIGHT / 2 && gameMap->getTile(i, j + 1) != game().getCurrentMap()->getWallType() * 24 + MAP_WALL_X)
        {
          quad[1].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[2].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[3].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[0].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
        else if (i > MAP_WIDTH / 2 && j <= MAP_HEIGHT / 2 && gameMap->getTile(i, j - 1) != game().getCurrentMap()->getWallType() * 24 + MAP_WALL_X)
        {
          quad[3].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[0].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[1].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[2].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
        else if (i > MAP_WIDTH / 2 && j > MAP_HEIGHT / 2 && gameMap->getTile(i, j + 1) != game().getCurrentMap()->getWallType() * 24 + MAP_WALL_X)
        {
          quad[2].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[1].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[0].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[3].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
        else if (j < MAP_HEIGHT / 2 && i <= MAP_WIDTH / 2)
        {
          quad[0].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[1].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[2].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[3].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
        else if (j < MAP_HEIGHT / 2 && i > MAP_HEIGHT / 2)
        {
          quad[1].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[0].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[3].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[2].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
        else if (j > MAP_HEIGHT / 2 && i <= MAP_WIDTH / 2)
        {
          quad[3].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[2].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[1].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[0].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
        else if (j > MAP_HEIGHT / 2 && i > MAP_WIDTH / 2)
        {
          quad[2].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[3].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[0].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[1].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }
      }
      else
      {
        quad[0].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
        quad[1].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
        quad[2].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        quad[3].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
      }

      quad[0].texCoords = sf::Vector2f(nx * tileBoxWidth, ny * tileBoxHeight);
      quad[1].texCoords = sf::Vector2f((nx + 1) * tileBoxWidth, ny * tileBoxHeight);
      quad[2].texCoords = sf::Vector2f((nx + 1) * tileBoxWidth, (ny + 1) * tileBoxHeight);
      quad[3].texCoords = sf::Vector2f(nx * tileBoxWidth, (ny + 1) * tileBoxHeight);
    }

  //if (roomType != game().getCurrentMap()->getRoomType())
  {
    std::stringstream ss;
    roomType = game().getCurrentMap()->getRoomType();
    switch (roomType)
    {
    case roomTypeChallenge:
      ImageManager::getInstance().getImage(IMAGE_OVERLAY)->loadFromFile("media/overlay_boss_01.png");
      break;
    case roomTypeTemple:
      ImageManager::getInstance().getImage(IMAGE_OVERLAY)->loadFromFile("media/overlay_temple.png");
      break;
    case roomTypeMerchant:
      ImageManager::getInstance().getImage(IMAGE_OVERLAY)->loadFromFile("media/overlay_shop.png");
      break;
    case roomTypeBoss:
      ss << "media/overlay_boss_0" << game().getLevel() << ".png";
      ImageManager::getInstance().getImage(IMAGE_OVERLAY)->loadFromFile(ss.str());
      break;
    default:
      if ( gameMap->getObjectTile(6, 2) == MAPOBJ_BANK_TOP
           || gameMap->getObjectTile(6, 2) == MAPOBJ_BANK
           || gameMap->getObjectTile(6, 2) == MAPOBJ_BANK_BOTTOM
           || gameMap->getObjectTile(8, 2) == MAPOBJ_BANK_TOP
           || gameMap->getObjectTile(8, 2) == MAPOBJ_BANK
           || gameMap->getObjectTile(8, 2) == MAPOBJ_BANK_BOTTOM
         )
        ImageManager::getInstance().getImage(IMAGE_OVERLAY)->loadFromFile("media/overlay_temple.png");
      else
        ImageManager::getInstance().getImage(IMAGE_OVERLAY)->loadFromFile("media/overlay_00.png");
      break;
    }
    overlaySprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_OVERLAY));
  }

  int n = game().getCurrentMap()->getRandomTileElement().type;
  if ( n > -1)
  {
    randomSprite.setPosition(game().getCurrentMap()->getRandomTileElement().x + randomDungeonTiles[n].width / 2,
                             game().getCurrentMap()->getRandomTileElement().y + randomDungeonTiles[n].height / 2);
    randomSprite.setOrigin(randomDungeonTiles[n].width / 2, randomDungeonTiles[n].height / 2);
    randomSprite.setTextureRect(sf::IntRect(randomDungeonTiles[n].xOffset, randomDungeonTiles[n].yOffset, randomDungeonTiles[n].width, randomDungeonTiles[n].height));
    randomSprite.setRotation(game().getCurrentMap()->getRandomTileElement().rotation);
  }
}

void DungeonMapEntity::computeOverVertices()
{
  DungeonMap* gameMap = game().getCurrentMap();
  int tilesProLine = 10;
  int tileWidth = 64;
  int tileHeight = 64;
  int tileBoxWidth = 64;
  int tileBoxHeight = 64;

  overVertices.setPrimitiveType(sf::Quads);
  overVertices.resize(gameMap->getWidth() * gameMap->getHeight() * 4);

  for (int i = 0; i < gameMap->getWidth(); i++)
    for (int j = 0; j < gameMap->getHeight(); j++)
    {
      //if (gameMap->getObjectTile(i, j) > 2)
      {
        int nx = gameMap->getObjectTile(i, j) % tilesProLine;
        int ny = gameMap->getObjectTile(i, j) / tilesProLine;

        sf::Vertex* quad = &overVertices[(i + j * gameMap->getWidth()) * 4];
        {
          quad[0].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
          quad[1].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
          quad[2].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
          quad[3].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
        }

        quad[0].texCoords = sf::Vector2f(nx * tileBoxWidth, ny * tileBoxHeight);
        quad[1].texCoords = sf::Vector2f((nx + 1) * tileBoxWidth, ny * tileBoxHeight);
        quad[2].texCoords = sf::Vector2f((nx + 1) * tileBoxWidth, (ny + 1) * tileBoxHeight);
        quad[3].texCoords = sf::Vector2f(nx * tileBoxWidth, (ny + 1) * tileBoxHeight);
      }
    }
}

void DungeonMapEntity::computeShadowVertices()
{
  DungeonMap* gameMap = game().getCurrentMap();
  int tileWidth = 64;
  int tileHeight = 64;
  int tileBoxWidth = 64;
  int tileBoxHeight = 64;

  int x0 = 0;
  int y0 = 0;
  int xf = MAP_WIDTH - 1;
  int yf = MAP_HEIGHT - 1;
  int xd = 4;
  int yd = 4;

  if (gameMap->getLogicalTile(1, 1) == LogicalWall
      && gameMap->getLogicalTile(2, 1) != LogicalWall
      && gameMap->getLogicalTile(1, 2) != LogicalWall)
  {
    // corner
    shadowType = ShadowTypeCorner;
  }
  else
  {
    x0 = MAP_WIDTH / 2;
    while (gameMap->getLogicalTile(x0, MAP_HEIGHT / 2 + 1) != LogicalWall) x0--;
    xf = MAP_WIDTH / 2;
    while (gameMap->getLogicalTile(xf, MAP_HEIGHT / 2 + 1) != LogicalWall) xf++;
    y0 = MAP_HEIGHT / 2;
    while (gameMap->getLogicalTile(MAP_WIDTH / 2 + 1, y0) != LogicalWall) y0--;
    yf = MAP_HEIGHT / 2;
    while (gameMap->getLogicalTile(MAP_WIDTH / 2 + 1, yf) != LogicalWall) yf++;

    if (yf - y0 <= 4)
    {
      shadowType = ShadowTypeSmall;
      xd = 3;
      yd = 2;
    }
    else if (yf - y0 <= 6 || xf - x0 <= 6)
    {
      shadowType = ShadowTypeMedium;
      xd = 3;
      yd = 3;
    }
    else
    {
      shadowType = ShadowTypeStandard;
    }
  }

  shadowVertices.setPrimitiveType(sf::Quads);
  shadowVertices.resize(8 * 4);

  int xm0 = x0 + xd;
  int xmf = xf - xd;
  int ym0 = y0 + yd;
  int ymf = yf - yd;

  // top left
  {
    sf::Vertex* quad = &shadowVertices[0 * 4];
    quad[0].position = sf::Vector2f(x0 * tileWidth, y0 * tileHeight);
    quad[1].position = sf::Vector2f((x0 + xd)* tileWidth, y0 * tileHeight);
    quad[2].position = sf::Vector2f((x0 + xd) * tileWidth, (y0 + yd) * tileHeight);
    quad[3].position = sf::Vector2f(x0 * tileWidth, (y0 + yd) * tileHeight);

    quad[0].texCoords = sf::Vector2f(0 * tileBoxWidth, 0 * tileBoxHeight);
    quad[1].texCoords = sf::Vector2f(xd * tileBoxWidth, 0 * tileBoxHeight);
    quad[2].texCoords = sf::Vector2f(xd * tileBoxWidth, yd * tileBoxHeight);
    quad[3].texCoords = sf::Vector2f(0 * tileBoxWidth, yd * tileBoxHeight);
  }
  // top
  {
    sf::Vertex* quad = &shadowVertices[1 * 4];
    quad[0].position = sf::Vector2f(xm0 * tileWidth, y0 * tileHeight);
    quad[1].position = sf::Vector2f((xmf + 1)* tileWidth, y0 * tileHeight);
    quad[2].position = sf::Vector2f((xmf + 1) * tileWidth, (y0 + yd) * tileHeight);
    quad[3].position = sf::Vector2f(xm0 * tileWidth, (y0 + yd) * tileHeight);

    quad[0].texCoords = sf::Vector2f(xd * tileBoxWidth, 0 * tileBoxHeight);
    quad[1].texCoords = sf::Vector2f((xd + 1) * tileBoxWidth, 0 * tileBoxHeight);
    quad[2].texCoords = sf::Vector2f((xd + 1) * tileBoxWidth, yd * tileBoxHeight);
    quad[3].texCoords = sf::Vector2f(xd * tileBoxWidth, yd * tileBoxHeight);
  }
  // top right
  {
    sf::Vertex* quad = &shadowVertices[2 * 4];
    quad[0].position = sf::Vector2f((xf - xd + 1) * tileWidth, y0 * tileHeight);
    quad[1].position = sf::Vector2f((xf + 1)* tileWidth, y0 * tileHeight);
    quad[2].position = sf::Vector2f((xf + 1) * tileWidth, (y0 + yd) * tileHeight);
    quad[3].position = sf::Vector2f((xf - xd + 1) * tileWidth, (y0 + yd) * tileHeight);

    quad[0].texCoords = sf::Vector2f((xd + 1) * tileBoxWidth, 0 * tileBoxHeight);
    quad[1].texCoords = sf::Vector2f((xd + xd + 1) * tileBoxWidth, 0 * tileBoxHeight);
    quad[2].texCoords = sf::Vector2f((xd + xd + 1) * tileBoxWidth, yd * tileBoxHeight);
    quad[3].texCoords = sf::Vector2f((xd + 1) * tileBoxWidth, yd * tileBoxHeight);
  }
  // left
  {
    sf::Vertex* quad = &shadowVertices[3 * 4];
    quad[0].position = sf::Vector2f(x0 * tileWidth, ym0 * tileHeight);
    quad[1].position = sf::Vector2f((x0 + xd) * tileWidth, ym0 * tileHeight);
    quad[2].position = sf::Vector2f((x0 + xd) * tileWidth, (ymf + 1) * tileHeight);
    quad[3].position = sf::Vector2f(x0 * tileWidth, (ymf + 1) * tileHeight);

    quad[0].texCoords = sf::Vector2f(0 * tileBoxWidth, yd * tileBoxHeight);
    quad[1].texCoords = sf::Vector2f(xd * tileBoxWidth, yd * tileBoxHeight);
    quad[2].texCoords = sf::Vector2f(xd * tileBoxWidth, (yd + 1) * tileBoxHeight);
    quad[3].texCoords = sf::Vector2f(0 * tileBoxWidth, (yd + 1) * tileBoxHeight);
  }
  // right
  {
    sf::Vertex* quad = &shadowVertices[4 * 4];
    quad[0].position = sf::Vector2f((xf - xd + 1) * tileWidth, ym0 * tileHeight);
    quad[1].position = sf::Vector2f((xf + 1) * tileWidth, ym0 * tileHeight);
    quad[2].position = sf::Vector2f((xf + 1) * tileWidth, (ymf + 1) * tileHeight);
    quad[3].position = sf::Vector2f((xf - xd + 1) * tileWidth, (ymf + 1) * tileHeight);

    quad[0].texCoords = sf::Vector2f((xd + 1) * tileBoxWidth, yd * tileBoxHeight);
    quad[1].texCoords = sf::Vector2f((xd + xd + 1) * tileBoxWidth, yd * tileBoxHeight);
    quad[2].texCoords = sf::Vector2f((xd + xd + 1) * tileBoxWidth, (yd + 1) * tileBoxHeight);
    quad[3].texCoords = sf::Vector2f((xd + 1) * tileBoxWidth, (yd + 1) * tileBoxHeight);
  }
  // bottom left
  {
    sf::Vertex* quad = &shadowVertices[5 * 4];
    quad[0].position = sf::Vector2f(x0 * tileWidth, (yf - yd + 1) * tileHeight);
    quad[1].position = sf::Vector2f((x0 + xd)* tileWidth, (yf - yd + 1) * tileHeight);
    quad[2].position = sf::Vector2f((x0 + xd) * tileWidth, (1 + yf) * tileHeight);
    quad[3].position = sf::Vector2f(x0 * tileWidth, (1 + yf) * tileHeight);

    quad[0].texCoords = sf::Vector2f(0 * tileBoxWidth, (yd + 1) * tileBoxHeight);
    quad[1].texCoords = sf::Vector2f(xd * tileBoxWidth, (yd + 1)  * tileBoxHeight);
    quad[2].texCoords = sf::Vector2f(xd * tileBoxWidth, (yd + yd + 1)  * tileBoxHeight);
    quad[3].texCoords = sf::Vector2f(0 * tileBoxWidth, (yd + yd + 1)  * tileBoxHeight);
  }
  // bottom
  {
    sf::Vertex* quad = &shadowVertices[6 * 4];
    quad[0].position = sf::Vector2f(xm0 * tileWidth, (yf - yd + 1) * tileHeight);
    quad[1].position = sf::Vector2f((xmf + 1)* tileWidth, (yf - yd + 1) * tileHeight);
    quad[2].position = sf::Vector2f((xmf + 1) * tileWidth, (1 + yf) * tileHeight);
    quad[3].position = sf::Vector2f(xm0 * tileWidth, (1 + yf) * tileHeight);

    quad[0].texCoords = sf::Vector2f(xd * tileBoxWidth, (yd + 1) * tileBoxHeight);
    quad[1].texCoords = sf::Vector2f((xd + 1) * tileBoxWidth, (yd + 1) * tileBoxHeight);
    quad[2].texCoords = sf::Vector2f((xd + 1) * tileBoxWidth, (yd + yd + 1) * tileBoxHeight);
    quad[3].texCoords = sf::Vector2f(xd * tileBoxWidth, (yd + yd + 1) * tileBoxHeight);
  }
  // bottom right
  {
    sf::Vertex* quad = &shadowVertices[7 * 4];
    quad[0].position = sf::Vector2f((xf - xd + 1) * tileWidth, (yf - yd + 1) * tileHeight);
    quad[1].position = sf::Vector2f((xf + 1)* tileWidth, (yf - yd + 1) * tileHeight);
    quad[2].position = sf::Vector2f((xf + 1) * tileWidth, (1 + yf) * tileHeight);
    quad[3].position = sf::Vector2f((xf - xd + 1) * tileWidth, (1 + yf) * tileHeight);

    quad[0].texCoords = sf::Vector2f((xd + 1) * tileBoxWidth, (yd + 1) * tileBoxHeight);
    quad[1].texCoords = sf::Vector2f((xd + xd + 1) * tileBoxWidth, (yd + 1) * tileBoxHeight);
    quad[2].texCoords = sf::Vector2f((xd + xd + 1) * tileBoxWidth, (yd + yd + 1) * tileBoxHeight);
    quad[3].texCoords = sf::Vector2f((xd + 1) * tileBoxWidth, (yd + yd + 1) * tileBoxHeight);
  }
}

void DungeonMapEntity::computeBloodVertices()
{
  bloodVertices.setPrimitiveType(sf::Quads);
  bloodVertices.resize(blood.size() * 4);

  for (unsigned int i = 0; i < blood.size(); i++)
  {
    auto particle = blood[i];

    sf::Vertex* quad = &bloodVertices[i * 4];

    float middle = 8.0f * particle.scale;
    int nx = particle.frame % 6;
    int ny = particle.frame / 6;

    quad[0].position = sf::Vector2f(particle.x - middle, particle.y - middle);
    quad[1].position = sf::Vector2f(particle.x + middle, particle.y - middle);
    quad[2].position = sf::Vector2f(particle.x + middle, particle.y + middle);
    quad[3].position = sf::Vector2f(particle.x - middle, particle.y + middle);

    quad[0].texCoords = sf::Vector2f(nx * 16, ny * 16);
    quad[1].texCoords = sf::Vector2f((nx + 1) * 16, ny * 16);
    quad[2].texCoords = sf::Vector2f((nx + 1) * 16, (ny + 1) * 16);
    quad[3].texCoords = sf::Vector2f(nx * 16, (ny + 1) * 16);
  }
}

void DungeonMapEntity::computeCorpsesVertices()
{
  corpsesVertices.setPrimitiveType(sf::Quads);
  corpsesVertices.resize(corpses.size() * 4);
  for (unsigned int i = 0; i < corpses.size(); i++)
  {
    auto particle = corpses[i];

    sf::Vertex* quad = &corpsesVertices[i * 4];

    float middle = 32;
    int nx = particle.frame % 10;
    int ny = particle.frame / 10;

    quad[0].position = sf::Vector2f(particle.x - middle, particle.y - middle);
    quad[1].position = sf::Vector2f(particle.x + middle, particle.y - middle);
    quad[2].position = sf::Vector2f(particle.x + middle, particle.y + middle);
    quad[3].position = sf::Vector2f(particle.x - middle, particle.y + middle);

    quad[0].texCoords = sf::Vector2f(nx * 64, ny * 64);
    quad[1].texCoords = sf::Vector2f((nx + 1) * 64, ny * 64);
    quad[2].texCoords = sf::Vector2f((nx + 1) * 64, (ny + 1) * 64);
    quad[3].texCoords = sf::Vector2f(nx * 64, (ny + 1) * 64);
  }

  corpsesLargeVertices.setPrimitiveType(sf::Quads);
  corpsesLargeVertices.resize(corpsesLarge.size() * 4);
  for (unsigned int i = 0; i < corpsesLarge.size(); i++)
  {
    auto particle = corpsesLarge[i];

    sf::Vertex* quad = &corpsesLargeVertices[i * 4];

    float middle = 64;
    int nx = (particle.frame - FRAME_CORPSE_KING_RAT) % 8;
    int ny = (particle.frame - FRAME_CORPSE_KING_RAT) / 8;

    quad[0].position = sf::Vector2f(particle.x - middle, particle.y - middle);
    quad[1].position = sf::Vector2f(particle.x + middle, particle.y - middle);
    quad[2].position = sf::Vector2f(particle.x + middle, particle.y + middle);
    quad[3].position = sf::Vector2f(particle.x - middle, particle.y + middle);

    quad[0].texCoords = sf::Vector2f(nx * 128, ny * 128);
    quad[1].texCoords = sf::Vector2f((nx + 1) * 128, ny * 128);
    quad[2].texCoords = sf::Vector2f((nx + 1) * 128, (ny + 1) * 128);
    quad[3].texCoords = sf::Vector2f(nx * 128, (ny + 1) * 128);
  }
}

displayEntityStruct& DungeonMapEntity::generateBlood(float x, float y, BaseCreatureEntity::enumBloodColor bloodColor)
{
  displayEntityStruct bloodEntity;

  int b0 = 0 + 6 * (int)bloodColor;
  bloodEntity.frame = b0 + rand()%6;
  bloodEntity.velocity = Vector2D(rand()%250);
  bloodEntity.x = x;
  bloodEntity.y = y;
  bloodEntity.scale = /*bloodColor == 0 ?*/ 1.0f + (rand() % 10) * 0.1f /*: 1.0f*/;

  bloodEntity.moving = true;

  blood.push_back(bloodEntity);

  return blood[blood.size() - 1];
}

void DungeonMapEntity::addBlood(float x, float y, int frame, float scale)
{
  displayEntityStruct bloodEntity;

  bloodEntity.frame = frame;
  bloodEntity.x = x;
  bloodEntity.y = y;
  bloodEntity.velocity.x = 0;
  bloodEntity.velocity.y = 0;
  bloodEntity.scale = scale;

  bloodEntity.moving = true;

  blood.push_back(bloodEntity);
}

void DungeonMapEntity::addCorpse(float x, float y, int frame)
{
  displayEntityStruct corpseEntity;

  corpseEntity.frame = frame;
  corpseEntity.x = x;
  corpseEntity.y = y;
  corpseEntity.velocity.x = 0;
  corpseEntity.velocity.y = 0;
  corpseEntity.scale = 0.0f;

  corpseEntity.moving = true;

  if (frame >= FRAME_CORPSE_KING_RAT)
  {
    corpsesLarge.push_back(corpseEntity);
  }
  else
  {
    corpses.push_back(corpseEntity);
  }
}

void DungeonMapEntity::activateKeyRoomEffect()
{
  keyRoomEffect.isBlinking = true;
  keyRoomEffect.delay = 1.0f;
}

/////////////////////////////////////////////////////////////////////////


DungeonMapEntityPost::DungeonMapEntityPost(DungeonMapEntity* parent) : GameEntity (0.0f, 0.0f)
{
  this->parent = parent;
}

void DungeonMapEntityPost::animate(float delay)
{
}

void DungeonMapEntityPost::render(sf::RenderTarget* app)
{
  parent->renderPost(app);
}
/////////////////////////////////////////////////////////////////////////


DungeonMapEntityOverlay::DungeonMapEntityOverlay(DungeonMapEntity* parent) : GameEntity (0.0f, 0.0f)
{
  this->parent = parent;
}

void DungeonMapEntityOverlay::animate(float delay)
{
}

void DungeonMapEntityOverlay::render(sf::RenderTarget* app)
{
  parent->renderOverlay(app);
}
