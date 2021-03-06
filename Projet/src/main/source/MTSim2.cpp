//
// Created by thomas on 05/10/16.
//

#include <iostream>
#include <unistd.h>
#include <cmath>
#include "../include/MTSim2.h"

using namespace std;

/**
 * Moves a single entity to its target
 * @param p Entity
 * @return
 */
static void* updateEntity(void* p)
{
    auto entity = static_cast<Entity*>(p);

    while(!entity->isDestroyed()){

        entity->update();
        //usleep(100000);
        //cout << *(entity->getMap()) << endl;

    }

    entity->release();

    return NULL;
}

MTSim2::MTSim2(int n, Scenario scenario) : Simulation(scenario)
{
    cout << "Loading scenario T2..." << endl;

    World::Sync  sync;

    if(scenario == E1)
        sync = World::NoSync;
    else
        sync = World::FullSync;

    mWorld.spawn(Entity::Human,pow(2,n),sync);

}

/**
 * Create a thread for each entity and associates it
 * with static function updateEntity.
 * Wait for every thread to finish
 */
void MTSim2::run() {

    for(int i = 0; i < mWorld.getActiveHumans().size(); i++){

            //updateEntity(mWorld.getActiveHumans()[i]);
            pthread_t id;
            pthread_create(&id,NULL,updateEntity,mWorld.getActiveHumans()[i]);

            mThreads.push_back(id);

        }

    for(int i = 0; i < mWorld.getActiveHumans().size(); i++) {

        mWorld.getActiveHumans()[i]->acquire();
    }

    mThreads.clear();


}
