/**********************************************\
|*        (c) 2013-2014 GiR-Zippo             *|
|*        gir_zippo@hellokitty.com            *|
\**********************************************/

#include "scriptmgr.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "scriptbuilder/scriptbuilder.h"
#include "scriptstdstring/scriptstdstring.h"
#include "scriptarray/scriptarray.h"
#include "scriptmath/scriptmath.h"
#include "scriptmath/scriptmathcomplex.h"
#include "weakref/weakref.h"

#include "Launcher.hpp"
#include "ScriptObject.hpp"

using namespace std;

CScriptMgr::CScriptMgr()
{
    engine           = 0;
    hasCompileErrors = false;
}

CScriptMgr::~CScriptMgr()
{
    for( unsigned int n = 0; n < controllers.size(); n++ )
        delete controllers[n];

    for( unsigned int n = 0; n < contexts.size(); n++ )
        contexts[n]->Release();

    if( engine )
        engine->Release();
}

int CScriptMgr::Init(Launcher *launcher)
{
    int r;

    engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

    // Set the message callback to print the human readable messages that the engine gives in case of errors
    r = engine->SetMessageCallback(asMETHOD(CScriptMgr, MessageCallback), this, asCALL_THISCALL);
    assert( r >= 0 );

    // Register the string typehttp://de.wikipedia.org/wiki/Cron
    RegisterStdString(engine);
    RegisterScriptArray(engine, false);
    RegisterScriptMath(engine);
    RegisterScriptMathComplex(engine);
    RegisterStdStringUtils(engine);
    
    // Register the generic handle type, called 'ref' in the script
    RegisterScriptHandle(engine);

    // Register the weak ref template type
    RegisterScriptWeakRef(engine);

    r = engine->RegisterObjectType("CScript", 0, asOBJ_REF); assert( r >= 0 );
    r = engine->RegisterObjectBehaviour("CScript", asBEHAVE_ADDREF, "void f()", asMETHOD(ScriptObject, AddRef), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectBehaviour("CScript", asBEHAVE_RELEASE, "void f()", asMETHOD(ScriptObject, Release), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void print(string &in)", asMETHOD(ScriptObject, PrintString), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void print(uint64 data)", asMETHOD(ScriptObject, PrintUInt), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void print(float data)", asMETHOD(ScriptObject, PrintFloat), asCALL_THISCALL); assert( r >= 0 );

    ///- Objects
    r = engine->RegisterObjectMethod("CScript", "int CreateObject(float x, float y, float z, float sx, float sy, uint16 tex, bool depth, bool ortho, bool clickable)", asMETHOD(ScriptObject, CreateObject), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "int CreateLoadingObject(float x, float y, float z, float sx, float sy, uint16 texture, bool depth)", asMETHOD(ScriptObject, CreateLoadingObject), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "int CreateSprite(float x, float y, float z, string tileimage, uint32 tileX, uint32 tileY)", asMETHOD(ScriptObject, CreateSprite), asCALL_THISCALL); assert( r >= 0 );

    ///- Textausgabe
    r = engine->RegisterObjectMethod("CScript", "int CreateTextObject(float x, float y, float z, bool depth, float height, string text, string font)", asMETHOD(ScriptObject, CreateTextObject), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "int CreateTextbox(float x, float y, float z, bool depth, float height, string text, string font)", asMETHOD(ScriptObject, CreateTextbox), asCALL_THISCALL); assert( r >= 0 );

    ///- 3DModelle
    r = engine->RegisterObjectMethod("CScript", "int CreateMS3DModel(float x, float y, float z, string path, string filename)", asMETHOD(ScriptObject, CreateMS3DModel), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "int CreateWavefrontModel(float x, float y, float z, string path, string filename)", asMETHOD(ScriptObject, CreateWavefrontModel), asCALL_THISCALL); assert( r >= 0 );

    ///- TileMapping
    r = engine->RegisterObjectMethod("CScript", "int CreateTileMap(float x, float y, float z, string filename)", asMETHOD(ScriptObject, CreateTileMap), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void SetActiveMap(uint16 id)", asMETHOD(ScriptObject, SetActiveMap), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "int GetActiveMap(uint16 id)", asMETHOD(ScriptObject, GetActiveMap), asCALL_THISCALL); assert( r >= 0 );

    ///- WorldObjekte
    r = engine->RegisterObjectMethod("CScript", "int CreateWorldObject(float x, float y, float z, string Scriptname)", asMETHOD(ScriptObject, CreateWorldObject), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "int CreateWorldObjectCreature(float x, float y, float z, string Scriptname)", asMETHOD(ScriptObject, CreateWorldObjectCreature), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "int CreateWorldObjectPlayer(float x, float y, float z, string tilesetname)", asMETHOD(ScriptObject, CreateWorldObjectPlayer), asCALL_THISCALL); assert( r >= 0 );

    ///-ObjectMgr
    r = engine->RegisterObjectMethod("CScript", "int DeleteObject(uint16 id)", asMETHOD(ScriptObject, DeleteObject), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void AddObjectToUpdate(uint16 pos, uint16 id)", asMETHOD(ScriptObject, AddObjectToUpdate), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void FlushUpdateList()", asMETHOD(ScriptObject, FlushUpdateList), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void RemoveObjectFromUpdateByPosition(uint16 pos)", asMETHOD(ScriptObject, RemoveObjectFromUpdateByPosition), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void RemoveObjectFromUpdate(uint16 id)", asMETHOD(ScriptObject, RemoveObjectFromUpdate), asCALL_THISCALL); assert( r >= 0 );

    ///- Object Manipulation
    r = engine->RegisterObjectMethod("CScript", "void SetObjectRGBA(uint32 id, float r, float g, float b, float a)", asMETHOD(ScriptObject, SetObjectRGBA), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void MoveObjectXY(uint32 id, float x, float y)", asMETHOD(ScriptObject, MoveObjectXY), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void MoveObjectXYZ(uint32 id, float x, float y, float z)", asMETHOD(ScriptObject, MoveObjectXYZ), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void SetRotationXYZ(uint32 id, float x, float y, float z)", asMETHOD(ScriptObject, SetRotationXYZ), asCALL_THISCALL); assert( r >= 0 );   
    r = engine->RegisterObjectMethod("CScript", "void RotateObjectX(uint32 id, float x)", asMETHOD(ScriptObject, RotateObjectX), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void RotateObjectY(uint32 id, float y)", asMETHOD(ScriptObject, RotateObjectY), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void RotateObjectZ(uint32 id, float z)", asMETHOD(ScriptObject, RotateObjectZ), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void SetObjectSizeX(uint32 id, float x)",   asMETHOD(ScriptObject, ObjectSizeX), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void SetObjectSizeY(uint32 id, float y)",   asMETHOD(ScriptObject, ObjectSizeY), asCALL_THISCALL); assert( r >= 0 );

    r = engine->RegisterObjectMethod("CScript", "float GetObjectPosX(uint32 id)",   asMETHOD(ScriptObject, GetPositionX), asCALL_THISCALL); assert( r >= 0 );    
    r = engine->RegisterObjectMethod("CScript", "float GetObjectPosY(uint32 id)",   asMETHOD(ScriptObject, GetPositionY), asCALL_THISCALL); assert( r >= 0 );    
     
    ///- TextObject
    r = engine->RegisterObjectMethod("CScript", "void SetNewText(uint32 id, string Text)", asMETHOD(ScriptObject, SetNewText), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void SetTextCenterX(uint32 id, bool arg)", asMETHOD(ScriptObject, SetTextCenterX), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void SetTextCenterY(uint32 id, bool arg)", asMETHOD(ScriptObject, SetTextCenterY), asCALL_THISCALL); assert( r >= 0 );

    ///- TextboxZeugs
    r = engine->RegisterObjectMethod("CScript", "int SetTextboxBackgroundTexture(uint32 id, string texture)", asMETHOD(ScriptObject, SetTextboxBackgroundTexture), asCALL_THISCALL); assert( r >= 0 );

    ///- Sorted
    ///- SoundZeugs
    r = engine->RegisterObjectMethod("CScript", "uint32 LoadSound(string file)", asMETHOD(ScriptObject, LoadSound), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void PlaySound(uint32 id)", asMETHOD(ScriptObject, PlaySound), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void StopSound(uint32 id)", asMETHOD(ScriptObject, StopSound), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void SetSoundVolume(uint32 id, float vol)", asMETHOD(ScriptObject, SetSoundVolume), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "uint32 GetSoundIdByName(string file)", asMETHOD(ScriptObject, GetSoundIdByName), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void UnloadSound(uint32 id)", asMETHOD(ScriptObject, UnloadSound), asCALL_THISCALL); assert( r >= 0 );

    ///- Textures
    r = engine->RegisterObjectMethod("CScript", "int Load2DMidmap(string &in)", asMETHOD(ScriptObject, Load2DMidmap), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "void DeleteTexture(uint16 texture)", asMETHOD(ScriptObject, DeleteTexture), asCALL_THISCALL); assert( r >= 0 );

    ///- Others
    r = engine->RegisterObjectMethod("CScript", "int Rand()", asMETHOD(ScriptObject, Rand), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("CScript", "int Rand(int lo, int hi)", asMETHOD(ScriptObject, RandAB), asCALL_THISCALL); assert( r >= 0 );

    r = engine->RegisterObjectMethod("CScript", "void SetRealmlist(string wow, string realmlist)", asMETHOD(ScriptObject, SetRealmlist), asCALL_THISCALL); assert( r >= 0 );

    // Register the game object. The scripts cannot create these directly, so there is no factory function.
    r = engine->RegisterObjectType("Launcher", 0, asOBJ_REF | asOBJ_NOHANDLE); assert( r >= 0 );
    r = engine->RegisterObjectType("Memory", 0, asOBJ_REF | asOBJ_NOHANDLE); assert( r >= 0 );

    // Register the game manager's methods
    r = engine->RegisterGlobalProperty("Launcher launcher", launcher); assert( r >= 0 );
    r = engine->RegisterGlobalProperty("Memory memory", launcher); assert( r >= 0 );

    r = engine->RegisterObjectMethod("Launcher", "void LoadScript(string name)", asMETHOD(Launcher, LoadScript), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("Launcher", "void CheckOrDownload(string name, bool blocking)", asMETHOD(Launcher, CheckFileOrDownload), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("Launcher", "void DownloadFile(string name, bool blocking)", asMETHOD(Launcher, LoadFileFromServer), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("Launcher", "bool IsLastFile(string name)", asMETHOD(Launcher, IsLastFile), asCALL_THISCALL); assert( r >= 0 );

    ///- bare
    r = engine->RegisterObjectMethod("Launcher", "void mkdir(string dir)", asMETHOD(Launcher, mkdir), asCALL_THISCALL); assert( r >= 0 );

    ///- Background Download
    r = engine->RegisterObjectMethod("Launcher", "void BackgroundDownload(string ip, uint16 port, string name)", asMETHOD(Launcher, Download), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("Launcher", "uint64 BackgroundDownload_GetActualBlock()" , asMETHOD(Launcher, GetActualBlock), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("Launcher", "uint64 BackgroundDownload_GetTotalBlocks()" , asMETHOD(Launcher, GetTotalBlocks), asCALL_THISCALL); assert( r >= 0 );
 
    ///- Kamerasystem
    r = engine->RegisterObjectMethod("Launcher", "void SetViewXYZ(float x, float y, float z)" , asMETHOD(Launcher, SetViewXYZ), asCALL_THISCALL); assert( r >= 0 );
    
    r = engine->RegisterObjectMethod("Memory", "void Set(string, string)" , asMETHOD(Launcher, SetVarCon), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("Memory", "string GetString(string)" , asMETHOD(Launcher, GetStringFromVarCon), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("Memory", "float GetFloat(string)" , asMETHOD(Launcher, GetFloatFromVarCon), asCALL_THISCALL); assert( r >= 0 );
    r = engine->RegisterObjectMethod("Memory", "float GetUint32(string)" , asMETHOD(Launcher, GetUint32FromVarCon), asCALL_THISCALL); assert( r >= 0 );
    // The game engine will determine the class that represents the controller
    // by checking if the class implements the IController interface. No methods
    // are registered for this interface, as the script shouldn't be required to
    // implement the methods. This will allow the game engine to avoid calling
    // methods that doesn't do anything, thus improving performance.
    r = engine->RegisterInterface("IController"); assert( r >= 0 );

    return 0;
}

void CScriptMgr::MessageCallback(const asSMessageInfo &msg)
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

void CScriptMgr::RemoveScript(const string &script)
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


CScriptMgr::SController *CScriptMgr::GetControllerScript(const string &script)
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
    string s = string(type->GetName()) + "@ " + string(type->GetName()) + "(CScript @)";
    ctrl->factoryFunc = type->GetFactoryByDecl(s.c_str());
    if( ctrl->factoryFunc == 0 )
    {
        cout << "Couldn't find the appropriate factory for the type '" << script << "'" << endl;
        controllers.pop_back();
        delete ctrl;
        return 0;
    }

    // Find the optional event handlers
    ctrl->onInitGL          = type->GetMethodByDecl("void OnInitGL()");
    ctrl->onDrawGL          = type->GetMethodByDecl("void OnDrawGL()");
    ctrl->onTick            = type->GetMethodByDecl("void OnTick()");
    ctrl->onMouseClick      = type->GetMethodByDecl("void OnMouseClick(float x, float y, float z, float button, float action, uint16 object)");
    ctrl->onMouseMove       = type->GetMethodByDecl("void OnMouseMove(float x, float y)");
    ctrl->onKeyPressed      = type->GetMethodByDecl("void onKeyPressed(uint8 key)");
    ctrl->onThinkMethod     = type->GetMethodByDecl("void OnThink()");
    ctrl->onMessageMethod   = type->GetMethodByDecl("void OnMessage(ref @msg, const CScript @sender)");

    // Add the cache as user data to the type for quick access
    type->SetUserData(ctrl);

    return ctrl;
}

asIScriptObject *CScriptMgr::CreateController(const string &script, ScriptObject *gameObj)
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

void CScriptMgr::CallOnInitGL(asIScriptObject *object)
{
    // Find the cached onThink method id
    SController *ctrl = reinterpret_cast<SController*>(object->GetObjectType()->GetUserData());

    // Call the method using the shared context
    if( ctrl->onInitGL != 0 )
    {
        asIScriptContext *ctx = PrepareContextFromPool(ctrl->onInitGL);
        ctx->SetObject(object);
        ExecuteCall(ctx);
        ReturnContextToPool(ctx);
    }
}

void CScriptMgr::CallOnDrawGL(asIScriptObject *object)
{
    // Find the cached onThink method id
    SController *ctrl = reinterpret_cast<SController*>(object->GetObjectType()->GetUserData());

    // Call the method using the shared context
    if( ctrl->onDrawGL != 0 )
    {
        asIScriptContext *ctx = PrepareContextFromPool(ctrl->onDrawGL);
        ctx->SetObject(object);
        ExecuteCall(ctx);
        ReturnContextToPool(ctx);
    }
}

void CScriptMgr::CallOnTick(asIScriptObject* object)
{
    // Find the cached onThink method id
    SController *ctrl = reinterpret_cast<SController*>(object->GetObjectType()->GetUserData());

    // Call the method using the shared context
    if( ctrl->onTick != 0 )
    {
        asIScriptContext *ctx = PrepareContextFromPool(ctrl->onTick);
        ctx->SetObject(object);
        ExecuteCall(ctx);
        ReturnContextToPool(ctx);
    }
}

void CScriptMgr::CallOnMouseClick(asIScriptObject *object, float x, float y, float z, int button, int action, int clickedobj)
{
    // Find the cached onThink method id
    SController *ctrl = reinterpret_cast<SController*>(object->GetObjectType()->GetUserData());

    // Call the method using the shared context
    if( ctrl->onMouseClick != 0 )
    {
        asIScriptContext *ctx = PrepareContextFromPool(ctrl->onMouseClick);
        ctx->SetObject(object);
        ctx->SetArgFloat(0, x);
        ctx->SetArgFloat(1, y);
        ctx->SetArgFloat(2, z);
        ctx->SetArgFloat(3, button);
        ctx->SetArgFloat(4, action);
        ctx->SetArgWord(5, clickedobj);
        ExecuteCall(ctx);
        ReturnContextToPool(ctx);
    }
}

void CScriptMgr::CallOnMouseMove(asIScriptObject *object, float x, float y)
{
    // Find the cached onThink method id
    SController *ctrl = reinterpret_cast<SController*>(object->GetObjectType()->GetUserData());

    // Call the method using the shared context
    if( ctrl->onMouseMove != 0 )
    {
        asIScriptContext *ctx = PrepareContextFromPool(ctrl->onMouseMove);
        ctx->SetObject(object);
        ctx->SetArgFloat(0, x);
        ctx->SetArgFloat(1, y);
        ExecuteCall(ctx);
        ReturnContextToPool(ctx);
    }
}

void CScriptMgr::CallOnKeyPressed(asIScriptObject* object, uint8 key)
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

void CScriptMgr::CallOnThink(asIScriptObject *object)
{
    // Find the cached onThink method id
    SController *ctrl = reinterpret_cast<SController*>(object->GetObjectType()->GetUserData());

    // Call the method using the shared context
    if( ctrl->onThinkMethod != 0 )
    {
        asIScriptContext *ctx = PrepareContextFromPool(ctrl->onThinkMethod);
        ctx->SetObject(object);
        ExecuteCall(ctx);
        ReturnContextToPool(ctx);
    }
}

void CScriptMgr::CallOnMessage(asIScriptObject *object, CScriptHandle &msg, ScriptObject *caller)
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

int CScriptMgr::ExecuteCall(asIScriptContext *ctx)
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

asIScriptContext *CScriptMgr::PrepareContextFromPool(asIScriptFunction *func)
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

void CScriptMgr::ReturnContextToPool(asIScriptContext *ctx)
{
    contexts.push_back(ctx);

    // Unprepare the context to free any objects that might be held
    // as we don't know when the context will be used again.
    ctx->Unprepare();
}


