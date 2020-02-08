#include <Climber.h>
Climber::Climber()
//does not work, this is all code pulled from the internet, and will not work unless we download the whole file. 
/*{
    stop = false;

}

void climber::Climber::onInit()
{
    StartInternalCycle();
}

//void climber::Climber::StartInternalCycle()
{
    mutStartStop.lock();

    stop = false;

    internalThread = new boost::thread(Climber::InternalThreadFunction, this);

    mutStartStop.unlock();

}

void climber::Climber::StopInternalCycle()
{
    mutStartStop.lock();

    stop = true;
    internalThread->join();
    delete internalThread;
    internalThread = NULL;

    mutStartStop.unlock();

}

void climber::Climber::InternalThreadFunction(Climber* climber)
{
    climber->InternalCycleProcedure();
}

void climber::Climber::InternalCycleProcedure()
{
    while (!stop)
    {
    }
}


climber::Climber::~DianeClimber(){
    StopInternalCycle();
}

climber::ClimberMsg::DianeClimberMsg()
{

}

climber::ClimberMsg::~DianeClimberMsg(){

}

Climber::Climber(){}*/