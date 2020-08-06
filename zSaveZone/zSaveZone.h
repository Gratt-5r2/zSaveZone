// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	struct zTIcon {
		zCView* Icon;

		zTIcon();
		bool FadeOut( zVEC3 position );
		bool OutOfBounds( zVEC3 position );
		void Init();
		void Update( zVEC3 position, bool show );
		~zTIcon();
	};

	struct zTSaveZone {
		float Height;
    float Bottom;
    float Top;
		Array<zVEC2> Points;
    zVEC2 Min;
    zVEC2 Max;
    zVEC2 Center;
		zTIcon Icon;
    string Func;
		bool Condition;

		void DrawZone();
    bool CheckIsInUpLimit( zVEC3 position );
    bool CheckIsInBounds( zVEC3 position );
		bool HasCollision( zVEC3 position );
		bool operator == (const zTSaveZone & other);
		void UpdateIcon();
    void UpdateBounds();
		void UpdateCondition();
	};

	struct zTWorldSaveZone {
		string WorldName;
		Array<zTSaveZone> ZoneList;
		zTSaveZone* ActiveZone;
		bool Show;
		bool CanSave;

		void LoadList();
		void SaveList();
		void DeleteCurrentZone();
		void DeleteZoneList();
		void UpdateIcon();
		void UpdateSaveStatus();
		void UpdateConditions();
		void ShowZones();
		void HideZones();
		void DrawZones();
		bool IsCanSave();
    bool SetFunc( string name );
	};

	zTWorldSaveZone WorldSaveZone;
}