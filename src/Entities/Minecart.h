
// Minecart.h

// Declares the cMinecart class representing a minecart in the world





#pragma once

#include "Entity.h"
#include "World.h"
#include "../UI/WindowOwner.h"





class cMinecart :
	public cEntity
{
	typedef cEntity super;
	
public:
	CLASS_PROTODEF(cMinecart)
	
	/** Minecart payload, values correspond to packet subtype */
	enum ePayload
	{
		mpNone    = 0,  // Empty minecart, ridable by player or mobs
		mpChest   = 1,  // Minecart-with-chest, can store a grid of 3*8 items
		mpFurnace = 2,  // Minecart-with-furnace, can be powered
		mpTNT     = 3,  // Minecart-with-TNT, can be blown up with activator rail
		mpHopper  = 5,  // Minecart-with-hopper, can be hopper
		// TODO: Spawner minecarts, (and possibly any block in a minecart with NBT editing)
	} ;
	
	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void HandlePhysics(float a_Dt, cChunk & a_Chunk) override;
	virtual bool DoTakeDamage(TakeDamageInfo & TDI) override;
	virtual void Destroyed() override;
	
	int LastDamage(void) const { return m_LastDamage; }
	ePayload GetPayload(void) const { return m_Payload; }
	
protected:
	ePayload m_Payload;
	int m_LastDamage;
	Vector3i m_DetectorRailPosition;
	bool m_bIsOnDetectorRail;
	
	cMinecart(ePayload a_Payload, double a_X, double a_Y, double a_Z);

	/** Handles physics on normal rails
		For each tick, slow down on flat rails, speed up or slow down on ascending/descending rails (depending on direction), and turn on curved rails
	*/
	void HandleRailPhysics(NIBBLETYPE a_RailMeta, float a_Dt);

	/** Handles powered rail physics
		Each tick, speed up or slow down cart, depending on metadata of rail (powered or not)
	*/
	void HandlePoweredRailPhysics(NIBBLETYPE a_RailMeta);

	/** Handles detector rail activation
		Activates detector rails when a minecart is on them. Calls HandleRailPhysics() for physics simulations
	*/
	void HandleDetectorRailPhysics(NIBBLETYPE a_RailMeta, float a_Dt);

	/** Handles activator rails - placeholder for future implementation */
	void HandleActivatorRailPhysics(NIBBLETYPE a_RailMeta, float a_Dt);

	/** Snaps a mincecart to a rail's axis, resetting its speed
		For curved rails, it changes the cart's direction as well as snapping it to axis */
	void SnapToRail(NIBBLETYPE a_RailMeta);
	/** Tests if a solid block is in front of a cart, and stops the cart (and returns true) if so; returns false if no obstruction */
	bool TestBlockCollision(NIBBLETYPE a_RailMeta);
	/** Tests if this mincecart's bounding box is intersecting another entity's bounding box (collision) and pushes mincecart away */
	bool TestEntityCollision(NIBBLETYPE a_RailMeta);

} ;





class cRideableMinecart :
	public cMinecart
{
	typedef cMinecart super;
	
public:
	CLASS_PROTODEF(cRideableMinecart)
	
	cRideableMinecart(double a_X, double a_Y, double a_Z, const cItem & a_Content, int a_Height);

	const cItem & GetContent(void) const {return m_Content;}
	int GetBlockHeight(void) const {return m_Height;}
	// cEntity overrides:
	virtual void OnRightClicked(cPlayer & a_Player) override;
protected:

	cItem m_Content;
	int m_Height;
} ;





class cMinecartWithChest :
	public cMinecart,
	public cItemGrid::cListener,
	public cEntityWindowOwner
{
	typedef cMinecart super;
	
public:
	CLASS_PROTODEF(cMinecartWithChest)
	
	cMinecartWithChest(double a_X, double a_Y, double a_Z);

	enum
	{
		ContentsHeight = 3,
		ContentsWidth = 9,
	};
	
	const cItem & GetSlot(int a_Idx) const { return m_Contents.GetSlot(a_Idx); }
	void SetSlot(size_t a_Idx, const cItem & a_Item) { m_Contents.SetSlot(static_cast<int>(a_Idx), a_Item); }

protected:
	cItemGrid m_Contents;
	void OpenNewWindow(void);
	virtual void Destroyed() override;

	// cItemGrid::cListener overrides:
	virtual void OnSlotChanged(cItemGrid * a_Grid, int a_SlotNum)
	{
		UNUSED(a_SlotNum);
		ASSERT(a_Grid == &m_Contents);
		if (m_World != nullptr)
		{
			if (GetWindow() != nullptr)
			{
				GetWindow()->BroadcastWholeWindow();
			}

			m_World->MarkChunkDirty(GetChunkX(), GetChunkZ());
		}
	}
	
	// cEntity overrides:
	virtual void OnRightClicked(cPlayer & a_Player) override;
} ;





class cMinecartWithFurnace :
	public cMinecart
{
	typedef cMinecart super;
	
public:
	CLASS_PROTODEF(cMinecartWithFurnace)
	
	cMinecartWithFurnace(double a_X, double a_Y, double a_Z);
	
	// cEntity overrides:
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	// Set functions.
	void SetIsFueled(bool a_IsFueled, int a_FueledTimeLeft = -1) {m_IsFueled = a_IsFueled; m_FueledTimeLeft = a_FueledTimeLeft;}

	// Get functions.
	int  GetFueledTimeLeft(void) const {return m_FueledTimeLeft; }
	bool IsFueled (void)         const {return m_IsFueled;}

private:

	int m_FueledTimeLeft;
	bool m_IsFueled;

} ;





class cMinecartWithTNT :
	public cMinecart
{
	typedef cMinecart super;
	
public:
	CLASS_PROTODEF(cMinecartWithTNT)
	
	cMinecartWithTNT(double a_X, double a_Y, double a_Z);
} ;





class cMinecartWithHopper :
	public cMinecart
{
	typedef cMinecart super;
	
public:
	CLASS_PROTODEF(cMinecartWithHopper)
	
	cMinecartWithHopper(double a_X, double a_Y, double a_Z);
} ;
