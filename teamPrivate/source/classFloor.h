#pragma once
#include <list>
#include <memory>

enum FLOOR_TYPE
{
	FLOOR_NORMAL_LEFT,
	FLOOR_NORMAL_RIGHT,
	FLOOR_MOVE_LEFT,
	FLOOR_MOVE_RIGHT,
	FLOOR_MAX
};

class BaseFloor;

using sharedFloor = std::shared_ptr<BaseFloor>;
using sharedFloorList = std::list<sharedFloor>;
using sharedListFloor = std::shared_ptr<sharedFloorList>;
using weakListFloor = std::weak_ptr<sharedFloorList>;
using ListFloor_itr = sharedFloorList::iterator;

struct AddFloorList	// AddObjList()(追加するﾘｽﾄのｼｪｱｰﾄﾞﾎﾟｲﾝﾀ,追加するｵﾌﾞｼﾞｪのﾕﾆｰｸﾎﾟｲﾝﾀ);
{
	ListFloor_itr operator()(weakListFloor floorList, sharedFloor floorPtr)
	{
		floorList.expired();
		// weakPtrから実態にｱｸｾｽするときはlock()が必要
		floorList.lock()->push_back(std::move(floorPtr));
		ListFloor_itr itr = floorList.lock()->end();
		itr--;
		return itr;
	}
};