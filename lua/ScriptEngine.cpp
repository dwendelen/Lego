//
// Created by xtrit on 31.12.17.
//

#include "ScriptEngine.hpp"

#include <iostream>
#include <cstring>

#include "SDL_gamecontroller.h"
#include "../lego/BrickCache.hpp"
#include "../engine/Events.hpp"
#include "../engine/EventBus.hpp"

using namespace std;
using namespace lua;
using namespace engine;

void lua::dump_i(lua_State* l, int i) {
    int t = lua_type(l, i);
    switch (t) {
        case LUA_TSTRING:  /* strings */
            printf("string: '%s'\n", lua_tostring(l, i));
            break;
        case LUA_TBOOLEAN:  /* booleans */
            printf("boolean %s\n",lua_toboolean(l, i) ? "true" : "false");
            break;
        case LUA_TNUMBER:  /* numbers */
            printf("number: %g\n", lua_tonumber(l, i));
            break;
        default:  /* other values */
            printf("%s\n", lua_typename(l, t));
            break;
    }
    printf("  ");  /* put a separator */
}

// https://www.lua.org/manual/5.2/manual.html
void lua::stackdump_g(lua_State* l)
{
    int i;
    int top = lua_gettop(l);

    printf("total in stack %d\n",top);

    for (i = 1; i <= top; i++)
    {  /* repeat for each level */
        dump_i(l, i);
    }
    printf("\n");  /* end the listing */
}

int engine_getBrick(lua_State* L) {
    lego::BrickCache& brickCache = ScriptEngine::getFromClosure<lego::BrickCache>(L, 1);
    OVR::Vector3i size = ScriptEngine::toVec3i(L, 1);
    lego::BrickModel *model = brickCache.getBrick(size.x, size.y, size.z);

    cout << "getBrick" << size.x << size.y << size.z;

    lua_pushlightuserdata(L, model);

    return 1;
}

int engine_newObject(lua_State* L) {
    ScriptEngine &scriptEngine = ScriptEngine::getFromClosure<ScriptEngine>(L, 1);

    EventBus& eventBus = scriptEngine.eventBus;
    RenderingEngine& renderingEngine = scriptEngine.renderingEngine;

    Object object = renderingEngine.newObject();

    engine::ModelData& model = ScriptEngine::getFromStack<ModelData>(L, 1);
    OVR::Vector3f position = ScriptEngine::toVec3f(L, 2);
    OVR::Quatf orientation = ScriptEngine::toQuatf(L, 3);
    OVR::Vector4f color = ScriptEngine::toVec4fFromColor(L, 4);

    NewObjectEvent* event = eventBus.writeNext<NewObjectEvent>();
    *event = NewObjectEvent{object, &model, position, orientation, color};

    lua_pop(L, 4);
    lua_pushinteger(L, object);

    return 1;
}

int engine_updateObject(lua_State *L) {
    EventBus& eventBus = ScriptEngine::getFromClosure<EventBus>(L, 1);

    Object object = static_cast<Object>(luaL_checkinteger(L, 1));
    OVR::Vector3f position = ScriptEngine::toVec3f(L, 2);
    OVR::Quatf orientation = ScriptEngine::toQuatf(L, 3);
    OVR::Vector4f color = ScriptEngine::toVec4fFromColor(L, 4);

    UpdateObjectEvent* event = eventBus.writeNext<UpdateObjectEvent>();
    *event = {object, position, orientation, color};

    return 0;
}

int engine_quit(lua_State* L) {
    ScriptEngine &scriptEngine = ScriptEngine::getFromClosure<ScriptEngine>(L, 1);
    scriptEngine.quit = true;

    return 0;
}

int engine_updateModel(lua_State* L) {
    EventBus& eventBus = ScriptEngine::getFromClosure<EventBus>(L, 1);

    Object object = static_cast<Object>(luaL_checkinteger(L, 1));
    ModelData& model = ScriptEngine::getFromStack<ModelData>(L, 2);

    ChangeModelEvent* event = eventBus.writeNext<ChangeModelEvent>();
    *event = ChangeModelEvent{object, &model};

    return 0;
}

void ScriptEngine::update(engine::Input &input) {

    lua_getglobal(L, "update");
    if(!lua_isfunction(L, 1)) {
        throw runtime_error("global(input) could not be found");
    }
    //Stack: update
    lua_newtable(L);
    //Stack: engine input


    lua_newtable(L);
    //Stack: engine input buttonsPressed
    int inputIndex = 1;

    for(auto button: input.pressedButtons) {
        lua_pushinteger(L, inputIndex++);
        lua_pushinteger(L, button);
        lua_settable(L, 3);
    }
    lua_setfield(L, 2, "pressedButtons");
    //Stack: engine input


    lua_newtable(L);
    //Stack: engine input axes
    for(auto axis: input.axes) {
        lua_pushinteger(L, axis.first);
        lua_pushnumber(L, axis.second);
        lua_settable(L, 3);
    }
    lua_setfield(L, 2, "axes");
    //Stack: engine input

    int result = lua_pcall(L, 1, 0, 0);
    if(result != LUA_OK) {
        throw runtime_error("Could not execute update(input):\n" + string(lua_tostring(L, 1)));
    }

    CommitEvent *pEvent = eventBus.writeNext<CommitEvent>();
    *pEvent = CommitEvent();

    eventBus.commitWrites();
}

void lua::ScriptEngine::init() {
    int result;

    L = luaL_newstate();
    luaL_openlibs(L);

    lua_newtable(L);
    //Stack: engine
    addFunctionToTable("getBrick", engine_getBrick, &brickCache);
    addFunctionToTable("newObject", engine_newObject, this);
    addFunctionToTable("updateObject", engine_updateObject, &eventBus);
    addFunctionToTable("updateModel", engine_updateModel, &eventBus);
    addFunctionToTable("quit", engine_quit, this);

    //Stack: engine
    lua_setglobal(L, "engine");

    result = luaL_dofile(L, "scripts/game.lua");
    if(result != LUA_OK) {
        throw runtime_error("Could not execute game.lua:\n" + string(lua_tostring(L, 1)));
    }

    lua_getglobal(L, "init");
    if(!lua_isfunction(L, 1)) {
        throw runtime_error("init() could not be found");
    }
    //Stack: init
    result = lua_pcall(L, 0, 0, 0);
    if(result != LUA_OK) {
        throw runtime_error("Could not execute init():\n" + string(lua_tostring(L, 1)));
    }
}
void lua::ScriptEngine::addFunctionToTable(std::string name, lua_CFunction function, void* closure) {
    //Stack: table
    lua_pushlightuserdata(L, closure);
    lua_pushcclosure(L, function, 1);
    lua_setfield(L, -2, name.c_str());
}


lua::ScriptEngine::~ScriptEngine() {
    if(L) {
        lua_close(L);
    }
}





