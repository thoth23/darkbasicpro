
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// BASIC 3D COMMANDS ////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
#include "..\Include\DarkSDKBasic3D.h"
#include "..\Include\globstruct.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void		Load								( SDK_LPSTR szFilename, int iID );
void		Load								( SDK_LPSTR szFilename, int iID, int iDBProMode );
void 		Save								( SDK_LPSTR szFilename, int iID );
void 		Append								( SDK_LPSTR pString, int iID, int iFrame );
void 		Clone								( int iDestinationID, int iSourceID );
void 		Instance							( int iDestinationID, int iSourceID );
void 		DeleteEx							( int iID );

void 		Show								( int iID );
void 		Hide								( int iID );
*/

/*
void 		Position  							( int iID, float fX, float fY, float fZ );
void		dbtScaleObject 						( int iID, float fX, float fY, float fZ );
void 		dbtRotateObject						( int iID, float fX, float fY, float fZ );
void 		Move      							( int iID, float fStep );
void 		Point     							( int iID, float fX, float fY, float fZ );
void 		MoveDown  							( int iID, float fStep );
void 		MoveLeft  							( int iID, float fStep );
void 		MoveRight 							( int iID, float fStep );
void 		MoveUp    							( int iID, float fStep );
void 		XRotate   							( int iID, float fX );
void 		YRotate   							( int iID, float fY );
void 		ZRotate   							( int iID, float fZ );
void 		TurnLeft  							( int iID, float fAngle );
void 		TurnRight 							( int iID, float fAngle );
void 		PitchUp   							( int iID, float fAngle );
void 		PitchDown 							( int iID, float fAngle );
void 		RollLeft  							( int iID, float fAngle );
void 		RollRight 							( int iID, float fAngle );
*/

////

/*
void dbtObjectPosition  ( int iID, float fX, float fY, float fZ );
void dbtObjectScale     ( int iID, float fX, float fY, float fZ );
void dbtObjectRotate    ( int iID, float fX, float fY, float fZ );
void dbtObjectMove      ( int iID, float fStep );
void dbtObjectPoint     ( int iID, float fX, float fY, float fZ );
void dbtObjectMoveDown  ( int iID, float fStep );
void dbtObjectMoveLeft  ( int iID, float fStep );
void dbtObjectMoveRight ( int iID, float fStep );
void dbtObjectMoveUp    ( int iID, float fStep );
void dbtObjectXRotate   ( int iID, float fX );
void dbtObjectYRotate   ( int iID, float fY );
void dbtObjectZRotate   ( int iID, float fZ );
void dbtObjectTurnLeft  ( int iID, float fAngle );
void dbtObjectTurnRight ( int iID, float fAngle );
void dbtObjectPitchUp   ( int iID, float fAngle );
void dbtObjectPitchDown ( int iID, float fAngle );
void dbtObjectRollLeft  ( int iID, float fAngle );
void dbtObjectRollRight ( int iID, float fAngle );

/////

void 		Play								( int iID );
void 		Play 								( int iID, int iStart );
void 		Play								( int iID, int iStart, int iEnd );
void 		Loop								( int iID );
void 		Loop								( int iID, int iStart );
void 		Loop								( int iID, int iStart, int iEnd );
void 		Stop								( int iID );

void 		SetFrameEx							( int iID, float fFrame );
void 		SetSpeed							( int iID, int iSpeed );
void 		SetInterpolation					( int iID, int iJump );

void 		MakeObject							( int iID, int iMeshID, int iTextureID );
void 		MakeBox								( int iID, float fWidth, float fHeight, float fDepth );
void 		MakeCone							( int iID, float fSize );
void 		MakeCube							( int iID, float iSize );
void 		MakeCylinder						( int iID, float fSize );
void 		MakePlane							( int iID, float fWidth, float fHeight );
void 		MakeSphere							( int iID, float fRadius );
void 		MakeSphere							( int iID, float fRadius, int iRings, int iSegments );
void 		MakeTriangle						( int iID, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3 );
void 		MakeObjectFromLimb					( int iID, int iSrcObj, int iLimbID );

void 		SetTexture							( int iID, int iImage );
void 		Color								( int iID, DWORD dwRGB );
void		GhostOn								( int iID );
void 		GhostOn								( int iID, int iFlag );
void 		GhostOff							( int iID );
void 		Fade								( int iID, float iPercentage );
void 		LockOn								( int iID );
void 		LockOff								( int iID );
void 		SetTextureMode						( int iID, int iMode, int iMipGeneration );
void 		ScaleTexture						( int iID, float fU, float fV );
void 		ScrollTexture						( int iID, float fU, float fV );
void 		SetSmoothing						( int iID, float fAngle );
void 		ShowBoundsEx						( int iID, int iBoxOnly );
void 		ShowBounds							( int iID );
void 		HideBounds							( int iID );

void 		Set									( int iID, SDK_BOOL bWireframe, SDK_BOOL bTransparency, SDK_BOOL bCull );
void 		Set									( int iID, SDK_BOOL bWireframe, SDK_BOOL bTransparency, SDK_BOOL bCull, int iFilter );
void 		Set									( int iID, SDK_BOOL bWireframe, SDK_BOOL bTransparency, SDK_BOOL bCull, int iFilter, SDK_BOOL bLight );
void 		Set									( int iID, SDK_BOOL bWireframe, SDK_BOOL bTransparency, SDK_BOOL bCull, int iFilter, SDK_BOOL bLight, SDK_BOOL bFog );
void 		Set									( int iID, SDK_BOOL bWireframe, SDK_BOOL bTransparency, SDK_BOOL bCull, int iFilter, SDK_BOOL bLight, SDK_BOOL bFog, SDK_BOOL bAmbient );
void 		SetWireframe						( int iID, SDK_BOOL bFlag );
void 		SetTransparency						( int iID, SDK_BOOL bFlag );
void 		SetCull								( int iID, SDK_BOOL bCull );
void 		SetFilter							( int iID, int  iFilter );
void 		SetFilterStage						( int iID, int iStage, int iFilter );
void 		SetLight							( int iID, SDK_BOOL bLight );
void 		SetFog								( int iID, SDK_BOOL bFog  );
void 		SetAmbient							( int iID, SDK_BOOL bAmbient );
void 		SetRotationXYZ						( int iID );
void 		SetRotationZYX						( int iID );
void 		SetToCameraOrientation				( int iID );
void 		SetToObjectOrientation				( int iID, int iWhichID );
void 		DisableZDepth						( int iID );
void 		EnableZDepth						( int iID );
void 		GlueToLimb							( int iSource, int iTarget, int iLimb );
void 		UnGlue								( int iID );
void 		FixPivot							( int iID );
void 		SetDiffuseMaterial					( int iID, DWORD dwRGB );
void 		SetAmbienceMaterial					( int iID, DWORD dwRGB );
void 		SetSpecularMaterial					( int iID, DWORD dwRGB );
void 		SetEmissiveMaterial					( int iID, DWORD dwRGB );
void 		SetSpecularPower					( int iID, float fPower );

void 		SetLightMap							( int iID, int iImage );
void 		SetDetailMap						( int iID, int iImage );
void 		SetBlendMap							( int iID, int iImage, int iMode );
void 		SetSphereMap						( int iID, int iSphereImage );
void 		SetCubeMap							( int iID, int i1, int i2, int i3, int i4, int i5, int i6 );
void 		SetBumpMap							( int iID, int iBumpMap );
void 		SetCartoonShadingOn					( int iID, int iStandardImage, int iEdgeImage );
void 		SetRainbowRenderingOn				( int iID, int iStandardImage );
void 		SetShadowOn							( int iID );
void 		SetReflectionOn						( int iID );
void 		SetAlphaFactor						( int iID, float fPercentage );
void 		SetEffectOn							( int iID, SDK_LPSTR pFilename, int iUseDefaultTextures );
void 		SetShadingOff						( int iID );

void 		LoadEffect							( SDK_LPSTR pFilename, int iEffectID, int iUseDefaultTextures );
void 		DeleteEffect						( int iEffectID );
void 		SetObjectEffect						( int iID, int iEffectID );
void 		SetLimbEffect						( int iID, int iLimbID, int iEffectID );
void 		PerformChecklistForEffectValues		( int iEffectID );
void 		PerformChecklistForEffectErrors		( void );
void 		PerformChecklistForEffectErrors		( int iEffectID );
void 		SetEffectConstantB					( int iEffectID, SDK_LPSTR pConstantName, int iValue );
void 		SetEffectConstantI					( int iEffectID, SDK_LPSTR pConstantName, int iValue );
void 		SetEffectConstantF					( int iEffectID, SDK_LPSTR pConstantName, float fValue );
void 		SetEffectConstantV					( int iEffectID, SDK_LPSTR pConstantName, int iValue );
void 		SetEffectConstantM					( int iEffectID, SDK_LPSTR pConstantName, int iValue );
void 		SetEffectTechnique					( int iEffectID, SDK_LPSTR pTechniqueName );
void 		SetEffectTranspose					( int iEffectID, int iTransposeFlag );

void 		MakeCollisionBox					( int iID, float iX1, float iY1, float iZ1, float iX2, float iY2, float iZ2, int iRotatedBoxFlag );
void 		DeleteCollisionBox					( int iID );
void 		SetCollisionOn						( int iID );
void 		SetCollisionOff						( int iID );
void 		SetCollisionToBoxes					( int iID );
void 		SetCollisionToPolygons				( int iID );
void 		SetCollisionToSpheres				( int iID );
void		SetGlobalCollisionOn				( void );
void		SetGlobalCollisionOff				( void );
void		SetSphereRadius						( int iID, float fRadius );
void 		AutomaticObjectCollision			( int iObjectID, float fRadius, int iResponse );
void 		AutomaticCameraCollision			( int iCameraID, float fRadius, int iResponse );
void 		MakeNodeTreeCollisionBox			( float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2 );

void 		PerformCheckListForLimbs			( int iID );
void 		OffsetLimb							( int iID, int iLimbID, float fX, float fY, float fZ );
void 		OffsetLimb							( int iID, int iLimbID, float fX, float fY, float fZ, int iBoundFlag );
void 		ScaleLimb							( int iID, int iLimbID, float fX, float fY, float fZ );
void 		ScaleLimb							( int iID, int iLimbID, float fX, float fY, float fZ, int iBoundFlag );
void 		RotateLimb							( int iID, int iLimbID, float fX, float fY, float fZ );
void 		RotateLimb							( int iID, int iLimbID, float fX, float fY, float fZ, int iBoundFlag );
void 		ShowLimb							( int iID, int iLimbID );
void 		HideLimb							( int iID, int iLimbID );
void 		TextureLimb							( int iID, int iLimbID, int iImageID );
void 		TextureLimbStage					( int iID, int iLimbID, int iStage, int iImageID );
void 		ColorLimb							( int iID, int iLimbID, DWORD dwColor );
void 		ScaleLimbTexture					( int iID, int iLimbID, float fU, float fV );
void 		ScrollLimbTexture					( int iID, int iLimbID, float fU, float fV );
void 		AddLimb								( int iID, int iLimbID, int iMeshID );
void 		LinkLimb							( int iID, int iParentID, int iLimbID );
void 		RemoveLimb							( int iID, int iLimbID );
void 		SetLimbSmoothing					( int iID, int iLimbID, float fPercentage );
void 		SetLimbNormals						( int iID, int iLimbID );

void  		LoadMesh							( SDK_LPSTR pFilename, int iID );
void  		SaveMesh							( SDK_LPSTR pFilename, int iMeshID );
void  		DeleteMesh							( int iID );
void  		ChangeMesh							( int iObjectID, int iLimbID, int iMeshID );
void  		MakeMeshFromObject					( int iID, int iObjectID );

void 		CreateVertexShaderFromFile 			( int iID, SDK_LPSTR szFile );
void 		DeleteVertexShader					( int iShader );
void 		SetVertexShaderOn          			( int iID, int iShader );
void 		SetVertexShaderOff         			( int iID );
void 		SetVertexShaderVector      			( int iID, DWORD dwRegister, int iVector, DWORD dwConstantCount );
void 		SetVertexShaderMatrix      			( int iID, DWORD dwRegister, int iMatrix, DWORD dwConstantCount );
void 		SetVertexShaderStream      			( int iID, int iStreamPos, int iData, int iDataType );
void 		SetVertexShaderStreamCount 			( int iID, int iCount );
void 		CloneMeshToNewFormat       			( int iID, DWORD dwFVF );
void 		CreatePixelShaderFromFile			( int iID, SDK_LPSTR szFile );
void 		DeletePixelShader					( int iShader );
void 		SetPixelShaderOn					( int iID, int iShader );
void 		SetPixelShaderOff					( int iID );
void 		SetPixelShaderTexture				( int iShader, int iSlot, int iTexture );

void		FlushVideoMemory					( void );

int 		GetExist    						( int iID );
int 		GetVisible       					( int iID );
int 		GetPlaying       					( int iID );
int 		GetLooping       					( int iID );
*/
/*
DWORD 		GetXPositionEx 						( int iID );
DWORD 		GetYPositionEx 						( int iID );
DWORD 		GetZPositionEx 						( int iID );
*/

