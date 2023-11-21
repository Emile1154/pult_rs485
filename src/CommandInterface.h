#pragma once
#include <Carotage.h>
#include <stdint.h>

class CommandInterface{
    public:
        virtual void execute()  = 0;
        virtual ~CommandInterface() noexcept = default; //destructor
};

class UpCommand : public CommandInterface
{
private:
     Carotage carotage;
    /* data */
public:
    UpCommand(const Carotage& carotage);
    ~UpCommand();
    void execute()  override;
};

class DownCommand : public CommandInterface
{
private:
     Carotage carotage;
    /* data */
public:
    DownCommand(const Carotage& carotage);
    ~DownCommand();
    void execute()  override;
};

class LeftCommand : public CommandInterface
{
private:
     Carotage carotage;
    /* data */
public:
    LeftCommand(const Carotage& carotage);
    ~LeftCommand();
    void execute()  override;
};

class RightCommand : public CommandInterface
{
private:
     Carotage carotage;
    /* data */
public:
    RightCommand(const Carotage& carotage);
    ~RightCommand();
    void execute()  override;
};

class StopCommand : public CommandInterface
{
private:
     Carotage carotage;
    /* data */
public:
    StopCommand(const Carotage& carotage);
    ~StopCommand();
    void execute()  override;
};

class LayerStopCommand : public CommandInterface
{
private:
     Carotage carotage;
    /* data */
public:
    LayerStopCommand(const Carotage& carotage);
    ~LayerStopCommand();
    void execute()  override;
};

class ReelSetSpeedCommand : public CommandInterface
{
private:
     Carotage carotage;
    /* data */
public:
    ReelSetSpeedCommand(const Carotage& carotage);
    ~ReelSetSpeedCommand();
    void execute()  override;
};

class LayerSetSpeedCommand : public CommandInterface
{
private:
     Carotage carotage;
    /* data */
public:
    LayerSetSpeedCommand(const Carotage& carotage);
    ~LayerSetSpeedCommand();
    void execute()  override;
};
