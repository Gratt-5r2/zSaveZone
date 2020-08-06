// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  int SaveZoneEval( const zSTRING& line_, zSTRING& message ) {
    string line = line_;
    
    string tag = line.GetWordSmart( 1 );
    if( tag == "SaveZone" ) {
      string action = line.GetWordSmart( 2 );

           if( action == "Begin"  ) SaveZoneEditor.Begin();
      else if( action == "End"    ) SaveZoneEditor.End();
      else if( action == "Cancel" ) SaveZoneEditor.Cancel();
      else if( action == "Add"    ) SaveZoneEditor.AddPoint( ogame->GetCameraVob()->GetPositionWorld() );
      else if( action == "Remove" ) SaveZoneEditor.RemovePoint();
      else if( action == "Lim"    ) SaveZoneEditor.SetLimited();
      else if( action == "UnLim"  ) SaveZoneEditor.SetUnlimited();
      else if( action == "Save"   ) WorldSaveZone.SaveList();
      else if( action == "Load"   ) WorldSaveZone.LoadList();
      else if( action == "Show"   ) WorldSaveZone.ShowZones();
      else if( action == "Hide"   ) WorldSaveZone.HideZones();
      else if( action == "Func"   ) {
        string name = line.GetWordSmart( 3 );
        if( !WorldSaveZone.SetFunc( name ) )
          message = "Ops! Please, move a player to the target zone";
      }
      else if( action == "Delete" ) {
        string count = line.GetWordSmart( 3 );

             if( count.IsEmpty() ) WorldSaveZone.DeleteCurrentZone();
        else if( count == "All" )  WorldSaveZone.DeleteZoneList();
        else message = Z "What to delete?: " + Z action;
      }
      else message = Z "What to do?: " + Z action;

      return True;
    }

    return False;
  }
}