/*
SDK_FLOAT	GetSize								( int iID );
SDK_FLOAT	GetXSize							( int iID );
SDK_FLOAT	GetYSize							( int iID );
SDK_FLOAT	GetZSize							( int iID );
*/

/*
DWORD 		GetXRotationEx 						( int iID );
DWORD 		GetYRotationEx 						( int iID );
DWORD 		GetZRotationEx 						( int iID );
*/

/*
DWORD dbtObjectGetXPositionEx ( int iID );
DWORD dbtObjectGetYPositionEx ( int iID );
DWORD dbtObjectGetZPositionEx ( int iID );
DWORD dbtObjectGetXRotationEx ( int iID );
DWORD dbtObjectGetYRotationEx ( int iID );
DWORD dbtObjectGetZRotationEx ( int iID );

SDK_FLOAT	GetFrame         					( int iID );
SDK_FLOAT	GetSpeed         					( int iID );
SDK_FLOAT	GetInterpolation 					( int iID );
int 		GetNumberOfFrames 					( int iID );

int 		GetHit								( int iObjectA, int iObjectB );
int 		GetCollision 						( int iObjectA, int iObjectB );
SDK_FLOAT	GetCollisionX						( void );
SDK_FLOAT	GetCollisionY						( void );
SDK_FLOAT	GetCollisionZ						( void );
SDK_FLOAT	GetObjectCollisionRadius 			( int iID );
SDK_FLOAT	GetObjectCollisionCenterX 			( int iID );
SDK_FLOAT	GetObjectCollisionCenterY 			( int iID );
SDK_FLOAT	GetObjectCollisionCenterZ 			( int iID );
SDK_FLOAT	IntersectObject						( int iObjectID, float fX, float fY, float fZ, float fNewX, float fNewY, float fNewZ );

int 		GetLimbExist 						( int iID, int iLimbID );
SDK_LPSTR	GetLimbName							( DWORD pDestStr, int iID, int iLimbID );
int 		GetLimbVisible 						( int iID, int iLimbID );
SDK_FLOAT	GetLimbOffsetX						( int iID, int iLimbID );
SDK_FLOAT	GetLimbOffsetY    					( int iID, int iLimbID );
SDK_FLOAT	GetLimbOffsetZ    					( int iID, int iLimbID );
SDK_FLOAT	GetLimbScaleX				  		( int iID, int iLimbID );
SDK_FLOAT	GetLimbScaleY				  		( int iID, int iLimbID );
SDK_FLOAT	GetLimbScaleZ				  		( int iID, int iLimbID );
SDK_FLOAT	GetLimbAngleX     					( int iID, int iLimbID );
SDK_FLOAT	GetLimbAngleY     					( int iID, int iLimbID );
SDK_FLOAT	GetLimbAngleZ     					( int iID, int iLimbID );
SDK_LPSTR	GetLimbTextureName 					( SDK_RETSTR int iID, int iLimbID );
SDK_FLOAT	GetLimbXDirection 					( int iID, int iLimbID );
SDK_FLOAT	GetLimbYDirection 					( int iID, int iLimbID );
SDK_FLOAT	GetLimbZDirection 					( int iID, int iLimbID );
SDK_FLOAT	GetLimbXPosition  					( int iID, int iLimbID );
SDK_FLOAT	GetLimbYPosition					( int iID, int iLimbID );
SDK_FLOAT	GetLimbZPosition  					( int iID, int iLimbID );
int 		GetLimbLink 						( int iID, int iLimbID );

int 		GetStaticLineOfSight				( float fSx, float fSy, float fSz, float fDx, float fDy, float fDz, float fWidth, float fAccuracy );
SDK_FLOAT	GetStaticX							( void );
SDK_FLOAT	GetStaticY							( void );
SDK_FLOAT	GetStaticZ							( void );
int 		GetStaticHit						( float fOldX1, float fOldY1, float fOldZ1, float fOldX2, float fOldY2, float fOldZ2, float fNX1,   float fNY1,   float fNZ1,   float fNX2,   float fNY2,   float fNZ2   );
SDK_FLOAT	GetStaticLineOfSightX 				( void );
SDK_FLOAT	GetStaticLineOfSightY 				( void );
SDK_FLOAT	GetStaticLineOfSightZ 				( void );

int 		GetInScreen      					( int iID );
int			GetScreenX       					( int iID );
int 		GetScreenY       					( int iID );
int			PickScreenObject					( int iX, int iY, int iObjectStart, int iObjectEnd );
void		PickScreen2D23D						( int iX, int iY, float fDistance );
SDK_FLOAT	GetPickDistance						( void );
SDK_FLOAT	GetPickVectorX						( void );
SDK_FLOAT	GetPickVectorY						( void );
SDK_FLOAT	GetPickVectorZ						( void );

int   		GetAlphaBlending					( void );
SDK_BOOL    GetAlpha							( void );
SDK_BOOL    GetAlphaComparision					( void );
SDK_BOOL    GetAnisotropicFiltering				( void );
SDK_BOOL    GetAntiAliasEdges					( void );
SDK_BOOL    GetBlitSysOntoLocal					( void );
SDK_BOOL	GetCalibrateGamma					( void );
SDK_BOOL    GetClipAndScalePoints				( void );
SDK_BOOL    GetClipTLVerts						( void );
SDK_BOOL    GetColorPerspective					( void );
SDK_BOOL    GetColorWriteEnable					( void );
SDK_BOOL    GetCubeMap							( void );
SDK_BOOL    GetCullCCW							( void );
SDK_BOOL    GetCullCW							( void );
int			GetDeviceType						( void );
SDK_BOOL    GetDither							( void );
int  		GetBlending							( void );
SDK_BOOL    GetFogRange							( void );
SDK_BOOL    GetFogTable							( void );
SDK_BOOL    GetFogVertex						( void );
int   		GetFog								( void );
SDK_BOOL    GetFullScreenGamma					( void );
int			GetMaxLights						( void );
SDK_FLOAT	GetMaxPixelShaderValue				( void );
int			GetMaxTextureWidth					( void );
int			GetMaxTextureHeight					( void );
int			GetMaxVertexShaderConstants			( void );
int			GetMaxVolumeExtent					( void );
SDK_BOOL    GetMipCubeMap						( void );
SDK_BOOL    GetMipMap							( void );
SDK_BOOL    GetMipMapLODBias					( void );
SDK_BOOL    GetMipMapVolume						( void );
SDK_BOOL    GetNonPowerOf2Textures				( void );
SDK_BOOL    GetPerspective						( void );
SDK_FLOAT	GetPixelShaderVersion				( void );
SDK_BOOL    GetProjected						( void );
SDK_BOOL    GetRenderAfterFlip					( void );
SDK_BOOL	GetRenderWindowed					( void );
SDK_BOOL    GetSeparateTextureMemories			( void );
SDK_BOOL    GetSquareOnly						( void );
int   		GetTNLAvailable						( void );
SDK_BOOL    GetDrawPrimTLVertex					( void );
SDK_BOOL    GetHWTransformAndLight				( void );
SDK_BOOL    GetTLVertexSystemMemory				( void );
SDK_BOOL    GetTLVertexVideoMemory				( void );
SDK_BOOL    GetTextureNonLocalVideoMemory		( void );
SDK_BOOL    GetTextureSystemMemory				( void );
SDK_BOOL    GetTextureVideoMemory				( void );
SDK_FLOAT	GetVertexShaderVersion				( void );
SDK_BOOL    GetVolumeMap						( void );
SDK_BOOL    GetWBuffer							( void );
SDK_BOOL    GetWFog								( void );
SDK_BOOL    GetZBuffer							( void );
SDK_BOOL    GetZFog								( void );
int   		GetMeshExist						( int iID );
int 		GetEffectExist						( int iEffectID );
SDK_BOOL	VertexShaderExist			  		( int iShader );
SDK_BOOL	PixelShaderExist			  		( int iShader );

void		ConstructorBasic3D					( HINSTANCE hSetup, HINSTANCE hImage, HINSTANCE hTypes, HINSTANCE hCamera );
void		SetErrorHandlerBasic3D				( LPVOID pErrorHandlerPtr );
void		PassCoreDataBasic3D					( LPVOID pGlobPtr );
void		RefreshD3DBasic3D					( int iMode );
void		DestructorBasic3D					( void );

void dbtUpdateBasic3D				( void );
void UpdateNoZDepth		( void );
void StencilRenderStart	( void );
void StencilRenderEnd	( void );
void AutomaticStart		( void );
void AutomaticEnd		( void );

void 		MakePyramid							( int iID, float fSize );
void 		SetFrame							( int iID, int iFrame );

void 		DisableZRead						( int iID );
void 		EnableZRead							( int iID );
void 		DisableZWrite						( int iID );
void 		EnableZWrite						( int iID );
void		SetObjectFOV						( int iID, float fRadius );
void		Deletes								( int iFrom, int iTo );
void 		ClearObjectsOfTextureRef 			( LPDIRECT3DTEXTURE9 pTextureRef );
void 		DisableBias							( int iID );
void 		EnableZBias							( int iID, float fSlopeScale, float fDepth );
void 		SetTextureStage						( int iID, int iStage, int iImage );
void 		SetNormals							( int iID );
void 		SetTextureModeStage					( int iID, int iStage, int iMode, int iMipGeneration );
void 		SetTextureMD3						( int iID, int iH0, int iH1, int iL0, int iL1, int iL2, int iU0 );
void 		SetShadowColor						( int iRed, int iGreen, int iBlue, int iAlphaLevel );
void 		SetShadowShades						( int iShades );
void 		SetReflectionColor					( int iRed, int iGreen, int iBlue, int iAlphaLevel );

void 		SaveAnimation						( int iID, SDK_LPSTR pFilename );
void 		AppendAnimation						( int iID, SDK_LPSTR pFilename );
void 		ClearAllKeyFrames					( int iID );
void 		ClearKeyFrame						( int iID, int iFrame );
void 		SetKeyFrame							( int iID, int iFrame );

void 		CreateNodeTree						( float fX, float fY, float fZ );
void 		AddNodeTreeObject					( int iID, int iType, int iArbitaryValue, int iCastShadow, int iPortalBlocker );
void 		AddNodeTreeLimb						( int iID, int iLimb, int iType, int iArbitaryValue, int iCastShadow, int iPortalBlocker );
void 		RemoveNodeTreeObject				( int iID );
void 		DeleteNodeTree						( void );
void 		SetNodeTreeWireframeOn				( void );
void 		SetNodeTreeWireframeOff				( void );

void 		SetNodeTreeTextureMode				( int iMode );
void 		DisableNodeTreeOcclusion			( void );
void 		EnableNodeTreeOcclusion				( void );
void 		SaveNodeTreeObjects					( SDK_LPSTR pFilename );
void 		LoadNodeTreeObjects					( SDK_LPSTR pFilename, int iDivideTextureSize );
void 		AttachObjectToNodeTree				( int iID );
void 		DetachObjectFromNodeTree			( int iID );
void 		SetNodeTreePortalsOn				( void );
void 		SetNodeTreePortalsOff				( void );
void 		BuildNodeTreePortals				( void );
void 		SetNodeTreeScorchTexture			( int iImageID, int iWidth, int iHeight );
void 		AddNodeTreeScorch					( float fSize, int iType );
void 		AddNodeTreeLight					( int iLightIndex, float fX, float fY, float fZ, float fRange );

void 		PeformCSGUnion						( int iObjectA, int iObjectB );
void 		PeformCSGDifference					( int iObjectA, int iObjectB );
void 		PeformCSGIntersection				( int iObjectA, int iObjectB );
void 		PeformCSGClip						( int iObjectA, int iObjectB );
int 		ObjectBlocking						( int iID, float X1, float Y1, float Z1, float X2, float Y2, float Z2 );

void 		AddObjectToLightMapPool				( int iID );
void 		AddLimbToLightMapPool				( int iID, int iLimb );
void 		AddStaticObjectsToLightMapPool		( void );
void 		AddLightMapLight					( float fX, float fY, float fZ, float fRadius, float fRed, float fGreen, float fBlue, float fBrightness, SDK_BOOL bCastShadow );
void 		FlushLightMapLights					( void );
void 		CreateLightMaps						( int iLMSize, int iLMQuality, SDK_LPSTR dwPathForLightMaps );

void 		SetGlobalShadowsOn					( void );
void 		SetGlobalShadowsOff					( void );
void		ForceAutomaticEnd					( void );
void  		ReduceMesh							( int iMeshID, int iBlockMode, int iNearMode, int iGX, int iGY, int iGZ );

void  		LockVertexDataForLimbCore			( int iID, int iLimbID, int iReplaceOrUpdate );
void  		LockVertexDataForLimb				( int iID, int iLimbID );
void  		LockVertexDataForMesh				( int iID );
void  		UnlockVertexData					( void );
void  		SetVertexDataPosition				( int iVertex, float fX, float fY, float fZ );
void  		SetVertexDataNormals				( int iVertex, float fNX, float fNY, float fNZ );
void  		SetVertexDataDiffuse				( int iVertex, DWORD dwDiffuse );
void  		SetVertexDataUV						( int iVertex, float fU, float fV );
void  		SetVertexDataSize 					( int iVertex, float fSize );

void  		SetIndexData						( int iIndex, int iValue );
void		SetVertexDataUV						( int iVertex, int iIndex, float fU, float fV );

void  		AddMeshToVertexData					( int iMeshID );
void  		DeleteMeshFromVertexData			( int iVertex1, int iVertex2, int iIndex1, int iIndex2 );
void  		PeformCSGUnionOnVertexData			( int iBrushMeshID );
void  		PeformCSGDifferenceOnVertexData		( int iBrushMeshID );
void  		PeformCSGIntersectionOnVertexData	( int iBrushMeshID );
int   		GetVertexDataVertexCount			( void );
int   		GetVertexDataIndexCount				( void );
SDK_FLOAT	GetVertexDataPositionX				( int iVertex );
SDK_FLOAT	GetVertexDataPositionY				( int iVertex );
SDK_FLOAT	GetVertexDataPositionZ				( int iVertex );
SDK_FLOAT	GetVertexDataNormalsX				( int iVertex );
SDK_FLOAT	GetVertexDataNormalsY				( int iVertex );
SDK_FLOAT	GetVertexDataNormalsZ				( int iVertex );
DWORD		GetVertexDataDiffuse				( int iVertex );
SDK_FLOAT	GetVertexDataU						( int iVertex );
SDK_FLOAT	GetVertexDataV						( int iVertex );

SDK_FLOAT	GetVertexDataU						( int iVertex, int iIndex );
SDK_FLOAT	GetVertexDataV						( int iVertex, int iIndex );

void		SetMipmapMode						( int iMode );

void		DisableTNL							( void );
void		EnableTNL							( void );
void		Convert3DStoX						( DWORD pFilename1, DWORD pFilename2 );

void 		MakeEmitter							( int iID, int iSize );
void 		GetEmitterData  					( int iID, BYTE** ppVertices, DWORD* pdwVertexCount, int** ppiDrawCount );
void		UpdateEmitter   					( int iID );

void 		ExcludeOn	  						( int iID );
void 		ExcludeOff							( int iID );

float		GetXPosition						( int iID );
float		GetYPosition						( int iID );
float		GetZPosition						( int iID );
float		GetXRotation						( int iID );
float		GetYRotation						( int iID );
float		GetZRotation						( int iID );
D3DXVECTOR3	GetPosVec							( int iID );
D3DXVECTOR3	GetRotVec							( int iID );

int 		GetLimbCollision					( int iObjectA, int iLimbA, int iObjectB, int iLimbB );
int 		GetLimbHit							( int iObjectA, int iLimbA, int iObjectB, int iLimbB );

int 		GetStaticRayCast					( float fSx, float fSy, float fSz, float fDx, float fDy, float fDz );
int 		GetStaticVolumeCast					( float fX, float fY, float fZ, float fNewX, float fNewY, float fNewZ, float fSize );

int 		GetStaticFloor						( void );
int 		GetStaticColCount					( void );
int 		GetStaticColValue 					( void );
int 		GetLimbTexture 						( int iID, int iLimbID );
int			Get3DBLITAvailable					( void );
int   		GetStatistic						( int iCode );
int			GetLimbCount				  		( int iID );

SDK_BOOL    GetTexture							( void );

void 		SetDisableTransform					( int iID, bool bTransform );
void 		CreateMeshForObject					( int iID, DWORD dwFVF, DWORD dwVertexCount, DWORD dwIndexCount );
void 		SetWorldMatrix						( int iID, D3DXMATRIX* pMatrix );
void 		GetWorldMatrix						( int iID, int iLimb, D3DXMATRIX* pMatrix );

D3DXVECTOR3 GetCameraLook     					( void );
D3DXVECTOR3 GetCameraPosition					( void );
D3DXVECTOR3 GetCameraUp							( void );
D3DXVECTOR3 GetCameraRight						( void );
D3DXMATRIX  GetCameraMatrix						( void );
*/
/*
void		dbSetDeleteCallBack					( int iID, ON_OBJECT_DELETE_CALLBACK pfn, int userData );
void		dbGetPositionData					( int iID, sPositionData** ppPosition );
void		dbUpdatePositionStructure			( sPositionData* pPosition );
sObject*	dbGetObject							( int iID );
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DARK SDK FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
void dbLoadObject ( char* szFilename, int iID )
{
	Load ( ( DWORD ) szFilename, iID );
}

void dbLoadObject ( char* szFilename, int iID, int iDBProMode )
{
	Load ( ( DWORD ) szFilename, iID, iDBProMode );
}

void dbSaveObject ( char* szFilename, int iID )
{
	Save (  ( DWORD ) szFilename, iID );
}

void dbAppendObject ( char* pString, int iID, int iFrame )
{
	Append ( ( DWORD ) pString, iID, iFrame );
}

void dbCloneObject ( int iDestinationID, int iSourceID )
{
	Clone ( iDestinationID, iSourceID );
}

void dbInstanceObject ( int iDestinationID, int iSourceID )
{
	Instance ( iDestinationID, iSourceID );
}

void dbDeleteObject ( int iID )
{
	DeleteEx ( iID );
}

void dbShowObject ( int iID )
{
	Show ( iID );
}

void dbHideObject ( int iID )
{
	Hide ( iID );
}

void dbPositionObject ( int iID, float fX, float fY, float fZ )
{
	dbtObjectPosition ( iID, fX, fY, fZ );
}

void dbScaleObject ( int iID, float fX, float fY, float fZ )
{
	dbtObjectScale ( iID, fX, fY, fZ );
}

void dbRotateObject ( int iID, float fX, float fY, float fZ )
{
	dbtObjectRotate ( iID, fX, fY, fZ );
}

void dbMoveObject ( int iID, float fStep )
{
	dbtObjectMove ( iID, fStep );
}

void dbPointObject ( int iID, float fX, float fY, float fZ )
{
	dbtObjectPoint ( iID, fX, fY, fZ );
}

void dbMoveObjectDown ( int iID, float fStep )
{
	dbtObjectMoveDown ( iID, fStep );
}

void dbMoveObjectLeft ( int iID, float fStep )
{
	dbtObjectMoveLeft ( iID, fStep );
}

void dbMoveObjectRight ( int iID, float fStep )
{
	dbtObjectMoveRight ( iID, fStep );
}

void dbMoveObjectUp ( int iID, float fStep )
{
	dbtObjectMoveUp ( iID, fStep );
}

void dbXRotateObject ( int iID, float fX )
{
	dbtObjectXRotate ( iID, fX );
}

void dbYRotateObject ( int iID, float fY )
{
	dbtObjectYRotate ( iID, fY );
}

void dbZRotateObject ( int iID, float fZ )
{
	dbtObjectZRotate ( iID, fZ );
}

void dbTurnObjectLeft ( int iID, float fAngle )
{
	dbtObjectTurnLeft ( iID, fAngle );
}

void dbTurnObjectRight ( int iID, float fAngle )
{
	dbtObjectTurnRight ( iID, fAngle );
}

void dbPitchObjectUp ( int iID, float fAngle )
{
	dbtObjectPitchUp ( iID, fAngle );
}

void dbPitchObjectDown ( int iID, float fAngle )
{
	dbtObjectPitchDown ( iID, fAngle );
}

void dbRollObjectLeft ( int iID, float fAngle )
{
	dbtObjectRollLeft ( iID, fAngle );
}

void dbRollObjectRight ( int iID, float fAngle )
{
	dbtObjectRollRight ( iID, fAngle );
}

void dbPlayObject ( int iID )
{
	Play (  iID );
}

void dbPlayObject ( int iID, int iStart )
{
	Play (  iID,  iStart );
}

void dbPlayObject ( int iID, int iStart, int iEnd )
{
	Play (  iID,  iStart,  iEnd );
}

void dbLoopObject ( int iID )
{
	Loop (  iID );
}

void dbLoopObject ( int iID, int iStart )
{
	Loop (  iID,  iStart );
}

void dbLoopObject ( int iID, int iStart, int iEnd )
{
	Loop (  iID,  iStart,  iEnd );
}

void dbStopObject ( int iID )
{
	Stop (  iID );
}

void dbSetObjectFrame ( int iID, float fFrame )
{
	SetFrameEx ( iID, fFrame );
}

void dbSetObjectSpeed ( int iID, int iSpeed )
{
	SetSpeed ( iID, iSpeed );
}

void dbSetObjectInterpolation	( int iID, int iJump )
{
	SetInterpolation ( iID, iJump );
}

void dbMakeObject ( int iID, int iMeshID, int iTextureID )
{
	MakeObject (  iID,  iMeshID,  iTextureID );
}

void dbMakeObjectBox ( int iID, float fWidth, float fHeight, float fDepth )
{
	MakeBox (  iID,  fWidth,  fHeight,  fDepth );
}

void dbMakeObjectCone ( int iID, float fSize )
{
	MakeCone (  iID,  fSize );
}

void dbMakeObjectCube ( int iID, float iSize )
{
	MakeCube (  iID,  iSize );
}

void dbMakeObjectCylinder ( int iID, float fSize )
{
	MakeCylinder (  iID,  fSize );
}

void dbMakeObjectFromLimb ( int iID, int iSrcObj, int iLimbID )
{
	MakeObjectFromLimb (  iID,  iSrcObj,  iLimbID );
}

void dbMakeObjectPlane ( int iID, float fWidth, float fHeight )
{
	MakePlane (  iID,  fWidth,  fHeight );
}

void dbMakeObjectSphere ( int iID, float fRadius )
{
	MakeSphere (  iID,  fRadius );
}

void dbMakeObjectSphere ( int iID, float fRadius, int iRings, int iSegments )
{
	MakeSphere (  iID,  fRadius,  iRings,  iSegments );
}

void dbMakeObjectTriangle ( int iID, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3 )
{
	MakeTriangle (  iID,  x1,  y1,  z1,  x2,  y2,  z2,  x3,  y3,  z3 );
}

void dbTextureObject ( int iID, int iImage )
{
	SetTexture (  iID,  iImage );
}

void dbColorObject ( int iID, DWORD dwRGB )
{
	Color (  iID,  dwRGB );
}

void dbGhostObjectOn ( int iID )
{
	GhostOn (  iID );
}

void dbGhostObjectOn ( int iID, int iFlag )
{
	GhostOn (  iID,  iFlag );
}

void dbGhostObjectOff ( int iID )
{
	GhostOff ( iID );
}

void dbFadeObject ( int iID, float iPercentage )
{
	Fade (  iID,  iPercentage );
}

void dbLockObjectOn ( int iID )
{
	LockOn (  iID );
}

void dbLockObjectOff ( int iID )
{
	LockOff (  iID );
}

void dbSetObjectTexture ( int iID, int iMode, int iMipGeneration )
{
	SetTextureMode (  iID,  iMode,  iMipGeneration );
}

void dbScaleObjectTexture ( int iID, float fU, float fV )
{
	ScaleTexture (  iID,  fU,  fV );
}

void dbScrollObjectTexture ( int iID, float fU, float fV )
{
	ScrollTexture (  iID,  fU,  fV );
}

void dbSetObjectSmoothing ( int iID, float fAngle )
{
	SetSmoothing (  iID,  fAngle );
}

void dbShowObjectBounds ( int iID, int iBoxOnly )
{
	ShowBoundsEx (  iID, iBoxOnly );
}

void dbShowObjectBounds ( int iID )
{
	ShowBounds (  iID );
}

void dbHideObjectBounds ( int iID )
{
	HideBounds ( iID );
}

void dbSetObject ( int iID, SDK_BOOL bWireframe, SDK_BOOL bTransparency, SDK_BOOL bCull )
{
	Set (  iID,  bWireframe,  bTransparency,  bCull );
}

void dbSetObject ( int iID, SDK_BOOL bWireframe, SDK_BOOL bTransparency, SDK_BOOL bCull, int iFilter )
{
	Set (  iID,  bWireframe,  bTransparency,  bCull,  iFilter );
}

void dbSetObject ( int iID, SDK_BOOL bWireframe, SDK_BOOL bTransparency, SDK_BOOL bCull, int iFilter, SDK_BOOL bLight )
{
	Set (  iID,  bWireframe,  bTransparency,  bCull,  iFilter,  bLight );
}

void dbSetObject ( int iID, SDK_BOOL bWireframe, SDK_BOOL bTransparency, SDK_BOOL bCull, int iFilter, SDK_BOOL bLight, SDK_BOOL bFog )
{
	Set (  iID,  bWireframe,  bTransparency,  bCull,  iFilter,  bLight,  bFog );
}

void dbSetObject ( int iID, SDK_BOOL bWireframe, SDK_BOOL bTransparency, SDK_BOOL bCull, int iFilter, SDK_BOOL bLight, SDK_BOOL bFog, SDK_BOOL bAmbient )
{
	Set (  iID,  bWireframe,  bTransparency,  bCull,  iFilter,  bLight,  bFog,  bAmbient );
}

void dbSetObjectWireframe ( int iID, SDK_BOOL bFlag )
{
	SetWireframe ( iID, bFlag );
}

void dbSetObjectTransparency	( int iID, SDK_BOOL bFlag )
{
	SetTransparency	( iID, bFlag );
}

void dbSetObjectCull ( int iID, SDK_BOOL bCull )
{
	SetCull ( iID, bCull );
}

void dbSetObjectFilter ( int iID, int iFilter )
{
	SetFilter (  iID,  iFilter );
}

void dbSetObjectFilterStage ( int iID, int iStage, int iFilter )
{
	SetFilterStage (  iID,  iStage,  iFilter );
}

void dbSetObjectLight ( int iID, SDK_BOOL bLight )
{
	SetLight (  iID,  bLight );
}

void dbSetObjectFog ( int iID, SDK_BOOL bFog )
{
	SetFog (  iID,  bFog );
}

void dbSetObjectAmbient ( int iID, SDK_BOOL bAmbient )
{
	SetAmbient (  iID,  bAmbient );
}

void dbSetObjectRotationXYZ ( int iID )
{
	SetRotationXYZ (  iID );
}

void dbSetObjectRotationZYX ( int iID )
{
	SetRotationZYX ( iID );
}

void dbSetObjectToCameraOrientation ( int iID )
{
	SetToCameraOrientation ( iID );
}

void dbSetObjectToObjectOrientation ( int iID, int iWhichID )
{
	SetToObjectOrientation (  iID,  iWhichID );
}

void dbEnableObjectZDepth ( int iID )
{
	EnableZDepth (  iID );
}

void dbDisableObjectZDepth ( int iID )
{
	DisableZDepth (  iID );
}

void dbGlueObjectToLimb ( int iSource, int iTarget, int iLimb )
{
	GlueToLimb (  iSource,  iTarget,  iLimb );
}

void dbUnGlueObject ( int iID )
{
	UnGlue (  iID );
}

void dbFixObjectPivot ( int iID )
{
	FixPivot ( iID );
}

void dbSetObjectDiffuse ( int iID, DWORD dwRGB )
{
	SetDiffuseMaterial (  iID,  dwRGB );
}

void dbSetObjectAmbience ( int iID, DWORD dwRGB )
{
	SetAmbienceMaterial (  iID,  dwRGB );
}

void dbSetObjectSpecular ( int iID, DWORD dwRGB )
{
	SetSpecularMaterial (  iID,  dwRGB );
}

void dbSetObjectEmissive ( int iID, DWORD dwRGB )
{
	SetEmissiveMaterial (  iID,  dwRGB );
}

void dbSetObjectSpecularPower ( int iID, float fPower )
{
	SetSpecularPower (  iID,  fPower );
}

void dbSetLightMappingOn ( int iID, int iImage )
{
	SetLightMap (  iID,  iImage );
}

void dbSetDetailMappingOn ( int iID, int iImage )
{
	SetDetailMap (  iID,  iImage );
}

void dbSetBlendMappingOn ( int iID, int iImage, int iMode )
{
	SetBlendMap (  iID,  iImage,  iMode );
}

void dbSetSphereMappingOn ( int iID, int iSphereImage )
{
	SetSphereMap (  iID,  iSphereImage );
}

void dbSetCubeMappingOn ( int iID, int i1, int i2, int i3, int i4, int i5, int i6 )
{
	SetCubeMap (  iID,  i1,  i2,  i3,  i4,  i5,  i6 );
}

void dbSetBumpMappingOn ( int iID, int iBumpMap )
{
	SetBumpMap (  iID,  iBumpMap );
}

void dbSetCartoonShadingOn ( int iID, int iStandardImage, int iEdgeImage )
{
	SetCartoonShadingOn (  iID,  iStandardImage,  iEdgeImage );
}

void dbSetRainbowShadingOn ( int iID, int iStandardImage )
{
	SetRainbowRenderingOn (  iID,  iStandardImage );
}

void dbSetShadowShadingOn ( int iID )
{
	SetShadowOn ( iID );
}

void dbSetReflectionShadingOn ( int iID )
{
	SetReflectionOn ( iID );
}

void dbSetAlphaMappingOn ( int iID, float fPercentage )
{
	SetAlphaFactor (  iID,  fPercentage );
}

void dbSetEffectOn ( int iID, char* pFilename, int iUseDefaultTextures )
{
	SetEffectOn (  iID, ( DWORD ) pFilename,  iUseDefaultTextures );
}

void dbSetShadingOff ( int iID )
{
	SetShadingOff (  iID );
}

void dbLoadEffect ( char* pFilename, int iEffectID, int iUseDefaultTextures )
{
	LoadEffect ( ( DWORD ) pFilename,  iEffectID,  iUseDefaultTextures );
}

void dbDeleteEffect ( int iEffectID )
{
	DeleteEffect (  iEffectID );
}

void dbSetObjectEffect ( int iID, int iEffectID )
{
	SetObjectEffect (  iID, iEffectID );
}

void dbSetLimbEffect ( int iID, int iLimbID, int iEffectID )
{
	SetLimbEffect (  iID,  iLimbID,  iEffectID );
}

void dbPerformChecklistForEffectValues ( int iEffectID )
{
	PerformChecklistForEffectValues ( iEffectID );
}

void dbPerformChecklistForEffectErrors ( void )
{
	PerformChecklistForEffectErrors ( );
}

void dbPerformChecklistForEffectErrors ( int iEffectID )
{
	PerformChecklistForEffectErrors ( iEffectID );
}

void dbSetEffectConstantBoolean ( int iEffectID, char* pConstantName, int iValue )
{
	SetEffectConstantB ( iEffectID, ( DWORD )pConstantName,  iValue );
}

void dbSetEffectConstantInteger ( int iEffectID, char* pConstantName, int iValue )
{
	SetEffectConstantI (  iEffectID,  ( DWORD )pConstantName,  iValue );
}

void dbSetEffectConstantFloat ( int iEffectID, char* pConstantName, float fValue )
{
	SetEffectConstantF (  iEffectID,  ( DWORD )pConstantName,  fValue );
}

void dbSetEffectConstantVector ( int iEffectID, char* pConstantName, int iValue )
{
	SetEffectConstantV (  iEffectID,  ( DWORD )pConstantName,  iValue );
}

void dbSetEffectConstantMatrix ( int iEffectID, char* pConstantName, int iValue )
{
	SetEffectConstantM (  iEffectID,  ( DWORD )pConstantName,  iValue );
}

void dbSetEffectTechnique ( int iEffectID, char* pTechniqueName )
{
	SetEffectTechnique (  iEffectID,  ( DWORD )pTechniqueName );
}

void dbSetEffectTranspose ( int iEffectID, int iTransposeFlag )
{
	SetEffectTranspose (  iEffectID,  iTransposeFlag );
}

void dbMakeObjectCollisionBox ( int iID, float iX1, float iY1, float iZ1, float iX2, float iY2, float iZ2, int iRotatedBoxFlag )
{
	MakeCollisionBox (  iID,  iX1,  iY1,  iZ1,  iX2,  iY2,  iZ2,  iRotatedBoxFlag );
}

void dbDeleteObjectCollisionBox ( int iID )
{
	DeleteCollisionBox ( iID );
}

void dbSetObjectCollisionOn ( int iID )
{
	SetCollisionOn ( iID );
}

void dbSetObjectCollisionOff ( int iID )
{
	SetCollisionOff ( iID );
}

void dbSetObjectCollisionToBoxes ( int iID )
{
	SetCollisionToBoxes ( iID );
}

void dbSetObjectCollisionToPolygons ( int iID )
{
	SetCollisionToPolygons ( iID );
}

void dbSetObjectCollisionToSpheres ( int iID )
{
	SetCollisionToSpheres ( iID );
}

void dbSetGlobalCollisionOn ( void )
{
	SetGlobalCollisionOn ( );
}

void dbSetGlobalCollisionOff ( void )
{
	SetGlobalCollisionOff ( );
}

void dbSetObjectRadius ( int iID, float fRadius )
{
	SetSphereRadius ( iID, fRadius );
}

void dbAutomaticObjectCollision	( int iObjectID, float fRadius, int iResponse )
{
	AutomaticObjectCollision	(  iObjectID,  fRadius,  iResponse );
}

void dbAutomaticCameraCollision	( int iCameraID, float fRadius, int iResponse )
{
	AutomaticCameraCollision	(  iCameraID,  fRadius,  iResponse );
}

void dbMakeStaticCollisionBox	( float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2 )
{
	MakeNodeTreeCollisionBox	(  fX1,  fY1,  fZ1,  fX2,  fY2,  fZ2 );
}

void dbPerformCheckListForObjectLimbs ( int iID )
{
	PerformCheckListForLimbs	( iID );
}

void dbOffsetLimb ( int iID, int iLimbID, float fX, float fY, float fZ )
{
	OffsetLimb (  iID,  iLimbID,  fX,  fY,  fZ );
}

void dbOffsetLimb ( int iID, int iLimbID, float fX, float fY, float fZ, int iBoundFlag )
{
	OffsetLimb (  iID,  iLimbID,  fX,  fY,  fZ,  iBoundFlag );
}

void dbScaleLimb ( int iID, int iLimbID, float fX, float fY, float fZ )
{
	ScaleLimb (  iID,  iLimbID,  fX,  fY,  fZ );
}

void dbScaleLimb ( int iID, int iLimbID, float fX, float fY, float fZ, int iBoundFlag )
{
	ScaleLimb (  iID,  iLimbID,  fX,  fY,  fZ,  iBoundFlag );
}

void dbRotateLimb ( int iID, int iLimbID, float fX, float fY, float fZ )
{
	RotateLimb (  iID,  iLimbID,  fX,  fY,  fZ );
}

void dbRotateLimb ( int iID, int iLimbID, float fX, float fY, float fZ, int iBoundFlag )
{
	RotateLimb (  iID,  iLimbID,  fX,  fY,  fZ,  iBoundFlag );
}

void dbShowLimb ( int iID, int iLimbID )
{
	ShowLimb (  iID,  iLimbID );
}

void dbHideLimb ( int iID, int iLimbID )
{
	HideLimb (  iID,  iLimbID );
}

void dbTextureLimb ( int iID, int iLimbID, int iImageID )
{
	TextureLimb (  iID,  iLimbID,  iImageID );
}

void dbTextureLimbStage ( int iID, int iLimbID, int iStage, int iImageID )
{
	TextureLimbStage (  iID,  iLimbID,  iStage,  iImageID );
}

void dbColorLimb ( int iID, int iLimbID, DWORD dwColor )
{
	ColorLimb (  iID,  iLimbID,  dwColor );
}

void dbScaleLimbTexture ( int iID, int iLimbID, float fU, float fV )
{
	ScaleLimbTexture (  iID,  iLimbID,  fU,  fV );
}

void dbScrollLimbTexture ( int iID, int iLimbID, float fU, float fV )
{
	ScrollLimbTexture (  iID,  iLimbID,  fU,  fV );
}

void dbAddLimb ( int iID, int iLimbID, int iMeshID )
{
	AddLimb (  iID,  iLimbID,  iMeshID );
}

void dbLinkLimb ( int iID, int iParentID, int iLimbID )
{
	LinkLimb (  iID,  iParentID,  iLimbID );
}

void dbRemoveLimb ( int iID, int iLimbID )
{
	RemoveLimb (  iID,  iLimbID );
}

void dbSetLimbSmoothing ( int iID, int iLimbID, float fPercentage )
{
	SetLimbSmoothing (  iID,  iLimbID,  fPercentage );
}

void dbSetLimbNormals ( int iID, int iLimbID )
{
	SetLimbNormals (  iID,  iLimbID );
}

void dbLoadMesh ( char* pFilename, int iID )
{
	LoadMesh (  ( DWORD ) pFilename,  iID );
}

void dbSaveMesh ( char* pFilename, int iMeshID )
{
	SaveMesh (  ( DWORD ) pFilename,  iMeshID );
}

void dbDeleteMesh ( int iID )
{
	DeleteMesh ( iID );
}

void dbChangeMesh ( int iObjectID, int iLimbID, int iMeshID )
{
	ChangeMesh (  iObjectID,  iLimbID,  iMeshID );
}

void dbMakeMeshFromObject ( int iID, int iObjectID )
{
	MakeMeshFromObject (  iID,  iObjectID );
}

void dbCreateVertexShaderFromFile ( int iID, char* szFile )
{
	CreateVertexShaderFromFile (  iID,  ( DWORD ) szFile );
}

void dbDeleteVertexShader ( int iShader )
{
	DeleteVertexShader (  iShader );
}

void dbSetVertexShaderOn ( int iID, int iShader )
{
	SetVertexShaderOn (  iID,  iShader );
}

void dbSetVertexShaderOff ( int iID )
{
	SetVertexShaderOff (  iID );
}

void dbSetVertexShaderVector ( int iID, DWORD dwRegister, int iVector, DWORD dwConstantCount )
{
	SetVertexShaderVector (  iID,  dwRegister,  iVector,  dwConstantCount );
}

void dbSetVertexShaderMatrix ( int iID, DWORD dwRegister, int iMatrix, DWORD dwConstantCount )
{
	SetVertexShaderMatrix (  iID,  dwRegister,  iMatrix,  dwConstantCount );
}

void dbSetVertexShaderStream ( int iID, int iStreamPos, int iData, int iDataType )
{
	SetVertexShaderStream (  iID,  iStreamPos,  iData,  iDataType );
}

void dbSetVertexShaderStreamCount ( int iID, int iCount )
{
	SetVertexShaderStreamCount (  iID,  iCount );
}

void dbConvertObjectFVF ( int iID, DWORD dwFVF )
{
	CloneMeshToNewFormat (  iID,  dwFVF );
}

void dbCreatePixelShaderFromFile	( int iID, char* szFile )
{
	CreatePixelShaderFromFile	( iID, ( DWORD ) szFile );
}

void dbDeletePixelShader ( int iShader )
{
	DeletePixelShader ( iShader );
}

void dbSetPixelShaderOn ( int iID, int iShader )
{
	SetPixelShaderOn (  iID,  iShader );
}

void dbSetPixelShaderOff ( int iID )
{
	SetPixelShaderOff ( iID );
}

void dbSetPixelShaderTexture ( int iShader, int iSlot, int iTexture )
{
	SetPixelShaderTexture (  iShader,  iSlot,  iTexture );
}

void dbFlushVideoMemory ( void )
{
	FlushVideoMemory ( );
}

int dbObjectExist ( int iID )
{
	return GetExist ( iID );
}

int dbObjectVisible ( int iID )
{
	return GetVisible (  iID );
}

int dbObjectPlaying ( int iID )
{
	return GetPlaying ( iID );
}

int dbObjectLooping ( int iID )
{
	return GetLooping ( iID );
}

float dbObjectPositionX ( int iID )
{
	DWORD dwReturn = dbtObjectGetXPositionEx ( iID );
	
	return *( float* ) &dwReturn;
}

float dbObjectPositionY ( int iID )
{
	DWORD dwReturn = dbtObjectGetYPositionEx ( iID );
	
	return *( float* ) &dwReturn;
}

float dbObjectPositionZ ( int iID )
{
	DWORD dwReturn = dbtObjectGetZPositionEx ( iID );
	
	return *( float* ) &dwReturn;
}

float dbObjectSize ( int iID )
{
	DWORD dwReturn = GetSize ( iID );
	
	return *( float* ) &dwReturn;
}

float dbObjectSizeX ( int iID )
{
	DWORD dwReturn = GetXSize ( iID );
	
	return *( float* ) &dwReturn;
}

float dbObjectSizeY ( int iID )
{
	DWORD dwReturn = GetYSize ( iID );
	
	return *( float* ) &dwReturn;
}

float dbObjectSizeZ ( int iID )
{
	DWORD dwReturn = GetZSize ( iID );
	
	return *( float* ) &dwReturn;
}

float dbObjectAngleX ( int iID )
{
	DWORD dwReturn = dbtObjectGetXRotationEx ( iID );
	
	return *( float* ) &dwReturn;
}

float dbObjectAngleY ( int iID )
{
	DWORD dwReturn = dbtObjectGetYRotationEx ( iID );
	
	return *( float* ) &dwReturn;
}

float dbObjectAngleZ ( int iID )
{
	DWORD dwReturn = dbtObjectGetZRotationEx ( iID );
	
	return *( float* ) &dwReturn;
}

float dbObjectFrame ( int iID )
{
	DWORD dwReturn = GetFrame ( iID );
	
	return *( float* ) &dwReturn;
}

float dbObjectSpeed ( int iID )
{
	DWORD dwReturn = GetSpeed ( iID );
	
	return *( float* ) &dwReturn;
}

float dbObjectInterpolation ( int iID )
{
	DWORD dwReturn = GetInterpolation ( iID );
	
	return *( float* ) &dwReturn;
}

int dbTotalObjectFrames ( int iID )
{
	return GetNumberOfFrames ( iID );
}

int dbObjectHit ( int iObjectA, int iObjectB )
{
	return GetHit ( iObjectA, iObjectB );
}

int dbObjectCollision ( int iObjectA, int iObjectB )
{
	return GetCollision ( iObjectA, iObjectB );
}

float dbObjectCollisionX ( void )
{
	DWORD dwReturn = GetCollisionX ( );
	
	return *( float* ) &dwReturn;
}

float dbObjectCollisionY ( void )
{
	DWORD dwReturn = GetCollisionY ( );
	
	return *( float* ) &dwReturn;
}

float dbObjectCollisionZ ( void )
{
	DWORD dwReturn = GetCollisionZ ( );
	
	return *( float* ) &dwReturn;
}

float dbObjectCollisionRadius ( int iID )
{
	DWORD dwReturn = GetObjectCollisionRadius ( iID );
	
	return *( float* ) &dwReturn;
}

float dbObjectCollisionCenterX ( int iID )
{
	DWORD dwReturn = GetObjectCollisionCenterX ( iID );
	
	return *( float* ) &dwReturn;
}

float dbObjectCollisionCenterY ( int iID )
{
	DWORD dwReturn = GetObjectCollisionCenterY ( iID );
	
	return *( float* ) &dwReturn;
}

float dbObjectCollisionCenterZ ( int iID )
{
	DWORD dwReturn = GetObjectCollisionCenterZ ( iID );
	
	return *( float* ) &dwReturn;
}

float dbIntersectObject ( int iObjectID, float fX, float fY, float fZ, float fNewX, float fNewY, float fNewZ )
{
	DWORD dwReturn = IntersectObject (  iObjectID,  fX,  fY,  fZ,  fNewX,  fNewY,  fNewZ );
	
	return *( float* ) &dwReturn;
}

int dbLimbExist ( int iID, int iLimbID )
{
	return GetLimbExist (  iID,  iLimbID );
}

char* dbLimbName ( int iID, int iLimbID )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = GetLimbName ( NULL,  iID, iLimbID );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

int dbLimbVisible ( int iID, int iLimbID )
{
	return GetLimbVisible (  iID, iLimbID );
}

float dbLimbOffsetX ( int iID, int iLimbID )
{
	DWORD dwReturn = GetLimbOffsetX (  iID,  iLimbID );
	
	return *( float* ) &dwReturn;
}

float dbLimbOffsetY ( int iID, int iLimbID )
{
	DWORD dwReturn = GetLimbOffsetY (  iID,  iLimbID );
	
	return *( float* ) &dwReturn;
}

float dbLimbOffsetZ ( int iID, int iLimbID )
{
	DWORD dwReturn = GetLimbOffsetZ (  iID,  iLimbID );
	
	return *( float* ) &dwReturn;
}

float dbLimbScaleX ( int iID, int iLimbID )
{
	DWORD dwReturn = GetLimbScaleX (  iID,  iLimbID );
	
	return *( float* ) &dwReturn;
}

float dbLimbScaleY ( int iID, int iLimbID )
{
	DWORD dwReturn = GetLimbScaleY (  iID,  iLimbID );
	
	return *( float* ) &dwReturn;
}

float dbLimbScaleZ ( int iID, int iLimbID )
{
	DWORD dwReturn = GetLimbScaleZ (  iID,  iLimbID );
	
	return *( float* ) &dwReturn;
}

float dbLimbAngleX ( int iID, int iLimbID )
{
	DWORD dwReturn = GetLimbAngleX (  iID,  iLimbID );
	
	return *( float* ) &dwReturn;
}

float dbLimbAngleY ( int iID, int iLimbID )
{
	DWORD dwReturn = GetLimbAngleY (  iID,  iLimbID );
	
	return *( float* ) &dwReturn;
}

float dbLimbAngleZ ( int iID, int iLimbID )
{
	DWORD dwReturn = GetLimbAngleZ (  iID,  iLimbID );
	
	return *( float* ) &dwReturn;
}

char* dbLimbTextureName ( int iID, int iLimbID )
{
	static char* szReturn = NULL;
	DWORD		 dwReturn = GetLimbTextureName ( NULL,  iID, iLimbID );

	szReturn = ( char* ) dwReturn;

	return szReturn;
}

float dbLimbDirectionX ( int iID, int iLimbID )
{
	DWORD dwReturn = GetLimbXDirection ( iID, iLimbID );
	
	return *( float* ) &dwReturn;
}

float dbLimbDirectionY ( int iID, int iLimbID )
{
	DWORD dwReturn = GetLimbYDirection ( iID, iLimbID );
	
	return *( float* ) &dwReturn;
}

float dbLimbDirectionZ ( int iID, int iLimbID )
{
	DWORD dwReturn = GetLimbZDirection ( iID, iLimbID );
	
	return *( float* ) &dwReturn;
}

float dbLimbPositionX ( int iID, int iLimbID )
{
	DWORD dwReturn = GetLimbXPosition ( iID, iLimbID );
	
	return *( float* ) &dwReturn;
}

float dbLimbPositionY ( int iID, int iLimbID )
{
	DWORD dwReturn = GetLimbYPosition ( iID, iLimbID );
	
	return *( float* ) &dwReturn;
}

float dbLimbPositionZ ( int iID, int iLimbID )
{
	DWORD dwReturn = GetLimbZPosition ( iID, iLimbID );
	
	return *( float* ) &dwReturn;
}

int dbCheckLimbLink ( int iID, int iLimbID )
{
	return GetLimbLink ( iID, iLimbID );
}

int dbStaticLineOfSight ( float fSx, float fSy, float fSz, float fDx, float fDy, float fDz, float fWidth, float fAccuracy )
{
	return GetStaticLineOfSight (  fSx,  fSy,  fSz,  fDx,  fDy,  fDz,  fWidth,  fAccuracy );
}

float dbStaticLineOfSightX ( void )
{
	DWORD dwReturn = GetStaticLineOfSightX ( );
	
	return *( float* ) &dwReturn;
}

float dbStaticLineOfSightY ( void )
{
	DWORD dwReturn = GetStaticLineOfSightY ( );
	
	return *( float* ) &dwReturn;
}

float dbStaticLineOfSightZ ( void )
{
	DWORD dwReturn = GetStaticLineOfSightZ ( );
	
	return *( float* ) &dwReturn;
}

int dbGetStaticCollisionHit ( float fOldX1, float fOldY1, float fOldZ1, float fOldX2, float fOldY2, float fOldZ2, float fNX1, float fNY1, float fNZ1, float fNX2, float fNY2, float fNZ2 )
{
	return GetStaticHit ( fOldX1,  fOldY1,  fOldZ1,  fOldX2,  fOldY2,  fOldZ2,  fNX1,  fNY1,  fNZ1,  fNX2,  fNY2,  fNZ2 );
}

float dbGetStaticCollisionX ( void )
{
	DWORD dwReturn = GetStaticX ( );
	
	return *( float* ) &dwReturn;
}

float dbGetStaticCollisionY ( void )
{
	DWORD dwReturn = GetStaticY ( );
	
	return *( float* ) &dwReturn;
}

float dbGetStaticCollisionZ ( void )
{
	DWORD dwReturn = GetStaticZ ( );
	
	return *( float* ) &dwReturn;
}

int dbObjectInScreen ( int iID )
{
	return GetInScreen ( iID );
}

int dbObjectScreenX ( int iID )
{
	return GetScreenX ( iID );
}

int dbObjectScreenY ( int iID )
{
	return GetScreenY ( iID );
}

int dbPickObject ( int iX, int iY, int iObjectStart, int iObjectEnd )
{
	return PickScreenObject (  iX,  iY,  iObjectStart,  iObjectEnd );
}

void dbPickScreen ( int iX, int iY, float fDistance )
{
	PickScreen2D23D (  iX,  iY,  fDistance );
}

float	dbGetPickDistance ( void )
{
	DWORD dwReturn = GetPickDistance ( );
	
	return *( float* ) &dwReturn;
}

float	dbGetPickVectorX ( void )
{
	DWORD dwReturn = GetPickVectorX ( );
	
	return *( float* ) &dwReturn;
}

float	dbGetPickVectorY ( void )
{
	DWORD dwReturn = GetPickVectorY ( );
	
	return *( float* ) &dwReturn;
}

float	dbGetPickVectorZ ( void )
{
	DWORD dwReturn = GetPickVectorZ ( );
	
	return *( float* ) &dwReturn;
}

int dbGetAlphaBlending ( void )
{
	return GetAlphaBlending ( );
}

SDK_BOOL dbGetAlpha ( void )
{
	return GetAlpha ( );
}

SDK_BOOL dbGetAlphaComparision ( void )
{
	return GetAlphaComparision ( );
}

SDK_BOOL dbGetAnisotropicFiltering ( void )
{
	return GetAnisotropicFiltering ( );
}

SDK_BOOL dbGetAntiAliasEdges ( void )
{
	return GetAntiAliasEdges ( );
}

SDK_BOOL dbGetBlitSysOntoLocal ( void )
{
	return GetBlitSysOntoLocal ( );
}

SDK_BOOL	dbGetCalibrateGamma ( void )
{
	return GetCalibrateGamma ( );
}

SDK_BOOL dbGetClipAndScalePoints ( void )
{
	return GetClipAndScalePoints ( );
}

SDK_BOOL dbGetClipTLVerts ( void )
{
	return GetClipTLVerts ( );
}

SDK_BOOL dbGetColorPerspective ( void )
{
	return GetColorPerspective ( );
}

SDK_BOOL dbGetColorWriteEnable ( void )
{
	return GetColorWriteEnable ( );
}

SDK_BOOL dbGetCubeMap ( void )
{
	return GetCubeMap ( );
}

SDK_BOOL dbGetCullCCW ( void )
{
	return GetCullCCW ( );
}

SDK_BOOL dbGetCullCW ( void )
{
	return GetCullCW ( );
}

int dbGetDeviceType ( void )
{
	return GetDeviceType ( );
}

SDK_BOOL dbGetDither ( void )
{
	return GetDither ( );
}

int dbGetBlending ( void )
{
	return GetBlending ( );
}

SDK_BOOL dbGetFogRange ( void )
{
	return GetFogRange ( );
}

SDK_BOOL dbGetFogTable ( void )
{
	return GetFogTable ( );
}

SDK_BOOL dbGetFogVertex ( void )
{
	return GetFogVertex ( );
}

int dbGetFog ( void )
{
	return GetFog ( );
}

SDK_BOOL dbGetFullScreenGamma ( void )
{
	return GetFullScreenGamma ( );
}

int dbGetMaxLights ( void )
{
	return GetMaxLights ( );
}

float	dbGetMaxPixelShaderValue ( void )
{
	DWORD dwReturn = GetMaxPixelShaderValue ( );
	
	return *( float* ) &dwReturn;
}

int dbGetMaxTextureWidth ( void )
{
	return GetMaxTextureWidth ( );
}

int dbGetMaxTextureHeight ( void )
{
	return GetMaxTextureHeight ( );
}

int dbGetMaxVertexShaderConstants ( void )
{
	return GetMaxVertexShaderConstants ( );
}

int dbGetMaxVolumeExtent ( void )
{
	return GetMaxVolumeExtent ( );
}

SDK_BOOL dbGetMipCubeMap ( void )
{
	return GetMipCubeMap ( );
}

SDK_BOOL dbGetMipMap ( void )
{
	return GetMipMap ( );
}

SDK_BOOL dbGetMipMapLODBias ( void )
{
	return GetMipMapLODBias ( );
}

SDK_BOOL dbGetMipMapVolume ( void )
{
	return GetMipMapVolume ( );
}

SDK_BOOL dbGetNonPowerOf2Textures ( void )
{
	return GetNonPowerOf2Textures ( );
}

SDK_BOOL dbGetPerspective ( void )
{
	return GetPerspective ( );
}

float	dbGetPixelShaderVersion ( void )
{
	DWORD dwReturn = GetPixelShaderVersion ( );
	
	return *( float* ) &dwReturn;
}

SDK_BOOL dbGetProjected ( void )
{
	return GetProjected ( );
}

SDK_BOOL dbGetRenderAfterFlip ( void )
{
	return GetRenderAfterFlip ( );
}

SDK_BOOL	dbGetRenderWindowed ( void )
{
	return GetRenderWindowed ( );
}

SDK_BOOL dbGetSeparateTextureMemories ( void )
{
	return GetSeparateTextureMemories ( );
}

SDK_BOOL dbGetSquareOnly ( void )
{
	return GetSquareOnly ( );
}

int dbGetTNLAvailable ( void )
{
	return GetTNLAvailable ( );
}

SDK_BOOL dbGetDrawPrimTLVertex ( void )
{
	return GetDrawPrimTLVertex ( );
}

SDK_BOOL dbGetHWTransformAndLight ( void )
{
	return GetHWTransformAndLight ( );
}

SDK_BOOL dbGetTLVertexSystemMemory ( void )
{
	return GetTLVertexSystemMemory ( );
}

SDK_BOOL dbGetTLVertexVideoMemory ( void )
{
	return GetTLVertexVideoMemory ( );
}

SDK_BOOL dbGetTextureNonLocalVideoMemory ( void )
{
	return  GetTextureNonLocalVideoMemory ( );
}

SDK_BOOL dbGetTextureSystemMemory ( void )
{
	return GetTextureSystemMemory ( );
}

SDK_BOOL dbGetTextureVideoMemory ( void )
{
	return GetTextureVideoMemory ( );
}

float	dbGetVertexShaderVersion ( void )
{
	DWORD dwReturn = GetVertexShaderVersion ( );
	
	return *( float* ) &dwReturn;
}

SDK_BOOL dbGetVolumeMap ( void )
{
	return GetVolumeMap ( );
}

SDK_BOOL dbGetWBuffer ( void )
{
	return GetWBuffer ( );
}

SDK_BOOL dbGetWFog ( void )
{
	return GetWFog ( );
}

SDK_BOOL dbGetZBuffer ( void )
{
	return GetZBuffer ( );
}

SDK_BOOL dbGetZFog ( void )
{
	return GetZFog ( );
}

int dbGetMeshExist ( int iID )
{
	return GetMeshExist ( iID );
}

int dbGetEffectExist ( int iEffectID )
{
	return GetEffectExist (  iEffectID );
}

SDK_BOOL	dbVertexShaderExist ( int iShader )
{
	return VertexShaderExist ( iShader );
}

SDK_BOOL	dbPixelShaderExist ( int iShader )
{
	return PixelShaderExist ( iShader );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbConstructorBasic3D ( HINSTANCE hSetup, HINSTANCE hImage, HINSTANCE hTypes, HINSTANCE hCamera )
{
	ConstructorBasic3D ( hSetup, hImage, hTypes, hCamera );
}

void dbSetErrorHandlerBasic3D ( LPVOID pErrorHandlerPtr )
{
	SetErrorHandlerBasic3D ( pErrorHandlerPtr );
}

void dbPassCoreDataBasic3D ( LPVOID pGlobPtr )
{
	PassCoreDataBasic3D ( pGlobPtr );
}

void dbRefreshD3DBasic3D ( int iMode )
{
	RefreshD3DBasic3D ( iMode );
}

void dbDestructorBasic3D ( void )
{
	DestructorBasic3D ( );
}

void dbUpdateBasic3D				( void )
{
	dbtUpdateBasic3D	( );
}

void dbUpdateNoZDepth		( void )
{
	UpdateNoZDepth		(  );
}

void dbStencilRenderStart	( void )
{
	StencilRenderStart	(  );
}

void dbStencilRenderEnd	( void )
{
	StencilRenderEnd	(  );
}

void dbAutomaticStart		( void )
{
	AutomaticStart		(  );
}
void dbAutomaticEnd		( void )
{
	AutomaticEnd		(  );
}
*/

