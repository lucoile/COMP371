//
// Created by Thomas Buffard on 8/17/20.
//

#ifndef _VOXEL_H_
#define _VOXEL_H_

#include "mesh_m.h"

enum VoxelType
{
	VoxelType_Default = 0,
	VoxelType_Grass,
	VoxelType_Dirt,
	VoxelType_Water,
	VoxelType_Stone,
	VoxelType_Wood,
	VoxelType_Sand,

	VoxelType_NumTypes
};

class Voxel
{
 public:
	constexpr static float VOXEL_RENDER_SIZE = 0.1f;

	bool m_active;

	Voxel(bool active = false) { this->m_active = active; m_voxelType = VoxelType_Default;};
	~Voxel() {};

	bool IsActive()
	{
		return m_active;
	}

	void SetActive(bool active = true)
	{
		m_active = active;
	}

	void SetType(VoxelType type)
	{
        m_voxelType = type;
	}

	VoxelType getType()
	{
        return m_voxelType;
	}

 private:
	VoxelType m_voxelType;
};

#endif //_VOXEL_H_