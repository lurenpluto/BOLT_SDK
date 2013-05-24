#pragma once

#include <vector>

template<typename T>
class BaseEventContainerT
{
public:

	BaseEventContainerT()
		:m_cookie(0)
	{}

	~BaseEventContainerT()
	{
		Clear();
	}

	size_t AttachEvent(T* lpEvent)
	{
		assert(lpEvent);

		EventNode node;
		node.cookie = m_cookie++;
		node.lpEvent = lpEvent;

		m_vtEvents.push_back(node);

		return node.cookie;
	}

	bool RemoveEvent(size_t cookie)
	{
		EventNode* lpNode;
		vtEventNode::iterator cp, end = m_vtEvents.end();
		for (cp = m_vtEvents.begin(); cp != end; ++cp)
		{
			lpNode = &*cp;
			if (lpNode->cookie == cookie)
			{
				assert(lpNode->lpEvent);
				delete lpNode->lpEvent;

				m_vtEvents.erase(cp);

				return true;
			}
		}

		assert(false);
		return false;
	}

	size_t GetEventCount() const
	{
		return m_vtEvents.size();
	}

	T* GetEventByIndex(size_t nIndex) const
	{
		if (nIndex >= m_vtEvents.size())
		{
			assert(false);
			return NULL;
		}

		return m_vtEvents[nIndex].lpEvent;
	}

	void Clear()
	{
		EventNode* lpNode;
		vtEventNode::iterator cp, end = m_vtEvents.end();
		for (cp = m_vtEvents.begin(); cp != end; ++cp)
		{
			lpNode = &*cp;
			assert(lpNode->lpEvent);
			delete lpNode->lpEvent;
		}

		m_vtEvents.clear();
	}

private:

	struct EventNode
	{
		size_t  cookie;
		T*      lpEvent;
	};

	typedef std::vector<EventNode> vtEventNode;

	vtEventNode m_vtEvents;
	size_t m_cookie;
};

class LuaBaseEventFunctor
{
public:
	LuaBaseEventFunctor(lua_State* luaState,long luaRef)
		:m_luaState(luaState),
		m_luaFunRef(luaRef)
	{
		assert(m_luaState);
	}

	virtual ~LuaBaseEventFunctor()
	{
		assert(m_luaState);
		if(m_luaState != NULL)
		{
			luaL_unref(m_luaState,LUA_REGISTRYINDEX,m_luaFunRef);
			m_luaState = NULL;
			m_luaFunRef = NULL;
		}
	}

protected:

	LuaBaseEventFunctor(const LuaBaseEventFunctor& other);
	LuaBaseEventFunctor& operator=(const LuaBaseEventFunctor& other);

protected:

	lua_State* m_luaState;
	long m_luaFunRef;
};

class LuaConfigChangeEvent
	: public LuaBaseEventFunctor
{
public:
	LuaConfigChangeEvent(lua_State* luaState,long luaRef);

	long operator()(const char* lpSection, const char* lpKey, const char* lpValue);
	long operator()(const char* lpSection, const char* lpKey, int value);
};