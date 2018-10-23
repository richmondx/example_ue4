#include "SluaFix.h"
#include "LuaObject.h"
#include "LuaCppBinding.h"
#include "SluaUserWidget.h"

#define SLUA_REFID_USERTABLE_MAPPING "SLUA_REFID_USERTABLE_MAPPING"

namespace slua {

    static int lua_tovalue(lua_State* L, int narg, int def)
    {
        return lua_gettop(L)<abs(narg) ? def : narg;
    }

    void SluaFix::init(lua_State* L)
    {
        lua_pushstring(L, SLUA_REFID_USERTABLE_MAPPING);
        lua_newtable(L);
        lua_rawset(L, LUA_REGISTRYINDEX);

        //slua fix
        REG_EXTENSION_METHOD_IMP(USluaUserWidget, "SetLuaTable", {
            CheckUD(USluaUserWidget,L,1);
            if (UD == nullptr) {
                return 0;
            }
            if (lua_istable(L, 2)) {
                int tbl = lua_tovalue(L, 2, 0);
                SluaFix::add_usertable_by_refid(L, UD->m_luaID, tbl);
                return 0;
            }
            if (lua_isnil(L, 2)) {
                SluaFix::remove_usertable_by_refid(L, UD->m_luaID);
                return 0;
            }
            return 0;
        });
        REG_EXTENSION_METHOD_IMP(USluaUserWidget, "GetLuaTable", {
            CheckUD(USluaUserWidget,L,1);
            if (UD == nullptr) {
                return 0;
            }
            return SluaFix::get_usertable_by_refid(L, UD->m_luaID);
        });

    }

    void SluaFix::print_lua_stack(lua_State* L, const char* pre)
    {
        int stackTop = lua_gettop(L);
        printf("%s  ", pre);
        for (int nIdx = stackTop; nIdx > 0; --nIdx)
        {
            int nType = lua_type(L, nIdx);
            const char* st = lua_typename(L, nType);
            const char* sv = luaL_tolstring(L, nIdx, NULL);
            lua_pop(L, 1);
            printf("%d:%s(%s)|", nIdx, st, sv);
        }
        stackTop = lua_gettop(L);
        printf("\n");
    }
    
    void SluaFix::add_usertable_by_refid(lua_State* L, int refid, int usertable)
    {
        lua_pushstring(L, SLUA_REFID_USERTABLE_MAPPING);
        lua_rawget(L, LUA_REGISTRYINDEX);                               /* stack: refid_usertable */
        lua_pushinteger(L, refid);                                      /* stack: refid_usertable refid */
        lua_pushvalue(L, usertable);                                    /* stack: refid_usertable refid usertable */
        lua_rawset(L, -3);                                              /* refid_usertable[refid] = usertable, stack: usertable */
        lua_pop(L, 1);                                                  /* stack: - */
    }
    
    int SluaFix::get_usertable_by_refid(lua_State* L, int refid)
    {
        lua_pushstring(L, SLUA_REFID_USERTABLE_MAPPING);
        lua_rawget(L, LUA_REGISTRYINDEX);                               /* stack: ... refid_usertable */
        lua_pushinteger(L, refid);                                      /* stack: ... refid_usertable refid */
        lua_rawget(L, -2);                                              /* stack: ... refid_usertable usertable */
        lua_remove(L, -2);
        return 1;
    }
    
    void SluaFix::remove_usertable_by_refid(lua_State* L, int refid)
    {
        lua_pushstring(L, SLUA_REFID_USERTABLE_MAPPING);
        lua_rawget(L, LUA_REGISTRYINDEX);                               /* stack: refid_usertable */
        lua_pushinteger(L, refid);                                      /* stack: refid_usertable refid */
        lua_pushnil(L);                                                 /* stack: refid_usertable refid nil */
        lua_rawset(L, -3);                                              /* refid_usertable[refid] = nil, stack: nil */
        lua_pop(L, 1);                                                  /* stack: - */
    }

}