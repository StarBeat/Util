#pragma once
class IPlugin
{
public:
	virtual void install() = 0;

    virtual void initialize() = 0;

    virtual void termination() = 0;

    virtual void uninstall() = 0;
};

