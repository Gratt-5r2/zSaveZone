// This file added in headers queue
// File: "Sources.h"

namespace GOTHIC_ENGINE {

  // TO DO
  // Your code ...

  void Game_Entry() {
  }

  void Game_Init() {
    zcon->Register( "SAVEZONE BEGIN",      "Begin of creation new zone" );
    zcon->Register( "SAVEZONE END",        "Eng of creation new zone" );
    zcon->Register( "SAVEZONE CANCEL",     "Cancel of creation new zone" );
    zcon->Register( "SAVEZONE ADD",        "Add new point to new zone" );
    zcon->Register( "SAVEZONE REMOVE",     "Remove last point from new zone" );
    zcon->Register( "SAVEZONE SAVE",       "Save all zones to file" );
    zcon->Register( "SAVEZONE LOAD",       "Load zones from file" );
    zcon->Register( "SAVEZONE SHOW",       "Show zones bounds" );
    zcon->Register( "SAVEZONE HIDE",       "Hide zones bounds" );
    zcon->Register( "SAVEZONE DELETE",     "Delete current zone" );
    zcon->Register( "SAVEZONE DELETE ALL", "Delete all zones" );
    zcon->Register( "SAVEZONE FUNC",       "Script condition-func" );
    zcon->Register( "SAVEZONE LIM",        "Set vertical limit in bottom and (or) top points" );
    zcon->Register( "SAVEZONE UNLIM",      "Set unlimited vertical points" );
    zcon->AddEvalFunc( SaveZoneEval );
  }

  void Game_Exit() {
    SaveZoneEditor.Cancel();
    WorldSaveZone.DeleteZoneList();
  }

  void Game_Loop() {
    WorldSaveZone.UpdateSaveStatus();
    WorldSaveZone.DrawZones();
    SaveZoneEditor.DrawPoints();
  }

  // Information about current saving or loading world
  TSaveLoadGameInfo& SaveLoadGameInfo = UnionCore::SaveLoadGameInfo;

  void Game_SaveBegin() {
  }

  void Game_SaveEnd() {
  }

  void LoadBegin() {
  }

  void LoadEnd() {
    SaveZoneEditor.Cancel();
    WorldSaveZone.LoadList();
  }

  void Game_LoadBegin_NewGame() {
    LoadBegin();
  }

  void Game_LoadEnd_NewGame() {
    LoadEnd();
  }

  void Game_LoadBegin_SaveGame() {
    LoadBegin();
  }

  void Game_LoadEnd_SaveGame() {
    LoadEnd();
  }

  void Game_LoadBegin_ChangeLevel() {
    LoadBegin();
  }

  void Game_LoadEnd_ChangeLevel() {
    LoadEnd();
  }

  void Game_LoadBegin_Trigger() {
  }
  
  void Game_LoadEnd_Trigger() {
  }
  
  void Game_Pause() {
  }
  
  void Game_Unpause() {
  }
  
  void Game_DefineExternals() {
  }

  /*
  Functions call order on Game initialization:
    - Game_Entry           * Gothic entry point
    - Game_DefineExternals * Define external script functions
    - Game_Init            * After DAT files init
  
  Functions call order on Change level:
    - Game_LoadBegin_Trigger     * Entry in trigger
    - Game_LoadEnd_Trigger       *
    - Game_Loop                  * Frame call window
    - Game_LoadBegin_ChangeLevel * Load begin
    - Game_SaveBegin             * Save previous level information
    - Game_SaveEnd               *
    - Game_LoadEnd_ChangeLevel   *
  
  Functions call order on Save game:
    - Game_Pause     * Open menu
    - Game_Unpause   * Click on save
    - Game_Loop      * Frame call window
    - Game_SaveBegin * Save begin
    - Game_SaveEnd   *
  
  Functions call order on Load game:
    - Game_Pause              * Open menu
    - Game_Unpause            * Click on load
    - Game_LoadBegin_SaveGame * Load begin
    - Game_LoadEnd_SaveGame   *
  */

#define AppDefault True
  CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
    Enabled( AppDefault ) Game_Entry,
    Enabled( AppDefault ) Game_Init,
    Enabled( AppDefault ) Game_Exit,
    Enabled( AppDefault ) Game_Loop,
    Enabled( AppDefault ) Game_SaveBegin,
    Enabled( AppDefault ) Game_SaveEnd,
    Enabled( AppDefault ) Game_LoadBegin_NewGame,
    Enabled( AppDefault ) Game_LoadEnd_NewGame,
    Enabled( AppDefault ) Game_LoadBegin_SaveGame,
    Enabled( AppDefault ) Game_LoadEnd_SaveGame,
    Enabled( AppDefault ) Game_LoadBegin_ChangeLevel,
    Enabled( AppDefault ) Game_LoadEnd_ChangeLevel,
    Enabled( AppDefault ) Game_LoadBegin_Trigger,
    Enabled( AppDefault ) Game_LoadEnd_Trigger,
    Enabled( AppDefault ) Game_Pause,
    Enabled( AppDefault ) Game_Unpause,
    Enabled( AppDefault ) Game_DefineExternals
  );
}