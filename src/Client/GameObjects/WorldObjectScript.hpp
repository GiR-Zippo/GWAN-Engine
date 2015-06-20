/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef __WORLDOBJECTSCRIPT_HPP__
#define __WORLDOBJECTSCRIPT_HPP__

#include "Common.h"
#include <string>
#include <vector>
#include <angelscript.h>
#include "scripthandle/scripthandle.h"

class Launcher;
class WorldObject;

class WorldObjectScript
{
public:
    WorldObjectScript();
    ~WorldObjectScript();

    int Init();
    void RemoveScript(const std::string &script);

    asIScriptObject *CreateController(const std::string &type, WorldObject *obj);
    void CallOnInit(asIScriptObject *object);
    void CallPostDraw(asIScriptObject *object);
    void CallUpdate(asIScriptObject *object, uint32 diff);
    void CallOnKeyPressed(asIScriptObject *object, uint8 key);
    void CallOnSetData(asIScriptObject *object, uint16 index, uint32 data);
    void CallOnWaypointReached(asIScriptObject* object, uint32 waypoint);
    void CallOnMessage(asIScriptObject *object, CScriptHandle &msg, WorldObject *caller);

    bool hasCompileErrors;

protected:
    void MessageCallback(const asSMessageInfo &msg);
    asIScriptContext *PrepareContextFromPool(asIScriptFunction *func);
    void ReturnContextToPool(asIScriptContext *ctx);
    int ExecuteCall(asIScriptContext *ctx);

    struct SController
    {
        SController() : type(0), factoryFunc(0), onInit(0), Update(0), onSetData(0), PostDraw(0), onKeyPressed(0), onWaypointReached(0), onMessageMethod(0) {}
        std::string        module;
        asIObjectType     *type;
        asIScriptFunction *factoryFunc;
        asIScriptFunction *onInit;
        asIScriptFunction *Update;
        asIScriptFunction *onKeyPressed;
        asIScriptFunction *onSetData;
        asIScriptFunction *PostDraw;
        asIScriptFunction *onWaypointReached;
        asIScriptFunction *onMessageMethod;
    };

    SController *GetControllerScript(const std::string &type);

    asIScriptEngine  *engine;

    // Our pool of script contexts. This is used to avoid allocating
    // the context objects all the time. The context objects are quite
    // heavy weight and should be shared between function calls.
    std::vector<asIScriptContext *> contexts;

    // This is the cache of function ids etc that we use to avoid having
    // to search for the function ids everytime we need to call a function.
    // The search is quite time consuming and should only be done once.
    std::vector<SController *> controllers;
};

extern WorldObjectScript *worldObjectScript;

#endif
