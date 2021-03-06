//
// Created by david on 03/10/16.
//

#include <thread>
#include <algorithm>
#include <iostream>
#include "../include/RegionSyncEntity.h"
#include "../include/Simulation.h"
#include "../include/FullSyncEntity.h"
#include "../include/World.h"




World::World(int scenario):
mMap(512,128,scenario),
mTarget(0,0)
{

}

void World::update() {

        update(0);
}

bool World::update(int zone){
    bool keepRuning = false;
    for(int i = 0; i < mActiveHumans.size(); i++){

        if(!mActiveHumans[i]->isDestroyed()) {
            keepRuning = true;
            mActiveHumans[i]->update();
        }

    }

    return keepRuning;
}

bool World::spawn(Entity::Type type, int x, int y, Sync sync) {


    Entity* e = nullptr;
    for(int i = 0; i < Entity::Table[type].width ;i++)
        for(int j = 0; j < Entity::Table[type].height; j++)
            if(mMap.isSolid(x+i,y+j) && Entity::Table[type].solid)
                return false;

    if(sync==FullSync)
        e = new FullSyncEntity(&mMap,type,x,y);

    else if(sync == RegionSync)
        e = new RegionSyncEntity(&mMap,type,x,y);

    else
        e = new Entity(&mMap,type,x,y);

    if(type == Entity::Human) {
        mActiveHumans.push_back(e);
        e->setTarget(mTarget);
    }

    return true;
}

void World::spawn(Entity::Type type, int n, Sync sync) {
    std::cout << "Spawning " << n << " entities..." << std::endl;
    int i = 0;

    srand (time(NULL));
    while(i<n){

        int x = rand()%(mMap.getWidth()-2*Entity::Table[type].width) + Entity::Table[type].width;
        int y = rand()%(mMap.getHeight()-2*Entity::Table[type].height) + Entity::Table[type].height;


        if(spawn(type,x,y,sync))
            i++;
    }
}


void World::removeDestroyedEntities() {
    auto listBegin = std::remove_if(mActiveHumans.begin(), mActiveHumans.end(),
                                           std::mem_fn(&Entity::isDestroyed));

    mActiveHumans.erase(listBegin,mActiveHumans.end());
}

void World::reset() {
    for(int i = 0; i < mActiveHumans.size(); i++)
        mActiveHumans[i]->respawn();
    Entity::cnt = 0;
}

Map* World::getMapAddress() {
    return &mMap;
}
Map World::getMap() const {
    return mMap;
}

std::vector<Entity*> World::getActiveHumans() const {
    return mActiveHumans;
}

void World::setTarget(Vector2i v){
    mTarget = v;
}

