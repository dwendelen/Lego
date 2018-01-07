//
// Created by xtrit on 31.12.17.
//

#ifndef LEGO_SCRIPTENGINE_HPP
#define LEGO_SCRIPTENGINE_HPP

#include "lua.hpp"
#include "../engine/game_engine.hpp"
#include "../engine/Math.hpp"
#include "../engine/RenderingEngine.hpp"
#include "../engine/Input.hpp"
#include <string>

namespace lego {
    class BrickCache;
}

namespace lua {


// https://www.lua.org/manual/5.2/manual.html
    void dump_i(lua_State* l, int i);
    void stackdump_g(lua_State* l);

    class ScriptEngine {
    private:
        lua_State* L;
        void addFunctionToTable(std::string name, lua_CFunction function, void* closure);
    public:
        engine::EventBus& eventBus;
        engine::RenderingEngine& renderingEngine;
        lego::BrickCache& brickCache;

        bool quit = false;

        explicit ScriptEngine(engine::EventBus &eventBus, lego::BrickCache &brickCache, engine::RenderingEngine &renderingEngine)
            : eventBus(eventBus)
            , brickCache(brickCache)
            , renderingEngine(renderingEngine)
            , L(nullptr) {}
        void init();
        void update(engine::Input& input);
        ~ScriptEngine();

        template<class T>
        static T& getFromClosure(lua_State* L, int index) {
            return getFromStack<T>(L, lua_upvalueindex(index));
        }
        template<class T>
        static T& getFromStack(lua_State* L, int index) {
            void *userData = lua_touserdata(L, index);
            return *static_cast<T*>(userData);
        }
        static double getDouble(lua_State* L, int index, std::string key) {
            lua_getfield(L, index, key.c_str());
            double result = luaL_checknumber(L, -1);
            lua_pop(L, 1);
            return result;
        }
        static float getFloat(lua_State* L, int index, std::string key) {
            return static_cast<float> (getDouble(L, index, key));
        }
        static int getInteger(lua_State* L, int index, std::string key) {
            lua_getfield(L, index, key.c_str());
            int result = luaL_checkinteger(L, -1);
            lua_pop(L, 1);
            return result;
        }
        static OVR::Quatf toQuatf(lua_State* L, int index) {
            return {
                    getFloat(L, index, "x"),
                    getFloat(L, index, "y"),
                    getFloat(L, index, "z"),
                    getFloat(L, index, "w")
            };
        }
        static OVR::Vector3i toVec3i(lua_State* L, int index) {
            return {
                    getInteger(L, index, "x"),
                    getInteger(L, index, "y"),
                    getInteger(L, index, "z")
            };
        }
        static OVR::Vector3f toVec3f(lua_State* L, int index) {
            return {
                    getFloat(L, index, "x"),
                    getFloat(L, index, "y"),
                    getFloat(L, index, "z")
            };
        }
        static OVR::Vector4f toVec4fFromColor(lua_State* L, int index) {
            return {
                    getFloat(L, index, "r"),
                    getFloat(L, index, "g"),
                    getFloat(L, index, "b"),
                    getFloat(L, index, "a")
            };
        }
    };
}

#endif //LEGO_SCRIPTENGINE_HPP
