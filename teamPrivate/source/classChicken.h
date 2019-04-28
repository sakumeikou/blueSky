#pragma once

#include <list>
#include <memory>

class Chicken;

using uniqueChicken = std::unique_ptr<Chicken>;
using uniqueChickenList = std::list<uniqueChicken>;
using sharedListChicken = std::shared_ptr<uniqueChickenList>;
using weakListChicken = std::weak_ptr<uniqueChickenList>;
using ListChicken_itr = uniqueChickenList::iterator;

struct AddChickenList	// AddObjList()(追加するﾘｽﾄのｼｪｱｰﾄﾞﾎﾟｲﾝﾀ,追加するｵﾌﾞｼﾞｪのﾕﾆｰｸﾎﾟｲﾝﾀ);
{
	ListChicken_itr operator()(weakListChicken chickenList, uniqueChicken chickenPtr)
	{
		chickenList.expired();
		// weakPtrから実態にｱｸｾｽするときはlock()が必要
		chickenList.lock()->push_back(std::move(chickenPtr));
		ListChicken_itr itr = chickenList.lock()->end();
		itr--;
		return itr;
	}
};