/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "WorldObjectScript.hpp"
#include <iostream>  // cout
#include <stdio.h>  // fopen, fclose
#include <string.h> // strcmp
#include <assert.h>

#include "scriptbuilder/scriptbuilder.h"
#include "scriptstdstring/scriptstdstring.h"
#include "scriptarray/scriptarray.h"
#include "scriptmath/scriptmath.h"
#include "scriptmath/scriptmathcomplex.h"
#include "weakref/weakref.h"


#include "Launcher.hpp"
#include "WorldObject.hpp"

using namespace std;

WorldObjectScript::WorldObjectScript()
{
    engine           = 0;
    hasCompileErrors = false;
}

WorldObjectScript::~WorldObjectScript()
{
    for( unsigned int n = 0; n < controllers.size(); n++ )
        delete controllers[n];

    for( unsigned int n = 0; n < contexts.size(); n++ )
        contexts[n]->Release();

    if( engine )
        engine->Release();
}

int WorldObjectScript::Init()
{
    int r;

    engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

    // Set the message callback to print the human readable messages that the engine gives in case of errors
    r = engine->SetMessageCallback(asMETHOD(WorldObjectScript, MessageCallback), this, asCALL_THISCALL);
    assert( r >= 0 );

    // Register the string type
    RegisterStdString(engine);
    RegisterScriptArray(engine, false);
    RegisterScriptMath(engine);
    RegisterScriptMathComplex(engine);
    RegisterStdStringUtils(engine);
        
    // Register the generic handle type, called 'ref' in the script
    RegisterScriptHandle(engine);

    // Register the weak ref template type
    RegisterScriptWeakRef(engine);

    r = engine->RegisterObjectType("WorldObject", 0, asOBJ_REF); assert( r >= 0 );
    r = engine->RegisterObjectBehaviour("WorldObject", asBEHAVE_ADDREF, "void f()", asMETHOD(WorldObject, AddRef), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectBehaviour("WorldObject", asBEHAVE_RELEASE, "void f()", asMETHOD(WorldObject, Release), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("WorldObject", "void print(string &in)", asMETHOD(WorldObject, PrintString), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("WorldObject", "void print(uint64 data)", asMETHOD(WorldObject, PrintUint64), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("WorldObject", "void ActivateKeyInput()", asMETHOD(WorldObject, ActivateKeyInput), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("WorldObject", "void DeactivateKeyInput()", asMETHOD(WorldObject, DeactivateKeyInput), asCALL_THISCALL); assert( r >= 0 );
    
    ///-Texturzeugs
    r = engine->RegisterObjectMethod("WorldObject", "void AsSprite(string filename, uint32 x, uint32 y)", asMETHOD(WorldObject, AsSprite), asCALL_THISCALL); assert( r >= 0 );

    ///-Kollisionseinstellungen
    r = engine->RegisterObjectMethod("WorldObject", "void SetCollisionWithMap(bool i)", asMETHOD(WorldObject, SetCollisionWithMap), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("WorldObject", "void SetCollisionWithObjects(bool i)", asMETHOD(WorldObject, SetCollisionWithObjects), asCALL_THISCALL); assert( r >= 0 );

    ///-Animations- und Movementzeugs
    r = engine->RegisterObjectMethod("WorldObject", "void SetMovementSpeed(float speed)", asMETHOD(WorldObject, SetMovementSpeed), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("WorldObject", "void SetIdleAnimation(bool left, bool right, bool up, bool down)", asMETHOD(WorldObject, SetIdleAnimation), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("WorldObject", "bool MoveUp()", asMETHOD(WorldObject, MoveUp), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("WorldObject", "bool MoveDown()", asMETHOD(WorldObject, MoveDown), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("WorldObject", "bool MoveLeft()", asMETHOD(WorldObject, MoveLeft), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("WorldObject", "bool MoveRight()", asMETHOD(WorldObject, MoveRight), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("WorldObject", "bool Jump()", asMETHOD(WorldObject, Jump), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("WorldObject", "void MoveToPosition(float x, float y, float z)", asMETHOD(WorldObject, MoveToPosition), asCALL_THISCALL); assert( r >= 0 );

    ///-Subobjekte
    r = engine->RegisterObjectMethod("WorldObject", "void DeleteSubObject(uint8 id)", asMETHOD(WorldObject, DeleteObject), asCALL_THISCALL); assert( r >= 0 );
    ///-Textausgabe
    r = engine->RegisterObjectMethod("WorldObject", "int WriteText(float x, float y, float z, bool depth, float height, string text, string font)", asMETHOD(WorldObject, WriteText), asCALL_THISCALL); assert( r >= 0 );
    ///-Interop
    r = engine->RegisterObjectMethod("WorldObject", "void SetUint32(uint16 id, uint16 index, uint32 data)", asMETHOD(WorldObject, SetUint32), asCALL_THISCALL); assert( r >= 0 );
    // Register the game object. The scripts cannot create these directly, so there is no factory function.

    // The game engine will determine the class that represents the controller
    // by checking if the class implements the IController interface. No methods
    // are registered for this interface, as the script shouldn't be required to
    // implement the methods. This will allow the game engine to avoid calling
    // methods that doesn't do anything, thus improving performance.
    r = engine->RegisterInterface("IController"); assert( r >= 0 );

    return 0;
}

void WorldObjectScript::MessageCallback(const asSMessageInfo &msg)
{
    const char *type = "ERR ";
    if( msg.type == asMSGTYPE_WARNING )
        type = "WARN";
    else if( msg.type == asMSGTYPE_INFORMATION )
        type = "INFO";

    cout << msg.section << " (" << msg.row << ", " << msg.col << ") : " << type << " : " << msg.message << endl;

    if( msg.type == asMSGTYPE_ERROR )
        hasCompileErrors = true;
}

void WorldObjectScript::RemoveScript(const string &script)
{
    // Find the cached controller
    for( unsigned int n = 0; n < controllers.size(); n++ )
    {
        if( controllers[n]->module == script )
        {
            controllers.erase(controllers.begin() +n);
            break;
        }
    }
    engine->DiscardModule(script.c_str());
}


WorldObjectScript::SController *WorldObjectScript::GetControllerScript(const string &script)
{
    int r;

    // Find the cached controller
    for( unsigned int n = 0; n < controllers.size(); n++ )
    {
        if( controllers[n]->module == script )
            return controllers[n];
    }

    // No controller, check if the script has already been loaded
    asIScriptModule *mod = engine->GetModule(script.c_str(), asGM_ONLY_IF_EXISTS);
    if( mod )
    {
        // We've already attempted loading the script before, but there is no controller
        return 0;
    }

    // Compile the script into the module
    CScriptBuilder builder;
    r = builder.StartNewModule(engine, script.c_str());
    if( r < 0 )
        return 0;

    // If the script file doesn't exist, then there is no script controller for this type
    FILE *f;
    if( (f = fopen((script + ".as").c_str(), "r")) == 0 )
        return 0;
    fclose(f);

    // Let the builder load the script, and do the necessary pre-processing (include files, etc)
    r = builder.AddSectionFromFile((script + ".as").c_str());
    if( r < 0 )
        return 0;

    r = builder.BuildModule();
    if( r < 0 )
        return 0;

    // Cache the functions and methods that will be used
    SController *ctrl = new SController;
    controllers.push_back(ctrl);
    ctrl->module = script;

    // Find the class that implements the IController interface
    mod = engine->GetModule(script.c_str(), asGM_ONLY_IF_EXISTS);
    asIObjectType *type = 0;
    int tc = mod->GetObjectTypeCount();
    for( int n = 0; n < tc; n++ )
    {
        bool found = false;
        type = mod->GetObjectTypeByIndex(n);
        int ic = type->GetInterfaceCount();
        for( int i = 0; i < ic; i++ )
        {
            if( strcmp(type->GetInterface(i)->GetName(), "IController") == 0 )
            {
                found = true;
                break;
            }
        }

        if( found == true )
        {
            ctrl->type = type;
            break;
        }
    }

    if( ctrl->type == 0 )
    {
        cout << "Couldn't find the controller class for the type '" << script << "'" << endl;
        controllers.pop_back();
        delete ctrl;
        return 0;
    }

    // Find the factory function
    // The game engine will pass in the owning CGameObj to the controller for storage
    string s = string(type->GetName()) + "@ " + string(type->GetName()) + "(WorldObject @)";
    ctrl->factoryFunc = type->GetFactoryByDecl(s.c_str());
    if( ctrl->factoryFunc == 0 )
    {
        cout << "Couldn't find the appropriate factory for the type '" << script << "'" << endl;
        controllers.pop_back();
        delete ctrl;
        return 0;
    }

    // Find the optional event handlers
    ctrl->onInit            = type->GetMethodByDecl("void OnInit()");
    ctrl->PostDraw          = type->GetMethodByDecl("void PostDraw()");
    ctrl->Update            = type->GetMethodByDecl("void Update(uint32 diff)");
    ctrl->onKeyPressed      = type->GetMethodByDecl("void onKeyPressed(uint8 key)");
    ctrl->onSetData         = type->GetMethodByDecl("void onSetData(uint16 index, uint32 data)");
    ctrl->onWaypointReached = type->GetMethodByDecl("void onWaypointReached(uint32 waypoint)");
    ctrl->onMessageMethod   = type->GetMethodByDecl("void OnMessage(ref @msg, const WorldObject @sender)");

    // Add the cache as user data to the type for quick access
    type->SetUserData(ctrl);

    return ctrl;
}

asIScriptObject *WorldObjectScript::CreateController(const string &script, WorldObject *gameObj)
{
    int r;
    asIScriptObject *obj = 0;

    SController *ctrl = GetControllerScript(script);
    if( ctrl == 0 ) return 0;

    // Create the object using the factory function
    asIScriptContext *ctx = PrepareContextFromPool(ctrl->factoryFunc);

    // Pass the object pointer to the script function. With this call the
    // context will automatically increase the reference count for the object.
    ctx->SetArgObject(0, gameObj);

    // Make the call and take care of any errors that may happen
    r = ExecuteCall(ctx);
    if( r == asEXECUTION_FINISHED )
    {
        // Get the newly created object
        obj = *((asIScriptObject**)ctx->GetAddressOfReturnValue());

        // Since a reference will be kept to this object
        // it is necessary to increase the ref count
        obj->AddRef();
    }

    // Return the context to the pool so it can be reused
    ReturnContextToPool(ctx);

    return obj;
}

void WorldObjectScript::CallOnInit(asIScriptObject *object)
{
    // Find the cached onThink method id
    SController *ctrl = reinterpret_cast<SController*>(object->GetObjectType()->GetUserData());

    // Call the method using the shared context
    if( ctrl->onInit != 0 )
    {
        asIScriptContext *ctx = PrepareContextFromPool(ctrl->onInit);
        ctx->SetObject(object);
        ExecuteCall(ctx);
        ReturnContextToPool(ctx);
    }
}

void WorldObjectScript::CallPostDraw(asIScriptObject* object)
{
    // Find the cached onThink method id
    SController *ctrl = reinterpret_cast<SController*>(object->GetObjectType()->GetUserData());

    // Call the method using the shared context
    if( ctrl->PostDraw != 0 )
    {
        asIScriptContext *ctx = PrepareContextFromPool(ctrl->PostDraw);
        ctx->SetObject(object);
        ExecuteCall(ctx);
        ReturnContextToPool(ctx);
    }
}

void WorldObjectScript::CallOnWaypointReached(asIScriptObject* object, uint32 waypoint)
{
    // Find the cached onThink method id
    SController *ctrl = reinterpret_cast<SController*>(object->GetObjectType()->GetUserData());

    // Call the method using the shared context
    if( ctrl->onWaypointReached != 0 )
    {
        asIScriptContext *ctx = PrepareContextFromPool(ctrl->onWaypointReached);
        ctx->SetObject(object);
        ctx->SetArgDWord(0, waypoint);
        ExecuteCall(ctx);
        ReturnContextToPool(ctx);
    }
}

void WorldObjectScript::CallUpdate(asIScriptObject *object, uint32 diff)
{
    // Find the cached onThink method id
    SController *ctrl = reinterpret_cast<SController*>(object->GetObjectType()->GetUserData());

    // Call the method using the shared context
    if( ctrl->Update != 0 )
    {
        asIScriptContext *ctx = PrepareContextFromPool(ctrl->Update);
        ctx->SetObject(object);
        ctx->SetArgDWord(0, diff);
        ExecuteCall(ctx);
        ReturnContextToPool(ctx);
    }
}

void WorldObjectScript::CallOnKeyPressed(asIScriptObject* object, uint8 key)
{
    // Find the cached onThink method id
    SController *ctrl = reinterpret_cast<SController*>(object->GetObjectType()->GetUserData());

    // Call the method using the shared context
    if( ctrl->onKeyPressed != 0 )
    {
        asIScriptContext *ctx = PrepareContextFromPool(ctrl->onKeyPressed);
        ctx->SetObject(object);
        ctx->SetArgByte(0, key);
        ExecuteCall(ctx);
        ReturnContextToPool(ctx);
    }
}

void WorldObjectScript::CallOnSetData(asIScriptObject *object, uint16 index, uint32 data)
{
    // Find the cached onThink method id
    SController *ctrl = reinterpret_cast<SController*>(object->GetObjectType()->GetUserData());

    // Call the method using the shared context
    if( ctrl->onSetData != 0 )
    {
        asIScriptContext *ctx = PrepareContextFromPool(ctrl->onSetData);
        ctx->SetObject(object);
        ctx->SetArgWord(0, index);
        ctx->SetArgDWord(1, data);
        ExecuteCall(ctx);
        ReturnContextToPool(ctx);
    }
}

void WorldObjectScript::CallOnMessage(asIScriptObject *object, CScriptHandle &msg, WorldObject *caller)
{
    // Find the cached onMessage method id
    SController *ctrl = reinterpret_cast<SController*>(object->GetObjectType()->GetUserData());

    // Call the method using the shared context
    if( ctrl->onMessageMethod != 0 )
    {
        asIScriptContext *ctx = PrepareContextFromPool(ctrl->onMessageMethod);
        ctx->SetObject(object);
        ctx->SetArgObject(0, &msg);
        ctx->SetArgObject(1, caller);
        ExecuteCall(ctx);
        ReturnContextToPool(ctx);
    }
}

int WorldObjectScript::ExecuteCall(asIScriptContext *ctx)
{
    int r = ctx->Execute();
    if( r != asEXECUTION_FINISHED )
    {
        if( r == asEXECUTION_EXCEPTION )
        {
            cout << "Exception: " << ctx->GetExceptionString() << endl;
            cout << "Function: " << ctx->GetExceptionFunction()->GetDeclaration() << endl;
            cout << "Line: " << ctx->GetExceptionLineNumber() << endl;

            // It is possible to print more information about the location of the
            // exception, for example the call stack, values of variables, etc if
            // that is of interest.
        }
    }

    return r;
}

asIScriptContext *WorldObjectScript::PrepareContextFromPool(asIScriptFunction *func)
{
    asIScriptContext *ctx = 0;
    if( contexts.size() )
    {
        ctx = *contexts.rbegin();
        contexts.pop_back();
    }
    else
        ctx = engine->CreateContext();

    int r = ctx->Prepare(func);
    assert( r >= 0 );

    return ctx;
}

void WorldObjectScript::ReturnContextToPool(asIScriptContext *ctx)
{
    contexts.push_back(ctx);

    // Unprepare the context to free any objects that might be held
    // as we don't know when the context will be used again.
    ctx->Unprepare();
}