/*
void dbMakePyramid ( int iID, float fSize )
{
	MakePyramid (  iID,  fSize );
}

void dbSetFrame ( int iID, int iFrame )
{
	SetFrame (  iID,  iFrame );
}

void dbDisableZRead ( int iID )
{
	DisableZRead (  iID );
}

void dbEnableZRead ( int iID )
{
	EnableZRead ( iID );
}

void dbDisableZWrite ( int iID )
{
	DisableZWrite ( iID );
}

void dbEnableZWrite ( int iID )
{
	EnableZWrite ( iID );
}

void dbSetObjectFOV ( int iID, float fRadius )
{
	SetObjectFOV (  iID,  fRadius );
}

void dbDeletes ( int iFrom, int iTo )
{
	Deletes (  iFrom,  iTo );
}

void dbClearObjectsOfTextureRef ( LPDIRECT3DTEXTURE9 pTextureRef )
{
	ClearObjectsOfTextureRef (  pTextureRef );
}

void dbDisableBias ( int iID )
{
	DisableBias (  iID );
}

void dbEnableZBias ( int iID, float fSlopeScale, float fDepth )
{
	EnableZBias (  iID,  fSlopeScale,  fDepth );
}

void dbSetTextureStage ( int iID, int iStage, int iImage )
{
	SetTextureStage (  iID,  iStage,  iImage );
}

void dbSetNormals ( int iID )
{
	SetNormals ( iID );
}

void dbSetTextureModeStage ( int iID, int iStage, int iMode, int iMipGeneration )
{
	SetTextureModeStage (  iID,  iStage,  iMode,  iMipGeneration );
}

void dbSetTextureMD3 ( int iID, int iH0, int iH1, int iL0, int iL1, int iL2, int iU0 )
{
	SetTextureMD3 (  iID,  iH0,  iH1,  iL0,  iL1,  iL2,  iU0 );
}

void dbSetShadowColor ( int iRed, int iGreen, int iBlue, int iAlphaLevel )
{
	SetShadowColor (  iRed,  iGreen,  iBlue,  iAlphaLevel );
}

void dbSetShadowShades ( int iShades )
{
	SetShadowShades ( iShades );
}

void dbSetReflectionColor ( int iRed, int iGreen, int iBlue, int iAlphaLevel )
{
	SetReflectionColor (  iRed,  iGreen,  iBlue,  iAlphaLevel );
}

void dbSaveAnimation ( int iID, SDK_LPSTR pFilename )
{
	SaveAnimation (  iID,  pFilename );
}

void dbAppendAnimation ( int iID, SDK_LPSTR pFilename )
{
	AppendAnimation (  iID,  pFilename );
}

void dbClearAllKeyFrames ( int iID )
{
	ClearAllKeyFrames ( iID );
}

void dbClearKeyFrame ( int iID, int iFrame )
{
	ClearKeyFrame ( iID,  iFrame );
}

void dbSetKeyFrame ( int iID, int iFrame )
{
	SetKeyFrame (  iID,  iFrame );
}

void dbCreateNodeTree ( float fX, float fY, float fZ )
{
	CreateNodeTree (  fX,  fY,  fZ );
}

void dbAddNodeTreeObject ( int iID, int iType, int iArbitaryValue, int iCastShadow, int iPortalBlocker )
{
	AddNodeTreeObject (  iID,  iType,  iArbitaryValue,  iCastShadow,  iPortalBlocker );
}

void dbAddNodeTreeLimb ( int iID, int iLimb, int iType, int iArbitaryValue, int iCastShadow, int iPortalBlocker )
{
	AddNodeTreeLimb (  iID,  iLimb,  iType,  iArbitaryValue,  iCastShadow,  iPortalBlocker );
}

void dbRemoveNodeTreeObject ( int iID )
{
	RemoveNodeTreeObject ( iID );
}

void dbDeleteNodeTree ( void )
{
	DeleteNodeTree ( );
}

void dbSetNodeTreeWireframeOn ( void )
{
	SetNodeTreeWireframeOn ( );
}

void dbSetNodeTreeWireframeOff	( void )
{
	SetNodeTreeWireframeOff ( );
}

void dbSetNodeTreeTextureMode ( int iMode )
{
	SetNodeTreeTextureMode ( iMode );
}

void dbDisableNodeTreeOcclusion	( void )
{
	DisableNodeTreeOcclusion ( );
}

void dbEnableNodeTreeOcclusion	( void )
{
	EnableNodeTreeOcclusion ( );
}

void dbSaveNodeTreeObjects ( SDK_LPSTR pFilename )
{
	SaveNodeTreeObjects ( pFilename );
}

void dbLoadNodeTreeObjects ( SDK_LPSTR pFilename, int iDivideTextureSize )
{
	LoadNodeTreeObjects (  pFilename,  iDivideTextureSize );
}

void dbAttachObjectToNodeTree ( int iID )
{
	AttachObjectToNodeTree (  iID );
}

void dbDetachObjectFromNodeTree	( int iID )
{
	DetachObjectFromNodeTree	(  iID );
}

void dbSetNodeTreePortalsOn ( void )
{
	SetNodeTreePortalsOn ( );
}

void dbSetNodeTreePortalsOff ( void )
{
	SetNodeTreePortalsOff ( );
}

void dbBuildNodeTreePortals ( void )
{
	BuildNodeTreePortals ( );
}

void dbSetNodeTreeScorchTexture	( int iImageID, int iWidth, int iHeight )
{
	SetNodeTreeScorchTexture	(  iImageID,  iWidth,  iHeight );
}

void dbAddNodeTreeScorch ( float fSize, int iType )
{
	AddNodeTreeScorch (  fSize,  iType );
}

void dbAddNodeTreeLight ( int iLightIndex, float fX, float fY, float fZ, float fRange )
{
	AddNodeTreeLight (  iLightIndex,  fX,  fY,  fZ,  fRange );
}


void dbPeformCSGUnion ( int iObjectA, int iObjectB )
{
	dbPeformCSGUnion (  iObjectA,  iObjectB );
}

void dbPeformCSGDifference ( int iObjectA, int iObjectB )
{
	PeformCSGDifference ( iObjectA, iObjectB );
}

void dbPeformCSGIntersection ( int iObjectA, int iObjectB )
{
	PeformCSGIntersection (  iObjectA,  iObjectB );
}

void dbPeformCSGClip ( int iObjectA, int iObjectB )
{
	PeformCSGClip ( iObjectA,  iObjectB );
}

int dbObjectBlocking ( int iID, float X1, float Y1, float Z1, float X2, float Y2, float Z2 )
{
	return dbObjectBlocking (  iID,  X1,  Y1,  Z1,  X2,  Y2,  Z2 );
}

void dbAddObjectToLightMapPool ( int iID )
{
	AddObjectToLightMapPool ( iID );
}

void dbAddLimbToLightMapPool ( int iID, int iLimb )
{
	AddLimbToLightMapPool (  iID,  iLimb );
}

void dbAddStaticObjectsToLightMapPool ( void )
{
	AddStaticObjectsToLightMapPool ( );
}

void dbAddLightMapLight ( float fX, float fY, float fZ, float fRadius, float fRed, float fGreen, float fBlue, float fBrightness, SDK_BOOL bCastShadow )
{
	AddLightMapLight (  fX,  fY,  fZ,  fRadius,  fRed,  fGreen,  fBlue,  fBrightness,  bCastShadow );
}

void dbFlushLightMapLights ( void )
{
	FlushLightMapLights ( );
}

void dbCreateLightMaps ( int iLMSize, int iLMQuality, SDK_LPSTR dwPathForLightMaps )
{
	CreateLightMaps (  iLMSize,  iLMQuality,  dwPathForLightMaps );
}

void dbSetGlobalShadowsOn ( void )
{
	SetGlobalShadowsOn ( );
}

void dbSetGlobalShadowsOff ( void )
{
	SetGlobalShadowsOff ( );
}

void dbForceAutomaticEnd ( void )
{
	ForceAutomaticEnd ( );
}

void dbReduceMesh ( int iMeshID, int iBlockMode, int iNearMode, int iGX, int iGY, int iGZ )
{
	ReduceMesh (  iMeshID,  iBlockMode,  iNearMode,  iGX,  iGY,  iGZ );
}

void dbLockVertexDataForLimbCore ( int iID, int iLimbID, int iReplaceOrUpdate )
{
	LockVertexDataForLimbCore (  iID,  iLimbID,  iReplaceOrUpdate );
}

void dbLockVertexDataForLimb ( int iID, int iLimbID )
{
	LockVertexDataForLimb (  iID,  iLimbID );
}

void dbLockVertexDataForMesh ( int iID )
{
	LockVertexDataForMesh ( iID );
}

void dbUnlockVertexData ( void )
{
	UnlockVertexData ( );
}

void dbSetVertexDataPosition ( int iVertex, float fX, float fY, float fZ )
{
	SetVertexDataPosition (  iVertex,  fX,  fY,  fZ );
}

void dbSetVertexDataNormals ( int iVertex, float fNX, float fNY, float fNZ )
{
	SetVertexDataNormals (  iVertex,  fNX,  fNY,  fNZ );
}

void dbSetVertexDataDiffuse ( int iVertex, DWORD dwDiffuse )
{
	SetVertexDataDiffuse (  iVertex,  dwDiffuse );
}

void dbSetVertexDataUV ( int iVertex, float fU, float fV )
{
	SetVertexDataUV (  iVertex,  fU,  fV );
}

void dbSetVertexDataSize ( int iVertex, float fSize )
{
	SetVertexDataSize (  iVertex,  fSize );
}

void dbSetIndexData ( int iIndex, int iValue )
{
	SetIndexData (  iIndex,  iValue );
}

void dbSetVertexDataUV ( int iVertex, int iIndex, float fU, float fV )
{
	SetVertexDataUV (  iVertex,  iIndex,  fU,  fV );
}

void dbAddMeshToVertexData ( int iMeshID )
{
	AddMeshToVertexData (  iMeshID );
}

void dbDeleteMeshFromVertexData ( int iVertex1, int iVertex2, int iIndex1, int iIndex2 )
{
	DeleteMeshFromVertexData (  iVertex1,  iVertex2,  iIndex1,  iIndex2 );
}

void dbPeformCSGUnionOnVertexData ( int iBrushMeshID )
{
	PeformCSGUnionOnVertexData (  iBrushMeshID );
}

void dbPeformCSGDifferenceOnVertexData	( int iBrushMeshID )
{
	PeformCSGDifferenceOnVertexData	(  iBrushMeshID );
}

void dbPeformCSGIntersectionOnVertexData ( int iBrushMeshID )
{
	PeformCSGIntersectionOnVertexData ( iBrushMeshID );
}

int dbGetVertexDataVertexCount ( void )
{
	return GetVertexDataVertexCount ( );
}

int dbGetVertexDataIndexCount ( void )
{
	return GetVertexDataIndexCount ( );
}

SDK_FLOAT	dbGetVertexDataPositionX ( int iVertex )
{
	return GetVertexDataPositionX ( iVertex );
}

SDK_FLOAT	dbGetVertexDataPositionY ( int iVertex )
{
	return GetVertexDataPositionY ( iVertex );
}

SDK_FLOAT	dbGetVertexDataPositionZ ( int iVertex )
{
	return GetVertexDataPositionZ ( iVertex );
}

SDK_FLOAT	dbGetVertexDataNormalsX ( int iVertex )
{
	return GetVertexDataNormalsX ( iVertex );
}

SDK_FLOAT	dbGetVertexDataNormalsY ( int iVertex )
{
	return GetVertexDataNormalsY ( iVertex );
}

SDK_FLOAT	dbGetVertexDataNormalsZ ( int iVertex )
{
	return GetVertexDataNormalsZ ( iVertex );
}

DWORD dbGetVertexDataDiffuse ( int iVertex )
{
	return GetVertexDataDiffuse ( iVertex );
}

SDK_FLOAT	dbGetVertexDataU ( int iVertex )
{
	return GetVertexDataU ( iVertex );
}

SDK_FLOAT	dbGetVertexDataV ( int iVertex )
{
	return GetVertexDataV ( iVertex );
}

SDK_FLOAT	dbGetVertexDataU ( int iVertex, int iIndex )
{
	return GetVertexDataU (  iVertex, iIndex );
}

SDK_FLOAT	dbGetVertexDataV ( int iVertex, int iIndex )
{
	return GetVertexDataV ( iVertex, iIndex );
}

void dbSetMipmapMode ( int iMode )
{
	SetMipmapMode ( iMode );
}

void dbDisableTNL ( void )
{
	DisableTNL ( );
}

void dbEnableTNL ( void )
{
	EnableTNL ( );
}

void dbConvert3DStoX ( DWORD pFilename1, DWORD pFilename2 )
{
	Convert3DStoX (  pFilename1,  pFilename2 );
}

void dbMakeEmitter ( int iID, int iSize )
{
	MakeEmitter (  iID, iSize );
}

void dbGetEmitterData ( int iID, BYTE** ppVertices, DWORD* pdwVertexCount, int** ppiDrawCount )
{
	GetEmitterData (  iID,  ppVertices,  pdwVertexCount, ppiDrawCount );
}

void dbUpdateEmitter ( int iID )
{
	UpdateEmitter ( iID );
}
*/

