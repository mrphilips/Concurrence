//
// Created by david on 05/10/16.
//

#include <unistd.h>
#include <iostream>
#include <cmath>
#include "../include/MTSim1.h"

using namespace std;

Args::Args(World* w, Rectangle r) :
w(w),
r(r)
{

}

static void* updateZone(void* p){
    auto args = static_cast<Zone*>(p);

    args->update();

    return NULL;
}


MTSim1::MTSim1(int n, Scenario scenario) :
Simulation(scenario)
{

    cout << "Initializing T1 simulation" << endl;
    int humans = pow(2,n);

    World::Sync s;

    if(scenario==E1)
        s = World::NoSync;

    else s = World::RegionSync;

    mWorld.spawn(Entity::Human,humans,s);

    int width = mWorld.getMap().getWidth();
    int height = mWorld.getMap().getHeight();

    mZones.push_back(Zone(0,width/4,width/4,height,humans));
    mZones.push_back(Zone(width/4 +1, width/2,width/4,height,humans));
    mZones.push_back(Zone(width/2 + 1, 3*width/4,width/4,height,humans));
    mZones.push_back(Zone(3*width/4 +1,width,width/4,height,humans));

    initZones();
}

/**
 * add every entity to a given zone, depending
 * on its coordinates
 */
void MTSim1::initZones() {
    int w = mWorld.getMap().getWidth()/4;
    int h = mWorld.getMap().getHeight()/4;

   for(int i = 0; i < mZones.size(); i++){

       if(i-1 >=0)
           mZones[i].setPrev(&(mZones[i-1]));
       if(i+1 < mZones.size())
           mZones[i].setNext(&(mZones[i+1]));

       int l = i*w;
       int r = l + w;

      // cout << l << " : " << r << endl;

       for(int j = 0; j < mWorld.getActiveHumans().size(); j++){
           Entity* human = mWorld.getActiveHumans()[j];

           if(human->getPosition().x >= l && human->getPosition().x < r)
               mZones[i].add(human);
       }


   }
}

void MTSim1::run() {

    //create threads
   for(int i = 0; i < mZones.size(); i++) {
        pthread_t id;
        pthread_create(&id, NULL, updateZone, &(mZones[i]));
        mThreads.push_back(id);
    }

    //Wait for the threads to finish
    for(int i = 0; i < mZones.size(); i++)
        mZones[i].acquire();


    mThreads.clear();



}

void MTSim1::reset() {
    Simulation::reset();
    initZones();
}