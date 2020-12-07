#pragma once
/*
* This namespace contains constants used throughout the program.
* Also provides a central place to tweak physics and other gameplay
* mechanics to make things feel 'right'.
*/
namespace CONSTANTS {

  // SDL Setup Related
  const int SCREEN_WIDTH = 1200;
  const int SCREEN_HEIGHT = 800;
  const char* const GAME_NAME = "Cannon Commando";

  // FPS Related
  const int FPS = 60;
  const int FRAME_DURATION = 1000 / FPS;

  // Gameplay Related
  const int DEF_PLAYER_HEALTH = 100;
  const int DEF_PLAYER_AMMO = 50;

  const double DEF_CANNON_ANGLE = 45.0;

  const double DEF_GRAVITY = 0.05;

  const double PLAYER_BLOCK_COL_MODIFIER = 0.5;
  const double PROJ_MOMENTUM_MODIFIER = 0.75;

  // Engine Physics Related
  const double DT = 1000 / 60;
  const double PX_PER_M = 0.05; // 'Scaling' factor for simulation

  // Player State Animations //

  /* Idle State*/
  const int PLAYER_IDLE_F_DUR = 80;
  const int PLAYER_IDLE_F_CNT = 12;
  /* Hit State*/
  const int PLAYER_HIT_F_DUR = 50;
  const int PLAYER_HIT_F_CNT = 10;
  const double PLAYER_HIT_DUR = 500.0;
  /* Shooting Cannon State*/
  const int PLAYER_FIRE_F_DUR = 50;
  const int PLAYER_FIRE_F_CNT = 12;
  const double PLAYER_FIRE_DUR = 600.0;

  // Projectile State Animations //

  /* Idle State*/
  const int PROJ_IDLE_F_DUR = 50;
  const int PROJ_IDLE_F_CNT = 5;
  /* Explode State*/
  const int PROJ_EXPLD_F_DUR = 50;
  const int PROJ_EXPLD_F_CNT = 6;
  const double PROJ_EXPLD_DUR = 300.0;

  // Cannon State Animations //

  /* Loading State*/
  const int CNN_LOAD_F_DUR = 50;
  const int CNN_LOAD_F_CNT = 4;
  const double CNN_LOAD_DUR = 200.0;
  /* Firing State*/
  const int CNN_FIRE_F_DUR = 50;
  const int CNN_FIRE_F_CNT = 7;
  const double CNN_FIRE_DUR = 350.0;

  // Block Explosion //
  const double BLOCK_EXPD_DUR = 350.0;


};
