// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  zTSaveZoneEditor::zTSaveZoneEditor() {
    Bottom = NAN;
    Top    = NAN;
  }



  float zTSaveZoneEditor::GetHeight() {
    float height = 0.0f;

    for( uint i = 0; i < Points.GetNum(); i++ )
      height += Points[i][VY];

    height /= (float)Points.GetNum();
    return height;
  }



  void zTSaveZoneEditor::Begin() {
    Cancel();
  }



  void zTSaveZoneEditor::AddPoint( zVEC3 point ) {
    Points.Insert( point );
  }



  void zTSaveZoneEditor::RemovePoint() {
    if( Points.GetNum() > 0 )
      Points.RemoveAt( Points.GetNum() - 1 );
  }



  void zTSaveZoneEditor::Cancel() {
    Points.Clear();
  }



  void zTSaveZoneEditor::End() {
    if( Points.GetNum() == 0 )
      return;

    zTSaveZone& zoneList = WorldSaveZone.ZoneList.Create();
    float height = 0.0f;

    for( uint i = 0; i < Points.GetNum(); i++ )
      zoneList.Points.Insert( zVEC2( Points[i][VX], Points[i][VZ] ) );

    zoneList.Height = GetHeight();
    zoneList.Bottom = ISNAN( Bottom ) ? zoneList.Height : Bottom;
    zoneList.Top    = ISNAN( Top )    ? zoneList.Height : Top;

    zoneList.UpdateBounds();
    Cancel();
  }



  void zTSaveZoneEditor::DrawPoints() {
    // Dont draw 1D zone
    if( Points.GetNum() <= 1 )
      return;

    uint maximum = Points.GetNum();
    for( uint i = 0; i < Points.GetNum(); i++ ) {
      uint index1 = (i + 0) % maximum;
      uint index2 = (i + 1) % maximum;

      zVEC3 pos1 = Points[index1];
      zVEC3 pos2 = Points[index2];

      zlineCache->Line3D( pos1, pos2, GFX_RED, False );
    }

    if( !ISNAN( Bottom ) ) {
      zVEC3 pos1 = Points.GetFirst();
      zVEC3 pos2 = zVEC3( pos1[VX], Bottom, pos1[VY] );
      zlineCache->Line3D( pos1, pos2, GFX_RED, False );
    }

    if( !ISNAN( Top ) ) {
      zVEC3 pos1 = Points.GetFirst();
      zVEC3 pos2 = zVEC3( pos1[VX], Top, pos1[VY] );
      zlineCache->Line3D( pos1, pos2, GFX_RED, False );
    }
  }



  void zTSaveZoneEditor::SetLimited() {
    float ele    = ogame->GetCameraVob()->GetPositionWorld()[VY];
    float height = GetHeight();

    if( ele > height ) Top    = ele;
    else               Bottom = ele;
  }



  void zTSaveZoneEditor::SetUnlimited() {
    float ele    = ogame->GetCameraVob()->GetPositionWorld()[VY];
    float height = GetHeight();

    if( ele > height ) Top    = NAN;
    else               Bottom = NAN;
  }
}