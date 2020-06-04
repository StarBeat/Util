#pragma once
#include "IPlugin.h"
namespace Util 
{
class AesPlugin : public IPlugin
{
public:
    AesPlugin();

    virtual void install() override;

    virtual void initialize() override;

    virtual void termination() override;

    virtual void uninstall() override;
};
}

