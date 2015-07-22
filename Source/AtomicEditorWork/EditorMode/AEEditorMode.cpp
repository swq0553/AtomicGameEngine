
#include <Atomic/IO/Log.h>

#include <Atomic/IPC/IPC.h>
#include <Atomic/IPC/IPCEvents.h>
#include <Atomic/IPC/IPCBroker.h>

#include <ToolCore/ToolEnvironment.h>
#include <ToolCore/ToolSystem.h>
#include <ToolCore/Project/Project.h>

#include "AEEditorMode.h"

using namespace ToolCore;

namespace AtomicEditor
{

EditorMode::EditorMode(Context* context) :
    Object(context)
{    
    SubscribeToEvent(E_IPCWORKERSTART, HANDLER(EditorMode, HandleIPCWorkerStarted));
}

EditorMode::~EditorMode()
{

}

void EditorMode::HandleIPCWorkerStarted(StringHash eventType, VariantMap& eventData)
{
    VariantMap weventData;
    weventData[HelloFromBroker::P_HELLO] = "Hello";
    weventData[HelloFromBroker::P_LIFETHEUNIVERSEANDEVERYTHING] = 42;
    playerBroker_->PostMessage(E_IPCHELLOFROMBROKER, weventData);
}

void EditorMode::HandleIPCWorkerExit(StringHash eventType, VariantMap& eventData)
{
    //SendEvent(E_EDITORPLAYSTOP);
}


bool EditorMode::PlayProject()
{
    ToolEnvironment* env = GetSubsystem<ToolEnvironment>();
    ToolSystem* tsystem = GetSubsystem<ToolSystem>();

    const String& editorBinary = env->GetEditorBinary();

    Project* project = tsystem->GetProject();

    Vector<String> paths;
    paths.Push(env->GetCoreDataDir());
    paths.Push(env->GetPlayerDataDir());
    paths.Push(project->GetResourcePath());

    // fixme: this is for loading from cache
    paths.Push(project->GetProjectPath());
    paths.Push(project->GetProjectPath() + "Cache");

    String resourcePaths;
    resourcePaths.Join(paths, "!");

    Vector<String> vargs;

    String args = ToString("--player --project \"%s\"", AddTrailingSlash(project->GetProjectPath()).CString());

    vargs = args.Split(' ');

    String dump;
    dump.Join(vargs, " ");
    LOGINFOF("Launching Broker %s %s", editorBinary.CString(), dump.CString());

    IPC* ipc = GetSubsystem<IPC>();
    playerBroker_ = ipc->SpawnWorker(editorBinary, vargs);

    if (playerBroker_)
    {        
        SubscribeToEvent(playerBroker_, E_IPCWORKEREXIT, HANDLER(EditorMode, HandleIPCWorkerExit));

    }

    return playerBroker_.NotNull();

}

}
