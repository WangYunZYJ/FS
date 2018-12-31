#include "volume/volume.h"

std::shared_ptr<wyfs::volume> wyfs::volume::instance = nullptr;

int wyfs::volume::ID = 0;

wyfs::volume::volume()
{

}

wyfs::volume::~volume()
{
    while (!blocks.empty())
        blocks.pop();
}

std::shared_ptr<wyfs::volume> wyfs::volume::get_instance()
{
    if(instance.get() == nullptr)
    {
        instance = std::shared_ptr<wyfs::volume>(new wyfs::volume());//std::make_shared<wyfs::volume>();
    }
    return instance;
}