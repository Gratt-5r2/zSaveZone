// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

  zTIcon::zTIcon() {
    Icon = Null;
  }



  void zTIcon::Init() {
    if( !Icon ) {
      Icon = new zCView();
      Icon->InsertBack( "SaveZone" );
      Icon->SetAlphaBlendFunc( zRND_ALPHA_FUNC_ADD );
    }
  }



  bool zTIcon::FadeOut( zVEC3 position ) {
    zCVob* cameraVob = ogame->GetCameraVob();
    float distance   = position.Distance( cameraVob->GetPositionWorld() );
    float fadeBegin  = 5000.0f;
    float fadeEnd    = 3000.0f;
    float fadeLength = fadeBegin - fadeEnd;

    if( distance >= fadeBegin ) {
      screen->RemoveItem( Icon );
      return true;
    }

    float fade = 255.0f;
    if( distance > fadeEnd )
      fade = 255.0f / fadeLength * (fadeBegin - distance);

    Icon->SetTransparency( (int)fade );
    return false;
  }



  bool zTIcon::OutOfBounds( zVEC3 position ) {
    zCVob* cameraVob = ogame->GetCameraVob();
    zVEC3 normal     = (position - cameraVob->GetPositionWorld()).Normalize();
    zVEC3 lookAt     = cameraVob->GetAtVectorWorld();
    float angle      = Alg_AngleUnitRad( normal, lookAt );

    if( angle >= RAD90 ) {
      screen->RemoveItem( Icon );
      return true;
    }

    return false;
  }



  void zTIcon::Update( zVEC3 position, bool show ) {
    Init();
    if( !show )
      return screen->RemoveItem( Icon );

    if( OutOfBounds( position ) || FadeOut( position ) )
      return;

    int x, y;
    GetProjection( x, y, position );

    screen->InsertItem( Icon );
    Icon->SetSize( zPixelX( 80 ), zPixelY( 80 ) );
    Icon->SetPos( x, y );
  }



  zTIcon::~zTIcon() {
    if( Icon ) {
      if( screen )
        screen->RemoveItem( Icon );

      delete Icon;
    }
  }






  void zTSaveZone::DrawZone() {
    // Dont draw 1D zone
    if( Points.GetNum() <= 1 )
      return;

    zCOLOR color = WorldSaveZone.ActiveZone == this ? GFX_GREEN : GFX_AQUA;

    uint maximum = Points.GetNum();
    for( uint i = 0; i < Points.GetNum(); i++ ) {
      uint index1 = (i + 0) % maximum;
      uint index2 = (i + 1) % maximum;

      zVEC3 pos1 = zVEC3( Points[index1][VX], Height, Points[index1][VY] );
      zVEC3 pos2 = zVEC3( Points[index2][VX], Height, Points[index2][VY] );
      zlineCache->Line3D( pos1, pos2, color, False );
    }

    if( Height != Bottom ) {
      zVEC3 pos1 = zVEC3( Center[VX], Height, Center[VY] );
      zVEC3 pos2 = zVEC3( Center[VX], Bottom, Center[VY] );
      zlineCache->Line3D( pos1, pos2, color, False );
    }

    if( Height != Top ) {
      zVEC3 pos1 = zVEC3( Center[VX], Height, Center[VY] );
      zVEC3 pos2 = zVEC3( Center[VX], Top,    Center[VY] );
      zlineCache->Line3D( pos1, pos2, color, False );
    }
  }



  inline zVEC2 MakeOrthoNormal( zVEC3 vec ) {
    return zVEC2( vec[VX], vec[VZ] );
  }



  inline zVEC3 MakeProjectile( zVEC2 vec, float height = 0 ) {
    return zVEC3( vec[VX], height, vec[VY] );
  }



  bool zTSaveZone::CheckIsInUpLimit( zVEC3 position ) {
    float ele = position[VY];

    if( ele > Height ) return Height == Top    || ele <= Top;
    else               return Height == Bottom || ele >= Bottom;
  }



  bool zTSaveZone::CheckIsInBounds( zVEC3 position ) {
    zVEC2 ortho = MakeOrthoNormal( position );
    
    return
      ortho[VX] >= Min[VX] && ortho[VX] <= Max[VX] &&
      ortho[VY] >= Min[VY] && ortho[VY] <= Max[VY];
  }



  bool zTSaveZone::HasCollision( zVEC3 position ) {
    if( !CheckIsInUpLimit( position ) || !CheckIsInBounds( position ) )
      return false;

    uint collisions = 0;
    uint maximum    = Points.GetNum();
    zCLine2D tracer = zCLine2D(
      MakeOrthoNormal( position ),
      MakeOrthoNormal( position + zVEC3( 50000.0f, 0.0f, 0.0f ) )
      );

    for( uint i = 0; i < Points.GetNum(); i++ ) {
      uint index1 = (i + 0) % maximum;
      uint index2 = (i + 1) % maximum;

      zCLine2D edge = zCLine2D( Points[index1], Points[index2] );

      if( tracer.TraceRay( edge ) )
        collisions++;
    }

    return (collisions % 2) != 0;
  }



  bool zTSaveZone::operator == ( const zTSaveZone& other ) {
    return this == &other;
  }



  void zTSaveZone::UpdateIcon() {
    Icon.Update( MakeProjectile( Center, Height + 400.0f ), Condition );
  }



  void zTSaveZone::UpdateBounds() {
    Min = NAN;
    Max = NAN;
    for( uint i = 0; i < Points.GetNum(); i++ ) {
      zVEC2& point = Points[i];

      if( ISNAN( Min[VX] ) || point[VX] < Min[VX] ) Min[VX] = point[VX];
      if( ISNAN( Min[VY] ) || point[VY] < Min[VY] ) Min[VY] = point[VY];
      if( ISNAN( Max[VX] ) || point[VX] > Max[VX] ) Max[VX] = point[VX];
      if( ISNAN( Max[VY] ) || point[VY] > Max[VY] ) Max[VY] = point[VY];
    }

    Center = (Min + Max) / 2.0f;
  }



  void zTSaveZone::UpdateCondition() {
    if( Func.IsEmpty() ) {
      Condition = true;
      return;
    }

    int index = parser->GetIndex( Func );
    if( index == Invalid ) {
      Condition = true;
      return;
    }

    Condition = *(int*)parser->CallFunc( index ) != False;
  }




  void zTWorldSaveZone::LoadList() {
    ZoneList.Clear();

    string worldName = ogame->GetGameWorld()->GetWorldName();
    string fileName  = "SaveZones\\" + worldName;

    VirtualFile file( fileName );
    if( !file.IsExists() )
      return;

    string fileData;
    file.ReadString( fileData );
    rowString rowData = fileData;

    for( uint i = 0; i < rowData.GetNum(); i++ ) {
      string& line = rowData[i];
      if( line.Shrink().IsEmpty() )
        continue;

      zTSaveZone& zoneList  = ZoneList.Create();
      Array<string> vectors = line.Split( "##" );
      zoneList.Func         = vectors[0].Shrink().Upper();
      zoneList.Height       = vectors[1].Shrink().ToReal32();
      zoneList.Bottom       = vectors[2].Shrink().ToReal32();
      zoneList.Top          = vectors[3].Shrink().ToReal32();

      for( uint j = 4; j < vectors.GetNum(); j++ ) {
        Array<string> axis = vectors[j].Split( ";" );

        zVEC2 point;
        point[VX] = axis[VX].Shrink().ToReal32();
        point[VY] = axis[VY].Shrink().ToReal32();

        zoneList.Points += point;
      }

      zoneList.UpdateBounds();
    }
  }



  void zTWorldSaveZone::SaveList() {
    string worldName = ogame->GetGameWorld()->GetWorldName();
    string fileName  = "SaveZones\\" + worldName;
    rowString rowData;

    for( uint i = 0; i < ZoneList.GetNum(); i++ ) {
      zTSaveZone& zoneList = ZoneList[i];
      string line = string::Combine( "%s##%f##%f##%f", zoneList.Func, zoneList.Height, zoneList.Bottom, zoneList.Top );

      // Ignore 1D or 2D zones
      if( zoneList.Points.GetNum() <= 2 )
        continue;

      for( uint j = 0; j < zoneList.Points.GetNum(); j++ ) {
        string vector = string::Combine( "%f;%f", zoneList.Points[j][VX], zoneList.Points[j][VY] );
        line += "##" + vector;
      }

      rowData += line;
    }

    rowData.WriteToFile( fileName );
  }



  void zTWorldSaveZone::DeleteCurrentZone() {
    if( ActiveZone )
      ZoneList.Remove( *ActiveZone );
  }



  void zTWorldSaveZone::DeleteZoneList() {
    ZoneList.Clear();
  }



  void zTWorldSaveZone::UpdateIcon() {
    for( uint i = 0; i < ZoneList.GetNum(); i++ )
      ZoneList[i].UpdateIcon();
  }



  void zTWorldSaveZone::UpdateSaveStatus() {
    zVEC3 position = player->GetPositionWorld();
    ActiveZone     = Null;

    for( uint i = 0; i < ZoneList.GetNum(); i++ ) {
      if( ZoneList[i].HasCollision( position ) ) {
        ActiveZone = &ZoneList[i];
        break;
      }
    }

    UpdateConditions();
    bool condition = ActiveZone ? ActiveZone->Condition : false;
    CanSave = ActiveZone != Null && condition && oCZoneMusic::s_herostatus == oHERO_STATUS_STD;
    UpdateIcon();
  }



  void zTWorldSaveZone::UpdateConditions() {
    for( uint i = 0; i < ZoneList.GetNum(); i++ )
        ZoneList[i].UpdateCondition();
  }



  void zTWorldSaveZone::ShowZones() {
    Show = true;
  }



  void zTWorldSaveZone::HideZones() {
    Show = false;
  }



  void zTWorldSaveZone::DrawZones() {
    if( !Show )
      return;

    for( uint i = 0; i < ZoneList.GetNum(); i++ )
      ZoneList[i].DrawZone();
  }



  bool zTWorldSaveZone::IsCanSave() {
    return CanSave;
  }



  bool zTWorldSaveZone::SetFunc( string name ) {
    if( !ActiveZone )
      return false;

    ActiveZone->Func = name.Upper();
    return true;
  }






  HOOK Hook_CGameManager_MenuEnabled AS( &CGameManager::MenuEnabled, &CGameManager::MenuEnabled_Union );

  int CGameManager::MenuEnabled_Union( int& enabled ) {
    int Ok = THISCALL( Hook_CGameManager_MenuEnabled )(enabled);
    if( !WorldSaveZone.IsCanSave() )
      enabled = False;
    
    return Ok;
  }
}