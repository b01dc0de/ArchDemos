#ifndef ARCHDEMOS_GAMEENTITY_H
#define ARCHDEMOS_GAMEENTITY_H

namespace Arch
{
    // NOTE(CKA): 
    //  - This is the 'GameObject' struct designed to be inherited by structs in Game/* files
    //  - Going to first implement a typical inherited structure that holds all functionality for now
    //  - Would ideally move to a component-based system in the future, TBD
    struct GameEntity
    {
        bool bActive = 0;

        virtual void Tick() {}

        void Activate();
        void Deactivate();
        void Register();
        void Deregister();
    };
}

#endif // ARCHDEMOS_GAMEENTITY_H