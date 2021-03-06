#pragma once
#include<list>
#include<memory>

class Obj;

using sharedObj = std::shared_ptr<Obj>;
using sharedObjList = std::list <sharedObj>;
using sharedListObj = std::shared_ptr<sharedObjList>;
using weekListObj = std::weak_ptr<sharedObjList>;
using ListObj_itr = sharedObjList::iterator;

struct AddObjList		// objListの末尾に要素を追加する
{
	ListObj_itr operator()(weekListObj objList, sharedObj objPtr)
	{
		// objList.expired() true:終了している, false:続いている

		objList.lock()->push_back(std::move(objPtr));
		ListObj_itr itr = objList.lock()->end();
		itr--;
		return itr;
	}
};
