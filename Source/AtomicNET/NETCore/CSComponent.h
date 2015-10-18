//
// Copyright (c) 2008-2014 the Urho3D project.
// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include <Atomic/Script/ScriptComponent.h>

#include "NETAssemblyFile.h"

namespace Atomic
{

enum CSComponentMethod
{
    CSComponentMethod_Start,
    CSComponentMethod_DelayedStart,
    CSComponentMethod_Update,
    CSComponentMethod_PostUpdate,
    CSComponentMethod_FixedUpdate,
    CSComponentMethod_PostFixedUpdate
};

/// Helper base class for user-defined game logic components that hooks up to update events and forwards them to virtual functions similar to ScriptInstance class.
class ATOMIC_API CSComponent : public ScriptComponent
{
    friend class CSComponentFactory;

    enum EventFlags
    {
        USE_UPDATE = 0x1,
        USE_POSTUPDATE = 0x2,
        USE_FIXEDUPDATE = 0x4,
        USE_FIXEDPOSTUPDATE = 0x8
    };

    OBJECT(CSComponent);
    BASEOBJECT(ScriptComponent);

public:

    /// Construct.
    CSComponent(Context* context);
    /// Destruct.
    virtual ~CSComponent();

    /// Register object factory.
    static void RegisterObject(Context* context);

    bool Load(Deserializer& source, bool setInstanceDefault);
    bool LoadXML(const XMLElement& source, bool setInstanceDefault);
    void ApplyAttributes();

    /// Handle enabled/disabled state change. Changes update event subscription.
    virtual void OnSetEnabled();

    void ApplyFieldValues();
    VariantMap& GetFieldValues() { return fieldValues_; }

    void SetComponentClassName(const String& name);
    const String& GetComponentClassName() const { return componentClassName_; }

    virtual ScriptComponentFile* GetComponentFile() { return assemblyFile_; }

    NETAssemblyFile* GetAssemblyFile() { return assemblyFile_; }
    void SetAssemblyFile(NETAssemblyFile* assemblyFile);

    ResourceRef GetAssemblyFileAttr() const;
    void SetAssemblyFileAttr(const ResourceRef& value);

protected:
    /// Handle scene node being assigned at creation.
    virtual void OnNodeSet(Node* node);
    /// Handle scene being assigned.
    virtual void OnSceneSet(Scene* scene);

private:

    String componentClassName_;
    SharedPtr<NETAssemblyFile> assemblyFile_;

};

}
