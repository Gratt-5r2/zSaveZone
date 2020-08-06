// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	struct zTSaveZoneEditor {
    float Bottom;
    float Top;
		Array<zVEC3> Points;

    zTSaveZoneEditor();
    float GetHeight();
		void Begin();
		void AddPoint( zVEC3 point );
		void RemovePoint();
		void Cancel();
		void End();
		void DrawPoints();
    void SetLimited();
    void SetUnlimited();
	};

	zTSaveZoneEditor SaveZoneEditor;
}