/*
void dbGetPositionData ( int iID, sPositionData** ppPosition )
{
	GetPositionData ( iID, ppPosition );
}

*/

/*
void dbExcludeOn ( int iID )
{
	ExcludeOn ( iID );
}

void dbExcludeOff ( int iID )
{
	ExcludeOff ( iID );
}

int dbGetLimbCollision ( int iObjectA, int iLimbA, int iObjectB, int iLimbB )
{
	return GetLimbCollision (  iObjectA,  iLimbA,  iObjectB,  iLimbB );
}

int dbGetLimbHit ( int iObjectA, int iLimbA, int iObjectB, int iLimbB )
{
	return GetLimbHit (  iObjectA,  iLimbA,  iObjectB,  iLimbB );
}

int dbGetStaticRayCast ( float fSx, float fSy, float fSz, float fDx, float fDy, float fDz )
{
	return GetStaticRayCast (  fSx,  fSy,  fSz,  fDx,  fDy,  fDz );
}

int dbGetStaticVolumeCast ( float fX, float fY, float fZ, float fNewX, float fNewY, float fNewZ, float fSize )
{
	return GetStaticVolumeCast (  fX,  fY,  fZ,  fNewX,  fNewY,  fNewZ,  fSize );
}

int dbGetStaticFloor ( void )
{
	return GetStaticFloor ( );
}

int dbGetStaticColCount ( void )
{
	return GetStaticColCount ( );
}

int dbGetStaticColValue ( void )
{
	return GetStaticColValue ( );
}

int dbGetLimbTexture ( int iID, int iLimbID )
{
	return GetLimbTexture ( iID, iLimbID );
}

int dbGet3DBLITAvailable ( void )
{
	return Get3DBLITAvailable ( );
}

int dbGetStatistic ( int iCode )
{
	return GetStatistic ( iCode );
}

int dbGetLimbCount ( int iID )
{
	return GetLimbCount ( iID );
}

SDK_BOOL dbGetTexture ( void )
{
	return GetTexture ( );
}
*/

