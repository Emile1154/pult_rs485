#include <CommandInterface.h>

void UpCommand::execute() {
    carotage.up();
}

void DownCommand::execute() {
    carotage.down();
}

void StopCommand::execute() {
    carotage.stop();
}

void LeftCommand::execute() {
    carotage.left();
}

void RightCommand::execute() {
    carotage.right();
}

void LayerStopCommand::execute() {
    carotage.layer_stop();
}

void LayerSetSpeedCommand::execute() {
    carotage.layer_set_speed();
}

void ReelSetSpeedCommand::execute() {
    carotage.reel_set_speed();
}

//constuctors and destructors
UpCommand::UpCommand(const Carotage& carotage) : carotage(carotage)
{
    // Constructor body (if any)
}

UpCommand::~UpCommand()
{
}

DownCommand::DownCommand(const Carotage& carotage) : carotage(carotage)
{
    // Constructor body (if any)
}

DownCommand::~DownCommand()
{
}

LeftCommand::LeftCommand(const Carotage& carotage) : carotage(carotage)
{
    // Constructor body (if any)
}

LeftCommand::~LeftCommand()
{
}

RightCommand::RightCommand(const Carotage& carotage) : carotage(carotage)
{
    // Constructor body (if any)
}

RightCommand::~RightCommand()
{
}

StopCommand::StopCommand(const Carotage& carotage) : carotage(carotage)
{
    // Constructor body (if any)
}

StopCommand::~StopCommand()
{
}

LayerStopCommand::LayerStopCommand(const Carotage& carotage) : carotage(carotage)
{
    // Constructor body (if any)
}

LayerStopCommand::~LayerStopCommand()
{
}

ReelSetSpeedCommand::ReelSetSpeedCommand(const Carotage& carotage) : carotage(carotage)
{
    // Constructor body (if any)
}

ReelSetSpeedCommand::~ReelSetSpeedCommand()
{
    
}

LayerSetSpeedCommand::LayerSetSpeedCommand(const Carotage& carotage) : carotage(carotage)
{
    // Constructor body (if any)
}

LayerSetSpeedCommand::~LayerSetSpeedCommand()
{
}