#ifndef _CLIGHTDATA_H_
#define _CLIGHTDATA_H_

struct tagLightData
{
	D3DLIGHT9	light;

	int			iType;
	bool		bEnable;
	float		fRange;
	DWORD		dwColor;
};

#endif _CLIGHTDATA_H_