/*
void dbSetDeleteCallBack ( int iID, ON_OBJECT_DELETE_CALLBACK pfn, int userData )
{
	SetDeleteCallBack (  iID,  pfn,  userData );
}
*/

/*
void dbSetDisableTransform ( int iID, bool bTransform )
{
	SetDisableTransform (  iID,  bTransform );
}

void dbCreateMeshForObject ( int iID, DWORD dwFVF, DWORD dwVertexCount, DWORD dwIndexCount )
{
	CreateMeshForObject (  iID,  dwFVF,  dwVertexCount,  dwIndexCount );
}

void dbSetWorldMatrix ( int iID, D3DXMATRIX* pMatrix )
{
	SetWorldMatrix (  iID,  pMatrix );
}
*/

/*
void dbUpdatePositionStructure ( sPositionData* pPosition )
{
	UpdatePositionStructure (  pPosition );
}
*/

/*
void dbGetWorldMatrix ( int iID, int iLimb, D3DXMATRIX* pMatrix )
{
	GetWorldMatrix (  iID,  iLimb,  pMatrix );
}

D3DXVECTOR3 dbGetCameraLook ( void )
{
	return GetCameraLook ( );
}

D3DXVECTOR3 dbGetCameraPosition ( void )
{
	return GetCameraPosition ( );
}

D3DXVECTOR3 dbGetCameraUp ( void )
{
	return GetCameraUp ( );
}

D3DXVECTOR3 dbGetCameraRight ( void )
{
	return GetCameraRight ( );
}

D3DXMATRIX dbGetCameraMatrix ( void )
{
	return GetCameraMatrix ( );
}
*/

/*
sObject* dbGetObject ( int iID )
{
	return GetObject ( iID );
}
*/