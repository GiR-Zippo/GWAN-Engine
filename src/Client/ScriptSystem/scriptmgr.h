/**********************************************\
|*        (c) 2013-2015 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#ifndef SCRIPTMGR_H
#define SCRIPTMGR_H

#include <string>
#include <vector>
#include <angelscript.h>
#include "scripthandle/scripthandle.h"
#include "Common.h"

class Launcher;
class ScriptObject;

class CScriptMgr
{
public:
    CScriptMgr();
    ~CScriptMgr();

    int Init(Launcher *launcher);
    void RemoveScript(const std::string &script);

    asIScriptObject *CreateController(const std::string &type, ScriptObject *obj);
    void CallOnInitGL(asIScriptObject *object); //If openGL inits
    void CallOnDrawGL(asIScriptObject *object); //If openGL refreshes the screen
    void CallOnTick(asIScriptObject *object); //If theres a tick
    void CallOnMouseClick(asIScriptObject *object, float x, float y, float z, int button, int action, int clickedobj = 0); //If mouse clicked
    void CallOnMouseMove(asIScriptObject *object, float x, float y);
    void CallOnKeyPressed(asIScriptObject *object, uint8 key);
    void CallOnResize(asIScriptObject *object, int w, int h);
    void CallOnThink(asIScriptObject *object);
    void CallOnMessage(asIScriptObject *object, CScriptHandle &msg, ScriptObject *caller);

    bool hasCompileErrors;

protected:
    void MessageCallback(const asSMessageInfo &msg);
    asIScriptContext *PrepareContextFromPool(asIScriptFunction *func);
    void ReturnContextToPool(asIScriptContext *ctx);
    int ExecuteCall(asIScriptContext *ctx);

    struct SController
    {
        SController() : type(0), factoryFunc(0), onInitGL(0), onDrawGL(0), onTick(0),
                        onMouseClick(0), onMouseMove(0), onKeyPressed(0), onResize(0), onThinkMethod(0), onMessageMethod(0) {}
        std::string        module;
        asIObjectType     *type;
        asIScriptFunction *factoryFunc;
        asIScriptFunction *onInitGL;
        asIScriptFunction *onDrawGL;
        asIScriptFunction *onTick;
        asIScriptFunction *onMouseClick;
        asIScriptFunction *onMouseMove;
        asIScriptFunction *onKeyPressed;
        asIScriptFunction *onResize;
        asIScriptFunction *onThinkMethod;
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

extern CScriptMgr *scriptMgr;

#endif
